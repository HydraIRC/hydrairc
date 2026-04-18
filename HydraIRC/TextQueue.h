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

// Transfers.h : interface of the TextQueueManager class
//
/////////////////////////////////////////////////////////////////////////////

class TextQueueItem
{
public:
  char *m_From;
  char *m_Message;
  char *m_ServerName;
  int m_NetworkID; // the network ID of the server at the time the message
                   // was received, used later in case a uyser does a /server
                   // and connects to a server on a different/unknown network.
  IRCServer *m_pServer; // the validity of this should be checked before using.
                        // as text queue items can stay around longer than server
                        // instances.
  time_t m_Time;
  
  TextQueueItem( void )
  {
  }

  TextQueueItem( const char *From, char *Message, IRCServer *pServer);
  void Initialise( const char *From, char *Message, IRCServer *pServer);
  ~TextQueueItem( void );
};


class TextQueueManager //-DOCK : public IRCCommon
{
public:
  CSimpleArray<TextQueueItem *> m_TextQueue;
  int m_QueueType;
  CTextQueueView *m_pTextQueueView;

  TextQueueManager( void );
  ~TextQueueManager( void );

  void AddItem(const char *From, char *Message, IRCServer *pServer);
  void RemoveOldItems( void );

private:
  void RemoveItem( TextQueueItem *pTQI );
};

class PrivMsgQueueManager : 
  public TextQueueManager
{
public:
  PrivMsgQueueManager( void )
  {
    m_QueueType = CWQUEUE_PRIVMSG;
    m_pTextQueueView = new CTextQueueView(this, IDI_QUERY, &g_EventManager);
  }

  ~PrivMsgQueueManager( void )
  {
    delete m_pTextQueueView;
  }
};

class NoticeQueueManager : 
  public TextQueueManager
{
public:
  NoticeQueueManager( void )
  {
    m_QueueType = CWQUEUE_NOTICE;
    m_pTextQueueView = new CTextQueueView(this, IDI_NOTICE, &g_EventManager);
  }

  ~NoticeQueueManager( void )
  {
    delete m_pTextQueueView;
  }
};

enum URLCatcherQueueItemFlags 
{
  UCQIF_NONE       = 0,
  UCQIF_VISITED    = (1<<0)
};

class URLCatcherQueueItem :
  public TextQueueItem
{
  typedef TextQueueItem baseClass;
public:
  int m_Seen; // how many times we've seen this url.
  int m_Flags; // see URLCatcherQueueItemFlags

  URLCatcherQueueItem( const char *From, char *Message, IRCServer *pServer)
    : m_Seen(1), m_Flags(UCQIF_NONE)
  {
    baseClass::Initialise(From, Message, pServer);
  }
};

class UrlCatcherManager :
  public TextQueueManager
{
  typedef TextQueueManager baseClass;

public:
  UrlCatcherManager( void )
  {
    m_QueueType = CWQUEUE_URLCATCHER;
    m_pTextQueueView = new CTextQueueView(this, IDI_NOTICE, &g_EventManager); // TODO: needs icon
  }

  ~UrlCatcherManager( void )
  {
    delete m_pTextQueueView;
  }

  // Overrides
  void AddItem(const char *From, char *Message, IRCServer *pServer);
};
