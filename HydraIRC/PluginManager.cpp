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

// PluginManager.cpp : implementation of the CPluginManager class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

// must be in the same order as the API numbers
char *APINames[] = {
  "Basic Plugin",
  "Command Processor"
};

char *g_PluginManagerErrorNames[] = {
  "No Error",
  "Unknown Plugin",
  "Bad Plugin Name",
  "Plugin Busy",
  "Plugin is in-use"
};

enum PluginManagerErrors {
  PM_ERR_NOERROR,
  PM_ERR_UNKNOWNPLUGIN,
  PM_ERR_BADPLUGINNAME,
  PM_ERR_PLUGINBUSY,
  PM_ERR_INUSE
};

/*
 *	Plugin callback functions.
 */

extern CMainFrame *g_pMainWnd;

BOOL HydraIRC_NewServer(char *URI)
{
  IRCServerDetails ServerDetails;
  UserIdentity Identity;
  IRCServer *pNewServer = NULL;

  if (g_pPrefs->m_UserIdentitiesList.GetSize() == 0)
  {
    // TODO: tell user: "There are no configured user identities, please create one using the following dialog".
    g_pMainWnd->DoPrefs(PREFSPAGE_IDENTITIES);
  }
  else
  {
    if (!URI)
    {
      // display sever connection dialog box
	    CConnectDlg dlg = CConnectDlg(&ServerDetails, &Identity); 
	    int dlgresult = dlg.DoModal();
      if (dlgresult == 1)
      {
        // do the connect, once pNewServerDetails and pUser are correctly
        // filled in from the connect dialog.
        pNewServer = new IRCServer(&ServerDetails,&Identity);
      }
    }
    else
    {
      Identity.Copy(g_pPrefs->m_UserIdentitiesList[0]);
      if (ServerDetails.ParseURI(URI))
      {
        // already connected to this server?
        // NOTE: we still connect to the server if we're connected to
        // the server on a different port, or to another server on the same network

        IRCServer *pServer = FindServerByName(ServerDetails.m_Name,ServerDetails.m_Port);
        if (pServer)
        {
          // parse the URI!
          pServer->ParseURIParams(ServerDetails.m_URIParams);
        }
        else
        {
          // create a new server!
          pNewServer = new IRCServer(&ServerDetails,&Identity);
        }
      }
    }
  }

  return (BOOL)(pNewServer != NULL);
}

HWND HydraIRC_GetToolbar( void )
{
  CReBarCtrl g_Rebar;
  g_Rebar.Attach(g_pMainWnd->m_hWndToolBar);
  REBARBANDINFO rbbi = { sizeof(REBARBANDINFO), RBBIM_CHILD };
  if (g_Rebar.GetBandInfo(1,&rbbi) != 0) {
    //sys_Printf(BIC_INFO, "Got Band Info");
    return rbbi.hwndChild;
  }

  return NULL;//g_pMainWnd->m_hWndToolBar;
}

HIMAGELIST HydraIRC_GetToolbarImageList( void )
{
  return g_pMainWnd->m_images_toolbar;
}

int HydraIRC_GetNewToolbarID( void )
{
  return g_pMainWnd->m_ToolbarUserIDs.ObtainID();
}

void HydraIRC_ReleaseToolbarID( const int ID )
{
  g_pMainWnd->m_ToolbarUserIDs.RelinquishID( ID );
}

int HydraIRC_GetNewImageListID( void )
{
  return g_pMainWnd->m_ToolbarImageListIDs.ObtainID();
}

void HydraIRC_ReleaseImageListID( const int ID )
{
  g_pMainWnd->m_ToolbarImageListIDs.RelinquishID( ID );
}

#define BUILDSTRINGBUFFERSIZE 2048
char *HydraIRC_BuildString(const int MaxSize, const char *format, ...)
{
  va_list args;
  char *buf;

  buf = (char *)malloc(BUILDSTRINGBUFFERSIZE);
  if (buf)
  {
    va_start(args, format);
    _vsnprintf(buf, BUILDSTRINGBUFFERSIZE, format, args);
    va_end(args);
    buf[BUILDSTRINGBUFFERSIZE-1] = 0; // null terminate, just in case
  }
  return buf;
}

#define PLUGIN_PRINTF_BUFSIZE 1024

