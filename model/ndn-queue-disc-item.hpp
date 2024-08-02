#ifndef NDN_QUEUE_DISC_ITEM_H
#define NDN_QUEUE_DISC_ITEM_H

#include "ns3/object.h"
#include "ns3/queue-item.h"

#include "ndn-block-header.hpp"

namespace ns3{
  namespace ndn{
    class NdnQueueDiscItem : public QueueDiscItem {
      public:
        NdnQueueDiscItem (Ptr<Packet> p, const Address & addr, uint16_t protocol, const BlockHeader &header, uint16_t fb_field, std::string name);

        virtual ~ NdnQueueDiscItem ();

        virtual uint32_t GetSize (void) const;

        virtual uint16_t GetFbField(void) const;

	virtual std::string GetName(void) const;

        virtual void Print (std::ostream &os) const;

	virtual bool Mark (void);
        virtual void AddHeader(void);

      private:
        //default const
        NdnQueueDiscItem ();
        //copy const
        NdnQueueDiscItem (const NdnQueueDiscItem &);

        /**
         *    * \brief Assignment operator
         *      *
         *      * Defined and unimplemented to avoid misuse
         *      * \returns
         *    */
        NdnQueueDiscItem &operator = (const NdnQueueDiscItem &);
        BlockHeader m_header; //NDN block header
        uint16_t m_fb_field = 0; //forwarding behaviour field
	std::string m_name= ""; //NDN name

    };
  }
}
#endif
