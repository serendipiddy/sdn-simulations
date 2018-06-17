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
import gotos_model.FlowSwitchQueue;

package gotos_model;

//
// Module for a simple SDN switch queue.
// The first packet of each flow that arrives at the switch is forwarded to controller once, the rest are forwarded to the out port.
// The two outputs are for the controller queue and leaving this switch on the data plane. 
//
simple FlowSwitchQueue extends Queue
{        
    parameters:
        @class(FlowSwitchQueue);
        volatile double probVisitController;
        
        @signal[visitcontrol](type="long");
        @signal[packetservice](type="long");
        @signal[packetarrival](type="long");
        
        @statistic[contcount](title="controller count"; source=count(visitcontrol); record=last,vector?);
        // @statistic[dropped](title="drop event";source=count(dropped); record=last,vector?);
        @statistic[servicecount](title="service count"; source=count(packetservice); record=last,vector?); // job count
        @statistic[packetcount](title="packet count";source=count(packetarrival); record=last,vector?);
        @statistic[dropRate](title="drop rate";source=count(dropped)/count(packetservice); record=last,vector?);
        
    gates:
        output out_controller;
    
}



