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

// HydraIRCTypes.h
//
/////////////////////////////////////////////////////////////////////////////

//extern class CPrefsManager *g_pPrefs, *g_pNewPrefs;
//extern class CInheritedPrefs *g_pActivePrefs;

#ifdef DEBUG

#define DBGFLG_NONE         0
#define DBGFLG_EVENTDUMP1   (1<<0)
#define DBGFLG_EVENTDUMPALL (1<<1)
#define DBGFLG_TIMER        (1<<2)
#define DBGFLG_FLOODCONTROL (1<<3)

extern DWORD g_DebugFlags; // see DBGFLG_* defines

#endif

extern class Prefs *g_pPrefs, *g_pNewPrefs;
extern class ActiveSettings *g_pActiveSettings;
extern class CMainFrame *g_pMainWnd;
extern class NotificationManager *g_pNotificationManager;
extern class TransfersManager *g_pTransfersManager;
extern class PrivMsgQueueManager *g_pPrivMsgQueueManager;
extern class NoticeQueueManager *g_pNoticeQueueManager;
extern class UrlCatcherManager *g_pUrlCatcherManager;
extern class EventLogManager *g_pEventLogManager;
extern class CPluginManager *g_pPluginManager;
extern class IDManager g_IDManager;
extern class ThemeManager g_ThemeManager;
extern class CEventManager g_EventManager;
extern class Identd *g_pIdentd;
extern class CDNSResolver *g_pDNSResolver;

extern char g_DebugStr[];
extern char *g_HomeDir;
extern char g_StartDir[];
extern char g_FQDN[1024]; // seeds size for sizeof() call
extern char g_EventLogMessageBuffer[];
extern char *g_ServerStatusNames[];
extern char *g_ChannelStatusNames[];
extern char *g_NotificationEventNames[];
extern char *g_SoundIDDisplayNames[];
extern char *g_DefaultStrings[];
extern char *g_PluginManagerErrorNames[];
extern char *g_CommandLine;

extern char *g_VersionStr;
extern char *g_AuthorStr;
#define MAX_TIMESTAMP_LEN 128
extern char g_TimeStampBuffer[MAX_TIMESTAMP_LEN];
extern const char *g_VariableIDNames[];
//extern int g_NEIDtoELIDMappings[];

extern HCURSOR g_vSizeCursor;
extern CImageList g_UserListImages;

extern int g_DefaultFilterList_ChannelMonitor[];
extern int g_DefaultFilterList_ServerMonitor[];
extern int g_DefaultFilterList_Output[];
extern int g_DefaultFilterList_DirtyStatusServer[];
extern int g_DefaultFilterList_DirtyStatusChannel[];
extern int g_DefaultFilterList_DirtyStatusQuery[];
extern int g_DefaultFilterList_VerboseLevel3[];
extern int g_DefaultFilterList_VerboseLevel2[];
extern int g_DefaultFilterList_VerboseLevel1[];

// Note: see CPluginManager::GetFunction() for this typedef's use
// unsigned long must be the size of a function pointer, which I
// belive is compiler, cpu and OS specific.  If i'm not doing this
// right, or there's a better way, please let me know.
typedef unsigned long* FUNCPTR;

// used to hold a list of int's, often passed to CreateSimpleFilter()
typedef CSimpleArray<int> intarray;


#define CWQUEUE_PRIVMSG 1
#define CWQUEUE_NOTICE  2
#define CWQUEUE_URLCATCHER 3
#define CWQUEUE_EVENT 4

// IRCServer::CreateChannel() Flag masks.
#define CCF_NONE             0
#define CCF_DONTSHOWIFEXISTS (1<<0)
// #define CCF_*             (1<<1)

// internally we use COLORPREF() when Put()'ing to a MsgView
// this means that the COLORREF is converted from BGR to RGB
// then converted back to BGR by the CMsgView::Put() routine.
// otherwise we'd up one with one set of colors
// always being inverted. :)
// converts 0x00a1b2c3 to 0x00c3b2a1 (just swaps two bytes round)
#define RGBVALTOCOLORREF(color) (((color & 0x000000ff) << 16) + (color & 0xff00ff00) + ((color & 0x00ff0000) >> 16))

