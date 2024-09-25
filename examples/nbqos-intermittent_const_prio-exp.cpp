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

#include "ns3/config-store-module.h"


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
		  //std::cout << "TcPktsInQueue," << Simulator::Now() << ", TcPacketsInQueue0, " << oldValue << "," << newValue << std::endl;
	}

void
	TcPacketsInQueueTrace2 (uint32_t oldValue, uint32_t newValue)
	{
		 // std::cout << "TcPktsInQueue," << Simulator::Now() << ", TcPacketsInQueue1, " << oldValue << "," << newValue << std::endl;
	}

void
	TcDropTraceN2Q0(Ptr<const ns3::QueueDiscItem> pkt)
	{
		//std::cout << "TcDropTrace," << Simulator::Now() << ", TcDropTraceN2Q0, " << "drop" << std::endl;
	}
void
	TcDropTraceN2Q1(Ptr<const ns3::QueueDiscItem> pkt)
	{
		//std::cout << "TcDropTrace," << Simulator::Now() << ", TcDropTraceN2Q1, " << "drop" << std::endl;
	}

void
	QDiscDropTrace(Ptr<const ns3::QueueDiscItem> pkt)
	{
		//std::cout << "QDiscDropTrace," << Simulator::Now() << ", QDiscDropTraceN2Root, " << "drop" << std::endl;
	}
void
	ChildQDiscDropTrace(Ptr<const ns3::QueueDiscItem> pkt)
	{
		//std::cout << "ChildQDiscDropTrace," << Simulator::Now() << ", QDiscDropTraceN2Child0, " << "drop" << std::endl;
	}
void
	ChildQDiscDropTrace1(Ptr<const ns3::QueueDiscItem> pkt)
	{
		//std::cout << "ChildQDiscDropTrace," << Simulator::Now() << ", QDiscDropTraceN2Child1, " << "drop" << std::endl;
	}



void
	DevicePacketsInQueueTrace (uint32_t oldValue, uint32_t newValue)
	{
	        //std::cout << "DevicePacketsInQueue," << Simulator::Now() << ", DevicePacketsInQueueN2, " << oldValue << "," << newValue << std::endl;
	}
void
	DeviceDropTraceN2(Ptr<const ns3::Packet> pkt)
	{
		//std::cout << "DevDropTrace," << Simulator::Now() << ", DevDropTraceN2, " << "drop" << std::endl;
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
                  //std::cout << "SojournTimeTrace,"<<Simulator::Now() << ",RootQueueDisc, " << sojournTime.ToDouble (Time::MS) << "ms" << std::endl;
        }
void
        SojournTimeTraceC0 (Time sojournTime)
        {
                  //std::cout << "SojournTimeTrace,"<<Simulator::Now() << ",ChildQueueDisc0, " << sojournTime.ToDouble (Time::MS) << "ms" << std::endl;
        }
