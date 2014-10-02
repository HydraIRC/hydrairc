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

// 
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"
#include ".\channellistdlg.h"

int CALLBACK LVSorter_ChannelProperties(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
  CChannelProperties *pCP1 = (CChannelProperties *)lParam1;
  CChannelProperties *pCP2 = (CChannelProperties *)lParam2;

  switch(lParamSort)
  {
    case CLSM_NAME:
      return stricmp(pCP1->m_Name,pCP2->m_Name);
    case CLSM_NAMEREVERSE:
      return stricmp(pCP2->m_Name,pCP1->m_Name);
    case CLSM_USERS:
      return (pCP1->m_Users > pCP2->m_Users ? 0 : 1);
    case CLSM_USERSREVERSE:
      return (pCP2->m_Users > pCP1->m_Users ? 0 : 1);
    case CLSM_TOPIC:
      return stricmp(pCP1->m_Topic,pCP2->m_Topic);
    case CLSM_TOPICREVERSE:
      return stricmp(pCP2->m_Topic,pCP1->m_Topic);
  }
  return 0; // should not get here.
}

CChannelListDlg::~CChannelListDlg()
{
  DeleteChannelList();
}

void CChannelListDlg::DeleteChannelList( void )
{
  while (m_ChannelList.GetSize() > 0)
  {
    delete m_ChannelList[0];
    m_ChannelList.RemoveAt(0);
  }
}

LRESULT CChannelListDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  CenterWindow(GetParent());
  DlgResize_Init();

  m_SearchTextCtrl.Attach(GetDlgItem(ID_CHANNELLIST_SEARCHTEXT));
  m_UsersMinCtrl.Attach(GetDlgItem(ID_CHANNELLIST_USERSMIN));
  m_UsersMaxCtrl.Attach(GetDlgItem(ID_CHANNELLIST_USERSMAX));

  m_SearchSelectCtrl.Attach(GetDlgItem(ID_CHANNELLIST_SEARCHSELECT));
  m_SearchSelectCtrl.AddString("Name");
  m_SearchSelectCtrl.AddString("Topic");
  m_SearchSelectCtrl.AddString("Name and Topic");
  m_SearchSelectCtrl.SetCurSel(0);

  m_JoinCtrl.Attach(GetDlgItem(ID_CHANNELLIST_JOIN));
  m_PopulateCtrl.Attach(GetDlgItem(ID_CHANNELLIST_POPULATE));
  
  m_ShowAllCtrl.Attach(GetDlgItem(ID_CHANNELLIST_SHOWALL));
  m_ShowAllCtrl.SetCheck(BST_CHECKED);

  m_ShowFavoritesCtrl.Attach(GetDlgItem(ID_CHANNELLIST_SHOWFAVORITES));

  m_ProgressCtrl.Attach(GetDlgItem(ID_CHANNELLIST_PROGRESS));
  m_ProgressCtrl.SetRange(0,100);

  m_ChannelListCtrl.Attach(GetDlgItem(ID_CHANNELLIST_LIST));
  m_ChannelListCtrl.AddColumn("Name",0);
  m_ChannelListCtrl.AddColumn("Users",1);
  m_ChannelListCtrl.AddColumn("Topic",2);

  m_ChannelListCtrl.SetColumnWidth(0,MAKELPARAM((int)LVSCW_AUTOSIZE_USEHEADER,0));
  m_ChannelListCtrl.SetColumnWidth(1,MAKELPARAM((int)LVSCW_AUTOSIZE_USEHEADER,0));
  m_ChannelListCtrl.SetColumnWidth(2,MAKELPARAM((int)LVSCW_AUTOSIZE_USEHEADER,0));

  m_Initialised = TRUE;
	return TRUE;
}

LRESULT CChannelListDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CChannelListDlg::OnJoinCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  int i;
  UINT ItemState;
  CChannelProperties *pCP;

  for (i = 0 ; i < m_ChannelListCtrl.GetItemCount(); i++)
  {
    ItemState = m_ChannelListCtrl.GetItemState(i, LVIS_SELECTED);

    if ((ItemState & LVIS_SELECTED) == LVIS_SELECTED)
    {
      pCP = (CChannelProperties *)m_ChannelListCtrl.GetItemData(i);
      if (pCP)
      {
        if (m_pServer->IsLoggedIn())
          m_pServer->Send("JOIN %s %s\r\n",pCP->m_Name, UseEmptyString(pCP->m_Key));
      }
    }
  }
	return 0;
}

