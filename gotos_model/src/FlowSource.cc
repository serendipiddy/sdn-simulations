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

namespace queueing {

void FlowSource::initialize()
{
    SourceBase::initialize();
    startTime = par("startTime");
    stopTime = par("stopTime");
    numJobs = par("numJobs");

    avgFlowInterarrival = 5;  // par("flowInterarrival");
    avgFlowPacketCount = 10;  // par("flowCount");
    avgFlowDuration = 5;  // par("flowDuration");

    flowCounter = 0;

    // schedule the first message timer for start time
    cMessage* firstFlow = getNewFlow();  // new cMessage("newJobTimer"));
}

/*
 * Creates a pointer to a new flow message, responsible for controlling the
 * lifetime and packet generation for that flow
 */
cMessage* FlowSource::getNewFlow() {
    cMessage* newFlow = new cMessage("flowTimer");

    cMsgPar* packets = new cMsgPar(flowPacketsRemainingPar);
    cMsgPar* deadline = new cMsgPar(flowDeadlinePar);
    cMsgPar* isNewFlow = new cMsgPar(newFlowPar);
    cMsgPar* flowID = new cMsgPar(flowIDPar);

    packets->setLongValue(avgFlowPacketCount);
    deadline->setDoubleValue(simTime().dbl() + avgFlowDuration);
    isNewFlow->setBoolValue(true);
    flowID->setLongValue(getNextFlowID());

    newFlow->addPar(packets);
    newFlow->addPar(deadline);
    newFlow->addPar(isNewFlow);


    return newFlow;
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
            scheduleAt(simTime().dbl() + avgFlowInterarrival, getNewFlow());  // schedule flow message  // TODO par("flowInterarrival")
            isNewFlow->setBoolValue(false);
        }

        // check count and timer are > 0, else delete flow message (ending the flow life)
        if (packetsLeft->longValue() <= 0 || deadline->doubleValue() <= simTime().dbl() ) {
            // emit that the flow is now dead?
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

Define_Module(FlowSource);

}; //namespace
