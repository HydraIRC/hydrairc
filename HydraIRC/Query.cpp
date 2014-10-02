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

// Query.h : implementation of the IRCChannel class
//
/////////////////////////////////////////////////////////////////////////////

/*
The order of channel creation:
1. user issues /join #blah
2. IRCServer creates IRCChannel class instance (if it's not already created one for that channel name)
3. IRCChannel sends command to server
4. IRCChannel waits for response from server
5. Depending on server repsonse, class is deleted or we carry on..
6. IRCChannel gets channel information (user list, etc..)
7. IRCChannel builds GUI.
*/

#include "stdafx.h"
#include "HydraIRC.h"


char *g_QueryStatusNames[3] = 
{
  "Initialising",
  "Connected",
  "Disconnected",
};

void IRCQuery::Initialise( void )
{
  SetDirtyFilterList(g_DefaultFilterList_DirtyStatusQuery);

  m_QueryID = g_IDManager.ObtainID();

  m_ObjectType        = OTYPE_QUERY;
  m_Status            = QRY_STATE_INITIALISING;
  m_pChildWnd         = NULL;
  m_DescriptionString = NULL;
#ifdef DEBUG
  sys_Printf(BIC_INFO,"Created new IRCQuery instance: \"%s\"\n",m_OtherNick);
#endif

  // add to the server list window
  g_pMainWnd->m_ServerListView.Add(this);

  if (!BOOLPREF(PREF_bCreateQueryWindowsHidden))
  {
    ShowQueryWindow();
  }
  else
  {
    SetDirtyStatus(DIRTY_RECENT | DIRTY_TRUE);
  }

  char *message = HydraIRC_BuildString(128,"New Query from %s",m_OtherNick);
  
  if (message)
  {
    g_pMainWnd->SetStatusBar(STATUS_ICON_INFORMATION,message);
    free(message);
  }

  UpdateLogger();
  
  OnConnect();
}

void IRCQuery::ShowQueryWindow( void )
{
  if (m_pChildWnd)
  {
    m_pChildWnd->ActivateWindow();
    //::ShowWindow(m_pChildWnd->m_hWnd,SW_SHOW);
    //g_pMainWnd->MDIActivate(m_pChildWnd->m_hWnd);    
  }
  else
  {
    // TODO: I guess some error checking would be nice.
    m_pChildWnd = new CChildFrame(&g_EventManager, m_pServer,this);
    // create the window
//* <gliptic> Drop files in query window
    //m_pChildWnd->CreateEx(g_pMainWnd->m_hWndMDIClient);
    m_pChildWnd->CreateEx(g_pMainWnd->m_hWndMDIClient, NULL, NULL, 0, CMDIChildWinTraits::GetWndExStyle(0) | WS_EX_ACCEPTFILES);
// </gliptic>

    g_ChildWndList.Add(m_pChildWnd);
  }
}

IRCQuery::IRCQuery(IRCServer *Server,const char *OtherNick, const char *Hostmask)
{
  CListener::Start(&g_EventManager);
  m_pServer = Server;
  m_OtherNick = strdup(OtherNick);
  m_Hostmask = strdup(Hostmask);
  Initialise();
}

IRCQuery::~IRCQuery( void )
{
#ifdef DEBUG
  sys_Printf(BIC_FUNCTION,"Deleting Class Instance: IRCQuery %s\n",m_OtherNick);
#endif

  if (m_pServer->m_pChildWnd && m_pServer->m_pChildWnd->IsWindow())
    ::PostMessage(m_pServer->m_pChildWnd->m_hWnd,WM_UPDATEINFOPANE, (WPARAM)0, (LPARAM)NULL);

  if (m_pChildWnd && m_pChildWnd->IsWindow())
    g_pMainWnd->MDIDestroy(m_pChildWnd->m_hWnd);

  m_pChildWnd = NULL;

  g_pMainWnd->m_ServerListView.Remove(this);
  if (m_OtherNick) free(m_OtherNick);
  if (m_Hostmask) free(m_Hostmask);
  if (m_DescriptionString) free(m_DescriptionString);
  m_pServer->m_QueryList.Remove(this);

  g_QueryList.Remove(this);
  g_IDManager.RelinquishID(m_QueryID);
  ClearOutputBuffer();
}

void IRCQuery::OnDisconnect( void )
{
  if (g_pMainWnd->IsWindowVisible()) // TODO: remove this workaround for server instance destruction
  {
    if (m_Status != QRY_STATE_DISCONNECTED)
      if (m_pChildWnd && m_pChildWnd->IsActive())
      {
        char *VarStr = HydraIRC_BuildString(512,g_DefaultStrings[DEFSTR_Server_Disconnected],UseEmptyString(GetTimestamp()));
        Printf(BIC_CONNECTING,UseEmptyString(VarStr));
        if (VarStr) free(VarStr);
      }
  }
  m_Status = QRY_STATE_DISCONNECTED;
  UpdateStatus();  
}

void IRCQuery::OnConnect( void )
{
  switch (m_Status)
  {
    case QRY_STATE_DISCONNECTED:
      if (m_pChildWnd && m_pChildWnd->IsActive())
        Printf(BIC_CONNECTING,"*** Connected");
      break;
    case QRY_STATE_INITIALISING:
      Printf(BIC_CONNECTING,"*** Now talking to %s",m_OtherNick);
      m_pServer->Send("WHOIS %s\r\n",m_OtherNick);
      break;
  }
  m_Status = QRY_STATE_CONNECTED;
  UpdateStatus(); 
}

