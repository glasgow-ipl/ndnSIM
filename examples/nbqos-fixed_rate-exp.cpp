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
  // Prefix:
  time_t now;
  time(&now);
  char buf[sizeof "2011-10-08T07:07:09Z"]; // isodate
  strftime(buf, sizeof buf, "%Y-%m-%d-%H%M", gmtime(&now));
  string prefix = "";
  string date = buf;
  string exp_name = "ndn-prioqueue-small";
  uint32_t std_prefix_rate = 300; //default 300 pps
  uint32_t prio_prefix_rate = 300; //default 300 pps
  uint32_t queueSize=10;
  string out_dir = "./";
  uint64_t run_num = 1; //defaut run number

  CommandLine cmd (__FILE__);
  cmd.AddValue("stdPRate", "Standard packet rate (pps) per app instance", std_prefix_rate);
  cmd.AddValue("prioPRate", "Prioritised packet rate (pps) per app instance", prio_prefix_rate);
  cmd.AddValue("outDir", "Output directory",out_dir);
  cmd.AddValue("expName", "Experiment name override", exp_name);
  cmd.AddValue("runNum", "NS3 Run Number",run_num);
  cmd.AddValue("date","Date string override",date);
  cmd.Parse(argc,argv);
  ns3::RngSeedManager::SetRun(run_num); 
  prefix.append(date);
  prefix.append("-"+exp_name);
  prefix.append("-"+std::to_string(run_num));
  prefix.append("-"+std::to_string(std_prefix_rate)+"_std_pps");
  prefix.append("-"+std::to_string(prio_prefix_rate)+"_prio_pps");
  // setting default parameters for PointToPoint links and channels
  Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("2.0Mbps"));
  Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("20ms"));
  Config::SetDefault("ns3::DropTailQueue<Packet>::MaxSize", StringValue("20p"));
  // Devices queue configuration
  //Config::SetDefault ("ns3::DropTailQueue<Packet>::MaxSize",
                       //QueueSizeValue (QueueSize (QueueSizeUnit::PACKETS, queueSize)));
  std::cout << prefix << std::endl;
  // Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
 // CommandLine cmd;
 // cmd.Parse(argc, argv);

  // Creating nodes
  NodeContainer nodes;
  nodes.Create(6);
  std::cout << "Nodes created" << std::endl;


  NetDeviceContainer devices;
  // Connecting nodes using two links
  // Node 0: C1
  // Node 1: F1
  // Node 2: C2
  // Node 3: F3
  // Node 4: C3
  // Node 5: F2
  // Node 6: F4
  // Node 7: P1
  // Node 8: P2
  //
  PointToPointHelper p2p;

  PointToPointHelper consumerLink;
  consumerLink.SetDeviceAttribute ("DataRate", StringValue ("1Gbps"));
  consumerLink.SetChannelAttribute("Delay", StringValue("2ms"));
  consumerLink.SetQueue("ns3::DropTailQueue","MaxSize", StringValue ("100p"));

  
  PointToPointHelper bottleneckLink;
  bottleneckLink.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  bottleneckLink.SetChannelAttribute("Delay", StringValue("20ms"));
  bottleneckLink.SetQueue("ns3::DropTailQueue","MaxSize", StringValue ("100p"));


  PointToPointHelper producerLink;
  producerLink.SetDeviceAttribute ("DataRate", StringValue ("1Gbps"));
  producerLink.SetChannelAttribute("Delay", StringValue("2ms"));
  producerLink.SetQueue("ns3::DropTailQueue","MaxSize", StringValue ("100p"));

