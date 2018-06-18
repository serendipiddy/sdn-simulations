/*
 * FlowSwitchQueue.cc
 *
 *  Created on: 17/06/2018
 *      Author: iddy
 */

#include "FlowSwitchQueue.h"
#include "Job.h"

Define_Module(FlowSwitchQueue);

void FlowSwitchQueue::initialize() {
    Queue::initialize();
    most_recent_id_to_controller = -1;
    
    packetArrivalSignal = registerSignal("packetarrival"); // arrival signal
    packetServiceSignal = registerSignal("packetservice"); // job signal
    controlsignal = registerSignal("visitcontrol");
    dataplaneSignal = registerSignal("dataplaneSignal");
    missingIDs = cArray();
}

/*
 * Returns the decision variable for this job visiting the controller.
 * True means this job will visit the controller when it exits the queue
 */
bool FlowSwitchQueue::checkVisitController(Job *jjob) {
    FlowJob *job = (FlowJob*) jjob;
    bool willVisit = false;
    FlowIDElem* thisIDElem = new FlowIDElem(job->getFlowID());

    if (thisIDElem->id > most_recent_id_to_controller) {

//        std::cout << "NEW flow " << thisIDElem->id << " (> most recent) " << endl;
        // record any flows that are missing
        for (long i = most_recent_id_to_controller+1; i < thisIDElem->id; i++) {
//            std::cout << "  Missing flows " << i << endl;
            FlowIDElem* id = new FlowIDElem(i);
            if (!missingIDs.exist(missingIDs.find(id))) {
                missingIDs.add(id);
            }
        }
        most_recent_id_to_controller = thisIDElem->id;
        willVisit = true;
    }
    else if (missingIDs.exist(missingIDs.find(thisIDElem))) {
//        std::cout << "NEW flow " << thisIDElem->id << " (in missing) " << endl;
        missingIDs.remove(thisIDElem);
        willVisit = true;
    }

    if (willVisit) {
        EV << "Packet will visit controller " << endl;
        emit(controlsignal,1);
    }
    else {
        EV << "Packet won't visit controller " << endl;
        emit(dataplaneSignal,1);
    }

    return willVisit;
}

void FlowSwitchQueue::arrival(Job *job) {
    Queue::arrival(job);
    emit(packetArrivalSignal, 1);
}

simtime_t FlowSwitchQueue::startService(Job *job) {
    emit(packetServiceSignal,1);
    return Queue::startService(job);
}

void FlowSwitchQueue::endService(Job *job) {
    // Queue::endService(job);
    EV << "Finishing service of " << job->getName() << endl;
    simtime_t d = simTime() - job->getTimestamp();
    job->setTotalServiceTime(job->getTotalServiceTime() + d);

    if (checkVisitController(job)) {
        send(job, "out_controller");
    }
    else {
        send(job, "out");
    }
}

