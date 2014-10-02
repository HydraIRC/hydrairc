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

// Prefs_IdentitiesPage.h : interface of the CNotificationsPage class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

void CNotificationsPage::OnPageDisplay( void )
{
  int newitemnum,networkid,len;
  char *tmpstr, *groupname, *networkname;

  m_GroupCtrl.Attach(GetDlgItem(IDC_NOTIFICATIONS_GROUP));

  newitemnum = m_GroupCtrl.AddString("All Networks / Defaults");
  m_GroupCtrl.SetItemDataPtr(newitemnum,NULL);
  for (int i = 0 ; i < g_pNewPrefs->m_CustomUserGroups.m_UserGroupList.GetSize() ; i++ )
  {
    groupname = g_pNewPrefs->m_CustomUserGroups.m_UserGroupList[i]->m_GroupName;
    networkid = g_pNewPrefs->m_CustomUserGroups.m_UserGroupList[i]->m_NetworkID;
    if (networkid == -1)
      networkname = "All Networks";
    else
      networkname = GetNetworkName(networkid); // oops
    len = strlen(networkname) + 3 + strlen(groupname) + 1;
    tmpstr = (char *)malloc(len);
    if (tmpstr)
    {
      sprintf(tmpstr,"%s / %s",networkname, groupname);
      newitemnum = m_GroupCtrl.AddString(tmpstr);
      free(tmpstr);
      if (newitemnum != CB_ERR) 
        m_GroupCtrl.SetItemDataPtr(newitemnum,g_pNewPrefs->m_CustomUserGroups.m_UserGroupList[i]); 
      else 
        ATLASSERT(FALSE);    
    }
  }

  m_EventsCtrl.Attach(GetDlgItem(IDC_NOTIFICATIONS_EVENTS));
  m_EventsCtrl.AddColumn("Enabled",0);
  m_EventsCtrl.AddColumn("Event Name",1);
  m_EventsCtrl.AddColumn("Match",2);
  m_EventsCtrl.AddColumn("Sound Name",3);
  m_EventsCtrl.AddColumn("Play Sound",4);
  m_EventsCtrl.AddColumn("Flash Taskbar",5);
  m_EventsCtrl.AddColumn("Log",6);

  UpdateWindow();

  m_GroupCtrl.SetCurSel(0);
  // hmm, not working... 
  // m_GroupCtrl.SendMessage(WM_NOTIFY,MAKEWPARAM(0,CBN_SELCHANGE),(LPARAM)m_GroupCtrl.m_hWnd);
  // call it directly instead
  UpdateEventView(0);

}

void CNotificationsPage::OnPageDone( void )
{
  while (m_GroupCtrl.GetCount() > 0)
  {
    m_GroupCtrl.DeleteString(0);
  }
  m_GroupCtrl.Detach();

  while (m_EventsCtrl.GetItemCount() > 0)
  {
    m_EventsCtrl.DeleteAllItems();
  }
  m_EventsCtrl.DeleteColumn(6);
  m_EventsCtrl.DeleteColumn(5);
  m_EventsCtrl.DeleteColumn(4);
  m_EventsCtrl.DeleteColumn(3);
  m_EventsCtrl.DeleteColumn(2);
  m_EventsCtrl.DeleteColumn(1);
  m_EventsCtrl.DeleteColumn(0);
  m_EventsCtrl.Detach();
}

BOOL CNotificationsPage::OnPageValidate( void )
{
  return TRUE;
}

LRESULT CNotificationsPage::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  // the real init is done when the page is OnPageDisplay'd, as other preference items might
  // make a difference to the data
  DlgResize_Init(false,true,0);
  return 0;
}