BOOL HydraIRC_GetChannelInfo(int ChannelID, HIRC_ChannelInfo_t *pCI)
{
  if (!pCI)
    return FALSE;

  IRCChannel *pChannel = FindChannelByID(ChannelID);
  if (!pChannel)
    return FALSE;

  memset(pCI,0,sizeof(HIRC_ChannelInfo_t));

  pCI->Mask = 0;
  pCI->Name = pChannel->m_pProperties->m_Name;
  pCI->Topic = pChannel->m_pProperties->m_Topic;

  // Set the mask, depending on the respective values
  if (pCI->Name) pCI->Mask |= HIRC_CI_NAME;
  if (pCI->Topic) pCI->Mask |= HIRC_CI_TOPIC;

  return TRUE;
}

BOOL HydraIRC_GetServerInfo(int ServerID, HIRC_ServerInfo_t *pSI)
{
  if (!pSI)
    return FALSE;

  IRCServer *pServer = FindServerByID(ServerID);
  if (!pServer)
    return FALSE;

  memset(pSI,0,sizeof(HIRC_ServerInfo_t));

  pSI->Mask = 0;
  pSI->Name = pServer->m_pDetails->m_Name;
  pSI->Description = pServer->m_pDetails->m_Description;
  pSI->Nick = pServer->GetNick();

  // Set the mask, depending on the respective values
  if (pSI->Name) pSI->Mask |= HIRC_SI_NAME;
  if (pSI->Description) pSI->Mask |= HIRC_SI_DESCRIPTION;
  if (pSI->Nick) pSI->Mask |= HIRC_SI_NICK;

  return TRUE;
}

BOOL HydraIRC_GetChildWindowInfo(const CChildFrame *pChild, HIRC_WindowInfo_t *pWI )
{
  if (!pChild)
    return FALSE;

  pWI->WindowType = pChild->m_WindowType;

  switch (pWI->WindowType)
  {
    case CWTYPE_CHANNEL:
      pWI->ChannelID = pChild->m_pChannel->m_ChannelID;
      pWI->ServerID = pChild->m_pServer->m_ServerID;
      pWI->WindowID = pWI->ChannelID;
      break;
    case CWTYPE_QUERY:
      pWI->QueryID = pChild->m_pQuery->m_QueryID;
      pWI->ServerID = pChild->m_pServer->m_ServerID;
      pWI->WindowID = pWI->QueryID;
      break;
    case CWTYPE_SERVER:
      pWI->ServerID = pChild->m_pServer->m_ServerID;
      pWI->WindowID = pWI->ServerID;
      break;
    case CWTYPE_DCCCHAT:
      pWI->DCCChatID = pChild->m_pServer->m_ServerID;
      pWI->ServerID = pChild->m_pServer->m_pOtherServer->m_ServerID;
      pWI->WindowID = pWI->DCCChatID;
      break;
  }
  return TRUE;
}

BOOL HydraIRC_GetWindowInfo( const int WindowID, HIRC_WindowInfo_t *pWI )
{
  if (!pWI)
    return FALSE;

  memset(pWI,0,sizeof(HIRC_WindowInfo_t));

  CChildFrame *pChild = CHILDFRAMEPTR(FindChildWindowByID(WindowID));

  return HydraIRC_GetChildWindowInfo(pChild, pWI);

}

BOOL HydraIRC_GetActiveWindowInfo( HIRC_WindowInfo_t *pWI )
{
  if (!pWI)
    return FALSE;

  memset(pWI,0,sizeof(HIRC_WindowInfo_t));

  HWND hActiveWnd = g_pMainWnd->MDIGetActive();
  if (!hActiveWnd)
    return FALSE;

  CChildFrame *pChild = CHILDFRAMEPTR(FindChildWindow(hActiveWnd));

  return HydraIRC_GetChildWindowInfo(pChild, pWI);

  /*
  IRCChannel *pActiveChannel;
  pActiveChannel = FindIRCChannel(hActiveWnd);
  if (pActiveChannel != NULL)
  {
    pWI->ChannelID = pActiveChannel->m_ChannelID;
    pWI->ServerID = pActiveChannel->m_pServer->m_ServerID;
    pWI->WindowType = CWTYPE_CHANNEL;
    return TRUE;
  }
  else
  {
    IRCQuery *pActiveQuery;
    pActiveQuery = FindIRCQuery(hActiveWnd);
    if (pActiveQuery != NULL)
    {
      pWI->QueryID = pActiveQuery->m_QueryID;
      pWI->ServerID = pActiveQuery->m_pServer->m_ServerID;
      pWI->WindowType = CWTYPE_QUERY;
      return TRUE;
    }
    else
    {
      IRCServer *pActiveServer;
      pActiveServer = FindIRCServer(hActiveWnd,FALSE,FALSE);
      if (pActiveServer != NULL)
      {
        if (pActiveServer->m_IsDirect)
        {
          // DCC
          pWI->DCCChatID = pActiveServer->m_ServerID;
          pWI->ServerID = pActiveServer->m_pOtherServer->m_ServerID;
          pWI->WindowType = CWTYPE_DCCCHAT;
        }
        else
        {
          // Normal Server
          pWI->ServerID = pActiveServer->m_ServerID;
          pWI->WindowType = CWTYPE_SERVER;
        }
        return TRUE;
      }
    }
  }
  return FALSE;
  */
}