LRESULT CChannelListDlg::OnSomethingChanged(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  // Isn't this the name of a song by Pulp ?  :)
  return 0;
}

LRESULT CChannelListDlg::OnBnClickedChannellistPopulate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  if (m_pServer->IsLoggedIn())
    m_pServer->Send("LIST\r\n");
  // sit back and wait!
  return 0;
}

void CChannelListDlg::AddChannel( char *ChannelName, int Users, char *Topic)
{
  if (!ChannelName || !*ChannelName)
    return;

  // if the user opened the channel list window after doing /list
  // OnListStart() would not have been called, so we use and check a flag.
  if (!m_OnListStartCalled)
  {
    OnListStart();
  }

  CChannelProperties *pCP = new CChannelProperties(ChannelName, Users, Topic);
  if (pCP)
  {
    m_ChannelList.Add(pCP);
    int itemnum = m_ChannelListCtrl.InsertItem(0,pCP->m_Name);
    if (itemnum != -1)
    {
      m_ChannelListCtrl.SetItemData(itemnum,(DWORD_PTR)pCP);
      char numstr[10];
      _snprintf(numstr,sizeof(numstr)-1,"%d",pCP->m_Users);
      m_ChannelListCtrl.AddItem(itemnum,1,numstr);

      if (pCP->m_Topic)
      {
        char *topicstr = stripcodes(strdup(pCP->m_Topic));
        if (topicstr)
        {
          m_ChannelListCtrl.AddItem(itemnum,2,topicstr);
          free(topicstr);
        }
      }
      
    }
  }
}

void CChannelListDlg::EnableControls( BOOL Enable )
{
  m_PopulateCtrl.EnableWindow(Enable);
}

void CChannelListDlg::OnListStart( void )
{
  m_OnListStartCalled = TRUE;
  EnableControls( FALSE );
  DeleteChannelList();
  m_ChannelListCtrl.DeleteAllItems();
  m_ProgressCtrl.SetPos(0);
}

void CChannelListDlg::OnListEnd( void )
{
  m_ChannelListCtrl.SetColumnWidth(0,MAKELPARAM((int)LVSCW_AUTOSIZE,0));
  m_ChannelListCtrl.SetColumnWidth(1,MAKELPARAM((int)LVSCW_AUTOSIZE_USEHEADER,0));
  m_ChannelListCtrl.SetColumnWidth(2,MAKELPARAM((int)LVSCW_AUTOSIZE,0));
  m_ChannelListCtrl.SortItems(LVSorter_ChannelProperties, m_CurrentSortMode);

  m_ProgressCtrl.SetPos(100);
  EnableControls( TRUE );
  m_OnListStartCalled = FALSE; // unset out flag.
}


LRESULT CChannelListDlg::OnNMDblclkChannellistList(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
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
    CChannelProperties *pCP = (CChannelProperties *)m_ChannelListCtrl.GetItemData(ht.iItem);
    if (pCP)
    {
      if (m_pServer->IsLoggedIn())
        m_pServer->Send("JOIN %s %s\r\n",pCP->m_Name, UseEmptyString(pCP->m_Key));
    }
    
  }
  return 0;
}

LRESULT CChannelListDlg::OnLvnColumnclickChannellistList(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  // TODO: Add your control notification handler code here

  switch (pNMLV->iSubItem)
  {
    case 0: // name
      if (m_CurrentSortMode == CLSM_NAME)
        m_CurrentSortMode = CLSM_NAMEREVERSE;
      else
        m_CurrentSortMode = CLSM_NAME;
      break;
    case 1: // users
      if (m_CurrentSortMode == CLSM_USERS)
        m_CurrentSortMode = CLSM_USERSREVERSE;
      else
        m_CurrentSortMode = CLSM_USERS;
      break;
    case 2: // topic
      if (m_CurrentSortMode == CLSM_TOPIC)
        m_CurrentSortMode = CLSM_TOPICREVERSE;
      else
        m_CurrentSortMode = CLSM_TOPIC;
      break;
  };

  m_ChannelListCtrl.SortItems(LVSorter_ChannelProperties, m_CurrentSortMode);

  return 0;
}
