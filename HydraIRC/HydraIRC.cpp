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

// HydraIRC.cpp : main source file for HydraIRC.exe
//
/////////////////////////////////////////////////////////////////////////////

// enable memory leak detection.
#include "stdafx.h"
#include "HydraIRC.h"

#include "dllversion/dllversion.h"

#include<DockImpl.cpp>

/*
namespace dockwins{
CDockingBoxMessage CDockingBox::m_message;
}//namespace dockwins
*/

CSimpleArray<IRCServer*> g_ServerList;
CSimpleArray<IRCChannel*> g_ChannelList;
CSimpleArray<IRCQuery*> g_QueryList;
CSimpleArray<IRCServerDetails*> g_ServerDetailsList;
CSimpleArray<IRCNetworkDetails*> g_NetworkList;
CSimpleArray<CChildCommon *> g_ChildWndList;
CSimpleArray<BufferItem *> g_OutputLogBuffer;
CImageList g_UserListImages;
NotificationManager *g_pNotificationManager = NULL;
TransfersManager *g_pTransfersManager = NULL;
PrivMsgQueueManager *g_pPrivMsgQueueManager = NULL;
NoticeQueueManager *g_pNoticeQueueManager = NULL;
UrlCatcherManager *g_pUrlCatcherManager = NULL;
EventLogManager *g_pEventLogManager = NULL;
CPluginManager *g_pPluginManager = NULL;
CEventManager g_EventManager;
IDManager g_IDManager;
ThemeManager g_ThemeManager;

BOOL g_StorePrintfs = TRUE;
Identd *g_pIdentd = NULL;
CDNSResolver *g_pDNSResolver = NULL;

CAppModule _Module;
CMainFrame *g_pMainWnd = NULL;
Prefs *g_pPrefs = NULL;
Prefs *g_pNewPrefs = NULL;

char g_DebugStr[2048];
#ifdef DEBUG
// modify this line to show more/less debug output, can also be changed at run-time via the debugger.
DWORD g_DebugFlags = DBGFLG_NONE; // see DBGFLG_* defines
// DWORD g_DebugFlags = DBGFLG_EVENTDUMP1 | DBGFLG_TIMER | DBGFLG_FLOODCONTROL;
#endif

HCURSOR g_vSizeCursor;
ActiveSettings *g_pActiveSettings = NULL;
int g_PrefsEventSourceID = -1; // -1 = not set yet..
char *g_HomeDir; // does not have a trailing \ character
char *g_AppDir; // does not have a trailing \ character
char g_StartDir[MAX_PATH]; // does not have a trailing \ character
char g_AppName[MAX_PATH]; // includes path
char *g_CommandLine = NULL;
char g_FQDN[1024] = {0};
char g_EventLogMessageBuffer[4096] = {0};

char *g_VersionStr = VERSIONSTRING;
char *g_AuthorStr = AUTHORSTRING;
char g_ConsoleLogFile[1024] = "HydraIRC_Log.txt"; // so that class inits can still log..
char g_TimeStampBuffer[MAX_TIMESTAMP_LEN]; // global, for speed.  TODO: time it somehow. (cpu cache hits?)

extern StringPrefItem_t g_MainPrefs_Strings[];

IRCServerDetails *g_DefaultServerDetails;

IntPair_t g_FontIDLookupTable[PREF_FONT_MAX] = {
  {PREF_fServerInputFont,-1},
  {PREF_fServerOutputFont,-1},
  {PREF_fChannelInputFont,-1},
  {PREF_fChannelOutputFont,-1},
  {PREF_fOutputWindowFont,-1},
  {PREF_fChannelMonitorFont,-1},
  {PREF_fServerMonitorFont,-1},
  {PREF_fDCCChatInputFont,-1},
  {PREF_fDCCChatOutputFont,-1},
  {PREF_fQueryInputFont,-1},
  {PREF_fQueryOutputFont,-1},
  {PREF_fServerListFont,-1},
  {PREF_fServerListDirtyFont,-1},
  {PREF_fServerListRecentFont,-1},
  {PREF_fUserListFont,-1},
  {PREF_fHeaderFont,-1},
  {PREF_fInfoFont,-1}
};

IntPair_t g_IconIDLookupTable[] = {
  {ICONID_SERVER_CONNECTED,-1},
  //{ICONID_SERVER_DISCONNECTED,-1},
  {ICONID_TRANSFERS,-1},
  {ICONID_USERS,-1},
  {ICONID_NOTE,-1},
  {ICONID_WARNING,-1},
  {ICONID_INFORMATION,-1},
  {ICONID_USERNORMAL,-1},
  {ICONID_USERVOICE,-1},
  {ICONID_USERHALFOP,-1},
  {ICONID_USEROP,-1}
  //{ICONID_CHANNEL,-1},
  //{ICONID_QUERY,-1},
};

#define FONTIDTABLECOUNT (sizeof(g_FontIDLookupTable) / sizeof(IntPair_t))
#define ICONIDTABLECOUNT (sizeof(g_IconIDLookupTable) / sizeof(IntPair_t))

ObjectManager g_Fonts; // manage a load of fonts, some of which are custom fonts for channels
                       // some of which directly relate to the standard application fonts
                       // see g_FontIDLookupTable

ObjectManager g_Icons; // manage HydraIRC's icons.


const char *g_VariableIDNames[VID_LAST] = // must have matching ID's in VariableIDs
{
  "RESPONSE",
  "MYNICK",
  "MYHOST",
  "SERVER",
  "SERVERDESCRIPTION",
  "SERVEROUTPUT",
  "CHANNELOUTPUT",
  "QUERYOUTPUT",
  "DCCCHATOUTPUT",
  "ALL",
  "NICK",
  "UNPREFIXEDNICK",
  "USERNAME",
  "HOST",
  "NAME",
  "REMOTESERVER",
  "REMOTESERVERDESCRIPTION",
  "IDLESECS",
  "SIGNONTIME",
  "SERVERNOTICE",
  "CHANNEL",
  "CHANNELMODE",
  "OTHERNICK",
  "CTCP",
  "PARAM1",
  "PARAM2",
  "PARAM3",
  "PARAM4",
  "PARAM5"
};

// THEMES!
/*
  Note, EXACT theme matches must preceed INEXACT matches

  e.g

  {THEME_CHANNEL,RPL_CHANNELMODEIS,"### channel <CHANNEL> mode is <CHANNELMODE>"},

  must preceed

  {THEME_CHANNEL | THEME_SERVER,RPL_CHANNELMODEIS,"*** channel <CHANNEL> mode is <CHANNELMODE>"},

*/

OutputThemeItem_t g_OutputTheme1[] = {
  {THEME_SERVER,RPLS_NOTICE_CONNECT,"-<SERVER>- <ALL>"},
  {THEME_SERVER,RPLS_NOTICE_SERVER,"<SERVERNOTICE> <ALL>"},
  {THEME_SERVER,RPLS_PING,"PING <SERVER> <ALL>"},
  {THEME_SERVER,RPLS_PONG,"PONG <SERVER> <ALL>"},
  {THEME_SERVER,RPLS_PING_SENT,"-> PING <SERVER> <ALL>"},
  {THEME_SERVER,RPLS_PONG_SENT,"-> PONG <SERVER> <ALL>"},
  {THEME_ALL,RPLS_NOTICE,"-<NICK>- <ALL>"},
  {THEME_ALL,RPLS_WALLOPS,"*<PARAM1>* <PARAM2>"},
  {THEME_SERVER,RPLS_NOTICE_CTCP_RECEIVED,"CTCP/<CTCP> reply from <NICK> : <ALL>"},
  {THEME_SERVER,RPLS_NOTICE_CTCP_SENT,"-> [<NICK>] <ALL>"},
  {THEME_SERVER,RPLS_NOTICE_SENT,"-> [<NICK>] <ALL>"},
  {THEME_SERVER,RPLS_PRIVMSG_CTCP_RECEIVED,"CTCP/<CTCP> request from <NICK> : <ALL>"},
  {THEME_SERVER,RPLS_PRIVMSG_CTCP_SENT,"-> [<NICK>] <ALL>"},
  {THEME_SERVER,RPLS_NICK,"*** Nick changed to <NICK>"},
  {THEME_ALL,RPLS_REQUESTNICK,"*** Attempting to use nick \"<NICK>\""},
  {THEME_SERVER,RPLS_MODE,"*** <NICK> sets mode <ALL>"},  
  {THEME_CHANNEL,RPLS_MODE,"*** <NICK> sets channel <CHANNEL> mode <ALL>"},
  {THEME_CHANNEL,RPL_NOTOPIC,"*** topic is <ALL>"},
  {THEME_CHANNEL,RPL_TOPIC,"*** topic is <ALL>"},
  {THEME_CHANNEL,RPL_TOPICEND,"*** set by <NICK> on <PARAM3>"},
  {THEME_CHANNEL,RPL_CREATIONTIME,"*** channel created at <PARAM2>"},
  {THEME_CHANNEL,RPLS_TOPIC,"*** <NICK> changed topic to <ALL>"},
  {THEME_CHANNEL,RPLS_JOIN_WE,"*** now talking in <CHANNEL>"},
  {THEME_CHANNEL,RPLS_JOIN,"*** <NICK> (<HOST>) joined"},
  {THEME_CHANNEL,RPLS_PART,"*** <NICK> (<HOST>) left (<ALL>)"},
  {THEME_CHANNEL | THEME_QUERY,RPLS_QUIT,"*** <NICK> quit (<ALL>)"},
  {THEME_CHANNEL,RPLS_KICK_WE,"*** you were kicked from <CHANNEL> by <NICK> (<ALL>)"},
  {THEME_CHANNEL,RPLS_KICK,"*** <OTHERNICK> was kicked from <CHANNEL> by <NICK> (<ALL>)"},
  {THEME_ALL,RPLS_NICK,"*** <OTHERNICK> changed nick to <NICK>"},
  {THEME_ALL,RPLS_PRIVMSG_ACTION,"* <NICK> <ALL>"},
  {THEME_ALL,RPLS_PRIVMSG_CHANNELTEXT,"<<NICK>> <ALL>"},
  {THEME_ALL,RPLS_PRIVMSG_RECEIVED,"*<NICK>* <ALL>"},
  {THEME_ALL,RPLS_PRIVMSG_SENT,"-> *<NICK>* <ALL>"},
  {THEME_ALL,RPL_WHOISUSER,"<NICK> is <USERNAME>@<HOST> <NAME>"},
  {THEME_ALL,RPL_WHOISAWAY,"<NICK> is away, reason: <ALL>"},
  {THEME_ALL,RPL_WHOISHOST,"<NICK> is connecting from <HOST>"},  
  {THEME_ALL,RPL_WHOISSERVER,"<NICK> using <REMOTESERVER> <REMOTESERVERDESCRIPTION>"},
  {THEME_ALL,RPL_WHOISIDLE,"<NICK> idle <IDLESECS>, signed on <SIGNONTIME>"},
  {THEME_ALL,RPL_WHOISCHANNELS,"<NICK> on <ALL>"},
  {THEME_ALL,RPL_ENDOFWHOIS,"<NICK> End of /WHOIS list."},
  {THEME_SERVER,RPL_NAMREPLY,"<CHANNEL> <ALL>"},
  {THEME_SERVER,RPL_ENDOFNAMES,"<CHANNEL> End of /NAMES list."},
  {THEME_SERVER,RPL_LISTSTART,"Channel Name - Users - Topic"},
  {THEME_SERVER,RPL_LIST,"<CHANNEL> - Users: <PARAM2> Topic: <ALL>"},
  {THEME_SERVER,RPL_LISTEND,"<ALL>"},
  {THEME_SERVER,RPL_LINKS,"<PARAM1> <PARAM2> Hops: <PARAM3> Info: <PARAM4>"},
  {THEME_CHANNEL | THEME_SERVER,RPL_CHANNELMODEIS,"*** channel <CHANNEL> mode is <CHANNELMODE>"},
  {THEME_SERVER,ERR_UNAVAILRESOURCE,"<CHANNEL> - <ALL>"},
  {THEME_SERVER,ERR_TOOMANYCHANNELS,"<CHANNEL> - <ALL>"},
  {THEME_SERVER,ERR_NOSUCHCHANNEL,"<CHANNEL> - <ALL>"},
  {THEME_SERVER,ERR_CHANNELISFULL,"<CHANNEL> - <ALL>"},
  {THEME_SERVER,ERR_INVITEONLYCHAN,"<CHANNEL> - <ALL>"},
  {THEME_SERVER,ERR_BADCHANNELKEY,"<CHANNEL> - <ALL>"},
  {THEME_SERVER,ERR_BANNEDFROMCHAN,"<CHANNEL> - <ALL>"},
  {THEME_SERVER,ERR_CHANOPRIVSNEEDED,"<CHANNEL> - <ALL>"},
  {THEME_SERVER,ERR_NOSUCHNICK,"<NICK> no such nick/channel"},
  {THEME_SERVER,ERR_ERRONEUSNICKNAME,"<NICK> erroneous nickname"},
  {THEME_SERVER,ERR_NICKNAMEINUSE,"<NICK> - <ALL>"},
  {THEME_ALL,APP_CONNECT,"*** <ALL>"},
  {THEME_ALL,APP_ERROR,"*** <ALL>"},
  {THEME_END,THEME_END,NULL} // end of the list!
};

