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

// Prefs_BuddyGroupsPage.h : interface of the CBuddyGroupsPage class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"
#include "prefs_buddygroupspage.h"

void CBuddyGroupsPage::UpdateGroupCtrl( void )
{
  int newitemnum,networkid,len;
  char *tmpstr, *groupname, *networkname;

  m_GroupCtrl.ResetContent();
  for (int i = 0 ; i < g_pNewPrefs->m_CustomUserGroups.m_UserGroupList.GetSize() ; i++ )
  {
    groupname = g_pNewPrefs->m_CustomUserGroups.m_UserGroupList[i]->m_GroupName;
    networkid = g_pNewPrefs->m_CustomUserGroups.m_UserGroupList[i]->m_NetworkID;
    if (networkid == -1)
      networkname = "All Networks";
    else
      networkname = GetNetworkName(networkid);
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
}


void CBuddyGroupsPage::ResetDialog( int GroupNum )
{
  UpdateNickList(GroupNum);
  m_NickCtrl.SetWindowText("Add your buddy here");
  m_NickCtrl.SetSel(0,-1);
  m_HostCtrl.SetWindowText("<Host mask matching not implemented yet>");
}

void CBuddyGroupsPage::OnPageDisplay( void )
{
  m_GroupCtrl.Attach(GetDlgItem(IDC_BUDDYGROUPS_GROUP));
  UpdateGroupCtrl();
  m_NickListCtrl.Attach(GetDlgItem(IDC_BUDDYGROUPS_BUDDYLIST));
  m_NickListCtrl.AddColumn("Name",0);
  m_NickListCtrl.AddColumn("Host",1);

  m_NickCtrl.Attach(GetDlgItem(IDC_BUDDYGROUPS_NICK));
  m_HostCtrl.Attach(GetDlgItem(IDC_BUDDYGROUPS_HOST));
  UpdateWindow();

  m_GroupCtrl.SetCurSel(0);
  ResetDialog(m_GroupCtrl.GetCurSel()); // not 0, because there might not be any groups...
}

void CBuddyGroupsPage::OnPageDone( void )
{
  while (m_GroupCtrl.GetCount() > 0)
  {
    m_GroupCtrl.DeleteString(0);
  }
  m_GroupCtrl.Detach();

  while (m_NickListCtrl.GetItemCount() > 0)
  {
    m_NickListCtrl.DeleteAllItems();
  }
  m_NickListCtrl.DeleteColumn(1);
  m_NickListCtrl.DeleteColumn(0);
  m_NickListCtrl.Detach();
  m_NickCtrl.Detach();
  m_HostCtrl.Detach();
}

BOOL CBuddyGroupsPage::OnPageValidate( void )
{
  return TRUE;
}

LRESULT CBuddyGroupsPage::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  // the real init is done when the page is OnPageDisplay'd, as other preference items might
  // make a difference to the data
  DlgResize_Init(false,true,0);
  return 0;
}

int CALLBACK NickLickSorter(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
  return stricmp(((IRCUser *)(lParam1))->m_Nick,((IRCUser *)(lParam2))->m_Nick);
}

void CBuddyGroupsPage::UpdateNickList( int ItemNum)
{
  int GroupID;
  IRCUser *pUser;

  m_NickListCtrl.DeleteAllItems();

  if (ItemNum == CB_ERR)
    return;

  // find the groupid.
  IRCUserGroup *pGroup = (IRCUserGroup *)m_GroupCtrl.GetItemDataPtr(ItemNum);
  if (pGroup)
    GroupID = pGroup->m_GroupID;
  else
    return;

#ifdef DEBUG
  sys_Printf(BIC_INFO,"Group Name: %s\n",pGroup->m_GroupName);
#endif

  for (int i = 0 ; i < pGroup->m_UserList.GetSize(); i ++)
  {
    pUser = pGroup->m_UserList[i];
    m_NickListCtrl.AddItem(i,0,pUser->m_Nick ? pUser->m_Nick : "*");
    m_NickListCtrl.SetItemData(i,(DWORD_PTR)pUser);
    m_NickListCtrl.AddItem(i,1,pUser->m_Address ? pUser->m_Address : "*@*");
  }
  m_NickListCtrl.SetColumnWidth(0,MAKELPARAM((int)100,0));
  m_NickListCtrl.SetColumnWidth(1,MAKELPARAM((int)LVSCW_AUTOSIZE,0));
  m_NickListCtrl.SortItems(NickLickSorter,0);
}