void
        SojournTimeTraceC1 (Time sojournTime)
        {
                  //std::cout << "SojournTimeTrace,"<<Simulator::Now() << ",ChildQueueDisc1, " << sojournTime.ToDouble (Time::MS) << "ms" << std::endl;
        }

 // Function to Set Frequency
  void prio_freq_config_schedule_handler(int sim_time, uint32_t rate, uint32_t base_rate){
    uint32_t new_rate = sim_time*rate + base_rate;
    std::cout << "sim_time:"<<std::to_string(sim_time)<<" rate:"<<std::to_string(rate)<<" new_rate:"<<std::to_string(new_rate)<<std::endl;
    Config::Set("/NodeList/0/$ns3::Node/ApplicationList/0/$ns3::ndn::ConsumerCbr/Frequency",StringValue(std::to_string(new_rate)));
    if(new_rate==0){
      return;
    }
    std::cout << std::endl << "Setting /NodeList/0/$ns3::Node/ApplicationList/0/$ns3::ndn::ConsumerCbr/Frequency, "+std::to_string(new_rate) << std::endl << std::endl;
    Config::MatchContainer matches = Config::LookupMatches("/Nodelist/0/ApplicationList/0/$ns3::ndn::ConsumerCbr/Frequency/*");
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
  string exp_name = "nbqos_intermittent_const_prio_exp";
  uint32_t std_prefix_rate = 300; //default 300 pps
  uint32_t prio_prefix_rate = 300; //default 300 pps
  uint32_t std_payload_size = 1024;
  uint32_t prio_payload_size = 1024; //default 1024 Bytes
  uint32_t ndQueueSize=5;
  uint32_t q_disc_size=100;
  uint32_t increment_step = 25; //default 25 pps
  int increment_interval = 2;
  string out_dir = "./";
  uint64_t run_num = 1; //defaut run number
  double duration = 30.0; 

  CommandLine cmd (__FILE__);
  cmd.AddValue("stdPRate", "Standard packet rate (pps) per app instance", std_prefix_rate);
  cmd.AddValue("prioPRate", "Prioritised packet rate (pps) per app instance", prio_prefix_rate);
  cmd.AddValue("stdPSize", "Standard packet data payload size per app instance", std_payload_size);
  cmd.AddValue("prioPSize", "Prioritised packet data payload size per app instance", prio_payload_size);
  cmd.AddValue("outDir", "Output directory",out_dir);
  cmd.AddValue("expName", "Experiment name override", exp_name);
  cmd.AddValue("runNum", "NS3 Run Number",run_num);
  cmd.AddValue("date","Date string override",date);
  cmd.AddValue("ndQueueSize", "NetDeviceQueueSize",ndQueueSize);
  cmd.AddValue("QDiscSize", "QueueDisc child queue size",q_disc_size);
  cmd.AddValue("duration", "Duration of the experiment",duration);
  //cmd.AddValue("incr_step", "Increment step for prioritised_prefix",increment_step);
  cmd.AddValue("incr_interval","Increment interval for standard prefix", increment_interval);
  cmd.Parse(argc,argv);
  ns3::RngSeedManager::SetRun(run_num); 
  prefix.append(date);
  prefix.append("-"+exp_name);
  prefix.append("-"+std::to_string(duration)+"sec");
  prefix.append("-"+std::to_string(run_num));
  prefix.append("-"+std::to_string(std_prefix_rate)+"_std_pps");
  prefix.append("-"+std::to_string(prio_prefix_rate)+"_prio_pps");
  prefix.append("-"+std::to_string(std_payload_size)+"_std_bytes");
  prefix.append("-"+std::to_string(prio_payload_size)+"_prio_bytes");
  prefix.append("-"+std::to_string(ndQueueSize)+"_NDqueuePkts");
  prefix.append("-"+std::to_string(q_disc_size)+"_QDiscPkts");
  //prefix.append("-"+std::to_string(increment_step)+"_incr_step");
  prefix.append("-"+std::to_string(increment_interval)+"_incr_interval");


  if(out_dir.back() != '/'){
    out_dir.append("/");
  }

  std::string ndQueueSize_str=std::to_string(ndQueueSize);
  ndQueueSize_str.append("p");

  std::string q_disc_size_str=std::to_string(q_disc_size);
  q_disc_size_str.append("p");

  std::string std_payload_size_str=std::to_string(std_payload_size);
  std::string prio_payload_size_str=std::to_string(prio_payload_size);

  // setting default parameters for PointToPoint links and channels
  Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("2.0Mbps"));
  Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("20ms"));
  //Config::SetDefault("ns3::DropTailQueue<Packet>::MaxSize", StringValue("20p"));
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
  consumerLink.SetQueue("ns3::DropTailQueue","MaxSize", StringValue (ndQueueSize_str));

  
  PointToPointHelper bottleneckLink;
  bottleneckLink.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  bottleneckLink.SetChannelAttribute("Delay", StringValue("20ms"));
  bottleneckLink.SetQueue("ns3::DropTailQueue","MaxSize", StringValue (ndQueueSize_str));


  PointToPointHelper producerLink;
  producerLink.SetDeviceAttribute ("DataRate", StringValue ("1Gbps"));
  producerLink.SetChannelAttribute("Delay", StringValue("2ms"));
  producerLink.SetQueue("ns3::DropTailQueue","MaxSize", StringValue (ndQueueSize_str));

