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

// Channel.h : implementation of the IRCChannel class
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

// forward declare
CChildCommon *FindChildWindow(HWND hWnd);


char *g_ChannelStatusNames[7] =
{
  "Initialising",
  "Joining",
  "Joined",
  "Disconnected",
  "Kicked",
  "Banned",
  "Error"
};

void IRCChannel::Join( void )
{
  ClearUserList();
  m_HaveNames = FALSE;


  // send command to server
  m_Status = CHN_STATE_CONNECTING;
  m_pServer->Send("JOIN %s %s\n",m_pProperties->m_Name,UseEmptyString(m_pProperties->m_Key));
  UpdateStatus();
}

void IRCChannel::InitialiseGUI( void )
{
  if (!m_pChildWnd)
  {
    // add to the server list window
    g_pMainWnd->m_ServerListView.Add(this);

    // TODO: I guess some error checking would be nice.
    m_pChildWnd = new CChildFrame(&g_EventManager, m_pServer,this);
    CHILDFRAMEPTR(m_pChildWnd)->m_UserListView.SetSource(this);
    // create the window
	  m_pChildWnd->CreateEx(g_pMainWnd->m_hWndMDIClient);

    g_ChildWndList.Add(m_pChildWnd);

  }
}

void IRCChannel::Initialise( void )
{
  SetDirtyFilterList(g_DefaultFilterList_DirtyStatusChannel);

  m_ObjectType        = OTYPE_CHANNEL;
  m_ChannelID         = g_IDManager.ObtainID();
  m_Status            = CHN_STATE_INITIALISING;
  m_JoinErrorReason   = 0;
  m_Flags             = CHN_FLAG_NONE;
  m_KickedBy          = NULL;
  m_KickReason        = NULL;
  m_BannedBy          = NULL;
  m_pChildWnd         = NULL;
  m_HaveNames         = FALSE;
  m_DescriptionString = NULL;
  m_pPropertiesDlg    = NULL;
#ifdef DEBUG
  sys_Printf(BIC_INFO,"Created new IRCChannel instance: \"%s\"\n",m_pProperties->m_Name);
#endif

  //m_HasUserList       = BOOLPREF(PREF_bChannelWindowUserList);
  if (BOOLPREF(PREF_bMonitorChannelsOnJoin))
  {
    g_pMainWnd->m_ChannelMonitorView.AddChannel(this);
  }

  UpdateLogger();
}

void IRCChannel::OnJoin( void )
{
  m_Status = CHN_STATE_CONNECTED;
  m_JoinErrorReason = 0; // reset join failure reason.

  // delete any pending rejoins
  g_EventManager.DeleteEvents((CListener *)this,EV_CH_RETRYJOIN);

  // Query the channel modes, we will then remember the server's reply to this.
  m_pServer->Send("MODE %s\r\n",m_pProperties->m_Name);

  UpdateStatus();
  m_pServer->UpdateInfo();

  /* TODO: changes in 0.338 removed this bit,  not sure if we actually still need it...
  if (g_pMainWnd->m_pUserListView && g_pMainWnd->m_pUserListView->m_pChannel == this)
  {
    g_pMainWnd->UpdateGlobalUserList(this);
  }
  */

  // only refresh the global user list if this is the active MDI window
  // this helps when rejoining lots of channels with lots of users after a reconnect
  CChildFrame *pChildWnd = CHILDFRAMEPTR(FindChildWindow(g_pMainWnd->MDIGetActive()));
  if (pChildWnd && pChildWnd->m_WindowType == CWTYPE_CHANNEL && pChildWnd->m_pChannel == this)
  {
    g_pMainWnd->UpdateGlobalUserList(this,FALSE,TRUE);
  }

  // TODO: this bit is temporary, eventually you'll be able to assign command profiles
  // to networks and servers.

  if(GetAsyncKeyState(VK_SHIFT) & 0x8000)
  {
    Printf(BIC_ERROR,"SHIFT key pressed - Not running 'OnJoin' command profile");
  }
  else
  {
    // exec the <network>_OnJoin script
    char *CommandProfileName = HydraIRC_BuildString(512,"%s_%s_OnJoin",GetNetworkName(m_pServer->m_pDetails->m_NetworkID),m_pProperties->m_Name);
    char *Command = HydraIRC_BuildString(512,"/exec %s",UseEmptyString(CommandProfileName));

    if (m_pChildWnd && Command && CommandProfileName && g_pPrefs->FindCommandProfile(CommandProfileName))
      CHILDFRAMEPTR(m_pChildWnd)->ProcessTextInput(Command,TRUE);

    if (Command) free(Command);
    if (CommandProfileName) free(CommandProfileName);
  }

  // End of temporary bit

}