OutputThemeItem_t g_OutputTheme2[] = {
  {THEME_SERVER,RPLS_NOTICE_CONNECT,"0,2 NOTICE  <SERVER> - <ALL>"},
  {THEME_SERVER,RPLS_NOTICE_SERVER,"0,2 NOTICE  <SERVERNOTICE> - <ALL>"},
  {THEME_SERVER,RPLS_PING,"0,2 PING  <SERVER> <ALL>"},
  {THEME_SERVER,RPLS_PONG,"0,2 PONG  <SERVER> <ALL>"},
  {THEME_SERVER,RPLS_PING_SENT,"-> 0,2 PING  <SERVER> <ALL>"},
  {THEME_SERVER,RPLS_PONG_SENT,"-> 0,2 PONG  <SERVER> <ALL>"},
  {THEME_ALL,RPLS_NOTICE,"0,5 NOTICE  -<NICK>- <ALL>"},
  {THEME_ALL,RPLS_WALLOPS,"0,5 WALLOPS  *<PARAM1>* <PARAM2>"},
  {THEME_SERVER,RPLS_NOTICE_CTCP_RECEIVED,"0,6 CTCP  CTCP/<CTCP> reply from <NICK> : <ALL>"},
  {THEME_SERVER,RPLS_NOTICE_CTCP_SENT,"-> 0,6 CTCP   [<NICK>] <ALL>"},
  {THEME_SERVER,RPLS_NOTICE_SENT,"-> 0,2 NOTICE   [<NICK>] <ALL>"},
  {THEME_SERVER,RPLS_PRIVMSG_CTCP_RECEIVED,"0,6 CTCP  CTCP/<CTCP> request from <NICK> : <ALL>"},
  {THEME_SERVER,RPLS_PRIVMSG_CTCP_SENT,"-> 0,6 CTCP   [<NICK>] <ALL>"},
  {THEME_SERVER,RPLS_NICK,"0,6 NICK  Nick changed to <NICK>"},
  {THEME_ALL,RPLS_REQUESTNICK,"0,6 NICK  Attempting to use nick \"<NICK>\""},
  {THEME_SERVER,RPLS_MODE,"0,3 MODE  <NICK> sets mode <ALL>"},
  {THEME_CHANNEL,RPLS_MODE,"0,3 MODE  <NICK> sets channel <CHANNEL> mode <ALL>"},
  {THEME_CHANNEL,RPL_NOTOPIC,"0,7 <CHANNEL>  topic is <ALL>"},
  {THEME_CHANNEL,RPL_TOPIC,"0,7 <CHANNEL>  topic is <ALL>"},
  {THEME_CHANNEL,RPL_TOPICEND,"0,7 <CHANNEL>  set by <NICK> on <PARAM3>"},
  {THEME_CHANNEL,RPL_CREATIONTIME,"0,7 <CHANNEL>  created at <PARAM2>"},
  {THEME_CHANNEL,RPLS_TOPIC,"0,7 <CHANNEL>  <NICK> changed topic to <ALL>"},
  {THEME_CHANNEL,RPLS_JOIN_WE,"0,3 JOIN  now talking in <CHANNEL>"},
  {THEME_CHANNEL,RPLS_JOIN,"0,3 JOIN  <NICK> (<HOST>) joined"},
  {THEME_CHANNEL,RPLS_PART,"0,2 PART  <NICK> (<HOST>) left (<ALL>)"},
  {THEME_CHANNEL | THEME_CHANNEL,RPLS_QUIT,"0,2 QUIT  <NICK> quit (<ALL>)"},
  {THEME_CHANNEL,RPLS_KICK_WE,"0,4 KICK  you were kicked from <CHANNEL> by <NICK> (<ALL>)"},
  {THEME_CHANNEL,RPLS_KICK,"0,4 KICK  <OTHERNICK> was kicked from <CHANNEL> by <NICK> (<ALL>)"},
  {THEME_ALL,RPLS_NICK,"0,6 NICK  <OTHERNICK> changed nick to <NICK>"},
  {THEME_ALL,RPLS_PRIVMSG_ACTION,"* <NICK> <ALL>"},
  {THEME_ALL,RPLS_PRIVMSG_CHANNELTEXT,"<<NICK>> <ALL>"},
  {THEME_ALL,RPLS_PRIVMSG_RECEIVED,"*<NICK>* <ALL>"},
  {THEME_ALL,RPLS_PRIVMSG_SENT,"-> *<NICK>* <ALL>"},
  {THEME_ALL,RPL_WHOISUSER,"0,10 WHOIS  <NICK> is <USERNAME>@<HOST> <NAME>"},
  {THEME_ALL,RPL_WHOISAWAY,"0,10 WHOIS  <NICK> is away, reason: <ALL>"},
  {THEME_ALL,RPL_WHOISHOST,"0,10 WHOIS  <NICK> is connecting from <HOST>"},  
  {THEME_ALL,RPL_WHOISSERVER,"0,10 WHOIS  <NICK> using <REMOTESERVER> <REMOTESERVERDESCRIPTION>"},
  {THEME_ALL,RPL_WHOISIDLE,"0,10 WHOIS  <NICK> idle <IDLESECS>, signed on <SIGNONTIME>"},
  {THEME_ALL,RPL_WHOISCHANNELS,"0,10 WHOIS  <NICK> on <ALL>"},
  {THEME_ALL,RPL_ENDOFWHOIS,"0,10 WHOIS  <NICK> End of /WHOIS list."},
  {THEME_SERVER,RPL_NAMREPLY,"0,7 <CHANNEL>  <ALL>"},
  {THEME_SERVER,RPL_ENDOFNAMES,"0,7 <CHANNEL>  End of /NAMES list."},
  {THEME_SERVER,RPL_LISTSTART,"0,2 LIST  Channel Name - Users - Topic"},
  {THEME_SERVER,RPL_LIST,"0,2 LIST  <CHANNEL> - Users: <PARAM2> Topic: <ALL>"},
  {THEME_SERVER,RPL_LISTEND,"0,2 LIST  <ALL>"},
  {THEME_SERVER,RPL_LINKS,"0,2 LINKS  <PARAM1> <PARAM2> Hops: <PARAM3> Info: <PARAM4>"},
  {THEME_CHANNEL | THEME_SERVER,RPL_CHANNELMODEIS,"0,3 MODE  <CHANNEL> mode is <CHANNELMODE>"},
  {THEME_SERVER,ERR_UNAVAILRESOURCE,"0,4 <CHANNEL>  <ALL>"},
  {THEME_SERVER,ERR_TOOMANYCHANNELS,"0,7 <CHANNEL>  <ALL>"},
  {THEME_SERVER,ERR_NOSUCHCHANNEL,"0,7 <CHANNEL>  <ALL>"},
  {THEME_SERVER,ERR_CHANNELISFULL,"0,7 <CHANNEL>  <ALL>"},
  {THEME_SERVER,ERR_INVITEONLYCHAN,"0,7 <CHANNEL>  <ALL>"},
  {THEME_SERVER,ERR_BADCHANNELKEY,"0,7 <CHANNEL>  <ALL>"},
  {THEME_SERVER,ERR_BANNEDFROMCHAN,"0,7 <CHANNEL>  <ALL>"},
  {THEME_SERVER,ERR_CHANOPRIVSNEEDED,"0,7 <CHANNEL>  <ALL>"},
  {THEME_SERVER,ERR_NOSUCHNICK,"0,4 ERROR  <NICK> no such nick/channel"},
  {THEME_SERVER,ERR_ERRONEUSNICKNAME,"0,4 ERROR  <NICK> erroneous nickname"},
  {THEME_SERVER,ERR_NICKNAMEINUSE,"0,4 ERROR  <NICK> - <ALL>"},
  {THEME_SERVER,RPL_WELCOME,"0,5 NOTICE  <ALL>"},
  {THEME_SERVER,RPL_YOURHOST,"0,5 NOTICE  <ALL>"},
  {THEME_SERVER,RPL_CREATED,"0,5 NOTICE  <ALL>"},
  {THEME_SERVER,RPL_MYINFO,"0,5 NOTICE  <ALL>"},
  {THEME_SERVER,RPL_STATSDLINE,"0,5 NOTICE  <ALL>"},
  {THEME_SERVER,RPL_LUSERCLIENT,"0,5 NOTICE  <ALL>"},
  {THEME_SERVER,RPL_LUSERME,"0,5 NOTICE  <ALL>"},
  {THEME_SERVER,RPL_MOTDSTART,"0,3 MOTD  <ALL>"},
  {THEME_SERVER,RPL_MOTD,"0,3 MOTD  <ALL>"},
  {THEME_SERVER,RPL_ENDOFMOTD,"0,5 NOTICE  <ALL>"},
  {THEME_ALL,APP_CONNECT,"0,2 CONNECT  <ALL>"},
  {THEME_ALL,APP_ERROR,"0,4 ERROR  <ALL>"},

  // defaults
  {THEME_SERVER,THEME_ANYCOMMAND,"0,14 NOTICE  <ALL>"},
  {THEME_CHANNEL,THEME_ANYCOMMAND,"0,14 CHANNEL  <ALL>"},
  {THEME_ALL,THEME_ANYCOMMAND,"0,14 DEFAULT  <ALL>"},

  {THEME_END,THEME_END,NULL} // end of the list!
};