/*
void HydraIRC_FreeChannelInfo(HIRC_ChannelInfo_t *pCI)
{
  if (!pCI)
    return;

  if (pCI->Mask & HIRC_CI_NAME && pCI->Name) free(pCI->Name);
  if (pCI->Mask & HIRC_CI_TOPIC && pCI->Topic) free(pCI->Topic);

  free(pCI);
}
*/


void HydraIRC_SendRaw(const int ServerID, const char *Buffer)
{
  IRCServer *pServer = FindServerByID(ServerID);
  if (pServer)
  {
    if (pServer->GetStatus() == SVR_STATE_CONNECTED)
    {
      pServer->SendBuffer(Buffer);
    }
  }
}

void HydraIRC_ServerPut(const int ServerID, const int Contents, const char *Buffer)
{
  IRCServer *pServer = FindServerByID(ServerID);
  if (pServer)
  {
    // using IRCCommon::Printf, not IRCCommon::Put,
    pServer->Printf(Contents, Buffer);
  }
}

void HydraIRC_ChannelPut(const int ChannelID, const int Contents, const char *Buffer)
{
  IRCChannel *pChannel = FindChannelByID(ChannelID);
  if (pChannel)
  {
    pChannel->Printf(Contents, Buffer);
  }
}

void HydraIRC_ServerPrintf(const int ServerID, const int Contents, const char *format, ...)
{
  va_list args;

  char *buf;

  buf = (char *)malloc(PLUGIN_PRINTF_BUFSIZE);
  if (buf)
  {
    //ZeroMemory(buf, PLUGIN_PRINTF_BUFSIZE);
    va_start(args, format);
    _vsnprintf(buf, PLUGIN_PRINTF_BUFSIZE, format, args);
    va_end(args);
    buf[PLUGIN_PRINTF_BUFSIZE - 1] = 0;

    HydraIRC_ServerPut(ServerID, Contents, buf);
  }
  free(buf);
}

void HydraIRC_ChannelPrintf(const int ChannelID, const int Contents, const char *format, ...)
{
  va_list args;

  char *buf;

  buf = (char *)malloc(PLUGIN_PRINTF_BUFSIZE);
  if (buf)
  {
    //ZeroMemory(buf, PLUGIN_PRINTF_BUFSIZE);
    va_start(args, format);
    _vsnprintf(buf, PLUGIN_PRINTF_BUFSIZE, format, args);
    va_end(args);
    buf[PLUGIN_PRINTF_BUFSIZE - 1] = 0;

    HydraIRC_ChannelPut(ChannelID, Contents, buf);
  }
  free(buf);
}

void HydraIRC_CMD_PrivMsg(const int WindowID, const char *To, const char *Message)
{
  CChildFrame *pChild = CHILDFRAMEPTR(FindChildWindowByID(WindowID));
  if (pChild)
  {
    // no destination, try and find one...
    if (!To)
    {
      switch (pChild->m_WindowType)
      {
        case CWTYPE_CHANNEL:
          To = pChild->m_pChannel->m_pProperties->m_Name;
          break;
        case CWTYPE_QUERY:
          To = pChild->m_pQuery->m_OtherNick;
          break;
        case CWTYPE_SERVER:
          // if the window is a server window, it needs a destination!
          return;
      }
    }
    pChild->CMD_PrivMsg(To, Message);
  }  
}

void HydraIRC_CMD_Action(const int WindowID, const char *Message)
{
  CChildFrame *pChild = CHILDFRAMEPTR(FindChildWindowByID(WindowID));
  if (pChild)
  {
    pChild->CMD_Action(Message);
  }
}