IRCChannel::IRCChannel(IRCServer *Server,char *ChannelName, char *Key)
{
  m_pProperties = new CChannelProperties();
  CListener::Start(&g_EventManager);
  m_pServer = Server;
  m_pProperties->SetName(ChannelName);
  m_pProperties->SetKey(Key); // Key can be null.
  Initialise();
}

IRCChannel::~IRCChannel( void )
{
#ifdef DEBUG
  sys_Printf(BIC_FUNCTION,"Deleting Class Instance: IRCChannel %s\n",m_pProperties->m_Name);
#endif

  if (m_pServer->m_pChildWnd && m_pServer->m_pChildWnd->IsWindow())
    ::PostMessage(m_pServer->m_pChildWnd->m_hWnd,WM_UPDATEINFOPANE, (WPARAM)0, (LPARAM)NULL);

  if (m_Status == CHN_STATE_CONNECTED)
    m_pServer->CMD_Part(m_pProperties->m_Name,NULL);

  if (m_pChildWnd && m_pChildWnd->IsWindow())
    g_pMainWnd->MDIDestroy(m_pChildWnd->m_hWnd);

  m_pChildWnd = NULL;

  g_pMainWnd->UpdateGlobalUserList(this,TRUE);

  ClearUserList();
  m_HaveNames = FALSE;


  g_pMainWnd->m_ChannelMonitorView.RemoveChannel(this);
  g_pMainWnd->m_ServerListView.Remove(this);
  if (m_DescriptionString) free(m_DescriptionString);
  m_pServer->m_ChannelList.Remove(this);
  if (m_pProperties)
    delete m_pProperties;

  g_ChannelList.Remove(this);
  g_IDManager.RelinquishID(m_ChannelID);
  ClearOutputBuffer();
  SetString(m_KickedBy,NULL);
  SetString(m_KickReason,NULL);
  SetString(m_BannedBy,NULL);
}

void IRCChannel::AddUser(IRCUser *pUser)
{
  if (!pUser)
    return;

  int UserGroup = g_pPrefs->m_CustomUserGroups.FindUser(pUser->m_Nick, m_pServer->m_pDetails->m_NetworkID);

  if (UserGroup != -1)
    pUser->m_CustomGroupID = UserGroup;
  m_Users.AddUser(pUser);
  // add to userlist view
  if (m_pChildWnd)
  {
    if (CHILDFRAMEPTR(m_pChildWnd))
    {
      CHILDFRAMEPTR(m_pChildWnd)->m_UserListView.Add(pUser);
    }
    if (g_pMainWnd->m_UserListView.m_pChannel == this)
    {
      g_pMainWnd->m_UserListView.Add(pUser);
    }
  }

}

void IRCChannel::AddUser(char *User, char *Host)
{
  if (!User || !*User)
    return;

  stripwhitespace(User);
  if (!*User)
    return;

  __int64 usermode = IRCMODE_NORMAL;

  while(*User)
  {

    char mode = m_pServer->GetModeForPrefix(*User);

    if(mode == ' ')
      break;

    usermode |= IRCMODE(mode);

    User++;
  }

  IRCUser *pUser = new IRCUser(User, Host, usermode);
  // add user to channel list
  if (pUser)
  {
    AddUser(pUser);
  }
}

// split a ' ' seperated list of users and add to the user list.
void IRCChannel::AddUsers(char *Users)
{
  if (!Users || !*Users)
    return;

  char *user = strtok(Users," ");

  while (user)
  {
    AddUser(user,NULL);
    user = strtok(NULL," ");
  }
}

void IRCChannel::RemoveUser( char *User )
{
  IRCUser *pUser = m_Users.FindUser(User);
  RemoveUser(pUser);
}

void IRCChannel::RemoveUser( IRCUser *pUser )
{
  if (!pUser)
    return;

  m_Users.RemoveUser(pUser);
  if (m_pChildWnd)
  {
    CHILDFRAMEPTR(m_pChildWnd)->m_UserListView.Remove(pUser);
  }
  if (g_pMainWnd->m_UserListView.m_pChannel == this)
  {
    g_pMainWnd->m_UserListView.Remove(pUser);
  }
  delete pUser;
}

