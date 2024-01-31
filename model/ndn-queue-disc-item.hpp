#ifndef NDN_QUEUE_DISC_ITEM_H
#define NDN_QUEUE_DISC_ITEM_H

#include "ns3/object.h"
#include "ns3/queue-item.h"

namespace ns3{
  namespace ndn{
    class NdnQueueDiscItem : public QueueDiscItem {
      public:
        NdnQueueDiscItem (Ptr<Packet> p, uint16_t protocol, uint16_t fb_field);

        virtual ~ NdnQueueDiscItem ();

        virtual uint32_t GetSize (void) const;

        virtual uint16_t GetFbField(void) const;

        virtual void Print (std::ostream &os) const;

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

        uint16_t m_fb_field = 0; //forwarding behaviour field

    };
  }
}
#endif
