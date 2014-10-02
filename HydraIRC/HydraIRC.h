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

// HydraIRC.h
//
/////////////////////////////////////////////////////////////////////////////

//Not used, didn't do what I wanted.
//#include "MRUMDITab.h"

#include "PropertyList.h"
//#include "TreeListView.h"

#include "version.h"

#include <HydraIRCCommon.h>

#include <HydraIRCPlugin.h>
#include <API_Basic.h>
#include <API_CommandProcessor.h>
#include <API_GUI.h>

#include "HydraIRCTypes.h"
#include "SingleInstance.h"
#include "PluginManager.h"
#include "irc.h"
#include "resource.h"
#include "Socket.h"
#include "Identd.h"
#include "DNSResolver.h"
#include "Utility.h"
#include "Listener.h"
#include "EventManager.h"
#include "CLogger.h"
#include "Notification.h"
#include "AboutDlg.h"
#include "Prefs.h"
#include <FixedMDIChild.h>
#include "HeaderView.h"
#include "InfoView.h"
#include "MsgView.h"
#include "Output.h"
#include "ChannelMonitor.h"
#include "ServerMonitor.h"
#include "UserList.h"
#include "TextInputView.h"
#include "ChildCommon.h"
#include "TransfersFrm.h"
#include "ServerList.h"
//#include "PrefsManager.h"
#include "Common.h"
#include "TextQueueFrm.h"
#include "TextQueue.h"
#include "EventLog.h"
#include "EventLogFrm.h"
#include "Query.h"
#include "ChannelPropertiesDlg.h"
#include "ChannelListDlg.h"
#include "Channel.h"
#include "Server.h"
#include "DCCTransfer.h"
#include "Transfers.h"
#include "GUIHelpers.h"
#include "AddNetworkDlg.h"
#include "ConnectDlg.h"
#include "FavoritesDlg.h"
#include "EditNotificationDlg.h"
#include "EditBuddyGroupDlg.h"
#include "EditServerDetailsDlg.h"
#include "UserIdentityDlg.h"
#include "PrefsPage.h"
#include "Prefs_IdentitiesPage.h"
#include "Prefs_HighlightingPage.h"
#include "Prefs_NotificationsPage.h"
#include "Prefs_BuddyGroupsPage.h"
#include "Prefs_MiscPage.h"
#include "Prefs_LoggingPage.h"
#include "Prefs_MessagesPage.h"
#include "Prefs_CommandProfilesPage.h"
// TODO: Finish Filters Page
// #include "Prefs_FiltersPage.h"
#include "PrefsDlg.h"
#include "HidingMDIWindow.h"
#include "MainFrm.h"
#include "ChildFrm.h"

extern IRCServerDetails *g_DefaultServerDetails;

extern CSimpleArray<IRCServer*> g_ServerList;
extern CSimpleArray<IRCChannel*> g_ChannelList;
extern CSimpleArray<IRCQuery*> g_QueryList;
extern CSimpleArray<IRCServerDetails*> g_ServerDetailsList;
extern CSimpleArray<IRCNetworkDetails*> g_NetworkList;
extern CSimpleArray<CChildCommon *> g_ChildWndList;
extern ObjectManager g_Fonts;

// Functions defined in HydraIRC.cpp
void SetupFonts( void );
// CChildFrame *FindChildFrame(HWND hWnd); // depreciated
CChildCommon *FindChildWindow(HWND hWnd);
CChildCommon *FindChildWindowByID(int WindowID);
IRCServer *FindIRCServer(HWND hWnd,BOOL IncludeChannels = TRUE, BOOL IncludeQueries = TRUE); // gets the IRCServer from a channel/server window
IRCChannel *FindIRCChannel(HWND hWnd); // gets the IRCChannel from a channel window
IRCChannel *FindIRCChannelByName(char *Name);
IRCServer *FindDCCChat(HWND hWnd);
IRCServer *FindDCCChat(IRCServer *pOriginatingServer, char *Nick);
IRCQuery *FindIRCQuery(HWND hWnd);
IRCServer *FindServerOnNetwork(int NetworkID);
IRCServer *FindServerByName(char *Name, int port = -1);
IRCServer *FindServerByID(int ServerID);
IRCChannel *FindChannelByID(int ChannelID);
int GetConnectedServerCount( void );
void sys_Printf(int Contents, const char *format, ...); // this logs to a console log too (if console logging enabled)
void sys_Log(const int Contents, const char *buf);
void sys_Logf(const int Contents, const char *format, ...);
char *stripcrlf( char *str );
char *stripwhitespace( char *str );
char *strippathseparator( char *str );
char *stripcodes(char *buf);
BOOL CreatePath( char *Path);
char *UseEmptyString(char *string);
char *AllocEmptyString(char *string);
char *CopyWindowText(CWindow *Wnd,char * &buffer);
char *GetTimestamp(struct tm *t = NULL, char *formatstr = NULL);
char *GetOutputThemeFormatStr(OutputThemeItem_t *OutputTheme,int IRCCommandID, int TypeFlags);
void ReplaceVariables(char *FormatBuffer, const int MaxBufferSize, const char *FormatStr, const char **VariableNames, const char **VariableList, const int VariableCount);
BOOL ischannelstartchar( char c );
BOOL isvalidchannelname( char *name );
BOOL ischannelchar( char c );
void UpdateActivePrefs( void );
char *GetNetworkName(int ID);
IRCNetworkDetails *GetNetworkDetails(int ID);
HICON GetAppIcon(int IconID); // IconID, not IconObjectID!
HFONT GetAppFont(int PrefID); // PrefID, not FontObjectID!
int GetFontMaxHeight(HFONT hFont);
