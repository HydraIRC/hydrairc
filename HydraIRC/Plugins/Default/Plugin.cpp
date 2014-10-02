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
  3.2) Will become the property of HydraIRC's author.

*/

// Plugin.cpp : An example HydraIRC plugin
//

#include "stdafx.h"
#include <time.h>
#include <stdio.h>
#include <CList.h>
#include <HydraIRCCommon.h>
#include <HydraIRCPlugin.h>
#include <API_Basic.h>
#include <API_CommandProcessor.h>

#include "Plugin.h"
#include "sysinfo.h"

// You'll need to remove version.h if you are using this code as a template.
// VERSIONSTRING is just used for /PIMP

#include "..\..\Version.h"

#define MAX_COMMAND_BUFFER 1024

PluginBasicTable __BASICTABLENAME;

CPlugin Plugin;

// Declare plugin-specific variables here, initialise them in OnActivate()

char *g_KickReason;

// NOTE: This function is created for us by the linker.
// commented out here, because we don't need it for anything else
/*
BOOL APIENTRY DllMain( HANDLE hModule, 
DWORD  ul_reason_for_call, 
LPVOID lpReserved
)
{
switch (ul_reason_for_call)
{
case DLL_PROCESS_ATTACH:
case DLL_THREAD_ATTACH:
case DLL_THREAD_DETACH:
case DLL_PROCESS_DETACH:
break;
}

return TRUE;
}
*/

char *g_SlapMessages[] = 
{
  "slaps %s with a large smelly trout",
  "slaps %s upside da head with a hairy goldfish",
  "slaps %s a few times",
  "finds the closest large object and gives %s a slap with it",
  "instructs a pet piranha to munch on %s's shoulder blades"
};



// copied from HydraIRC/Utility.cpp
char *PickRandomString(char **strings,int count)
{
  int index = rand() % count;
  return strings[index];
}


// return TRUE if command has been processed to half further processing
BOOL Plugin_ProcessCommand(int ServerID, int ChannelID, char **Command, char **Args)
{
  BOOL Processed = FALSE;

// Uncomment this section if requried,  Alias.dll implements all of these aliases now.
/*
  char *NewCommand = NULL;
  char *NewArgs = NULL;

  // Implement some aliases

  if (stricmp(*Command,"J") == 0)
  {
    // Replace "/J" with "/JOIN"
    NewCommand = HydraIRC_BuildString(MAX_COMMAND_BUFFER,"JOIN");
  }
  else if (stricmp(*Command,"Q") == 0)
  {
    // Replace "/Q" with "/QUERY"
    NewCommand = HydraIRC_BuildString(MAX_COMMAND_BUFFER,"QUERY");
  }
  else if (stricmp(*Command,"LIST") == 0)
  {
    // Replace "/LIST" with "/RAW", "LIST <args>"
    NewCommand = HydraIRC_BuildString(MAX_COMMAND_BUFFER,"RAW");
    if (*Args && **Args)
      NewArgs = HydraIRC_BuildString(MAX_COMMAND_BUFFER,"LIST %s",*Args);
    else
      NewArgs = HydraIRC_BuildString(MAX_COMMAND_BUFFER,"LIST");
  }
  else if (stricmp(*Command,"EXITNOW") == 0)
  {
    // Replace "/EXITNOW", NULL with "/EXIT", "-NOW"
    NewCommand = HydraIRC_BuildString(MAX_COMMAND_BUFFER,"EXIT");
    NewArgs = HydraIRC_BuildString(MAX_COMMAND_BUFFER,"-NOW");
  }
  else if (stricmp(*Command,"MODE") == 0)
  {
    NewCommand = HydraIRC_BuildString(MAX_COMMAND_BUFFER,"RAW");
    NewArgs = HydraIRC_BuildString(MAX_COMMAND_BUFFER,"MODE %s",*Args ? *Args : "");
  }

  if (NewCommand != NULL)
  {
    sys_Free(*Command); // Free the old command, and then replace it...
    *Command = NewCommand;    
    // Note: NewCommand will eventually be  sys_Free()'d by HydraIRC

    // then do the same for the args
    if (NewArgs != NULL)
    {
      sys_Free(*Args);
      *Args = NewArgs;    
    }
  }
*/
  return Processed;
}