OutputThemeItem_t *g_DefaultOutputTheme = &g_OutputTheme1[0];

char *g_DefaultStrings[DEFSTR_LAST] = 
{
  "Ready",                                                            // DEFSTR_DefaultStatusBarMessage
  "Cancelled",                                                        // DEFSTR_Cancelled
  "Unknown",                                                          // DEFSTR_Unknown
  "Connect Failed (%s)",                                              // DEFSTR_ConnectFailed
  "Connection Closed (%s)",                                           // DEFSTR_ConnectionClosed

  "Socket Error (%d: %s)",                                            // DEFSTR_Socket_Error
  "Socket Disconnected",                                              // DEFSTR_Socket_Disconnected
  "Connected",                                                        // DEFSTR_Socket_Connected
  "Reconnect Aborted",                                                // DEFSTR_Socket_ReconnectAborted

  "CTCP/%s request from %s",                                          // DEFSTR_Status_NewCTCPMsg
  "Server %s (%s) connected",                                         // DEFSTR_Status_ServerConnected
  "Server %s (%s) disconnected!",                                     // DEFSTR_Status_ServerDisconnected
  "Welcome to HydraIRC " VERSIONSTRING " - Remember to join #HydraIRC on EFNet and say Hi", // DEFSTR_Status_Welcome

  "Connecting to \"%s:%d\" (Attempt %d of %d)",                       // DEFSTR_Server_Connect
  "Resolving host name",                                              // DEFSTR_Server_Resolving
  "Could not resolve host name",                                      // DEFSTR_Server_ResolveError
  "Connecting to %s:%d",                                              // DEFSTR_Server_ConnectAddress
  "Unable to connect to IRC server",                                  // DEFSTR_Server_ConnectError
  "Using Identity \"%s\", Nick %s",                                   // DEFSTR_Server_IdentityAndNick
  "Disconnected (%s)",                                                // DEFSTR_Server_Disconnected
  "Server disconnected. Delaying retry for %d seconds (Click \"Disconnect\" to cancel)",   // DEFSTR_Server_DisconnectRetry
  "Connect attempt %d of %d failed, delaying retry for %d seconds (Click \"Disconnect\" to cancel)",   // DEFSTR_Server_ConnectRetry
  "Giving up after %d failed connection attempts",                    // DEFSTR_Server_ConnectGiveUp
  "Nick changed to %s",                                               // DEFSTR_Server_NickChanged
  "Logged in to server %s (%s)",                                      // DEFSTR_Server_LoggedIn
  "Wallops from %s: %s",                                              // DEFSTR_Server_Wallops
  "%s quit : %s",                                                     // DEFSTR_Server_Quit

  "<%s> %s",                                                           // DEFSTR_User_Notice
  "<%s> %s",                                                           // DEFSTR_User_PrivMsg

  "%s wanted to play the sound \"%s\" on your computer",              // DEFSTR_CTCP_Sound
  "%s wanted to know the version of your irc client",                 // DEFSTR_CTCP_Version
  "%s PING'd you",                                                    // DEFSTR_CTCP_Ping
  "%s wanted to know your local time",                                // DEFSTR_CTCP_Time

  "%s is sending you %s",                                             // DEFSTR_DCC_Receive
  "%s is trying to send you %s",                                      // DEFSTR_DCC_ReceivePending
  "resuming receive of file %s from %s at position %d",               // DEFSTR_DCC_ReceiveResumed
  "offering file %s to %s",                                           // DEFSTR_DCC_Send
  "sending file %s to %s",                                            // DEFSTR_DCC_SendAccepted
  "resuming send of file %s to %s at position %d",                    // DEFSTR_DCC_SendResumed
  "Initialising DCC Chat to %s",                                      // DEFSTR_DCC_ChatInit
  "Sending DCC Chat Request to %s (Port %d)",                         // DEFSTR_DCC_ChatConnect

  "<%s> %s",                                                          // DEFSTR_Channel_Highlight
  "%s joined channel %s",                                             // DEFSTR_Channel_Join
  "%s left channel %s : %s",                                          // DEFSTR_Channel_Part
  "You were kicked from channel %s by %s : %s",                       // DEFSTR_Channel_Kicked
  "%s was kicked from channel %s by %s : %s",                         // DEFSTR_Channel_Kick
  "%s is talking in %s : %s",                                         // DEFSTR_Channel_Text
  "%s is doing actions in %s",                                        // DEFSTR_Channel_Action
  "%s changed the %s topic to %s"                                     // DEFSTR_Channel_Topic
};

char *g_OutputInfoContentsNames[BIC_CONSOLELAST - BIC_CONSOLEFIRST] =
{
  "FUNCTION",
  "SOCKET",
  "WARNING",
  "ERROR",
  "GUI",
  "INFO",
  "XML",
};

int g_DefaultFilterList_Output[] =
{
  FILTER_EXCLUDE,
  BIC_SOCKET,
  FILTER_END
};

int g_DefaultFilterList_ChannelMonitor[] =
{
  FILTER_INCLUDE,
  BIC_UNKNOWN,
  BIC_CHANNELTEXT,
  BIC_OWNTEXT,
  BIC_ACTION,
  BIC_TOPIC,
  FILTER_END
};

int g_DefaultFilterList_ServerMonitor[] =
{
  FILTER_EXCLUDE,
  FILTER_END
};

int g_DefaultFilterList_DirtyStatusServer[] =
{
  FILTER_INCLUDE,
  BIC_UNKNOWN,
  BIC_CTCP,
  BIC_QUERYTEXT,
  BIC_NOTICE,
  BIC_WALLOPS,
  FILTER_END
};

int g_DefaultFilterList_DirtyStatusChannel[] =
{
  FILTER_INCLUDE,
  BIC_UNKNOWN,
  BIC_CHANNELTEXT,
  BIC_QUERYTEXT,
  BIC_NOTICE,
  BIC_TOPIC,
  BIC_ACTION,
  FILTER_END
};

int g_DefaultFilterList_DirtyStatusQuery[] =
{
  FILTER_INCLUDE,
  BIC_UNKNOWN,
  BIC_CHANNELTEXT,
  BIC_QUERYTEXT,
  BIC_NOTICE,
  BIC_ACTION,
  FILTER_END
};

int g_DefaultFilterList_VerboseLevel1[] = // Filter More, Show Less
{
  FILTER_EXCLUDE,
  BIC_JOIN,
  BIC_PART,
  BIC_QUIT,
  BIC_MODE,
  BIC_KICK,
  FILTER_END
};

int g_DefaultFilterList_VerboseLevel2[] =
{
  FILTER_EXCLUDE,
  BIC_JOIN,
  BIC_PART,
  BIC_QUIT,
  FILTER_END
};

int g_DefaultFilterList_VerboseLevel3[] = // Filter Less, Show More
{
  FILTER_EXCLUDE,
  BIC_JOIN,
  BIC_PART,
  FILTER_END
};


void InstallUrlHandler() 
{
	HKEY hk; 
  char Buf[MAX_PATH];
	char *Command = HydraIRC_BuildString(MAX_PATH,"\"%s\" %%1",g_AppName);
  *Buf = 0;

	if(::RegOpenKeyEx(HKEY_CLASSES_ROOT, "irc\\Shell\\Open\\Command", 0, KEY_WRITE | KEY_READ, &hk) == ERROR_SUCCESS) {
		DWORD bufLen = sizeof(Command);
		DWORD type;
		::RegQueryValueEx(hk, NULL, 0, &type, (LPBYTE)Buf, &bufLen);
		::RegCloseKey(hk);
	}

	if(stricmp(Command, Buf) != 0) {
		::RegCreateKey(HKEY_CLASSES_ROOT, "irc", &hk);
		char* tmp = "URL:IRC Protocol";
		::RegSetValueEx(hk, NULL, 0, REG_SZ, (LPBYTE)tmp, strlen(tmp) + 1);
		::RegSetValueEx(hk, "URL Protocol", 0, REG_SZ, (LPBYTE)"", 1);
		::RegCloseKey(hk);

		::RegCreateKey(HKEY_CLASSES_ROOT, "irc\\Shell\\Open\\Command", &hk);
		::RegSetValueEx(hk, "", 0, REG_SZ, (LPBYTE)Command, strlen(Command) + 1);
		::RegCloseKey(hk); 

		::RegCreateKey(HKEY_CLASSES_ROOT, "irc\\DefaultIcon", &hk);
		::RegSetValueEx(hk, "", 0, REG_SZ, (LPBYTE)g_AppName, strlen(g_AppName) + 1);
		::RegCloseKey(hk);
	}
	free(Command);
} 

