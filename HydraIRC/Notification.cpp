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

// Audio.cpp : implementation of the NotificationManager class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

// see enum NotificationEventCodes, must be the same order.
char *g_NotificationEventNames[NE_LAST] = 
{
  "Logged In",     
  "Disconnect",
  "WeKick",
  "WeJoin",
  "WePart",
  "Highlight",   
  "Join",        
  "Kick",        
  "Part",        
  "Quit",        
  "Privmsg",
  "Notice",
  "ChannelText",
  "Action",
  "Topic",
  "DCC Chat Connect",
  "DCC Chat Disconnect",
  "DCC File Receive",
  "DCC File Send",
  "DCC File Receive Complete",
  "DCC File Send Complete",
  "DCC File Receive Failed",
  "DCC File Send Failed",
  "CTCP Ping"
};

/*
int g_NEIDtoELIDMappings[NE_LAST] = {
  ELID_Connect,      //"Logged In"
  ELID_Disconnect,   //"Disconnect"
  ELID_Kicked,       //"WeKick"
  ELID_Unknown,      //"WeJoin"
  ELID_Unknown,      //"WePart"
  ELID_Unknown,      //"Highlight"
  ELID_Unknown,      //"Join"
  ELID_Kicked,       //"Kick"
  ELID_Unknown,      //"Part"
  ELID_Unknown,      //"Quit"
  ELID_PrivMsg,      //"Privmsg"
  ELID_Notice,       //"Notice"
  ELID_Unknown,      //"ChannelText"
  ELID_Unknown,      //"Action"
  ELID_Unknown,      //"Topic"
  ELID_Connect,      //"DCC Chat Connect"
  ELID_Disconnect,   //"DCC Chat Disconnect"
  ELID_DCC_Receive,  //"DCC File Receive"
  ELID_DCC_Send,     //"DCC File Send"
  ELID_DCC_Complete, //"DCC File Receive Complete"
  ELID_DCC_Complete, //"DCC File Send Complete"
  ELID_DCC_Failed,   //"DCC File Receive Failed"
  ELID_DCC_Failed    //"DCC File Send Failed"  
};
*/

char *g_SoundIDNames[SID_LAST] = 
{
  ".Default",
  "SystemQuestion",
  "SystemNotification",
  "SystemExclamation",
  "SystemAsterisk"
};

// as displayed in prefs..
char *g_SoundIDDisplayNames[SID_LAST] = 
{
  "Default",
  "Question",
  "Notification",
  "Exclamation",
  "Asterisk"
};

int NotificationManager::ObtainSourceID(void)
{
  return m_SourceIDs.ObtainID();
}

void NotificationManager::DeleteSource(int SourceID)
{
  DeleteEvents(SourceID);
  m_SourceIDs.RelinquishID(SourceID);
}

void NotificationManager::DeleteEvents(int SourceID)
{
  NotificationEvent_t *pNE;
  for (int i = 0 ; i < m_NotificationEventList.GetSize() ; i ++)
  {
    pNE = m_NotificationEventList[i];
    if (pNE->m_SourceID == SourceID)
    {
      Delete(pNE);
      i--; // reset the loop counter 
    }
  }
}

void NotificationManager::Delete(NotificationEvent_t *pNE)
{
  if (pNE)
  {
    m_NotificationEventList.Remove(pNE);
    if (pNE->m_FileName) free(pNE->m_FileName);
    free(pNE);
  }
}

void NotificationManager::DeleteAll( void )
{
  while(m_NotificationEventList.GetSize() > 0)
  {
    Delete(m_NotificationEventList[0]);
  }
}

void NotificationManager::DoNotification(NotificationPrefInfo_t *pNPI)
{
  // convert pNPI to pNE and call the other DoNotification
  if (!pNPI)
    return;

  NotificationEvent_t *pNE;
  pNE = (NotificationEvent_t *)malloc(sizeof(NotificationEvent_t));
  if (pNE)
  {
    pNE->m_SourceID = -1;
    pNE->m_EventCode = pNPI->m_EventCode;
    pNE->m_SoundID = pNPI->m_SoundID;
    pNE->m_FileName = pNPI->m_FileName; // not taking a copy, we just use the ptr
    pNE->m_UserData = NULL; // overridden depending on EventCode
    pNE->m_Flags = pNPI->m_Flags;

    DoNotification(pNE);

    free(pNE);
  }

}

