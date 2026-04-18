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
