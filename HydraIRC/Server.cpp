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

// ChildFrm.cpp : implementation of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"


IRCServerDetails *GetServerDetails(char *ServerName, unsigned short Port)
{
  if (ServerName && *ServerName)
  {
    int i;
    for (i = 0 ; i < g_ServerDetailsList.GetSize(); i++)
    {
      IRCServerDetails *pServerDetails = g_ServerDetailsList[i];

      if (stricmp(ServerName,pServerDetails->m_Name) == 0 && Port == pServerDetails->m_Port)
      {
        return pServerDetails;
      }
    }
  }
  return NULL; // return unknown.
}

BOOL UpdateServerDetails(IRCServerDetails *pServerDetails)
{
  // If the servername is in the list of servers and networks already
  // then a) update pServerDetails->m_NetworkID with the correct network
  // ID and b) update pServerDetails->m_Description with the
  // correct description
  // returns TRUE if details are updated.

  if (!pServerDetails)
    return FALSE;

  IRCServerDetails *pNewServerDetails = GetServerDetails(pServerDetails->m_Name, pServerDetails->m_Port);
  if (pNewServerDetails)
  {
    pServerDetails->m_NetworkID = pNewServerDetails->m_NetworkID;
    if (pServerDetails->m_Description) free(pServerDetails->m_Description);
    pServerDetails->m_Description = strdup(pNewServerDetails->m_Description);
    return TRUE;
  }
  return FALSE;
}


char *g_QuitMessages[] = 
{
  //"WOW! This IRC Client ownz! \002HydraIRC\002 -> http://www.hydrairc.com <-",
  "\002HydraIRC\002 -> http://www.hydrairc.com <- \\o/",
  //"\002HydraIRC\002 -> http://www.hydrairc.com <- Get \002hot chicks\002 here!",
  "\002HydraIRC\002 -> http://www.hydrairc.com <- Chicks dig it",
  //"The IRC Client of the Gods! -> http://www.hydrairc.com <- \002HydraIRC\002",
  //"\002HydraIRC\002 -> http://www.hydrairc.com <- irc client ownage!",
  "\002HydraIRC\002 -> http://www.hydrairc.com <- *\002I\002* use it, so it must be good!",
  "\002HydraIRC\002 -> http://www.hydrairc.com <- Nine out of ten l33t h4x0rz prefer it",
  "\002HydraIRC\002 -> http://www.hydrairc.com <- s0 d4Mn l33t |t'z 5c4rY!",
  //"\002HydraIRC\002 rocks! -> http://www.hydrairc.com <-", 
  "I love my \002HydraIRC\002 -> http://www.hydrairc.com <-",
  //"Like VS.net's GUI?  Then try \002HydraIRC\002 -> http://www.hydrairc.com <-",
  "\002HydraIRC\002 -> http://www.hydrairc.com <-",
  //"\002HydraIRC\002 -> http://www.hydrairc.com <- The dawn of a new IRC era",
  //"\002HydraIRC\002 -> http://www.hydrairc.com <- IRC has never been so good",
  "\002HydraIRC\002 -> http://www.hydrairc.com <- The professional IRC Client :D",
  //"\002HydraIRC\002 -> http://www.hydrairc.com <- Try something fresh",
  //"\002HydraIRC\002 -> http://www.hydrairc.com <- Leading Edge IRC",
  //"\002HydraIRC\002 -> http://www.hydrairc.com <- The future of IRC",
  //"\002HydraIRC\002 -> http://www.hydrairc.com <- State of the art IRC",
  "\002HydraIRC\002 -> http://www.hydrairc.com <- IRC with a difference",
  "\002HydraIRC\002 -> http://www.hydrairc.com <- Go on, try it!",
  //"\002HydraIRC\002 -> http://www.hydrairc.com <- 100,000+ downloads can't be wrong",
  //"\002HydraIRC\002 -> http://www.hydrairc.com <- IRC for those that like to be different",
  "Want to be different? Try \002HydraIRC\002 -> http://www.hydrairc.com <-",
  "\002HydraIRC\002 -> http://www.hydrairc.com <- The alternative IRC client",
  "\002HydraIRC\002 -> http://www.hydrairc.com <- Like it?  Visit #hydrairc on Freenode",
  "\002HydraIRC\002 -> http://www.hydrairc.com <- Would you like to know more?",
  "\002HydraIRC\002 -> http://www.hydrairc.com <- Organize your IRC",
  "\002HydraIRC\002 -> http://www.hydrairc.com <- Now with extra fish!",
  "\002HydraIRC\002 -> http://www.hydrairc.com <- Wibbly Wobbly IRC",
  "\002HydraIRC\002 -> http://www.hydrairc.com <- It'll be on slashdot one day...",
  "\002HydraIRC\002 -> http://www.hydrairc.com <- In tests, 0x09 out of 0x0A l33t h4x0rz prefer it :)",
  "\002HydraIRC\002 -> http://www.hydrairc.com <- Po-ta-to, boil em, mash em, stick em in a stew.",
  "Try \002HydraIRC\002 -> http://www.hydrairc.com <-" 
};

char *g_ServerStatusNames[4] = 
{
  "Initialising",
  "Connecting",
  "Connected",
  "Disconnected"
};

void IRCServer::UpdateUserIdentity( void )
{
  // select the preferred user identity, if set, otherwise select the default, if set, otherwise select the first one
  UserIdentity *pIdentity = g_pPrefs->GetPreferredIdentity(m_pDetails->m_NetworkID, 
    GPIF_UseDefault | GPIF_UseFirst);

  if (pIdentity)
  {
    sys_Printf(BIC_INFO,"Changing identity to preferred/default identity (%s) for server \"%s\"\n",pIdentity->m_Description,m_pDetails->m_Name);
    m_pIdentity = new UserIdentity(pIdentity);
  }
}

IRCServer::IRCServer(IRCServerDetails *pServerDetails, UserIdentity *pUserIdentity /* = NULL */, BOOL Connect /* = TRUE */)
{
  CListener::Start(&g_EventManager);

  if (pServerDetails)
    m_pDetails = new IRCServerDetails(pServerDetails);
  else
    m_pDetails = new IRCServerDetails(g_DefaultServerDetails);

  m_pIdentity = NULL;
  // update the network id and description based on the current servername.
  UpdateServerDetails(m_pDetails);

  // if no identity supplied, then find one.
  if (!pUserIdentity)
  {
    UpdateUserIdentity();
  }
  else
    m_pIdentity = new UserIdentity(pUserIdentity);  // use the one supplied

  ATLASSERT(m_pDetails && m_pIdentity);

  // DCC Chat stuff, i.e. we're not a dcc chat.
  m_IsDirect = FALSE;
  m_Listen = FALSE;
  m_OtherNick = NULL;
  m_pOtherServer = NULL;

  // GO!
  Initialise(Connect);
}

IRCServer::IRCServer(IRCServerDetails *pServerDetails, IRCServer *pOther, char *OtherNick, BOOL Direct, BOOL Listen)
{
  CListener::Start(&g_EventManager);
  m_pDetails = new IRCServerDetails(pServerDetails);
  m_pIdentity = new UserIdentity(pOther->m_pIdentity);    

  // DCC Chat stuff
  m_IsDirect = Direct;
  if (Direct)
  {
    m_OtherNick = strdup(OtherNick);
    m_Listen = Listen;
    m_pOtherServer = pOther;
  }
  else
  {
    m_OtherNick = FALSE;
    m_Listen = FALSE;
    m_pOtherServer = NULL;
  }
  // GO!
  Initialise(TRUE);
}

IRCServer::~IRCServer()
{
#ifdef DEBUG
  sys_Printf(BIC_FUNCTION,"Deleting Class Instance: IRCServer\n");
#endif
  Disconnect(FALSE);

  if (m_IsDirect)
  {
    if (m_pOtherServer->m_pChildWnd && m_pOtherServer->m_pChildWnd->IsWindow())
      ::PostMessage(m_pOtherServer->m_pChildWnd->m_hWnd,WM_UPDATEINFOPANE, (WPARAM)0, (LPARAM)NULL);
  }

  if (m_pServerSocket)
    delete m_pServerSocket;

  if (m_pSocket)
    delete m_pSocket;

  // they all remove themselves from the list when they're deleted...
  while (m_ChannelList.GetSize() > 0)
    delete m_ChannelList[0];

  while (m_QueryList.GetSize() > 0)
    delete m_QueryList[0];

  // TODO: don't remove DCC active chats when deleting a server!
  // and move them somewhere else in the server list *sigh*
  // e.g. 
  // for (int i = 0 ; i < m_DCCChatList.GetSize() ; i++)
  //  m_DCCChatList[i]->ServerRemoved();
  // and then ServerRemoved() would do the nescessary.

  while (m_DCCChatList.GetSize() > 0)
    delete m_DCCChatList[0];

  while (m_ExecOnLoginList.GetSize() > 0)
  {
    CCommandProfile *pProfile = m_ExecOnLoginList[0];
    m_ExecOnLoginList.RemoveAt(0);
    delete pProfile;
  }

  if (m_pChildWnd && m_pChildWnd->IsWindow())
    g_pMainWnd->MDIDestroy(m_pChildWnd->m_hWnd);

  m_pChildWnd = NULL;

  if (m_IsDirect)
    m_pOtherServer->m_DCCChatList.Remove(this);

  g_pMainWnd->m_ServerListView.Remove(this);

  if (m_Nick) free(m_Nick);
  if (m_LastNick) free(m_LastNick);
  if (m_ReclaimNick) free(m_ReclaimNick);

  if (m_OtherNick) free(m_OtherNick); // for DCC Chats
  if (m_pDetails) 
    delete m_pDetails;
  if (m_pIdentity) 
    delete m_pIdentity;

  g_ServerList.Remove(this);

  g_IDManager.RelinquishID(m_ServerID);
  ClearOutputBuffer();
  if (m_DescriptionString) free(m_DescriptionString);
  g_pMainWnd->SetStatusBar(STATUS_ICON_DONTCHANGE,NULL); // update the server count on the status bar

  //
  if(m_UserModes != NULL) free(m_UserModes);
  if(m_Prefixes != NULL) free(m_Prefixes);

  for(int i = 0; i < 4; i++)
  {
    if(m_ChannelModes[i] != NULL)
      free(m_ChannelModes[i]);
  }

  if(m_ParamChannelModes) free(m_ParamChannelModes);
  if(m_ParamClearChannelModes) free(m_ParamClearChannelModes);
  //
}

IRCChannel *IRCServer::FindChannel(IRCChannel *pChannel)
{
  int index = m_ChannelList.Find(pChannel);
  if (index >= 0)
    return m_ChannelList[index];

  return NULL;
}

IRCChannel *IRCServer::FindChannel(const char *ChannelName)
{
  IRCChannel *pChannel;

  for (int i = 0 ; i < m_ChannelList.GetSize() ; i ++)
  {
    pChannel = m_ChannelList[i];
    if (stricmp(ChannelName, pChannel->m_pProperties->m_Name) == 0)
    {
      return pChannel;
    }
  }
  return NULL;
}

IRCQuery *IRCServer::FindQuery(const char *OtherNick)
{
  IRCQuery *pQuery;

  for (int i = 0 ; i < m_QueryList.GetSize() ; i ++)
  {
    pQuery = m_QueryList[i];
    if (stricmp(OtherNick, pQuery->m_OtherNick) == 0)
    {
      return pQuery;
    }
  }
  return NULL;
}

// hostmask NULL if unknown
IRCQuery *IRCServer::CreateQuery(const char *OtherNick, const char *Hostmask)
{
  IRCQuery *pNewQuery,*pQuery;
  IRCChannel *pChannel;

  pQuery = FindQuery(OtherNick);
  if (pQuery)
  {
    if (pQuery->m_pChildWnd)
      pQuery->m_pChildWnd->ActivateWindow();
      //g_pMainWnd->MDIActivate(pQuery->m_pChildWnd->m_hWnd);

    return NULL;
  }

  // query not already present, so create new IRCQuery instance

  // if hostmask is null, search the channels for the user.
  if (!Hostmask)
  {
    for (int i = 0 ; i < m_ChannelList.GetSize() ; i ++)
    {
      pChannel = m_ChannelList[i];
      int UserCount = pChannel->m_Users.m_UserList.GetSize();
      for (int j = 0 ; j < UserCount; j++)
      {
        IRCUser *pUser = pChannel->m_Users.m_UserList[j];
        if (stricmp(OtherNick,pUser->m_Nick) == 0)
        {
          // found our user!
          Hostmask = pUser->m_Address;
          break;
        }
      }
    }
  }

  pNewQuery = new IRCQuery(this, OtherNick, Hostmask);
  if (pNewQuery)
  {
    m_QueryList.Add(pNewQuery);
    g_QueryList.Add(pNewQuery);
  }
  return pNewQuery;
}

IRCChannel *IRCServer::CreateChannel(char *Args, int Flags /* = CCF_NONE */)
{
  IRCChannel *pNewChannel,*pChannel;
  char *channelnameptr, *channelname, *key, *tokens[2];
  int count;

  while (*Args == ' ')
    Args++;
  stripwhitespace(Args);

  count = GetTokens(Args, tokens, 2, NULL); // count should normally be 1, or 2 if a key was specified
  channelnameptr = AllocEmptyString(tokens[0]);
  key = AllocEmptyString(tokens[1]);

  if (!ischannelstartchar(*channelnameptr))
  {
    channelname = (char *)malloc(strlen(channelnameptr) + 2); // +1 for terminator + 1 more for extra #
    if (channelname)
    {
      channelname[0] = '#';
      strcpy(channelname+1,channelnameptr);
      free(channelnameptr);
      channelnameptr = channelname;
    }
  }

  if (!channelnameptr || !isvalidchannelname(channelnameptr))
  {
    if (channelnameptr) free(channelnameptr);
    if (key) free(key);
    return NULL;
  }

  // safety check...
  for (int i = 0 ; i < m_ChannelList.GetSize() ; i ++)
  {
    pChannel = m_ChannelList[i];
    if (stricmp(channelnameptr, pChannel->m_pProperties->m_Name) == 0)
    {
      // channel already exists..
      if (!pChannel->m_pChildWnd || pChannel->m_Status != CHN_STATE_CONNECTED)
      {
        // If the channel class exists, but the window doesn't exist then we
        // issue the /join command again.
        // rejoin the channel if banned, kicked or other error
        pChannel->m_pProperties->SetKey(key); // Key can be null.
        pChannel->Join();
      }
      else
      {
        // show the window!
        if (!(Flags & CCF_DONTSHOWIFEXISTS))
          pChannel->m_pChildWnd->ActivateWindow();
          //g_pMainWnd->MDIActivate(pChannel->m_pChildWnd->m_hWnd);

      }

      // tidy up.
      if (channelnameptr) free(channelnameptr);
      if (key) free(key);
      return NULL;
    }
  }
  
  // channel not already joined so create new IRCChannel instance

  // after this, the class is instanciated(!), but the GUI is NOT
  // displayed.  The IRCChannel class then sends the command
  // to the server, and then we wait for a response
  // if the reponse is good, we show the channel GUI, if bad
  // we kill the class instance.
  pNewChannel = new IRCChannel(this, channelnameptr, key);
  if (pNewChannel)
  {
    m_ChannelList.Add(pNewChannel);
    g_ChannelList.Add(pNewChannel);
  }
  if (channelnameptr) free(channelnameptr);
  if (key) free(key);

  return pNewChannel;
}

void IRCServer::Connect( void )
{  
#ifdef DEBUG
    sys_Printf(BIC_INFO,"Connecting \"%s\" - Previous Server State: %s\n",
      GetDescriptionString(),g_ServerStatusNames[m_Status]);
#endif

  time(&m_ConnectAttemptTime);

  UpdateServerDetails(m_pDetails);

  /*  The old way of doing it, before GetNetworkID()
  int i;
  for (i = 0 ; i < g_ServerDetailsList.GetSize(); i++)
  {
    IRCServerDetails *pServerDetails = g_ServerDetailsList[i];

    if (stricmp(m_pDetails->m_Name,pServerDetails->m_Name) == 0)
    {
      m_pDetails->m_NetworkID = pServerDetails->m_NetworkID;
      if (m_pDetails->m_Description) free(m_pDetails->m_Description);
      m_pDetails->m_Description = strdup(pServerDetails->m_Description);
      break; // don't bother looking through the rest of the list
    }
  }
  */

  if (!m_pDetails->m_Description)
    m_pDetails->m_Description = strdup(g_DefaultServerDetails->m_Description);

  // close old log file if needed, and start logging (if enabled)
  UpdateLogger();

  if (m_ConnectAttempt > 0)
    m_StatusFlags |= ISSF_RETRYINGCONNECT;
  else
    m_StatusFlags &= ~ISSF_WASLOGGEDIN;  // server was disconnected normally

  m_ConnectAttempt++;

  InitVariables();

  char *VarStr;

  if (m_IsDirect)
    VarStr = HydraIRC_BuildString(512,g_DefaultStrings[DEFSTR_DCC_ChatInit],m_OtherNick);
  else
  {
    VarStr = HydraIRC_BuildString(512,g_DefaultStrings[DEFSTR_Server_Connect],
      m_pDetails->m_Name,
      m_pDetails->m_Port,
      m_ConnectAttempt, 
      INTPREF(PREF_nReconnectRetries1) + INTPREF(PREF_nReconnectRetries2));
  }

  m_Variables[VID_ALL] = UseEmptyString(VarStr);
  OutputFormatter(BIC_CONNECTING,APP_CONNECT);
  if (VarStr) free(VarStr);

  // Make sure the user can see the message while we attempt the connect.
  m_pChildWnd->RedrawWindow();
  m_Status = SVR_STATE_CONNECTING;
  UpdateStatus();

  if (m_Listen)
  {
    m_pServerSocket = new Socket(m_pChildWnd->m_hWnd,WM_NETEVENT);
    if (!m_pServerSocket)
      Printf(BIC_ERROR,"Couldn't create socket");
    else
    {
      // find an available port to listen on...
      m_pDetails->m_Port = INTPREF(PREF_nDCCChatFirstPort);
      while ( TRUE )
      {
        if (m_pServerSocket->Listen(m_pDetails->m_Port) == SOCK_ERR_NONE) // .. socket is not in use by something else..
          break; // found a port, continue!
        if (m_pDetails->m_Port < INTPREF(PREF_nDCCChatLastPort) && m_pDetails->m_Port < 65535)
          m_pDetails->m_Port++;
        else
        {
          m_pDetails->m_Port = 0; // reset it so we know we failed.
          Printf(BIC_ERROR,"no free ports!");
          break;
        }
      }

      if (m_pDetails->m_Port > 0)
      {
        VarStr = HydraIRC_BuildString(512,g_DefaultStrings[DEFSTR_DCC_ChatConnect], m_OtherNick, m_pDetails->m_Port);
        m_Variables[VID_ALL] = UseEmptyString(VarStr);
        OutputFormatter(BIC_CONNECTING,APP_CONNECT);
        if (VarStr) free(VarStr);

        sys_Printf(BIC_INFO,"DCC: Chat available on port %d\n",m_pDetails->m_Port);
        // tell the remote user to accept the chat
        m_pOtherServer->CMD_CTCPRequest(m_OtherNick, "DCC CHAT chat %lu %d",          
          htonl(GetInternetIP()), 
          m_pDetails->m_Port);
        /*
        m_pOtherServer->Send("PRIVMSG %s :\001DCC CHAT chat %lu %d\001\r\n",
          m_OtherNick, 
          htonl(GetInternetIP()), 
          m_pDetails->m_Port);
        */

        // sit back and wait for a WM_NETEVENT message to appear, or timeout during an EV_TICK

      }
    }
  }
  else
  {

	  DWORD Address = inet_addr(m_pDetails->m_Name); // were we given an IP address ?
	  if (Address == INADDR_NONE)
    {
      // do the name resolution and wait for WM_DNSEVENT

      InitVariables();
      m_Variables[VID_ALL] = g_DefaultStrings[DEFSTR_Server_Resolving];
      OutputFormatter(BIC_CONNECTING,APP_CONNECT);
      //Printf(BIC_CONNECTING,g_DefaultStrings[DEFSTR_Server_Resolving]);

      g_pDNSResolver->StartResolve(m_pDetails->m_Name,m_pChildWnd->m_hWnd);
    }
    else
    {
      // ip address, skip DNS resolution
      ActualConnect(Address);
    }
  }
}