// return TRUE if command has been processed to half further processing
BOOL Plugin_ProcessChannelCommand(int ServerID, int ChannelID, char **Command, char **Args)
{
  BOOL Processed = FALSE;
  char *NewCommand = NULL;
  char *NewArgs = NULL;
  HIRC_ChannelInfo_t CI;
  
  HydraIRC_GetChannelInfo(ChannelID,&CI);

  if (stricmp(*Command,"MODE") == 0)
  {
    if (CI.Mask & HIRC_CI_NAME) // Got channel name ?
    {
      NewCommand = HydraIRC_BuildString(MAX_COMMAND_BUFFER,"RAW");

      if (*Args && (**Args == '+' || **Args == '-'))
        NewArgs = HydraIRC_BuildString(MAX_COMMAND_BUFFER,"MODE %s %s",CI.Name,*Args ? *Args : "");
      else
        NewArgs = HydraIRC_BuildString(MAX_COMMAND_BUFFER,"MODE %s",*Args ? *Args : "");
    }
  }
  else
  if (stricmp(*Command,"TOPIC") == 0)
  {
    if (CI.Mask & HIRC_CI_NAME) // Got channel name ?
    {
      NewCommand = HydraIRC_BuildString(MAX_COMMAND_BUFFER,"RAW");
      if (*Args && **Args)
        NewArgs = HydraIRC_BuildString(MAX_COMMAND_BUFFER,"TOPIC %s :%s",CI.Name,*Args);
      else
        NewArgs = HydraIRC_BuildString(MAX_COMMAND_BUFFER,"TOPIC %s",CI.Name);
    }
  }
  else
  if (stricmp(*Command,"PIMP") == 0)
  {
    NewCommand = HydraIRC_BuildString(MAX_COMMAND_BUFFER,"ME");
#ifdef RELEASE_VERSION
    NewArgs = HydraIRC_BuildString(MAX_COMMAND_BUFFER,"is using HydraIRC " VERSIONSTRING " - Grab it from www.HydraIRC.com");
#else
    NewArgs = HydraIRC_BuildString(MAX_COMMAND_BUFFER,"is using HydraIRC " VERSIONSTRING " - Grab it from #HydraIRC on EFNet");
#endif
  }
  else
  if (stricmp(*Command,"KR") == 0)
  {
    Processed = TRUE;
    if (!*Args || !**Args)
      HydraIRC_ChannelPrintf(ChannelID,BIC_ERROR,"ERROR: <kick reason>");
    else
    {    
      if (g_KickReason) free(g_KickReason);
      g_KickReason = strdup(*Args);
    }
  }
  else
  if (stricmp(*Command,"KICK") == 0)
  {
    Processed = TRUE; // set it to TRUE, in case of errors
    if (CI.Mask & HIRC_CI_NAME) // Got channel name ?
    {
      char *ArgsCpy = NULL;
      char *Nick = NULL;
      char *Message = NULL;

      if (*Args)
      {
        ArgsCpy = strdup(*Args);
        if (ArgsCpy)
        {
          Nick = strtok(ArgsCpy," ");
          Message = strtok(NULL,"");
        }
      }

      if (Nick && *Nick)
      {
        if (!Message)
        {
          if (g_KickReason) 
            Message = g_KickReason;
          else
            Message = "Kick!"; // supply a default kick message
        }

        NewCommand = HydraIRC_BuildString(MAX_COMMAND_BUFFER,"RAW");
        NewArgs = HydraIRC_BuildString(MAX_COMMAND_BUFFER,"KICK %s %s :%s",CI.Name,Nick,Message);
        Processed = FALSE;
      }
      else
        HydraIRC_ChannelPrintf(ChannelID,BIC_ERROR,"ERROR: <nick> [<message>]");

      if (ArgsCpy) free(ArgsCpy);
    }
  }
  else
  if (stricmp(*Command,"SLAP") == 0)
  {
    Processed = TRUE; // set it to TRUE, in case of errors
    if (CI.Mask & HIRC_CI_NAME) // Got channel name ?
    {
      if (*Args && **Args)
      {
        NewCommand = HydraIRC_BuildString(MAX_COMMAND_BUFFER,"ME");
        NewArgs = HydraIRC_BuildString(MAX_COMMAND_BUFFER,PickRandomString(g_SlapMessages,sizeof (g_SlapMessages) / sizeof(char *)),*Args);
        Processed = FALSE;
      }
      else
        HydraIRC_ChannelPrintf(ChannelID,BIC_ERROR,"ERROR: <nick>");
    }
  }
  else
  if (stricmp(*Command,"SYSINFO") == 0)
  {
    NewCommand = HydraIRC_BuildString(MAX_COMMAND_BUFFER,"ME");
    NewArgs = HydraIRC_BuildString(MAX_COMMAND_BUFFER,"is using \x02%s\x02 || \x02%s\x02 || \x02%s\x02", osversion(), cpuinfo(), meminfo());
    Processed = FALSE;
  }
 
  if (NewCommand != NULL)
  {
    sys_Free(*Command); // Free the old command, and then replace it...
    *Command = NewCommand;    
    // Note: NewCommand will eventually be  sys_Free()'d by HydraIRC

    // then do the same for the args
    if (NewArgs != NULL)
    {
      sys_Free(*Args);
      *Args = NewArgs;    
    }
  }

  return Processed;
}

