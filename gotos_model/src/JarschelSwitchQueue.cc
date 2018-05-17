/*
 * JarscheSwitch.cc
 *
 *  Created on: 27/04/2018
 *      Author: iddy
 */

#include "JarschelSwitchQueue.h"
#include "Job.h"

Define_Module(JarschelSwitchQueue);
// Register_Class(JarschelSwitchQueue);

void JarschelSwitchQueue::initialize()
{
    Queue::initialize();
    probability_visit_controller = registerSignal("prob_ctrl");
    
    arrivalSignal = registerSignal("arrival");//deepak
    jobSignal = registerSignal("jobarrival");//deepak
    controlsignal = registerSignal("visitcontrol");//deepak

    // toControllerSignal = registerSignal("toControllerSignal");
    dataplaneSignal = registerSignal("dataplaneSignal");
}

/*
 * Returns a uniform random number between min and max
 * http://stackoverflow.com/a/2704552
 */
double JarschelSwitchQueue::uniformRand(double min, double max)
{
    double f = (double) rand() / RAND_MAX;
    return min + f*(max - min);
}

/*
 * Returns the decision variable for this job visiting the controller.
 * True means this job will visit the controller when it exits the queue
 */
bool JarschelSwitchQueue::checkVisitController(Job *job)
{
    double rv = par("probVisitController").doubleValue();
    bool willVisit = rv > uniformRand(0,1);
    if (willVisit)
    {
        EV << "Packet will visit controller " << rv << endl;
        emit(dataplaneSignal,1);//deepak
    }
    else
    {
        EV << "Packet won't visit controller " << rv << endl;
        emit(dataplaneSignal,1);//deepak
    }

    return willVisit;
}

void JarschelSwitchQueue::arrival(Job *job)
{
    Queue::arrival(job);
    emit(arrivalSignal, 1);//deepak
}

simtime_t JarschelSwitchQueue::startService(Job *job)
{
    emit(jobSignal,1);//deepak
    return Queue::startService(job);
}

void JarschelSwitchQueue::endService(Job *job)
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

