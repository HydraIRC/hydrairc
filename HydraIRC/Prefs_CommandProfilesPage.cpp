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

// Prefs_CommandProfilesPage.h : interface of the CCommandProfilesPage class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"
#include "prefs_commandprofilespage.h"

void CCommandProfilesPage::OnPageDisplay( void )
{
  m_ProfilesListCtrl.Attach(GetDlgItem(IDC_COMMANDPROFILES_PROFILES));
  m_CommandsCtrl.Attach(GetDlgItem(IDC_COMMANDPROFILES_COMMANDS));
  m_ProfileNameCtrl.Attach(GetDlgItem(IDC_COMMANDPROFILES_NAME));

  m_AddCtrl.Attach(GetDlgItem(IDC_COMMANDPROFILES_ADD));
  m_DeleteCtrl.Attach(GetDlgItem(IDC_COMMANDPROFILES_DELETE));
  m_CopyCtrl.Attach(GetDlgItem(IDC_COMMANDPROFILES_COPY));
  m_RenameCtrl.Attach(GetDlgItem(IDC_COMMANDPROFILES_RENAME));
  
  m_CommandsCtrl.SetLimitText(MAX_COMMAND_PROFILE_LEN - 1);
  m_ProfileNameCtrl.SetLimitText(MAX_COMMAND_PROFILE_NAME_LEN - 1);

  ResetNameCtrl();

  int i,itemnum;
  CCommandProfile *pCommandProfile;
  for (i = 0; i < g_pNewPrefs->m_CommandProfileList.GetSize(); i++)
  {
    pCommandProfile = g_pNewPrefs->m_CommandProfileList[i];
    itemnum = m_ProfilesListCtrl.AddString(pCommandProfile->m_Name);
    if (itemnum != LB_ERR)
      m_ProfilesListCtrl.SetItemDataPtr(itemnum,pCommandProfile);
  }
  m_LastItemNum = LB_ERR;
}

void CCommandProfilesPage::OnPageDone( void )
{
  ClearLists();
  m_ProfilesListCtrl.Detach();
  m_CommandsCtrl.Detach();
  m_ProfileNameCtrl.Detach();
  m_AddCtrl.Detach();
  m_DeleteCtrl.Detach();
  m_CopyCtrl.Detach();
  m_RenameCtrl.Detach();

}

BOOL CCommandProfilesPage::OnPageValidate( void )
{
  SaveCurrentCommandProfile();

  // don't need any validation.  user can't break anything!
  BOOL OK = TRUE;
  return OK;
}

void CCommandProfilesPage::ClearLists( void )
{
  while (m_ProfilesListCtrl.GetCount() > 0)
  {
    m_ProfilesListCtrl.ResetContent();
  }
}

void CCommandProfilesPage::ResetNameCtrl( void )
{
  m_ProfileNameCtrl.SetWindowText("<< Enter Name Here >>");
  m_ProfileNameCtrl.SetSel(0,-1); 

  m_CommandsCtrl.SetWindowText("");
  m_CommandsCtrl.EnableWindow(FALSE);

}

LRESULT CCommandProfilesPage::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  // the real init is done when the page is OnPageDisplay'd, as other preference items might
  // make a difference to the data

  DlgResize_Init(false,true,0);

  return 0;
}

void CCommandProfilesPage::SaveCurrentCommandProfile( void )
{
  int itemnum = m_ProfilesListCtrl.GetCurSel();

  if (itemnum == LB_ERR || m_LastItemNum == LB_ERR)
    return;

  //if (itemnum == m_LastItemNum)
  //  return;

  CCommandProfile *pCommandProfile = (CCommandProfile *)m_ProfilesListCtrl.GetItemDataPtr(m_LastItemNum);
  if (!pCommandProfile)
    return;

  CopyWindowText(&m_CommandsCtrl,pCommandProfile->m_Commands);  
  m_CommandsCtrl.EnableWindow(FALSE);
}

LRESULT CCommandProfilesPage::OnBnClickedAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  char *Name = NULL;
  if (!CopyWindowText(&m_ProfileNameCtrl,Name))
    return 0; // we need a name for it!

  // don't create a new one with the same name
  if (g_pNewPrefs->FindCommandProfile(Name))
  {
    free(Name);
    return 0;
  }

  CCommandProfile *pCommandProfile = new CCommandProfile();
  if (!pCommandProfile)
  {
    free(Name);
    return 0;
  }

  pCommandProfile->m_Name = Name;

  SaveCurrentCommandProfile();
  AddItem(pCommandProfile);

  return 0;
}

void CCommandProfilesPage::AddItem(CCommandProfile *pCommandProfile)
{
  if (!pCommandProfile)
    return;

  int itemnum = m_ProfilesListCtrl.AddString(pCommandProfile->m_Name);
  if (itemnum != LB_ERR)
  {
    m_ProfilesListCtrl.SetItemDataPtr(itemnum,pCommandProfile);
    m_ProfilesListCtrl.SetCurSel(itemnum);
    g_pNewPrefs->m_CommandProfileList.Add(pCommandProfile);
    ShowItem(itemnum);
  }
  else
    delete pCommandProfile; // this free()'s "Name"

  return;
}