void IRCChannel::OnDisconnect( void )
{
  if (g_pMainWnd->IsWindowVisible()) // TODO: remove this workaround for channel/server instance destruction
  {
    if (m_Status != CHN_STATE_DISCONNECTED)
    {
      char *VarStr = HydraIRC_BuildString(512,g_DefaultStrings[DEFSTR_Server_Disconnected],UseEmptyString(GetTimestamp()));
      Printf(BIC_CONNECTING,UseEmptyString(VarStr));
      if (VarStr) free(VarStr);
    }
  }
  m_Status = CHN_STATE_DISCONNECTED;
  UpdateStatus();

}

void IRCChannel::ReJoin( void )
{
  // don't try and rejoin if we're already trying to join.
  // this happens if your OnLoggedIn command profiles join a channel
  // and then ReJoin is called by IRCServer::OnLoggedIn()

  if (m_Status == CHN_STATE_CONNECTING)
    return;

  Printf(BIC_CONNECTING,"*** Attempting to re-join");
  // wait for a new topic to be sent, if there is one
  m_pProperties->SetTopic(NULL);
  Join();
}

void IRCChannel::OnKicked( char *By, char *Reason )
{
  SetString(m_KickedBy,By);
  SetString(m_KickReason,Reason);
  m_Status = CHN_STATE_KICKED;
  UpdateStatus();

  if (BOOLPREF(PREF_bRejoinChannelOnKick))
  {
    if (INTPREF(PREF_KickRejoinDelay) > 0)
      g_EventManager.QueueTimedEvent((CListener *)this,FALSE,EV_CH_RETRYJOIN,INTPREF(PREF_KickRejoinDelay));
    else
      ReJoin();
  }
}

void IRCChannel::OnBanned( void )
{
  m_Status = CHN_STATE_BANNED;
  UpdateStatus();
  // TODO, add option to auto-reconnect, and to reconnect after a delay.
}

void IRCChannel::OnJoinError( int Reason )
{
  m_JoinErrorReason = Reason;
  m_Status = CHN_STATE_ERROR;
  UpdateStatus();

  // TODO, add more prefs to auto-reconnect, and to reconnect after a delay.

  switch (Reason)
  {
    case ERR_UNAVAILRESOURCE:
      // start a timer here, and then rejoin later.
      g_EventManager.QueueTimedEvent((CListener *)this,FALSE,EV_CH_RETRYJOIN,INTPREF(PREF_nChannelRejoinDelay));
      break;
  }
}

void IRCChannel::OnCycle( void )
{
  UpdateStatus();
  m_Flags &= ~ CHN_FLAG_CYCLING;
  ReJoin();
}

void IRCChannel::Cycle( void )
{
  if (m_Status == CHN_STATE_CONNECTED)
  {
    m_Flags |= CHN_FLAG_CYCLING;
    m_pServer->CMD_Part(m_pProperties->m_Name, "Cycling Channel");
  }
  else
  {
    // We're not joined, attempt a join.
    ReJoin();
  }
}
void IRCChannel::UpdateStatus( void )
{
  BuildDescriptionString();
  if (m_pChildWnd)
  {
    CHILDFRAMEPTR(m_pChildWnd)->UpdateStatus();
    CHILDFRAMEPTR(m_pChildWnd)->m_HeaderView.UpdateHeader(m_pProperties->m_Topic);
    UpdateInfo();
  }

  ::PostMessage(m_pServer->m_pChildWnd->m_hWnd,WM_UPDATEINFOPANE, (WPARAM)0, (LPARAM)NULL);
}

// topic can be null, in which case the current topic is removed.
void IRCChannel::SetTopic( char *Topic )
{
  m_pProperties->SetTopic(Topic);
  UpdateStatus();
}

char *IRCChannel::BuildDescriptionString( void )
{
  if (m_DescriptionString) free (m_DescriptionString);

  char *topicstr = NULL;

  if (m_pProperties->m_Topic)
  {
    topicstr = stripcodes(strdup(m_pProperties->m_Topic));
  }

  if (!topicstr)
    topicstr = "Topic is not set";

  // TODO: include join failure reason code in status string
  int len;

  len = strlen(m_pProperties->m_Name) + 2 +                          // "name ("
        strlen(g_ChannelStatusNames[m_Status]) + 4 +  // "status) - "
        strlen(topicstr) + 1;                          // "topic\0"
  m_DescriptionString = (char *) malloc(len);

  sprintf(m_DescriptionString, "%s (%s) - %s",
          m_pProperties->m_Name,
          g_ChannelStatusNames[m_Status],
          topicstr);
  //len = strlen(m_DescriptionString); // double check length
  if (m_pProperties->m_Topic) free(topicstr); // yes, not topicstr, look at the init.
  return m_DescriptionString;
}

