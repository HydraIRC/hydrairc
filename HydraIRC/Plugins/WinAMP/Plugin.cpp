/*

  HydraIRC
  Copyright (C) 2002-2006 Dominic Clifton aka Hydra

  This program is free software: you can redistribute it and/or modify  
  it under the terms of the GNU General Public License as published by  
  the Free Software Foundation, version 3.
 
  This program is distributed in the hope that it will be useful, but 
  WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
  General Public License for more details.
 
  You should have received a copy of the GNU General Public License 
  along with this program. If not, see <http://www.gnu.org/licenses/>.

*/

// Plugin.cpp : An example HydraIRC plugin
//

#include "stdafx.h"
//#include <stdio.h>
#include "resource.h"

#include <CList.h>
#include <HydraIRCCommon.h>
#include <HydraIRCPlugin.h>
#include <API_Basic.h>
#include <API_GUI.h>
#include <API_CommandProcessor.h>

//Winamp API
//#include "wa_ipc.h"
//Fun stuff, the loading of the file causes the compile to break. Hardcoded what I am using in here for now.
#define WM_WA_IPC WM_USER
#define IPC_ISPLAYING 104
#define IPC_GETOUTPUTTIME 105
#define IPC_GETINFO 126

#include "Plugin.h"

//Includes for String and IOStreams
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

//Put what we are using into our scope
using std::string;
using std::cin;
using std::endl;
using std::ios;
using std::ifstream;

CAppModule _Module;

#define MAX_COMMAND_BUFFER 1024

PluginBasicTable __BASICTABLENAME;

CPlugin Plugin;

// Declare plugin-specific variables here, initialise them in OnActivate()

#define TOOLBAR_BUTTON_COUNT 2
int ToolbarIDs[2] = {0};

//Define our default text for the reply
string strPlayingText = "is listening to %title%";


enum ToolBarItems {
  SEPERATOR_1,
  WINAMP_BUTTON1
};


string int2string(const int& number)
{
  std::ostringstream oss;
  oss << number;
  return oss.str();
}

string seconds2string(int seconds)
{
  //internal variables
  string strSecs = "00";
  string strMin = "00:";
  string strHrs = "";
  int hours = 0;
  int minutes = 0;

  //If we have at least one hour or more, find out how many hours
  if (seconds >= 3600)
  {
    hours = seconds / 3600;
    seconds = seconds % 3600;
  }

  //If we have at least one minute or more, find out how many hours
  if (seconds >= 60)
  {
    minutes = seconds / 60;
    seconds = seconds % 60;
  }

  //Construct our string
  if (hours > 0) //Only display hours area if we have at least one hour
  {
    strHrs = int2string(hours);
  }

  if (minutes > 0)
  {
    if (minutes < 10)
    {
      strMin = "0" + int2string(minutes) + ":";
    }
    else
    {
      strMin = int2string(minutes) + ":";
    }
  }

    if (seconds < 10)
    {
      strSecs = "0" + int2string(seconds);
    }
    else
    {
      strSecs = int2string(seconds);
    }

    return strHrs + strMin + strSecs;
}
 
//This function accepts three strings, a search term, the string to search in, and the string to replace our search with.
string ReplaceWildcard(string strNeedle, string strHaystack, string strReplacement)
{
  size_t intStartRepl = strPlayingText.find(strNeedle);

  if(intStartRepl != string::npos)
  {
    return strPlayingText.replace(intStartRepl, strNeedle.size(), strReplacement);
  }
  else
  {
    return strHaystack;
  }
}


