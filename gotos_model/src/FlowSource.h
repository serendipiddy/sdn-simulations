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

#ifndef FLOWSOURCE_H_
#define FLOWSOURCE_H_

#include "Source.h"
#include "FlowJob.h"

namespace queueing {

class FlowSource: public queueing::SourceBase {
private:
    long flowCounter;
    simtime_t startTime;
    simtime_t stopTime;

    long avgFlowPacketCount;
    double avgFlowDuration;
    double avgFlowInterarrival;

    // Names of cMessage parameters
    const char* flowPacketsRemainingPar = "flowPacketsRemaining";
    const char* flowDeadlinePar = "flowDeadlineLeft";
    const char* newFlowPar = "newFlow";
    const char* flowIDPar = "flowID";

    int numJobs;
protected:
    virtual void initialize() override;
    virtual Job *createJob() override;
    virtual void handleMessage(cMessage *msg) override;
private:
    long getNextFlowID();
    cMessage* getNewFlow();
};

}; //namespace

#endif /* FLOWSOURCE_H_ */