#define CHILDFRAMEPTR(child) reinterpret_cast<CChildFrame *>(child)

#define CWM_INITIALIZE	          (WMDF_LAST+1)
#define WM_TEXTINPUT              (WMDF_LAST+2) // sent by the textinput windows
#define WM_NETEVENT               (WMDF_LAST+3) // sent by the Socket class.
#define WM_BUFFERWAITING          (WMDF_LAST+4) // sent when there is still data in the output buffer
#define WM_PROCESSQUEUEWAITING    (WMDF_LAST+5) // sent when there is data waiting in the input buffer
#define WM_IDENTDEVENT            (WMDF_LAST+6) // sent by the identd's socket
#define WM_TRANSFEREVENT          (WMDF_LAST+7) // sent by the DCCTransfer's socket
#define WM_DNSEVENT               (WMDF_LAST+8) // sent to and from the DNSResolver class
#define WM_REQUESTSCROLLTOEND     (WMDF_LAST+9) // sent when the buffer needs to be scrolled to the end             
#define WM_REQUESTAUTOTILE        (WMDF_LAST+10) // sent when an auto tile is required.
#define WM_DLGSTRINGRETURNPRESSED (WMDF_LAST+11) // sent by subclassed edit controls on some dialogs when enter is pressed.
#define WM_PROCESSDCCS            (WMDF_LAST+12) // sent when more dcc data needs to be sent.
#define WM_HEADERCHANGED          (WMDF_LAST+13) // sent when the childwindow's header pane needs resizing
#define WM_UPDATEINFOPANE         (WMDF_LAST+14) // sent to force a child window up update it's info pane
#define WM_MSGVIEWCONTEXTMENU     (WMDF_LAST+15) // sent by a MsgView window when a user right-clicks.
#define WM_CHILDFRAMECLOSED       (WMDF_LAST+16) // sent by CChildFrame in OnFinalMessage()
#ifdef USE_TABBEDMDI
#define WM_UPDATETAB              (WMDF_LAST+17) // sent to force a child window up update it's tab title
#endif

// Thread IDs
#define THID_DNSRESOLVER 1

#define FONT_MAX_NAME_LEN             256 // TODO: rename to MAX_FONT_NAME_LEN
#define MAX_COMMAND_PROFILE_LEN       1024
#define MAX_COMMAND_PROFILE_NAME_LEN  128

#define NETWORK_UNKNOWN 0
#define NETWORK_ALL     -1

#define DEFALT_SCROLLBUFFER_MAXLINES 500

#define CTRLCODE_MODE_INTERPRET 0 // displays colors and styles
#define CTRLCODE_MODE_STRIP     1 // strips the codes completely
#define CTRLCODE_MODE_DISPLAY   2 // displays the asci representation of the codes

#define ID_TOOLBAR (ATL_IDW_BAND_FIRST+1)
//#define ID_PLUGINSBAR (ATL_IDW_BAND_FIRST+2)
#define ID_TOOLBARUSER_FIRST          0x5000
#define ID_TOOLBARUSER_LAST           0x5FFF
#define ID_FAVORITES_FIRST            0x6000
#define ID_FAVORITES_LAST             0x6FFF
#define ID_EVENTLOGMENU_FIRST         0x7000
#define ID_EVENTLOGMENU_LAST          0x70FF
#define ID_TRANSFERSMENU_FIRST        0x7100
#define ID_TRANSFERSMENU_LAST         0x71FF
#define ID_SERVERLISTMENU_FIRST       0x7200
#define ID_SERVERLISTMENU_LAST        0x72FF

//For the IRCUserGroup.m_Flags member
//#define IRCUG_EXPAND   1
//#define IRCUG_EXPANDED 2
//                       4,8...
//#define IRCUG_DEFAULTFLAGS (IRCUG_EXPAND)