// Utility Functions
//
/////////////////////////////////////////////////////////////////////////////

// TODO: get rid of this and use GetNetworkDetails() instead
char *GetNetworkName(int ID)
{
  for (int i = 0 ; i < g_NetworkList.GetSize() ; i++ )
  {
    if (g_NetworkList[i]->m_ID == ID)
      return g_NetworkList[i]->m_Name;
  }
  return "Unknown"; // be helpful! // don't use characters that are not allowed for filenames/paths in this string
}

IRCNetworkDetails *GetNetworkDetails(int ID)
{
  for (int i = 0 ; i < g_NetworkList.GetSize() ; i++ )
  {
    if (g_NetworkList[i]->m_ID == ID)
      return g_NetworkList[i];
  }
  return NULL;
}

// We do this a better/quicker way now..
/*
IRCServer *FindIRCServer(HWND hWnd)
{
  IRCServer *pServer;
  for (int i = 0; i < g_ServerList.GetSize() ; i++)
  {
    pServer = g_ServerList[i];

    if (pServer->m_pChildWnd->m_hWnd == hWnd)
    {
      return pServer;
    }
    for (int i = 0 ; i < pServer->m_ChannelList.GetSize(); i++ )
    {
      if (pServer->m_ChannelList[i]->m_pChildWnd && 
          pServer->m_ChannelList[i]->m_pChildWnd->m_hWnd == hWnd)
      {
        return pServer;
      }
    }
  }
  return NULL;
}
*/

/*
CChildFrame *FindChildFrame(HWND hWnd)
{
  if (!hWnd)
    return NULL;

  CChildFrame *pChildWnd;
  for (int i = 0 ; i < g_ChildWndList.GetSize() ; i ++)
  {
    pChildWnd = CHILDFRAMEPTR(g_ChildWndList[i]);
    if (pChildWnd->m_hWnd == hWnd)
      return pChildWnd;
  }
  return NULL;
}
*/

CChildCommon *FindChildWindow(HWND hWnd)
{
  if (!hWnd)
    return NULL;

  for (int i = 0 ; i < g_ChildWndList.GetSize() ; i ++ )
  {
    if (g_ChildWndList[i]->m_hWnd == hWnd)
      return g_ChildWndList[i];
  }
  return NULL;
}

CChildCommon *FindChildWindowByID(int WindowID)
{

  for (int i = 0 ; i < g_ChildWndList.GetSize() ; i ++ )
  {
    CChildFrame *pChild = CHILDFRAMEPTR(g_ChildWndList[i]);
    int ID = -1;
    switch (pChild->m_WindowType)
    {
      case CWTYPE_SERVER:
      case CWTYPE_DCCCHAT:
        ID = pChild->m_pServer->m_ServerID;
        break;  
      case CWTYPE_CHANNEL:
        ID = pChild->m_pChannel->m_ChannelID;
        break;  
      case CWTYPE_QUERY:
        ID = pChild->m_pQuery->m_QueryID;
        break;  
    }

    if (ID == WindowID)
      return (CChildCommon *)pChild;

  }
  return NULL;
}


// gets the IRCChannel from a channel window
IRCChannel *FindIRCChannel(HWND hWnd)
{
  if (!hWnd)
    return NULL;

  CChildFrame *pChildWnd = CHILDFRAMEPTR(FindChildWindow(hWnd));
  if (pChildWnd && pChildWnd->m_WindowType == CWTYPE_CHANNEL)
    return pChildWnd->m_pChannel;
  else
    return NULL;
}

// this finds the first channel with the name, NOTE, it might be preferable
// to search the server instance's channel list because the user might be
// connected to channels on different networks with the same name
IRCChannel *FindIRCChannelByName(char *Name)
{
  if (!Name)
    return NULL;

  for (int i = 0 ; i < g_ChannelList.GetSize(); i ++)
  {
    IRCChannel *pChannel = g_ChannelList[i];
    if (stricmp(pChannel->m_pProperties->m_Name,Name) == 0)
      return pChannel;
  }

  return NULL;
}

// gets the IRCServer from a channel OR server window
IRCServer *FindIRCServer(HWND hWnd, BOOL IncludeChannels, BOOL IncludeQueries)
{
  if (!hWnd)
    return NULL;

  CChildFrame *pChildWnd = CHILDFRAMEPTR(FindChildWindow(hWnd));
  if (pChildWnd && 
      (pChildWnd->m_WindowType == CWTYPE_SERVER || 
       (IncludeChannels && pChildWnd->m_WindowType == CWTYPE_CHANNEL) ||
       (IncludeChannels && pChildWnd->m_WindowType == CWTYPE_QUERY) ) )
    return pChildWnd->m_pServer;
  else
    return NULL;
}

// find a DCC Chat session that was created by a particular server
// WARNING: the pointer to the actual IRC Server instance pointed to
// by IRCServer->m_pOtherServer may not be valid.  check to see if
// the returned value is in g_ServerList before accessing it!
IRCServer *FindDCCChat(IRCServer *pOriginatingServer, char *Nick)
{
  if (!Nick || !*Nick || !pOriginatingServer)
    return NULL;

  // we can do it this way, which is neither good, nor bad...
  // but this way can't handle dcc's that don't have servers any more
  // TODO: i guess i really need a g_DCCChatList too...
  /*
  for (int i = 0 ; i < g_ServerList.GetSize(); i ++)
  {
    IRCServer *pServer = g_ServerList[i];

    if (pServer != pOriginatingServer)
      continue;

    for (int j = 0 ; j < pServer->m_DCCChatList.GetSize(); j++ )
    {
      IRCServer pDCCChat = pServer->m_DCCChatList[j];
      if (stricmp(Nick, pDCCChat) == 0)
        return pDCCChat;
    }

    break;
  }
  return NULL;
  */

  // or we can do it this way, which is also neither good, nor bad
  
  for (int i = 0 ; i < g_ChildWndList.GetSize() ; i ++ )
  {
    CChildFrame *pChild = CHILDFRAMEPTR(g_ChildWndList[i]);
    if (pChild->m_WindowType == CWTYPE_DCCCHAT &&
        pChild->m_pServer->m_pOtherServer == pOriginatingServer &&
        stricmp(pChild->m_pServer->m_OtherNick,Nick) == 0)
      return (pChild->m_pServer);
  }
  return NULL;
  
}

// gets the IRCServer from a DCC Chat window
IRCServer *FindDCCChat(HWND hWnd)
{
  if (!hWnd)
    return NULL;

  CChildFrame *pChildWnd = CHILDFRAMEPTR(FindChildWindow(hWnd));
  if (pChildWnd && 
    (pChildWnd->m_WindowType == CWTYPE_DCCCHAT) )
    return pChildWnd->m_pServer;
  else
    return NULL;
}

// gets the IRCQuery from a DCC Chat window
IRCQuery *FindIRCQuery(HWND hWnd)
{
  if (!hWnd)
    return NULL;

  CChildFrame *pChildWnd = CHILDFRAMEPTR(FindChildWindow(hWnd));
  if (pChildWnd && 
    (pChildWnd->m_WindowType == CWTYPE_QUERY) )
    return pChildWnd->m_pQuery;
  else
    return NULL;
}

IRCServer *FindServerByID(int ServerID)
{
  IRCServer *pServer;
  for (int i = 0 ; i < g_ServerList.GetSize() ; i ++)
  {
    pServer = g_ServerList[i];
    if (pServer->m_ServerID == ServerID)
      return pServer;
  }
  return NULL;
}

IRCServer *FindServerOnNetwork(int NetworkID)
{
  // only search for a server on the same network if the network is known
  if (NetworkID <= 0)
    return NULL;

  IRCServer *pServer;
  for (int i = 0 ; i < g_ServerList.GetSize() ; i ++)
  {
    pServer = g_ServerList[i];
    if (pServer->m_pDetails->m_NetworkID == NetworkID)
      return pServer;
  }
  return NULL;
}

IRCServer *FindServerByName(char *Name, int port /* = -1*/)
{
  if (!Name)
    return NULL;

  IRCServer *pServer;
  for (int i = 0 ; i < g_ServerList.GetSize() ; i ++)
  {
    pServer = g_ServerList[i];
    if (stricmp(pServer->m_pDetails->m_Name,Name) == 0)
    {
      if (port == -1 || port == pServer->m_pDetails->m_Port)
        return pServer;
    }
  }
  return NULL;
}

IRCChannel *FindChannelByID(int ChannelID)
{
  IRCChannel *pChannel;
  for (int i = 0 ; i < g_ChannelList.GetSize() ; i ++)
  {
    pChannel = g_ChannelList[i];
    if (pChannel->m_ChannelID == ChannelID)
      return pChannel;
  }
  return NULL;
}

IRCQuery *FindQueryByID(int QueryID)
{
  IRCQuery *pQuery;
  for (int i = 0 ; i < g_QueryList.GetSize() ; i ++)
  {
    pQuery = g_QueryList[i];
    if (pQuery->m_QueryID == QueryID)
      return pQuery;
  }
  return NULL;
}


char *stripcrlf( char *str )
{
  if (!str || !*str)
    return str;

  int len = strlen(str);
  while (len-- >= 0 && (str[len] == '\n' || str[len] == '\r')) 
    str[len] = 0;
  return str;
}

char *strippathseparator( char *str )
{
  if (!str || !*str)
    return str;

  int len = strlen(str)-1;
  
  if (str[len] == '\\' || str[len] == '//')
    str[len] = 0;
  return str;
}

char *stripwhitespace( char *str )
{
  if (!str || !*str)
    return str;

  int len = strlen(str);
  while (len-- >= 0 && (str[len] == ' ' || str[len] == '\t'))
    str[len] = 0;
  return str;
}

BOOL ischannelstartchar( char c )
{
  return (c == '#' || c == '&' || c == '+' || c == '!');
}

