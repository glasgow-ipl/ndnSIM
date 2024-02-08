#ifndef IPV6_PACKET_FILTER_H
#define IPV6_PACKET_FILTER_H

#include "ns3/object.h"
#include "ns3/packet-filter.h"

namespace ns3{
namespace ndn{ 
  class NdnPacketFilter: public PacketFilter {
    public:

      /**
       * Retnrs TypeID
       */
      static TypeId GetTypeId(void);
      
      /**
       * default constructor
       */
      NdnPacketFilter ();
      virtual ~NdnPacketFilter ();

    private:
      virtual bool CheckProtocol (Ptr<QueueDiscItem> item) const;
      virtual int32_t DoClassify (Ptr<QueueDiscItem> item) const;
  };
} //namespace ndn
} //namespace ns3

#endif
