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

// Plugin.cpp : An example HydraIRC plugin
//

#include "stdafx.h"
#include <stdio.h>
#include <time.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <CList.h>
#include <HydraIRCCommon.h>
#include <HydraIRCPlugin.h>
#include <API_Basic.h>
#include <API_CommandProcessor.h>

#include "Plugin.h"

#define MAX_COMMAND_BUFFER 1024

PluginBasicTable __BASICTABLENAME;

CPlugin Plugin;

// Declare plugin-specific variables here, initialise them in OnActivate()

// int g_MyGlobalVariable;

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

time_t PluginStartTime; 

void Uptime(int ServerID,int ChannelID)
{
  time_t now = time(NULL); 

  DWORD	d, h, m, s;

  BOOL GotUptime = FALSE;
  
  // we running on a winnt kernel ?
	if( GetVersion() < 0x80000000 ) 
  {
    // yes, so use a higher resolution uptime counter.

	  HQUERY		perfQuery;
	  HCOUNTER	uptimeCounter;
	  PDH_FMT_COUNTERVALUE uptimeValue;
    PDH_STATUS status;

  	if( PdhOpenQuery( NULL, 0, &perfQuery ) == ERROR_SUCCESS)
    {
      //if ( PdhAddCounter( perfQuery, "\\\\.\\System\\System Up Time", 0, &uptimeCounter ) == ERROR_SUCCESS )
      if ( PdhAddCounter( perfQuery, "\\System\\System Up Time", 0, &uptimeCounter ) == ERROR_SUCCESS )
      {
        if ((status = PdhCollectQueryData( perfQuery )) == ERROR_SUCCESS)
        {
          if ( PdhGetFormattedCounterValue( uptimeCounter, PDH_FMT_LARGE , NULL, &uptimeValue ) == ERROR_SUCCESS)
          {
	          d = (DWORD) (uptimeValue.largeValue / (3600*24));
	          h = (DWORD) (uptimeValue.largeValue % (3600*24) / 3600);
	          m = (DWORD) (uptimeValue.largeValue % 3600) / 60;
	          s = (DWORD) (uptimeValue.largeValue % 60);            
            GotUptime = TRUE;
          }
        }
      }
      PdhCloseQuery( &perfQuery );
    }

  }
  // use a low resolution timer if we failed to get high resolution uptime
  if (!GotUptime)
  {
    DWORD uptime = GetTickCount();

    uptime = uptime / 1000; // convert from milliseconds to seconds.
	  d = (DWORD) (uptime / (3600*24));
	  h = (DWORD) (uptime % (3600*24) / 3600);
	  m = (DWORD) (uptime % 3600) / 60;
	  s = (DWORD) (uptime % 60);
    GotUptime = TRUE;
  }


  char *timestr=(char *)sys_MAlloc(1024);
  int i;

  i = _snprintf(timestr,1023,"\0037[\0032System-Uptime\0037]\0033 ");
  if (d > 0) i += _snprintf(timestr+i,1023-i,"%i\017d\0033 ", d); 
  if (h > 0)	i += _snprintf(timestr+i,1023-i,"%i\017h\0033 ", h); 
  i += _snprintf(timestr+i,1023-i,"%i\017m\0033 %i\017s", m, s);

  time_t hydraircuptime = now - PluginStartTime;

	d = (DWORD) (hydraircuptime / (3600*24));
	h = (DWORD) (hydraircuptime % (3600*24) / 3600);
	m = (DWORD) (hydraircuptime % 3600) / 60;
	s = (DWORD) (hydraircuptime % 60);

  i += _snprintf(timestr+i,1023," \0037[\0032HydraIRC-Uptime\0037]\0033 ");
  if (d > 0) i += _snprintf(timestr+i,1023-i,"%i\017d\0033 ", d); 
  if (h > 0)	i += _snprintf(timestr+i,1023-i,"%i\017h\0033 ", h); 
  i += _snprintf(timestr+i,1023-i,"%i\017m\0033 %i\017s", m, s);

  // Post it to the debug window
  // sys_Printf(BIC_INFO,"%s\n",timestr);

  // Post it to the server window
  // HydraIRC_ServerPrintf(ServerID,BIC_UNKNOWN,"%s",timestr);

  if (ChannelID) 
  {
    // Echo it
    // HydraIRC_ChannelPrintf(ChannelID,BIC_UNKNOWN,"%s",timestr);

    // Send it to the channel!
    HydraIRC_CMD_PrivMsg(ChannelID,NULL, timestr);
  }
  sys_Free(timestr);
}

// return TRUE if command has been processed to half further processing
BOOL Plugin_ProcessCommand(int ServerID, int ChannelID, char **Command, char **Args)
{
  BOOL Processed = FALSE;

  return Processed;
}

// return TRUE if command has been processed to half further processing
BOOL Plugin_ProcessChannelCommand(int ServerID, int ChannelID, char **Command, char **Args)
{
  BOOL Processed = FALSE;

  if (stricmp(*Command,"UPTIME") == 0)
  {
    Uptime(ServerID,ChannelID);
    Processed = TRUE;
  }

  return Processed;
}

// return TRUE if command has been processed to half further processing
BOOL Plugin_ProcessInput(int ServerID, int ChannelID, char **Command)
{  
  BOOL Processed = FALSE;
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
  m_Name = "Uptime"; // only use alpha-numeric, -'s and _'s and NO SPACES
  m_Author = "Hydra";
  m_Info = __DATE__;
	return; 
}

BOOL CPlugin::RequestAPIs( void )
{
  if (AddAPI(PLUGIN_API_CMDPROC,"Command Processor for /UPTIME",sizeof(PluginCmdProcTable)) &&
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
      PluginCmdProcTable* pTable = static_cast<PluginCmdProcTable*>(pAPID->m_pTable);

      // fill in the table - note, for this API we only need to fill in the
      // functions that we provide.  all functions for the PLUGIN_API_CMDPROC API
      // are optional.
      pTable->m_pfnProcessChannelCommand = Plugin_ProcessChannelCommand;

      return TRUE;
    break;
  }
  return FALSE;
}

void CPlugin::OnActivate( void )
{
  OutputDebugString("CPlugin::OnActivate() called\n");

  sys_Printf(BIC_INFO,"%s Plugin by %s Activated\n", m_Name, m_Author);

  // do activation code here

  PluginStartTime = time(NULL);

}

void CPlugin::OnDeactivate( void )
{
  OutputDebugString("CPlugin::OnDeactivate() called\n");

  sys_Printf(BIC_INFO,"%s Plugin by %s Deactivated\n", m_Name, m_Author);

  // do de-activation code here
}


BOOL CPlugin::OnRequestUnload( void )
{
#ifdef DEBUG
  return TRUE;
  // example of how you can ask the user if your plugin is busy doing stuff
  //return (IDYES == MessageBox(NULL,"Allow Uptime.DLL Unload?", NULL, MB_YESNO | MB_ICONQUESTION));
#else
  return TRUE;
#endif
}
