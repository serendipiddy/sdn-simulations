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
    std::cout << "HELLO" << endl;
    probability_visit_controller = registerSignal("prob_ctrl");
    
    arrivalSignal = registerSignal("arrival");//deepak
    jobSignal = registerSignal("jobarrival");//deepak
    controlsignal = registerSignal("visitcontrol");//deepak
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
bool JarschelSwitchQueue::visitController(Job *job)
{
    double rv = par("probVisitController").doubleValue();
    bool visited = rv > uniformRand(0,1);
    if (visited)
    {
        EV << "Packet will visit controller " << rv << endl;
    }
    else
    {
        EV << "Packet won't visit controller " << rv << endl;
    }

    return visited;
}

void JarschelSwitchQueue::arrival(Job *job)
{
    Queue::arrival(job);
    emit(arrivalSignal, 1);//deepak
}

simtime_t JarschelSwitchQueue::startService(Job *job)
{
    emit(jobSignal,1);//deepak
    return Queue::startService(Job *job);
}

void JarschelSwitchQueue::endService(Job *job)
{
    // Queue::endService(job);
    EV << "Finishing service of " << job->getName() << endl;
    simtime_t d = simTime() - job->getTimestamp();
    job->setTotalServiceTime(job->getTotalServiceTime() + d);

    bool visitedController = visitController(job);
    if (visitedController)
    {
        send(job, "out_controller");
    }
    else
    {
        send(job, "out");
    }
}