NotificationEvent_t *NotificationManager::Add(int SourceID, NotificationPrefInfo_t *pNPI )
{
  if (!m_SourceIDs.IsValid(SourceID))
    return NULL; // not a valid source.

  if (pNPI->m_EventCode == NE_HIGHLIGHT)
    return NULL; // we don't add these!

  NotificationEvent_t *pNE;
  pNE = (NotificationEvent_t *)malloc(sizeof(NotificationEvent_t));
  if (pNE)
  {
    pNE->m_SourceID = SourceID;
    pNE->m_EventCode = pNPI->m_EventCode;
    pNE->m_SoundID = pNPI->m_SoundID;
    pNE->m_FileName = strdup(pNPI->m_FileName);
    pNE->m_UserData = NULL; // overridden depending on EventCode
    pNE->m_Flags = pNPI->m_Flags;

    switch(pNE->m_EventCode)
    {
      case NE_JOIN:
      case NE_KICK:
      case NE_PART:
      case NE_QUIT:
      case NE_PRIVMSG:
      case NE_NOTICE:
      case NE_CHANNELTEXT:
      case NE_ACTION:
      case NE_DCCCHATCONNECT:
      case NE_DCCCHATDISCONNECT:
      case NE_DCCFILERECEIVE:
      case NE_DCCFILESEND:
      case NE_DCCFILERECEIVECOMPLETE:
      case NE_DCCFILESENDCOMPLETE:
      case NE_DCCFILERECEIVEFAILED:
      case NE_DCCFILESENDFAILED:
      case NE_CTCPPING:

        if (pNPI->m_GroupID != -2)
          pNE->m_UserData = (void *)g_pPrefs->m_CustomUserGroups.FindByGroupID(pNPI->m_GroupID);
      break;
    }
    m_NotificationEventList.Add(pNE);
    return pNE;
  }
  return NULL;
}

NotificationManager::~NotificationManager( void )
{
  DeleteAll();
}

void NotificationManager::PlaySoundFile(char *FileName)
{
#ifdef DEBUG
  sys_Printf(BIC_INFO,"Playing Audio File: \"%s\"\n",FileName);
#endif
  if (!BOOLPREF(PREF_bAudioMute))
    PlaySound(FileName, NULL, SND_FILENAME | SND_ASYNC | SND_NOWAIT);
}

void NotificationManager::PlaySoundID(int SoundID)
{
#ifdef DEBUG
  sys_Printf(BIC_INFO,"Playing Audio ID: \"%d\" (%s)\n",SoundID,g_SoundIDNames[SoundID]);
#endif
  if (!BOOLPREF(PREF_bAudioMute))
    PlaySound(g_SoundIDNames[SoundID], NULL, SND_ALIAS | SND_ASYNC | SND_NOWAIT);
}


void NotificationManager::DoNotification(NotificationEvent_t *pNE)
{
  if (pNE->m_Flags & NE_FLAG_DISABLED)
    return;

  if (pNE->m_Flags & NE_FLAG_SOUNDFILE)
  {
    // if w've got a file to play, play it,
    if (pNE->m_FileName && *pNE->m_FileName)
    {
      PlaySoundFile(pNE->m_FileName);
    }
    else
    {
      // otherwise, beep! (maybe the user moved the sound file?)
      PlaySoundID(SID_DEFAULT);
    }
  }

  if (pNE->m_Flags & NE_FLAG_SYSTEMSOUND)
  {
    PlaySoundID(pNE->m_SoundID);
  }

  if (pNE->m_Flags & NE_FLAG_FLASHTASKBAR)
  {
    FLASHWINFO fwi;
    fwi.cbSize = sizeof(FLASHWINFO);
    fwi.hwnd = g_pMainWnd->m_hWnd;
    fwi.dwTimeout = 0;

    if (GetForegroundWindow() == g_pMainWnd->m_hWnd)
    {
      fwi.uCount = 3;
      fwi.dwFlags = FLASHW_ALL;
    }
    else
    {
      fwi.uCount = 0;
      fwi.dwFlags = FLASHW_TRAY | FLASHW_TIMERNOFG; // FLASHW_ALL // FLASHW_CAPTION
    }
    FlashWindowEx(&fwi);
  }

}

