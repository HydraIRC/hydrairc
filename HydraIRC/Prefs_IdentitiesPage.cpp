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

// Prefs_IdentitiesPage.h : interface of the CIdentitiesPage class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"
#include "prefs_identitiespage.h"

void CIdentitiesPage::OnPageDisplay( void )
{
  int newitemnum;
  m_IdentityCtrl.Attach(GetDlgItem(IDC_IDENTITIES_LIST));

  for (int i = 0 ; i < g_pNewPrefs->m_UserIdentitiesList.GetSize() ; i++ )
  {
    newitemnum = m_IdentityCtrl.AddString(g_pNewPrefs->m_UserIdentitiesList[i]->m_Description);
    if (newitemnum != LB_ERR) 
      m_IdentityCtrl.SetItemDataPtr(newitemnum,g_pNewPrefs->m_UserIdentitiesList[i]); 
    else 
      ATLASSERT(FALSE);    
  }

  ::SetWindowText(GetDlgItem(IDC_IDENTITY_DEFAULT).m_hWnd,g_pNewPrefs->m_StringPrefs[PREF_sDefaultIdentity - PREF_STRING_FIRST]);

  UpdateWindow();
}

void CIdentitiesPage::OnPageDone( void )
{
  while (m_IdentityCtrl.GetCount() > 0)
  {
    m_IdentityCtrl.DeleteString(0);
  }
  m_IdentityCtrl.Detach();
}

BOOL CIdentitiesPage::OnPageValidate( void )
{
  return TRUE;
}

LRESULT CIdentitiesPage::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  // the real init is done when the page is OnPageDisplay'd, as other preference items might
  // make a difference to the data
  DlgResize_Init(false,true,0);
  return 0;
}

LRESULT CIdentitiesPage::OnBnClickedIdentityNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  UserIdentity *pNewIdentity = new UserIdentity();

  pNewIdentity->m_ID = g_pNewPrefs->ObtainNewIdentityID();

  CUserIdentityDlg Dlg(pNewIdentity);

  int result = Dlg.DoModal();

  if (result == 1)
  {
    g_pNewPrefs->m_UserIdentitiesList.Add(pNewIdentity);
    int newitemnum = m_IdentityCtrl.AddString(pNewIdentity->m_Description);
    if (newitemnum != LB_ERR) 
    {
      m_IdentityCtrl.SetCurSel(newitemnum);
      m_IdentityCtrl.SetItemDataPtr(newitemnum,pNewIdentity); 
    }
    else 
      ATLASSERT(FALSE);
  }
  else
  {
    delete pNewIdentity;
  }
  return 0;
}

LRESULT CIdentitiesPage::OnBnClickedIdentityEdit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  int identitynum = m_IdentityCtrl.GetCurSel();
  if (identitynum >= 0)
  {

    UserIdentity *pOldIdentity = (UserIdentity *)m_IdentityCtrl.GetItemDataPtr(identitynum);
    ATLASSERT (pOldIdentity);
    UserIdentity *pNewIdentity = new UserIdentity(pOldIdentity);

    // edit a copy of the old one.
    CUserIdentityDlg Dlg(pNewIdentity);
    int result = Dlg.DoModal();

    if (result == 1)
    {
      g_pNewPrefs->m_UserIdentitiesList.Remove(pOldIdentity);
      g_pNewPrefs->m_UserIdentitiesList.Add(pNewIdentity);

      // we have to update the list too..
      m_IdentityCtrl.DeleteString(identitynum);
      int newitemnum = m_IdentityCtrl.AddString(pNewIdentity->m_Description);
      if (newitemnum != LB_ERR) 
      {
        m_IdentityCtrl.SetCurSel(newitemnum);
        m_IdentityCtrl.SetItemDataPtr(newitemnum,pNewIdentity); 
      }
      else 
        ATLASSERT(FALSE);
      

      delete pOldIdentity;
    }
    else
    {
      delete pNewIdentity;
    }
  }
  return 0;
}

LRESULT CIdentitiesPage::OnBnClickedIdentityDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  int identitynum = m_IdentityCtrl.GetCurSel();
  if (identitynum >= 0)
  {
    UserIdentity *pOldIdentity = (UserIdentity *)m_IdentityCtrl.GetItemDataPtr(identitynum);
    ATLASSERT (pOldIdentity);

    g_pNewPrefs->m_UserIdentitiesList.Remove(pOldIdentity);
    m_IdentityCtrl.DeleteString(identitynum);

    delete pOldIdentity;
  }
  return 0;
}

LRESULT CIdentitiesPage::OnBnClickedIdentitySetdefault(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  int identitynum = m_IdentityCtrl.GetCurSel();
  if (identitynum >= 0)
  {
    UserIdentity *pIdentity = (UserIdentity *)m_IdentityCtrl.GetItemDataPtr(identitynum);
    ATLASSERT (pIdentity);
    SetString(g_pNewPrefs->m_StringPrefs[PREF_sDefaultIdentity - PREF_STRING_FIRST],pIdentity->m_Description);

    ::SetWindowText(GetDlgItem(IDC_IDENTITY_DEFAULT).m_hWnd,pIdentity->m_Description);
  }  
  return 0;
}