BOOL ischannelchar( char c )
{
  char *NotAllowedChars=", \007"; // separators, whitespace, channel prefixes (RFC Spec)
  char *StrictNotAllowedChars=":#&+!"; // most servers don't allow channel names with these, but *sigh* some do...

  BOOL Valid = strchr(NotAllowedChars,c) == NULL;

  if (BOOLPREF(PREF_bStrictChannelCharacters) && strchr(StrictNotAllowedChars,c) != NULL)
    Valid = FALSE;

  return (Valid);
}

BOOL isvalidchannelname( char *name )
{
  int i;

  if (!ischannelstartchar(*name))
    return false;

  i = 1;
  while (name[i])
  {
    if (!ischannelchar(name[i]))
      return false;
    i++;
  }
  return TRUE;
}

int GetConnectedServerCount( void )
{
  int ConnectedServers = 0;
  for (int i = 0 ; i < g_ServerList.GetSize() ; i ++)
  {
    if (g_ServerList[i]->GetStatus() == SVR_STATE_CONNECTED)
    {
      ConnectedServers ++;
    }
  }
  return ConnectedServers;
}

#ifdef DEBUG
// big buffers for debug builds
#define SYS_PRINTF_BUFFER_SIZE 16384
#define SYS_LOGINFO_BUFFER_SIZE 16384
#else
// 10240 so we can dump the full socket buffer *AND* the rest of the preceding string....
#define SYS_PRINTF_BUFFER_SIZE 10240
#define SYS_LOGINFO_BUFFER_SIZE 10240
#endif

void sys_Log(const int Contents, const char *buf)
{
  HANDLE hFile;
  char loginfo[SYS_LOGINFO_BUFFER_SIZE];

  time_t now;
  struct tm *t;

  time(&now);
  t = localtime(&now);

  char *time = stripcrlf(asctime(t));
  _snprintf(loginfo,sizeof(loginfo),"%s - %s - %s\n",time,g_OutputInfoContentsNames[Contents - BIC_CONSOLEFIRST],buf);

  hFile = CreateFile(g_ConsoleLogFile,
                      GENERIC_WRITE,
                      FILE_SHARE_READ,
                      NULL,
                      OPEN_ALWAYS,
                      FILE_ATTRIBUTE_NORMAL,
                      NULL);
  if (hFile != INVALID_HANDLE_VALUE)
  {
    DWORD BytesWritten, BytesToWrite = strlen(loginfo);

    DWORD dwPtr = SetFilePointer (hFile, 0L, NULL, FILE_END);
    if (dwPtr != INVALID_SET_FILE_POINTER)
    {
      LockFile(hFile, dwPtr, 0, BytesToWrite, 0);
      WriteFile(hFile, loginfo, BytesToWrite, &BytesWritten, NULL);
      UnlockFile(hFile, dwPtr, 0, BytesToWrite, 0);
    }

    CloseHandle(hFile);
  }

  /*
  file  = _open(g_ConsoleLogFile,_O_WRONLY | _O_APPEND | _O_CREAT | _O_TEXT , _S_IWRITE);
  if (file != -1)
  {
    _write(file,loginfo,strlen(loginfo));
    int x = errno;
    _close(file);
  }
  */
}

void sys_Logf(const int Contents, const char *format, ...)
{
  if (!format)
    return;

  va_list args;
  char *sysLogfBuffer;

  sysLogfBuffer = (char *)malloc(SYS_PRINTF_BUFFER_SIZE);
  if (sysLogfBuffer)
  {
    va_start(args, format);
    _vsnprintf(sysLogfBuffer, SYS_PRINTF_BUFFER_SIZE, format, args);
    va_end(args);
    sysLogfBuffer[SYS_PRINTF_BUFFER_SIZE-1] = 0;


    sys_Log(Contents, sysLogfBuffer);

    free(sysLogfBuffer);
  }
}

void sys_Printf(const int Contents, const char *format, ...)
{
  if (!format)
    return;

	va_list args;
	char *sysPrintfBuf;

  sysPrintfBuf = (char *)malloc(SYS_PRINTF_BUFFER_SIZE);
  if (sysPrintfBuf)
  {
	  va_start(args, format);
	  _vsnprintf(sysPrintfBuf, SYS_PRINTF_BUFFER_SIZE, format, args);
	  va_end(args);
    sysPrintfBuf[SYS_PRINTF_BUFFER_SIZE-1] = 0;


#ifdef DEBUG
    // always display output to debugger
    OutputDebugString(sysPrintfBuf);
#endif

    if (g_pMainWnd &&
        IsWindow(g_pMainWnd->m_hWnd) &&
        //IsWindow(g_pMainWnd->m_OutputView.m_hWnd) && 
        IsWindow(g_pMainWnd->m_OutputView.m_MsgView.m_hWnd) /*&&
        g_pMainWnd->m_OutputView.m_MsgView.IsWindowVisible()*/)
    {

      BufferItem *pBI;
      while (g_OutputLogBuffer.GetSize() > 0)
      {        
        pBI = g_OutputLogBuffer[0];
        g_OutputLogBuffer.RemoveAt(0);
        stripcrlf(pBI->m_Buffer);
        g_pMainWnd->m_OutputView.m_MsgView.Put(pBI->m_Contents, pBI->m_Buffer);

        if (g_pPrefs != NULL && BOOLPREF(PREF_bConsoleLogging))
        {
          sys_Log(Contents, pBI->m_Buffer);
        }

        delete pBI;
      }

      stripcrlf(sysPrintfBuf);
      g_pMainWnd->m_OutputView.m_MsgView.Put(Contents, sysPrintfBuf);

      if (g_pPrefs != NULL && BOOLPREF(PREF_bConsoleLogging))
      {
        sys_Log(Contents, sysPrintfBuf);
      }

      free(sysPrintfBuf);
    }
    else
    {
      if (g_StorePrintfs)
      {
        // store it for displaying later
        BufferItem *pBI = new BufferItem(Contents,sysPrintfBuf);
        if (pBI)
        {
          g_OutputLogBuffer.Add(pBI);
        }
        else
        {
          free(sysPrintfBuf); // eek! we didn't display this buffer...
        }
      }
      else
      {
        free(sysPrintfBuf); // or this one.
      }
    }
  }
}

// buf is modified, it's always the same size, or smaller.
char *stripcodes(char *buf)
{  
  if (!buf || !*buf) 
    return buf;

  int ci,i,len = lstrlen(buf);
  char *stripcodesbuf = (char *)malloc(len+1);
  //ZeroMemory(sbuf, len+1); // TODO: needed?
  int si = 0;
  char col[3]= {0,0,0};
  int colornum;

	for (i = 0; i < len ; i++)
	{
		switch(buf[i])
		{
      // color
			case '\003':
        col[0]=0;
        // get the fg color
				if (i < lstrlen(buf)-1)
				{
					if (isdigit((unsigned char)buf[i+1]))
					{
						col[0] = buf[i+1];
						i++;

            if (i < lstrlen(buf)-1)
					  {
						  if (isdigit((unsigned char)buf[i+1]))
						  {
							  col[1] = buf[i+1];
							  i++;

                // check for invalid color number, step back a character if it's not valid
                colornum = atoi(col);
                if ((colornum <0 || colornum >15) && colornum != 99)
                {
                  col[1] = 0;
                  i--;
                }
						  }
					  }
					}
				}

        // if we've got a , then get the bg color
        if (i < lstrlen(buf)-1 && buf[i+1] == ',')
        {
          i++;

				  if (i < lstrlen(buf)-1)
				  {
					  if (isdigit((unsigned char)buf[i+1]))
					  {
						  col[0] = buf[i+1];
						  i++;
					    if (i < lstrlen(buf)-1)
					    {
						    if (isdigit((unsigned char)buf[i+1]))
						    {
							    col[1] = buf[i+1];
							    i++;

                  // check for invalid color number, step back a character if it's not valid
                  colornum = atoi(col);
                  if ((colornum <0 || colornum >15) && colornum != 99)
                  {
                    col[1] = 0;
                    i--;
                  }
						    }
					    }
					  }
				  }
        }
				break;

        case '\004': // 16 million color - hex
        case '\007': // 16 million color (HydraIRC Highlighting) - hex
          // get the fg color
				  ci = 0;
          while ((i < lstrlen(buf)-1) && (isxdigit((unsigned char)buf[i+1])) && (ci < 6))
          {
            ci++;
						i++;
          }
          if (ci != 6)
            i-=ci; //step back if we've not got 6 hex digits

          // if we've got a ',' then get the bg color
          ci = 0;
          if (i < lstrlen(buf)-1 && buf[i+1] == ',')
          {
            i++;
            // get the bg color
            while ((i < lstrlen(buf)-1) && (isxdigit((unsigned char)buf[i+1])) && (ci < 6))
            {
              ci++;
						    i++;
            }
            if (ci != 6)
              i-=ci; //step back if we've not got 6 hex digits
          }
          break;

      // bold
			case '\002':
			case '\006':
			case '\022':
			case '\023':
      // reverse
			case '\026':
      // italics
			case '\035':
      // underline
			case '\037':
      // normal
			case '\017':
				break;

			default:
				{
					stripcodesbuf[si] = buf[i];
					si++;
				}
				break;
		}
  }
  stripcodesbuf[si] = 0;
  strncpy(buf,stripcodesbuf,len);
  free(stripcodesbuf);
  return buf;
}

// pass a NULL timestamp struct to use the current time.
// pass a NULL formatstr to use the default formatting string
char *GetTimestamp(struct tm *t /*= NULL*/, char *formatstr /* = NULL */)
{
  struct tm *now;
  if (!t)
  {
    time_t ltime;
    time( &ltime );
    now = localtime( &ltime );
    t = now;
  }
  char *timestampformat = formatstr;
  if (!timestampformat)
  {
    if (g_pPrefs) // make sure we *reallllly* have a default
      timestampformat = STRINGPREF(PREF_sDefaultTimestampFormat);
    else
      timestampformat = g_MainPrefs_Strings[PREF_sDefaultTimestampFormat - PREF_STRING_FIRST].m_Value;
  }
  strftime(g_TimeStampBuffer,sizeof(g_TimeStampBuffer),timestampformat,t);
  return g_TimeStampBuffer;
}

char *UseEmptyString(char *string)
{
  return (string ? string : "");
}

char *AllocEmptyString(char *string)
{
  return (string ? string : strdup(""));
}