void HydraIRC_ActiveWindowPrintf(const int Contents, const char *format, ...)
{
  if (!g_pMainWnd)
    return;

  va_list args;

  char *buf;

  buf = (char *)malloc(PLUGIN_PRINTF_BUFSIZE);
  if (buf)
  {
    va_start(args, format);
    _vsnprintf(buf, PLUGIN_PRINTF_BUFSIZE, format, args);
    va_end(args);
    buf[PLUGIN_PRINTF_BUFSIZE - 1] = 0;

    BOOL Used = FALSE;
    // Broadcast the message to the active channel
    IRCChannel *pActiveChannel;
    pActiveChannel = FindIRCChannel(g_pMainWnd->MDIGetActive());
    if (pActiveChannel != NULL)
    {
      Used = TRUE;
      pActiveChannel->AddToOutputBuffer(Contents, buf);
    }
    else
    {
      IRCQuery *pActiveQuery;
      pActiveQuery = FindIRCQuery(g_pMainWnd->MDIGetActive());
      if (pActiveQuery != NULL)
      {
        Used = TRUE;
        pActiveQuery->AddToOutputBuffer(Contents, buf);
      }
      else
      {
        IRCServer *pActiveServer;
        pActiveServer = FindIRCServer(g_pMainWnd->MDIGetActive(),FALSE,FALSE);
        if (pActiveServer != NULL)
        {
          Used = TRUE;
          pActiveServer->AddToOutputBuffer(Contents, buf);
        }
      }
    }
    if (!Used)
      free(buf);
  }
}
/*
 *	Pluginmanager code...
 */

// fill in a plugin's required APID's table.
BOOL CPluginManager::Resolve(APIDescriptor_t *pAPID)
{
  HydraIRCPlugin *pPlugin;
  CNode *pNode;
  APIDescriptor_t *pOtherAPID;

  if (pAPID->m_Type != PLUGIN_REQUIRES)
    return FALSE;

  for (pPlugin = (HydraIRCPlugin *)m_Plugins.GetFirst(); 
        pPlugin->m_Next != NULL ; 
        pPlugin = (HydraIRCPlugin *)pPlugin->m_Next)
  {
    for (pNode = pPlugin->m_APIList.GetFirst(); pNode->m_Next != NULL ; pNode = pNode->m_Next)
    {
      pOtherAPID = (APIDescriptor_t *)pNode->m_Data;

      if (pOtherAPID->m_Type == PLUGIN_PROVIDES &&
          pOtherAPID->m_API == pAPID->m_API)
      {
        if (pPlugin->GetAPI(pAPID))
        {
          pAPID->m_TableInitDone = TRUE;
          return TRUE;
        }
        else
        {
          // we could carry on walking the list...
          return FALSE;
        }
      }
    }
  }
  return FALSE;
}


// relative or absolute paths are fine...
// make sure you're in the right directory before you use a relative path though
void CPluginManager::LoadPlugin( const char *FileName )
{
  HMODULE pDLL;
  HydraIRCPlugin *pPlugin,*pOtherPlugin;
  HYDRAIRCPLUGIN_DLL_INIT Init;

  if (!FileName)
    return;

  pDLL = LoadLibrary(FileName);
  if (!pDLL)
    return;

  Init = (HYDRAIRCPLUGIN_DLL_INIT)GetProcAddress(pDLL, NAME_HYDRAIRCPLUGIN_INIT);
  if (Init)
  {
    pPlugin = Init();
    if (pPlugin)
    {
      // TODO: Check pPlugin->m_Name for invalid characters. (see m_Name definition)
      // for now, just check we HAVE a name
      if (pPlugin->m_Name)
      {
        // is the plugin already loaded?
        pOtherPlugin = FindPluginByName(pPlugin->m_Name);
        if (pOtherPlugin)
        {
          sys_Printf(BIC_INFO,"Skipping \"%s\" (%s) - Already loaded\n",FileName,pPlugin->m_Name);
        }
        else
        {
          if (pPlugin->RequestAPIs())
          {
            m_Plugins.AddTail(pPlugin);
            pPlugin->m_pDLL = pDLL;
            pPlugin->m_FileName = strdup(FileName);
            //CopyMemory(&pPlugin->m_pDLL,pDLL,sizeof (pDLL));

            return;
          }
        }
      }
    }
  }

  FreeLibrary(pDLL);
}