// order of icons in serverlist.bmp
#define ICON_SERVER    0
#define ICON_CHANNELS  1
#define ICON_CHANNEL   2
#define ICON_QUERY     3
#define ICON_DCCCHAT   4

#define FIF_NONE      0
#define FIF_BOLD      (1<<0)
#define FIF_ITALIC    (1<<1)
#define FIF_UNDERLINE (1<<2)

typedef struct FontInfo_s {
  HFONT Font;
  int Height;
  int MaxHeight;
} FontInfo_t;

#define DIRTY_FALSE   0
#define DIRTY_TRUE    (1<<0)
#define DIRTY_RECENT  (1<<1)

#define SETSTATUSBARTEXT_BUFFER_SIZE 128 // max strlen for SB_SETTEXT is 127 characters
#define SETSTATUSBARNICK_BUFFER_SIZE 64

// don't change the order of these.
#define NE_FLAG_NONE                  0
#define NE_FLAG_FORWARD               (1<<0) // not currently used (idea is to forward the text that causes the event to the current window)
#define NE_FLAG_DISABLED              (1<<1)
#define NE_FLAG_SOUNDFILE             (1<<2)
#define NE_FLAG_SYSTEMSOUND           (1<<3)
#define NE_FLAG_FLASHTASKBAR          (1<<4)
#define NE_FLAG_HIGHLIGHT             (1<<5)
#define NE_FLAG_LOGINEVENTLOG         (1<<6) // default for new events: enabled!


#define HIGHLIGHT_NONE          (0)
#define HIGHLIGHT_SUBSTRING     (1<<0)
#define HIGHLIGHT_REGEXP        (1<<1) // not implemented yet
#define HIGHLIGHT_WHOLELINE     (1<<2) // if defined.  the whole line's color are changed.
#define HIGHLIGHT_CASESENSITIVE (1<<3)
#define HIGHLIGHT_RESERVED      (1<<4) // was HIGHLIGHT_LOGINEVENTLOG, renamed to NE_FLAG_LOGINEVENTLOG in 0.3.134


// here's where we store what we load from the xml file.
typedef struct NotificationPrefInfo_s
{
  int   m_EventCode; // a valid number from NotificationEventCodes
                     // taken from "name"
  int   m_GroupID;   // a valid group id. (or -2) for default.
                     // taken from the parent node's "id"
  char *m_FileName;  // filename of a sound file, or NULL if m_SoundID is to be used
                     // taken from "filename"
  int m_SoundID;     // number of a system sound, or -1 (e.g. default beep, exclamation)
                     // only used if m_FileName is NULL
                     // taken from "soundid"
  int m_Flags;       // built from various strings in the xml file, see NE_FLAG_* defines.

  int m_HighlightFlags; // see HIGHLIGHT_* defines

  char *m_MatchString; // depending on highlight flags this can be a regexp, or just a substring to match

  COLORREF m_ForegroundColor; // -1 = don't change
  COLORREF m_BackgroundColor; // -1 = don't change

} NotificationPrefInfo_t;

enum DefaultStrings
{
  DEFSTR_DefaultStatusBarMessage,
  DEFSTR_Cancelled,
  DEFSTR_Unknown,
  DEFSTR_ConnectFailed,
  DEFSTR_ConnectionClosed,

  DEFSTR_Socket_Error,
  DEFSTR_Socket_Disconnected,
  DEFSTR_Socket_Connected,
  DEFSTR_Socket_ReconnectAborted,

  DEFSTR_Status_NewCTCPMsg,
  DEFSTR_Status_ServerConnected,
  DEFSTR_Status_ServerDisconnected,
  DEFSTR_Status_Welcome,