BOOL CreatePath( char *Path)
{
  if (!Path || !*Path)
    return FALSE;

  char CurrentPath[MAX_PATH];
  int i;
  int Level = 0;
  int PathLen = -1; // so we know when to set it
  BOOL Created;
  DWORD Error;
  WIN32_FIND_DATA FindFileData;
  HANDLE hFind;

  strncpy(CurrentPath,Path,MAX_PATH);
  strippathseparator(CurrentPath);
  
  BOOL FoundPath = FALSE;
  do
  {
    // lock the directory
    hFind = FindFirstFile(CurrentPath, &FindFileData);

    if (hFind != INVALID_HANDLE_VALUE)
    {
      FindClose(hFind);

      if (Level == 0)
        return TRUE;
      else
        FoundPath = TRUE; // we got a valid path!
    }
    else
    {
      // couldn't lock this path,  try the level above.

      // build the string
      i = strlen(CurrentPath);

      if (PathLen == -1) 
        PathLen = i; // remember the length of the full path!

      if (i == 2)      
      {
        if (CurrentPath[1] == ':' || (CurrentPath[0] == '\\' && CurrentPath[1] == '\\'))
        {
          // giving up, reached root folder or start of UNC path.
          return FALSE;
        }
      }

      i--;

      while (i >= 0 && CurrentPath[i] != '\\')
        i--;
      if (i > 0)
      {
        CurrentPath[i] = 0; // strip the triling '\' and last directory name
      }
      else
      {
        // giving up, reached the start of the string.
        return FALSE;
      }
      // Look again, using the new CurrentPath...
      Level++;
    }
  } while (!FoundPath);

  // path doesn't exist, create it.

  i = strlen(CurrentPath);

  do
  {

    // the current path might look like this: c:\temp\0test1\0test2\0
    // so we just replace the first null terminator with a '\'
    CurrentPath[i++] = '\\'; 

    // or we could do this ;)
    /*
    while (Path[i] != '\\' && (CurrentPath[i] = Path[i])) 
      i++; // copy the rest of the string up to the '\' but not including.
    CurrentPath[i] = 0; // make sure!
    */


    Created = (CreateDirectory(CurrentPath,NULL) != 0);
    if (!Created)
    {
      Error = GetLastError();
      return FALSE; // failed!
    }
    // ok, created it!
    // now move to the next directory to create (unless we've got no more to create...)
    i = strlen(CurrentPath);

  } while (PathLen != i);

  return Created;
}

char *GetOutputThemeFormatStr(OutputThemeItem_t *OutputTheme,int IRCCommandID, int TypeFlags)
{
  // due to the parse order, exact matches must precede inexact matches
  char *DefaultFormatStr = "<ALL>";
  char *FormatStr = DefaultFormatStr;

  if (OutputTheme)
  {
    while (OutputTheme->m_WindowType != -1)
    {
      if (OutputTheme->m_IRCCommandID == IRCCommandID)
      {
        if (OutputTheme->m_WindowType == TypeFlags)
        {
          FormatStr = OutputTheme->m_FormatStr;
          break; // we've found an exact match
        }
        if (OutputTheme->m_WindowType & TypeFlags)
        {
          FormatStr = OutputTheme->m_FormatStr;
          break; // we've found a match that will do.
        }
      }
      if (OutputTheme->m_IRCCommandID == -1)
      {
        if (OutputTheme->m_WindowType == -1)
        {
          if (FormatStr == DefaultFormatStr)
            FormatStr = OutputTheme->m_FormatStr;
        }
        else if (OutputTheme->m_WindowType == TypeFlags)
        {
          if (FormatStr == DefaultFormatStr)
            FormatStr = OutputTheme->m_FormatStr;
        }
        else if (OutputTheme->m_WindowType & TypeFlags)
        {
          if (FormatStr == DefaultFormatStr)
            FormatStr = OutputTheme->m_FormatStr;
        }
        // keep searching, but remember the customized default.
      }
      OutputTheme++;
    }    
  }
  return FormatStr;
}

// variable count can be 0, if it is make sure VariableNames and VariableList are also null
void ReplaceVariables(char *FormatBuffer, const int MaxBufferSize, const char *FormatStr, const char **VariableNames, const char **VariableList, const int VariableCount)
{
  BOOL IDs;

  IDs=-1;

  int i,j,t,v;

  i = 0; // position in FormatStr;
  j = 0; // position in FormatBuffer;
  
  do
  {
    if (FormatStr[i] == '<')
    {
      if (IDs == -1)
      {
        IDs = i;
      }
      else
      {
        for (t = IDs; t < i; t++)
          FormatBuffer[j++] = FormatStr[t];

        IDs = i;
      }
      continue;
    }
    if (FormatStr[i] == '>')
    {
      if (IDs == -1)
      {
        FormatBuffer[j++] = FormatStr[i];
      }
      else
      {
        const char *comparestr = FormatStr + IDs + 1;
        int comparelen = i - IDs - 1;
        //FormatStr[i] = '\0'; // set the end of the compare string temporarily.
        char *defaultvalue = "<UNKNOWN>";
        const char *value = defaultvalue;
        //BOOL freevalue = FALSE;

        if (comparelen)
        {
          if (VariableCount)
          {
            // find the variable by name
            for (t = 0 ; t < VariableCount ; t ++)
            {
              if (strnicmp(comparestr,VariableNames[t],comparelen) == 0 && strlen(VariableNames[t]) == comparelen)
              {
                value = VariableList[t];

                if (!value)
                  value = "N/A";

                break;
              }
            }
          }

          if (value == defaultvalue) // we didn't find the name in our variable array
          {
            // maybe it's an application defined one
            char *comparecopy = (char *)malloc(comparelen+1);
            if (comparecopy)
            {
              strncpy(comparecopy,comparestr,comparelen);
              comparecopy[comparelen] = 0; // null terminate it, strncpy() implementations differ..

              char *option = comparecopy;
              char *param = strchr(comparecopy,'=');

              if (param)
              {
                *param = 0; // null terminate option
                param++; // skip the '='
              }

              // we could strip whitespace in option here, but we want to keep this as fast as possible...
              // params doesn't need to be stripped for whitespace in some cases.

              _strupr(option);  //convert once, to so we can use strcmp instead of strIcmp

              if (strcmp(option,"TIMESTAMP") == 0)
              {
                time_t ltime;
                struct tm *now;
                time( &ltime );
                now = localtime( &ltime );

                /*
                char *timestampformat;
                if (param) // "<TIMESTAMP>" is valid, and so is "<TIMESTAMP=timestampformat>
                  timestampformat = param;
                else
                {
                  if (g_pPrefs) // make sure we *reallllly* have a default
                    timestampformat = STRINGPREF(PREF_sDefaultTimestampFormat);
                  else
                    timestampformat = g_MainPrefs_Strings[PREF_sDefaultTimestampFormat - PREF_STRING_FIRST].m_Value;
                }

                strftime(g_TimeStampBuffer,sizeof(g_TimeStampBuffer),timestampformat,now);
                value = g_TimeStampBuffer;
                */
                value = GetTimestamp(now,param); // param can be null...
              } 
              else if (strcmp(option,"VERSION") == 0)
              {
                value = VERSIONSTRING;
              }

              free(comparecopy);
            }
          }
        }

        v = 0;
        while ((FormatBuffer[j] = value[v++]) && (j < MaxBufferSize-1))
          j++;

        ATLASSERT(!(j == MaxBufferSize));
        //if (freevalue) free(value); 

        //FormatStr[i] = '>'; // put the > back in the formatstr
        FormatBuffer[j] = 0; // null terminate the output buffer in case this is the last '>'
        IDs = -1;
        //IDe = 0;
      }
      continue;
    }
    if (IDs != -1)
    {
      continue;
    }
    FormatBuffer[j++] = FormatStr[i];

      
  } while(FormatStr[i++] && j < MaxBufferSize);

}



/** Free the current buffer and copy the text from the window to a new buffer
 *
 *  buffer can be NULL
 *  returns a pointer to the buffer.
 */
char *CopyWindowText(CWindow *Wnd,char * &buffer)
{
  int len;
  if (buffer)
    free (buffer);
  if ((len = Wnd->GetWindowTextLength()) == 0)
    buffer = NULL;
  else
  {
    buffer = (char *)malloc(len+1);
    Wnd->GetWindowText(buffer,len+1);
    stripwhitespace(buffer);
  }
  return buffer;
}

// Fonts...

HFONT GetAppFont(int PrefID) // PrefID, not FontObjectID!
{
  int FontObjectID = GetLookupID(g_FontIDLookupTable, FONTIDTABLECOUNT, PrefID);
  return (HFONT)g_Fonts.GetObjectByID(FontObjectID);
}

int GetFontMaxHeight(HFONT hFont)
{
  int MaxHeight = 0;

  HDC hDC;
  SIZE Size;
  char CharSet[(255-33) + 1];
  int i;

  // Ok, this is a bit poop.
  // I can't find a PlatformSDK function that returns the maximum size of
  // the largest letter in a given font.  We need this info so that the text input
  // box can be made tall enough to display the font correctly.
  // So the workaround is to build a string that contains all the printable characters
  // from the ascii character set, select each font object after creation
  // and then call GetTextExtentPoint32 which returns the size of a rectangle that
  // would hold the entire character set.  From this size, we just grab the height
  // and store it for later use.

  // create the string
  for (i = 0 ; i < 255-33 ; i ++)
  {
    CharSet[i] = i+33;
  }
  CharSet[255-33] = 0;

  hDC = GetWindowDC(g_pMainWnd->m_hWnd);
  ATLASSERT(hDC);
 
  // get the size of the rectangle that holds the character set string
  SelectObject(hDC,hFont);
  GetTextExtentPoint32(hDC,CharSet,strlen(CharSet),&Size);
  MaxHeight = Size.cy; // store just the height.

  ReleaseDC(g_pMainWnd->m_hWnd,hDC);

  return MaxHeight;
}

/** Initialise/load the fonts that we use.
 *
 *  The default/user's preferences *must* have been initalised/loaded first.
 */