// devices = p2p.Install(nodes.Get(0), nodes.Get(1));
 //devices.Add(p2p.Install(nodes.Get(1), nodes.Get(2)));
 //devices.Add(p2p.Install(nodes.Get(2), nodes.Get(3)));
 //p2p.SetQueue("ns3::DropTailQueue","MaxSize", StringValue("50p"));

  string PrioQDMap = "1 1 1 1 0 1 1 1 1 1 1 1 1 1 1 1";
  string PrioQDMapFlip = "0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0";
  //consumerLink
  TrafficControlHelper tchConsumerLink;
  uint16_t tchConsumerLinkHandle = tchConsumerLink.SetRootQueueDisc("ns3::PrioQueueDisc","Priomap", StringValue(PrioQDMap));
  TrafficControlHelper::ClassIdList cidConsumerLink = tchConsumerLink.AddQueueDiscClasses(tchConsumerLinkHandle, 2, "ns3::QueueDiscClass");
  uint16_t consumerQdhandle0 = tchConsumerLink.AddChildQueueDisc(tchConsumerLinkHandle,cidConsumerLink[0], "ns3::FifoQueueDisc");
  uint16_t consumerQdhandle1 = tchConsumerLink.AddChildQueueDisc(tchConsumerLinkHandle,cidConsumerLink[1], "ns3::FifoQueueDisc");
  tchConsumerLink.AddPacketFilter(tchConsumerLinkHandle,"ns3::ndn::NdnPacketFilter");
  tchConsumerLink.AddInternalQueues(consumerQdhandle0, 1, "ns3::DropTailQueue","MaxSize",StringValue("100p"));
  tchConsumerLink.AddInternalQueues(consumerQdhandle1, 1, "ns3::DropTailQueue","MaxSize",StringValue("100p"));
  //
  //tchConsumerLink.AddInternalQueues(tchConsumerLinkHandle, 2, "ns3::DropTailQueue","MaxSize",StringValue("100p")); 
 
//producerLink
  TrafficControlHelper tchProducerLink;
  uint16_t tchProducerLinkHandle = tchProducerLink.SetRootQueueDisc("ns3::PrioQueueDisc","Priomap", StringValue(PrioQDMap));
  TrafficControlHelper::ClassIdList cidProducerLink = tchProducerLink.AddQueueDiscClasses(tchProducerLinkHandle, 2, "ns3::QueueDiscClass");
  uint16_t producerQdhandle0 = tchProducerLink.AddChildQueueDisc(tchProducerLinkHandle,cidProducerLink[0], "ns3::FifoQueueDisc");
  uint16_t producerQdhandle1 = tchProducerLink.AddChildQueueDisc(tchProducerLinkHandle,cidProducerLink[1], "ns3::FifoQueueDisc");
  tchProducerLink.AddPacketFilter(tchProducerLinkHandle,"ns3::ndn::NdnPacketFilter");
  
  tchProducerLink.AddInternalQueues(producerQdhandle0, 1, "ns3::DropTailQueue","MaxSize",StringValue("100p"));
  tchProducerLink.AddInternalQueues(producerQdhandle1, 1, "ns3::DropTailQueue","MaxSize",StringValue("100p"));

 // tchProducerLink.AddInternalQueues(tchProducerLinkHandle, 2, "ns3::DropTailQueue","MaxSize",StringValue("100p")); 


//bottleneck
  TrafficControlHelper tchBottleneckLink;
  uint16_t tchBottleneckLinkHandle = tchBottleneckLink.SetRootQueueDisc("ns3::PrioQueueDisc","Priomap", StringValue(PrioQDMap));
  TrafficControlHelper::ClassIdList cidBottleneckLink = tchBottleneckLink.AddQueueDiscClasses(tchBottleneckLinkHandle, 2, "ns3::QueueDiscClass");
  uint16_t bottleneckQdhandle0 = tchBottleneckLink.AddChildQueueDisc(tchBottleneckLinkHandle,cidBottleneckLink[0], "ns3::FifoQueueDisc");
  uint16_t bottleneckQdhandle1 = tchBottleneckLink.AddChildQueueDisc(tchBottleneckLinkHandle,cidBottleneckLink[1], "ns3::FifoQueueDisc");
  tchBottleneckLink.AddPacketFilter(tchBottleneckLinkHandle,"ns3::ndn::NdnPacketFilter");
  
  tchBottleneckLink.AddInternalQueues(bottleneckQdhandle0, 1, "ns3::DropTailQueue","MaxSize",StringValue("100p"));
  tchBottleneckLink.AddInternalQueues(bottleneckQdhandle1, 1, "ns3::DropTailQueue","MaxSize",StringValue("100p"));