void IRCQuery::UpdateStatus( void )
{
  BuildDescriptionString();  
  if (m_pChildWnd)
  {
    CHILDFRAMEPTR(m_pChildWnd)->UpdateStatus();
    CHILDFRAMEPTR(m_pChildWnd)->m_HeaderView.UpdateHeader(m_DescriptionString);    
    UpdateInfo();
  }

  ::PostMessage(m_pServer->m_pChildWnd->m_hWnd,WM_UPDATEINFOPANE, (WPARAM)0, (LPARAM)NULL);
  g_pMainWnd->m_ServerListView.Update(this);
}

char *IRCQuery::BuildDescriptionString( void )
{
  if (m_DescriptionString) 
    free (m_DescriptionString);

  m_DescriptionString = HydraIRC_BuildString(1024,"%s - %s (%s)",
          m_OtherNick,
          m_Hostmask ? m_Hostmask : "Hostmask Unknown",
          g_QueryStatusNames[m_Status]);

  return m_DescriptionString;
}

char *IRCQuery::GetDescriptionString( void )
{
  if (!m_DescriptionString)
  {
    return BuildDescriptionString();
  }
  return m_DescriptionString;
}

// update the user lists..
void IRCQuery::ChangeUserNick(char *OtherNick)
{
  if (m_OtherNick) free(m_OtherNick);
  m_OtherNick = strdup(OtherNick);
  UpdateStatus();  
}

void IRCQuery::OnEvent(int EventID, void *pData)
{
  switch(EventID)
  {
    case EV_PREFSCHANGED:
      UpdateLogger();
      break;
  }
}


void IRCQuery::UpdateLogger( void )
{
  DWORD LoggingFlags = LOGF_NONE;
  char *Folder;

  if (BOOLPREF(PREF_bCreateNetworkLogFolder))
  {    
    Folder = HydraIRC_BuildString(MAX_PATH,"%s\\%s",PATHPREF(PREF_pLogRootFolder),GetNetworkName(m_pServer->m_pDetails->m_NetworkID));
  }
  else
  {
    Folder = strdup(PATHPREF(PREF_pLogRootFolder));
  }
  m_Logger.SetPaths(Folder,HydraIRC_BuildString(MAX_PATH,"Query_%s.txt",m_OtherNick));

  if (BOOLPREF(PREF_bLogging) && BOOLPREF(PREF_bQueryLogging)) LoggingFlags |= LOGF_ENABLED;
  if (BOOLPREF(PREF_bStripCodesFromQueryLogs)) LoggingFlags |= LOGF_STRIPCODES;

  // update the settings and start logging!
  m_Logger.UpdateSettings(LoggingFlags);
}

// all output to a query goes through this
// to print directly to a query window, use IRCServer::Printf()
// for all normal output use this IRCServer::OutputFormatter()
// the output formatter takes a given input and creates logfile
// output AND window output which are then passed to m_Logger and Printf()
// the contents is also passes though the theme engine too
#define FORMAT_BUFFER_SIZE 1024

void IRCQuery::OutputFormatter(int Contents,int IRCCommandID)
{
  char *FormatStr = GetOutputThemeFormatStr(m_pTheme->m_ThemeItems,IRCCommandID,THEME_QUERY);

  if (!FormatStr)
    return;

  char *FormatBuffer = (char *)malloc(FORMAT_BUFFER_SIZE);

  ReplaceVariables(FormatBuffer,FORMAT_BUFFER_SIZE,FormatStr,g_VariableIDNames,m_pServer->m_Variables,VID_LAST);

  m_pServer->m_Variables[VID_QUERYOUTPUT] = FormatBuffer;

  // log the output
  if (m_Logger.IsEnabled() && !m_Logger.IsFiltered(Contents))
  {
    char LogFormatBuffer[1024];
    char *LogFormatStr = STRINGPREF(PREF_sQueryLogFormat);
    ReplaceVariables(LogFormatBuffer,sizeof(LogFormatBuffer),LogFormatStr,g_VariableIDNames,m_pServer->m_Variables,VID_LAST);

    m_Logger.Log(LogFormatBuffer);
  }

  AddToOutputBuffer(Contents,FormatBuffer);
}

void IRCQuery::UpdateInfo( void )
{
  if (!m_pChildWnd || !m_pChildWnd->IsWindow() || !CHILDFRAMEPTR(m_pChildWnd)->m_InfoView.IsWindow() )
    return;

  CString Message;

  switch(m_Status)
  {
    case QRY_STATE_CONNECTED:
      Message.Format("Chatting to %s",m_OtherNick);
      break;
    case QRY_STATE_DISCONNECTED:
      Message.Format("Disconnected!");
      break;
  }

  struct tm *then;
  then = localtime( &m_LastActivityTime );
  char *TimeStamp = GetTimestamp(then);
  Message.AppendFormat("\nLast activity at %s",TimeStamp);

  
  CHILDFRAMEPTR(m_pChildWnd)->m_InfoView.Put(Message);
}
