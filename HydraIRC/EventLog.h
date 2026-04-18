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

class CEventLogView;

// see g_EventLogTypeStrings in EventLogFrm.cpp
enum EventLogTypes
{
  ELIT_UNKNOWN,
  ELIT_SERVER,
  ELIT_DCC,
  ELIT_CHANNEL,
  ELIT_USER,

  ELIT_LAST
};

// see g_EventLogIDStrings in EventLogFrm.cpp
enum EventLogIDs
{
  ELID_FIRST = 0,
  ELID_Unknown = ELID_FIRST,
  ELID_Connect,
  ELID_Disconnect,
  ELID_LoggedIn,
  ELID_Kicked,
  ELID_Notice,
  ELID_PrivMsg,
  ELID_CTCP_Sound,
  ELID_CTCP_Version,
  ELID_CTCP_Ping,
  ELID_CTCP_Time,
  ELID_DCC_Receive,
  ELID_DCC_Send,
  ELID_DCC_ReceiveComplete,
  ELID_DCC_SendComplete,
  ELID_DCC_ReceiveFailed,
  ELID_DCC_SendFailed,
  ELID_Highlight,
  ELID_Wallops,
  ELID_Notification,
  ELID_Quit,
  ELID_Part,
  ELID_Join,

  ELID_LAST
};

class EventLogItem
{
public:
  int m_Type; // see ELIT_* defines.
  int m_ID;
  char *m_From;
  char *m_Message;
  char *m_ServerName;
  char *m_ChannelName;
  int m_NetworkID; // the network ID of the server at the time the message
                   // was received, used later in case a user does a /server
                   // and connects to a server on a different/unknown network.
  IRCServer *m_pServer; // the validity of this should be checked before using.
                        // as text queue items can stay around longer than server
                        // instances.
  IRCChannel *m_pChannel; // same goes for this.
  
  time_t m_Time;
  
  EventLogItem( const int Type, const int ID, const char *From, const char *Message, IRCServer *pServer, IRCChannel *pChannel);
  ~EventLogItem( void );
};


#define EVENTLOG_MESSAGE_BUFFER_SIZE 4096

class EventLogManager
{
public:
  CSimpleArray<EventLogItem *> m_EventLog;
  CEventLogView *m_pEventLogView;
  char m_MessageBuffer[EVENTLOG_MESSAGE_BUFFER_SIZE];

  EventLogManager( void );
  ~EventLogManager( void );

  void AddItem( const int Type, const int ID, const char *From, const char *Message, IRCServer *pServer, IRCChannel *pChannel);
  void RemoveOldItems( void );
  void BuildMessage(const char *format, ...);

private:
  void RemoveItem( EventLogItem *pELI );
};