//This is where all of our processing happens.
//We look for a Winamp window in here and if found, we continue on to build a string to send to advertise our music.
void WinampDisplay()
{
  //Not a typo. Winamp uses "Winamp v1.x" for the window ID. this works for 1.x, 2.x, and 5.x versions.
  HWND hWinamp = FindWindow("Winamp v1.x",NULL);

  //Test to see if we got a reference to the window.
  if (!hWinamp)
  {
    //We didn't find the window, so print an error message and stop.
    sys_Printf(BIC_ERROR,"%s: Can't find the Winamp window. Refusing to run.\n",Plugin.m_Name);
  }
  else
  {
    //Open a file handle to the custom format file.
    ifstream inFormatFile("plugins\\WinAMP.ini", ios::in);

    //Does the handle exist (aka file exists)?
    if(!inFormatFile)
    {
      //File doesn't exist, print an error. The default text will be used.
      sys_Printf(BIC_ERROR,"%s: Failed to open format file. Using default text.\n",Plugin.m_Name);
    }
    else
    {
      //String to hold our format info
      string strFormatLine;

      //File exists, get the top line from it.
      getline(inFormatFile,strFormatLine);

      //Check to make sure the file isn't blank.
      if (strFormatLine.size() > 0)
      {
        strPlayingText = strFormatLine;
      }

      //Be a good coder and close our file since we don't need it open anymore.
      inFormatFile.close();
    }

    //Get our Winamp Info. Winamp info is prefixed wa

    //waPlayingState
    LRESULT waPlayStateInt = SendMessage(hWinamp, WM_WA_IPC, 0, IPC_ISPLAYING); //Ask Winamp the state.
    string waPlayState;
    switch (waPlayStateInt)
    {
      case 0: waPlayState = "Stopped"; break;
      case 1: waPlayState = "Playing"; break;
      case 3: waPlayState = "Paused"; break;
    }

    //waCurrentTime
    LRESULT waCurrentTimeInt = SendMessage(hWinamp, WM_WA_IPC, 0, IPC_GETOUTPUTTIME); //The current position in miliseconds (-1 = not playing)

    //If WinAmp isn't playing, it returns -1 as the current time.
    //For our purposes, this is the same as being at time 0
    if (waCurrentTimeInt == -1)
    {
      waCurrentTimeInt = 0;
    }

    //WinAmp returns the playing time in milliseconds, which we need to convert to seconds for our purposes
    waCurrentTimeInt /= 1000;

    string waCurrentTime = seconds2string((int)waCurrentTimeInt);

    //waCurrentLength
    LRESULT waCurrentLengthInt = SendMessage(hWinamp, WM_WA_IPC, 1, IPC_GETOUTPUTTIME); //The length in seconds

    //If WinAmp isn't playing, it returns -1 as the current length.
    //For our purposes, this is the same as a length of 0
    if (waCurrentLengthInt == -1)
    {
      waCurrentLengthInt = 0;
    }

    string waCurrentLength = seconds2string((int)waCurrentLengthInt);


    //waCurrentPcnt - Calculate percentages
    int waCurrentPcntInt;
    string waCurrentPcnt;

    //Catch divide by zero errors (length CAN be zero if WA isn't playing)
    if (waCurrentLengthInt != 0)
    {
      //Calculate our percent of play
      waCurrentPcntInt = int (waCurrentTimeInt * 100 / waCurrentLengthInt);
    }
    else
    {
      waCurrentPcntInt = 0;
    }

    waCurrentPcnt = int2string(waCurrentPcntInt) + "%";

    //waBarPlay and waBarNoPlay
    int waBarPlayInt;
    int waBarNoPlayInt;
    string waBarPlay = "";
    string waBarNoPlay = "";

    //Calculate the number of bars we will need
    if (waCurrentPcntInt != 0)
    {
      waBarPlayInt = waCurrentPcntInt / 10;
      waBarNoPlayInt = 10 - waBarPlayInt;
    }
    else
    {
      waBarPlayInt = 0;
      waBarNoPlayInt = 10;
    }

    //Make our strings
    if (waBarPlayInt > 0)
    {
      for(int i = 0; i < waBarPlayInt; i++)
      {
        waBarPlay += ">";
      }

      for(int i = 0; i < waBarNoPlayInt; i++)
      {
        waBarNoPlay += ">";
      }

    }
    else
    {
      waBarPlay = "";
      waBarNoPlay= ">>>>>>>>>>";
    }

    //waSampleRate
    LRESULT waSampleRateInt = SendMessage(hWinamp, WM_USER, 0, IPC_GETINFO);
    string waSampleRate = int2string((int)waSampleRateInt) + "kHz";

    //waBitRate
    LRESULT waBitRateInt = SendMessage(hWinamp, WM_USER, 1, IPC_GETINFO);
    string waBitRate = int2string((int)waBitRateInt) + "kbps";

    //waChannels
    LRESULT waChannelsInt = SendMessage(hWinamp, WM_USER, 2, IPC_GETINFO);
    string waChannels (waChannelsInt==2?"stereo":"mono"); //Simple Trinary. Need to figure out what to do with 0 or >2 channels.



    //waTitle
    char waTitleBuffer[2048];  //A character buffer to hold the window title

    //Put the window title in the variable, truncate at the size of our buffer
    ::GetWindowText(hWinamp, waTitleBuffer, sizeof(waTitleBuffer));

    string waTitle = waTitleBuffer;

    //TODO - Fix for scrolling titles

    //Remove playlist position number (Add 2 to include period and space after)
    waTitle = waTitle.substr(waTitle.find(".")+2, waTitle.size());

    //Remove the Winamp tag at the end of the string 
    int titlePos = waTitle.rfind(" - Winamp");
    if (titlePos >= 0) {
      waTitle = waTitle.substr(0, titlePos);
    }
    // TODO: find a better way is the response from Media Monkey contains a
    // - to seperate the artist from the track name and has no trailing Winamp tag

    //Replace the values in our playing string
    strPlayingText = ReplaceWildcard("%title%", strPlayingText, waTitle);
    strPlayingText = ReplaceWildcard("%state%", strPlayingText, waPlayState);
    strPlayingText = ReplaceWildcard("%time%", strPlayingText, waCurrentTime);
    strPlayingText = ReplaceWildcard("%length%", strPlayingText, waCurrentLength);
    strPlayingText = ReplaceWildcard("%percent%", strPlayingText, waCurrentPcnt);
    strPlayingText = ReplaceWildcard("%barplay%", strPlayingText, waBarPlay);
    strPlayingText = ReplaceWildcard("%barnoplay%", strPlayingText, waBarNoPlay);
    strPlayingText = ReplaceWildcard("%samplerate%", strPlayingText, waSampleRate);
    strPlayingText = ReplaceWildcard("%bitrate%", strPlayingText, waBitRate);
    strPlayingText = ReplaceWildcard("%channels%", strPlayingText, waChannels);

    //Build our message to send out.
    char *Message = HydraIRC_BuildString(512,"%s", strPlayingText.c_str());

    if (Message && *waTitleBuffer)
    {
      HIRC_WindowInfo_t WI;
      if (HydraIRC_GetActiveWindowInfo(&WI) && WI.WindowType != CWTYPE_SERVER)
      {
        switch(WI.WindowType)
        {
          case CWTYPE_CHANNEL:
          case CWTYPE_DCCCHAT:
          case CWTYPE_QUERY:
            HydraIRC_CMD_Action( WI.WindowID, Message);
            break;
          // Note: can't send to servers...
        }
      }
      else
        sys_Printf(BIC_ERROR,"%s: Nowhere to display the winamp info!\n",Plugin.m_Name);
    }
  }
}

