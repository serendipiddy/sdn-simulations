/*
 * JarschelSwitchQueue.h
 *
 *  Created on: 27/04/2018
 *      Author: iddy
 */

#ifndef JARSCHELSWITCHQUEUE_H_
#define JARSCHELSWITCHQUEUE_H_

#include <omnetpp.h>
#include "Queue.h"
#include "QueueingDefs.h"

using namespace queueing;
// using namespace omnetpp;

/**
 * JarschelSwitchQueue module that splits outgoing traffic between two output ports, given a random decision variable.
 */
class JarschelSwitchQueue : public queueing::Queue
{
  private:
    // simsignal_t toControllerSignal;
    simsignal_t dataplaneSignal;
    
    simsignal_t controlsignal;//deepak
    simsignal_t packetArrivalSignal;//deepak
    simsignal_t packetServiceSignal; //deepak
    
    double probability_visit_controller;
    long most_recent_id_to_controller;

  protected:
    virtual void initialize() override;
    virtual void arrival(Job *job) override;
    // virtual void handleMessage(omnetpp::cMessage *msg) override;
    virtual simtime_t startService(Job *job) override;
    virtual void endService(Job *job) override;

    virtual bool checkVisitController(Job *job);
    virtual double uniformRand(double min, double max);
};

#endif /* JARSCHELSWITCHQUEUE_H_ */