void IRCServer::ActualConnect( DWORD Address )
{
  // User may have already cancelled the connect.
  // or we might get called due to a late DNS resolution attempt.
  if (m_Status != SVR_STATE_CONNECTING)
    return;

  if (!Address)
  {
    InitVariables();
    m_Variables[VID_ALL] = g_DefaultStrings[DEFSTR_Server_ResolveError];
    OutputFormatter(BIC_ERROR,APP_ERROR);
    //Printf(BIC_ERROR,g_DefaultStrings[DEFSTR_Server_ResolveError]);
    Disconnect();
    return;
  }

  // do socket connect

  InitVariables();
  char *AddressStr = GetIPv4AddressString(Address);
  char *VarStr = HydraIRC_BuildString(512,g_DefaultStrings[DEFSTR_Server_ConnectAddress],UseEmptyString(AddressStr),m_pDetails->m_Port);
  m_Variables[VID_ALL] = UseEmptyString(VarStr);
  OutputFormatter(BIC_CONNECTING,APP_CONNECT);
  if (VarStr) free(VarStr);
  if (AddressStr) free(AddressStr);
  //Printf(BIC_CONNECTING,"*** Connecting to %d.%d.%d.%d:%d",AddressDigits[0],AddressDigits[1],AddressDigits[2],AddressDigits[3],(unsigned short)m_pDetails->m_Port);

  //switch(m_pSocket->Connect(m_pDetails->m_Name,(unsigned short)m_pDetails->m_Port))
  switch(m_pSocket->Connect(Address,(unsigned short)m_pDetails->m_Port))
  {
    case SOCK_ERR_NONE:
      break;
/*
    case SOCK_ERR_RESOLVE:
      Printf(BIC_ERROR,"*** Could not resolve host name");
      Disconnect();
      break;
*/
    default:
      InitVariables();
      int Error = WSAGetLastError();
      VarStr = HydraIRC_BuildString(512,g_DefaultStrings[DEFSTR_Socket_Error],Error,m_pSocket->GetWSASelectErrorString(Error));
      m_Variables[VID_ALL] = UseEmptyString(VarStr);
      OutputFormatter(BIC_ERROR,APP_ERROR);
      if (VarStr) free(VarStr);
      //Printf(BIC_ERROR,g_DefaultStrings[DEFSTR_Socket_Error],Error,m_pSocket->GetWSASelectErrorString(Error));
      Disconnect();
      break;
  }
}

void IRCServer::ResetNick( void )
{
  if (m_Nick) free(m_Nick);
  if (m_ReclaimNick) free(m_ReclaimNick);
  if (m_LastNick) free(m_LastNick);

  // setup the default nick.
  m_NickIndex = 0;
  if (m_IsDirect)
  {
    m_Nick = strdup(m_pOtherServer->m_Nick);
    m_pOtherServer->m_DCCChatList.Add(this);
  }
  else
    m_Nick = strdup(m_pIdentity->GetNick(m_NickIndex++)); // point to the next one

  m_ReclaimNick = NULL;
  m_LastNick = NULL;
}


void IRCServer::Initialise( BOOL ConnectNow )
{ 

// CRASH TEST!
#if 0
  typedef struct CrashTest_s
  {
    char *str;
  } CrashTest_t;

  CrashTest_t *pCrash = NULL;

  pCrash->str = NULL; // CRASH!

  // When this happens, check the crash address and search HydraIRC.map for a function
  // with an address as close to the crash address as possible
#endif


  //Server modes
  m_UserModes = strdup("aohv");
  m_Prefixes = strdup("!@%+");
  m_ChannelModes[0] = strdup("b");
  m_ChannelModes[1] = strdup("k");
  m_ChannelModes[2] = strdup("l");
  m_ChannelModes[3] = strdup("i");

  m_ParamClearChannelModes = strdup("bk");
  m_ParamChannelModes = strdup("bkl");
  //End Server modes


  // TODO: error checking on the class creation would be good at some point...
  m_Status = SVR_STATE_INITIALISING;
  m_StatusFlags = ISSF_RETRYCONNECT;

  time(&m_CreateTime);

  if (m_IsDirect)
    m_ObjectType        = OTYPE_DCCCHAT;
  else
    m_ObjectType        = OTYPE_SERVER;

  // apply the status filter list
  if (!m_IsDirect)
    SetDirtyFilterList(g_DefaultFilterList_DirtyStatusServer);

  m_ServerID = g_IDManager.ObtainID();

  m_ConnectAttempt = 0;
  m_LoggedInCount = 0;

  ATLASSERT(m_pDetails && m_pIdentity);
  
  if (!m_pDetails->m_Description)
    SetString(m_pDetails->m_Description,g_DefaultServerDetails->m_Description);

  m_FloodControlCounter = 0; 

  m_DescriptionString = NULL;

  m_Nick = NULL;
  m_NickIndex = 0;
  m_ReclaimNick = NULL;
  m_LastNick = NULL;

  ResetNick();

  m_pChannelListDlg = NULL;

  // add to the server list
  g_pMainWnd->m_ServerListView.Add(this);

  m_pChildWnd = new CChildFrame(&g_EventManager, this);
  // create the window
	m_pChildWnd->CreateEx(g_pMainWnd->m_hWndMDIClient);

  g_ServerList.Add(this);

  // add to the child window list.
  g_ChildWndList.Add(m_pChildWnd);

  if (BOOLPREF(PREF_bMonitorServersByDefault))
  {
    g_pMainWnd->m_ServerMonitorView.AddServer(this);
  }

  //setup sockets, buffers etc.
  //ZeroMemory(&m_Buf, sizeof(m_Buf));
  //m_BufPos = 0;
  m_SocketBufPtr = m_SocketBuf;
  m_pServerSocket = NULL;
  m_pSocket = new Socket(m_pChildWnd->m_hWnd, WM_NETEVENT);

  if (ConnectNow)
    Connect(); 
  else
  {
    m_Status = SVR_STATE_DISCONNECTED;
    UpdateStatus();
  }
}

// todo: add parameter so that we can prompt for passwords as required by the supplied irc params
// then adjust code that calls this. function
// e.g. handle ,needkey and ,needpass
// returns TRUE if extra commands are run.
BOOL IRCServer::ParseURIParams( char *URIParams)
{
  if (!URIParams || !*URIParams)
    return FALSE;
  
  BOOL Parsed = FALSE;

  char *data = (char *)malloc(strlen(URIParams)+1);
  char *commandstr = NULL;
  *data = 0;
  int i = 0,j = 0;
  while (ischannelchar(URIParams[i]))
  {
    data[j++] = URIParams[i++];
    data[j] = 0;
  }
  
  if (*data)
  {
    if (strstr(URIParams+i,",isnick") != NULL)
    {
      // nick
      commandstr = HydraIRC_BuildString(512,"/query %s",data);
    }
    else
    {
      // channel
      char *key = NULL; // TODO: add support for channel keys/"needpass" in the URI
      commandstr = HydraIRC_BuildString(512,"/join %s%s%s%s",ischannelstartchar(*data) ? "" : "#", // TODO: store the # as a default channel prefix in a pref somewhere..
        data,key ? " " : "", key ? key : "");
    }
  }

  if (commandstr)
  {
    CCommandProfile *pCommands = new CCommandProfile("URI Commands",commandstr);
    if (pCommands)
    {
      ExecCommandProfile(pCommands,TRUE);
      Parsed = TRUE;
    }
  }
  free(data);
  return Parsed;
}

void IRCServer::UpdateLogger( void )
{
  DWORD LoggingFlags = LOGF_NONE;  

  char *LogFolder;
  if (BOOLPREF(PREF_bCreateNetworkLogFolder))
  {    
    LogFolder = HydraIRC_BuildString(MAX_PATH,"%s\\%s",PATHPREF(PREF_pLogRootFolder),GetNetworkName(m_pDetails->m_NetworkID));
  }
  else
  {
    LogFolder = strdup(PATHPREF(PREF_pLogRootFolder));
  }
  char *LogFile; // free()'d by the CLogger class.
  
  if (m_IsDirect)
  {
    LogFile = HydraIRC_BuildString(MAX_PATH,"DCCChat_%s.txt",m_pDetails->m_Name);
    if (BOOLPREF(PREF_bLogging) && BOOLPREF(PREF_bDCCChatLogging)) LoggingFlags |= LOGF_ENABLED;
    if (BOOLPREF(PREF_bStripCodesFromDCCChatLogs)) LoggingFlags |= LOGF_STRIPCODES;
  }
  else
  {
    if (m_pDetails->m_Port == 6667)
      LogFile = HydraIRC_BuildString(MAX_PATH,"Server_%s.txt",m_pDetails->m_Name);
    else
      LogFile = HydraIRC_BuildString(MAX_PATH,"Server_%s_%d.txt",m_pDetails->m_Name, m_pDetails->m_Port);
    if (BOOLPREF(PREF_bLogging) && BOOLPREF(PREF_bServerLogging)) LoggingFlags |= LOGF_ENABLED;
    if (BOOLPREF(PREF_bStripCodesFromServerLogs)) LoggingFlags |= LOGF_STRIPCODES;
  }

  // update the settings and start logging!
  m_Logger.SetPaths(LogFolder,LogFile);
  m_Logger.UpdateSettings(LoggingFlags);
}

void IRCServer::OnConnect( void )
{  
  char *VarStr;

  m_Status = SVR_STATE_CONNECTED;
  g_EventManager.DeleteEvents((CListener *)this,EV_SV_RECONNECT); // delete any pending reconnects
  UpdateStatus();


  InitVariables();
  m_Variables[VID_ALL] = g_DefaultStrings[DEFSTR_Socket_Connected];
  OutputFormatter(BIC_CONNECTING,APP_CONNECT);
  //Printf(BIC_CONNECTING, g_DefaultStrings[DEFSTR_Socket_Connected]);

  char *message = HydraIRC_BuildString(512,g_DefaultStrings[DEFSTR_Status_ServerConnected],m_pDetails->m_Description,m_pDetails->m_Name);
  
  if (message)
  {
    g_pMainWnd->SetStatusBar(STATUS_ICON_INFORMATION,message);    
  }

  if (m_IsDirect)
  {
    m_pLastNE = g_pNotificationManager->NotificationEvent(NE_DCCCHATCONNECT,m_pDetails->m_NetworkID,m_OtherNick);
    if (m_pLastNE && m_pLastNE->m_Flags & NE_FLAG_LOGINEVENTLOG)
    {
      g_pEventLogManager->AddItem(ELIT_DCC, ELID_Connect,m_OtherNick, message, this, NULL);
    }

    m_StatusFlags |= ISSF_LOGGEDIN; // we don't "log-in" to dcc servers
    OnLoggedIn();
  }
  else
  {
    // Issue the PASS command if there is a password for this server
    if (m_pDetails->m_Password && *m_pDetails->m_Password)
    {
      Send("PASS %s\r\n",m_pDetails->m_Password);
    }

    InitVariables();
    VarStr = HydraIRC_BuildString(512,g_DefaultStrings[DEFSTR_Server_IdentityAndNick], m_pIdentity->m_Description, m_Nick);
    m_Variables[VID_ALL] = UseEmptyString(VarStr);
    OutputFormatter(BIC_CONNECTING,APP_CONNECT);
    if (VarStr) free(VarStr);
    // Printf(BIC_CONNECTING, g_DefaultStrings[DEFSTR_Server_IdentityAndNick], m_pIdentity->m_Description, m_Nick);

    // Issue, NICK then USER commands.
    Send("NICK %s\r\nUSER %s 0 * :%s\r\n",
      m_Nick,
      m_pIdentity->m_UserName,
      m_pIdentity->m_RealName);
  /*
      Send("NICK %s\r\nUSER %s # # :%s\r\n",
      m_Nick,
      m_pIdentity->m_UserName,
      m_pIdentity->m_RealName);
  */
    // TODO: notify plugins of event here..
    // sitback and wait for network events!
  }
  if (message) free(message);
}

char *IRCServer::BuildDescriptionString( void )
{
  if (m_DescriptionString) free (m_DescriptionString);

  int len;

  len = strlen(m_pDetails->m_Description) + 2 + // "description ("
        strlen(m_pDetails->m_Name) + 4 +        // "servername) - " (oops, had +3 not +4)
        strlen(m_Nick) + 1;                     // "nick\0"
  m_DescriptionString = (char *) malloc(len);

  sprintf(m_DescriptionString, "%s (%s) - %s",
          m_pDetails->m_Description,
          m_pDetails->m_Name,
          m_Nick); // the actual current nick, not the default from the userdetails.
  //len = strlen(m_DescriptionString); // double check length
  return m_DescriptionString;
}

char *IRCServer::GetDescriptionString( void )
{
  if (!m_DescriptionString)
  {
    return BuildDescriptionString();
  }
  return m_DescriptionString;
}

void IRCServer::UpdateInfo( void )
{
  if (!m_pChildWnd || !m_pChildWnd->IsWindow() || !CHILDFRAMEPTR(m_pChildWnd)->m_InfoView.IsWindow() )
    return;

  CString Message;

  if (m_IsDirect)
  {
    char *AddressStr = GetIPv4AddressString((DWORD)m_pSocket->GetAddress());
    switch(m_Status)
    {
      case SVR_STATE_CONNECTING:
        Message.Format("Connecting to %s\n",m_OtherNick);
        break;
      case SVR_STATE_CONNECTED:
        Message.Format("Connected\n\nNow talking to %s\n", m_OtherNick);
        break;
      case SVR_STATE_DISCONNECTED:
        Message.Format("Disconnected from %s\n", m_OtherNick);
        break;

    }
    if (AddressStr)
    {
      Message.AppendFormat("IP: %s:%d",UseEmptyString(AddressStr),m_pDetails->m_Port);
      free(AddressStr);
    }

  }
  else
  {
    switch(m_Status)
    {
      case SVR_STATE_CONNECTING:
        Message.Format("Connecting to server");
        break;
      case SVR_STATE_CONNECTED:
        Message.Format("Connected\n");
        if (!(m_StatusFlags & ISSF_LOGGEDIN))
        {
          Message.Append("Waiting for login...");
        }
        else
        {
          if (m_ChannelList.GetSize() == 0)
          {
            Message.AppendFormat("You have not joined any channels, use /JOIN <ChannelName> to join one");
          }
          else
          {
            Message.AppendFormat("Channels: %ld\nQueries: %ld\nDCC Chats: %ld",
              m_ChannelList.GetSize(),
              m_QueryList.GetSize(),
              m_DCCChatList.GetSize());
          }
        }
        break;
      case SVR_STATE_DISCONNECTED:
        Message.Format("Disconnected from server");
        break;
    }
  }
  
  CHILDFRAMEPTR(m_pChildWnd)->m_InfoView.Put(Message);
}

void IRCServer::UpdateStatus( void )
{
  // update window titles on all windows related to this server
  BuildDescriptionString();
  if (m_pChildWnd)
  {
    CHILDFRAMEPTR(m_pChildWnd)->UpdateStatus();
    CHILDFRAMEPTR(m_pChildWnd)->m_HeaderView.UpdateHeader(m_DescriptionString);
    UpdateInfo();
  }

  // if this is a DCC Chat then update the parent server's info pane
  if (m_IsDirect)
  {
    ::PostMessage(m_pOtherServer->m_pChildWnd->m_hWnd,WM_UPDATEINFOPANE, (WPARAM)0, (LPARAM)NULL);
    //m_pOtherServer->UpdateInfo();

  }
  g_pMainWnd->m_ServerListView.Update(this);

  g_pMainWnd->SetStatusBar(STATUS_ICON_DONTCHANGE,NULL); // show the nick/getnick/severcount in the status bar.

  // TODO: update server list tree view with a different icon
}

void IRCServer::Disconnect( BOOL Reconnect, BOOL SendQuit /* = TRUE */ )
{
  // if the user cancelled a reconnect, we need to remove the existing
  // timed events from the queue.
  g_EventManager.DeleteEvents((CListener *)this,EV_SV_RECONNECT);
  g_EventManager.DeleteEvents((CListener *)this,EV_SV_SENDKEEPALIVE);


  if (m_Status == SVR_STATE_CONNECTED && m_StatusFlags & ISSF_LOGGEDIN)
  {
    // reset the nick index.
    m_NickIndex = 0; 

    // make a note of the nick we were using when we got disconnected.
    SetString(m_LastNick,m_Nick);

    // only do this if we've not already got a nick that we want to change to
    if (!m_ReclaimNick)
      m_ReclaimNick = strdup(m_Nick);
    else
    {
      SetString(m_Nick,m_ReclaimNick);
      // causes "Unknown" to be used as a nick sometimes.
      //m_NickIndex = -1; // if the Reclaim nick is inuse, start at the beginning again.
    }
  }

  if (m_Status == SVR_STATE_CONNECTED || m_Status == SVR_STATE_CONNECTING)
  {
#ifdef DEBUG
    sys_Printf(BIC_INFO,"Disconnecting \"%s\" - Previous Server State: %s\n",
      GetDescriptionString(),g_ServerStatusNames[m_Status]);
#endif

    if (m_IsDirect)
    {
      // NEVER reconnect a dcc.
      m_StatusFlags &= ~ISSF_RETRYCONNECT;
    }
    else
    {
      // normal server
      if (m_StatusFlags & ISSF_LOGGEDIN)
      {
        if (SendQuit)
          Send("QUIT : %s\r\n",PickRandomString(g_QuitMessages,sizeof (g_QuitMessages) / sizeof(char *)) );

        m_ConnectAttempt = 0; // reset the counter, disconnected normally...
        m_StatusFlags |= ISSF_RETRYCONNECT;
        m_StatusFlags &= ~ISSF_RETRYINGCONNECT;
      }

      if (!Reconnect)
      {
        m_StatusFlags &= ~ISSF_RETRYCONNECT;

        if (m_Status == SVR_STATE_CONNECTING)
        {
          InitVariables();
          m_Variables[VID_ALL] = g_DefaultStrings[DEFSTR_Socket_ReconnectAborted];
          OutputFormatter(BIC_ERROR,APP_CONNECT);         
          //Printf(BIC_ERROR,g_DefaultStrings[DEFSTR_Socket_ReconnectAborted]);
        }
      }
    }

    m_StatusFlags &= ~ISSF_LOGGEDIN;

    InitVariables();

    char *VarStr = HydraIRC_BuildString(512,g_DefaultStrings[DEFSTR_Server_Disconnected],UseEmptyString(GetTimestamp()));
    m_Variables[VID_ALL] = UseEmptyString(VarStr);
    OutputFormatter(BIC_CONNECTING,APP_CONNECT);
    if (VarStr) free(VarStr);
 
    //Printf(BIC_CONNECTING, g_DefaultStrings[DEFSTR_Server_Disconnected]);

    // TODO: change window icons

    m_Status = SVR_STATE_DISCONNECTED;
    UpdateStatus();

    // Hmmm.....
    //Sleep(250); // allow enough time for the quit message to be sent..

    // disconnect socket (if it's open)
    if (m_pSocket->GetSocket() != INVALID_SOCKET)
      m_pSocket->CloseSocket();

    OnDisconnect();
  } 
  else
  {
    // reset the counter and flag, so that we call Reconnect() we'll try
    // to connect a few more times again.
    m_ConnectAttempt = 0;
    m_StatusFlags |= ISSF_RETRYCONNECT;
    m_StatusFlags &= ~ISSF_RETRYINGCONNECT;
  }
}