void CPluginManager::ListPlugins( void ) {
  HydraIRCPlugin *pPlugin;
  for (pPlugin = (HydraIRCPlugin *)m_Plugins.GetFirst(); 
       pPlugin->m_Next != NULL ; 
       pPlugin = (HydraIRCPlugin *)pPlugin->m_Next)
  {
    sys_Printf(BIC_INFO,"Plugin: %s (%s, %s) - %s\n",
      pPlugin->m_Name,
      pPlugin->m_Author,
      pPlugin->m_Info,
      pPlugin->m_Active ? "ACTIVE" : "INACTIVE"
    );
  }
}

void CPluginManager::RefreshPlugins( void )
{
#ifdef DEBUG
  OutputDebugString("CPluginManager::RefreshPlugins() Called\n");
#endif

  HydraIRCPlugin *pPlugin;

  WIN32_FIND_DATA FileData;
  HANDLE hSearch = NULL;
  char PluginFile[MAX_PATH];
  char PluginFilter[MAX_PATH];
  char *PluginFolder = "plugins\\"; // note trailing "\"
  BOOL Done = FALSE;
  
  strcpy(PluginFilter,PluginFolder);
  strcat(PluginFilter,"*.dll");
  
  SetCurrentDirectory(g_HomeDir);

  // Scan "Plugins\" directory for .dll files
  // and load each one.
  
  hSearch = FindFirstFile(PluginFilter, &FileData);

  if (hSearch == INVALID_HANDLE_VALUE) 
  {
    Done = TRUE;
  }

  while (!Done)
  {
    // build the filename
    _snprintf(PluginFile,MAX_PATH,"%s%s",PluginFolder,FileData.cFileName);
    LoadPlugin(PluginFile);

    if (!FindNextFile(hSearch, &FileData) != 0) 
    {
      if (GetLastError() == ERROR_NO_MORE_FILES) 
      {
        Done = TRUE;
      }
      else
      {
        Done = TRUE;
        // some other error..
      }
    }
  }

  FindClose(hSearch);


  // start the resolve.

  CNode *pNode;
  APIDescriptor_t *pAPID;

  int ResolveErrors = 0;
  for (pPlugin = (HydraIRCPlugin *)m_Plugins.GetFirst(); 
        pPlugin->m_Next != NULL ; 
        pPlugin = (HydraIRCPlugin *)pPlugin->m_Next)
  {
    if (!pPlugin->m_Resolved)
    {
      pPlugin->m_Resolved = TRUE;

      for (pNode = pPlugin->m_APIList.GetFirst(); pNode->m_Next != NULL ; pNode = pNode->m_Next)
      {
        pAPID = (APIDescriptor_t *)pNode->m_Data;
        if (!pAPID->m_TableInitDone)
        {
          if (pAPID->m_Type == PLUGIN_PROVIDES)
          {
            sys_Printf(BIC_INFO,"Plugin: %s provides API %d (%s)\n",pPlugin->m_Name,pAPID->m_API,pAPID->m_Name);
            if (pPlugin->GetAPI(pAPID))
            {
              pAPID->m_TableInitDone = TRUE;
            }
          }
          else
          {
            if (!Resolve(pAPID))
              ResolveErrors ++;
          }
        }

        if (!pAPID->m_TableInitDone)
          pPlugin->m_Resolved = FALSE; // still some tables waiting to be filled.
      }        
    }
  }

  // report resolve errors.
  for (pPlugin = (HydraIRCPlugin *)m_Plugins.GetFirst(); 
       pPlugin && pPlugin->m_Next != NULL ; 
       pPlugin = (HydraIRCPlugin *)pPlugin->m_Next)
  {
    if (!pPlugin->m_Resolved)
    {
      sys_Printf(BIC_WARNING,"Could not resolve plugin: %s\n",pPlugin->m_Name);
      for (pNode = pPlugin->m_APIList.GetFirst(); pNode->m_Next != NULL ; pNode = pNode->m_Next)
      {
        pAPID = (APIDescriptor_t *)pNode->m_Data;
        if (!pAPID->m_TableInitDone)
        {
          int KnownAPICount = sizeof(APINames) / sizeof (char *); // TODO, write an API name lookup function...
          sys_Printf(BIC_WARNING,"Unresolved API: \"%s (ID: %d)\" - Table Size Requested: %d\n",pAPID->m_API < KnownAPICount ? APINames[pAPID->m_API] : "Unknown API",pAPID->m_API, pAPID->m_Size);
        }
      }

      // remove this plugin from the list of plugins
      // (remembering that we're looping in a list, so update item pointer..)
      HydraIRCPlugin *pTempPtr = (HydraIRCPlugin *)pPlugin->m_Next;
      Unload(pPlugin,TRUE,TRUE);
      pPlugin = pTempPtr;
    }
  }

  for (pPlugin = (HydraIRCPlugin *)m_Plugins.GetFirst(); 
       pPlugin->m_Next != NULL ; 
       pPlugin = (HydraIRCPlugin *)pPlugin->m_Next)
  {
    if (pPlugin->m_Resolved && pPlugin->m_Active == FALSE)
    {
      sys_Printf(BIC_INFO,"Activating Plugin: %s (%s, %s)\n",pPlugin->m_Name,pPlugin->m_Author,pPlugin->m_Info);
      // now init the plugin
      pPlugin->Init();
    }
  }
}