// devices = p2p.Install(nodes.Get(0), nodes.Get(1));
 //devices.Add(p2p.Install(nodes.Get(1), nodes.Get(2)));
 //devices.Add(p2p.Install(nodes.Get(2), nodes.Get(3)));
 //p2p.SetQueue("ns3::DropTailQueue","MaxSize", StringValue("50p"));

  string PrioQDMap = "1 1 1 1 0 1 1 1 1 1 1 1 1 1 1 1";
  string PrioQDMapFlip = "0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0";
  //consumerLink
  TrafficControlHelper tchConsumerLink;
  uint16_t tchConsumerLinkHandle = tchConsumerLink.SetRootQueueDisc("ns3::PrioQueueDisc","Priomap", StringValue(PrioQDMap));
  //tchConsumerLink.AddInternalQueues(tchConsumerLinkHandle,2, "ns3::DropTailQueue", "MaxSize", StringValue("100p"));
  TrafficControlHelper::ClassIdList cidConsumerLink = tchConsumerLink.AddQueueDiscClasses(tchConsumerLinkHandle, 2, "ns3::QueueDiscClass");
  uint16_t consumerQdhandle0 = tchConsumerLink.AddChildQueueDisc(tchConsumerLinkHandle,cidConsumerLink[0], "ns3::FifoQueueDisc");
  uint16_t consumerQdhandle1 = tchConsumerLink.AddChildQueueDisc(tchConsumerLinkHandle,cidConsumerLink[1], "ns3::FifoQueueDisc");
  tchConsumerLink.AddPacketFilter(tchConsumerLinkHandle,"ns3::ndn::NdnPacketFilter");
  
  tchConsumerLink.AddInternalQueues(consumerQdhandle0, 1, "ns3::DropTailQueue","MaxSize",StringValue(q_disc_size_str));
  tchConsumerLink.AddInternalQueues(consumerQdhandle1, 1, "ns3::DropTailQueue","MaxSize",StringValue(q_disc_size_str));
  
  //tchConsumerLink.AddInternalQueues(tchConsumerLinkHandle, 2, "ns3::DropTailQueue","MaxSize",StringValue("100p")); 
 
//producerLink
  TrafficControlHelper tchProducerLink;
  uint16_t tchProducerLinkHandle = tchProducerLink.SetRootQueueDisc("ns3::PrioQueueDisc","Priomap", StringValue(PrioQDMap));
  //tchProducerLink.AddInternalQueues(tchProducerLinkHandle,2, "ns3::DropTailQueue", "MaxSize", StringValue("100p"));
  TrafficControlHelper::ClassIdList cidProducerLink = tchProducerLink.AddQueueDiscClasses(tchProducerLinkHandle, 2, "ns3::QueueDiscClass");
  uint16_t producerQdhandle0 = tchProducerLink.AddChildQueueDisc(tchProducerLinkHandle,cidProducerLink[0], "ns3::FifoQueueDisc");
  uint16_t producerQdhandle1 = tchProducerLink.AddChildQueueDisc(tchProducerLinkHandle,cidProducerLink[1], "ns3::FifoQueueDisc");
  tchProducerLink.AddPacketFilter(tchProducerLinkHandle,"ns3::ndn::NdnPacketFilter");
  
  tchProducerLink.AddInternalQueues(producerQdhandle0, 1, "ns3::DropTailQueue","MaxSize",StringValue(q_disc_size_str));
  tchProducerLink.AddInternalQueues(producerQdhandle1, 1, "ns3::DropTailQueue","MaxSize",StringValue(q_disc_size_str));

 // tchProducerLink.AddInternalQueues(tchProducerLinkHandle, 2, "ns3::DropTailQueue","MaxSize",StringValue("100p")); 


//bottleneck
  TrafficControlHelper tchBottleneckLink;
  uint16_t tchBottleneckLinkHandle = tchBottleneckLink.SetRootQueueDisc("ns3::PrioQueueDisc","Priomap", StringValue(PrioQDMap));
  //tchBottleneckLink.AddInternalQueues(tchBottleneckLinkHandle,2, "ns3::DropTailQueue", "MaxSize", StringValue("100p"));
  TrafficControlHelper::ClassIdList cidBottleneckLink = tchBottleneckLink.AddQueueDiscClasses(tchBottleneckLinkHandle, 2, "ns3::QueueDiscClass");
  uint16_t bottleneckQdhandle0 = tchBottleneckLink.AddChildQueueDisc(tchBottleneckLinkHandle,cidBottleneckLink[0], "ns3::FifoQueueDisc");
  uint16_t bottleneckQdhandle1 = tchBottleneckLink.AddChildQueueDisc(tchBottleneckLinkHandle,cidBottleneckLink[1], "ns3::FifoQueueDisc");
  tchBottleneckLink.AddPacketFilter(tchBottleneckLinkHandle,"ns3::ndn::NdnPacketFilter");
  
  tchBottleneckLink.AddInternalQueues(bottleneckQdhandle0, 1, "ns3::DropTailQueue","MaxSize",StringValue(q_disc_size_str));
  tchBottleneckLink.AddInternalQueues(bottleneckQdhandle1, 1, "ns3::DropTailQueue","MaxSize",StringValue(q_disc_size_str));
  

  //tchBottleneckLink.SetQueueLimits("ns3::DynamicQueueLimits", "HoldTime", StringValue ("4ms"));