void IRCServer::OnDisconnect( void )
{
  int i;
  char *VarStr;

  char *message = HydraIRC_BuildString(512,g_DefaultStrings[DEFSTR_Status_ServerDisconnected],m_pDetails->m_Description,m_pDetails->m_Name);
  
  if (message)
  {
    g_pMainWnd->SetStatusBar(STATUS_ICON_WARNING,message);


    if (m_StatusFlags & ISSF_WASLOGGEDIN) // don't keep logging the message...
    {
      if (m_IsDirect)
        m_pLastNE = g_pNotificationManager->NotificationEvent(NE_DCCCHATDISCONNECT,m_pDetails->m_NetworkID,m_OtherNick);
      else
        m_pLastNE = g_pNotificationManager->NotificationEvent(NE_DISCONNECT,m_pDetails->m_NetworkID, NULL);

      if (m_pLastNE && m_pLastNE->m_Flags & NE_FLAG_LOGINEVENTLOG)
      {
        g_pEventLogManager->AddItem(m_IsDirect ? ELIT_DCC : ELIT_SERVER, ELID_Disconnect,NULL, message, this, NULL);
      }      
    }
    free(message);
  }

  // post *** disconnected to all related channels
  for (i = 0 ; i < m_ChannelList.GetSize() ; i ++ )
  {
    m_ChannelList[i]->OnDisconnect();
  }

  // post *** disconnected to all related pm's
  for (i = 0 ; i < m_QueryList.GetSize() ; i ++ )
  {
    m_QueryList[i]->OnDisconnect();
  }

  if (m_pChildWnd && !m_IsDirect)
  {
    // TODO: notify plugins of event here..
    int MaxRetries = INTPREF(PREF_nReconnectRetries1) + INTPREF(PREF_nReconnectRetries2);
    int RetryDelay = m_ConnectAttempt < INTPREF(PREF_nReconnectRetries1) ? INTPREF(PREF_nReconnectDelay1) : INTPREF(PREF_nReconnectDelay2);

    if (m_ConnectAttempt < MaxRetries && m_StatusFlags & ISSF_RETRYCONNECT)
    {
      InitVariables();

      if (m_ConnectAttempt == 0 && m_StatusFlags & ISSF_WASLOGGEDIN)
        VarStr = HydraIRC_BuildString(512,g_DefaultStrings[DEFSTR_Server_DisconnectRetry],RetryDelay );
      else
        VarStr = HydraIRC_BuildString(512,g_DefaultStrings[DEFSTR_Server_ConnectRetry], m_ConnectAttempt, MaxRetries,RetryDelay );

      m_Variables[VID_ALL] = UseEmptyString(VarStr);

      OutputFormatter(BIC_ERROR,APP_CONNECT);
      if (VarStr) free(VarStr);
      /*
      Printf(BIC_ERROR, g_DefaultStrings[DEFSTR_Server_ConnectRetry],
        m_ConnectAttempt, 
        MaxRetries,
        RetryDelay );
      */

      // start a timer here, and then reconnect later.
      g_EventManager.QueueTimedEvent((CListener *)this,FALSE,EV_SV_RECONNECT,RetryDelay);
    }
    else
    {
      if (m_StatusFlags & ISSF_RETRYCONNECT)
      {
        InitVariables();
        VarStr = HydraIRC_BuildString(512,g_DefaultStrings[DEFSTR_Server_ConnectGiveUp],m_ConnectAttempt);
        m_Variables[VID_ALL] = UseEmptyString(VarStr);
        OutputFormatter(BIC_ERROR,APP_CONNECT);
        if (VarStr) free(VarStr);
      }
      // reset the counter and flag, so that we call Reconnect() we'll try
      // to connect a few more times again.
      m_ConnectAttempt = 0;
      m_StatusFlags |= ISSF_RETRYCONNECT;
      m_StatusFlags &= ~ISSF_RETRYINGCONNECT;
    }
  }
}

void IRCServer::Reconnect( void )
{
  if ((m_Status == SVR_STATE_CONNECTED) || (m_Status == SVR_STATE_CONNECTING))
  {
    Disconnect();
  }
  Connect();
}

#define IRCSERVER_CHANNELBROADCAST_BUFSIZE 1024

void IRCServer::ChannelBroadcast(int Contents, const char *format, ...)
{
  if (!format)
    return;

	va_list args;

	char *buf;

  buf = (char *)malloc(IRCSERVER_CHANNELBROADCAST_BUFSIZE);
  if (buf)
  {
	  //ZeroMemory(buf, IRCSERVER_CHANNELBROADCAST_BUFSIZE);
	  va_start(args, format);
	  _vsnprintf(buf, IRCSERVER_CHANNELBROADCAST_BUFSIZE, format, args);
	  va_end(args);
    buf[IRCSERVER_CHANNELBROADCAST_BUFSIZE - 1] = 0;

    IRCChannel *pChannel;
    for (int i = 0 ; i < m_ChannelList.GetSize() ; i ++)
    {
      pChannel = m_ChannelList[i];
      pChannel->Printf(Contents,buf);
    }
    free(buf);
  }
}

// all output to an server window goes through this
// to print directly to a server window, use IRCServer::Printf()
// for all normal output use this IRCServer::OutputFormatter()
// the output formatter takes a given input and creates logfile
// output AND window output which are then passed to m_Logger and Printf()
#define FORMAT_BUFFER_SIZE 1024

void IRCServer::OutputFormatter(int Contents,int IRCCommandID)
{
  char *FormatStr = GetOutputThemeFormatStr(m_pTheme->m_ThemeItems,IRCCommandID,THEME_SERVER);

  if (!FormatStr)
    return;

  char *FormatBuffer = (char *)malloc(FORMAT_BUFFER_SIZE);

  ReplaceVariables(FormatBuffer,FORMAT_BUFFER_SIZE,FormatStr,g_VariableIDNames,m_Variables,VID_LAST);

  if (m_IsDirect)
    m_Variables[VID_DCCCHATOUTPUT] = FormatBuffer;
  else
    m_Variables[VID_SERVEROUTPUT] = FormatBuffer;

  // log the output
  if (m_Logger.IsEnabled() && !m_Logger.IsFiltered(Contents))
  {
    char LogFormatBuffer[1024];
    char *LogFormatStr;
    
    if (m_IsDirect)
      LogFormatStr = STRINGPREF(PREF_sDCCChatLogFormat);
    else
      LogFormatStr = STRINGPREF(PREF_sServerLogFormat);

    ReplaceVariables(LogFormatBuffer,sizeof(LogFormatBuffer),LogFormatStr,g_VariableIDNames,m_Variables,VID_LAST);

    m_Logger.Log(LogFormatBuffer);
  }

  AddToOutputBuffer(Contents,FormatBuffer);
}

void IRCServer::InitVariables( void )
{
  // clear the variables.
  int i;
  for (i = VID_CHANGABLESTART; i < VID_LAST; i++)
  {
    m_Variables[i] = NULL;
  }
  // assign the common variables
  m_Variables[VID_MYNICK] = m_Nick;
  m_Variables[VID_MYHOST] = "localhost"; // TODO: change
  m_Variables[VID_SERVER] = m_pDetails->m_Name;
  m_Variables[VID_SERVERDESCRIPTION] = m_pDetails->m_Description;
  m_Variables[VID_RESPONSE] = m_CurrentResponse;
}