void CPluginManager::Shutdown( void )
{
#ifdef VERBOSE_DEBUG
  sys_Printf(BIC_FUNCTION,"CPluginManager::Shutdown\n");
#endif

  HydraIRCPlugin *pPlugin;

  while (!m_Plugins.IsEmpty())
  {
    pPlugin = (HydraIRCPlugin *)m_Plugins.GetFirst();
    Unload(pPlugin,TRUE,TRUE); // FORCE Unload, even if plugin is in-use
  }
}

// returns TRUE if no plugins are busy
BOOL CPluginManager::RequestShutdown( void )
{
#ifdef DEBUG
  //sys_Printf(BIC_FUNCTION,"CPluginManager::RequestShutdown\n");
#endif

  HydraIRCPlugin *pPlugin;
  BOOL AllowShutdown = TRUE;

  for (pPlugin = (HydraIRCPlugin *)m_Plugins.GetFirst(); 
       pPlugin->m_Next != NULL ; 
       pPlugin = (HydraIRCPlugin *)pPlugin->m_Next)
  {
    if (!RequestUnload(pPlugin,TRUE))  // ignore in-use plugins, we're requesting a shutdown!
      AllowShutdown = FALSE;       
    // And keep reiterating through the rest of the plugins...
  }
  return AllowShutdown;
}