//  tchBottleneckLink.AddInternalQueues(tchBottleneckLinkHandle, 2, "ns3::DropTailQueue","MaxSize",StringValue("100p")); 

  std::cout << "setting up links" << std::endl;

  NetDeviceContainer devicesConsumerLink = consumerLink.Install (nodes.Get(0),nodes.Get(1));
  devicesConsumerLink.Add(consumerLink.Install (nodes.Get(4),nodes.Get(1)));

  std::cout << "Consumer links NetDevice Count:" << devicesConsumerLink.GetN() << std::endl;

  NetDeviceContainer devicesProducerLink = producerLink.Install (nodes.Get(3),nodes.Get(2));
  devicesProducerLink.Add( producerLink.Install(nodes.Get(5),nodes.Get(2)));

  std::cout << "Producer links NetDevice Count:" << devicesProducerLink.GetN() << std::endl;

  NetDeviceContainer devicesBottleneckLink = bottleneckLink.Install (nodes.Get(1),nodes.Get(2));

  std::cout << "Bottleneck link NetDevice Count:" << devicesBottleneckLink.GetN() << std::endl;
  
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
    std::cout << "Setting up node:" << i << std::endl;
    //CreateAndAggregateObjectFromTypeId (nodes.Get(i), "ns3::TrafficControlLayer");
    Ptr<TrafficControlLayer> tc = nodes.Get(i)->GetObject<TrafficControlLayer>();
    tc->ScanDevices();
  }

 

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

  std::cout << "setting up queue telem " << std::endl;
  std::cout << "qdiscs.GetN() = "<<qdiscs.GetN() << std::endl;
  Ptr<QueueDisc> q = qdiscs.Get (0);
  std::cout << "q0: " << q->GetTypeId().GetName() << std::endl;
  //q->TraceConnectWithoutContext ("PacketsInQueue", MakeCallback (&TcPacketsInQueueTrace));
  //$ns3::NodeListPriv/NodeList/1/$ns3::Node/$ns3::TrafficControlLayer/RootQueueDiscList/1/$ns3::PrioQueueDisc/QueueDiscClassList/0/$ns3::QueueDiscClass/QueueDisc/$ns3::FifoQueueDisc/InternalQueueList/0/
  Config::ConnectWithoutContext ("/NodeList/2/$ns3::TrafficControlLayer/RootQueueDiscList/2/$ns3::PrioQueueDisc/QueueDiscClassList/0/$ns3::QueueDiscClass/QueueDisc/$ns3::FifoQueueDisc/InternalQueueList/0/PacketsInQueue",MakeCallback(&TcPacketsInQueueTrace));
  Config::ConnectWithoutContext ("/NodeList/2/$ns3::TrafficControlLayer/RootQueueDiscList/2/$ns3::PrioQueueDisc/QueueDiscClassList/1/$ns3::QueueDiscClass/QueueDisc/$ns3::FifoQueueDisc/InternalQueueList/0/PacketsInQueue",MakeCallback(&TcPacketsInQueueTrace2));
  Config::ConnectWithoutContext ("/NodeList/2/$ns3::TrafficControlLayer/RootQueueDiscList/2/$ns3::PrioQueueDisc/QueueDiscClassList/0/$ns3::QueueDiscClass/QueueDisc/$ns3::FifoQueueDisc/InternalQueueList/0/Drop",
                                 MakeCallback (&TcDropTraceN2Q0));
  Config::ConnectWithoutContext ("/NodeList/2/$ns3::TrafficControlLayer/RootQueueDiscList/2/$ns3::PrioQueueDisc/QueueDiscClassList/1/$ns3::QueueDiscClass/QueueDisc/$ns3::FifoQueueDisc/InternalQueueList/0/Drop",
                                 MakeCallback (&TcDropTraceN2Q1));

  Config::ConnectWithoutContext ("/NodeList/2/$ns3::TrafficControlLayer/RootQueueDiscList/2/$ns3::PrioQueueDisc/Drop",MakeCallback(&QDiscDropTrace));
  Config::ConnectWithoutContext ("/NodeList/2/$ns3::TrafficControlLayer/RootQueueDiscList/2/$ns3::PrioQueueDisc/QueueDiscClassList/0/$ns3::QueueDiscClass/QueueDisc/$ns3::FifoQueueDisc/Drop",MakeCallback(&ChildQDiscDropTrace));
  Config::ConnectWithoutContext ("/NodeList/2/$ns3::TrafficControlLayer/RootQueueDiscList/2/$ns3::PrioQueueDisc/QueueDiscClassList/1/$ns3::QueueDiscClass/QueueDisc/$ns3::FifoQueueDisc/Drop",MakeCallback(&ChildQDiscDropTrace1));