void IRCServer::Parse( char *processstr )
{
#if 1
  // when debugging it's helpful to be able to feed in some raw text
  // so using the debugger, set debugflag to 1 and then put some text
  // in raw form into this server's text input window.
  int debugflag = 0;
  char debugstr[4096];
  if (debugflag != 0)
  {
    CChildFrame *pChild = CHILDFRAMEPTR(m_pChildWnd);
    pChild->m_TextInputView.GetWindowText(debugstr,4096);
    processstr = debugstr;
  }
#endif

/* BEGIN_CEL_PATCH */
  char *BufferCopy = strdup(processstr);
  if (BufferCopy) {
    CHILDFRAMEPTR(m_pChildWnd)->CheckPluginProcessing(BufferCopy, m_pChildWnd->m_WindowType);
    free(BufferCopy);
  }
/* END_CEL_PATCH */

  char ForwardBuffer[1024+1];
  int ForwardContents = BIC_SERVERMSG; // --CHANGED: 0.3.085 - changed from BIC_NOTICE
  *ForwardBuffer = 0;

  // Note: Using BIC_SOCKET so we don't fill up the OUTPUT docking window unless
  // socket logging is on...
  sys_Printf(BIC_SOCKET,"Processing \"%s\"\n",processstr);

  InitVariables();;

  m_Variables[VID_RESPONSE] = processstr;

  if (m_IsDirect)
  {
    m_Variables[VID_NICK] = m_OtherNick;
    m_Variables[VID_ALL] = UseEmptyString(processstr);
    OutputFormatter(BIC_CHANNELTEXT,RPLS_PRIVMSG_CHANNELTEXT);
    //Printf(BIC_CHANNELTEXT,"<%s> %s",m_OtherNick, processstr);
    return;
  }

  char *tokens[3];
  char *args,*token1,*token2,*token3;
  char *tmp, *comm, tmpnick[96];
  int count, num;

  // chop up the input from the server...
  count = GetTokens(processstr, tokens, 3, &args);

  token1 = AllocEmptyString(tokens[0]); // allocate empty strings in case of failure/low token count
  token2 = AllocEmptyString(tokens[1]);
  token3 = AllocEmptyString(tokens[2]);
  args = AllocEmptyString(args);

  if (token1[0] != ':')
  {
    comm = strtok(processstr, " ");

    // check for b0rkage!
    if (!comm)
    {
      sys_Printf(BIC_ERROR,"Server sent a blank line!\n");
    }
    else
    {
      tmp = strtok(NULL, "");

      if (tmp && tmp[0] == ':') 
        tmp++;

      if (!strcmpi(comm, "NOTICE")) 
      {
        m_Variables[VID_ALL] = args;
        OutputFormatter(BIC_NOTICE,RPLS_NOTICE_CONNECT);
      }
      else if (!strcmpi(comm, "PING")) 
      {
        if (BOOLPREF(PREF_bShowPings))
        {
          m_Variables[VID_ALL] = UseEmptyString(tmp);
          OutputFormatter(BIC_SERVERMSG,RPLS_PING);
        }
        if (BOOLPREF(PREF_bShowPongs))
        {
          m_Variables[VID_ALL] = UseEmptyString(tmp);
          OutputFormatter(BIC_SERVERMSG,RPLS_PONG_SENT);
        }
        Send("PONG :%s\r\n", UseEmptyString(tmp));
      }
      else
      {
        m_Variables[VID_ALL] = UseEmptyString(tmp);
        m_Variables[VID_SERVERNOTICE] = comm;
        OutputFormatter(BIC_NOTICE,RPLS_NOTICE_SERVER);
      }
    }
    free(token1);
    free(token2);
    free(token3);
    free(args);
    return;
  }
  // set these variables right
  comm = token2; 
  char *to   = token3; if (*to   == ':')to++;
  char *from = token1; if (*from == ':')from++;
  char *rest = args;   if (*rest == ':')rest++;

  // extract nick from from-field
  strncpy(tmpnick, from, sizeof(tmpnick));
  tmp = tmpnick;
  while (*tmp != '!' && *tmp) 
    tmp++;
  if (*tmp == '!') 
    *tmp = 0;

  m_Variables[VID_ALL] = rest;

  num = atoi(comm);
  if (num)
  {
    switch(num)
    {
    case RPL_WELCOME: 
      {
        char *FQDN;
        int Count = RegExpToArray(rest,"((.)* )+([^!]*)?(\\!(.)*@{(.)*})?",&FQDN);
        if (Count == 1 && FQDN)
        {
          if (*FQDN) // not a blank string..
          {
            int MaxLen = sizeof(g_FQDN)-1;
            strncpy(g_FQDN,FQDN,MaxLen);
            g_FQDN[MaxLen] = 0;
            sys_Printf(BIC_INFO,"Obtained DNS/Host information, server says you are: \"%s\"\n",g_FQDN);
          }
          free(FQDN);
        }

      }
      // make sure the we have the same nick as the server thinks we have
      SetNick(to);

      OutputFormatter(BIC_NOTICE,RPL_WELCOME);
      m_StatusFlags |= ISSF_LOGGEDIN;
      m_StatusFlags |= ISSF_WASLOGGEDIN;
      m_StatusFlags &= ~ISSF_RETRYINGCONNECT;
      OnLoggedIn();      
      break;

    case 302: // "/raw userhost :<nick>" reply
      {
        char *FQDN;
        int Count = RegExpToArray(rest,"((.)*@{[^ ]*})",&FQDN);        
        if (Count == 1 && FQDN)
        {
          if (*FQDN) // not a blank string..
          {
            int MaxLen = sizeof(g_FQDN)-1;
            strncpy(g_FQDN,FQDN,MaxLen);
            g_FQDN[MaxLen] = 0;
            sys_Printf(BIC_INFO,"Obtained DNS/Host information, server says you are: \"%s\"\n",g_FQDN);
          }
          free(FQDN);
        }
      }
      OutputFormatter(BIC_NOTICE,num);
      break;
      break;
    case RPL_PROTOCTL:
      {
        OutputFormatter(BIC_NOTICE,num);

        char *setting = strtok(rest, " ");

        while(setting != NULL && setting[0] != ':')
        {
          char *div = strchr(setting, '=');

          if(div != NULL)
          {
            // setting has value
            div[0] = 0; // terminate
            char *value = div + 1;

            if(!strcmpi(setting, "prefix"))
            {
              div = strchr(value, ')');
              if(div != NULL)
              {
                div[0] = 0; // terminate
                char *prefixes = div + 1;
                char *modes = value + 1;

                if(m_UserModes) free(m_UserModes);
                if(m_Prefixes) free(m_Prefixes);

                m_UserModes = strdup(modes);
                m_Prefixes = strdup(prefixes);
                //OutputFormatter(BIC_NOTICE, "Modes: %s, prefixes: %s", m_UserModes, m_Prefixes);
                //sys_Printf(BIC_NOTICE, "Modes: %s, prefixes: %s", m_UserModes, m_Prefixes);
              }
            }
            else if(!strcmpi(setting, "chanmodes"))
            {
              // parse available channel modes

              // the CHANMODES string is divided into 4 parts:
              // 1. Modes that always has a nick or address as parameter
              // 2. Modes that changes a setting and always has a parameter
              // 3. Modes that only has a parameter when set
              // 4. Modes that never has a parameter

              char *chanmodes;

              chanmodes = strtok(value, ",");

              int i = 0;

              while(chanmodes != NULL)
              {
                if(m_ChannelModes[i] != NULL)
                  free(m_ChannelModes[i]);

                m_ChannelModes[i] = strdup(chanmodes);

                i++;
                if(i >= 4)
                  break;

                chanmodes = strtok(NULL, ",");
              }

              if(m_ParamClearChannelModes != NULL)
                free(m_ParamClearChannelModes);

              m_ParamClearChannelModes = HydraIRC_BuildString(
                  strlen(m_ChannelModes[0]) + strlen(m_ChannelModes[1]) + 1,
                  "%s%s", m_ChannelModes[0], m_ChannelModes[1]);

              if(m_ParamChannelModes != NULL)
                free(m_ParamChannelModes);

              m_ParamChannelModes = HydraIRC_BuildString(
                  strlen(m_ParamClearChannelModes) + strlen(m_ChannelModes[1]) + 1,
                  "%s%s", m_ParamClearChannelModes, m_ChannelModes[2]);

              /*
              sys_Printf(BIC_NOTICE, "Channel modes: (%s, %s, %s, %s)",
                              m_ChannelModes[0],
                              m_ChannelModes[1],
                              m_ChannelModes[2],
                              m_ChannelModes[3]);*/

            }

          }
          else
          {
            // setting doesn't have value
          }

          // TODO: catch more settings

          setting = strtok(NULL, " ");
        }
      }
      break;

    case RPL_YOURHOST:
    case RPL_CREATED:
    case RPL_MYINFO:
    case RPL_STATSDLINE:
    case RPL_LUSERCLIENT:
    case RPL_LUSERME:
      OutputFormatter(BIC_NOTICE,num);
      break;

    case RPL_MOTDSTART:
    case RPL_MOTD:
    case RPL_ENDOFMOTD:
      if (BOOLPREF(PREF_bShowMOTD))
      {
        OutputFormatter(BIC_NOTICE,num);
      }
      break;

    //case RPL_LISTEND:
    //case RPL_PROTOCTL:
    case RPL_LUSEROP:
    case RPL_LUSERCHANNELS:
    case RPL_LOCALUSERS:
    case RPL_GLOBALUSERS:
      OutputFormatter(BIC_NOTICE,num);
      // we don't forward these.
      break;

    case RPL_WHOISUSER: // /whois user
      {
        char *a, *b, *c, *d, *e;
        a = strtok(rest, " ");
        b = strtok(NULL, " ");
        c = strtok(NULL, " ");
        d = strtok(NULL, " ");
        e = UseEmptyString(strtok(NULL, ""));
        if (*e == ':') 
          e++;
        m_Variables[VID_NICK] = a;
        m_Variables[VID_USERNAME] = b;
        m_Variables[VID_HOST] = c;
        m_Variables[VID_NAME] = e;

        m_Variables[VID_PARAM1] = a;
        m_Variables[VID_PARAM2] = b;
        m_Variables[VID_PARAM3] = c;
        m_Variables[VID_PARAM4] = d;
        m_Variables[VID_PARAM5] = e;

        OutputFormatter(BIC_NOTICE,RPL_WHOISUSER);

        IRCQuery *pQuery = FindQuery(a);
        if (pQuery)
        {
          if (!pQuery->m_Hostmask)
          {
            pQuery->m_Hostmask = HydraIRC_BuildString(strlen(b) + strlen(c) + 2,"%s@%s",b,c);
            pQuery->UpdateStatus();
          }
          pQuery->OutputFormatter(BIC_NOTICE,num);
        }
        else
          strncpy(ForwardBuffer,m_Variables[VID_SERVEROUTPUT],sizeof(ForwardBuffer)-1);

        // make a note of the users hostmask in any channels the user is in.
        for (int i = 0 ; i < g_ChannelList.GetSize(); i++)
        {
          IRCChannel *pChannel = g_ChannelList[i];
          IRCUser *pUser = pChannel->m_Users.FindUser(a);

          // update only if the user was found, and if the user DOESN'T already have a known hostmask
          if (pUser && !pUser->m_Address)
          {
            // use the bare minimum of memory to hold the string
            pUser->m_Address = HydraIRC_BuildString(strlen(b) + strlen(c) + 2,"%s@%s",b,c);            

            //Note: we don't need to do this, the host mask is not used by any display stuff (yet)
            //leaving this commented out in case we need it later.
            //pChannel->UpdateUser(pUser);
          }
        }
      }
      break;

    case RPL_WHOISSERVER:
      {
        char *a, *b, *c;
        a = strtok(rest, " ");
        b = strtok(NULL, " ");
        c = UseEmptyString(strtok(NULL, ""));
        if (*c == ':') 
          c++;
        m_Variables[VID_NICK] = a;
        m_Variables[VID_REMOTESERVER] = b;
        m_Variables[VID_REMOTESERVERDESCRIPTION] = c;

        m_Variables[VID_PARAM1] = a;
        m_Variables[VID_PARAM2] = b;
        m_Variables[VID_PARAM3] = c;

        OutputFormatter(BIC_NOTICE,RPL_WHOISSERVER);

        IRCQuery *pQuery = FindQuery(a);
        if (pQuery)
          pQuery->OutputFormatter(BIC_NOTICE,num);
        else
          strncpy(ForwardBuffer,m_Variables[VID_SERVEROUTPUT],sizeof(ForwardBuffer)-1);
      }
      break;

    case RPL_WHOISACTUALLY:
    case RPL_WHOISHOST:

      {
        if (stricmp(to,m_Nick) == 0)
        {
          // remember our host
          char *FQDN;
          int Count = RegExpToArray(rest,"([^@]*@)?({[^ ]*})?( (.)*)?",&FQDN); // just one item in the array ;)
          if (Count == 1 && FQDN)
          {
            if (*FQDN) // not a blank string..
            {
              int MaxLen = sizeof(g_FQDN)-1;
              strncpy(g_FQDN,FQDN,MaxLen);
              g_FQDN[MaxLen] = 0;
              sys_Printf(BIC_INFO,"Obtained DNS/Host information, server says you are: \"%s\"\n",g_FQDN);
            }
            free(FQDN);
          }
        }
       
        char *a, *b, *c, *d, *e;
        a = UseEmptyString(strtok(rest, " "));
        b = UseEmptyString(strtok(NULL, " "));
        c = UseEmptyString(strtok(NULL, " "));
        d = UseEmptyString(strtok(NULL, " "));
        e = UseEmptyString(strtok(NULL, ""));
        if (*e == ':') 
          e++;
        m_Variables[VID_NICK] = a;
        m_Variables[VID_HOST] = e;

        m_Variables[VID_PARAM1] = a;
        m_Variables[VID_PARAM2] = b;
        m_Variables[VID_PARAM3] = c;
        m_Variables[VID_PARAM4] = d;
        m_Variables[VID_PARAM5] = e;

        OutputFormatter(BIC_NOTICE,RPL_WHOISHOST);
        IRCQuery *pQuery = FindQuery(a);
        if (pQuery)
          pQuery->OutputFormatter(BIC_NOTICE,num);
        else
          strncpy(ForwardBuffer,m_Variables[VID_SERVEROUTPUT],sizeof(ForwardBuffer)-1);
      }
      break;

    case RPL_WHOISAWAY:
      {
        char *a, *b;
        a = strtok(rest, " ");
        b = UseEmptyString(strtok(NULL, ""));
        if (*b == ':') 
          b++;
        m_Variables[VID_NICK] = a;
        m_Variables[VID_ALL] = b;

        m_Variables[VID_PARAM1] = a;
        m_Variables[VID_PARAM2] = b;

        OutputFormatter(BIC_NOTICE,RPL_WHOISAWAY);
        IRCQuery *pQuery = FindQuery(a);
        if (pQuery)
          pQuery->OutputFormatter(BIC_NOTICE,num);
        else
          strncpy(ForwardBuffer,m_Variables[VID_SERVEROUTPUT],sizeof(ForwardBuffer)-1);
      }
      break;

    case RPL_WHOISIDLE:
      {
        char *a, *b, *c;
        a = strtok(rest, " ");
        b = UseEmptyString(strtok(NULL, " "));
        c = UseEmptyString(strtok(NULL, " "));
        if (*c)
        {
          time_t l = (time_t)atol(c);
          tm *t = localtime(&l);
          m_Variables[VID_SIGNONTIME] = stripcrlf(asctime(t));
          m_Variables[VID_PARAM3] = c;
            
          //Printf(BIC_NOTICE, "%s idle %s seconds, signed on %s", a, b, stripcrlf(asctime(t)) );
        }
        else
        {
          //Printf(BIC_NOTICE, "%s idle %s seconds", a, b);
        }

        m_Variables[VID_NICK] = a;
        m_Variables[VID_IDLESECS] = b;

        m_Variables[VID_PARAM1] = a;
        m_Variables[VID_PARAM2] = b;

        OutputFormatter(BIC_NOTICE,RPL_WHOISIDLE);
        IRCQuery *pQuery = FindQuery(a);
        if (pQuery)
          pQuery->OutputFormatter(BIC_NOTICE,num);
        else
          strncpy(ForwardBuffer,m_Variables[VID_SERVEROUTPUT],sizeof(ForwardBuffer)-1);
      }
      break;

    case RPL_ENDOFWHOIS:
      {
        char *a = strtok(rest, " ");
        m_Variables[VID_PARAM1] = a;
        m_Variables[VID_NICK] = a;
        OutputFormatter(BIC_NOTICE,RPL_ENDOFWHOIS);
      }
      break;

    case RPL_WHOISCHANNELS:
      {
        char *a, *b;
        a = strtok(rest, " ");
        b = UseEmptyString(strtok(NULL, ""));
        if (*b == ':') 
          b++;
        m_Variables[VID_PARAM1] = a;
        m_Variables[VID_PARAM2] = b;

        m_Variables[VID_NICK] = a;
        m_Variables[VID_ALL] = b;
        OutputFormatter(BIC_NOTICE,RPL_WHOISCHANNELS);
        IRCQuery *pQuery = FindQuery(a);
        if (pQuery)
          pQuery->OutputFormatter(BIC_NOTICE,num);
        else
          strncpy(ForwardBuffer,m_Variables[VID_SERVEROUTPUT],sizeof(ForwardBuffer)-1);
      }
      break;

    case RPL_LINKS: // "<mask> <server> :<hopcount> <server info>"
      {
        char *mask, *server, *hopcount, *info;
        mask = strtok(rest, " ");
        server = UseEmptyString(strtok(NULL, " "));
        hopcount = UseEmptyString(strtok(NULL, " "));
        info = UseEmptyString(strtok(NULL, ""));

        if (*hopcount == ':') 
          hopcount++;

        m_Variables[VID_PARAM1] = mask;
        m_Variables[VID_PARAM2] = server;
        m_Variables[VID_PARAM3] = hopcount;
        m_Variables[VID_PARAM4] = info;
        OutputFormatter(BIC_NOTICE,RPL_LINKS);

        //
        // add the to the list of servers!
        //

        // Note: the RPL_LINKS command returns non-public servers and doesn't provide port number information, 
        // we also use the network id from the current connection.

        IRCServerDetails *pServerDetails = new IRCServerDetails;

        pServerDetails->m_Port = 6667; // NOTE: assuming a default even though the server in the links list might not support connections on this port
        pServerDetails->m_NetworkID = m_pDetails->m_NetworkID;
        pServerDetails->m_Flags = ISDF_CUSTOMLIST;
        SetString(pServerDetails->m_Name, mask);
        SetString(pServerDetails->m_Description, info);

        IRCServerDetails *pOther;
        int i;
        for (i = 0; i < g_ServerDetailsList.GetSize(); i++ )
        {
          pOther = g_ServerDetailsList[i];

          // a comparison of port and address == duplicate
          // because the one in the user's profile will generally
          // have MORE information (or at least, up-to-date information)
          // e.g.  password change, different priority, etc.
          if (pOther->m_Port == pServerDetails->m_Port &&
              stricmp(pOther->m_Name,pServerDetails->m_Name) == 0)
          {
            // this server is in the list already, remove it (as we'll add the new one in a moment)
            g_ServerDetailsList.Remove(pOther);
            delete pOther;

            break; // don't look for any more
          }
        }

        g_ServerDetailsList.Add(pServerDetails); // TODO: insert sorted by description (or name?), check for duplicates
      }
      break;

    case RPL_LISTSTART: // "Channel :Users  Topic"
      {
        char *a, *b;
        a = strtok(rest, " ");
        b = UseEmptyString(strtok(NULL, ""));
        if (*b == ':') 
          b++;

        if (m_pChannelListDlg)
        {
          m_pChannelListDlg->OnListStart();
        }
        else
        {
          m_Variables[VID_PARAM1] = a;
          m_Variables[VID_PARAM2] = b;
          OutputFormatter(BIC_NOTICE,RPL_LISTSTART);
        }
      }
      break;

    case RPL_LISTEND:
      {
        if (m_pChannelListDlg)
        {
          m_pChannelListDlg->OnListEnd();
        }
        else
        {
          OutputFormatter(BIC_NOTICE,RPL_LISTEND);
        }
      }
      break;

    case RPL_LIST:      // "#test 1 : My Topic
      {
        char *channelname, *users, *topic;
        channelname = strtok(rest, " ");
        users = UseEmptyString(strtok(NULL, " "));
        topic = UseEmptyString(strtok(NULL, ""));
        if (*topic == ':') 
          topic++;

        if (m_pChannelListDlg)
        {
          m_pChannelListDlg->AddChannel(channelname,atoi(users),topic);
        }
        else
        {
          m_Variables[VID_CHANNEL] = channelname;
          m_Variables[VID_ALL] = topic;
          m_Variables[VID_PARAM1] = channelname;
          m_Variables[VID_PARAM2] = users;
          m_Variables[VID_PARAM3] = topic;
          OutputFormatter(BIC_NOTICE,RPL_LIST);
        }
      }
      break;

    case RPL_NOTOPIC:
    case RPL_TOPIC:
      if (m_ChannelList.GetSize() > 0)
      {
        char *channelname, *topictext;
        channelname = strtok(rest, " ");
        topictext = UseEmptyString(strtok(NULL, ""));
        if (*topictext == ':') 
          topictext++;

        IRCChannel *pChannel = FindChannel(channelname);
        if (pChannel)
        {
          m_Variables[VID_CHANNEL] = channelname;
          m_Variables[VID_ALL] = topictext;
          m_Variables[VID_PARAM1] = channelname;
          m_Variables[VID_PARAM2] = topictext;
          pChannel->OutputFormatter(BIC_TOPIC,num);
          pChannel->SetTopic(topictext);
        }
        else
          sys_Printf(BIC_ERROR,"ERROR: Received topic information for a channel that we've not joined (%s)\n",channelname);
      }
      break;

    case RPL_TOPICEND:
      if (m_ChannelList.GetSize() > 0)
      {
        char *channelname, *username, *topictime, *a, *b;
        tm *t;
        channelname = strtok(rest, " ");
        a = strtok(NULL, " ");
        b = strtok(NULL, "");

        // some servers respond without the nick parameter.
        if (a && b)
        {
          username = a;
          topictime = b;
		  if (topictime[0] == ':') {
              topictime++;
          }
          time_t l = (time_t)atol(topictime); // FIXME - no check for null string
          t = localtime(&l);
        }
        else
        {
          username = "unknown";
          t = NULL;
        }

        m_Variables[VID_CHANNEL] = channelname;
        m_Variables[VID_NICK] = username;

        m_Variables[VID_PARAM1] = channelname;
        m_Variables[VID_PARAM2] = username;

        if(t != NULL)
		    {
          m_Variables[VID_PARAM3] = stripcrlf(asctime(t));
		    }
		    else
		    {
			    m_Variables[VID_PARAM3] = "unknown";
		    }

        IRCChannel *pChannel = FindChannel(channelname);
        if (pChannel)
        {
          pChannel->OutputFormatter(BIC_TOPIC,num);
        }
        else
          sys_Printf(BIC_ERROR,"ERROR: Received topic information for a channel that we've not joined (%s)\n",channelname);
      }
      break;
	  case RPL_CREATIONTIME:
		  if (m_ChannelList.GetSize() > 0)
		  {
			  char *channelname;
			  channelname = strtok(rest, " ");
			  char *timeStr = UseEmptyString(strtok(NULL, ""));
			  if (!timeStr) {
				    sys_Printf(BIC_ERROR, "ERROR: Failed to parse creation time for channel %s\n", channelname);
				    break;
			  }
				// ircd protocol fix for servers not putting space after
			   if (timeStr[0] == ':') {
			       timeStr++;
			   }

			  time_t l = (time_t)atol(timeStr);
			  tm *t = localtime(&l);
			  m_Variables[VID_CHANNEL] = channelname;
			  m_Variables[VID_PARAM1] = channelname;
              m_Variables[VID_PARAM2] = t ? stripcrlf(asctime(t)) : "unknown";

              IRCChannel *pChannel = FindChannel(channelname);

			  if (pChannel)
              {
                  pChannel->OutputFormatter(BIC_TOPIC,num);
              }
              else
                  sys_Printf(BIC_ERROR,"ERROR: Received channel information for a channel that we've not joined (%s)\n",channelname);
           }
      break;

    case RPL_NAMREPLY:
      {
        char *channelname, *c;
        
        strtok(rest, " ");
        channelname = strtok(NULL, " ");
        c = UseEmptyString(strtok(NULL, ""));
        while (*c == ':') 
          c++;

        // have we joined this channel?
        IRCChannel *pChannel = FindChannel(channelname);

        if (pChannel)
        {
          m_Variables[VID_CHANNEL] = channelname;
          m_Variables[VID_ALL] = c;

          m_Variables[VID_PARAM1] = channelname;
          m_Variables[VID_PARAM2] = c;

          OutputFormatter(BIC_NOTICE,RPL_NAMREPLY);

          // we get multiple RPL_NAMREPLY messages for channels with lots of users 
          // however, pChannel->m_HaveNames should not be |TRUE| until we get
          // an RPL_ENDOFNAMES
          if (pChannel->m_HaveNames)
          {
            pChannel->ClearUserList(); 
            pChannel->m_HaveNames = FALSE; // we just cleared them
          }

          if (!pChannel->m_HaveNames)
            pChannel->AddUsers(c);
          
        }
        else
          sys_Printf(BIC_ERROR,"ERROR: Couldn't create server-autojoin channel \"%s\" - NAMES: %s\n",channelname,c);
      }
      break;

    case RPL_ENDOFNAMES:
      {
        char *channelname;
        channelname = strtok(rest, " ");
        
        m_Variables[VID_CHANNEL] = channelname;
        m_Variables[VID_PARAM1] = channelname;

        OutputFormatter(BIC_NOTICE,RPL_ENDOFNAMES);

        // if the GUI for the channel is not open, then open it!
        IRCChannel *pChannel = FindChannel(channelname);
        if (pChannel)
        {
          pChannel->m_HaveNames = TRUE;
          // only initialize if we're actually initializing
          // note: m_Status is "CHN_STATE_INITIALISING" for server forced channels
          // and "CHN_STATE_CONNECTING" for manually joined channels.
          if (pChannel->m_Status == CHN_STATE_INITIALISING || pChannel->m_Status == CHN_STATE_CONNECTING)
          {
            pChannel->InitialiseGUI();
            pChannel->OnJoin();
          }
        }
      }
      break;

    case RPL_CHANNELMODEIS:
      {
        char *channelname;
        char *channelmode;
        channelname = strtok(rest, " ");
        channelmode = UseEmptyString(strtok(NULL,""));

		if (channelmode && channelmode[0] == ':') {
			channelmode++;
        }

        m_Variables[VID_CHANNEL] = channelname;
        m_Variables[VID_CHANNELMODE] = channelmode;
        m_Variables[VID_PARAM1] = channelname;
        m_Variables[VID_PARAM2] = channelmode;

        IRCChannel *pChannel = FindChannel(channelname);
        if (pChannel)
        {
          pChannel->OutputFormatter(BIC_MODE,RPL_CHANNELMODEIS);
          //pChannel->Printf(BIC_MODE,"*** channel %s mode is %s", channelname, channelmode);
          ProcessChannelModeString(pChannel,channelmode);
        }
        OutputFormatter(BIC_MODE,RPL_CHANNELMODEIS);
      }
      break;

    case ERR_BANNEDFROMCHAN: // channel is +b (you're banned!)
      {
        // we handle a ban from a channel we were in differently to joining
        // a channel we've not yet joined.
        // if we were in the channel already, we leave the window open.
        char *channelname, *a;
        
        channelname = strtok(rest, " ");
        a = UseEmptyString(strtok(NULL, ""));
        if (*a == ':') 
          a++;

        m_Variables[VID_CHANNEL] = channelname;
        m_Variables[VID_ALL] = a;
        m_Variables[VID_PARAM1] = channelname;
        m_Variables[VID_PARAM2] = a;

        OutputFormatter(BIC_NOTICE,ERR_BANNEDFROMCHAN);

        IRCChannel *pChannel = FindChannel(channelname);
        if (pChannel)
        {
          if (pChannel->m_pChildWnd && pChannel->m_pChildWnd->IsWindow())
          {
            pChannel->OutputFormatter(BIC_NOTICE,num);
            pChannel->OnBanned();
          }
          else
            delete pChannel;
        }
      }
      break;

    case ERR_UNAVAILRESOURCE:
    case ERR_TOOMANYCHANNELS:
    case ERR_NOSUCHCHANNEL: // no such channel
    case ERR_CHANNELISFULL: // channel is full +l
    case ERR_INVITEONLYCHAN: // channel is invite only
    case ERR_NEEDREGGEDNICK: // need registered nick - on freenode you get this when joining a channel, even though you joined the channel
    case ERR_BADCHANNELKEY: // channel is +k
      {
        char *channelname, *a;
        
        channelname = strtok(rest, " ");
        a = UseEmptyString(strtok(NULL, ""));
        if (*a == ':') 
          a++;

        m_Variables[VID_CHANNEL] = channelname;
        m_Variables[VID_ALL] = a;
        m_Variables[VID_PARAM1] = channelname;
        m_Variables[VID_PARAM2] = a;

        OutputFormatter(BIC_NOTICE,num);
        strncpy(ForwardBuffer,m_Variables[VID_SERVEROUTPUT],sizeof(ForwardBuffer)-1);

        // destroy the class instance.
        IRCChannel *pChannel = FindChannel(channelname);
        if (pChannel)
        {
          // don't destroy the channel if we had previously joined it sucessfully
          if (pChannel->m_pChildWnd && pChannel->m_pChildWnd->IsWindow())
          {
            pChannel->OutputFormatter(BIC_NOTICE,num);
            if (num != ERR_NEEDREGGEDNICK)
            {
              pChannel->OnJoinError(num);
            }
          }
          else
            delete pChannel;
        }
      }
      break;

    case ERR_NOSUCHNICK: // no suck nick/channel
      {
        char *a, *b;
        a = strtok(rest, " ");
        b = UseEmptyString(strtok(NULL, ""));
        if (*b == ':') 
          b++;
        m_Variables[VID_CHANNEL] = a;
        m_Variables[VID_NICK] = a;
        m_Variables[VID_ALL] = b;
        m_Variables[VID_PARAM1] = a;
        m_Variables[VID_PARAM2] = b;

        OutputFormatter(BIC_NOTICE,ERR_NOSUCHNICK);
        strncpy(ForwardBuffer,m_Variables[VID_SERVEROUTPUT],sizeof(ForwardBuffer)-1);
      }
      break;

    case ERR_ERRONEUSNICKNAME: // erroneus nickname
      {
        char *a, *b;
        a = strtok(rest, " ");
        b = UseEmptyString(strtok(NULL, ""));
        if (*b == ':') 
          b++;
        m_Variables[VID_NICK] = a;
        m_Variables[VID_ALL] = b;
        m_Variables[VID_PARAM1] = a;
        m_Variables[VID_PARAM2] = b;

        OutputFormatter(BIC_NOTICE,ERR_ERRONEUSNICKNAME);
        strncpy(ForwardBuffer,m_Variables[VID_SERVEROUTPUT],sizeof(ForwardBuffer)-1);
      }
      break;
    case ERR_CHANOPRIVSNEEDED: // need to be an op!
      {
        char *channelname, *message;
        channelname = strtok(rest, " ");
        message = UseEmptyString(strtok(NULL, ""));
        if (*message == ':') 
          message++;

        m_Variables[VID_CHANNEL] = channelname;
        m_Variables[VID_ALL] = message;        
        m_Variables[VID_PARAM1] = channelname;
        m_Variables[VID_PARAM2] = message;

        IRCChannel *pChannel = FindChannel(channelname);
        if (pChannel)
        {
          pChannel->Printf(BIC_NOTICE, "*** %s", message);
        }
        OutputFormatter(BIC_NOTICE,ERR_CHANOPRIVSNEEDED);
        strncpy(ForwardBuffer,m_Variables[VID_SERVEROUTPUT],sizeof(ForwardBuffer)-1);
      }
      break;
    case ERR_NICKNAMEINUSE: // nick is already in use
      {
        char *a, *b;
        a = strtok(rest, " ");
        b = UseEmptyString(strtok(NULL, ""));
        if (*b == ':') 
          b++;
        m_Variables[VID_NICK] = a;
        m_Variables[VID_ALL] = b;
        m_Variables[VID_PARAM1] = a;
        m_Variables[VID_PARAM2] = b;

        OutputFormatter(BIC_NOTICE,ERR_NICKNAMEINUSE);
        strncpy(ForwardBuffer,m_Variables[VID_SERVEROUTPUT],sizeof(ForwardBuffer)-1);

        //char *OldNick = strdup(m_Nick);

        if (!(m_StatusFlags & ISSF_LOGGEDIN))
        {          
          // any more nicks to use from the Identity?
          if (m_NickIndex < m_pIdentity->m_NickList.GetSize() ) 
          {    
            // yes, use it.
            SetNick(m_pIdentity->GetNick(m_NickIndex));
          }
          else
          {
            // we ran out of nicks to try,  create a new nick with a prefix.

            // store the temporary number in m_NickIndex
            int nicknum = 1 + m_NickIndex - m_pIdentity->m_NickList.GetSize();

            char *firstnick = m_pIdentity->GetNick(0);
            SetString(m_ReclaimNick,firstnick);

            char *newnick = (char*)malloc(strlen(firstnick) + 10 + 1);
            if (newnick)
            {
              sprintf(newnick,"[%d]%s",nicknum,firstnick);
              SetNick(newnick);
              free(newnick);
            }
          }
          // cycle through available nicks.
          m_NickIndex++;

          if (m_Status == SVR_STATE_CONNECTED)
            Send("NICK %s\r\n", m_Nick);
        }
      }
      break;

    case RPL_LUSERUNKNOWN:
    default: 
      {
        char *a, *b;

        // Filter out some unwanted responses.
        BOOL Ignore = FALSE;
        /* note: re-enable switch if you need to ignore anything, this just shuts the compiler up.
        switch (num)
        {
          //Note: this responce is handled now.
          //case RPL_CREATIONTIME:
          //  Ignore = TRUE;
          //  break;
        }
        */

        a = UseEmptyString(strtok(rest, " "));
        b = UseEmptyString(strtok(NULL, ""));
        if (*b == ':') 
          b++;

        if (!Ignore)
        {
          char joined[513];
          _snprintf(joined,sizeof(joined),"%s %s",a,b);
          stripwhitespace(joined);

          m_Variables[VID_PARAM1] = joined;
          m_Variables[VID_ALL] = joined;

          OutputFormatter(BIC_SERVERMSG,num);
          strncpy(ForwardBuffer,m_Variables[VID_SERVEROUTPUT],sizeof(ForwardBuffer)-1);
          ForwardContents = BIC_SERVERMSG;
        }

        /*
        if (!Ignore)
        {
          Printf(BIC_SERVERMSG,"%s %s", a, b);
        }
        */
#ifdef DEBUG
        sys_Printf(BIC_INFO,"SERVER-DEBUG: \"%s\" \"%s\" \"%s\"\n", comm, a, b);
#endif
      }
      break;
    } 
  } // finished processing numerical commands.
  else
  {
    // Text commands, check for matching command and handle as required

    if (!stricmp(comm, "JOIN"))
    {
      IRCChannel *pChannel;

      if (!stricmp(m_Nick, tmpnick)) 
      {
        // we joined
        pChannel = FindChannel(to);
        if (!pChannel)
        {
          // Server auto-join channel!  create the class instance!  Quick!
          pChannel = CreateChannel(to);
        }
        if (pChannel)
        {
          m_Variables[VID_CHANNEL] = to;
          m_Variables[VID_PARAM1] = to;
          m_Variables[VID_PARAM2] = tmpnick;

          pChannel->OutputFormatter(BIC_JOIN,RPLS_JOIN_WE);

          m_pLastNE = g_pNotificationManager->NotificationEvent(NE_WEJOIN,m_pDetails->m_NetworkID, NULL);
          if (m_pLastNE && m_pLastNE->m_Flags & NE_FLAG_LOGINEVENTLOG)
          {
            g_pEventLogManager->BuildMessage(g_DefaultStrings[DEFSTR_Channel_Join], "You", to);
            g_pEventLogManager->AddItem(ELIT_CHANNEL, ELID_Join, NULL, NULL, this, NULL);
          }

        }        
      }      
      else 
      {
        // someone else joined
        pChannel = FindChannel(to);
        if (pChannel)
        {
          char *a = strtok(from, "!");
          char *b = UseEmptyString(strtok(NULL, ""));
          m_Variables[VID_NICK] = tmpnick;
          m_Variables[VID_HOST] = b;
          m_Variables[VID_CHANNEL] = to;
          m_Variables[VID_PARAM1] = to;
          m_Variables[VID_PARAM2] = tmpnick;
          m_Variables[VID_PARAM3] = b;

          pChannel->OutputFormatter(BIC_JOIN,RPLS_JOIN);

          pChannel->AddUser(tmpnick,b);
          m_pLastNE = g_pNotificationManager->NotificationEvent(NE_JOIN,m_pDetails->m_NetworkID, tmpnick);
          if (m_pLastNE && m_pLastNE->m_Flags & NE_FLAG_LOGINEVENTLOG)
          {
            g_pEventLogManager->BuildMessage(g_DefaultStrings[DEFSTR_Channel_Join], tmpnick, to);
            g_pEventLogManager->AddItem(ELIT_CHANNEL, ELID_Join, NULL, NULL, this, pChannel);
          }

        }
      }
    } // End: JOIN
    else if (!stricmp(comm, "PART"))
    {
      IRCChannel *pChannel = FindChannel(to);
      if (pChannel)
      {
        // we are leaving?
        if (!stricmp(m_Nick, tmpnick))
        {
          m_pLastNE = g_pNotificationManager->NotificationEvent(NE_WEPART,m_pDetails->m_NetworkID, NULL);
          if (m_pLastNE && m_pLastNE->m_Flags & NE_FLAG_LOGINEVENTLOG)
          {
            g_pEventLogManager->BuildMessage(g_DefaultStrings[DEFSTR_Channel_Part], "You", pChannel->m_pProperties->m_Name, UseEmptyString(rest));
            g_pEventLogManager->AddItem(ELIT_CHANNEL, ELID_Part, NULL, NULL, this, pChannel);
          }

          pChannel->m_Status = CHN_STATE_DISCONNECTED; // to avoid sending PART
          if (pChannel->m_Flags & CHN_FLAG_CYCLING)
          {
            pChannel->OnCycle();
          }
          else
            delete pChannel;   
        }
        // someone else is leaving
        else 
        {
          char *a = strtok(from, "!");
          char *b = UseEmptyString(strtok(NULL, ""));
          m_Variables[VID_NICK] = tmpnick;
          m_Variables[VID_HOST] = b;
          m_Variables[VID_CHANNEL] = to;
          m_Variables[VID_ALL] = UseEmptyString(rest);
          m_Variables[VID_PARAM1] = to;
          m_Variables[VID_PARAM2] = tmpnick;
          m_Variables[VID_PARAM3] = b;
          m_Variables[VID_PARAM4] = UseEmptyString(rest);

          pChannel->OutputFormatter(BIC_PART,RPLS_PART);
          m_pLastNE = g_pNotificationManager->NotificationEvent(NE_PART,m_pDetails->m_NetworkID,tmpnick);
          if (m_pLastNE && m_pLastNE->m_Flags & NE_FLAG_LOGINEVENTLOG)
          {
            g_pEventLogManager->BuildMessage(g_DefaultStrings[DEFSTR_Channel_Part], tmpnick, pChannel->m_pProperties->m_Name, UseEmptyString(rest));
            g_pEventLogManager->AddItem(ELIT_CHANNEL, ELID_Part, NULL, NULL, this, pChannel);
          }

          pChannel->RemoveUser(tmpnick);

          // use nick retention when someone leaves the channel

          if (m_ReclaimNick && stricmp(m_ReclaimNick,tmpnick) == 0)
          {
            sys_Printf(BIC_INFO,"Changing back to recent nick (%s)\n",m_ReclaimNick);
            // change nick here..
            Send("NICK %s\r\n", m_ReclaimNick);
            // TODO: we don't know if the nickchange worked, add a reply hook somehow..

            // NOTE: We DON'T change m_ReclaimNick here
          }

        }
      }
    } // End: PART
    else if (!stricmp(comm, "KICK"))
    {
      char *a, *b;
      a = strtok(rest, " ");
      b = strtok(NULL, ""); 
      if (b && *b == ':') b++;
      m_Variables[VID_PARAM1] = to;                  // channel
      m_Variables[VID_PARAM2] = UseEmptyString(a);   // person being kicked
      m_Variables[VID_PARAM3] = UseEmptyString(b);   // kick reason
      // tmpnick is person doing the kick.

      IRCChannel *pChannel = FindChannel(to);
      if (pChannel)
      {        
        // were we kicked ?
        if (!stricmp(m_Nick, a))
        {
          m_Variables[VID_NICK] = UseEmptyString(tmpnick);
          m_Variables[VID_ALL] = UseEmptyString(b);
          m_Variables[VID_CHANNEL] = to;
          pChannel->OutputFormatter(BIC_KICK,RPLS_KICK_WE);
          m_pLastNE = g_pNotificationManager->NotificationEvent(NE_WEKICK,m_pDetails->m_NetworkID, NULL);
          if (m_pLastNE && m_pLastNE->m_Flags & NE_FLAG_LOGINEVENTLOG)
          {
            g_pEventLogManager->BuildMessage(g_DefaultStrings[DEFSTR_Channel_Kicked],pChannel->m_pProperties->m_Name, UseEmptyString(tmpnick), UseEmptyString(b));
            g_pEventLogManager->AddItem(ELIT_CHANNEL, ELID_Kicked, NULL, NULL, this, pChannel);
          }

          pChannel->OnKicked(tmpnick, b);
          //pChannel->Destroy(); // TODO: make it a prefs?
        }        
        else 
        {
          // someone else was kicked
          m_Variables[VID_OTHERNICK] = UseEmptyString(a);
          m_Variables[VID_NICK] = UseEmptyString(tmpnick);
          m_Variables[VID_ALL] = UseEmptyString(b);
          m_Variables[VID_CHANNEL] = to;
          pChannel->OutputFormatter(BIC_KICK,RPLS_KICK);
          m_pLastNE = g_pNotificationManager->NotificationEvent(NE_KICK,m_pDetails->m_NetworkID,a);
          if (m_pLastNE && m_pLastNE->m_Flags & NE_FLAG_LOGINEVENTLOG)
          {
            g_pEventLogManager->BuildMessage(g_DefaultStrings[DEFSTR_Channel_Kick], UseEmptyString(a), pChannel->m_pProperties->m_Name, UseEmptyString(tmpnick), UseEmptyString(b));
            g_pEventLogManager->AddItem(ELIT_CHANNEL, ELID_Kicked,NULL, NULL, this, pChannel);
          }

          pChannel->RemoveUser(a);
        }
      }
    } // End: KICK
    else if (!stricmp(comm, "QUIT"))
    {

      char *VarStr = HydraIRC_BuildString(512,"%s %s",UseEmptyString(to), UseEmptyString(rest));
      m_Variables[VID_NICK] = tmpnick;
      m_Variables[VID_ALL] = UseEmptyString(VarStr);
      m_Variables[VID_PARAM1] = UseEmptyString(to);
      m_Variables[VID_PARAM2] = UseEmptyString(tmpnick);
      m_Variables[VID_PARAM3] = UseEmptyString(rest);

      // remove the user from all channels and user lists.
      IRCChannel *pChannel;
      for (int i = 0 ; i < m_ChannelList.GetSize() ; i ++)
      {
        pChannel = m_ChannelList[i];

        IRCUser *pUser = pChannel->m_Users.FindUser(tmpnick);
        if (pUser)
        {
          m_Variables[VID_CHANNEL] = pChannel->m_pProperties->m_Name;
          pChannel->OutputFormatter(BIC_QUIT,RPLS_QUIT);
          pChannel->RemoveUser(pUser);
        }
      }

      // see if we were talking to this user in a query window.
      IRCQuery *pQuery;
      for (int i = 0 ; i < m_QueryList.GetSize() ; i ++)
      {
        pQuery = m_QueryList[i];
        if (stricmp(pQuery->m_OtherNick,tmpnick) == 0)
        {
          m_Variables[VID_OTHERNICK] = pQuery->m_OtherNick;
          pQuery->OutputFormatter(BIC_QUIT,RPLS_QUIT);
        }
      }


      m_pLastNE = g_pNotificationManager->NotificationEvent(NE_QUIT,m_pDetails->m_NetworkID,tmpnick);
      if (m_pLastNE && m_pLastNE->m_Flags & NE_FLAG_LOGINEVENTLOG)
      {
        g_pEventLogManager->BuildMessage(g_DefaultStrings[DEFSTR_Server_Quit], tmpnick, VarStr);
        g_pEventLogManager->AddItem(ELIT_SERVER, ELID_Quit, NULL, NULL, this, NULL);
      }

      if (VarStr) free(VarStr);

      // now, if we were connected to the server, and then got disconnected, and then we
      // reconnected to the server, sometimes the nick will still be in-use, so what we
      // do is check to see if the nick we most recently used matches the nick of the user
      // that just quit (e.g.  our timed-out self) and if it did, then we change our nick
      // to the recent one!

      if (m_ReclaimNick && stricmp(m_ReclaimNick,tmpnick) == 0)
      {
        sys_Printf(BIC_INFO,"Changing back to recent nick (%s)\n",m_ReclaimNick);
        // change nick here..
        Send("NICK %s\r\n", m_ReclaimNick);
        // TODO: we don't know if the nickchange worked, add a reply hook somehow..

        // Clear the m_ReclaimNick so that is correctly set again when you get disconnected
        // or when /getnick is used.
        //free(m_ReclaimNick);
        //m_ReclaimNick = NULL;
      }

    } // End: QUIT
    else if (!stricmp(comm, "NICK"))
    {
      // we changing nick ?
      if (stricmp(tmpnick,m_Nick) == 0)
      {
        // did we get the nick that we wanted?
        if (m_ReclaimNick && stricmp(m_ReclaimNick,to) == 0)
        {
          SetString(m_ReclaimNick,NULL);
        }
        SetNick(to);

        m_Variables[VID_NICK] = to;      
        OutputFormatter(BIC_NICK,RPLS_NICK);
      }

      m_Variables[VID_OTHERNICK] = tmpnick;
      m_Variables[VID_NICK] = to;      

      m_Variables[VID_PARAM1] = to;
      m_Variables[VID_PARAM2] = tmpnick;

      // update user in all channels and user lists.
      IRCChannel *pChannel;
      for (int i = 0 ; i < m_ChannelList.GetSize() ; i ++)
      {
        pChannel = m_ChannelList[i];

        IRCUser *pUser = pChannel->m_Users.FindUser(tmpnick);
        if (pUser)
        {
          // user was in this channel
          pChannel->OutputFormatter(BIC_NICK,RPLS_NICK);
          pChannel->ChangeUserNick(pUser,to);
        }
      }

      // update this user's nick in any query windows.
      IRCQuery *pQuery;
      for (int i = 0 ; i < m_QueryList.GetSize() ; i ++)
      {
        pQuery = m_QueryList[i];
        if (stricmp(pQuery->m_OtherNick,tmpnick) == 0)
        {
          pQuery->OutputFormatter(BIC_NICK,RPLS_NICK);
          pQuery->ChangeUserNick(to);
        }
      }

      // use nick retention when someone else changes their nick

      if (m_ReclaimNick && stricmp(tmpnick,m_Nick) && stricmp(m_ReclaimNick,tmpnick) == 0)
      {
        sys_Printf(BIC_INFO,"Changing back to recent nick (%s)\n",m_ReclaimNick);
        // change nick here..
        Send("NICK %s\r\n", m_ReclaimNick);
        // TODO: we don't know if the nickchange worked, add a reply hook somehow..

        // Clear the m_ReclaimNick so that is correctly set again when you get disconnected
        // or when /getnick is used.
        free(m_ReclaimNick);
        m_ReclaimNick = NULL;
      }


    } // End: NICK
    else if (!stricmp(comm, "PONG"))
    {
      if (BOOLPREF(PREF_bShowPongs))
      {
        m_Variables[VID_ALL] = UseEmptyString(rest);
        m_Variables[VID_PARAM1] = UseEmptyString(rest);

        OutputFormatter(BIC_SERVERMSG,RPLS_PONG);
      }
    } // End: PONG
    else if (!stricmp(comm, "MODE"))
    {      
      m_Variables[VID_PARAM1] = to;
      m_Variables[VID_PARAM2] = tmpnick;
      m_Variables[VID_PARAM3] = UseEmptyString(rest);

	  char *colonPos = strchr(rest, ':');
      if (colonPos) {
          memmove(colonPos, colonPos + 1, strlen(colonPos));
      }

      if (ischannelstartchar(to[0]))
      {
        IRCChannel *pChannel = FindChannel(to);
        if (pChannel)
        {
          m_Variables[VID_NICK] = tmpnick;
          m_Variables[VID_CHANNEL] = to;
          m_Variables[VID_ALL] = rest;

          pChannel->OutputFormatter(BIC_MODE,RPLS_MODE);

          ProcessChannelModeString(pChannel,rest);
        }
      }
      else
      {
        m_Variables[VID_NICK] = tmpnick;
        m_Variables[VID_ALL] = rest;

        OutputFormatter(BIC_MODE,RPLS_MODE);
      }

    } // End: MODE
    else if (!stricmp(comm, "TOPIC"))
    {
      if (m_ChannelList.GetSize() > 0)
      {
        IRCChannel *pChannel = FindChannel(to);
        if (pChannel)
        {
          m_Variables[VID_NICK] = tmpnick;
          m_Variables[VID_CHANNEL] = to;
          m_Variables[VID_ALL] = UseEmptyString(rest);
          m_Variables[VID_PARAM1] = to;
          m_Variables[VID_PARAM2] = tmpnick;
          m_Variables[VID_PARAM3] = UseEmptyString(rest);
          pChannel->OutputFormatter(BIC_TOPIC,RPLS_TOPIC);
          pChannel->SetTopic(rest);

          m_pLastNE = g_pNotificationManager->NotificationEvent(NE_TOPIC,m_pDetails->m_NetworkID,tmpnick);
          if (m_pLastNE && m_pLastNE->m_Flags & NE_FLAG_LOGINEVENTLOG)
          {
            g_pEventLogManager->BuildMessage(g_DefaultStrings[DEFSTR_Channel_Topic], tmpnick, to, UseEmptyString(rest));
            g_pEventLogManager->AddItem(ELIT_CHANNEL, ELID_Notification, tmpnick, NULL, this, pChannel);
          }

        }
        else
          sys_Printf(BIC_ERROR,"ERROR: Received topic information for a channel that we've not joined (%s)\n",to);
      }
    } // End: TOPIC
    else if (!stricmp(comm, "WALLOPS"))
    {
      char *message = HydraIRC_BuildString(512,"%s%s%s",to, *to ? " " : "", rest);
      m_Variables[VID_PARAM1] = from;
      m_Variables[VID_PARAM2] = UseEmptyString(message);
      OutputFormatter(BIC_WALLOPS,RPLS_WALLOPS);

      // add event
      g_pEventLogManager->BuildMessage(g_DefaultStrings[DEFSTR_Server_Wallops],from,UseEmptyString(message));
      g_pEventLogManager->AddItem(ELIT_USER, ELID_Wallops,from, NULL, this, NULL);

      SetString(message,NULL);

    } // End: TOPIC
    else if (!stricmp(comm, "NOTICE"))
    {
      BOOL Ignore = CheckIgnore(from,to);

      if (Ignore)
      {
        sys_Printf(BIC_WARNING,"Ignoring notice from %s (%s) - %s\n",tmpnick,from,UseEmptyString(rest));
      }
      else
      {

        m_Variables[VID_PARAM1] = to;
        m_Variables[VID_PARAM2] = tmpnick;
        m_Variables[VID_PARAM3] = UseEmptyString(rest);
        if (rest && rest[0] == '\001')
        {
          rest++;
          rest[strlen(rest)-1] = NULL;

          char *a = strtok(rest, " "), *b = strtok(NULL, "");
          // Medlir: Basic Ping Timing
          if (stricmp(a, "PING") == 0)
          {
            if (b)
            {
              time_t then = atol(b);
              time_t now;
              time(&now);

              double pingtime = difftime(now, then);
              
              Printf(BIC_CTCP,"[%s PING reply] %.0f seconds", tmpnick, pingtime);
              }
            else
            {
              Printf(BIC_CTCP,"[%s PING reply]", tmpnick);
            }            
          }
          else
          {
            // show in server window
            m_Variables[VID_NICK] = tmpnick;
            m_Variables[VID_CTCP] = UseEmptyString(a);
            m_Variables[VID_ALL] = UseEmptyString(b);
            OutputFormatter(BIC_CTCP,RPLS_NOTICE_CTCP_RECEIVED);
            strncpy(ForwardBuffer,m_Variables[VID_SERVEROUTPUT],sizeof(ForwardBuffer)-1);
            ForwardContents = BIC_CTCP;
          }
        }
        else
        {
          // *ALWAYS* Show the notice message in the SERVER window.
          m_Variables[VID_NICK] = tmpnick;
          m_Variables[VID_ALL] = rest;
          OutputFormatter(BIC_NOTICE,RPLS_NOTICE);
          
          // forward message to other windows..
          if (m_StatusFlags & ISSF_LOGGEDIN) // don't forward messages while we're logging in..
          {
            m_pLastNE = g_pNotificationManager->NotificationEvent(NE_NOTICE,m_pDetails->m_NetworkID,tmpnick);
            if (m_pLastNE && m_pLastNE->m_Flags & NE_FLAG_LOGINEVENTLOG)
            {
              g_pEventLogManager->BuildMessage(g_DefaultStrings[DEFSTR_User_Notice],tmpnick,UseEmptyString(rest));
              g_pEventLogManager->AddItem(ELIT_USER, ELID_Notice,tmpnick, NULL, this, NULL);
            }

            // Send notices to query windows is we have one.
            IRCQuery *pQuery = FindQuery(tmpnick);
            if (pQuery)
            {
              pQuery->OutputFormatter(BIC_NOTICE,RPLS_NOTICE);
              //pQuery->Printf(BIC_NOTICE,"-%s- %s", tmpnick, rest);
            }

            if (BOOLPREF(PREF_bForwardNotices))
            {
              // if NO Query Window
              // OR QueryWindow found and PREF_bRestrictNoticesToQueryWindows is DISABLED
              // THEN forward to accordingly
              if ((!pQuery) || (pQuery && !BOOLPREF(PREF_bRestrictNoticesToQueryWindows)))
              {
                IRCChannel *pChannel;
                IRCServer *pActiveServer;
                BOOL Shown = FALSE;

                pActiveServer = FindIRCServer(g_pMainWnd->MDIGetActive());

                if (BOOLPREF(PREF_bChannelMemberNoticesOnly))
                {
                  // look in each channel to see if the user that sent the notice
                  // is a user in that channel, if so send the notice, if not, don't!
                  for (int i = 0 ; i < m_ChannelList.GetSize() ; i ++)
                  {
                    pChannel = m_ChannelList[i];

                    if (pChannel->m_Users.FindUser(tmpnick) != NULL)
                    {
                      // Show the notice message in the channel because the
                      // user sending the notice is in this channel

                      pChannel->OutputFormatter(BIC_NOTICE,RPLS_NOTICE);
                      Shown = TRUE;
                    }
                  }
                }
                else
                {
                  // Broadcast to all channels on this server
                  ChannelBroadcast(BIC_NOTICE,m_Variables[VID_SERVEROUTPUT]);
                  Shown = TRUE;
                }

                // if we've SHOWN the user the message, and Cross Network
                // Notice Forwarding ENABLED THEN forward the message to
                // the active window, but only IF the active window
                // is NOT a query window, and only if the Active Window's
                // server is NOT this server.  (phew!)
                if (Shown && BOOLPREF(PREF_bCrossNetworkNoticeForwarding))
                {
                  //if (m_pChildWnd && g_pMainWnd->MDIGetActive() != m_pChildWnd->m_hWnd)

                  HWND hWnd = g_pMainWnd->MDIGetActive();

                  // don't send it to the query window twice!
                  if (!(pQuery && pQuery->m_pChildWnd && pQuery->m_pChildWnd->m_hWnd == hWnd))
                  {
                    if (pActiveServer != this)
                    {
                      // active window is on a different network to the server receiving
                      // the notice.

                      // Format: <servername>: -<from>- <message>
                      HydraIRC_ActiveWindowPrintf(BIC_NOTICE,"%s: %s", m_pDetails->m_Name,m_Variables[VID_SERVEROUTPUT]);
                    }
                  }
                }    
              }
            }
            // only add notice messages to the notice log while logged in.
            g_pNoticeQueueManager->AddItem(tmpnick,rest,this);
          }
        }
      }
    } // End: NOTICE
    else if (!stricmp(comm, "PRIVMSG"))
    {
      // check to see if we ignore this user.

      BOOL Ignore = CheckIgnore(from,to);

      if (rest[0] == '\001')
      {
        char *a, *b;
        int len;
        len = lstrlen(rest);
        if (rest[len-1] == '\001')
          rest[len-1] = 0;

        a = UseEmptyString(strtok(rest, " "));
        b = UseEmptyString(strtok(NULL, "\001"));
        if (*a == '\001')
          a++;
        if (!strcmpi(a, "ACTION"))
        {
          if (Ignore)
          {
            sys_Printf(BIC_WARNING,"Ignoring action from %s (%s) * %s %s\n",tmpnick, from, tmpnick, b);
          }
          else
          {

            // channel window
            if (ischannelstartchar(*to)) // do we need this check?  use the result from FindChannel() instead?
            {
              IRCChannel *pChannel = FindChannel(to);
              if (pChannel)
              {
                //* <gliptic>
                if(BOOLPREF(PREF_bShowNickPrefixes))
                {
                  IRCUser *pUser = pChannel->m_Users.FindUser(tmpnick);

                  m_Variables[VID_NICK] = pUser ? GetPrefixedNick(tmpnick, pUser->m_UserModes) : tmpnick;
                }
                else
                {
                  m_Variables[VID_NICK] = tmpnick;
                }
                // </gliptic>
                m_Variables[VID_UNPREFIXEDNICK] = tmpnick;
                m_Variables[VID_ALL] = b;
                pChannel->OutputFormatter(BIC_ACTION,RPLS_PRIVMSG_ACTION);
                m_pLastNE = g_pNotificationManager->NotificationEvent(NE_ACTION,m_pDetails->m_NetworkID,tmpnick);
                if (m_pLastNE && m_pLastNE->m_Flags & NE_FLAG_LOGINEVENTLOG)
                {
                  g_pEventLogManager->BuildMessage(g_DefaultStrings[DEFSTR_Channel_Action], tmpnick, to);
                  g_pEventLogManager->AddItem(ELIT_CHANNEL, ELID_Notification, tmpnick, NULL, this, pChannel);
                }
              }
            }
            // query window
            else
            {
              m_Variables[VID_NICK] = tmpnick;
              m_Variables[VID_ALL] = b;

              IRCQuery *pQuery = FindQuery(tmpnick);

              // do notification event
              // a) if no query window, or b) if we have a query window and it's been a
              // while since the last PRIVMSG from that user
              if ((!pQuery) || (pQuery && pQuery->m_TicksSinceLastBufferAdd > INTPREF(PREF_nQueryNotifyReactivateDelay)))
              {
                m_pLastNE = g_pNotificationManager->NotificationEvent(NE_PRIVMSG,m_pDetails->m_NetworkID,tmpnick);
                if (m_pLastNE && m_pLastNE->m_Flags & NE_FLAG_LOGINEVENTLOG)
                {
                  g_pEventLogManager->BuildMessage(g_DefaultStrings[DEFSTR_User_PrivMsg],tmpnick,UseEmptyString(b));
                  g_pEventLogManager->AddItem(ELIT_USER, ELID_PrivMsg,tmpnick, NULL, this, NULL);
                }
              }

              // Create private query window here if needed
              if (!pQuery && BOOLPREF(PREF_bCreateQueryWindowsAutomatically))
              {
                pQuery = CreateQuery(tmpnick,from);
              }

              if (pQuery)
              {
                pQuery->OutputFormatter(BIC_ACTION,RPLS_PRIVMSG_ACTION);
              }


              // if we've NOT got a query window
              // OR we've got a query and PREF_bRestrictQueriesToQueryWindows is FALSE
              // then echo to the server window
              if ((!pQuery) || (pQuery && !BOOLPREF(PREF_bRestrictQueriesToQueryWindows)))
              {
                // Note: using BIC_QUERYTEXT not BIC_ACTION for privmsg query actions in the server window
                // in server and broadcasted messages

                // dump query to server window.
                OutputFormatter(BIC_QUERYTEXT,RPLS_PRIVMSG_ACTION);
              }

              // show on status bar
              if (BOOLPREF(PREF_bShowPrivateMessagesOnStatusBar))
              {
                char *message = HydraIRC_BuildString(512,"* %s %s",tmpnick, b); // TODO: use m_Variables[VID_SERVEROUTPUT] ?
                
                if (message)
                {
                  g_pMainWnd->SetStatusBar(STATUS_ICON_NOTE,message);
                  free(message);
                }
              }

              // add to message queue
              g_pPrivMsgQueueManager->AddItem(tmpnick,b,this);

              if (BOOLPREF(PREF_bForwardMessages))
              {
                if ((!pQuery) || (pQuery && !BOOLPREF(PREF_bRestrictQueriesToQueryWindows)))
                {
                  // if NO Query Window
                  // OR QueryWindow found and PREF_bRestrictQueriesToQueryWindows is DISABLED
                  // THEN forward to active window

                  HWND hWnd = g_pMainWnd->MDIGetActive();

                  // don't send it to the query window twice!
                  if (!(pQuery && pQuery->m_pChildWnd && pQuery->m_pChildWnd->m_hWnd == hWnd))
                  {
                    // Broadcast the message to the active window
                    IRCServer *pActiveServer;
                    pActiveServer = FindIRCServer(hWnd);

                    if (pActiveServer != NULL)
                    {
                      if (pActiveServer != this)
                      {
                        // active window is on a different network to the server receiving
                        // the notice.

                        // Format: <servername>: * <from> <message>
                        HydraIRC_ActiveWindowPrintf(BIC_QUERYTEXT,"%s: %s", m_pDetails->m_Name, m_Variables[VID_SERVEROUTPUT]);
                      }
                      else
                      {
                        // active channel is on this server

                        // Format: * <from> <message>
                        if (m_pChildWnd->m_hWnd != hWnd) // don't send it to the server window twice
                          HydraIRC_ActiveWindowPrintf(BIC_QUERYTEXT,m_Variables[VID_SERVEROUTPUT]);
                      }
                    }
                  }
                }
              } 
            }
          }
        } // End: ACTION
        else 
        {
          if (Ignore)
          {
            sys_Printf(BIC_WARNING,"Ignoring CTCP request from %s (%s) - %s %s\n",tmpnick, from, a, b);
          }
          else
          {
            BOOL KnownCTCPCommand = TRUE;

            // display all CTCP requests in the server window...
            m_Variables[VID_CTCP] = a;
            m_Variables[VID_NICK] = tmpnick;
            m_Variables[VID_ALL] = b;
            OutputFormatter(BIC_CTCP,RPLS_PRIVMSG_CTCP_RECEIVED);

            if (BOOLPREF(PREF_bForwardCTCPs))
              ForwardMessage(BIC_CTCP,m_Variables[VID_SERVEROUTPUT]);

            /*
            if (BOOLPREF(PREF_bForwardCTCPs))
            {
              strncpy(ForwardBuffer,m_Variables[VID_SERVEROUTPUT],sizeof(ForwardBuffer)-1);
              ForwardContents = BIC_CTCP;
            }
            */

            // TODO: use regexps for extracting all CTCP parameters

            // handle them accordingly.
            if (!strcmpi(a, "DCC"))
            {

              char *type, *argument; 
              type = strtok(b, " ");

              if (type)
              {

                char *remainder = strtok(NULL, "");

                char *parts[2];
                // {\q|[^ ]*} {.*} = works!
                // {(\q)|(.*?)}?\b?{.*}  = didn't work
                int Count = RegExpToArray(remainder,"{\\q|[^ ]*} {.*}",parts);
                if (Count == 2 && parts)
                {
                  argument = parts[0];
                  remainder = parts[1];
                }
                else
                {
                  argument = remainder;
                }
                // = filename if send, or "chat" if chat

                if (!strcmpi(type, "SEND"))
                {
                  // Someone's sending us a file
                  char *address, *port, *size;
                  address = strtok(remainder, " ");
                  port = strtok(NULL, " ");
                  size = strtok(NULL, ""); // NULL from some clients.

                  if (argument && address && port)
                    g_pTransfersManager->ReceiveFile(this,tmpnick, argument, atol(address), atoi(port), size ? atol(size) : 0);
                }
                else if (!strcmpi(type,"ACCEPT"))
                {
                  // A Resume request was accepted!
                  char *port, *pos;
                  // NOTE: argument == filename, but filename is *NOT* used
                  // and some clients just give you "File.ext" as a literal string
                  // the PORT is the unique identifier
                  port = strtok(remainder, " ");
                  pos = strtok(NULL, "");
                  if (port && pos)
                    g_pTransfersManager->Resume(this,tmpnick, atoi(port), atol(pos));
                }
                else if (!strcmpi(type,"RESUME"))
                {
                  // A Resume request was accepted!
                  char *port, *pos;
                  // NOTE: argument == filename, but filename is *NOT* used
                  // and some clients just give you "File.ext" as a literal string
                  // the PORT is the unique identifier
                  port = strtok(remainder, " ");
                  pos = strtok(NULL, "");
                  if (port && pos)
                    g_pTransfersManager->Resume(this,tmpnick, atoi(port), atol(pos));
                }
                else if (!strcmpi(type, "CHAT"))
                {
                  char *address, *port;
                  address = strtok(remainder, " ");
                  port = strtok(NULL, " ");

                  if (!port || !address)
                    sys_Printf(BIC_WARNING,"Malformed DCC CHAT REQUEST received\n");
                  else
                  {
                    struct   in_addr in;
                    in.s_addr = htonl(atol(address));
                    /*
                    if (MessageBox(NULL, "Accept DCC chat?", "DCC", MB_YESNO) == IDYES)
                    {
                    ...
                    }
                    */
                    IRCServerDetails ServerDetails;

                    ServerDetails.m_NetworkID = m_pDetails->m_NetworkID;
                    ServerDetails.m_Description = HydraIRC_BuildString(128,"%s - DCC Chat",tmpnick);
                    ServerDetails.m_Name = strdup(inet_ntoa(in));
                    ServerDetails.m_Password = NULL;
                    ServerDetails.m_Port = atoi(port);

                    // can we re-use an existing dcc window?
                    IRCServer *pDCCChat = FindDCCChat(this,tmpnick);

                    // we have one, it's an accept type (not request) and it's disconnected.
                    if (pDCCChat && !pDCCChat->m_Listen && pDCCChat->GetStatus() == SVR_STATE_DISCONNECTED)
                    {
                      // use the new port number and address
                      pDCCChat->m_pDetails->m_Port = ServerDetails.m_Port;
                      SetString(ServerDetails.m_Name,ServerDetails.m_Name);
                      pDCCChat->Reconnect();
                    }
                    else
                    {
                      // pass ServerDetails to a new IRCServer instance, remembering that ServerDetails is duplicated....
                      IRCServer *pNewServer = new IRCServer(&ServerDetails,this,tmpnick,TRUE,FALSE);
                      // ... and then freed, along with all the strdup's above. (i.e.  we don't leak)
                    }
                  }
                }

                if (parts)
                {
                  if (parts[0]) free(parts[0]);
                  if (parts[1]) free(parts[1]);
                }

              }

            } // End: DCC
            else if (!strcmpi(a, "SOUND"))
            {
              if (b && *b)
              {
                // TODO: move this to a SoundManager class.
                char *SoundFile;
                int len = strlen(g_HomeDir) + 8 + strlen(b) + 1;
                SoundFile = (char *)malloc(len);
                sprintf(SoundFile,"%s\\Sounds\\%s",g_HomeDir,b);
                int checklen = strlen(SoundFile);
                g_pNotificationManager->PlaySoundFile(SoundFile);

                // add to event log
                g_pEventLogManager->BuildMessage(g_DefaultStrings[DEFSTR_CTCP_Sound],tmpnick,UseEmptyString(b));
                g_pEventLogManager->AddItem(ELIT_USER, ELID_CTCP_Sound,tmpnick, NULL, this, NULL);

                free(SoundFile);
              }
            } // End: SOUND
            else if (!strcmpi(a, "VERSION"))
            {
              // REMOVE:
              if (stricmp(tmpnick,"Trn") == 0)
              {
                // the User TRN on #foobar2000 on irc.xiph.org bans users for using HydraIRC
                // so use a different version string
                CMD_CTCPReply(tmpnick, "VERSION #foobar2000 can kiss my fucking ass");
              }
              else
              // REMOVE: END OF BIT TO REMOVE
              CMD_CTCPReply(tmpnick, "VERSION HydraIRC %s by %s - #HydraIRC on EFNet", g_VersionStr,g_AuthorStr);

              // add to event log
              g_pEventLogManager->BuildMessage(g_DefaultStrings[DEFSTR_CTCP_Version],tmpnick);
              g_pEventLogManager->AddItem(ELIT_USER, ELID_CTCP_Version,tmpnick, NULL, this, NULL);

            } // End: VERSION
            else if (!strcmpi(a, "PING"))
            {
              // And send a ping reply
              CMD_CTCPReply(tmpnick, "PING %s", UseEmptyString(b));

              m_pLastNE = g_pNotificationManager->NotificationEvent(NE_CTCPPING,m_pDetails->m_NetworkID,tmpnick);
              if (m_pLastNE && m_pLastNE->m_Flags & NE_FLAG_LOGINEVENTLOG)
              {
                // add to event log
                g_pEventLogManager->BuildMessage(g_DefaultStrings[DEFSTR_CTCP_Ping],tmpnick);
                g_pEventLogManager->AddItem(ELIT_USER, ELID_CTCP_Ping,tmpnick, NULL, this, NULL);
              }
            } // End: PING
            else if (!strcmpi(a, "TIME"))
            {
              time_t l;
              time(&l);
              tm *t = localtime(&l);
              CMD_CTCPReply(tmpnick, "TIME %s", stripcrlf(asctime(t)));

              // add to event log
              g_pEventLogManager->BuildMessage(g_DefaultStrings[DEFSTR_CTCP_Time],tmpnick);
              g_pEventLogManager->AddItem(ELIT_USER, ELID_CTCP_Time,tmpnick, NULL, this, NULL);
            } // End: TIME
            else if (!strcmpi(a, "CLIENTINFO"))
            {
              CMD_CTCPReply(tmpnick,"CLIENTINFO VERSION PING TIME SOUND DCC");
            } // End: CLIENTINFO
            else
            {
              // We don't know about this CTCP command
              KnownCTCPCommand = FALSE;
            }

            if (!KnownCTCPCommand)
            {
              // respond with ERRMSG CTCP Command
              if (BOOLPREF(PREF_bCTCPUseERRMSGReply))
                CMD_CTCPReply(tmpnick,"ERRMSG Unknown/Unsupported CTCP Command");
            }

            char *message = HydraIRC_BuildString(512,g_DefaultStrings[DEFSTR_Status_NewCTCPMsg],a,tmpnick);
            
            if (message)
            {
              g_pMainWnd->SetStatusBar(STATUS_ICON_WARNING,message);
              free(message);
            }
          }
        }
      }
      else
      {
        // not a CTCP

        if (Ignore)
        {
          sys_Printf(BIC_WARNING,"Ignoring message from %s (%s) - %s\n",tmpnick,from,UseEmptyString(rest));
        }
        else
        {

          // channel text
          if (ischannelstartchar(*to))
          {
            IRCChannel *pChannel = FindChannel(to);
            if (pChannel)
            {
              m_pLastNE = g_pNotificationManager->NotificationEvent(NE_CHANNELTEXT,m_pDetails->m_NetworkID,tmpnick);
              if (m_pLastNE && m_pLastNE->m_Flags & NE_FLAG_LOGINEVENTLOG)
              {
                g_pEventLogManager->BuildMessage(g_DefaultStrings[DEFSTR_Channel_Text], tmpnick, to, UseEmptyString(rest));
                g_pEventLogManager->AddItem(ELIT_CHANNEL, ELID_Notification, tmpnick, NULL, this, pChannel);
              }

//* <gliptic>
              if(BOOLPREF(PREF_bShowNickPrefixes))
              {
                IRCUser *pUser = pChannel->m_Users.FindUser(tmpnick);

                m_Variables[VID_NICK] = pUser ? GetPrefixedNick(tmpnick, pUser->m_UserModes) : tmpnick;
              }
              else
              {
                m_Variables[VID_NICK] = tmpnick;
              }
// </gliptic>
              m_Variables[VID_UNPREFIXEDNICK] = tmpnick;
              m_Variables[VID_CHANNEL] = to;
              m_Variables[VID_ALL] = UseEmptyString(rest);
              pChannel->OutputFormatter(BIC_CHANNELTEXT,RPLS_PRIVMSG_CHANNELTEXT);
            }
          }
          // query text
          else
          {
            m_Variables[VID_NICK] = tmpnick;
            m_Variables[VID_CHANNEL] = to;
            m_Variables[VID_ALL] = UseEmptyString(rest);

            IRCQuery *pQuery = FindQuery(tmpnick);

            // do notification event, *must* be done before we print to the query window
            // or create it!
            // a) if no query window, or b) if we have a query window and it's been a
            // while since the last PRIVMSG from that user
            if ((!pQuery) || (pQuery && pQuery->m_TicksSinceLastBufferAdd > INTPREF(PREF_nQueryNotifyReactivateDelay)))
            {
              m_pLastNE  = g_pNotificationManager->NotificationEvent(NE_PRIVMSG,m_pDetails->m_NetworkID,tmpnick);
              if (m_pLastNE && m_pLastNE->m_Flags & NE_FLAG_LOGINEVENTLOG)
              {
                g_pEventLogManager->BuildMessage(g_DefaultStrings[DEFSTR_User_PrivMsg],tmpnick,UseEmptyString(rest));
                g_pEventLogManager->AddItem(ELIT_USER, ELID_PrivMsg,tmpnick, NULL, this, NULL);
              }
            }

            // Create private query window here if needed
            if (!pQuery && BOOLPREF(PREF_bCreateQueryWindowsAutomatically))
            {
              // get the hostmask
              char *a = strtok(from, "!");
              char *b = UseEmptyString(strtok(NULL, ""));

              pQuery = CreateQuery(tmpnick,b);
            }

            if (pQuery)
            {
              // Send the message to the query window
              // TODO: BIC_CHANNELTEXT | BIC_QUERYTEXT | BIC_SomethingElse ??
              pQuery->OutputFormatter(BIC_CHANNELTEXT,RPLS_PRIVMSG_CHANNELTEXT); // note params!
            }

            // if we've NOT got a query window
            // OR we've got a query and PREF_bRestrictQueriesToQueryWindows is FALSE
            // then echo to the server window
            if ((!pQuery) || (pQuery && !BOOLPREF(PREF_bRestrictQueriesToQueryWindows)))
            {
              // Note: using BIC_QUERYTEXT not BIC_ACTION for privmsg query actions
              // dump query to server window.
              OutputFormatter(BIC_QUERYTEXT,RPLS_PRIVMSG_RECEIVED);
              //Printf(BIC_QUERYTEXT, "*%s* %s", tmpnick, rest);
            }

            // show on status bar
            if (BOOLPREF(PREF_bShowPrivateMessagesOnStatusBar))
            {
              char *message = HydraIRC_BuildString(512,"%s: %s",tmpnick, rest);
              
              if (message)
              {
                g_pMainWnd->SetStatusBar(STATUS_ICON_NOTE,message);
                free(message);
              }
            }

            // add to message queue
            g_pPrivMsgQueueManager->AddItem(tmpnick,rest,this);

            if (BOOLPREF(PREF_bForwardMessages))
            {
              if ((!pQuery) || (pQuery && !BOOLPREF(PREF_bRestrictQueriesToQueryWindows)))
              {
                // if NO Query Window
                // OR QueryWindow found and PREF_bRestrictQueriesToQueryWindows is DISABLED
                // THEN forward to active window

                HWND hWnd = g_pMainWnd->MDIGetActive();

                // don't send it to the query window twice!
                if (!(pQuery && pQuery->m_pChildWnd && pQuery->m_pChildWnd->m_hWnd == hWnd))
                {
                  // Broadcast the message to the active window, unless this is the active window.
                  if (m_pChildWnd && g_pMainWnd->MDIGetActive() != m_pChildWnd->m_hWnd)
                  {
                    IRCServer *pActiveServer;
                    pActiveServer = FindIRCServer(g_pMainWnd->MDIGetActive());

                    if (pActiveServer != NULL)
                    {
                      if (pActiveServer != this)
                      {
                        // active window is on a different network to the server receiving
                        // the notice.

                        // Format: <servername>: *<from>* <message>
                        HydraIRC_ActiveWindowPrintf(BIC_QUERYTEXT,"%s: %s", m_pDetails->m_Name ,m_Variables[VID_SERVEROUTPUT]);
                      }
                      else
                      {
                        // active window is on this server
                        // Format: *<from>* <message>
                        HydraIRC_ActiveWindowPrintf(BIC_QUERYTEXT,"%s", m_Variables[VID_SERVEROUTPUT]);
                      }
                    }
                  }
                }
              }
            } // End: forward message
          } // End: query
        } // End: !Ignore
      } // End: !CTCP
    } // End: PRIVMSG
    else
    {
      sys_Printf(BIC_INFO,"DEBUG: \"%s\" \"%s\" \"%s\"\n", comm, to, rest);
      Printf(BIC_SERVERMSG, "DEBUG: \"%s\" \"%s\" \"%s\"", comm, to, rest);
    }
  } // End: finished processing string commands.


  if (*ForwardBuffer)
  {
    ForwardMessage(ForwardContents, ForwardBuffer);
  }

  free(token1);
  free(token2);
  free(token3);
  free(args);

}

