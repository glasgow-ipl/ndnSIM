/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2015  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

// ndn-simple.cpp

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ndnSIM-module.h"
#include <chrono>
#include <ctime>



#include "ns3/internet-stack-helper.h"
#include "ns3/object.h"
#include "ns3/traffic-control-module.h"
#include "ns3/flow-monitor-module.h"

namespace ns3 {

/**
 * This scenario simulates a very simple network topology:
 *
 *
 *      +----------+     1Mbps      +--------+     1Mbps      +----------+
 *      | consumer | <------------> | router | <------------> | producer |
 *      +----------+         10ms   +--------+          10ms  +----------+
 *
 *
 * Consumer requests data from producer with frequency 10 interests per second
 * (interests contain constantly increasing sequence number).
 *
 * For every received interest, producer replies with a data packet, containing
 * 1024 bytes of virtual payload.
 *
 * To run scenario and see what is happening, use the following command:
 *
 *     NS_LOG=ndn.Consumer:ndn.Producer ./waf --run=ndn-simple
 */

  class PcapWriter {
  public:
    PcapWriter(const std::string& file)
    {
      PcapHelper helper;
      m_pcap = helper.CreateFile(file, std::ios::out, PcapHelper::DLT_PPP);
    }
    void
    TracePacket(Ptr<const Packet> packet)
    {
      static PppHeader pppHeader;
      pppHeader.SetProtocol(0x0077);
      m_pcap->Write(Simulator::Now(), pppHeader, packet);
    }
  private:
    Ptr<PcapFileWrapper> m_pcap;
  };


void
	TcPacketsInQueueTrace (uint32_t oldValue, uint32_t newValue)
	{
		  std::cout << Simulator::Now() << " TcPacketsInQueue " << oldValue << " to " << newValue << std::endl;
	}

void
	DevicePacketsInQueueTrace (uint32_t oldValue, uint32_t newValue)
	{
		  std::cout << Simulator::Now() << " DevicePacketsInQueue " << oldValue << " to " << newValue << std::endl;
	}

void 
	CreateAndAggregateObjectFromTypeId (Ptr<Node> node, const std::string typeId)
	{
	  ObjectFactory factory;
	    factory.SetTypeId (typeId);
	      Ptr<Object> protocol = factory.Create <Object> ();
	        node->AggregateObject (protocol);
	}

void
        SojournTimeTrace (Time sojournTime)
        {
                  std::cout << "Sojourn time " << sojournTime.ToDouble (Time::MS) << "ms" << std::endl;
        }

int
main(int argc, char* argv[])
{
  //auto start = std::chrono::system_clock::now();
  //auto start_clock = std::chrono::system_clock::to_time_t(start);
  //auto start_clock = std::localtime(&start);
  //string start_string = put_time(start_clock,"%Y-%m-%d-%H%M");

  time_t now;
  time(&now);
  char buf[sizeof "2011-10-08T07:07:09Z"];
  strftime(buf, sizeof buf, "%Y-%m-%d-%H%M", gmtime(&now));
  string start_string = buf;
  uint32_t queueSize=10;
  // setting default parameters for PointToPoint links and channels
  Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("3.0Mbps"));
  Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
  Config::SetDefault("ns3::DropTailQueue<Packet>::MaxSize", StringValue("50p"));
  // Devices queue configuration
  //Config::SetDefault ("ns3::DropTailQueue<Packet>::MaxSize",
                       //QueueSizeValue (QueueSize (QueueSizeUnit::PACKETS, queueSize)));

  // Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
  CommandLine cmd;
  cmd.Parse(argc, argv);

  // Creating nodes
  NodeContainer nodes;
  nodes.Create(4);
  std::cout << "Nodes created" << std::endl;

  NetDeviceContainer devices;
  // Connecting nodes using two links
  PointToPointHelper p2p;
  devices = p2p.Install(nodes.Get(0), nodes.Get(1));// new NetDeviceCOntainer for nodes with p2p connection
  devices.Add(p2p.Install(nodes.Get(1), nodes.Get(2))); // another NetDevice Container appended at the back of the netDeviceContainer
  devices.Add(p2p.Install(nodes.Get(3), nodes.Get(1)));
  p2p.SetQueue("ns3::DropTailQueue","MaxSize", StringValue("50p")); // Queue to pass packets through type followed by attributes

// Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  ndnHelper.SetDefaultRoutes(true);
  ndnHelper.InstallAll();


  // traffic-control-layer
 // TrafficControlLayer tcl0 = TrafficControlLayer();
 // TrafficControlLayer tcl1 = TrafficControlLayer();
 // TrafficControlLayer tcl2 = TrafficControlLayer();
 // tcl0.SetNode(nodes.Get(0));
 // tcl1.SetNode(nodes.Get(1));
 // tcl2.SetNode(nodes.Get(2));
 // std::cout << "TCL: nodes set" <<std::endl; 
 // CreateAndAggregateObjectFromTypeId (nodes.Get(0), "ns3::TrafficControlLayer");
 // CreateAndAggregateObjectFromTypeId (nodes.Get(1), "ns3::TrafficControlLayer");
 // CreateAndAggregateObjectFromTypeId (nodes.Get(2), "ns3::TrafficControlLayer");



// set queue
  //setup traffic control 
  std::cout << "About to setup PrioQueueDisc" << std::endl;
  TrafficControlHelper tch;
  //uint16_t handle = tch.SetRootQueueDisc("ns3::PrioQueueDisc", "Priomap", StringValue("0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1"));
  uint16_t handle = tch.SetRootQueueDisc("ns3::PrioQueueDisc", "Priomap", StringValue("1 1 1 1 0 1 1 1 1 1 1 1 1 1 1 1"));