char *IRCChannel::GetDescriptionString( void )
{
  if (!m_DescriptionString)
  {
    return BuildDescriptionString();
  }
  return m_DescriptionString;
}

// update the user lists..
void IRCChannel::UpdateUser(IRCUser *pUser)
{
  if (m_pChildWnd)
  {
    CHILDFRAMEPTR(m_pChildWnd)->m_UserListView.Update(pUser);
  }
  if (g_pMainWnd->m_UserListView.m_pChannel == this)
  {
    g_pMainWnd->m_UserListView.Update(pUser);
  }
}


void IRCChannel::ChangeUserNick(IRCUser *pUser, char *NewNick)
{
  free(pUser->m_Nick);
  pUser->m_Nick = strdup(NewNick);
  UpdateUser(pUser);
}

void IRCChannel::ChangeChannelMode(char *NewMode, char *Param)
{
  BOOL Failed = FALSE;

  if (NewMode[0] == '+')
  {
    switch (NewMode[1])
    {
      case 'i':
        m_pProperties->m_Modes |= CHANNELMODE_INVITE;
        break;
      case 'k':
        m_pProperties->m_Modes |= CHANNELMODE_KEY;
        m_pProperties->SetKey(Param);
        break;
      case 'l':
        m_pProperties->m_Modes |= CHANNELMODE_LIMIT;
        if (Param)
          m_pProperties->m_Limit = atoi(Param);
        break;
      default:
        Failed = TRUE;
    }
  }
  else if (NewMode[0] == '-')
  {
    switch (NewMode[1])
    {
      case 'i':
        m_pProperties->m_Modes &= ~CHANNELMODE_INVITE;
        break;
      case 'k':
        m_pProperties->m_Modes &= ~CHANNELMODE_KEY;
        break;
      case 'l':
        m_pProperties->m_Modes &= ~CHANNELMODE_LIMIT;
        break;
      default:
        Failed = TRUE;
    }
  }

  if (Failed)
  {
    sys_Printf(BIC_INFO,"Unimplemented channel mode '%c'\n",NewMode[1]);
  }

}

void IRCChannel::ChangeUserMode(IRCUser *pUser, char *NewMode)
{
#ifdef DEBUG
  sys_Printf(BIC_INFO,"Processing mode change for user: \"%s\", New mode: \"%s\"\n",pUser->m_Nick,NewMode);
#endif

  BOOL Failed = FALSE;
  IRCUser *pNewUser = new IRCUser(pUser);

  if (NewMode[0] == '+')
  {
    pNewUser->m_UserModes |= IRCMODE(NewMode[1]);
  }
  else
  {
    pNewUser->m_UserModes &= ~IRCMODE(NewMode[1]);
  }

  pNewUser->SetTypeFromModes();

  if (Failed) // impossible to fail now. woohoo!
  {
    sys_Printf(BIC_INFO,"Unimplemented user mode '%c'\n",NewMode[1]);
    delete pNewUser;
  }
  else
  {
    RemoveUser(pUser);
    AddUser(pNewUser);
  }
}


