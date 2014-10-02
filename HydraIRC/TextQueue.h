/*

  HydraIRC
  Copyright (C) 2002-2006 Dominic Clifton aka Hydra

  HydraIRC limited-use source license

  1) You can:
  1.1) Use the source to create improvements and bug-fixes to send to the
       author to be incorporated in the main program.
  1.2) Use it for review/educational purposes.

  2) You can NOT:
  2.1) Use the source to create derivative works. (That is, you can't release
       your own version of HydraIRC with your changes in it)
  2.2) Compile your own version and sell it.
  2.3) Distribute unmodified, modified source or compiled versions of HydraIRC
       without first obtaining permission from the author. (I want one place
       for people to come to get HydraIRC from)
  2.4) Use any of the code or other part of HydraIRC in anything other than 
       HydraIRC.
       
  3) All code submitted to the project:
  3.1) Must not be covered by any license that conflicts with this license 
       (e.g. GPL code)
  3.2) Will become the property of the author.

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