LRESULT CCommandProfilesPage::OnBnClickedDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  int itemnum = m_ProfilesListCtrl.GetCurSel();
  if (itemnum == LB_ERR)
    return 0;

  CCommandProfile *pCommandProfile = (CCommandProfile *)m_ProfilesListCtrl.GetItemDataPtr(itemnum);
  if (!pCommandProfile)
    return 0;

  g_pNewPrefs->m_CommandProfileList.Remove(pCommandProfile);
  m_ProfilesListCtrl.DeleteString(itemnum);
  delete pCommandProfile;

  itemnum--;
  if (itemnum < 0) 
    itemnum = 0;
  int count = m_ProfilesListCtrl.GetCount();
  if (itemnum >= count)
    itemnum = count-1; // index starts at 0

  m_ProfilesListCtrl.SetCurSel(itemnum);
  ShowItem(itemnum);


  return 0;
}

LRESULT CCommandProfilesPage::OnBnClickedCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{  
  int itemnum = m_ProfilesListCtrl.GetCurSel();
  if (itemnum == LB_ERR)
    return 0;

  CCommandProfile *pCommandProfile = (CCommandProfile *)m_ProfilesListCtrl.GetItemDataPtr(itemnum);
  if (!pCommandProfile)
    return 0;

  CCommandProfile *pNewCommandProfile = new CCommandProfile();

  if (pNewCommandProfile)
  {
    pNewCommandProfile->m_Name = HydraIRC_BuildString(strlen(pCommandProfile->m_Name) + 1 + 8,"Copy Of %s",pCommandProfile->m_Name);
    pNewCommandProfile->m_Commands = strdup(pCommandProfile->m_Commands);
  }
  
  // did it get created correctly?
  if (!pNewCommandProfile || !pNewCommandProfile->m_Name || !pNewCommandProfile->m_Commands)
  {
    if (pNewCommandProfile) delete pNewCommandProfile;
    return 0;
  }

  // don't create a new one with the same name
  if (g_pNewPrefs->FindCommandProfile(pNewCommandProfile->m_Name))
  {
    delete pNewCommandProfile;
    return 0;
  }

  SaveCurrentCommandProfile();
  // yup, so make it the current one and add it to the list..
  AddItem(pNewCommandProfile);

  return 0;
}

LRESULT CCommandProfilesPage::OnBnClickedRename(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  int itemnum = m_ProfilesListCtrl.GetCurSel();
  if (itemnum == LB_ERR)
    return 0;

  CCommandProfile *pCommandProfile = (CCommandProfile *)m_ProfilesListCtrl.GetItemDataPtr(itemnum);
  if (!pCommandProfile)
    return 0;

  char *Name = NULL;
  if (!CopyWindowText(&m_ProfileNameCtrl,Name))
    return 0; // we need a name for it!


  // check new name is not the same as the old name
  if (stricmp(pCommandProfile->m_Name, Name) == 0) {
    MessageBox(_T("Name not changed - change the name then click 'Rename'"),_T("Error"), MB_ICONEXCLAMATION);
    free(Name);
    return 0;
  }

  // can't rename to the same name as one that already exists
  if (g_pNewPrefs->FindCommandProfile(Name))
  {
    free(Name);
    MessageBox(_T("A command profile with that new already exists"),_T("Error"), MB_ICONEXCLAMATION);  
    return 0;
  }

  SaveCurrentCommandProfile();

  if (pCommandProfile->m_Name) free(pCommandProfile->m_Name);

  pCommandProfile->m_Name = Name;

  m_ProfilesListCtrl.DeleteString(itemnum);
  g_pNewPrefs->m_CommandProfileList.Remove(pCommandProfile);

  AddItem(pCommandProfile);

  return 0;
}

void CCommandProfilesPage::ShowItem(int ItemNum)
{
  m_LastItemNum = ItemNum;

  if (ItemNum == LB_ERR)
  {
    ResetNameCtrl();
  }
  else
  {
    CCommandProfile *pCommandProfile = (CCommandProfile *)m_ProfilesListCtrl.GetItemDataPtr(ItemNum);
    if (!pCommandProfile)
      return;

    m_ProfileNameCtrl.SetWindowText(pCommandProfile->m_Name);

    m_CommandsCtrl.EnableWindow(TRUE);
    if (pCommandProfile->m_Commands)
    {
      m_CommandsCtrl.SetWindowText(pCommandProfile->m_Commands);
      m_CommandsCtrl.SetSel(0,0); // set sel at start
    }
    else
    {
      m_CommandsCtrl.SetWindowText("<< Enter commands here!>>");
      m_CommandsCtrl.SetSel(0,-1); // select all of it
    }

    m_CommandsCtrl.SetFocus();
  }
}

LRESULT CCommandProfilesPage::OnLbnSelchangeCommandprofilesProfiles(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  SaveCurrentCommandProfile();
  int itemnum = m_ProfilesListCtrl.GetCurSel();
  if (itemnum == LB_ERR)
    return 0;

  ShowItem(itemnum);
  return 0;
}

LRESULT CCommandProfilesPage::OnEnSetfocusCommandprofilesName(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  // TODO: hmm, doesn't work right...
  /*

  SendDlgItemMessage(IDC_COMMANDPROFILES_ADD,BM_SETSTYLE,BS_DEFPUSHBUTTON,MAKELPARAM(TRUE,0));

  // TODO: this should be done on the parent window!
  SendDlgItemMessage(IDOK  ,BM_SETSTYLE,BS_PUSHBUTTON   ,MAKELPARAM(TRUE,0));
  */
  return 0;
}