BOOL IRCChannel::CompleteNick(char *Nick, char **Dest, BOOL Forwards, BOOL AllowNext)
{
  if (!Nick /*|| !*Nick*/ || Dest == NULL)
    return FALSE;

  // TODO: make this check to see if the first few characters of the Nick are
  // part of a valid nick, at the moment supplying "a" and will give back "Barney"
  // (if barney is a user and there were no users beginning with a)
  // it should really just leave dest alone and return FALSE
  // that is, if we want it to work the same as other nick completers
  // but I actually kinda like what it does, so maybe I'll leave it...

  // Going backwards is fun!

  CList NickList;
  CNode *pNewNode,*pNode;
  int i;
  BOOL Added;
  BOOL result = FALSE;

  // Build a list of nicks, sorting as we go..
  // TODO: better sort algorithm needed? check performance on a 1000+ user channel
  for (i = 0 ; i < m_Users.m_UserList.GetSize() ; i ++)
  {
    Added = FALSE;
    pNewNode = new CNode(m_Users.m_UserList[i]->m_Nick);

    if (i==0)
    {
      NickList.AddHead(pNewNode);
    }
    else
    {
      for (pNode = NickList.GetFirst() ; /*pNode && */pNode->m_Next && !Added; pNode = pNode->m_Next)
      {
        if ((Forwards && (stricmp(m_Users.m_UserList[i]->m_Nick,(char *)pNode->m_Data) < 0)) ||
            (!Forwards && (stricmp(m_Users.m_UserList[i]->m_Nick,(char *)pNode->m_Data) > 0)) )
        {
          NickList.InsertBefore(pNode,pNewNode);
          Added = TRUE;
        }
      }
      if (!Added)
        NickList.AddTail(pNewNode);
    }
  }

  int compare;
  for (pNode = NickList.GetFirst() ; /*pNode &&*/ pNode->m_Next && !result; pNode = pNode->m_Next)
  {
    compare = stricmp(Nick,(char *)pNode->m_Data);

    if (compare == 0 && !AllowNext)
    {
      *Dest = strdup((char *)pNode->m_Data);
      result = TRUE;
    }
    if (compare < 0 && Forwards)
    {
      *Dest = strdup((char *)pNode->m_Data);
      result = TRUE;
    }
    if (compare > 0 && !Forwards)
    {
      *Dest = strdup((char *)pNode->m_Data);
      result = TRUE;
    }

  }

  while (!NickList.IsEmpty())
  {
    (NickList.GetFirst())->Delete();
  }

  return result;
}

void IRCChannel::DoPropertiesDlg( void )
{
  CChannelProperties *pNewProperties = new CChannelProperties(m_pProperties);

  m_pPropertiesDlg = new CChannelPropertiesDlg(this,pNewProperties);
  if (m_pPropertiesDlg)
  {
	  int result = m_pPropertiesDlg->DoModal();
    if (result == 1)
    {
      // Compare the new channel properties to the current channel
      // propererties and issue the commands to the server if required

      // TODO: if i could be arsed, I'd join some of the mode commands
      // together so that we send less seperate MODE commands to the server
      // e.g. instead of sending "MODE #test +k key" and then
      // "MODE #test +l 30", we could just send "MODE #test +kl key 30"

      // Heh, bring on the complicated if statements! :)

      // Topic changed ?
      // ((don't have old topic AND have new topic) OR (old topic AND new topic AND they're different))
      // Note: there is a third case that we ignore, which is "have old topic, don't have new topic"
      if ((!m_pProperties->m_Topic && pNewProperties->m_Topic) ||
          (m_pProperties->m_Topic && pNewProperties->m_Topic && stricmp(m_pProperties->m_Topic,pNewProperties->m_Topic) != 0))
      {
        m_pServer->Send("TOPIC %s :%s\r\n",m_pProperties->m_Name,pNewProperties->m_Topic);
      }

      // Key changed ?
      if (pNewProperties->m_Key && // must have a new key regardless!
          ((m_pProperties->m_Key && stricmp(m_pProperties->m_Key,pNewProperties->m_Key) != 0) ||
           ((m_pProperties->m_Modes & CHANNELMODE_KEY) != (pNewProperties->m_Modes & CHANNELMODE_KEY))))
      {
        m_pServer->Send("MODE %s %ck %s\r\n",m_pProperties->m_Name,(pNewProperties->m_Modes & CHANNELMODE_KEY) ? '+' : '-',pNewProperties->m_Key);
      }

      // Key changed ?
      if ((m_pProperties->m_Limit != pNewProperties->m_Limit) ||
          ((m_pProperties->m_Modes & CHANNELMODE_LIMIT) != (pNewProperties->m_Modes & CHANNELMODE_LIMIT)))
      {
        m_pServer->Send("MODE %s %cl %d\r\n",m_pProperties->m_Name,(pNewProperties->m_Modes & CHANNELMODE_LIMIT) ? '+' : '-',pNewProperties->m_Limit);
      }

      if ((m_pProperties->m_Modes & CHANNELMODE_INVITE) != (pNewProperties->m_Modes & CHANNELMODE_INVITE))
      {
        m_pServer->Send("MODE %s %ci\r\n",m_pProperties->m_Name,(pNewProperties->m_Modes & CHANNELMODE_INVITE) ? '+' : '-');
      }
    }

    delete m_pPropertiesDlg;
    m_pPropertiesDlg = NULL;
  }
  delete pNewProperties;
}

