#include "ns3/log.h"
#include "ns3/enum.h"
#include "ns3/uinteger.h"
#include "ndn-packet-filter.hpp"
#include "ndn-queue-disc-item.hpp"

NS_LOG_COMPONENT_DEFINE ("ndn.NdnPacketFilter");
namespace ns3{
namespace ndn{


NS_OBJECT_ENSURE_REGISTERED(NdnPacketFilter);

TypeId
NdnPacketFilter::GetTypeId(void)
{
  static TypeId tid=
          TypeId("ns3::ndn::NdnPacketFilter")
          .SetParent<PacketFilter> ()
	  .AddConstructor<Object> ()
          .SetGroupName("ndn")
          ;
  return tid;
}

NdnPacketFilter::NdnPacketFilter ()
{
  NS_LOG_FUNCTION(this);
}

NdnPacketFilter::~NdnPacketFilter ()
{
  NS_LOG_FUNCTION(this);
}


bool
NdnPacketFilter::CheckProtocol (Ptr<QueueDiscItem> item) const
{
  NS_LOG_FUNCTION (this << item);
  return (DynamicCast<NdnQueueDiscItem> (item) !=0);

}

}
}