// returns TRUE if any of the plugins PROVIDE API's are in use by other plugins that REQUIRE it.
BOOL CPluginManager::InUse( HydraIRCPlugin *pPlugin)
{
  HydraIRCPlugin *pOtherPlugin;
  CNode *pNode,*pOtherNode;
  APIDescriptor_t *pAPID,*pOtherAPID;

  if (!pPlugin || !pPlugin->m_Active)
    return FALSE;

  for (pNode = pPlugin->m_APIList.GetFirst(); pNode->m_Next != NULL ; pNode = pNode->m_Next)
  {
    pAPID = (APIDescriptor_t*)pNode->m_Data;

    if (pAPID->m_Type != PLUGIN_PROVIDES)
      continue;

    for (pOtherPlugin = (HydraIRCPlugin *)m_Plugins.GetFirst(); 
        pOtherPlugin->m_Next != NULL ; 
        pOtherPlugin = (HydraIRCPlugin *)pOtherPlugin->m_Next)
    {
      if (pOtherPlugin == pPlugin) // skip ourself!
        continue;

      if (!pOtherPlugin->m_Active)
        continue; // this plugin is inactive, skip it

      for (pOtherNode = pOtherPlugin->m_APIList.GetFirst(); pOtherNode->m_Next != NULL ; pOtherNode = pOtherNode->m_Next)
      {
        pOtherAPID = (APIDescriptor_t*)pOtherNode->m_Data;
        if (pOtherAPID->m_Type == PLUGIN_REQUIRES && pOtherAPID->m_API == pAPID->m_API)
        {
          sys_Printf(BIC_INFO,"Plugin %s's API %d (%s) is REQUIRED by Plugin %s\n",pPlugin->m_Name,pAPID->m_API,pAPID->m_Name,pOtherPlugin->m_Name);
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

// unload a plugin, generally you'd call RequestUnload() first.
// returns TRUE if plugin is unloaded.
BOOL CPluginManager::Unload( HydraIRCPlugin *pPlugin,const BOOL Force,const BOOL IgnoreInUse)
{
  if (!pPlugin)
    return FALSE;

  if (pPlugin->m_Active)
  {

    if (!Force)
    {
      if (!RequestUnload(pPlugin,IgnoreInUse)) // unload even if in use
        return FALSE; // plugin is busy, and we're not FORCING an unload
    }

    sys_Printf(BIC_INFO,"De-activating Plugin: %s\n",pPlugin->m_Name);
    pPlugin->OnDeactivate();
    pPlugin->m_Active = FALSE;
  }
  pPlugin->Remove();

  if (pPlugin->m_FileName)
  {
    free(pPlugin->m_FileName);
    pPlugin->m_FileName = FALSE;
  }

  if (pPlugin->m_pDLL) // core plugins don't have a m_pDLL
  {
    char *PluginName = strdup(pPlugin->m_Name); // make a note of the name, after FreeLibrary() pPlugin is invalid!
    if (FreeLibrary(pPlugin->m_pDLL) != 0)
    {
      sys_Printf(BIC_INFO,"Unloading Plugin: %s succeeded\n", UseEmptyString(PluginName));
      // the plugin destructor will remove it from the list.
    }
    else
    {
      sys_Printf(BIC_INFO,"Unloading Plugin: %s failed (Error: %d)\n", UseEmptyString(PluginName), GetLastError());

      // so that HydraIRC doesn't use it again...
      pPlugin->m_pDLL = FALSE;
      pPlugin->m_Resolved = FALSE; // TODO: remove this 
    }  
    if (PluginName) free(PluginName);
  }
  
  return TRUE;
}

// returns TRUE if pPlugin can be unloaded.
BOOL CPluginManager::RequestUnload( HydraIRCPlugin *pPlugin, const BOOL IgnoreInUse )
{
  if (!pPlugin) // invalid plugin handle
    return FALSE;

  if (pPlugin->m_Active)
  {
    if (!IgnoreInUse && InUse(pPlugin))
      return FALSE; // plugin is in use by another plugin

    return pPlugin->OnRequestUnload(); // call the plugin function
  }
  return TRUE; // not active, can be unloaded.
}

HydraIRCPlugin *CPluginManager::FindPluginByName(const char *PluginName)
{
  if (!PluginName)
    return NULL;

  HydraIRCPlugin *pPlugin;

  for (pPlugin = (HydraIRCPlugin *)m_Plugins.GetFirst(); 
       pPlugin->m_Next != NULL ; 
       pPlugin = (HydraIRCPlugin *)pPlugin->m_Next)
  {
    if (stricmp(PluginName,pPlugin->m_Name) == 0)
    {
      return pPlugin;
    }
  }
  return NULL;
}


int CPluginManager::UnloadByName(const char *PluginName, const BOOL Force, const BOOL IgnoreInUse)
{
  if (!PluginName) return PM_ERR_BADPLUGINNAME;

  HydraIRCPlugin *pPlugin;

  // TODO: move this bit to a new function: CPluginManager::CheckName()
  int len,i;
  len = strlen(PluginName);
  for (i = 0 ; i < len ; i++)
  {
    if (!(isalpha(PluginName[i]) || isdigit(PluginName[i]) || PluginName[i] == '_' || PluginName[i] == '-'))
      return PM_ERR_BADPLUGINNAME;
  }

  pPlugin = FindPluginByName(PluginName);
  if (pPlugin)
  {
    if (!IgnoreInUse && InUse(pPlugin))
      return PM_ERR_INUSE;

    if (Unload(pPlugin, Force,IgnoreInUse))
      return PM_ERR_NOERROR;
    else
      return PM_ERR_PLUGINBUSY;
  }
  return PM_ERR_UNKNOWNPLUGIN;
}
// this may have to be reworked if porting to 64bit/other OS or platforms.
// My knowledge of function pointers and typecasting them is limited, so 
// if you see i'm doing something not quite right, please let me know.
// however, this is a cool little function :)
FUNCPTR CPluginManager::GetFunction(const int API, const int TableOffset,HydraIRCPlugin **pPlugin, CNode **pNode )
{
  //HydraIRCPlugin *pPlugin;
  //CNode *pNode;
  //PluginCmdProcessorTable *pTable = NULL;
  APIDescriptor_t *pAPID;

  if (!*pPlugin)
  {
    *pPlugin = (HydraIRCPlugin *)g_pPluginManager->m_Plugins.GetFirst();
  }

  if ((*pPlugin)->m_Next == NULL)
    return NULL;

  do 
  {
    do 
    {
      if (!*pNode)
      {
        *pNode = (*pPlugin)->m_APIList.GetFirst(); 
      }
      pAPID = (APIDescriptor_t *)(*pNode)->m_Data;
      *pNode = (*pNode)->m_Next;

      if (pAPID->m_Type == PLUGIN_PROVIDES && pAPID->m_API == API)
      {

        return (FUNCPTR)static_cast<FUNCPTR>(pAPID->m_pTable)[TableOffset];
      }      

    } while((*pNode)->m_Next != NULL);

    *pNode = NULL;

    *pPlugin = (HydraIRCPlugin *)(*pPlugin)->m_Next;

  } while ((*pPlugin)->m_Next != NULL);

  return NULL;
  
}

CPluginManager::CPluginManager( void )
{
#ifdef VERBOSE_DEBUG
  sys_Printf(BIC_FUNCTION,"CPluginManager::CPluginManager\n");
#endif

  // we provide these interfaces
  m_pCorePlugin = new CBasicPlugin;

  if (m_pCorePlugin->RequestAPIs()) // though this will never fail.. ;)
    m_Plugins.AddTail(m_pCorePlugin);
}

BOOL CBasicPlugin::RequestAPIs( void )
{
  AddAPI(PLUGIN_API_BASIC,"HydraIRC Basic Functions",sizeof(PluginBasicTable));
  return TRUE;
}

CPluginManager::~CPluginManager( void )
{
#ifdef VERBOSE_DEBUG
  sys_Printf(BIC_FUNCTION,"CPluginManager::~CPluginManager\n");
#endif
  Shutdown();
  delete m_pCorePlugin;
}


BOOL CBasicPlugin::GetAPI(APIDescriptor_t *pAPID)
{
  switch (pAPID->m_API)
  {
    case PLUGIN_API_BASIC:
      PluginBasicTable* pTable= static_cast<PluginBasicTable*>(pAPID->m_pTable);

      // fill in the table with functions from the core
      pTable->m_pfnsys_Printf = sys_Printf;
      pTable->m_pfnsys_MAlloc = malloc;
      pTable->m_pfnsys_Free = free;     
      pTable->m_pfnHydraIRC_BuildString = HydraIRC_BuildString;

      pTable->m_pfnHydraIRC_SendRaw = HydraIRC_SendRaw;

      pTable->m_pfnHydraIRC_ServerPut = HydraIRC_ServerPut;
      pTable->m_pfnHydraIRC_ServerPrintf = HydraIRC_ServerPrintf;
      pTable->m_pfnHydraIRC_ChannelPut = HydraIRC_ChannelPut;
      pTable->m_pfnHydraIRC_ChannelPrintf = HydraIRC_ChannelPrintf;

      pTable->m_pfnHydraIRC_CMD_PrivMsg = HydraIRC_CMD_PrivMsg;
      pTable->m_pfnHydraIRC_CMD_Action = HydraIRC_CMD_Action;

      pTable->m_pfnHydraIRC_GetChannelInfo = HydraIRC_GetChannelInfo;
      pTable->m_pfnHydraIRC_GetServerInfo = HydraIRC_GetServerInfo;
      pTable->m_pfnHydraIRC_GetWindowInfo = HydraIRC_GetWindowInfo;
      pTable->m_pfnHydraIRC_GetActiveWindowInfo = HydraIRC_GetActiveWindowInfo;

      pTable->m_pfnHydraIRC_GetToolbar = HydraIRC_GetToolbar;
      pTable->m_pfnHydraIRC_GetToolbarImageList = HydraIRC_GetToolbarImageList;
      pTable->m_pfnHydraIRC_GetNewToolbarID = HydraIRC_GetNewToolbarID;
      pTable->m_pfnHydraIRC_ReleaseToolbarID = HydraIRC_ReleaseToolbarID;
      pTable->m_pfnHydraIRC_GetNewImageListID = HydraIRC_GetNewImageListID;
      pTable->m_pfnHydraIRC_ReleaseImageListID = HydraIRC_ReleaseImageListID;
      pTable->m_pfnHydraIRC_AliasProcessor = AliasProcessor;
      return TRUE;
      break;
    // process more API's here...
  }
  return FALSE;
}

CBasicPlugin::CBasicPlugin( void )
{ 
  m_Name = "HydraIRC_Core";
  m_Author = "Hydra";
  m_Info = __DATE__;
	return; 
}

BOOL CBasicPlugin::OnRequestUnload( void ) 
{ 
  return TRUE; // can always be unloaded, unless other plugins are using it.
}; 