/** Play a sound for an event if one has been configured.
 *
 * If a sound has NOT been configured, then we stay silent.
 * If a buddy group specific sound event is configured then
 * it overrides the default events.
 */
NotificationEvent_t *NotificationManager::NotificationEvent(int EventCode, int NetworkID, const void *Data)
{
#ifdef DEBUG
  //sys_Printf(BIC_INFO,"Notification Event: \"%s\"\n",g_NotificationEventNames[EventCode]);
#endif
  BOOL FoundEvent = FALSE;
  NotificationEvent_t *pNE = NULL;
  IRCUserGroup *pGroup = NULL;
  char *Nick = NULL;

  // do an init for the rest of this part..
  switch(EventCode)
  {
    case NE_JOIN:
    case NE_KICK:
    case NE_PART:
    case NE_QUIT:
    case NE_PRIVMSG:
    case NE_NOTICE:
    case NE_CHANNELTEXT:
    case NE_ACTION:
    case NE_TOPIC:
    case NE_DCCCHATCONNECT:
    case NE_DCCCHATDISCONNECT:
    case NE_DCCFILERECEIVE:
    case NE_DCCFILESEND:
    case NE_DCCFILERECEIVECOMPLETE:
    case NE_DCCFILESENDCOMPLETE:
    case NE_DCCFILERECEIVEFAILED:
    case NE_DCCFILESENDFAILED:
    case NE_CTCPPING:

      Nick = (char *)Data;
      if (Nick)
      {
        pGroup = g_pPrefs->m_CustomUserGroups.FindUserGroup(Nick,NetworkID);
      }
#ifdef DEBUG
      else
      {
        sys_Printf(BIC_WARNING,"Passed NULL data for an event that requires data!\n");
      }
#endif
    break;
  }
  for (int pass = 1 ; pass <= 2 ; pass ++)
  {
    //sys_Printf(BIC_INFO,"Searching for %s event\n",pass == 1 ? "ACTUAL" : "DEFAULT");

    for (int i = 0 ; !FoundEvent && i < m_NotificationEventList.GetSize() ; i ++)
    {
      pNE = m_NotificationEventList[i];
      if (pNE->m_EventCode == EventCode)
      {
        //int EventLogID = g_NEIDtoELIDMappings[EventCode];
        switch(EventCode)
        {
          case NE_LOGGEDIN:
          case NE_DISCONNECT:
          case NE_WEKICK:
          case NE_WEJOIN:
          case NE_WEPART:
              DoNotification(pNE);
              FoundEvent = TRUE;
            break;
    
          case NE_JOIN:
          case NE_KICK:
          case NE_PART:
          case NE_QUIT:
          case NE_PRIVMSG:
          case NE_NOTICE:
          case NE_CHANNELTEXT:
          case NE_ACTION:
          case NE_TOPIC:
          case NE_DCCCHATCONNECT:
          case NE_DCCCHATDISCONNECT:
          case NE_DCCFILERECEIVE:
          case NE_DCCFILESEND:
          case NE_DCCFILERECEIVECOMPLETE:
          case NE_DCCFILESENDCOMPLETE:
          case NE_DCCFILERECEIVEFAILED:
          case NE_DCCFILESENDFAILED:
          case NE_CTCPPING:

            // do we have a match ?

            if (pGroup && pGroup == (IRCUserGroup *)pNE->m_UserData)
            {
              // don't just match the group! match the user in it to the nick too!
              if (pGroup->FindUser(Nick))
              {
                DoNotification(pNE);
                FoundEvent = TRUE;
              }
            }
            else
            {
              // default events have a NULL group, but there might be
              // more events that we've not looked at yet.
              // so only do the event when we've checked them all.
              // so make sure pass == 2
              if (pass==2 && pNE->m_UserData == NULL)
              {
                DoNotification(pNE);
                FoundEvent = TRUE;
              }
            }
            break;
        } // End: Switch EventCode
      } // End: Matching Event Code
    } // End: event search loop
   
    if (FoundEvent || Data == NULL)
      break;
  } // End: 2 pass loop

  return ((FoundEvent) && pNE) ? pNE : NULL;
}