  DEFSTR_Server_Connect,
  DEFSTR_Server_Resolving,
  DEFSTR_Server_ResolveError,
  DEFSTR_Server_ConnectAddress,
  DEFSTR_Server_ConnectError,
  DEFSTR_Server_IdentityAndNick,
  DEFSTR_Server_Disconnected,
  DEFSTR_Server_DisconnectRetry,
  DEFSTR_Server_ConnectRetry,
  DEFSTR_Server_ConnectGiveUp,
  DEFSTR_Server_NickChanged,
  DEFSTR_Server_LoggedIn,
  DEFSTR_Server_Wallops,
  DEFSTR_Server_Quit,   // them

  DEFSTR_User_Notice,
  DEFSTR_User_PrivMsg,

  DEFSTR_CTCP_Sound,
  DEFSTR_CTCP_Version,
  DEFSTR_CTCP_Ping,
  DEFSTR_CTCP_Time,

  DEFSTR_DCC_Receive,
  DEFSTR_DCC_ReceivePending,
  DEFSTR_DCC_ReceiveResumed,
  DEFSTR_DCC_Send,
  DEFSTR_DCC_SendAccepted,
  DEFSTR_DCC_SendResumed, 
  DEFSTR_DCC_ChatInit,
  DEFSTR_DCC_ChatConnect,

  DEFSTR_Channel_Highlight,
  DEFSTR_Channel_Join,   // both, first param changed to "You" if it's us.
  DEFSTR_Channel_Part,   // them
  DEFSTR_Channel_Kicked, // us
  DEFSTR_Channel_Kick,   // them
  DEFSTR_Channel_Text,
  DEFSTR_Channel_Action,
  DEFSTR_Channel_Topic,
 
  DEFSTR_LAST
};

enum StatusBarIconIDs // for CMainFrame::SetStatusBar()
{
  STATUS_ICON_NONE,
  STATUS_ICON_NOTE,
  STATUS_ICON_INFORMATION,
  STATUS_ICON_WARNING,

  STATUS_ICON_DONTCHANGE // flag to say don't change the icon
};

enum IconIDs // for GetAppIcon()
{
  ICONID_SERVER_CONNECTED,
  //ICONID_SERVER_DISCONNECTED,
  ICONID_TRANSFERS,
  ICONID_USERS,
  ICONID_NOTE,
  ICONID_WARNING,
  ICONID_INFORMATION,
  ICONID_USERNORMAL,
  ICONID_USERVOICE,
  ICONID_USERHALFOP,
  ICONID_USEROP
  //ICONID_CHANNEL
  //ICONID_QUERY
};

// also see g_EventIDNames in EventManager.cpp
enum EventIDs
{
  // IRCServer specific events
  EV_SV_RECONNECT,
  EV_SV_SENDKEEPALIVE,  

  EV_PREFSCHANGED,
  // Add new events here
  EV_CH_RETRYJOIN,
  EV_CW_TIMER,

  // Common broadcasted events
  EV_TICK, // sent every one second to all listeners
  EV_DIEDIEDIE,

  // Add new events here

  // Placeholder - see definition of g_EventIDNames
  EV_LAST,  // all events above here *must* have a name in g_EventIDNames

  // TODO: These are unused as yet, the idea being that the event manager
  // can hand out unique id's from this range, with a call to
  // myid = CEventManager::GetNewEventID() and CEventManager::RelinquishEventID(myid)
  // by implementing this, plugins can create their own custom events.
  EV_USERFIRST = 0x0000FFFF,
  EV_USERLAST  = 0x0FFFFFFF
};

#ifdef DEBUG
  extern char *g_EventIDNames[EV_LAST];
#endif

typedef struct EventMsg_s
{
  int EventID;
  void *Data;
} EventMsg_t;

enum TimerIDs
{
  TID_MAINFRAME // only need one for now
};

// Channel mode flags (as we don't store the actual IRC protocol characters)
#define CHANNELMODE_INVITE (1<<0) // 1
#define CHANNELMODE_KEY    (1<<1) // 2
#define CHANNELMODE_LIMIT  (1<<2) // 4

// Output theme stuff

