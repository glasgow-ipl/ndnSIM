#include "ns3/log.h"
#include "ns3/enum.h"
#include "ns3/uinteger.h"

#include "ns3/socket.h"

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
	  .AddConstructor<NdnPacketFilter> ()
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

int32_t 
NdnPacketFilter::DoClassify (Ptr<QueueDiscItem> item) const
{
  NS_LOG_FUNCTION ("NdnPacketFilter::DoClassify()");
  NS_LOG_FUNCTION (DynamicCast<NdnQueueDiscItem>(item));
  return Socket::IpTos2Priority(DynamicCast<NdnQueueDiscItem> (item)->GetFbField());
}

}
}