//  tchBottleneckLink.AddInternalQueues(tchBottleneckLinkHandle, 2, "ns3::DropTailQueue","MaxSize",StringValue("100p")); 


  NetDeviceContainer devicesConsumerLink = consumerLink.Install (nodes.Get(0),nodes.Get(1));
  devicesConsumerLink.Add(consumerLink.Install (nodes.Get(4),nodes.Get(1)));

  NetDeviceContainer devicesProducerLink = producerLink.Install (nodes.Get(3),nodes.Get(2));
  devicesProducerLink.Add( producerLink.Install(nodes.Get(5),nodes.Get(2)));

  NetDeviceContainer devicesBottleneckLink = bottleneckLink.Install (nodes.Get(1),nodes.Get(2));
  std::cout << "links created" << std::endl;
  // Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  ndnHelper.SetDefaultRoutes(true);
  ndnHelper.InstallAll();

  // NetDevice init
  tchConsumerLink.Install(devicesConsumerLink);

  tchProducerLink.Install(devicesProducerLink);

  QueueDiscContainer qdiscs =  tchBottleneckLink.Install(devicesBottleneckLink);

 // Ptr<TrafficControlLayer> tc = devicesConsumerLink.Get(0)->GetNode ()->GetObject<TrafficControlLayer> ();
 // tc->ScanDevices();
 // Ptr<TrafficControlLayer> tc4 = devicesConsumerLink.Get(2)->GetNode ()->GetObject<TrafficControlLayer> ();
 // tc4->ScanDevices();
 // Ptr<TrafficControlLayer> tc1 = devicesProducerLink.Get(0)->GetNode ()->GetObject<TrafficControlLayer> ();
 // tc1->ScanDevices();
 // Ptr<TrafficControlLayer> tc2 = devicesBottleneckLink.Get(0)->GetNode ()->GetObject<TrafficControlLayer> ();
 // tc2->ScanDevices();
 // Ptr<TrafficControlLayer> tc3 = devicesBottleneckLink.Get(1)->GetNode ()->GetObject<TrafficControlLayer> ();
 // tc3->ScanDevices();
  for(int i = 0; i < 6; i++){
    Ptr<TrafficControlLayer> tc = nodes.Get(i)->GetObject<TrafficControlLayer>();
    tc->ScanDevices();
  }

 
  // traffic-control-layer
 // TrafficControlLayer tcl0 = TrafficControlLayer();
 // TrafficControlLayer tcl1 = TrafficControlLayer();
 // TrafficControlLayer tcl2 = TrafficControlLayer();
 // tcl0.SetNode(nodes.Get(0));
 //
 // tcl1.SetNode(nodes.Get(1));
 // tcl2.SetNode(nodes.Get(2));
 // std::cout << "TCL: nodes set" <<std::endl; 
 // CreateAndAggregateObjectFromTypeId (nodes.Get(0), "ns3::TrafficControlLayer");
 // CreateAndAggregateObjectFromTypeId (nodes.Get(1), "ns3::TrafficControlLayer");
 // CreateAndAggregateObjectFromTypeId (nodes.Get(2), "ns3::TrafficControlLayer");