//Config::ConnectWithoutContext ("/NodeList/1/$ns3::TrafficControlLayer/RootQueueDiscList/0/InternalQueueList/1",MakeCallback(&TcPacketsInQueueTrace2));

  Config::ConnectWithoutContext ("/NodeList/2/$ns3::TrafficControlLayer/RootQueueDiscList/2/SojournTime",
                                 MakeCallback (&SojournTimeTrace));
  Config::ConnectWithoutContext ("/NodeList/2/$ns3::TrafficControlLayer/RootQueueDiscList/2/$ns3::PrioQueueDisc/QueueDiscClassList/0/$ns3::QueueDiscClass/QueueDisc/$ns3::FifoQueueDisc/SojournTime",MakeCallback(&SojournTimeTraceC0));
  Config::ConnectWithoutContext ("/NodeList/2/$ns3::TrafficControlLayer/RootQueueDiscList/2/$ns3::PrioQueueDisc/QueueDiscClassList/1/$ns3::QueueDiscClass/QueueDisc/$ns3::FifoQueueDisc/SojournTime",MakeCallback(&SojournTimeTraceC1));

  Ptr<QueueDisc> q2 = qdiscs.Get (1);
  //q2->TraceConnectWithoutContext ("PacketsInQueue", MakeCallback (&TcPacketsInQueueTrace2));
  Config::ConnectWithoutContext ("/NodeList/2/$ns3::Node/DeviceList/2/$ns3::PointToPointNetDevice/TxQueue/PacketsInQueue",
                                 MakeCallback (&DevicePacketsInQueueTrace));
  Config::ConnectWithoutContext ("/NodeList/2/$ns3::Node/DeviceList/2/$ns3::PointToPointNetDevice/TxQueue/Drop",
                                 MakeCallback (&DeviceDropTraceN2));


  //Config::ConnectWithoutContext ("
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
  consumerHelperN4.SetPrefix(social_network_a+"/resource/static");
  consumerHelperN4.SetAttribute("Frequency", StringValue(std::to_string(std_prefix_rate))); //  interests a second


  //ndn::AppHelper prioConsumerHelper("ns3::ndn::ConsumerCbr");
  //prioConsumerHelper.SetPrefix("/prio");
  //prioConsumerHelper.SetAttribute("Frequency", StringValue("3000")); 

  ndn::AppHelper prioConsumerHelperN0("ns3::ndn::ConsumerCbr");
  prioConsumerHelperN0.SetPrefix(streaming_service_a+"/live/formula1");
  prioConsumerHelperN0.SetAttribute("Frequency", StringValue(std::to_string(prio_prefix_rate))); 

  int peak_duration = 2;

// if(std_prefix_rate >0) {
//    auto appsN4 = consumerHelperN4.Install(nodes.Get(4));
//    appsN4.Stop(Seconds(duration));
//  }
  if(prio_prefix_rate >0) {
    auto prioappsN0 = prioConsumerHelperN0.Install(nodes.Get(0));
    prioappsN0.Stop(Seconds(duration));
  }


  // Producer
  ndn::AppHelper producerHelper("ns3::ndn::Producer");
  // Producer will reply to all requests starting with /prefix
  producerHelper.SetPrefix(social_network_a);
  producerHelper.SetAttribute("PayloadSize", StringValue(std_payload_size_str));
  producerHelper.Install(nodes.Get(3)); // last node
 
  // Producer
  ndn::AppHelper prioProducerHelper("ns3::ndn::Producer");
  // Producer will reply to all requests starting with /prefix
  prioProducerHelper.SetPrefix(streaming_service_a);
  prioProducerHelper.SetAttribute("PayloadSize", StringValue(prio_payload_size_str));
  prioProducerHelper.Install(nodes.Get(5)); // last node
 

    // Schedule increate in rate
  for (int time_i = 1; time_i<duration;time_i++){
    if(time_i % increment_interval == 0){
	      
      if(std_prefix_rate > 0) {
	std::cout<<"Setting up stdApp at: "<<time_i<<std::endl;
        auto appsN4 = consumerHelperN4.Install(nodes.Get(4));
	appsN4.Start(Seconds(time_i));
        appsN4.Stop(Seconds(time_i+peak_duration));
      }
    }
  }

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