BOOL Plugin_OnCommandMessage( WORD wID )
{
  BOOL bHandled = FALSE;

  if (wID == ToolbarIDs[WINAMP_BUTTON1])
  {
    bHandled = TRUE; // it's one of ours!
    WinampDisplay();
  }

  return bHandled;
}
/*
  if (ChannelID) 
  {
    // Send it to the channel!
    HydraIRC_CMD_ChannelMsg(ChannelID,"Message");
  }
*/


/*
 *	Actual Plugin -> HydraIRC functions start here
 */

BOOL Plugin_ProcessCommand(int ServerID, int ChannelID, char **Command, char **Args)
{
  BOOL Processed = FALSE;

  return Processed;
}

// return TRUE if command has been processed to half further processing
BOOL Plugin_ProcessChannelCommand(int ServerID, int ChannelID, char **Command, char **Args)
{
  BOOL Processed = FALSE;

  if (stricmp(*Command,"WINAMP") == 0)
  {
    WinampDisplay();
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

PLUGIN_API HydraIRCPlugin *HydraIRCPlugin_Init(void)
{
  return &Plugin;
}

/*
 *	Here's the plugin interface.
 */

CPlugin::CPlugin( void )
{ 
  m_Name = "WinAMP"; // only use alpha-numeric, -'s and _'s and NO SPACES
  m_Author = "Hydra";
  m_Info = __DATE__;
	return; 
}

BOOL CPlugin::RequestAPIs( void )
{
  bool loadedPlugin = FALSE;

  if (Plugin.AddAPI(PLUGIN_API_GUI,"Adds winamp toolbar button",sizeof(PluginGUITable)) &&
      Plugin.AddAPI(PLUGIN_API_BASIC,NULL,sizeof(PluginBasicTable),PLUGIN_REQUIRES,&__BASICTABLENAME))
    loadedPlugin = TRUE;
  else
    loadedPlugin = FALSE;

  if (AddAPI(PLUGIN_API_CMDPROC,"Command Processor for /winamp",sizeof(PluginCmdProcTable)) &&
      AddAPI(PLUGIN_API_BASIC,NULL,sizeof(PluginBasicTable),PLUGIN_REQUIRES,&__BASICTABLENAME))
    loadedPlugin = TRUE;
  else
    loadedPlugin = FALSE;

  return loadedPlugin;
}

BOOL CPlugin::GetAPI(APIDescriptor_t *pAPID)
{
  switch (pAPID->m_API)
  {
    case PLUGIN_API_GUI:
      // fill in the table if we're being asked to.
      if (pAPID->m_Type == PLUGIN_PROVIDES)
      {
        PluginGUITable* pTable= static_cast<PluginGUITable*>(pAPID->m_pTable);

        // fill in the table with the functions we implement.
        pTable->m_pfnOnCommandMessage = Plugin_OnCommandMessage;
        
        return TRUE;
      }
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

HWND g_hToolBarWnd;
//HIMAGELIST g_hToolbarImageList;
CImageList g_ToolbarImageList;
CBitmap g_ToolbarBitmap;
CReBarCtrl g_Rebar;
CToolBarCtrl g_Toolbar;
//int g_FirstImage,g_LastImage;
BOOL g_ButtonsAdded = FALSE;
int g_ImageListID = -1;

void CPlugin::OnActivate( void )
{
  OutputDebugString("CPlugin::OnActivate() called\n");

  sys_Printf(BIC_INFO,"%s Plugin by %s Activated\n", m_Name, m_Author);

  // do activation/initialisation code here

  g_ImageListID = HydraIRC_GetNewImageListID();

  int i;

  for (i = 0 ; i < TOOLBAR_BUTTON_COUNT; i ++)
    ToolbarIDs[i] = HydraIRC_GetNewToolbarID();

  /*
  g_hToolbarImageList = HydraIRC_GetToolbarImageList();
  if (g_hToolbarImageList)
  {
    g_ToolbarImageList.Attach(g_hToolbarImageList);

    g_FirstImage = g_ToolbarImageList.GetImageCount();
	  g_ToolbarBitmap.LoadBitmap(IDB_TOOLBAR);
	  g_ToolbarImageList.Add( g_ToolbarBitmap, RGB(255,0,255));
    g_LastImage = g_ToolbarImageList.GetImageCount() - 1;
  }
  else
    sys_Printf(BIC_ERROR,"%s: Failed to get imagelist!\n",Plugin.m_Name);
  */

  HWND toolbarChild = HydraIRC_GetToolbar();
  if (!toolbarChild) {
    sys_Printf(BIC_ERROR,"%s: Failed to get toolbar band info!\n",Plugin.m_Name);
    return;
  }

  g_Toolbar.Attach(toolbarChild);

  //g_ToolbarBitmap.LoadBitmap(IDB_TOOLBAR);

  //g_ToolbarImageList.Add( g_ToolbarBitmap, RGB(255,0,255));

  g_ToolbarImageList.CreateFromImage(IDB_TOOLBAR, 16, 17, RGB(255,0,255), IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_SHARED);
  g_Toolbar.SetImageList(g_ToolbarImageList,g_ImageListID);

  //g_ToolbarImageList.Add( g_ToolbarBitmap, RGB(255,0,255));    

  TBBUTTON tb[TOOLBAR_BUTTON_COUNT];
  memset(tb, 0, sizeof(tb));
  int n = 0, image = 0;//g_FirstImage;

  tb[n].iBitmap = 0;
  tb[n].idCommand = ToolbarIDs[n];
  tb[n].fsState = TBSTATE_ENABLED;
  tb[n].fsStyle = TBSTYLE_SEP;

  n++;
  tb[n].iBitmap = MAKELONG(image++,g_ImageListID);
  tb[n].idCommand = ToolbarIDs[n];
  tb[n].fsState = TBSTATE_ENABLED;
  tb[n].fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE;
  //tb[n].iString = (int)("Show Winamp song\0");

  g_ButtonsAdded = g_Toolbar.AddButtons(TOOLBAR_BUTTON_COUNT,tb);

  // TODO: force a recalcuate of the toolbar's ideal size
  // see WTL::CFrameWindowImplBase::AddSimpleReBarBandCtrl
  // the bit where it calculates the size.
}

void CPlugin::OnDeactivate( void )
{
  OutputDebugString("CPlugin::OnDeactivate() called\n");

  sys_Printf(BIC_INFO,"%s Plugin by %s Deactivated\n", m_Name, m_Author);

  // do de-activation code here

  int i;

  /*
  if (g_hToolbarImageList)
  {
    int i = g_LastImage;
    int count = 0;
    while (i >= g_FirstImage)
    {
      count++;
      g_ToolbarImageList.Remove(i--);
    }

    g_ToolbarImageList.Detach();
  }
  */

  if (g_hToolBarWnd)
  {
    g_ToolbarImageList.Destroy();
    // remove the buttons (remembering that other buttons may have been added
    // or removed since we started, so we can't do it by remember the indexes when
    // we add the buttons...
    if (g_ButtonsAdded)
    {
      for (i = 0 ; i < TOOLBAR_BUTTON_COUNT; i ++)
        g_Toolbar.DeleteButton(g_Toolbar.CommandToIndex(ToolbarIDs[i]));
    }

    g_Rebar.Detach();
  }

  for (i = 0 ; i < TOOLBAR_BUTTON_COUNT; i ++)
    HydraIRC_ReleaseToolbarID(ToolbarIDs[i]);

  HydraIRC_ReleaseImageListID(g_ImageListID);

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

// see: http://www.winamp.com/nsdn/winamp2x/dev/sdk/api.jhtml

