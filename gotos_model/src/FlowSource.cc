//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "FlowSource.h"

Define_Module(FlowSource);

void FlowSource::initialize()
{
    SourceBase::initialize();
    startTime = par("startTime");
    stopTime = par("stopTime");
    numJobs = par("numJobs");

    flowBeginSignal = registerSignal("flowbegin");
    flowEndSignal = registerSignal("flowend");

//    avgFlowInterarrival = 5;  // par("flowInterarrival");
//    avgFlowPacketCount = par("flowCount").longValue();
//    avgFlowDuration = par("flowDuration").doubleValue();

    flowCounter = 0;

    // schedule the first message timer for start time
    createNewFlow(startTime.dbl());
}

/*
 * Creates a pointer to a new flow message, responsible for controlling the
 * lifetime and packet generation for that flow
 */
void FlowSource::createNewFlow(double startTime) {
    cMessage* newFlow = new cMessage("flowTimer");

    cMsgPar* packetsLeft = new cMsgPar(flowPacketsRemainingPar);
    cMsgPar* deadline = new cMsgPar(flowDeadlinePar);
    cMsgPar* isNewFlow = new cMsgPar(newFlowPar);
    cMsgPar* msgFlowID = new cMsgPar(flowIDPar);

    packetsLeft->setLongValue( par("flowPacketCount") );
    deadline->setDoubleValue( startTime + par("flowDuration").doubleValue() );
    isNewFlow->setBoolValue(true);
    msgFlowID->setLongValue(getNextFlowID());

    newFlow->addPar(packetsLeft);
    newFlow->addPar(deadline);
    newFlow->addPar(isNewFlow);
    newFlow->addPar(msgFlowID);

//    std::cout << "NEW flow " << msgFlowID->longValue() << ": pkts " << packetsLeft->longValue() << ", time left " << deadline->doubleValue()-startTime << endl;
    emit(flowBeginSignal, msgFlowID->longValue());
    scheduleAt(startTime, newFlow);
}

void FlowSource::handleMessage(cMessage *msg)
{
    ASSERT(msg->isSelfMessage());

    ASSERT(msg->hasPar(flowPacktsRemainingPar));
    ASSERT(msg->hasPar(flowTimeLeftPar));
    ASSERT(msg->hasPar(newFlowPar));
    ASSERT(msg->hasPar(flowIDPar));


    if ((numJobs < 0 || numJobs > jobCounter) && (stopTime < 0 || stopTime > simTime())) {
        // get msg params
        cMsgPar* packetsLeft = (cMsgPar*) msg->getParList()[msg->findPar( flowPacketsRemainingPar )];
        cMsgPar* deadline    = (cMsgPar*) msg->getParList()[msg->findPar( flowDeadlinePar )];
        cMsgPar* isNewFlow   = (cMsgPar*) msg->getParList()[msg->findPar( newFlowPar )];
        cMsgPar* msgFlowID   = (cMsgPar*) msg->getParList()[msg->findPar( flowIDPar )];

        // if it's a new flow, schedule next flow message to start circulating
        if (isNewFlow->boolValue()) {
            createNewFlow(simTime().dbl() + par("flowInterarrival").doubleValue());
            isNewFlow->setBoolValue(false);
        }

//        std::cout << "flow " << msgFlowID->longValue() << ": pkts " << packetsLeft->longValue() << ", time left " << deadline->doubleValue()-simTime().dbl() << endl;
        // check count and timer are > 0, else delete flow message (ending the flow life)
        if (packetsLeft->longValue() <= 0 || deadline->doubleValue() <= simTime().dbl() ) {
//            std::cout << "KILLING FLOW " << msgFlowID->longValue() << endl;
            emit(flowEndSignal, msgFlowID->longValue());
            delete msg;
            return;
        }


        // decrement flow packet count and flow duration timer
        packetsLeft->setLongValue(packetsLeft->longValue() - 1);

        // schedule next packet in the flow by rescheduling the timer for the next message
        scheduleAt(simTime() + par("interArrivalTime").doubleValue(), msg);  // schedule flow message

        // send the current message
        FlowJob *job = (FlowJob*) createJob();
        job->setFlowID(msgFlowID->longValue());


        send(job, "out");
    }
    else {
        // finished simulation
        delete msg;
    }
}

long FlowSource::getNextFlowID() {
    return flowCounter++;
}

/*
 * Produces a FlowJob with a jobID == flowID. Set it outside this function for real flow ID
 */
queueing::Job *FlowSource::createJob() {
    char buf[80];
    sprintf(buf, "%.60s-%d", jobName.c_str(), ++jobCounter);
    FlowJob *job = new FlowJob(buf);  // this is the important part
    job->setKind(par("jobType"));
    job->setPriority(par("jobPriority"));
    job->setFlowID(jobCounter);
    return job;
}
