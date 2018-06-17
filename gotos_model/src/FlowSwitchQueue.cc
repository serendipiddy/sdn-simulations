/*
 * FlowSwitchQueue.cc
 *
 *  Created on: 17/06/2018
 *      Author: iddy
 */

#include "FlowSwitchQueue.h"
#include "Job.h"

Define_Module(FlowSwitchQueue);

void FlowSwitchQueue::initialize()
{
    Queue::initialize();
    probability_visit_controller = registerSignal("prob_ctrl");
    most_recent_id_to_controller = 0;
    
    packetArrivalSignal = registerSignal("packetarrival");//deepak  // arrival signal
    packetServiceSignal = registerSignal("packetservice");//deepak  // job signal
    controlsignal = registerSignal("visitcontrol");//deepak

    // toControllerSignal = registerSignal("toControllerSignal");
    dataplaneSignal = registerSignal("dataplaneSignal");
}

/*
 * Returns a uniform random number between min and max
 * http://stackoverflow.com/a/2704552
 */
double FlowSwitchQueue::uniformRand(double min, double max)
{
    double f = (double) rand() / RAND_MAX;
    return min + f*(max - min);
}

/*
 * Returns the decision variable for this job visiting the controller.
 * True means this job will visit the controller when it exits the queue
 */
bool FlowSwitchQueue::checkVisitController(Job *job)
{
    double rv = par("probVisitController").doubleValue();
    bool willVisit = rv > uniformRand(0,1);
    if (job->getId() <= most_recent_id_to_controller)
    {
        willVisit = false;
    }

    if (willVisit)
    {
        most_recent_id_to_controller = job->getId();  // Improvement on Jarschel's model

        EV << "Packet will visit controller " << rv << endl;
        emit(controlsignal,1);//deepak
    }
    else
    {
        EV << "Packet won't visit controller " << rv << endl;
        emit(dataplaneSignal,1);//deepak
    }

    return willVisit;
}

void FlowSwitchQueue::arrival(Job *job)
{
    Queue::arrival(job);
    emit(packetArrivalSignal, 1);//deepak
}

simtime_t FlowSwitchQueue::startService(Job *job)
{
    emit(packetServiceSignal,1);//deepak
    return Queue::startService(job);
}

void FlowSwitchQueue::endService(Job *job)
{
    // Queue::endService(job);
    EV << "Finishing service of " << job->getName() << endl;
    simtime_t d = simTime() - job->getTimestamp();
    job->setTotalServiceTime(job->getTotalServiceTime() + d);

    bool visitedController = checkVisitController(job);
    if (visitedController)
    {
        send(job, "out_controller");
    }
    else
    {
        send(job, "out");
    }
}

