/*

  HydraIRC
  Copyright (C) 2002-2026 Dominic Clifton aka Hydra

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

void EventLogManager::AddItem( const int Type, const int ID, const char *From, const char *Message, IRCServer *pServer, IRCChannel *pChannel)
{
  EventLogItem *pELI = new EventLogItem(Type, ID, From, Message ? Message : m_MessageBuffer, pServer, pChannel);
  if (pELI)
  {
    m_EventLog.Add(pELI);
    m_pEventLogView->AddItem(pELI,TRUE);
  }
  RemoveOldItems();
}

void EventLogManager::RemoveItem( EventLogItem *pELI )
{
  if (!pELI)
    return;

  m_pEventLogView->RemoveItem(pELI);

  m_EventLog.Remove(pELI);
  delete pELI;
}

void EventLogManager::RemoveOldItems( void )
{
  BOOL RemoveOldItems = FALSE;
  int MaxAgeMins;
  int MaxItems;

  RemoveOldItems = BOOLPREF(PREF_bRemoveOldEventLogItems);
  MaxAgeMins = INTPREF(PREF_nMaxEventLogAgeMins);
  MaxItems = INTPREF(PREF_nMaxEventLogItems);

  if (!RemoveOldItems)
    return;

  // FIFO
  while (m_EventLog.GetSize() > MaxItems)
  {
    RemoveItem(m_EventLog[0]);
  }

  time_t now;
  time(&now);
  EventLogItem *pELI;

  for (int i = 0 ; i < m_EventLog.GetSize(); i ++)
  {
    pELI = m_EventLog[i];

    if ((difftime(now,pELI->m_Time) / 60) >= MaxAgeMins)
    {
      RemoveItem(pELI);
    }
  }
}

EventLogManager::EventLogManager( void )
{
  m_pEventLogView = new CEventLogView(this, IDI_EVENTLOG, &g_EventManager);  
}

EventLogManager::~EventLogManager( void )
{
  while (m_EventLog.GetSize() > 0)
  {
    delete m_EventLog[0];
    m_EventLog.RemoveAt(0);
  }
  
  delete m_pEventLogView;  
}

void EventLogManager::BuildMessage(const char *format, ...)
{
  va_list args;

  va_start(args, format);
  _vsnprintf(m_MessageBuffer, EVENTLOG_MESSAGE_BUFFER_SIZE, format, args);
  va_end(args);
  m_MessageBuffer[EVENTLOG_MESSAGE_BUFFER_SIZE-1] = 0;
}


EventLogItem::EventLogItem( const int Type, const int ID, const char *From, const char *Message, IRCServer *pServer, IRCChannel *pChannel)
{
  m_Type = Type;
  m_ID = ID;
  m_From = From ? strdup(From) : NULL;
  m_Message = Message ? stripcodes(strdup(Message)) : NULL;
  char *networkname = NULL;
  char *serverstr = NULL;

  if (pServer && pServer->m_pDetails)
  {
    networkname = GetNetworkName(pServer->m_pDetails->m_NetworkID);
    serverstr = HydraIRC_BuildString(strlen(pServer->m_pDetails->m_Name) + strlen(networkname),
      "%s (%s)",
      pServer->m_pDetails->m_Name,
      networkname);

    m_NetworkID = pServer->m_pDetails->m_NetworkID;
  }
  else
  {
    m_NetworkID = -1;
  }

  m_pServer = pServer;
  m_ServerName = serverstr;

  m_ChannelName = pChannel ? strdup(pChannel->m_pProperties->m_Name) : NULL;
  m_pChannel = pChannel;
  time(&m_Time);
}

EventLogItem::~EventLogItem( void )
{
  if (m_From) free (m_From);
  if (m_Message) free (m_Message);
  if (m_ServerName) free(m_ServerName);
  if (m_ChannelName) free(m_ChannelName);
}