void IRCChannel::OnEvent(int EventID, void *pData)
{
  switch(EventID)
  {
    case EV_PREFSCHANGED:
    /* // prefscollections
      m_pPC = g_pPrefs->GetChannelPrefsCollection(m_pProperties->m_Name,m_pServer->m_pDetails->m_Network);
      if (m_pChildWnd)
        CHILDFRAMEPTR(m_pChildWnd)->UpdateChildFrameLayout(FALSE);
    */
      UpdateLogger();
      break;
    case EV_CH_RETRYJOIN:
      if (m_Status == CHN_STATE_ERROR || m_Status == CHN_STATE_KICKED)
        ReJoin();
      break;
  }
}

void IRCChannel::UpdateLogger( void )
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
  m_Logger.SetPaths(Folder,HydraIRC_BuildString(MAX_PATH,"Channel_%s.txt",m_pProperties->m_Name));

  if (BOOLPREF(PREF_bLogging) && BOOLPREF(PREF_bChannelLogging)) LoggingFlags |= LOGF_ENABLED;
  if (BOOLPREF(PREF_bStripCodesFromChannelLogs)) LoggingFlags |= LOGF_STRIPCODES;

  // update the settings and start logging!
  m_Logger.UpdateSettings(LoggingFlags);
}

// all output to a server goes through this
// to print directly to a server window, use IRCServer::Printf()
// for all normal output use this IRCServer::OutputFormatter()
// the output formatter takes a given input and creates logfile
// output AND window output which are then passed to m_Logger and Printf()
#define FORMAT_BUFFER_SIZE 1024