#define THEME_END -1
#define THEME_SERVER  (1<<0)
#define THEME_CHANNEL (1<<1)
#define THEME_QUERY   (1<<2)
#define THEME_DCCCHAT (1<<3)
#define THEME_ALL     (THEME_DCCCHAT | THEME_QUERY | THEME_CHANNEL | THEME_SERVER)

#define THEME_ANYCOMMAND -1
#define RPLS_NOTICE_CONNECT   10000
#define RPLS_NOTICE_SERVER    10001
#define RPLS_NOTICE           10002
#define RPLS_NICK             10003
#define RPLS_MODE             10004
#define RPLS_TOPIC            10005
#define RPLS_JOIN             10006
#define RPLS_JOIN_WE          10007
#define RPLS_PART             10008
#define RPLS_QUIT             10010
#define RPLS_KICK_WE          10011
#define RPLS_KICK             10012
#define RPLS_NOTICE_CTCP_RECEIVED 10013
#define RPLS_NOTICE_CTCP_SENT 10014
#define RPLS_PRIVMSG_CTCP_RECEIVED 10015
#define RPLS_PRIVMSG_ACTION   10016
#define RPLS_PRIVMSG_CHANNELTEXT 10017
#define RPLS_PRIVMSG_RECEIVED 10018
#define RPLS_PRIVMSG_SENT     10019
#define RPLS_PRIVMSG_CTCP_SENT 10020
#define RPLS_PING             10021
#define RPLS_PONG             10022
#define RPLS_PING_SENT        10023
#define RPLS_PONG_SENT        10024
#define RPLS_WALLOPS          10025
#define RPLS_REQUESTNICK      10026
#define RPLS_NOTICE_SENT      10027
#define APP_ERROR             20000
#define APP_CONNECT           20001

// Variable ID's (VID) (Must have matching strings in g_VariableIDNames)
enum VariableIDs {
  VID_RESPONSE,
  VID_MYNICK,
  VID_MYHOST,
  VID_SERVER,
  VID_SERVERDESCRIPTION,
  VID_CHANGABLESTART = VID_SERVERDESCRIPTION+1,
  
  VID_SERVEROUTPUT = VID_CHANGABLESTART,
  VID_CHANNELOUTPUT,
  VID_QUERYOUTPUT,
  VID_DCCCHATOUTPUT,
  // TODO: change ALL to show a combined string of all variables VID_PARAM1 - VID_PARAM5
  // and then use VID_GENERAL instead of VID_ALL everywhere
  VID_ALL,   // for general use,  e.g  the actual channel text, server notice, etc.
  VID_NICK,           // may contain user's nick prefix depending on prefs - don't use for nick matching
  VID_UNPREFIXEDNICK, // used for nick matching purposes - see VID_NICK
  VID_USERNAME,
  VID_HOST,
  VID_NAME,
  VID_REMOTESERVER,
  VID_REMOTESERVERDESCRIPTION,
  VID_IDLESECS,
  VID_SIGNONTIME,
  VID_SERVERNOTICE,
  VID_CHANNEL,
  VID_CHANNELMODE,
  VID_OTHERNICK,
  VID_CTCP,
  VID_PARAM1, // for general use  e.g.  /list channel user counts
  VID_PARAM2, // for general use 
  VID_PARAM3, // for general use 
  VID_PARAM4, // for general use 
  VID_PARAM5, // for general use 
  VID_LAST
};

typedef struct OutputThemeItem_s {
  // TODO: int m_Contents; ?
  int m_WindowType; // THEME_* or -1 for end of list
  int m_IRCCommandID; // RPL_* || RPLS_* || -1 for default
  char *m_FormatStr;
} OutputThemeItem_t;

// hehe, you gotta love this enum usage :)
enum PrefIDs {

  PREF_END_OF_LIST = -1,