// forward a messge to the active window, prefs are used to deterimine whether to
// forward the messages based on the Contents value.
void IRCServer::ForwardMessage(const int Contents, const char *Message)
{
  if (!*Message)
    return;

  // check to see if we don't forward this message
  if (Contents == BIC_SERVERMSG && !BOOLPREF(PREF_bForwardServerNotices))
    return;

  // and also display to the active window, if this server isn't
  // the active window. (to avoid printing it twice to the same window)
  if (m_pChildWnd && m_pChildWnd->m_hWnd != g_pMainWnd->MDIGetActive())
  {
    char percentescapedbuf[512*2 + 1];
    EscapePercentCharacters(percentescapedbuf,Message,sizeof(percentescapedbuf));


    IRCServer *pActiveServer;
    pActiveServer = FindIRCServer(g_pMainWnd->MDIGetActive());

    if (pActiveServer == this)
    {
      HydraIRC_ActiveWindowPrintf(Contents,percentescapedbuf);
    }
    else
    {
      BOOL Forward = TRUE;

      // check the contents and related prefs
      if ((Contents == BIC_NOTICE || Contents == BIC_SERVERMSG) && !BOOLPREF(PREF_bCrossNetworkNoticeForwarding))
        Forward = FALSE;

      if (Forward)
      {
        // Format: <servername>: -<from>- <message>
        HydraIRC_ActiveWindowPrintf(Contents,"%s: %s", m_pDetails->m_Name,percentescapedbuf);
      }
    }
  }
}