  TrafficControlHelper::ClassIdList cid = tch.AddQueueDiscClasses(handle, 2, "ns3::QueueDiscClass");
  uint16_t qdhandle0 = tch.AddChildQueueDisc(handle,cid[0], "ns3::FifoQueueDisc");
  uint16_t qdhandle1 = tch.AddChildQueueDisc(handle,cid[1], "ns3::RedQueueDisc");
  tch.AddPacketFilter(handle,"ns3::ndn::NdnPacketFilter");
  
  tch.AddInternalQueues(qdhandle0, 1, "ns3::DropTailQueue","MaxSize",StringValue("100p"));
  tch.AddInternalQueues(qdhandle1, 1, "ns3::DropTailQueue","MaxSize",StringValue("100p"));

  QueueDiscContainer qdiscs = tch.Install (devices);

  Ptr<QueueDisc> q = qdiscs.Get (1);
  q->TraceConnectWithoutContext ("PacketsInQueue", MakeCallback (&TcPacketsInQueueTrace));
  Config::ConnectWithoutContext ("/NodeList/1/$ns3::TrafficControlLayer/RootQueueDiscList/0/SojournTime",
                                 MakeCallback (&SojournTimeTrace));

  Ptr<NetDevice> nd = devices.Get(1);
  Ptr<PointToPointNetDevice> ptpnd = DynamicCast<PointToPointNetDevice>(nd);
  Ptr<Queue<Packet> > queue = ptpnd->GetQueue();
  queue->TraceConnectWithoutContext("PacketsInQueue", MakeCallback(&DevicePacketsInQueueTrace));
  
  Ptr<TrafficControlLayer> tc = devices.Get(0)->GetNode ()->GetObject<TrafficControlLayer> ();
  tc->ScanDevices();
  Ptr<TrafficControlLayer> tc1 = devices.Get(1)->GetNode ()->GetObject<TrafficControlLayer> ();
  tc1->ScanDevices();
  Ptr<TrafficControlLayer> tc2 = devices.Get(2)->GetNode ()->GetObject<TrafficControlLayer> ();
  tc2->ScanDevices();
  Ptr<TrafficControlLayer> tc3 = devices.Get(3)->GetNode ()->GetObject<TrafficControlLayer> ();
  tc3->ScanDevices();
  Ptr<TrafficControlLayer> tc4 = devices.Get(4)->GetNode ()->GetObject<TrafficControlLayer> ();
  tc4->ScanDevices();
  


    // Choosing forwarding strategy
  ndn::StrategyChoiceHelper::InstallAll("/prefix", "/localhost/nfd/strategy/multicast");
  ndn::StrategyChoiceHelper::InstallAll("/prio","/localhost/nfd/strategy/multicast");
  // Installing applications

  // Consumer
  ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
  // Consumer will request /prefix/0, /prefix/1, ...
  consumerHelper.SetPrefix("/prefix");
  consumerHelper.SetAttribute("Frequency", StringValue("180")); //  interests a second
  
  auto apps = consumerHelper.Install(nodes.Get(0));                        // first node
  apps.Stop(Seconds(10.0)); // stop the consumer app at 10 seconds mark
  
  ndn::AppHelper prioConsumerHelper("ns3::ndn::ConsumerCbr");
  prioConsumerHelper.SetPrefix("/prio");
  prioConsumerHelper.SetAttribute("Frequency", StringValue("180")); 
  
  auto prioapps = prioConsumerHelper.Install(nodes.Get(3));
  prioapps.Stop(Seconds(10.0));

  // Producer
  ndn::AppHelper producerHelper("ns3::ndn::Producer");
  // Producer will reply to all requests starting with /prefix
  producerHelper.SetPrefix("/prefix");
  producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
  producerHelper.Install(nodes.Get(2)); // last node
 
// Producer
  ndn::AppHelper prioProducerHelper("ns3::ndn::Producer");
  // Producer will reply to all requests starting with /prefix
  producerHelper.SetPrefix("/prio");
  producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
  producerHelper.Install(nodes.Get(2)); // last node
  

  // Application-layer Latency tracer
  ndn::AppDelayTracer::InstallAll(start_string + "-app-delays-trace.txt");
  ndn::AppDelayTracer::Install(nodes.Get(0),start_string +"-app-delays-trace-n0.txt"); 
  ndn::AppDelayTracer::Install(nodes.Get(3),start_string + "-app-delays-trace-n3.txt");


  PcapWriter trace("ndn-simple-trace-link.pcap");
  Config::ConnectWithoutContext("/NodeList/1/DeviceList/*/$ns3::PointToPointNetDevice/MacTx",
		  MakeCallback(&PcapWriter::TracePacket, &trace));
  Config::ConnectWithoutContext("/NodeList/1/DeviceList/*/$ns3::PointToPointNetDevice/MacRx",
		  MakeCallback(&PcapWriter::TracePacket, &trace));

  Simulator::Stop(Seconds(20.0));
  std::cout << "Start the simulation" << std::endl;
  Simulator::Run();
  Simulator::Destroy();

  return 0;
}

} // namespace ns3

int
main(int argc, char* argv[])
{
  return ns3::main(argc, argv);
}