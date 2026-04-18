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