//   PcapWriter traceN3(out_dir+prefix+"-ndn_sim_n3.pcap");
//   Config::ConnectWithoutContext("/NodeList/3/DeviceList/*/$ns3::PointToPointNetDevice/MacTx",
// 		  MakeCallback(&PcapWriter::TracePacket, &traceN3));
//   Config::ConnectWithoutContext("/NodeList/3/DeviceList/*/$ns3::PointToPointNetDevice/MacRx",
// 		  MakeCallback(&PcapWriter::TracePacket, &traceN3));
// 
// 
//   PcapWriter traceN5(out_dir+prefix+"-ndn_sim_n5.pcap");
//   Config::ConnectWithoutContext("/NodeList/5/DeviceList/*/$ns3::PointToPointNetDevice/MacTx",
// 		  MakeCallback(&PcapWriter::TracePacket, &traceN5));
//   Config::ConnectWithoutContext("/NodeList/5/DeviceList/*/$ns3::PointToPointNetDevice/MacRx",
// 		  MakeCallback(&PcapWriter::TracePacket, &traceN5));
// 
// 
//   PcapWriter traceN2(out_dir+prefix+"-ndn_sim_n2-dev_2.pcap");
//   Config::ConnectWithoutContext("/NodeList/2/DeviceList/2/$ns3::PointToPointNetDevice/MacTx",
// 		                MakeCallback(&PcapWriter::TracePacket, &traceN2));
//   Config::ConnectWithoutContext("/NodeList/2/DeviceList/2/$ns3::PointToPointNetDevice/MacRx",
// 			        MakeCallback(&PcapWriter::TracePacket, &traceN2));
//   PcapWriter traceN2D2TX(out_dir+prefix+"-ndn_sim_n2-dev_2-tx.pcap");
//   Config::ConnectWithoutContext("/NodeList/2/DeviceList/2/$ns3::PointToPointNetDevice/MacTx",
// 		                MakeCallback(&PcapWriter::TracePacket, &traceN2D2TX));
// 
//   PcapWriter traceN2D1(out_dir+prefix+"-ndn_sim_n2-dev_1.pcap");
//   Config::ConnectWithoutContext("/NodeList/2/DeviceList/1/$ns3::PointToPointNetDevice/MacTx",
// 		                MakeCallback(&PcapWriter::TracePacket, &traceN2D1));
//   Config::ConnectWithoutContext("/NodeList/2/DeviceList/1/$ns3::PointToPointNetDevice/MacRx",
// 			        MakeCallback(&PcapWriter::TracePacket, &traceN2D1)); 
//   
// 
//   PcapWriter traceN2D1rx(out_dir+prefix+"-ndn_sim_n2-dev_1-rx.pcap");
//   Config::ConnectWithoutContext("/NodeList/2/DeviceList/1/$ns3::PointToPointNetDevice/MacRx",
// 			        MakeCallback(&PcapWriter::TracePacket, &traceN2D1rx));
// 
//   PcapWriter traceN2D0(out_dir+prefix+"-ndn_sim_n2-dev_0.pcap");
//   Config::ConnectWithoutContext("/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/MacTx",
// 		                MakeCallback(&PcapWriter::TracePacket, &traceN2D0));
//   Config::ConnectWithoutContext("/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/MacRx",
// 			        MakeCallback(&PcapWriter::TracePacket, &traceN2D0)); 
//   
//   
//   PcapWriter traceN1(out_dir+prefix+"-ndn_sim_n1.pcap");
//   Config::ConnectWithoutContext("/NodeList/1/DeviceList/*/$ns3::PointToPointNetDevice/MacTx",
// 				MakeCallback(&PcapWriter::TracePacket, &traceN1));
//   Config::ConnectWithoutContext("/NodeList/1/DeviceList/*/$ns3::PointToPointNetDevice/MacRx",
// 			        MakeCallback(&PcapWriter::TracePacket, &traceN1));

  Simulator::Stop(Seconds(duration));

  std::string ConfigOutputPath = out_dir+prefix+"-output-attributes.txt";

  Config::SetDefault ("ns3::ConfigStore::Filename", StringValue(ConfigOutputPath));
  Config::SetDefault ("ns3::ConfigStore::FileFormat", StringValue("RawText"));
  Config::SetDefault("ns3::ConfigStore::Mode",StringValue("Save"));
  ConfigStore outputConfig2;
  outputConfig2.ConfigureAttributes();

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