void CNotificationsPage::UpdateEventView( int ItemNum)
{
  NotificationPrefInfo_t *pNPI;
  int GroupID;
  m_EventsCtrl.DeleteAllItems();
  char *soundidstr;
  char *filename;

  if (ItemNum == CB_ERR)
    return;

  // find the groupid.
  IRCUserGroup *pGroup = (IRCUserGroup *)m_GroupCtrl.GetItemDataPtr(ItemNum);
  if (pGroup)
    GroupID = pGroup->m_GroupID;
  else
    GroupID = -2;

  int EventCount = 0;
#ifdef DEBUG
  sys_Printf(BIC_INFO,"Group Name: %s\n",GroupID == -2 ? "Default" : pGroup->m_GroupName);
#endif

  for (int i = 0 ; i < g_pNewPrefs->m_NotificationPrefList.GetSize(); i ++)
  {
    pNPI = g_pNewPrefs->m_NotificationPrefList[i];
    if (pNPI->m_GroupID == GroupID)
    {
      // TODO: add the events to the list.
      m_EventsCtrl.AddItem(EventCount,0,pNPI->m_Flags & NE_FLAG_DISABLED ? "Disabled" : "Enabled");
      m_EventsCtrl.AddItem(EventCount,1,g_NotificationEventNames[pNPI->m_EventCode]);      

      if (pNPI->m_MatchString)
        m_EventsCtrl.AddItem(EventCount,2,pNPI->m_MatchString);
      else
        m_EventsCtrl.AddItem(EventCount,2,"Not Specified");

      if (pNPI->m_SoundID < SID_LAST)
        soundidstr = g_SoundIDDisplayNames[pNPI->m_SoundID];
      else
        soundidstr = "Invalid SoundID!";

      if (pNPI->m_Flags & NE_FLAG_SOUNDFILE)
      {
        // just show the filename, not the directory it's in too...
        if (pNPI->m_FileName)
        {
          filename = strrchr(pNPI->m_FileName,'\\');
          if (!filename || !*++filename) // skip the \, check we've still got data.
            filename = pNPI->m_FileName;
        }
        else
          filename = "Bad Sound File!";

        m_EventsCtrl.AddItem(EventCount,3,filename);
      }
      else if (pNPI->m_Flags & NE_FLAG_SYSTEMSOUND)
        m_EventsCtrl.AddItem(EventCount,3,(pNPI->m_SoundID == -1) ? "Bad Sound ID!" : soundidstr);
      else
        m_EventsCtrl.AddItem(EventCount,3,"Not Specified");

      m_EventsCtrl.AddItem(EventCount,4,(pNPI->m_Flags & (NE_FLAG_SOUNDFILE | NE_FLAG_SYSTEMSOUND)) > 0 ? "Yes" : "No");
      m_EventsCtrl.AddItem(EventCount,5,(pNPI->m_Flags & NE_FLAG_FLASHTASKBAR) ? "Yes" : "No");
      m_EventsCtrl.AddItem(EventCount,6,(pNPI->m_Flags & NE_FLAG_LOGINEVENTLOG) ? "Yes" : "No");
      m_EventsCtrl.SetItemData(EventCount,(DWORD_PTR)pNPI);      
      EventCount++;
#ifdef DEBUG
      sys_Printf(BIC_INFO,"Found event: EventCode: %04d Flags: 0x%04x SoundID: %04d SoundName: \"%s\"\n",pNPI->m_EventCode,pNPI->m_Flags,pNPI->m_SoundID,pNPI->m_FileName ? pNPI->m_FileName : "No Filename");
#endif
    }
  }

#ifdef DEBUG
  sys_Printf(BIC_INFO,"Found %d event(s) for this group\n",EventCount);
#endif
}

LRESULT CNotificationsPage::OnCbnSelchangeNotificationsGroup(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  UpdateEventView(m_GroupCtrl.GetCurSel());
  return 0;
}