// set queue
  //setup traffic control 
 // std::cout << "About to setup PrioQueueDisc" << std::endl;
 // TrafficControlHelper tch;
 // //uint16_t handle = tch.SetRootQueueDisc("ns3::PrioQueueDisc", "Priomap", StringValue("0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1"));
 // uint16_t handle = tch.SetRootQueueDisc("ns3::PrioQueueDisc", "Priomap", StringValue("1 1 1 1 0 1 1 1 1 1 1 1 1 1 1 1"));

 // TrafficControlHelper::ClassIdList cid = tch.AddQueueDiscClasses(handle, 2, "ns3::QueueDiscClass");
 // uint16_t qdhandle0 = tch.AddChildQueueDisc(handle,cid[0], "ns3::FifoQueueDisc");
 // uint16_t qdhandle1 = tch.AddChildQueueDisc(handle,cid[1], "ns3::RedQueueDisc");
 // tch.AddPacketFilter(handle,"ns3::ndn::NdnPacketFilter");
 // 
 // tch.AddInternalQueues(qdhandle0, 1, "ns3::DropTailQueue","MaxSize",StringValue("100p"));
 // tch.AddInternalQueues(qdhandle1, 1, "ns3::DropTailQueue","MaxSize",StringValue("100p"));
 //
 // tch.AddInternalQueues(qdhandle0, 2, "ns3::DropTailQueue","MaxSize",StringValue("100p"));
 // tch.AddInternalQueues(qdhandle1, 2, "ns3::DropTailQueue","MaxSize",StringValue("100p"));

  std::cout<<"setting up queue telem"<<std::endl;
  std::cout << qdiscs.GetN() << std::endl;
  Ptr<QueueDisc> q = qdiscs.Get (0);
  q->TraceConnectWithoutContext ("PacketsInQueue", MakeCallback (&TcPacketsInQueueTrace));
  Config::ConnectWithoutContext ("/NodeList/1/$ns3::TrafficControlLayer/RootQueueDiscList/0/SojournTime",
                                 MakeCallback (&SojournTimeTrace));
  Ptr<QueueDisc> q2 = qdiscs.Get (1);
  q2->TraceConnectWithoutContext ("PacketsInQueue", MakeCallback (&TcPacketsInQueueTrace));
  Config::ConnectWithoutContext ("/NodeList/2/$ns3::TrafficControlLayer/RootQueueDiscList/0/SojournTime",
                                 MakeCallback (&SojournTimeTrace));


  //Ptr<NetDevice> nd = devices.Get(3);
  //Ptr<PointToPointNetDevice> ptpnd = DynamicCast<PointToPointNetDevice>(nd);
  //Ptr<Queue<Packet> > queue = ptpnd->GetQueue();
  //queue->TraceConnectWithoutContext("PacketsInQueue", MakeCallback(&DevicePacketsInQueueTrace));
 
  //Ptr<TrafficControlLayer> tc3 = devices.Get(3)->GetNode ()->GetObject<TrafficControlLayer> ();
  //tc3->ScanDevices();
 // Ptr<TrafficControlLayer> tc4 = devices.Get(4)->GetNode ()->GetObject<TrafficControlLayer> ();
 // tc4->ScanDevices();
 // Ptr<TrafficControlLayer> tc5 = devices.Get(5)->GetNode ()->GetObject<TrafficControlLayer> ();
 // tc5-> ScanDevices();
 // Ptr<TrafficControlLayer> tc6 = devices.Get(6)->GetNode ()->GetObject<TrafficControlLayer> ();
 // tc6 -> ScanDevices();
 // Ptr<TrafficControlLayer> tc7 = devices.Get(7)->GetNode ()->GetObject<TrafficControlLayer> (); 
 // tc7 -> ScanDevices();
 // Ptr<TrafficControlLayer> tc8 = devices.Get(8)->GetNode ()->GetObject<TrafficControlLayer> (); 
 // tc8 -> ScanDevices();

  string streaming_service_a = "/streaming_service_A";
  string social_network_a = "/social_network_A";
  