  PREF_BOOL_FIRST = 0,
  PREF_bPromptOnServerClose = PREF_BOOL_FIRST,
  PREF_bPromptOnDisconnect,
  PREF_bShowChannelListDialogOnConnect,
  PREF_bShowChannelWindowUserListPaneByDefault,
  PREF_bShowChannelWindowHeaderPaneByDefault,
  PREF_bShowChannelWindowInfoPaneByDefault,
  PREF_bShowServerWindowUserListPaneByDefault,
  PREF_bShowServerWindowHeaderPaneByDefault,
  PREF_bShowServerWindowInfoPaneByDefault,
  PREF_bShowQueryWindowUserListPaneByDefault,
  PREF_bShowQueryWindowHeaderPaneByDefault,
  PREF_bShowQueryWindowInfoPaneByDefault,
  PREF_bShowDCCChatWindowUserListPaneByDefault,
  PREF_bShowDCCChatWindowHeaderPaneByDefault,
  PREF_bShowDCCChatWindowInfoPaneByDefault,
  PREF_bMonitorChannelsOnJoin,
  PREF_bMonitorServersByDefault,
  PREF_bShowChannelMonitorMessageOnStatusBar,
  PREF_bRememberAudioMute,
  PREF_bAudioMute,
  PREF_bAutoTile,
  PREF_bAutoMaximize,
  PREF_bHideNotMinimize,
  PREF_bRemoveOldPrivmsgLogItems,
  PREF_bRemoveOldNoticeLogItems,
  PREF_bRemoveOldURLCatcherItems,
  PREF_bRemoveOldEventLogItems,
  PREF_bChannelMemberNoticesOnly,

  PREF_bOutputWindowTimestamps,
  PREF_bChannelMonitorTimestamps,
  PREF_bServerMonitorTimestamps,

  PREF_bServerWindowTimestamps,
  PREF_bChannelWindowTimestamps,
  PREF_bQueryWindowTimestamps,
  PREF_bDCCChatWindowTimestamps,

  PREF_bShowUserListGroupCounts,
  PREF_bSendKeepAliveMessages,
  PREF_bShowPings,
  PREF_bShowPongs,
  PREF_bShowMOTD,
  PREF_bIgnoreCTCP,
  PREF_bConsoleLogging,
  PREF_bSocketLogging,
  PREF_bCrossNetworkNoticeForwarding,
  PREF_bMinimizeToSysTray,
  PREF_bCreateQueryWindowsAutomatically,
  PREF_bCreateQueryWindowsHidden,
  PREF_bRestrictQueriesToQueryWindows,
  PREF_bRestrictNoticesToQueryWindows,
  PREF_bForwardNotices,
  PREF_bForwardMessages,
  PREF_bForwardCTCPs,
  PREF_bForwardServerNotices,
  PREF_bDCCFastSends,
  PREF_bDCCVerboseTransfers,
  PREF_bDCCReceiveDoubleClickOpensFile,
  PREF_bDCCAutoAccept,
  PREF_bCreateFoldersForDCCTransfers,
  PREF_bIdentdEnabled,
  PREF_bSelectiveFloodControl,
  PREF_bStrictChannelCharacters,
  PREF_bServerListToolTips,
  PREF_bServerListChannelGroups,
  PREF_bLogging,
  PREF_bCreateNetworkLogFolder,
  PREF_bServerLogging,
  PREF_bChannelLogging,
  PREF_bQueryLogging,
  PREF_bDCCChatLogging,
  PREF_bStripCodesFromServerLogs,
  PREF_bStripCodesFromChannelLogs,
  PREF_bStripCodesFromQueryLogs,
  PREF_bStripCodesFromDCCChatLogs,
  PREF_bStripCodesFromServerWindows,
  PREF_bStripCodesFromChannelWindows,
  PREF_bStripCodesFromQueryWindows,
  PREF_bStripCodesFromDCCChatWindows,
  PREF_bShowPrivateMessagesOnStatusBar,
  PREF_bCTCPUseERRMSGReply,
  PREF_bRejoinChannelOnKick,
  PREF_bUseGradientFilledPanes,
  PREF_bClearDirtyFlagOnDeactivate,
  PREF_bShowNickPrefixes,
  PREF_bShowBuddyGroupsInUserList,
  PREF_bActivateLinkOnDoubleClick,

