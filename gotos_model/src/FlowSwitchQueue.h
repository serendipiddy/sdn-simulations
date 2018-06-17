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
#include "FlowJob.h"
#include "QueueingDefs.h"

using namespace queueing;

class FlowIDElem : public cObject {
    public:
        FlowIDElem(long id) : cObject(){};
        long id;
};

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
    
    long most_recent_id_to_controller;
    cArray missingIDs;

  protected:
    virtual void initialize() override;
    virtual void arrival(Job *job) override;
    // virtual void handleMessage(omnetpp::cMessage *msg) override;
    virtual simtime_t startService(Job *job) override;
    virtual void endService(Job *job) override;

    virtual bool checkVisitController(Job *job);
};

#endif /* FLOWSWITCHQUEUE_H_ */
