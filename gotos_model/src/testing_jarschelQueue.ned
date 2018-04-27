
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

package gotos_model;

import org.omnetpp.queueing.Queue;
import org.omnetpp.queueing.Sink;
import org.omnetpp.queueing.Source;

//
// TODO auto-generated type
//
network TestingJarschelQueue
{
    submodules:
        queue1: Queue {
            @display("p=193,28");
        }
        source: Source {
            @display("p=43,125");
        }
        sink: Sink {
            @display("p=335,125");
        }
        jarshelSwitchQueue: JarshelSwitchQueue {
            @display("p=193,125");
        }
    connections:
        source.out --> jarshelSwitchQueue.in++;
        jarshelSwitchQueue.out --> sink.in++;
        jarshelSwitchQueue.out_controller --> queue1.in++;
        queue1.out --> jarshelSwitchQueue.in++;
}