void SetupFonts( void )
{
  if (!g_pMainWnd) 
    return;

  FontPrefItem_t *pFPI;
  HFONT hFont;
  int FontObjectID;

  int i,Height;
  HDC hDC;

#ifdef ZANN_FONTS
  // Pop up a font requester
  CFontDialog Dlg; 
  Dlg.DoModal(); 
  // If you examine the Dlg.m_lf memeber, you'll see it's correctly filled out
  // if the user clicked "OK"
#endif

  hDC = GetWindowDC(g_pMainWnd->m_hWnd);
  ATLASSERT(hDC);

  for (i = 0 ; i < PREF_FONT_MAX ; i ++)
  {

    // get the font object id from the font pref id...
    FontObjectID = GetLookupID(g_FontIDLookupTable, FONTIDTABLECOUNT, PREF_FONT_FIRST + i); 

    if (FontObjectID != -1)
    {
      // free the existing font.

      hFont = (HFONT)g_Fonts.GetObjectByID(FontObjectID);
      if (hFont)
      {
        DeleteObject((void *)hFont); // delete the font.
        g_Fonts.RemoveObjectByID(FontObjectID); // remove the font object.
      }
    }    

    pFPI = &g_pPrefs->m_FontPrefs[i];

    Height = MulDiv(pFPI->m_Size, GetDeviceCaps(hDC, LOGPIXELSY), 72);
    
#ifdef ZANN_FONTS
    hFont = CreateFontIndirect(&Dlg.m_lf);
#else    
    hFont = CreateFont(0-Height, 0, 
                        0, 0, 
                        pFPI->m_Flags & FIF_BOLD ? FW_BOLD : FW_NORMAL, 
                        pFPI->m_Flags & FIF_ITALIC ? TRUE : FALSE,
                        pFPI->m_Flags & FIF_UNDERLINE ? TRUE : FALSE,
                        FALSE, 
                        DEFAULT_CHARSET, 
                        OUT_RASTER_PRECIS, 
                        CLIP_DEFAULT_PRECIS, 
                        DEFAULT_QUALITY, 
                        FIXED_PITCH | FF_MODERN, 
                        pFPI->m_Name);
#endif
    FontObjectID = g_Fonts.AddObject(hFont);
    SetLookupID(g_FontIDLookupTable,FONTIDTABLECOUNT,PREF_FONT_FIRST + i,FontObjectID);
  }  
  ReleaseDC(g_pMainWnd->m_hWnd,hDC);

}

/** Free the fonts that we used.
 *
 */
void CleanupFonts( void )
{
  HFONT hFont;
  while (!g_Fonts.IsEmpty())
  {
    hFont = (HFONT)g_Fonts.GetFirstObject();
    g_Fonts.RemoveObject(hFont);
    DeleteObject(hFont);
  }
}

// Icons...

HICON GetAppIcon(int IconID) // IconID, not IconObjectID!
{
  int IconObjectID = GetLookupID(g_IconIDLookupTable, ICONIDTABLECOUNT, IconID);
  return (HICON)g_Icons.GetObjectByID(IconObjectID);
}

void SetupIcons( void )
{
  HICON hIcon;
  int i,IconObjectID;
  
  for (i = 0 ; i < ICONIDTABLECOUNT ; i ++)
  {

    // get the icon object id from the list of icon id's
    IconObjectID = GetLookupID(g_IconIDLookupTable, ICONIDTABLECOUNT, i); 

    if (IconObjectID != -1)
    {
      // free the existing icon.

      hIcon = (HICON)g_Icons.GetObjectByID(IconObjectID);
      if (hIcon)
      {
        DeleteObject((void *)hIcon); // delete the font.
        g_Icons.RemoveObjectByID(IconObjectID); // remove the font object.
      }
    }    

    // create the icons, based on their ID.
    switch(i)
    {
      case ICONID_NOTE:
        hIcon = AtlLoadIconImage(IDI_NOTE, LR_DEFAULTCOLOR);        
        break;
      case ICONID_INFORMATION:
        hIcon = AtlLoadIconImage(IDI_MYINFORMATION, LR_DEFAULTCOLOR);
        break;
      case ICONID_WARNING:
        hIcon = AtlLoadIconImage(IDI_MYWARNING, LR_DEFAULTCOLOR);
        break;
      case ICONID_USERS:
        hIcon = AtlLoadIconImage(IDI_USERLIST, LR_DEFAULTCOLOR);
        break;
      case ICONID_TRANSFERS:
        hIcon = AtlLoadIconImage(IDI_TRANSFERS, LR_DEFAULTCOLOR);
        break;
      case ICONID_SERVER_CONNECTED:
        hIcon = AtlLoadIconImage(IDI_SERVER, LR_DEFAULTCOLOR);
        break;
      case ICONID_USERNORMAL:
        hIcon = g_UserListImages.GetIcon(2); // 2 == icon ofset in userlist.bmp file (from 0)
        break;
      case ICONID_USERVOICE:
        hIcon = g_UserListImages.GetIcon(3);
        break;
      case ICONID_USERHALFOP:
        hIcon = g_UserListImages.GetIcon(4);
        break;
      case ICONID_USEROP:
        hIcon = g_UserListImages.GetIcon(5);
        break;
    }

    IconObjectID = g_Icons.AddObject(hIcon);
    SetLookupID(g_IconIDLookupTable,ICONIDTABLECOUNT,i,IconObjectID);
  }
}

/** Free the icons that we used.
 *
 */
void CleanupIcons( void )
{
  HICON hIcon;
  while (!g_Icons.IsEmpty())
  {
    hIcon = (HICON)g_Icons.GetFirstObject();
    g_Icons.RemoveObject(hIcon);
    DeleteObject(hIcon);
  }
}


void InitImages( void )
{
  // this must be called before SetupIcons();

	g_UserListImages.Create(16, 16, ILC_COLOR24 | ILC_MASK, 6, 0);
	ATLASSERT(g_UserListImages.m_hImageList);
	CBitmap bmp;
	bmp.LoadBitmap(IDB_USERLIST);
	g_UserListImages.Add( bmp, RGB(255,0,255));
}

// call this after any changes to the prefs, or after a prefs load.
void UpdateActivePrefs( void )
{
  int i;

  SetupFonts();

  g_pNotificationManager->DeleteEvents(g_PrefsEventSourceID);

  for (i = 0 ; i < g_pPrefs->m_NotificationPrefList.GetSize(); i++ )
  {
    g_pNotificationManager->Add(g_PrefsEventSourceID, g_pPrefs->m_NotificationPrefList[i]);
  }

  // TODO: move this paths checking code somewhere else.
  // check the paths are ok
  for (i = 0 ; i < PREF_PATH_MAX ; i ++)
  {
    char *Path = strdup(g_pPrefs->m_PathPrefs[i]);
    ReplaceString(&Path,"<HOMEDIR>",g_HomeDir);

    char *Message = HydraIRC_BuildString(MAX_PATH + 100,"Path for pref: \"%s\" (%s) ",g_PrefsStringIDNames[PREF_PATH_FIRST + i],Path);

    if (CreatePath(Path))
      sys_Printf(BIC_INFO,"%sis OK\n",Message);
    else
      sys_Printf(BIC_ERROR,"%scould not be created/accessed\n",Message);
    
    if (Message) free(Message);
    if (Path) free(Path);
  }

  
}

// Main App
//
/////////////////////////////////////////////////////////////////////////////

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{ 
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

  // TODO: should really check to see if all these "new" calls succeed.

  // create the unbound network info.
/*
  IRCNetworkDetails *pUnboundnetwork = new IRCNetworkDetails("Unbound",0);
  g_NetworkList.Add(pUnboundnetwork);
*/
  g_IDManager.SetIDRange(1); // 0 == Invalid ID
  g_pActiveSettings = new ActiveSettings;
  g_pNotificationManager = new NotificationManager;
  g_pPrefs = new Prefs;
  g_PrefsEventSourceID = g_pNotificationManager->ObtainSourceID(); 
  UpdateActivePrefs();
/*
#ifdef DEBUG
  // copy the prefs, to make sure we can load all the prefs
  // and duplicate the data into a new instance.
  // if we can't we'll get problems later on.

  // create new, based on old prefs
  Prefs *tmpprefs = new Prefs(g_pPrefs);

  // delete the old prefs
  delete g_pPrefs;

  // use the copy for here on out...
  g_pPrefs = tmpprefs;
#endif
*/
/*
  // Enable this section for testing of IgnoreInfo stuff..
  IgnoreInfo *pIgnore;
  
  // ignore IgnoreMe everywhere
  pIgnore = new IgnoreInfo("IgnoreMe",NULL,NETWORK_ALL,IIF_IGNORECHANNELLIST);
  g_pPrefs->m_IgnoreList.Add(pIgnore);

  // ignore IgnoreMe2 in #test on all networks
  pIgnore = new IgnoreInfo("IgnoreMe2",NULL,NETWORK_ALL,0);
  pIgnore->AddChannel("#test");
  g_pPrefs->m_IgnoreList.Add(pIgnore);

  // ignore IgnoreMe3 on network 9999
  pIgnore = new IgnoreInfo("IgnoreMe3",NULL,9999,IIF_IGNORECHANNELLIST);
  g_pPrefs->m_IgnoreList.Add(pIgnore);

  // ignore IgnoreMe4 on network 9999 in #test
  pIgnore = new IgnoreInfo("IgnoreMe4",NULL,9999,0);
  pIgnore->AddChannel("#test");
  g_pPrefs->m_IgnoreList.Add(pIgnore);

  // ignore IgnoreMe5 on network 9999 in #test,#test2,#test3
  pIgnore = new IgnoreInfo("IgnoreMe5",NULL,9999,0);
  pIgnore->AddChannel("#test");
  pIgnore->AddChannel("#test2");
  pIgnore->AddChannel("#test3");
  g_pPrefs->m_IgnoreList.Add(pIgnore);
*/
  InitImages(); // must be done before SetupIcons();
  SetupIcons();

  g_pPluginManager = new CPluginManager();

  // Get a handle to the default theme (see NULL param) and retain it
  // so that we don't keep allocating the themehandle for each new object
  // that uses the default theme

  ThemeHandle *pDefaultTheme = g_ThemeManager.GetThemeHandle(NULL);

  g_pMainWnd = new CMainFrame;

  if(g_pMainWnd->CreateEx() == NULL)
	{
		ATLTRACE(_T("Main window creation failed!\n"));
		return 0;
	}

  SetupFonts(); // move before main window create, need a to use soming other than the main window's hDC

  g_DefaultServerDetails = new IRCServerDetails;

  g_DefaultServerDetails->m_Description = strdup("Unknown Description");
  g_DefaultServerDetails->m_Name = strdup("Unknown");
  g_DefaultServerDetails->m_Password = NULL;
  g_DefaultServerDetails->m_Port = 6667;
  g_DefaultServerDetails->m_Flags = ISDF_CUSTOMLIST;
  g_DefaultServerDetails->m_NetworkID = NETWORK_UNKNOWN; // 0 == Unknown Network.
  g_DefaultServerDetails->m_Priority = PRI_NORMAL;

	g_vSizeCursor = LoadCursor(NULL, IDC_SIZEWE); // IDC_SIZENS

  // Note: I think commenting out this will make shortcuts that tell the app
  // to start minimized/maximized will have no effect now.
  // but this stops the window being drawn, and then being redrawn
  // when the state manager restores the previous state.
  // g_pMainWnd->ShowWindow(nCmdShow);
  // g_pMainWnd->ShowWindow(SW_MAXIMIZE); // not SW_SHOWMAXIMIZED

  
	int nRet = theLoop.Run();

  g_pPrefs->SavePrefs();

#ifdef DEBUG
  sys_Printf(BIC_INFO,"Killing %d server connection(s)\n",g_ServerList.GetSize());
#endif

  // if all is well, the messages should have floated around and done all the work
  // of killing the channel and server windows and their respective classes.
  while (g_ServerList.GetSize() > 0)
  {
    delete g_ServerList[0];
    g_ServerList.RemoveAt(0);
  }

  g_ThemeManager.ReleaseThemeHandle(pDefaultTheme);

  delete g_pPluginManager;

  // TODO: Save profile and prefs HERE (before we delete everything..)
  // (or maybe, save prefs before before main window is closed, then
  // we can confirm app exit if prefs save failed)

  // there should be no remaining MDI windows at this point.
  ATLASSERT(g_ChildWndList.GetSize() == 0);

  while (g_ServerDetailsList.GetSize() > 0)
  {
    delete g_ServerDetailsList[0];
    g_ServerDetailsList.RemoveAt(0);
  }
  
  while (g_NetworkList.GetSize() > 0)
  {
    delete g_NetworkList[0];
    g_NetworkList.RemoveAt(0);
  }

  delete g_DefaultServerDetails;

	_Module.RemoveMessageLoop();

  delete g_pMainWnd;
  g_pMainWnd = NULL; // must do this, or sys_Printf() will break.

  CleanupFonts();
  CleanupIcons();

  if (g_pIdentd)              delete g_pIdentd;
  if (g_pDNSResolver)         delete g_pDNSResolver;
  if (g_pTransfersManager)    delete g_pTransfersManager;
  if (g_pNoticeQueueManager)  delete g_pNoticeQueueManager;
  if (g_pPrivMsgQueueManager) delete g_pPrivMsgQueueManager;
  if (g_pEventLogManager)     delete g_pEventLogManager;
  if (g_pUrlCatcherManager)   delete g_pUrlCatcherManager;

  delete g_pPrefs;
  delete g_pActiveSettings;
  delete g_pNotificationManager;

  g_StorePrintfs = FALSE;
  BufferItem *pBI;
  while (g_OutputLogBuffer.GetSize() > 0)
  {        
    pBI = g_OutputLogBuffer[0];
    g_OutputLogBuffer.RemoveAt(0);
    delete pBI;
  }

	return nRet;
}