  PREF_BOOL_LAST = PREF_bActivateLinkOnDoubleClick,
  PREF_BOOL_MAX = (PREF_BOOL_LAST - PREF_BOOL_FIRST) + 1,

  PREF_INT_FIRST = PREF_BOOL_LAST + 1,
  PREF_nDefaultUserListWidth = PREF_INT_FIRST,
  PREF_nDefaultInfoHeight,
  PREF_nMaxHeaderLines,
  PREF_nMaxScrollBufferLines,
  PREF_nMaxPrivMsgLogAgeMins,
  PREF_nMaxNoticeLogAgeMins,
  PREF_nMaxURLCatcherAgeMins,
  PREF_nMaxEventLogAgeMins,
  PREF_nMaxPrivMsgLogItems,
  PREF_nMaxNoticeLogItems,
  PREF_nMaxURLCatcherItems,
  PREF_nMaxEventLogItems,
  PREF_nReconnectRetries1,
  PREF_nReconnectDelay1,
  PREF_nReconnectRetries2,
  PREF_nReconnectDelay2,
  PREF_nKeepAliveDelay,
  PREF_nTransferViewUpdateFrequency,
  PREF_nInactivityScrollDelay,
  PREF_nScrollLockTimeout,
  PREF_nFloodControlThreshold,
  PREF_nFloodControlItemsPerSecond,
  PREF_nQueryNotifyReactivateDelay,
  PREF_nDCCTransferTimeout,
  PREF_nDCCResumeTimeout,
  PREF_nDCCNegotiateTimeout,
  PREF_nDCCSendFirstPort,
  PREF_nDCCSendLastPort,
  PREF_nDCCSendBufferSize,
  PREF_nLogBufferSize,
  PREF_nLogFlushInterval,
  PREF_nMaxClipboardPasteLines,
  PREF_nMaxInputHistoryLines,
  PREF_nServerListServerRecentSeconds,
  PREF_nServerListDCCChatRecentSeconds,
  PREF_nServerListChannelRecentSeconds,
  PREF_nServerListQueryRecentSeconds,
  PREF_nServerConnectTimeout,
  PREF_nDCCChatConnectTimeout,
  PREF_nDCCChatFirstPort,
  PREF_nDCCChatLastPort,
  PREF_nChannelRejoinDelay,
  PREF_KickRejoinDelay,

  PREF_INT_LAST = PREF_KickRejoinDelay,
  PREF_INT_MAX = (PREF_INT_LAST - PREF_INT_FIRST) + 1,

  PREF_STRING_FIRST = PREF_INT_LAST + 1,
  PREF_sIdentdOS = PREF_STRING_FIRST,
  PREF_sIdentdUserName,
  PREF_sDefaultTimestampFormat,
  PREF_sDefaultWindowTimestampFormat,
  PREF_sDefaultIdentity,
  PREF_sServerLogFormat,
  PREF_sChannelLogFormat,
  PREF_sQueryLogFormat,
  PREF_sDCCChatLogFormat,
  PREF_sRouterAddress,
  PREF_sDefaultTheme,
  PREF_sNickCompletionSuffix,
  PREF_sLogFileViewer,
  PREF_sChannelBookmark1,
  PREF_sChannelBookmark2,
  PREF_sChannelBookmark3,
  PREF_sChannelBookmark4,
  PREF_sChannelBookmark5,
  PREF_sChannelBookmark6,
  PREF_sChannelBookmark7,
  PREF_sChannelBookmark8,
  PREF_sChannelBookmark9,
  PREF_STRING_LAST = PREF_sChannelBookmark9,
  PREF_STRING_MAX = (PREF_STRING_LAST - PREF_STRING_FIRST) + 1,

  PREF_PATH_FIRST = PREF_STRING_LAST + 1,
  PREF_pDCCDownloadsFolder = PREF_PATH_FIRST,
  PREF_pDCCUploadsFolder,
  PREF_pLogRootFolder,
  PREF_PATH_LAST = PREF_pLogRootFolder,
  PREF_PATH_MAX = (PREF_PATH_LAST - PREF_PATH_FIRST) + 1,

