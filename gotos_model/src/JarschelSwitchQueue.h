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
    simsignal_t arrivalSignal;//deepak
    simsignal_t jobSignal; //deepak
    
    double probability_visit_controller;
  protected:
    virtual void initialize() override;
    virtual void arrival(Job *job) override;
    // virtual void handleMessage(omnetpp::cMessage *msg) override;
    virtual void endService(Job *job) override;

    virtual bool visitController(Job *job);
    virtual double uniformRand(double min, double max);
};

#endif /* JARSCHELSWITCHQUEUE_H_ */