LRESULT CBuddyGroupsPage::OnCbnSelchangeBuddyGroupsGroup(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  ResetDialog(m_GroupCtrl.GetCurSel());
  return 0;
}

LRESULT CBuddyGroupsPage::OnBnClickedBuddyGroupsAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  CEditBuddyGroupDlg Dlg;
  int result = Dlg.DoModal();
  if (result == 1)
  {
    // create a new group ID.
    int highestnum = 1;
    for (int i = 0 ; i < g_pNewPrefs->m_CustomUserGroups.m_UserGroupList.GetSize(); i ++)
    {
      highestnum = max(highestnum, g_pNewPrefs->m_CustomUserGroups.m_UserGroupList[i]->m_GroupID);
    }
    IRCUserGroup *pGroup = new IRCUserGroup(Dlg.m_Name,highestnum+1,Dlg.m_NetworkID);
    if (pGroup)
    {
      g_pNewPrefs->m_CustomUserGroups.Add(pGroup);
    }
    UpdateGroupCtrl();
    // todo: select this one..
    m_GroupCtrl.SetCurSel(0);
    ResetDialog(m_GroupCtrl.GetCurSel());
  }
  if (Dlg.m_Name) free (Dlg.m_Name);


  return 0;
}

LRESULT CBuddyGroupsPage::OnBnClickedBuddyGroupsDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  int ItemNum = m_GroupCtrl.GetCurSel();

  if (ItemNum == LB_ERR)
    return 0;

  IRCUserGroup *pGroup = (IRCUserGroup *)m_GroupCtrl.GetItemData(ItemNum);
  
  if (!pGroup || (int)pGroup == -1)
    return 0;

  // delete all the associated buddy group notifications for this group.
  NotificationPrefInfo_t *pNPI;
  int i;
  for (i = 0 ; i < g_pNewPrefs->m_NotificationPrefList.GetSize(); i ++)
  {
    pNPI = g_pNewPrefs->m_NotificationPrefList[i];
    if (pNPI->m_GroupID == pGroup->m_GroupID)
    {
      g_pNewPrefs->m_NotificationPrefList.Remove(pNPI);
      if (pNPI->m_FileName) free(pNPI->m_FileName);
      if (pNPI->m_MatchString) free(pNPI->m_MatchString);
      free(pNPI);
      i--; // step back one
    }
  }

  // TODO: At this point, we need to update anything else that may have
  // been using this group.
  // ++nothing other than notifications at this point in time.

  g_pNewPrefs->m_CustomUserGroups.m_UserGroupList.Remove(pGroup);
  delete pGroup;
  m_GroupCtrl.DeleteString(ItemNum);

  m_GroupCtrl.SetCurSel(0);
  ResetDialog(m_GroupCtrl.GetCurSel());
  // reset the controls..

  return 0;
}

