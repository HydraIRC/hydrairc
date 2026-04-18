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

// Transfers.cpp : implementation of the TextQueueManager class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

void UrlCatcherManager::AddItem(const char *From, char *Message, IRCServer *pServer)
{
  // don't add duplicates, increase the counter.

  
  TextQueueItem *pTQI;
  BOOL Found = FALSE;
  int i;
  for (i = 0; i < m_TextQueue.GetSize(); i++)
  {
    pTQI = m_TextQueue[i];
    if (stricmp(Message, pTQI->m_Message) == 0)
    {
      Found = TRUE;
      break;
    }
  }

  if (Found)
  {
    // cast to url grabber type
    URLCatcherQueueItem *pUCQI = (URLCatcherQueueItem *)pTQI;

    // increase seen counter and set time/date to current time
    // (so that when we remove old items we don't remove this item
    // until we've not seen it for a while...)
    pUCQI->m_Seen++;
    time(&pUCQI->m_Time);
    m_pTextQueueView->UpdateItem((TextQueueItem *)pUCQI);
  }
  else
  {
    URLCatcherQueueItem *pUCQI = new URLCatcherQueueItem(From,Message,pServer);
    if (pUCQI)
    {
      m_TextQueue.Add((TextQueueItem *)pUCQI);
      m_pTextQueueView->AddItem((TextQueueItem *)pUCQI);
    }
  }
  RemoveOldItems();
}

void TextQueueManager::AddItem(const char *From, char *Message, IRCServer *pServer)
{
  TextQueueItem *pTQI = new TextQueueItem(From,Message,pServer);
  if (pTQI)
  {
    m_TextQueue.Add(pTQI);
    // update the window if open
    /* -DOCK
    if (m_pChildWnd)
    {
      (reinterpret_cast<CTextQueueFrame *>(m_pChildWnd))->AddItem(pTQI);
    }
    */
    m_pTextQueueView->AddItem(pTQI);
  }
  RemoveOldItems();
}

void TextQueueManager::RemoveItem( TextQueueItem *pTQI )
{
  if (!pTQI)
    return;

  // update the window if open
  /* -DOCK
  if (m_pChildWnd)
  {
    (reinterpret_cast<CTextQueueFrame *>(m_pChildWnd))->RemoveItem(pTQI);
  }
  */
  m_pTextQueueView->RemoveItem(pTQI);

  m_TextQueue.Remove(pTQI);
  delete pTQI;
}

void TextQueueManager::RemoveOldItems( void )
{
  BOOL RemoveOldItems = FALSE;
  int MaxAgeMins;
  int MaxItems;
  switch(m_QueueType)
  {
    case CWQUEUE_PRIVMSG:
      RemoveOldItems = BOOLPREF(PREF_bRemoveOldPrivmsgLogItems);
      MaxAgeMins = INTPREF(PREF_nMaxPrivMsgLogAgeMins);
      MaxItems = INTPREF(PREF_nMaxPrivMsgLogItems);
      break;
    case CWQUEUE_NOTICE:
      RemoveOldItems = BOOLPREF(PREF_bRemoveOldNoticeLogItems);
      MaxAgeMins = INTPREF(PREF_nMaxNoticeLogAgeMins);
      MaxItems = INTPREF(PREF_nMaxNoticeLogItems);
      break;      
    case CWQUEUE_URLCATCHER:
      RemoveOldItems = BOOLPREF(PREF_bRemoveOldURLCatcherItems);
      MaxAgeMins = INTPREF(PREF_nMaxURLCatcherAgeMins);
      MaxItems = INTPREF(PREF_nMaxURLCatcherItems);
      break;
  }

  if (!RemoveOldItems)
    return;

  // FIFO
  while (m_TextQueue.GetSize() > MaxItems)
  {
    RemoveItem(m_TextQueue[0]);
  }

  time_t now;
  time(&now);
  TextQueueItem *pTQI;

  for (int i = 0 ; i < m_TextQueue.GetSize(); i ++)
  {
    pTQI = m_TextQueue[i];

    if ((difftime(now,pTQI->m_Time) / 60) >= MaxAgeMins)
    {
      RemoveItem(pTQI);
    }
  }
}

TextQueueManager::TextQueueManager( void )
{
}

TextQueueManager::~TextQueueManager( void )
{
  while (m_TextQueue.GetSize() > 0)
  {
    delete m_TextQueue[0];
    m_TextQueue.RemoveAt(0);
  }
}

TextQueueItem::TextQueueItem( const char *From, char *Message, IRCServer *pServer)
{
  Initialise(From, Message, pServer);
}

void TextQueueItem::Initialise( const char *From, char *Message, IRCServer *pServer)
{
  m_From = strdup(From);
  m_Message = stripcodes(strdup(Message));

  char *networkname = GetNetworkName(pServer->m_pDetails->m_NetworkID);
  char *serverstr = HydraIRC_BuildString(strlen(pServer->m_pDetails->m_Name) + strlen(networkname),
    "%s (%s)",
    pServer->m_pDetails->m_Name,
    networkname);

  m_ServerName = serverstr;
  m_NetworkID = pServer->m_pDetails->m_NetworkID;
  m_pServer = pServer;
  time(&m_Time);
}

TextQueueItem::~TextQueueItem( void )
{
  if (m_From) free (m_From);
  if (m_Message) free (m_Message);
  if (m_ServerName) free(m_ServerName);
}
