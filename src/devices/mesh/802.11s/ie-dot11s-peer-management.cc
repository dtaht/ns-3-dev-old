/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors: Kirill Andreev <andreev@iitp.ru>
 *          Aleksey Kovalenko <kovalenko@iitp.ru>
 */


#include "ns3/ie-dot11s-peer-management.h"
#include "ns3/assert.h"


//NS_LOG_COMPONENT_DEFINE ("MeshPeerLinkManagementelement");

namespace ns3 {

IeDot11sPeerManagement::IeDot11sPeerManagement ():
    m_length (0),
    m_subtype (PEER_OPEN),
    m_localLinkId (0),
    m_peerLinkId (0),
    m_reasonCode (REASON11S_RESERVED)
{}


void
IeDot11sPeerManagement::SetPeerOpen (uint16_t localLinkId)
{
  m_length = 3;
  m_subtype = PEER_OPEN;
  m_localLinkId = localLinkId;
}
void
IeDot11sPeerManagement::SetPeerClose (uint16_t localLinkId, uint16_t peerLinkId, dot11sReasonCode reasonCode)
{
  m_length = 7;
  m_subtype = PEER_CLOSE;
  m_localLinkId = localLinkId;
  m_peerLinkId = peerLinkId;
  m_reasonCode = reasonCode;
}

void
IeDot11sPeerManagement::SetPeerConfirm (uint16_t localLinkId, uint16_t peerLinkId)
{
  m_length = 5;
  m_subtype = PEER_CONFIRM;
  m_localLinkId = localLinkId;
  m_peerLinkId = peerLinkId;
}

dot11sReasonCode
IeDot11sPeerManagement::GetReasonCode () const
{
  return m_reasonCode;
}

uint16_t
IeDot11sPeerManagement::GetLocalLinkId () const
{
  return m_localLinkId;
}

uint16_t
IeDot11sPeerManagement::GetPeerLinkId () const
{
  return m_peerLinkId;
}

uint8_t
IeDot11sPeerManagement::GetInformationSize (void) const
{
  return m_length;
}

bool
IeDot11sPeerManagement::SubtypeIsOpen () const
{
  return (m_subtype == PEER_OPEN);
}
bool
IeDot11sPeerManagement::SubtypeIsClose () const
{
  return (m_subtype == PEER_CLOSE);
}
bool
IeDot11sPeerManagement::SubtypeIsConfirm () const
{
  return (m_subtype == PEER_CONFIRM);
}

void
IeDot11sPeerManagement::SerializeInformation (Buffer::Iterator i) const
{
  i.WriteU8 (m_subtype);
  i.WriteHtonU16 (m_localLinkId);
  if (m_length > 3)
    i.WriteHtonU16 (m_peerLinkId);
  if (m_length > 5)
    i.WriteHtonU16 (m_reasonCode);
}
uint8_t
IeDot11sPeerManagement::DeserializeInformation (Buffer::Iterator start, uint8_t length)
{
  Buffer::Iterator i = start;
  m_subtype  = i.ReadU8 ();
  m_length = length;
  if (m_subtype == PEER_OPEN)
    NS_ASSERT (length == 3);
  if (m_subtype == PEER_CONFIRM)
    NS_ASSERT (length == 5);
  if (m_subtype == PEER_CLOSE)
    NS_ASSERT (length == 7);
  m_localLinkId  = i.ReadNtohU16 ();
  if (m_length > 3)
    m_peerLinkId = i.ReadNtohU16 ();
  if (m_length > 5)
    m_reasonCode = (dot11sReasonCode)i.ReadNtohU16 ();
  return i.GetDistanceFrom (start);
}
void
IeDot11sPeerManagement::PrintInformation (std::ostream& os) const
{
  //TODO
}
} //namespace NS3