static void sendCmdLine(HWND hOther, LPTSTR lpstrCmdLine)
{
	char *cmdLine = _T(lpstrCmdLine);
	LRESULT result;

	COPYDATASTRUCT cpd;
	cpd.dwData = 0;
	cpd.cbData = strlen(cmdLine) + 1;
	cpd.lpData = (void *)cmdLine;
	result = SendMessage(hOther, WM_COPYDATA, NULL,	(LPARAM)&cpd);
}

BOOL CALLBACK searchOtherInstance(HWND hWnd, LPARAM lParam) {
	DWORD result;
	LRESULT ok = ::SendMessageTimeout(hWnd, WMU_WHERE_ARE_YOU, 0, 0,
		SMTO_BLOCK | SMTO_ABORTIFHUNG, 5000, &result);
	if(ok == 0)
		return TRUE;
	if(result == WMU_WHERE_ARE_YOU) {
		// found it
		HWND *target = (HWND *)lParam;
		*target = hWnd;
		return FALSE;
	}
	return TRUE;
}

BOOL FileExists( char *FileName )
{
  if (!FileName || !*FileName)
    return FALSE;

  // TODO: switch all file create/read/write methods over to the
  // same API
  HANDLE hFile;
  hFile = CreateFile(FileName,
                      GENERIC_READ,
                      FILE_SHARE_READ,
                      NULL,
                      OPEN_EXISTING,
                      FILE_ATTRIBUTE_NORMAL,
                      NULL);
  if (hFile != INVALID_HANDLE_VALUE)
  {
    // File exists!
    CloseHandle(hFile);
    return TRUE;
  }
    
  return FALSE;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{

  CDLLVersion *pDLL = new CDLLVersion((LPSTR)CRichEditCtrl::GetLibraryName());

  if (stricmp(pDLL->GetFullVersion(),"5.0.152.0") == 0)
  {
    MessageBox(NULL,"HydraIRC requires an updated version of RichEdit (riched20.dll)\n"
      "The version you have (\"5.0.152.0\") is not supported\nFor further information consult the documentation\n\n"
      "HydraIRC will continue as best it can - Please upgrade!","RichEdit problem!",MB_ICONERROR | MB_OK);
  }

#ifdef DEBUG
  // Leave this in to enable memory leak output at end of run.
  // All reported memory leaks should be investigated and fixed!

  _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

  // Enable this to break when the nth block of memory is allocated
	//_CrtSetBreakAlloc(23456);
#endif

  srand( (unsigned)time( NULL ) );

  g_CommandLine = lpstrCmdLine;
  GetCurrentDirectory(sizeof(g_StartDir),g_StartDir);
  GetModuleFileName(NULL, g_AppName, sizeof(g_AppName));
  g_AppDir = GetPathPart(g_AppName);
  g_HomeDir = strdup(g_StartDir);

  char *PrefsFile = NULL;
  BOOL PrefsFound = FALSE;
  
  // make sure HydraIRC.xml can be found in the current directory
  char *path = HydraIRC_BuildString(MAX_PATH,"%s\\HydraIRC.xml", g_HomeDir);
  SetString(PrefsFile, path);
  if (path != NULL) free(path);

  if (FileExists(PrefsFile))
    PrefsFound = TRUE;
      
  if (!PrefsFound)
  {
    // check again in new location.
    SetString(g_HomeDir,g_AppDir);
    SetCurrentDirectory(g_HomeDir);
    path = HydraIRC_BuildString(MAX_PATH,"%s\\HydraIRC.xml",g_HomeDir);
    SetString(PrefsFile, path);
    if (path != NULL) free(path);

    if (FileExists(PrefsFile))
      PrefsFound = TRUE;
  }

  if (!PrefsFound)
  {
    // look again in the parent of the exe's folder (generally only for when running from the debugger location)
    char *ParentFolder = GetPathPart(g_AppDir);
    if (ParentFolder)
    {
      path = HydraIRC_BuildString(MAX_PATH,"%s\\HydraIRC.xml",ParentFolder);
      SetString(PrefsFile, path);
      if (path != NULL) free(path);

      if (FileExists(PrefsFile))
      {
        // ... then change directory to the same place the .exe is located.
        SetString(g_HomeDir,ParentFolder);
        SetCurrentDirectory(g_HomeDir);
        PrefsFound = TRUE;
      }
      free(ParentFolder);
    }
  }

  SetString(PrefsFile,NULL);

  sprintf(g_ConsoleLogFile,"%s\\HydraIRC_Log.txt",g_HomeDir);

//#ifndef DEBUG
	SingleInstance hydraircapp("HYDRAIRC-{27F36160-BB35-4a38-8BF0-FED47B0565A0}");

	if(hydraircapp.IsAnotherInstanceRunning())
  {
		HWND hOther = NULL;
		EnumWindows(searchOtherInstance, (LPARAM)&hOther);

		if( hOther != NULL )
    {
			// pop up
			::SetForegroundWindow(hOther);

			if( IsIconic(hOther))
      {
				// restore
				::ShowWindow(hOther, SW_RESTORE);
			}
			sendCmdLine(hOther, g_CommandLine);
		}

		return FALSE;
	}
//#endif

  sys_Log(BIC_INFO,"*** HydraIRC " VERSIONSTRING " Started ***");
  sys_Logf(BIC_INFO,"Richedit Version %s",pDLL->GetFullVersion());

  delete pDLL;

  InstallUrlHandler();

	HRESULT hRes = ::CoInitialize(NULL);

// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);

  ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES | ICC_USEREX_CLASSES | ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES);	// add flags to support other controls

  WSADATA wsa;
  WSAStartup(MAKEWORD(1, 1), &wsa);

	hRes = _Module.Init(NULL, hInstance);
  ATLASSERT(SUCCEEDED(hRes));

	HMODULE hInstRich = ::LoadLibrary(CRichEditCtrl::GetLibraryName());

  ATLASSERT(hInstRich != NULL);

  /*
  HRSRC hVersionHandle = FindResource(hInstRich, (LPCSTR)VS_VERSION_INFO, RT_VERSION);
  HGLOBAL hResourceHandle = LoadResource(hInstRich, hVersionHandle);

  char *lpVersion = NULL;
  UINT uiVersion;

  VerQueryValue(hResourceHandle, 
    TEXT("\\StringFileInfo\\040904E4\\FileVersion"),
    (LPVOID*)&lpVersion,
    &uiVersion);
  */

  sys_Printf(BIC_INFO,"Homedir: \"%s\"",g_HomeDir);
  sys_Printf(BIC_INFO,"Appdir: \"%s\"",g_AppDir);
  sys_Printf(BIC_INFO,"Startdir: \"%s\"",g_StartDir);

	int nRet = Run(lpstrCmdLine, nCmdShow);

	::FreeLibrary(hInstRich);

	_Module.Term();

  WSACleanup();

	::CoUninitialize();

  sys_Log(BIC_INFO,"*** HydraIRC " VERSIONSTRING " Stopped ***");

  SetString(g_AppDir,NULL);
  SetString(g_HomeDir,NULL);

	return nRet;
}