// return TRUE if command has been processed to half further processing
BOOL Plugin_ProcessInput(int ServerID, int ChannelID, char **Command)
{  
  BOOL Processed = FALSE;

// Uncomment this section if required:

/*
  char *NewCommand = NULL;

  char *CmdCopy = strdup(*Command); // we need to work on a copy of the command
  if (!CmdCopy)
    return FALSE; // can't continue without our copy..

  char *ArgsPtr,*CmdPtr;

  CmdPtr = strtok(CmdCopy," ");  // strtok modifies CmdCopy.  which is why we need a copy!
  ArgsPtr = strtok(NULL,"");

  // if the user typed in "/eat ice cream"
  // then CmdPtr would point to "/eat" and ArgsPtr would point to "ice cream"

  if (CmdPtr && *CmdPtr == '/')
  {
    CmdPtr++; // skip over the '/' character.

    // a simple alias: "newcommand=/raw command args"
    if (stricmp(CmdPtr,"newcommand") == 0)
    {
      // change this as you see fit
      NewCommand = HydraIRC_BuildString(MAX_COMMAND_BUFFER,"/RAW %s %s",CmdPtr, ArgsPtr);
    }

    if (NewCommand != NULL)
    {
      sys_Free(*Command); // Free the old command, and then replace it...
      *Command = NewCommand; 
      
      // Note: NewCommand will eventually be  sys_Free()'d by HydraIRC
    }
  }

  if (CmdCopy) free (CmdCopy);

  // if we get here, it means we didn't processed the Command
  // so we return FALSE so that the command is still processed
  // by the core and/or other plugins.

*/

  return Processed;
}

/*
 *	Actual Plugin -> HydraIRC functions start here
 */

PLUGIN_API HydraIRCPlugin *HydraIRCPlugin_Init(void)
{
  return &Plugin;
}

/*
 *	Here's the plugin interface.
 */

CPlugin::CPlugin( void )
{ 
  m_Name = "Default"; // only use alpha-numeric, -'s and _'s and NO SPACES
  m_Author = "Hydra";
  m_Info = __DATE__;
	return; 
}

BOOL CPlugin::RequestAPIs( void )
{
  if (AddAPI(PLUGIN_API_CMDPROC,"Example Command Processor",sizeof(PluginCmdProcTable)) &&
      //AddAPI(99999,NULL,10,PLUGIN_REQUIRES,&__BASICTABLENAME) && // API 99999 doesn't exist, enable to do a core resolve check!
      AddAPI(PLUGIN_API_BASIC,NULL,sizeof(PluginBasicTable),PLUGIN_REQUIRES,&__BASICTABLENAME))
	  return TRUE;
  else
    return FALSE;
}

BOOL CPlugin::GetAPI(APIDescriptor_t *pAPID)
{
  switch (pAPID->m_API)
  {
    case PLUGIN_API_CMDPROC:
      PluginCmdProcTable* pTable= static_cast<PluginCmdProcTable*>(pAPID->m_pTable);

      // fill in the table!
      pTable->m_pfnProcessInput = Plugin_ProcessInput;
      pTable->m_pfnProcessCommand = Plugin_ProcessCommand;
      pTable->m_pfnProcessChannelCommand = Plugin_ProcessChannelCommand;

      return TRUE;
    break;
  }
  return FALSE;
}

void CPlugin::OnActivate( void )
{
  OutputDebugString("CPlugin::OnActivate() called\n");

  // ************************* WARNING ******************************
  // Hmm, I want shared memory, but Windows is being st00pid.
  // for now, all memory allocated by the plugin must be freed by it
  // and all memory allocated by the core must be freed by the core.
  //                   !!!! NO EXCEPTIONS !!!!
/*
#ifdef DEBUG
  // Testing, makes sure the dll can free memory allocated via the core
  free(m_AllocTest1);
  // Testing, makes sure the core can free memory allocated via the dll
  m_AllocTest2 = malloc(100);
#endif
*/
  sys_Printf(BIC_INFO,"%s Plugin by %s Activated\n", m_Name, m_Author);

  // do activation code here

  g_KickReason = NULL;
  srand( (unsigned)time( NULL ) );

}

void CPlugin::OnDeactivate( void )
{
  OutputDebugString("CPlugin::OnDeactivate() called\n");

  sys_Printf(BIC_INFO,"%s Plugin by %s Deactivated\n", m_Name, m_Author);

  // do de-activation code here
  if (g_KickReason) free(g_KickReason);
}


BOOL CPlugin::OnRequestUnload( void )
{
#ifdef DEBUG
  return TRUE;
  // example of how you can ask the user if your plugin is busy doing stuff
  //return (IDYES == MessageBox(NULL,"Allow Plugin.DLL Unload?", NULL, MB_YESNO | MB_ICONQUESTION));
#else
  return TRUE;
#endif
}