// Install NDN stack on all nodes
  //ndn::StackHelper ndnHelper;
  //ndnHelper.SetDefaultRoutes(true);
  //ndnHelper.InstallAll();
  std::cout<<"setting up strategy"<<endl; 
    // Choosing forwarding strategy
  ndn::StrategyChoiceHelper::InstallAll(social_network_a,"/localhost/nfd/strategy/multicast");
  ndn::StrategyChoiceHelper::InstallAll(streaming_service_a,"/localhost/nfd/strategy/multicast");
  // Installing applications

  // Consumer
  //ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
  // Consumer will request /prefix/0, /prefix/1, ...
  //consumerHelper.SetPrefix("/prefix");
  //consumerHelper.SetAttribute("Frequency", StringValue("300")); //  interests a second
   
  ndn::AppHelper consumerHelperN4("ns3::ndn::ConsumerCbr");
  consumerHelperN4.SetPrefix(social_network_a+"/static");
  consumerHelperN4.SetAttribute("Frequency", StringValue(std::to_string(std_prefix_rate))); //  interests a second


  //ndn::AppHelper prioConsumerHelper("ns3::ndn::ConsumerCbr");
  //prioConsumerHelper.SetPrefix("/prio");
  //prioConsumerHelper.SetAttribute("Frequency", StringValue("3000")); 

  ndn::AppHelper prioConsumerHelperN0("ns3::ndn::ConsumerCbr");
  prioConsumerHelperN0.SetPrefix(streaming_service_a+"/live/formula1");
  prioConsumerHelperN0.SetAttribute("Frequency", StringValue(std::to_string(prio_prefix_rate))); 

  
  auto prioappsN0 = prioConsumerHelperN0.Install(nodes.Get(0));
  prioappsN0.Stop(Seconds(10.0));

  auto appsN4 = consumerHelperN4.Install(nodes.Get(4));
  appsN4.Stop(Seconds(10.0));


  // Producer
  ndn::AppHelper producerHelper("ns3::ndn::Producer");
  // Producer will reply to all requests starting with /prefix
  producerHelper.SetPrefix(social_network_a);
  producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
  producerHelper.Install(nodes.Get(3)); // last node
 
// Producer
  ndn::AppHelper prioProducerHelper("ns3::ndn::Producer");
  // Producer will reply to all requests starting with /prefix
  prioProducerHelper.SetPrefix(streaming_service_a);
  prioProducerHelper.SetAttribute("PayloadSize", StringValue("1024"));
  prioProducerHelper.Install(nodes.Get(5)); // last node
  

  // Application-layer Latency tracer
  ndn::AppDelayTracer::InstallAll(out_dir+prefix+"-adt-all.txt");
  ndn::AppDelayTracer::Install(nodes.Get(0),out_dir+prefix+"-adt-n0.txt"); 
  ndn::AppDelayTracer::Install(nodes.Get(4),out_dir+prefix+"-adt-n4.txt");


  PcapWriter traceN0(out_dir+prefix+"-ndn_sim_n0.pcap");
  Config::ConnectWithoutContext("/NodeList/0/DeviceList/*/$ns3::PointToPointNetDevice/MacTx",
		  MakeCallback(&PcapWriter::TracePacket, &traceN0));
  Config::ConnectWithoutContext("/NodeList/0/DeviceList/*/$ns3::PointToPointNetDevice/MacRx",
		  MakeCallback(&PcapWriter::TracePacket, &traceN0));

  PcapWriter traceN4(out_dir+prefix+"-ndn_sim_n4.pcap");
  Config::ConnectWithoutContext("/NodeList/4/DeviceList/*/$ns3::PointToPointNetDevice/MacTx",
		  MakeCallback(&PcapWriter::TracePacket, &traceN4));
  Config::ConnectWithoutContext("/NodeList/4/DeviceList/*/$ns3::PointToPointNetDevice/MacRx",
		  MakeCallback(&PcapWriter::TracePacket, &traceN4));


  Simulator::Stop(Seconds(10.0));
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
