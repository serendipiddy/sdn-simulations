/*
 * FlowSwitchQueue.h
 *
 *  Created on: 17/06/2018
 *      Author: iddy
 */

#ifndef FLOWSWITCHQUEUE_H_
#define FLOWSWITCHQUEUE_H_

#include <omnetpp.h>
#include "Queue.h"
#include "QueueingDefs.h"

using namespace queueing;

/**
 * FlowSwitchQueue module that splits outgoing traffic between two output ports, given a flow ID in the traffic - sending the first packet from each flow to the controller exactly once.
 */
class FlowSwitchQueue : public queueing::Queue
{
  private:
    simsignal_t dataplaneSignal;
    simsignal_t controlsignal;
    simsignal_t packetArrivalSignal;
    simsignal_t packetServiceSignal;
    
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

#endif /* FLOWSWITCHQUEUE_H_ */
