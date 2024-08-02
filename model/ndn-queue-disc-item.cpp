#include "ndn-queue-disc-item.hpp"

#include <boost/utility/binary.hpp>

#include "ns3/log.h"
#include "ns3/queue.h"

namespace ns3 {
  using namespace ndn;

  NS_LOG_COMPONENT_DEFINE("NdnQueueDiscItem");

  NdnQueueDiscItem::NdnQueueDiscItem (Ptr<Packet> p, const Address& addr, uint16_t protocol, const BlockHeader &header, uint16_t fb_field = 0, std::string name="")
    : QueueDiscItem (p, addr , protocol),
    m_fb_field(fb_field),
    m_name(name)
  {
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

  std::string NdnQueueDiscItem::GetName(void) const
  {
    NS_LOG_FUNCTION(this);
    return this->m_name;
  }

  void NdnQueueDiscItem::AddHeader(void)
  {
    NS_LOG_FUNCTION(this);
  }

  bool
  NdnQueueDiscItem::Mark(void)
  {
    NS_LOG_FUNCTION(this);
    return false;
  }


}
