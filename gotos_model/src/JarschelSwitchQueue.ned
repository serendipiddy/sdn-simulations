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
        
        @signal[visitcontrol](type="long"); //deepak
        @signal[jobarrival](type="long"); //deepak
        @signal[arrival](type="long"); //deepak
        
        @statistic[contcount](title="controller count"; source=count(visitcontrol); record=last,vector?); //deepak
        @statistic[dropped](title="drop event";source=count(dropped); record=last,vector?);//deepak
        @statistic[jobcount](title="job count"; source=count(jobarrival); record=last,vector?); //deepak
        @statistic[packetcount](title="packet count";source=count(arrival); record=last,vector?); //deepak
        @statistic[dropRate](title="drop rate";source=count(dropped)/count(arrival); record=last,vector?);   //deepak
        
//        @statistic[contcount](title="controller count"; source="visitcontrol"; record=vector,count; interpolationmode=none); //deepak  
//        @statistic[dropCount](source=count(droppedSignal); record=last,vector?); //deepak        
//        @statistic[dropped](title="drop event";record=vector?,count;interpolationmode=none);
//        @statistic[jobcount](title="job count"; source="jobarrival"; record=vector,count; interpolationmode=none); //deepak  
//        @statistic[packetcount](title="packet count"; source="arrival"; record=vector,count; interpolationmode=none); //deepak
//        @statistic[packetcount](title="packet count"; source="arrival"; record=vector,stats; interpolationmode=none); //deepak
//        @statistic[dropCount](title="number of dropped packets";source=count(dropped); record=last,vector?); //deepak

    gates:
        output out_controller;
    
}



