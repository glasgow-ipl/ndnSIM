// -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*-
//
// Copyright (c) 2008 University of Washington
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//

#ifndef IPV4_GLOBAL_ROUTING_UNORDERED_NEXTHOPS_H
#define IPV4_GLOBAL_ROUTING_UNORDERED_NEXTHOPS_H

#include "ns3/ipv4-global-routing.h"
#include "ns3/trie.h"
#include "ns3/ipv4-routing-table-entry.h"
#include "ns3/simple-ref-count.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>

namespace ns3 {

/**
 * \brief Global routing protocol for IP version 4 stacks.
 *
 * Each prefix entry stores a list of ordered by metric next-hops
 *
 * This class deals with Ipv4 unicast routes only.
 *
 * \see Ipv4GlobalRouting
 * \see Ipv4RoutingProtocol
 * \see GlobalRouteManager
 */
class Ipv4GlobalRoutingUnorderedNexthops : public Ipv4GlobalRouting
{
private:
  class EntryContainer
    : public SimpleRefCount<EntryContainer>
    , public
      std::vector<Ipv4RoutingTableEntry>
  {
  };
  
public:
  static TypeId GetTypeId (void);

  Ipv4GlobalRoutingUnorderedNexthops ();
  
  // These methods inherited from base class
  // from Ipv4RoutingProtocol
  virtual Ptr<Ipv4Route> RouteOutput (Ptr<Packet> p, const Ipv4Header &header,
                                      Ptr<NetDevice> oif, Socket::SocketErrno &sockerr);

  virtual bool RouteInput  (Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev,
                            UnicastForwardCallback ucb, MulticastForwardCallback mcb,
                            LocalDeliverCallback lcb, ErrorCallback ecb);

  virtual void PrintRoutingTable (Ptr<OutputStreamWrapper> stream) const;

  // from Ipv4GlobalRouting  
  virtual void AddRouteTo (Ipv4Address dest, 
                           Ipv4Mask destMask, 
                           Ipv4Address nextHop, 
                           uint32_t interface,
                           uint32_t metric=0);

  virtual void DeleteRoutes ();

  virtual void FixRoutes ();

protected:
  virtual Ptr<Ipv4Route> LookupGlobal (uint32_t entryNum, Ipv4Address dest, Ptr<NetDevice> oif = 0);
  
private:
  typedef Ipv4AddressTrie<Ptr<EntryContainer> > Ipv4AddressTrieMap;
  Ipv4AddressTrieMap m_routes;

  uint32_t m_numLogicalEntries;
};

} // Namespace ns3

#endif /* IPV4_GLOBAL_ROUTING_UNORDERED_NEXTHOPS_H */
