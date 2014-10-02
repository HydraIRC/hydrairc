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

// Plugin.cpp : An example HydraIRC plugin
//

#include "stdafx.h"
#include <stdio.h>
#include <CList.h>
#include <HydraIRCCommon.h>
#include <HydraIRCPlugin.h>
#include <API_Basic.h>
#include <API_CommandProcessor.h>

#include "Plugin.h"

// You'll need to remove version.h if you are using this code as a template.
// VERSIONSTRING is just used for /PIMP

#include "..\..\Version.h"

#define MAX_COMMAND_BUFFER 1024

PluginBasicTable __BASICTABLENAME;

CPlugin Plugin;

// Declare plugin-specific variables here, initialise them in OnActivate()

// TODO: Add your variables here

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

// TODO: Add your function and other plugin code here

/*
typedef struct AliasPair_s {
  char *m_Command;
  char *m_Alias;
} AliasPair_t;

AliasPair_t AliasList[] = 
{
  {"op","mode $# +ooo $1 $2 $3"},
  {"dop","mode $# -ooo $1 $2 $3"},
  {NULL,NULL} // NULL denotes end of list
};

char *GetAliasCommand(char *Command)
{
  AliasPair_t *pAlias = AliasList;
  while (pAlias->m_Command)
  {
    if (stricmp(pAlias->m_Command,Command) == 0)
    {
      return (strdup(pAlias->m_Alias));
    }
    pAlias++;
  }
  return NULL;
}

*/

char *AliasList[] =
{
  "op=mode $# +ooo $1 $2 $3",
  "dop=mode $# -ooo $1 $2 $3",
  "voice=mode $# +vvv $1 $2 $3",
  "hop=mode $# +hhh $1 $2 $3",
  "devoice=mode $# -vvv $1 $2 $3",
  "dehop=mode $# -hhh $1 $2 $3",
  "deadmin=mode $# -aaa $1 $2 $3",
  "admin=mode $# +aaa $1 $2 $3",
  "j=join $1-",
  "qj=quietjoin $1-",
  "q=query $1-",
  "disconnect=quit $1-",
  "leave=part $1-",
  "quote=raw $1-",
  "nserver=newserver $1-",
  "xget=ctcp $1 xdcc send $2-",
  "xlist=ctcp $1 xdcc list $2-",
  "ns=msg NICKSERV $1-",
  "ms=msg MEMOSERV $1-",
  "cs=msg CHANSERV $1-",
  "dalns=msg nickserv@services.dal.net $1-",
  "auth=msg NICKSERV identify $1-",
  "dalauth=msg nickserv@services.dal.net identify $1-",
  "qauth=msg q@cserve.quakenet.org auth $1-",
  //"notice=NOTICE $1 :$2-",        
  "away=AWAY :$1-",
  "operwall=OPERWALL :$1-",
  "wallop=WALLOP :$1-",
  "umode=mode $! $1-",
  "action=me $1-",
  "onotice=NOTICE @$# $1-",
  "omsg=PRIVMSG @$# :$1-",
  "kill=KILL $1 :$2-",
  "zline=ZLINE $1 :$2-",
  "kline=KLINE $1 :$2-",
  "gline=GLINE $1 :$2-",
  "shun=SHUN $1 :$2-",
  "gzline=GZLINE $1 :$2-",
  "tkline=TKLINE $1 :$2-",
  "tzline=TZLINE $1 :$2-",
  NULL
};

char *GetAliasCommand(char *Command)
{
  char **pAlias = AliasList;
  int CommandLen = (int) strlen(Command);
  while (*pAlias)
  {
    char *Pos = strchr(*pAlias,'=');
    if (CommandLen == Pos - *pAlias && strnicmp(*pAlias,Command,CommandLen) == 0)
    {
      return (strdup(Pos+1));
    }
    pAlias++;
  }
  return NULL;
}

// return TRUE if command has been processed to half further processing
BOOL Plugin_ProcessInput(int ServerID, int ChannelID, char **Command)
{  
  BOOL Processed = FALSE;
  char *NewCommand = NULL;

  char *CmdCopy = strdup(*Command); // we need to work on a copy of the command
  if (!CmdCopy)
    return FALSE; // can't continue without our copy..

  char *ArgsPtr,*CmdPtr;

  CmdPtr = strtok(CmdCopy," ");  // strtok modifies CmdCopy.  which is why we need a copy!
  ArgsPtr = strtok(NULL,"");

  if (CmdPtr && *CmdPtr == '/')
  {
    HIRC_ChannelInfo_t CI;
    HIRC_ServerInfo_t SI;

    HydraIRC_GetChannelInfo(ChannelID,&CI);
    HydraIRC_GetServerInfo(ServerID,&SI);

    char *SubsitutionTable[] = {"#","","!","",NULL,NULL};

    if (CI.Mask & HIRC_CI_NAME)
      SubsitutionTable[1] = CI.Name;

    if (SI.Mask & HIRC_SI_NICK)
      SubsitutionTable[3] = SI.Nick;

    CmdPtr++; // skip over the '/' character.

    // Implement an alias processor

    char *Alias;
    char *AliasOutput;

    Alias = GetAliasCommand(CmdPtr);

    if (Alias)
    { 
      AliasOutput = HydraIRC_AliasProcessor(*Command,Alias,4,'$',SubsitutionTable);
      if (AliasOutput)
      {
        NewCommand = HydraIRC_BuildString(MAX_COMMAND_BUFFER,"/%s",AliasOutput);
      }
      sys_Free(AliasOutput);
      free(Alias);
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

  return Processed;
}

BOOL Plugin_ProcessChannelCommand(int ServerID, int ChannelID, char **Command, char **Args)
{
  return FALSE;
}

BOOL Plugin_ProcessCommand(int ServerID, int ChannelID, char **Command, char **Args)
{
  return FALSE;
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
  m_Name = "Alias"; // only use alpha-numeric, -'s and _'s and NO SPACES
  m_Author = "Hydra";
  m_Info = __DATE__;
	return; 
}

BOOL CPlugin::RequestAPIs( void )
{
  if (AddAPI(PLUGIN_API_CMDPROC,"Alias Processor",sizeof(PluginCmdProcTable)) &&
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
#ifdef VERBOSE_DEBUG
  OutputDebugString("CPlugin::OnActivate() called\n");
#endif

  sys_Printf(BIC_INFO,"%s Plugin by %s Activated\n", m_Name, m_Author);

  // TODO: do your plugin activation here (e.g. alloc memory that you need)

}

void CPlugin::OnDeactivate( void )
{
#ifdef VERBOSE_DEBUG
  OutputDebugString("CPlugin::OnDeactivate() called\n");
#endif

  sys_Printf(BIC_INFO,"%s Plugin by %s Deactivated\n", m_Name, m_Author);

  // TODO: do your plugin de-activation here (e.g. free memory you used.)
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