LRESULT CBuddyGroupsPage::OnBnClickedBuddyGroupsEdit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  int ItemNum = m_GroupCtrl.GetCurSel();

  if (ItemNum == LB_ERR)
    return 0;

  IRCUserGroup *pGroup = (IRCUserGroup *)m_GroupCtrl.GetItemData(ItemNum); 

  if (!pGroup || (int)pGroup == -1)
    return 0;

  CEditBuddyGroupDlg Dlg;

  Dlg.m_Name=strdup(pGroup->m_GroupName);
  Dlg.m_NetworkID=pGroup->m_NetworkID;

  int result = Dlg.DoModal();
  if (result == 1)
  {
    if (pGroup->m_GroupName) free(pGroup->m_GroupName);
    pGroup->m_GroupName = strdup(Dlg.m_Name);
    pGroup->m_NetworkID = Dlg.m_NetworkID;
    UpdateGroupCtrl();
    m_GroupCtrl.SetCurSel(0);
    ResetDialog(m_GroupCtrl.GetCurSel()); // not 0, because there might not be any groups...
  }

  if (Dlg.m_Name) free (Dlg.m_Name);

  return 0;
}

LRESULT CBuddyGroupsPage::OnLvnEndlabeleditBuddygroupsBuddylist(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
  NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

  // cancelled ?
  if (!pDispInfo->item.pszText)
    return 0;

  IRCUser *pUser = (IRCUser *) m_NickListCtrl.GetItemData(pDispInfo->item.iItem);

  // item has a group ?
  if (!pUser)
    return 0;

  if (pUser->m_Nick) free(pUser->m_Nick);

  pUser->m_Nick = strdup(pDispInfo->item.pszText);
  m_NickListCtrl.SetItemText(pDispInfo->item.iItem,0,pUser->m_Nick);
  m_NickListCtrl.SortItems(NickLickSorter,0);
  return 0;
}

LRESULT CBuddyGroupsPage::OnBnClickedBuddygroupsNickdelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  int NickItemNum = m_NickListCtrl.GetSelectedIndex();
  int GroupItemNum = m_GroupCtrl.GetCurSel();

  if (NickItemNum == LB_ERR || GroupItemNum == CB_ERR)
    return 0;

  IRCUserGroup *pGroup = (IRCUserGroup *)m_GroupCtrl.GetItemDataPtr(GroupItemNum);
  IRCUser *pUser = (IRCUser *) m_NickListCtrl.GetItemData(NickItemNum);

  // item has a group ?
  if (!pUser || !pGroup)
    return 0;

  m_NickCtrl.SetWindowText(pUser->m_Nick);
  //m_HostCtrl.SetWindowText(pUser->m_Address);
  pGroup->DeleteUser(pUser);
  m_NickListCtrl.DeleteItem(NickItemNum);

  return 0;
}

LRESULT CBuddyGroupsPage::OnBnClickedBuddygroupsNickadd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  int GroupItemNum = m_GroupCtrl.GetCurSel();
  if (GroupItemNum == CB_ERR)
    return 0;

  IRCUserGroup *pGroup = (IRCUserGroup *)m_GroupCtrl.GetItemDataPtr(GroupItemNum);
  if (!pGroup)
    return 0;

  int Len;
  char *nick = NULL;
  //char *host = NULL;
  char *nickbuffer;
  //char *hostbuffer;
  char *bufferptr;

  // repeat this bit...
  Len = m_NickCtrl.GetWindowTextLength();
  nickbuffer = (char *)malloc(Len + 1);
  if (nickbuffer)
  {
    *nickbuffer = 0; // null terminate, just in case.
    m_NickCtrl.GetWindowText(nickbuffer,Len + 1);
    bufferptr = nickbuffer;
    while (*bufferptr && (*bufferptr == ' ' || *bufferptr == '\t')) 
      bufferptr++;
    nick = stripwhitespace(bufferptr);
  }
  // to here...

  // TODO: ... for the m_HostCtrl,host

  if (*nick) // & *host)
  {
    //IRCUser *pUser = new IRCUser(nick, NULL);
    IRCUser *pUser = new IRCUser(nick, IRCMODE_NORMAL);
    pGroup->AddUser(pUser);
    ResetDialog(GroupItemNum);
  }


  free(nickbuffer);
  //free(hostbuffer);
  return 0;
}