/*
void IRCServer::ProcessInputBuffer( void )
{  
  // Strip trailing \n's
  int i = m_BufPos-1;
  while (i >= 0 && m_Buf[i] == '\n')
    m_Buf[i--] = 0;

  sys_Printf(BIC_SOCKET,"Server %s processing: \"%s\"\n",m_pDetails->m_Name,m_Buf); // using BIC_SOCKET for easier filtering.

  // anything left in the buffer ?
  if (*m_Buf)
  {
    m_ProcessQueue.Add(strdup(m_Buf));
    m_pChildWnd->PostMessage(WM_PROCESSQUEUEWAITING,0,0);
  }
  m_BufPos = 0;
  *m_Buf = 0;
}
*/
void IRCServer::OnProcessQueue( void )
{
  char *input;
    
  if (m_ProcessQueue.GetSize() > 0)
  {
    input = m_ProcessQueue[0];
    m_ProcessQueue.RemoveAt(0);

    m_CurrentResponse = input;
    Parse(input);
    m_CurrentResponse = NULL;
    free(input);
  }
}

void IRCServer::OnServerSocket(int Event, int Error)
{  
  if (Error != 0)
    sys_Printf(BIC_WARNING,"Server Socket Error: (%d : %s)\n", Error, m_pServerSocket->GetWSASelectErrorString(Error));

  switch(Event)
  {
    case FD_ACCEPT:
      m_pSocket = new Socket(m_pChildWnd->m_hWnd,WM_NETEVENT);
      if (m_pSocket)
      {
        if (m_pSocket->Accept(m_pServerSocket,FD_CLOSE | FD_READ | FD_CONNECT | FD_WRITE))
        {
          m_pServerSocket->CloseSocket(); // we don't want any more connections
          //Printf(BIC_CONNECTING,"Socket connection accepted");
          OnConnect();
        }
        else
        {
          Printf(BIC_ERROR,"Couldn't accept socket");
          return;
        }
      }
      else
      {
        Printf(BIC_ERROR,"Couldn't create socket");
        return;
      }
      break;
    case FD_CLOSE:
#ifdef DEBUG
      sys_Printf(BIC_INFO,"Server socket close notification, m_Status = %d\n",m_Status);
#endif
      if (m_pServerSocket->GetSocket() != INVALID_SOCKET)
        m_pServerSocket->CloseSocket();
      break;

#ifdef DEBUG
    default:
      sys_Printf(BIC_INFO,"unprocessed: OnServerSocketEvent(%d,%d), m_Status = %d\n",Event, Error, m_Status);
#endif

  }
}

