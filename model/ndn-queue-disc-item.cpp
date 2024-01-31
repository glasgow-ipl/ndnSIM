#include "ndn-queue-disc-item.hpp"

#include "ns3/log.h"
#include "ns3/queue.h"

namespace ns3 {
  using namespace ndn;

  NS_LOG_COMPONENT_DEFINE("NdnQueueDiscItem");

  NdnQueueDiscItem::NdnQueueDiscItem (Ptr<Packet> p, uint16_t protocol, uint16_t fb_field = 0)
    : QueueDiscItem (p, ns3::Address() , protocol)
  {
    m_fb_field = fb_field;
  }

  NdnQueueDiscItem::~NdnQueueDiscItem()
  {
    NS_LOG_FUNCTION(this);
  }
  
  uint32_t NdnQueueDiscItem::GetSize (void) const
  {
    NS_LOG_FUNCTION(this);
    Ptr<Packet> p = GetPacket ();
    NS_ASSERT (p!=0);
    uint32_t ret = p->GetSize();
    return ret;
  }

  void
  NdnQueueDiscItem::Print (std::ostream& os) const
  {
    os << GetPacket() << " "
       << "proto " << (uint16_t) GetProtocol () << " "
       << "txq " << (uint16_t) GetTxQueueIndex ()
    ;
  }

  uint16_t NdnQueueDiscItem::GetFbField (void) const
  {
    NS_LOG_FUNCTION(this);
    return this->m_fb_field;
  }


}