  PREF_FONT_FIRST = PREF_PATH_LAST + 1,
  PREF_fServerInputFont = PREF_FONT_FIRST,
  PREF_fServerOutputFont,
  PREF_fChannelInputFont,
  PREF_fChannelOutputFont,
  PREF_fOutputWindowFont,
  PREF_fChannelMonitorFont,
  PREF_fServerMonitorFont,
  PREF_fDCCChatInputFont,
  PREF_fDCCChatOutputFont,
  PREF_fQueryInputFont,
  PREF_fQueryOutputFont,
  PREF_fServerListFont,
  PREF_fServerListDirtyFont,
  PREF_fServerListRecentFont,
  PREF_fUserListFont,
  PREF_fHeaderFont,
  PREF_fInfoFont,
  PREF_FONT_LAST = PREF_fInfoFont,
  PREF_FONT_MAX = (PREF_FONT_LAST - PREF_FONT_FIRST) + 1,

  PREF_COLOR_FIRST = PREF_FONT_LAST + 1,
  color_white = PREF_COLOR_FIRST,  // the first 16 MUST appear in this order.
  color_black,                     // as they translate directly to the 16 color
  color_darkblue,                  // color code sequences
  color_darkgreen,
  color_red,
  color_brown,
  color_purple,
  color_orange,
  color_yellow,
  color_green,
  color_darkcyan,
  color_cyan,
  color_blue,
  color_pink,
  color_gray,
  color_lightgray,



  item_background,
  item_normal,
  item_notice,
  item_topic,
  item_join,
  item_kick,
  item_mode,
  item_nick,
  item_part,
  item_action,
  item_ctcp,
  item_quit,
  item_connecting,
  item_querytext,
  item_owntext,
  item_wallops,

  item_info,
  item_warning,
  item_error,

  item_timestamp,



  item_userlistbackground,
  item_userlistnormaltext,
  item_userlistselectedbackground,

  item_textinputbackground,
  item_textinputnormaltext,

  item_globaluserlistbackground,
  item_globaluserlistnormaltext,
  item_globaluserlistselectedbackground,

  item_serverlistbackground,
  item_serverlistnormaltext,
  item_serverlistselectedbackground,

  item_serverlistdisconnected,
  item_serverlistconnecting,
  item_serverlistconnected,
  item_serverlistserverdirtytext,
  item_serverlistserverdirtybackground,
  item_serverlistdccchatdirtytext,
  item_serverlistdccchatdirtybackground,
  item_serverlistchanneldirtytext,
  item_serverlistchanneldirtybackground,
  item_serverlistquerydirtytext,
  item_serverlistquerydirtybackground,
  item_serverlistserverrecenttext,
  item_serverlistserverrecentbackground,
  item_serverlistdccchatrecenttext,
  item_serverlistdccchatrecentbackground,
  item_serverlistchannelrecenttext,
  item_serverlistchannelrecentbackground,
  item_serverlistqueryrecenttext,
  item_serverlistqueryrecentbackground,

  item_transferlisterrortext,
  item_transferlistcompletetext,
  item_transferlistnormaltext,
  item_transferlistbackground,

  item_eventlogtext,
  item_eventlogbackground,

  item_urlcatchertext,
  item_urlcatcherurltext,
  item_urlcatchervisitedurltext,
  item_urlcatcherbackground,

  item_noticelogtext,
  item_noticelogbackground,

  item_querylogtext,
  item_querylogbackground,

  item_headertext,
  item_headerbackground,

  item_infotext,
  item_infobackground,

  PREF_COLOR_LAST = item_infobackground,
  PREF_COLOR_MAX = (PREF_COLOR_LAST - PREF_COLOR_FIRST) + 1,

  PREFSTRINGID_LAST = PREF_COLOR_LAST + 1

};

extern OutputThemeItem_t *g_DefaultOutputTheme;