void IRCServer::OnNetEvent(int Event, int Error)
{  
  char *VarStr;
  if (Error != 0)
  {
    InitVariables();
    VarStr = HydraIRC_BuildString(512,g_DefaultStrings[DEFSTR_Socket_Error],Error,m_pSocket->GetWSASelectErrorString(Error));
    m_Variables[VID_ALL] = UseEmptyString(VarStr);
    OutputFormatter(BIC_ERROR,APP_ERROR);
    if (VarStr) free(VarStr);
    //Printf(BIC_ERROR,g_DefaultStrings[DEFSTR_Socket_Error], Error, m_pSocket->GetWSASelectErrorString(Error));
  }
  switch(Event)
  {
    case FD_CONNECT:
      if (m_pSocket->GetSocket() == INVALID_SOCKET || Error != 0)
      {
        InitVariables();
        m_Variables[VID_ALL] = g_DefaultStrings[DEFSTR_Server_ConnectError];
        OutputFormatter(BIC_ERROR,APP_ERROR);
        //Printf(BIC_ERROR,g_DefaultStrings[DEFSTR_Server_ConnectError]);
        Disconnect();
      }
      else
      {
        OnConnect();
      }
      break;
    case FD_CLOSE:
      if (m_Status == SVR_STATE_CONNECTED) {
        InitVariables();
        m_Variables[VID_ALL] = g_DefaultStrings[DEFSTR_Socket_Disconnected];
        OutputFormatter(BIC_ERROR,APP_ERROR);
        //Printf(BIC_ERROR,g_DefaultStrings[DEFSTR_Socket_Disconnected]);
        Disconnect();
      }
      break;
    case FD_READ:
      {
        int BytesRead;

        *m_SocketBufPtr = 0;

        if (BOOLPREF(PREF_bSocketLogging) && m_SocketBufPtr != m_SocketBuf)
        {
          sys_Printf(BIC_WARNING,"Recieving data while still data in input buffer\n");
        }

        // fill the socket buffer with as much data as possible.
        // leaving space for a null terminator.
        do
        {
#ifdef DEBUG
          int AmountToReceive = sizeof(m_SocketBuf) - (m_SocketBufPtr - m_SocketBuf) -1;
#endif
          BytesRead = m_pSocket->Receive((LPSTR)m_SocketBufPtr, sizeof(m_SocketBuf) - (m_SocketBufPtr - m_SocketBuf) -1, 0);
          if (BytesRead == SOCKET_ERROR)
          {
#ifdef DEBUG
            //sys_Printf(BIC_SOCKET,"Socket: no more data\n");
#endif
            break;
          }
          if (BytesRead > 0)
          {
            m_SocketBufPtr += BytesRead;
            *m_SocketBufPtr = 0;
            if (BOOLPREF(PREF_bSocketLogging))
            {
              sys_Printf(BIC_SOCKET,"Socket Received %d bytes: \"%s\"\n",BytesRead,m_SocketBufPtr-BytesRead);
            }
          }
        } while ( ( (m_SocketBufPtr - m_SocketBuf) < (sizeof(m_SocketBuf) - 1) ) && 
                  ( BytesRead > 0 ) );

        // check for overrun buffer: cbLen must be < sizeof(m_SocketBuf)
        ATLASSERT((m_SocketBufPtr - m_SocketBuf) < sizeof(m_SocketBuf));

        // check for null terminator
        ATLASSERT(*m_SocketBufPtr == 0);


        // Process the socket buffer until we get a NULL terminator or we reach
        // the end of the socket buffer or the end of input buffer

        char *pLineStart = m_SocketBuf;

        do
        {
          char *pLinePtr = pLineStart;
          BOOL GotLine = FALSE;

          while ( *pLinePtr != 0 )
          {
            // if we've got a \n we've got a line of text.
            if (*pLinePtr == '\n')
            {
              GotLine = TRUE;
              *pLinePtr = 0; // null terminate the buffer
              break;
            }
            pLinePtr++;
          };

          if (GotLine)
          {
            // trim the string *WITHOUT* parsing the whole string! (for speed)
            // SKIP leading \r's and \n's
            while (*pLineStart && (*pLineStart == '\r' || *pLineStart == '\n'))
              pLineStart++;

            char *pEnd = pLinePtr-1;

            // STRIP trailing \r's and \n's
            while (*pEnd && (*pEnd == '\r' || *pEnd == '\n'))
            {
              *pEnd-- = 0;
            }

            if (*pLineStart)
            {
              m_ProcessQueue.Add(strdup(pLineStart));
              m_pChildWnd->PostMessage(WM_PROCESSQUEUEWAITING,0,0);
            }
            else
            {
              sys_Printf(BIC_WARNING,"Got a blank line from the server!\n");
            }

            // set the line pointer to point at the next line.
            pLinePtr++;

          }
          else
          {
            // ok, we've got some data in the socket buffer, but no \n!

/*

when buffer is NOT full things look like this:
[----- socket buffer ---------------------------------]
 1234567890\n1234567890\n1234567890123456789\n\0......
 ^                     ^                      ^
 m_SocketBuf           pLineStart             m_LinePtr & m_SocketBufPtr 

or, they can look like this:
when buffer is NOT full things look like this:
[----- socket buffer ---------------------------------]
 1234567890\n1234567890\n123456789012345\0............   <-- no \n
 ^                       ^              ^
 m_SocketBuf             pLineStart     m_LinePtr & m_SocketBufPtr 


when buffer is completely full, things look like this:
[----- socket buffer ---------------------------------]
 12345678901234567890\n123456789012345678901234567890\0
 ^                     ^                             ^
 m_SocketBuf           pLineStart                   m_LinePtr & m_SocketBufPtr 

if we reach a null terminator without getting a \n
then we copy the data from the end of the buffer
starting from the byte after the last \n the to the 
beginning of the buffer (can be overlapped)

like this: (full buffer)
[----- socket buffer ---------------------------------]
 12345678901234567890\n123456789012345678901234567890\0
                      [------ chars to copy ----------]
[------ destination of copy-----]

or this: (not full)
[----- socket buffer ---------------------------------]
 1234567890\n\12345678901234567890\0..................
              [-- chars to copy --]
[-- dest. of copy --]
*/
            
            if (pLineStart == m_SocketBuf)
            {
              // start of line == start of socket buffer

              if (pLinePtr == m_SocketBuf + (sizeof(m_SocketBuf) -1))
              {
                // If the buffer is full, process as is...

                // we have a full buffer with no \n's!! (wtf!)
                // null terminate it (should already be null terminated...)
                *pLinePtr = 0;

                // too much data came in (i.e. 8191 bytes on a single line)
                sys_Printf(BIC_ERROR,"Warning, server is sending too much data (%d bytes) attempting to process it as is..", sizeof(m_SocketBuf)-1);

                if (*pLineStart)
                {
                  m_ProcessQueue.Add(strdup(pLineStart));
                  m_pChildWnd->PostMessage(WM_PROCESSQUEUEWAITING,0,0);
                }
                else
                {
                  sys_Printf(BIC_ERROR,"please enable socket and console logging and contact the author!\n");
                }

                // point to the start of the buffer again.
                m_SocketBufPtr = m_SocketBuf;
                // exit the loop and wait for more data!
                break;

              }
              else
              {
                // buffer not full, but start of line is start of buffer
                // so just exit the loop and wait for more data!
                break;
              }
            }
            else
            {
              // the start of the line is not at the start of the buffer


              if (pLinePtr == pLineStart)
              {
                // the last line of the socket data was a \n and has been processed
                // pLinePtr is just pointing to the null terminator..
                // this means we processed the entire socket buffer and it ended on a \n
                // so just reset the pointer to the start of the buffer again.
                m_SocketBufPtr = m_SocketBuf;
              }
              else
              {
                // copy from the start of the line to the beginning of the buffer
                // and wait for more data

                char *pSource = pLineStart;
                char *pDest = m_SocketBuf;

                int charstomove = pLinePtr - pLineStart;

                if (BOOLPREF(PREF_bSocketLogging))
                  sys_Printf(BIC_SOCKET,"Moving %d bytes to start of buffer\n",charstomove);

                // reset the socket buffer pointer to point to the new end.
                m_SocketBufPtr = m_SocketBuf + charstomove;

                // copy the end of the buffer to the start.
                while (charstomove)
                {
                  *pDest++ = *pSource++;
                  charstomove --;
                }
                *pDest = 0; // null terminate the socket buffer

              }

              // exit the loop and wait for more data!
              break; 
            }

          }
          pLineStart = pLinePtr;
        } while (TRUE);
      }
      break;
  }
}

void IRCServer::OnLoggedIn( void )
{
  m_LoggedInCount++;
  UpdateInfo();

  if (m_IsDirect) // ignore the rest of this for DCCChats
    return;

  m_pLastNE = g_pNotificationManager->NotificationEvent(NE_LOGGEDIN,m_pDetails->m_NetworkID, NULL);
  if (m_pLastNE && m_pLastNE->m_Flags & NE_FLAG_LOGINEVENTLOG)
  {
    g_pEventLogManager->BuildMessage(g_DefaultStrings[DEFSTR_Server_LoggedIn], m_pDetails->m_Description, m_pDetails->m_Name);
    g_pEventLogManager->AddItem(ELIT_SERVER, ELID_LoggedIn,NULL, NULL, this, NULL);
  }

  // start the keepalive reconnect 
  if (BOOLPREF(PREF_bSendKeepAliveMessages))
  {
    g_EventManager.QueueTimedEvent((CListener *)this,FALSE,EV_SV_SENDKEEPALIVE,INTPREF(PREF_nKeepAliveDelay),TRUE,TRUE,NULL);
  }

  Send("USERHOST :%s\r\n",m_Nick);  

  // TODO: notify plugins of event here..

  // TODO: make this a prefs item

  // TODO: this bit is temporary, eventually you'll be able to assign command profiles
  // to networks and servers.  

  BOOL RanCommandProfile = FALSE;
  BOOL ShiftPressed = FALSE;

  if(GetAsyncKeyState(VK_SHIFT) & 0x8000)
  {
    ShiftPressed = TRUE;
    Printf(BIC_ERROR,"SHIFT key pressed - Not running 'OnLoggedIn' command profile");
  }
  else
  {
    // exec the <network>_OnLoggedIn script
    char *CommandProfileName = HydraIRC_BuildString(512,"%s_OnLoggedIn",GetNetworkName(m_pDetails->m_NetworkID));
    char *Command = HydraIRC_BuildString(512,"/exec %s",UseEmptyString(CommandProfileName));

    if (m_pChildWnd && Command && CommandProfileName && g_pPrefs->FindCommandProfile(CommandProfileName))
    {
      RanCommandProfile = TRUE;
      CHILDFRAMEPTR(m_pChildWnd)->ProcessTextInput(Command,TRUE);
    }

    if (CommandProfileName) free (CommandProfileName);
    if (Command) free (Command);

    // Medlir: If network and server name are the same, don't run the command profile twice.
    if (stricmp(GetNetworkName(m_pDetails->m_NetworkID),m_pDetails->m_Name) != 0)
    {
    // exec the <server>_OnLoggedIn script
    CommandProfileName = HydraIRC_BuildString(512,"%s_OnLoggedIn",m_pDetails->m_Name);
    Command = HydraIRC_BuildString(512,"/exec %s",UseEmptyString(CommandProfileName));

    if (m_pChildWnd && Command && CommandProfileName && g_pPrefs->FindCommandProfile(CommandProfileName))
    {
      RanCommandProfile = TRUE;
      CHILDFRAMEPTR(m_pChildWnd)->ProcessTextInput(Command,TRUE);
    }

    if (CommandProfileName) free (CommandProfileName);
    if (Command) free (Command);
  }
    }
  // End of temporary bit

  // make sure we run the OnLoggedIn command profile before we try and rejoin any channels.

  // rejoin all channels
  int i;
  for (i = 0 ; i < m_ChannelList.GetSize() ; i ++)
  {
    m_ChannelList[i]->ReJoin();
  }

  // post connected messages to this server's query windows
  for (i = 0 ; i < m_QueryList.GetSize() ; i ++)
  {
    m_QueryList[i]->OnConnect();
  }

  while (m_ExecOnLoginList.GetSize() > 0)
  {
    RanCommandProfile = TRUE;
    CCommandProfile *pProfile = m_ExecOnLoginList[0];
    ExecCommandProfile(pProfile, FALSE);
    m_ExecOnLoginList.RemoveAt(0);
  }

  // parse the uri params
  if (ParseURIParams(m_pDetails->m_URIParams))
    RanCommandProfile = TRUE;
  // free the uri params so they're not parsed twice.
  SetString(m_pDetails->m_URIParams, NULL);

  // only show channel list dialog when not running command profiles
  // and only on the first connction attempt
  // and only if not parsing URI commands
  // and only if not processing an ExecOnLoginList of commands (e.g. channel/query via favorites)
  // and only if the pref is enabled.  
  if ((!RanCommandProfile) && (m_LoggedInCount == 1) &&
    BOOLPREF(PREF_bShowChannelListDialogOnConnect))
  {
    ShowChannelListDialog();
  }

}

void IRCServer::SetNick( char *Nick)
{
  if (!Nick || !*Nick)
    return;

#ifdef DEBUG
  sys_Printf(BIC_INFO,"%s: Changing nick from %s to %s\n",m_pDetails->m_Name, m_Nick, Nick);
#endif

  BOOL ShowMessage = (stricmp(m_Nick,Nick));
  SetString(m_Nick, Nick);

  UpdateStatus();

  if (ShowMessage) // only if nick changed
  {
    m_Variables[VID_NICK] = Nick;
    OutputFormatter(BIC_NICK,RPLS_REQUESTNICK);
  }

  // TODO: notify plugins of event here..
}

//+ <gliptic>
char IRCServer::GetNickPrefix(__int64 Mode)
{
  long i;
  
  for(i = 0; m_UserModes[i] != '\0'; i++)
  {
    if(Mode & IRCMODE(m_UserModes[i]))
    {
      return m_Prefixes[i];
    }
  }
  
  return ' ';
}