LRESULT CNotificationsPage::OnNMRclickNotificationsEvents(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
{
  LVHITTESTINFO ht = {0};
  DWORD dwpos = GetMessagePos();
  ht.pt.x = GET_X_LPARAM(dwpos);
  ht.pt.y = GET_Y_LPARAM(dwpos);
  // TODO: check ok on multi-monitor system
  ::MapWindowPoints(HWND_DESKTOP, pnmh->hwndFrom, &ht.pt, 1);
  ListView_HitTest(pnmh->hwndFrom, &ht);
  if(LVHT_ONITEM  & ht.flags)
  {
    NotificationPrefInfo_t *pNPI = (NotificationPrefInfo_t *)m_EventsCtrl.GetItemData(ht.iItem);

    // if an event has both a sound file, AND a systemsound (it shouldn't, but
    // it doesn't hurt) the play the sound file first, if set.
    if ((pNPI->m_Flags & (NE_FLAG_SOUNDFILE | NE_FLAG_SYSTEMSOUND)) > 0)
    {
      if (pNPI->m_FileName)
      {
        g_pNotificationManager->PlaySoundFile(pNPI->m_FileName);
      }
      else
      {
        g_pNotificationManager->PlaySoundID(pNPI->m_SoundID);
      }
    }
  }

  return 0;
}

void CNotificationsPage::EditEvent(int ItemNum)
{
  NotificationPrefInfo_t *pNewNPI = (NotificationPrefInfo_t *)malloc(sizeof(NotificationPrefInfo_t));
  if (!pNewNPI)
    return;

  pNewNPI->m_MatchString = NULL;
  pNewNPI->m_FileName = NULL;

  NotificationPrefInfo_t *pNPI = (NotificationPrefInfo_t *)m_EventsCtrl.GetItemData(ItemNum);

  CopyMemory(pNewNPI,pNPI,sizeof(NotificationPrefInfo_t));
  pNewNPI->m_FileName = strdup(pNPI->m_FileName);

  // if there was a filename top copy, make sure we copied it.
  if (!pNewNPI->m_FileName && pNPI->m_FileName)
  {
    free(pNewNPI);
    return;
  }

  pNewNPI->m_MatchString = strdup(pNPI->m_MatchString);
  // if there was a matchstring top copy, make sure we copied it.
  if (!pNewNPI->m_MatchString && pNPI->m_MatchString)
  {
    free(pNewNPI->m_FileName);
    free(pNewNPI);
    return;
  }

  // edit a copy of the old one.
  CEditNotificationDlg Dlg(pNewNPI);
  int result = Dlg.DoModal();

  if (result == 1)
  {
    free(pNPI->m_FileName);
    free(pNPI->m_MatchString);
    CopyMemory(pNPI,pNewNPI,sizeof(NotificationPrefInfo_t));
    free(pNewNPI);
    UpdateEventView(m_GroupCtrl.GetCurSel());
  }
  else
  {
    if (pNewNPI->m_FileName) free(pNewNPI->m_FileName);
    if (pNewNPI->m_MatchString) free(pNewNPI->m_MatchString);
    free(pNewNPI);
  }
}

LRESULT CNotificationsPage::OnNMDblclkNotificationsEvents(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
  LVHITTESTINFO ht = {0};
  DWORD dwpos = GetMessagePos();
  ht.pt.x = GET_X_LPARAM(dwpos);
  ht.pt.y = GET_Y_LPARAM(dwpos);
  // TODO: check ok on multi-monitor system
  ::MapWindowPoints(HWND_DESKTOP, pNMHDR->hwndFrom, &ht.pt, 1);
  ListView_HitTest(pNMHDR->hwndFrom, &ht);
  if(LVHT_ONITEM  & ht.flags)
  {
    EditEvent(ht.iItem);
  }
  return 0;
}

LRESULT CNotificationsPage::OnBnClickedNotificationsNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  int ItemNum = m_GroupCtrl.GetCurSel();

  if (ItemNum == CB_ERR)
    return 0;

  NotificationPrefInfo_t *pNewNPI = (NotificationPrefInfo_t *)malloc(sizeof(NotificationPrefInfo_t));
  if (!pNewNPI)
    return 0;

  // find the groupid.
  ZeroMemory(pNewNPI,sizeof(NotificationPrefInfo_t));

  pNewNPI->m_ForegroundColor = -1; // because 0 == rgb black
  pNewNPI->m_BackgroundColor = -1; // because 0 == rgb black

  pNewNPI->m_HighlightFlags = HIGHLIGHT_NONE;
  pNewNPI->m_Flags = NE_FLAG_NONE | NE_FLAG_LOGINEVENTLOG;

  pNewNPI->m_EventCode = -1; // setting this tells the dialog that the event is a new one..
  IRCUserGroup *pGroup = (IRCUserGroup *)m_GroupCtrl.GetItemDataPtr(ItemNum);
  if (pGroup)
    pNewNPI->m_GroupID = pGroup->m_GroupID;
  else
    pNewNPI->m_GroupID = -2;

  
  CEditNotificationDlg Dlg(pNewNPI);
  int result = Dlg.DoModal();

  if (result == 1)
  {
    g_pNewPrefs->m_NotificationPrefList.Add(pNewNPI);
    UpdateEventView(m_GroupCtrl.GetCurSel());
  }
  else
  {
    if (pNewNPI->m_FileName) free(pNewNPI->m_FileName);
    if (pNewNPI->m_MatchString) free(pNewNPI->m_MatchString);
    free(pNewNPI);
  }


  return 0;
}

LRESULT CNotificationsPage::OnBnClickedNotificationsDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  int ItemNum = m_EventsCtrl.GetSelectedIndex();

  if (ItemNum != -1)
  {
    NotificationPrefInfo_t *pNPI = (NotificationPrefInfo_t *)m_EventsCtrl.GetItemData(ItemNum);
    
    if (pNPI)
    {
      g_pNewPrefs->m_NotificationPrefList.Remove(pNPI);
      if (pNPI->m_FileName) free(pNPI->m_FileName);
      if (pNPI->m_MatchString) free(pNPI->m_MatchString);
      free(pNPI);
    }
    UpdateEventView(m_GroupCtrl.GetCurSel());
  }
  return 0;
}

LRESULT CNotificationsPage::OnBnClickedNotificationsEdit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  int ItemNum = m_EventsCtrl.GetSelectedIndex();

  if (ItemNum != -1)
  {
    EditEvent(ItemNum);
  }
  return 0;
}