void IRCChannel::OutputFormatter(int Contents,int IRCCommandID)
{
  char *FormatStr = GetOutputThemeFormatStr(m_pTheme ? m_pTheme->m_ThemeItems : NULL,IRCCommandID,THEME_CHANNEL);

  if (!FormatStr)
    return;

  char *FormatBuffer = (char *)malloc(FORMAT_BUFFER_SIZE);

  ReplaceVariables(FormatBuffer,FORMAT_BUFFER_SIZE,FormatStr,g_VariableIDNames,m_pServer->m_Variables,VID_LAST);

  m_pServer->m_Variables[VID_CHANNELOUTPUT] = FormatBuffer;

  // log the output
  if (m_Logger.IsEnabled() && !m_Logger.IsFiltered(Contents))
  {
    char LogFormatBuffer[1024];
    char *LogFormatStr = STRINGPREF(PREF_sChannelLogFormat);
    ReplaceVariables(LogFormatBuffer,sizeof(LogFormatBuffer),LogFormatStr,g_VariableIDNames,m_pServer->m_Variables,VID_LAST);

    m_Logger.Log(LogFormatBuffer);
  }

  // Catch URLs (URLGrabber)
  char *Url; // only one part, don't define as an array - pass address to RegExpToArray() call (see below)
  int PartsCount;
  PartsCount = RegExpToArray(FormatBuffer,"^(.* )?{http([^ ])+}(.*)$",&Url,false);
  if (PartsCount == 1 && Url)
  {
    g_pUrlCatcherManager->AddItem(m_pServer->m_Variables[VID_UNPREFIXEDNICK], Url, this->m_pServer);
    free(Url);
  }

  // check for string matching filters.. (ignore stuff that we typed in..)
  if (Contents == BIC_CHANNELTEXT ||
      (
        // we don't need to compare the nick for BIC_CHANNELTEXT (as we have BIC_OWNTEXT for channels...)
        (Contents == BIC_QUERYTEXT || Contents == BIC_ACTION) &&
        (stricmp(m_pServer->m_Variables[VID_NICK],m_pServer->m_Variables[VID_MYNICK]))
        )
      )
  {
    int i;
    NotificationPrefInfo_t *pNPI;
    for (i = 0; i < g_pPrefs->m_NotificationPrefList.GetSize(); i ++)
    {
      pNPI = g_pPrefs->m_NotificationPrefList[i];
      if (pNPI->m_EventCode != NE_HIGHLIGHT || !pNPI->m_MatchString || (pNPI->m_Flags & NE_FLAG_DISABLED)) // be safe!
        continue;

      // check for buddy group matching!!
      IRCUserGroup *pGroup = NULL;
      if (pNPI->m_GroupID != -2)
        pGroup = g_pPrefs->m_CustomUserGroups.FindUserGroup(m_pServer->m_Variables[VID_UNPREFIXEDNICK],m_pServer->m_pDetails->m_NetworkID);

      if ((pNPI->m_GroupID != -2 && !pGroup) || (pGroup && pGroup->m_GroupID != pNPI->m_GroupID))
        continue;

      // we've got a highlight pref,  see if there's a match!

      char *RegExp;
      char *parts[3];
      char highlightcolors[14];
      int Count;

      *highlightcolors = 0;
      if (pNPI->m_ForegroundColor != -1)
        sprintf(highlightcolors,"%06x",RGBVALTOCOLORREF(pNPI->m_ForegroundColor));

      if (pNPI->m_BackgroundColor != -1)
        sprintf(highlightcolors+strlen(highlightcolors),",%06x",RGBVALTOCOLORREF(pNPI->m_BackgroundColor));


      // search for a non-alpha-numberic seperated word
      // in the following string we want to match the word "test"
      // "test"
      // "123:test:123"
      // "test yeah!"
      // "this is a test"
      // but we don't want it to match "testing 1 2 3" or "ttttesting"

      // this regexp is not correct, it doesn't correcly deal with ":test:" but it manges "abc test 123" ok..

#ifndef ALTREGEXPMETHOD
      RegExp = HydraIRC_BuildString(1024,"^{((.)* )?}{%s}{( (.)*)?}$",pNPI->m_MatchString);
      //RegExp = HydraIRC_BuildString(1024,"^{(.)*?!\\a?}{%s}{!\\a(.)*?}$",pNPI->m_MatchString);
      // some weird shit going on here..  regexps are a bit fucked i think.
      //RegExp = HydraIRC_BuildString(1024,"^{((.)*\\b)?}{%s}{(\\b(.)*)?}$",HighlightString);
#else
      char *HighlightString = pNPI->m_MatchString;

      if (!(pNPI->m_HighlightFlags & HIGHLIGHT_CASESENSITIVE))
      {
        HighlightString = strdup(HighlightString);
        if (!HighlightString)
          return;
        HighlightString = _strlwr(HighlightString);
      }

      RegExp = HydraIRC_BuildString(1024,"^{(.)*?!\\a?}{%s}{!\\a(.)*?}$",HighlightString);
      // some weird shit going on here..  regexps are a bit fucked i think.
      //RegExp = HydraIRC_BuildString(1024,"^{((.)*\\b)?}{%s}{(\\b(.)*)?}$",HighlightString);

      if (!(pNPI->m_HighlightFlags & HIGHLIGHT_CASESENSITIVE))
        free(HighlightString);
#endif

      if (!RegExp)
        continue;


      char *MatchString = strdup(FormatBuffer);
      BOOL Matched = FALSE;
      if (!MatchString)
      {
        free(RegExp);
        continue;
      }

      *FormatBuffer = 0;

      char *NewBuffer;

      // recursively replace all occurences of the text to be highlighted..
      BOOL Done = FALSE;
      do
      {
        Count = 0;

#ifndef ALTREGEXPMETHOD
        Count = RegExpToArray(MatchString,RegExp,parts, (pNPI->m_HighlightFlags & HIGHLIGHT_CASESENSITIVE) == HIGHLIGHT_CASESENSITIVE);
#else
        char *TestString = MatchString;

        if (!(pNPI->m_HighlightFlags & HIGHLIGHT_CASESENSITIVE))
        {
          TestString = strdup(TestString);
          if (!TestString)
            break;
          TestString = _strlwr(TestString);
        }
        Count = RegExpToArray(TestString,RegExp,parts);//, (pNPI->m_HighlightFlags & HIGHLIGHT_CASESENSITIVE) == HIGHLIGHT_CASESENSITIVE);
#endif

        if (Count > 0)
        {
          // we've got a match!
          Matched = TRUE;

#ifdef ALTREGEXPMETHOD
          if (!(pNPI->m_HighlightFlags & HIGHLIGHT_CASESENSITIVE))
          {
            int lengths[3];
            lengths[0] = strlen(parts[0]);
            lengths[1] = strlen(parts[1]);
            lengths[2] = strlen(parts[2]);
            strncpy(parts[0],MatchString,lengths[0]);
            strncpy(parts[1],MatchString+lengths[0],lengths[1]);
            strncpy(parts[2],MatchString+lengths[0]+lengths[1],lengths[2]);
            free(TestString);
          }
#endif

          if (pNPI->m_HighlightFlags & HIGHLIGHT_WHOLELINE)
          {
            NewBuffer = HydraIRC_BuildString(FORMAT_BUFFER_SIZE-1,"\007%s%s%s%s\007",highlightcolors,parts[0],parts[1],parts[2]);
            if (NewBuffer)
            {
              strncat(FormatBuffer,NewBuffer,(FORMAT_BUFFER_SIZE-1) - strlen(FormatBuffer));
              free(NewBuffer);
            }
            free(parts[0]);
            free(parts[1]);
            free(parts[2]);
            Done = TRUE; // don't need to recursively highlight stuff when highlight whole line..
          }
          else
          {

            NewBuffer = HydraIRC_BuildString(FORMAT_BUFFER_SIZE-1,"%s\007%s%s\007",parts[0],highlightcolors,parts[1]);
            if (NewBuffer)
            {
              strncat(FormatBuffer,NewBuffer,(FORMAT_BUFFER_SIZE-1) - strlen(FormatBuffer));
              free(NewBuffer);
            }
            free(parts[0]);
            free(parts[1]);
            free(MatchString);
            MatchString = parts[2]; // check the rest of the string too!

            // end of string? (empty match buffer?)
            if (!*MatchString)
            {
              free(MatchString);
              Done = TRUE;
            }
          }
        }
        else
        {
          // no more matches, add the remainder to the output buffer..
          strncat(FormatBuffer,MatchString,(FORMAT_BUFFER_SIZE-1) - strlen(FormatBuffer));
          free(MatchString);
          Done = TRUE;
        }

      } while (!Done);

      if (Matched)
      {
        g_pNotificationManager->DoNotification(pNPI);

        if (pNPI->m_Flags & NE_FLAG_LOGINEVENTLOG)
        {
          // add to event log
          g_pEventLogManager->BuildMessage(g_DefaultStrings[DEFSTR_Channel_Highlight],m_pServer->m_Variables[VID_UNPREFIXEDNICK],m_pServer->m_Variables[VID_ALL],m_pProperties->m_Name);
          g_pEventLogManager->AddItem(ELIT_CHANNEL, ELID_Highlight,m_pServer->m_Variables[VID_UNPREFIXEDNICK], NULL, m_pServer, this);
        }
      }

      free(RegExp);
    }
  }

  AddToOutputBuffer(Contents,FormatBuffer);
}