const char* IRCServer::GetPrefixedNick(const char* Nick, __int64 Modes)
{
  static char prefixed_nick[97];
  
  char bNickPrefix = GetNickPrefix(Modes);
  if(bNickPrefix != ' ')
  {
    strcpy(prefixed_nick + 1, Nick);
    prefixed_nick[0] = bNickPrefix;
  }
  else
  {
    strcpy(prefixed_nick, Nick);
  }
  
  return prefixed_nick;
}

// </gliptic>


void IRCServer::OnEvent(int EventID, void *pData)
{
  switch(EventID)
  {
    case EV_TICK:      
      ProcessSendQueue();

      // check for timeout if we've not connected (dcc or server) or if we're connected to a server and we've not logged in
      if (m_Status == SVR_STATE_CONNECTING || (!m_IsDirect && m_Status == SVR_STATE_CONNECTED && !(m_StatusFlags & ISSF_LOGGEDIN)) )
      {
        time_t now;
        time(&now);
        int timeoutseconds;
        if (m_IsDirect)
          timeoutseconds = PREF_nDCCChatConnectTimeout;
        else
          timeoutseconds = PREF_nServerConnectTimeout;

        if (difftime(m_ConnectAttemptTime,now) > timeoutseconds)
        {
          Disconnect(); // don't retry if this is a dcc chat
        }
      }
      break;
    case EV_SV_RECONNECT:
      Reconnect();
      break;
    case EV_SV_SENDKEEPALIVE:
      if (m_Status == SVR_STATE_CONNECTED)
      {
        if (BOOLPREF(PREF_bShowPings))
        {
          InitVariables();
          m_Variables[VID_ALL] = UseEmptyString(m_Nick);
          OutputFormatter(BIC_SERVERMSG,RPLS_PING_SENT);
        }
        Send("PING %s\n",m_Nick);
      }
      break;
    case EV_PREFSCHANGED:
      UpdateLogger();
      break;

  }
}

char *g_FloodCausingCommands[] = {
  "PRIVMSG",
  "NOTICE",
  NULL
};

BOOL IRCServer::IsFloodCausingCommand(const char *Buffer)
{
  char **cmdptr = g_FloodCausingCommands;

  while (*cmdptr != NULL)
  {
    if (strnicmp(Buffer,*cmdptr,strlen(*cmdptr)) == 0)
      return TRUE;
    cmdptr ++;
  }
  return FALSE;
}

void IRCServer::ProcessSendQueue( void )
{
  int SendQueueItems = m_SendQueue.GetSize();

  // send some queue items
  if (SendQueueItems > 0)
  {
    // always send at least 1 item, but never more than the amount of items in the queue
    int ToSend = min(max(1,INTPREF(PREF_nFloodControlItemsPerSecond)),SendQueueItems);

#ifdef DEBUG
    if (g_DebugFlags & DBGFLG_FLOODCONTROL)
      sys_Printf(BIC_INFO,"%s: Send queue has %d items, sending %d items\n",m_pDetails->m_Name, SendQueueItems, ToSend);
#endif

    for (int SendCounter = 0; SendCounter < ToSend; SendCounter++)
    {
      char *buffer = m_SendQueue[0];

      if (buffer)
      {
        m_pSocket->SendBuffer(buffer);
        free (buffer);
      }

      m_SendQueue.RemoveAt(0);
    }
    // note, we never decrement the m_FloodControlCounter, this makes it
    // stay active until this is nothing left in the send queue.
    // otherwise it'd turn on and off between EV_TICK events.

    if (m_SendQueue.GetSize() == 0)
    {
      m_FloodControlCounter = 0; // reset the counter when queue is empty
      sys_Printf(BIC_WARNING,"*** Deactivating flood control for %s\n", m_pDetails->m_Name);
    }
  }
  else
  {
    // reset the counter once per EV_TICK if the threshold was not reached.
    m_FloodControlCounter = 0;
  }
}

// reply to a received CTCP
BOOL IRCServer::CMD_CTCPReply(const char *to, const char *format, ...)
{

  if (BOOLPREF(PREF_bIgnoreCTCP))
    return FALSE;

  // note: this was IsLoggedIn(), but some servers send ctcp version requests before you've logged in!
	if (!to || !format || m_Status != SVR_STATE_CONNECTED)
    return FALSE;

  char CTCPBuf[513]; // IRC Spec says MAX 512 chars per line, +1 for null terminator.

	va_list args;
	
	va_start(args, format);
	_vsnprintf(CTCPBuf, sizeof(CTCPBuf), format, args);
	va_end(args);
  CTCPBuf[sizeof(CTCPBuf) - 1] = 0;

  /* Medlir: Basic Ping Timing */
  if (stricmp(CTCPBuf,"PING") == 0)
  {
    time_t now;
    time(&now);
    sprintf(CTCPBuf, "PING %d", now);
  }

  Send("NOTICE %s :\001%s\001\r\n",to, CTCPBuf);

  InitVariables();
  m_Variables[VID_NICK] = to;
  m_Variables[VID_ALL] = CTCPBuf;
  OutputFormatter(BIC_CTCP,RPLS_NOTICE_CTCP_SENT);

  return TRUE;
}

// request a ctcp response from another user.
BOOL IRCServer::CMD_CTCPRequest(const char *to, const char *format, ...)
{
	if (!to || !format || !IsLoggedIn())
    return FALSE;

  char CTCPBuf[513]; // IRC Spec says MAX 512 chars per line, +1 for null terminator.

	va_list args;
	
	va_start(args, format);
	_vsnprintf(CTCPBuf, sizeof(CTCPBuf), format, args);
	va_end(args);
  CTCPBuf[sizeof(CTCPBuf) - 1] = 0;

  if (stricmp(CTCPBuf,"PING") == 0)
  {
    time_t now;
    time(&now);
    sprintf(CTCPBuf, "PING %d", now);
  }

  Send("PRIVMSG %s :\001%s\001\r\n",to, CTCPBuf);

  InitVariables();
  m_Variables[VID_NICK] = to;
  m_Variables[VID_ALL] = CTCPBuf;
  OutputFormatter(BIC_CTCP,RPLS_PRIVMSG_CTCP_SENT);
  return TRUE;
}

BOOL IRCServer::CMD_Part(const char *Channel, const char *Msg)
{
  if (!Channel || !*Channel || !IsLoggedIn())
    return FALSE;

  Send("PART %s :%s\r\n",Channel, Msg ? Msg : "");
  return TRUE;
}

void IRCServer::CMD_Quit(const char *Msg)
{
  BOOL SendQuit = TRUE;
  if (IsLoggedIn())
  {
    Send("QUIT : %s\r\n",(Msg && *Msg) ? Msg : PickRandomString(g_QuitMessages,sizeof (g_QuitMessages) / sizeof(char *)) );
    SendQuit = FALSE;
  }
  // cancel any connect, and DON'T reconnect, and DON'T send another QUIT if we've already sent one...
  Disconnect(FALSE,SendQuit);
}

BOOL IRCServer::CMD_Raw(const char *Command )
{
  if (!Command || !*Command)
    return FALSE;

  sys_Printf(BIC_ERROR,"RAWCOMMAND: \"%s\"",Command);
  Send("%s\r\n",Command);
  return TRUE;
}

// create and show a query window, and send a message (if not null) to the user.
BOOL IRCServer::CMD_Query(const char *To, const char *Msg)
{
  if (!To || !*To || !IsLoggedIn())
    return FALSE;

  BOOL Result = FALSE;

  IRCQuery *pQuery = FindQuery(To);

  if (!pQuery)
  {
    pQuery = CreateQuery(To, NULL);
  }

  if (Msg && IsLoggedIn())
  {
    Result = CMD_PrivMsg(To, Msg);
  }

  // if we've just created the query, show it.
  if (pQuery)
  {
    pQuery->ShowQueryWindow();
  }
  return Result;
}

BOOL IRCServer::CMD_Notice(const char *To, const char *Msg)
{
  if (!Msg || !*Msg || !IsLoggedIn() || m_IsDirect)
    return FALSE;

  InitVariables();
  m_Variables[VID_ALL] = Msg;
  m_Variables[VID_NICK] = To;
  OutputFormatter(BIC_NOTICE,RPLS_NOTICE_SENT);
  Send("NOTICE %s :%s\r\n", To, Msg);
  return TRUE;
}

BOOL IRCServer::CMD_PrivMsg(const char *To, const char *Msg)
{
  if (!Msg || !*Msg || !IsLoggedIn())
    return FALSE;

  InitVariables();
  m_Variables[VID_ALL] = Msg;

  if (m_IsDirect) // DCCChat, *To should be NULL
  {    
    m_Variables[VID_NICK] = m_Nick;
    m_Variables[VID_OTHERNICK] = m_OtherNick;
    OutputFormatter(BIC_CHANNELTEXT,RPLS_PRIVMSG_CHANNELTEXT);
    Send("%s\r\n", Msg);
  }
  else
  {
    if (!To || !*To)
      return FALSE;

    if (ischannelstartchar(*To))
    {
      IRCChannel *pChannel = FindChannel(To);
      if (pChannel)
      {
//* <gliptic>
        if(BOOLPREF(PREF_bShowNickPrefixes))
        {
          IRCUser *pUser = pChannel->m_Users.FindUser(m_Nick);

          m_Variables[VID_NICK] = pUser ? GetPrefixedNick(m_Nick, pUser->m_UserModes) : m_Nick;
        }
        else
        {
          m_Variables[VID_NICK] = m_Nick;
        }
// </gliptic>
        m_Variables[VID_UNPREFIXEDNICK] = m_Nick;
        m_Variables[VID_CHANNEL] = To;
        pChannel->OutputFormatter(BIC_OWNTEXT,RPLS_PRIVMSG_CHANNELTEXT);
      }
    }
    else
    {
      IRCQuery *pQuery = FindQuery(To);
      if (pQuery)
      {
        m_Variables[VID_NICK] = m_Nick;
        pQuery->OutputFormatter(BIC_OWNTEXT,RPLS_PRIVMSG_CHANNELTEXT);
      }
      // if we've NOT got a query window
      // OR we've got a query and PREF_bRestrictQueriesToQueryWindows is FALSE
      // then echo to the server window
      if ((!pQuery) || (pQuery && !BOOLPREF(PREF_bRestrictQueriesToQueryWindows)))
      {
        // echo queries to server window
        m_Variables[VID_NICK] = To;
        OutputFormatter(BIC_QUERYTEXT,RPLS_PRIVMSG_SENT);
      }
    }
    Send("PRIVMSG %s :%s\r\n", To, Msg);
  }
  return TRUE;
}

BOOL IRCServer::CMD_Nick(const char *Args)
{
  if (!Args || !*Args)
    return FALSE;

  char *temp = strdup(Args);
  if (!temp)
    return FALSE;

  char *nick = strtok(temp," ");
  char *otherargs = strtok(NULL,"");

  if (m_Status == SVR_STATE_CONNECTED)
  {
    Send("NICK %s%s%s\r\n",nick,otherargs ? ":" : "", UseEmptyString(otherargs));
  }
  // we then wait for a NICK command to be sent back to us...

  if (!IsLoggedIn())
  {
    // the server doesn't send a nick command if we've not logged in.
    // so we have to make a note of the nick change here
    SetNick(nick);
  }

  free(temp);
  return TRUE;
}

BOOL IRCServer::CMD_Action(const char *To, const char *Msg)
{
  if (!Msg || !*Msg || !IsLoggedIn())
    return FALSE;

  InitVariables();
  m_Variables[VID_ALL] = Msg;

  if (m_IsDirect) // DCCChat, *To should be NULL
  {    
    m_Variables[VID_NICK] = m_Nick;
    m_Variables[VID_OTHERNICK] = m_OtherNick;
    OutputFormatter(BIC_ACTION,RPLS_PRIVMSG_ACTION);
    Send("\001ACTION %s\001\r\n",Msg);
  }
  else
  {
    if (!To || !*To)
      return FALSE;

    if (ischannelstartchar(*To))
    {
      IRCChannel *pChannel = FindChannel(To);
      if (pChannel)
      {
        m_Variables[VID_NICK] = m_Nick;
        m_Variables[VID_CHANNEL] = To;
        pChannel->OutputFormatter(BIC_ACTION,RPLS_PRIVMSG_ACTION);
      }
    }
    else
    {
      IRCQuery *pQuery = FindQuery(To);
      if (pQuery)
      {
        m_Variables[VID_NICK] = m_Nick;
        pQuery->OutputFormatter(BIC_ACTION,RPLS_PRIVMSG_ACTION);
      }
    }
    Send("PRIVMSG %s :\001ACTION %s\001\r\n", To, Msg);
  }
  return TRUE;
}

BOOL IRCServer::Send( const char *format, ...)
{
	if (!format)
    return FALSE;

	va_list args;
	
	va_start(args, format);
	_vsnprintf(m_SendBuf, sizeof(m_SendBuf), format, args);
	va_end(args);
  m_SendBuf[sizeof(m_SendBuf) - 1] = 0;

  // set these anyway, just in case the buffer was full.
  m_SendBuf[sizeof(m_SendBuf) - 3] = '\r';
  m_SendBuf[sizeof(m_SendBuf) - 2] = '\n';
  m_SendBuf[sizeof(m_SendBuf) - 1] = 0;


  SendBuffer(m_SendBuf);
  return TRUE;
}

void IRCServer::SendBuffer(const char *Buffer)
{

  int BytesSent = 0;
  // don't use Floodcontrol for DCC Windows
  if (m_IsDirect)
  {
    BytesSent = m_pSocket->SendBuffer(Buffer);
  }
  else
  {    
    m_FloodControlCounter++;
#ifdef DEBUG
    if (g_DebugFlags & DBGFLG_FLOODCONTROL)
    {
      sys_Printf(BIC_INFO,"%s: Flood Control - Counter: %d, Threshold: %d\n",m_pDetails->m_Name, m_FloodControlCounter, INTPREF(PREF_nFloodControlThreshold));

      // testing!
      if (m_FloodControlCounter > 8)
      {
        ATLASSERT(0);
      }
    }
#endif

    // only enable flood control for certain commands when SelectiveFloodControl is enabled

    //if (IsFloodCausingCommand(Buffer) || !BOOLPREF(PREF_bSelectiveFloodControl))
    if (!BOOLPREF(PREF_bSelectiveFloodControl) ||
        (BOOLPREF(PREF_bSelectiveFloodControl) && IsFloodCausingCommand(Buffer)))      
    {
      if (m_FloodControlCounter >= INTPREF(PREF_nFloodControlThreshold))
      {
        // Queue it
        if (m_FloodControlCounter == INTPREF(PREF_nFloodControlThreshold))
        {
          sys_Printf(BIC_WARNING,"*** Activating flood control for server \"%s\"\n", m_pDetails->m_Name);
        }
        m_SendQueue.Add(strdup(Buffer));
      }
      else
      {
        // Send it
        BytesSent = m_pSocket->SendBuffer(Buffer);
      }
    }
    else
    {
      // if flood control is already active, then add it to the queue
      if (m_FloodControlCounter >= INTPREF(PREF_nFloodControlThreshold))
      {
        // Queue it
        m_SendQueue.Add(strdup(Buffer));
        // note: the flood control counter is still incremented in this case
      }
      else
      {
        // Send it
        BytesSent = m_pSocket->SendBuffer(Buffer);
        m_FloodControlCounter--; // not a command that causes flooding
      }
    }
  }

  if (BytesSent == SOCKET_ERROR)
  {
    int Error = WSAGetLastError();
    sys_Printf(BIC_ERROR,"Server socket send error: %d : %s - Data: %s\n", Error, m_pSocket->GetWSASelectErrorString(Error),Buffer);
  }
}

void IRCServer::ProcessChannelModeString(IRCChannel *pChannel, char *Modes)
{
  // SetString(m_BannedBy,By);  
  char action = Modes[0]; Modes++;
  char newmode[3];
  char *param;

  newmode[0] = action;
  newmode[2] = 0;

  // get the next token, and parse each character of it.
  for (char *commands = strtok(Modes, " "); commands != NULL && *commands; commands++)
  {           
    if (*commands == '-' || *commands == '+')
    {
      // e.g. -o+v mynick mynick
      action = *commands;
      newmode[0] = action;
      continue;
    }
    newmode[1] = *commands;

    // Only some modes have parameters!

    if(action == '+')
    {
      if (strchr(m_ParamChannelModes, newmode[1]) != NULL
       || strchr(m_UserModes, newmode[1]) != NULL)
      {
        param = strtok(NULL, " ");
      }
      else
      {
        param = NULL;
      }
    }
    else
    {
      if (strchr(m_ParamClearChannelModes, newmode[1]) != NULL
       || strchr(m_UserModes, newmode[1]) != NULL)
    {
      param = strtok(NULL, " ");
    }
    else
    {
      param = NULL;
    }
    }

/*
    // process channel modes
    if (strchr("kil",newmode[1]) != NULL) // heh, "kil"
    {

    }
*/
    // process user modes (treat as a user mode only if there's a respective parameter..
    if ( (strchr(m_UserModes, newmode[1]) != NULL
          ||  strchr(m_ChannelModes[0], newmode[1]) != NULL) &&
         param != NULL)  // Note: safety check, irc.stardock.com doesn't support +h in the same way as other ircd's
    {
      IRCUser *pUser = pChannel->m_Users.FindUser(param);
      if (pUser)
      {
        pChannel->ChangeUserMode(pUser, newmode);
      }
    }
    else
    {
      pChannel->ChangeChannelMode(newmode, param);
    }
  }
}

void IRCServer::ExecCommandProfile( CCommandProfile *pProfile, BOOL WaitForLogin )
{
  if (!pProfile)
    return;

  if (WaitForLogin && !(m_StatusFlags & ISSF_LOGGEDIN))
    m_ExecOnLoginList.Add(pProfile);
  else
  {
    if (m_pChildWnd)
    {
      char *Command = pProfile->GetFirstCommand();
      while (Command)
      {
        CHILDFRAMEPTR(m_pChildWnd)->ProcessTextInput(Command,TRUE);
        Command = pProfile->GetNextCommand();
      };
      delete pProfile;
    }
    else
      sys_Printf(BIC_INFO,"Couldn't exec %s\n",pProfile->m_Name);
  }
}

BOOL IRCServer::CheckIgnore(char *HostMask, char *Target)
{
  int i;
  IgnoreInfo *pIgnoreInfo;
  
  for (i = 0; i < g_pPrefs->m_IgnoreList.GetSize(); i ++)
  {
    pIgnoreInfo = g_pPrefs->m_IgnoreList[i];

    if (pIgnoreInfo->m_Flags & IIF_DISABLED)
      continue; // ignore if this IgnoreInfo item is disabled

    if (!(pIgnoreInfo->m_NetworkID == NETWORK_ALL || 
          pIgnoreInfo->m_NetworkID == m_pDetails->m_NetworkID))
      continue; // network is not the same, or is not network_all

    // same hostmask as in the ignoreinfo ?
    if (matchhostmask(pIgnoreInfo->m_HostMask,HostMask) == 0)
    {
      // yes, do we need to check the channel list?
      if (pIgnoreInfo->m_Flags & IIF_IGNORECHANNELLIST)
      {
        // message to us / or IIF_IGNORECHANNELLIST is set
        return TRUE;
        break;
      }
      else 
      {
        // message to channel
        
        // are we ignoring this user in this channel?
        if (pIgnoreInfo->IgnoringChannel(Target))
        {
          // yes
          return TRUE;
          break;
        }
      }
    }
  }
  return FALSE;
}

char IRCServer::GetPrefixForMode(char Mode)
{
  char *pPrefix = strchr(m_UserModes, Mode);

  if(!pPrefix)
    return ' ';
  else
    return m_Prefixes[pPrefix - m_UserModes];
}

char IRCServer::GetModeForPrefix(char Prefix)
{
  char *pMode = strchr(m_Prefixes, Prefix);

  if(!pMode)
    return ' ';
  else
    return m_UserModes[pMode - m_Prefixes];
}

void IRCServer::ShowChannelListDialog( void )
{
  if (m_pChannelListDlg == NULL)
  {
    m_pChannelListDlg = new CChannelListDlg(this);
    if (!m_pChannelListDlg)
      return;

    int result = m_pChannelListDlg->DoModal();

    if (result == 1)
    {
      // urr..  TODO
    }
    delete m_pChannelListDlg;
    m_pChannelListDlg = NULL;
  }
  // TODO: else show existing instance.
}
