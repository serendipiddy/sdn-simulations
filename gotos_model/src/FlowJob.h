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

#ifndef FLOWJOB_H_
#define FLOWJOB_H_

#include "Job.h"

namespace queueing {

class FlowJob: public Job {
public:
    FlowJob(const char *name=nullptr, int kind=0, JobList *table=nullptr) : Job(name, kind, table) { };
    FlowJob(const Job& job) : Job(job) { };

    long getFlowID();
    void setFlowID(long id);
private:
    long flowID;
};

}; //namespace

#endif /* FLOWJOB_H_ */