void IRCChannel::UpdateInfo( void )
{
  if (!m_pChildWnd || !m_pChildWnd->IsWindow() || !CHILDFRAMEPTR(m_pChildWnd)->m_InfoView.IsWindow() )
    return;

  CString Message;

  switch(m_Status)
  {
    case CHN_STATE_CONNECTING:
      Message.Format("Joining...");
      break;
    case CHN_STATE_CONNECTED:
      Message.Format("Joined");
      break;
    case CHN_STATE_DISCONNECTED:
      Message.Format("Not Joined!");
      break;
    case CHN_STATE_KICKED:
      Message.Format("Kicked by %s: %s",
        UseEmptyString(m_KickedBy),
        UseEmptyString(m_KickReason));
      break;
    case CHN_STATE_BANNED:
      Message.Format("Banned by %s\n(Kicked by %s: %s)",
        m_BannedBy ? m_BannedBy : "<Unknown>",
        UseEmptyString(m_KickedBy),
        UseEmptyString(m_KickReason));
      break;
    case CHN_STATE_ERROR:
      Message.Format("Not Joined!");
      break;
  }

  struct tm *then;
  then = localtime( &m_LastActivityTime );
  char *TimeStamp = GetTimestamp(then, "%H:%M:%S");
  Message.AppendFormat("\nLast activity at %s\n",TimeStamp);
  Message.AppendFormat("%d User(s)\n",m_Users.m_UserList.GetSize());

  time_t now;
  time (&now);

  double s = difftime(now,m_CreateTime);

  int		d, h, m;

  m = (int)(s / 60);
  s = s - (m * 60);
  h = m / 60;
  m = m - 60 * h;
  d= h / 24;
  h= h - d * 24;

  char *timestr=(char *)malloc(1024);
  if (timestr)
  {
    int i;

    i = _snprintf(timestr,1023,"Chatting for ");
    if (d > 0) i += _snprintf(timestr+i,1023-i,"%id ", d);
    if (h > 0)	i += _snprintf(timestr+i,1023-i,"%ih ", h);
    i += _snprintf(timestr+i,1023-i,"%02im %02.0fs", m, s);
    Message.Append(timestr);
    free(timestr);
  }

  CHILDFRAMEPTR(m_pChildWnd)->m_InfoView.Put(Message);
}
