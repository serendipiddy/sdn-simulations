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
import org.omnetpp.queueing.Queue;
import gotos_model.JarshelSwitchQueue;

package gotos_model;
//@namespace(gotos_model);

//
// Module for a simple SDN switch queue.
// Packets that arrive at the switch are forwarded to one of two outputs, controlled with a random variable.
// The two outputs are for the controller queue and leaving this switch on the data plane. 
//
simple JarshelSwitchQueue extends Queue
{        
    parameters:
        @class(JarschelSwitchQueue);
        volatile double probVisitController;
    gates:
        output out_controller;
    
}



