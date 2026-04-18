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

// AddNetworkDlg.cpp : implementation of the CAddNetworkDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

char *g_UseDefaultIdentityText = "<< Use Default >>";

BOOL CAddNetworkDlg::Validate( void )
{
  BOOL Result = FALSE;

  if (m_Initialised)
  {
    CopyWindowText(&m_NameCtrl,m_pND->m_Name);   

    int itemnum = m_IdentityCtrl.GetCurSel();     
    UserIdentity *pIdentity = (UserIdentity *) m_IdentityCtrl.GetItemData(itemnum);
    if (pIdentity)
      SetString(m_pND->m_IdentityName, pIdentity->m_Description);
    else
      SetString(m_pND->m_IdentityName, NULL);

    stripwhitespace(m_pND->m_Name);

    if (m_pND->m_Name && *m_pND->m_Name) // still got something left?
      Result = TRUE;
  }
  m_OKCtrl.EnableWindow(Result);

  return Result;
}

LRESULT CAddNetworkDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  CenterWindow(GetParent());
  DlgResize_Init();

  m_NameCtrl.Attach(GetDlgItem(IDC_ANET_NAME));
  m_IdentityCtrl.Attach(GetDlgItem(IDC_ANET_IDENTITYLIST));
  m_OKCtrl.Attach(GetDlgItem(ID_ANET_OK));
  
  // can't edit the names of networks that come from the master list.
  if (m_pND->m_Flags & INDF_MASTERLIST)
    m_NameCtrl.EnableWindow(FALSE);

  if (m_pND->m_Name) m_NameCtrl.SetWindowText(m_pND->m_Name);

  UserIdentity *pIdentity = NULL;
  if (m_pND->m_IdentityName)
  {
    pIdentity = g_pPrefs->FindIdentityByDescripton(m_pND->m_IdentityName);    
  }
  PopulateIdentityComboCtrl(m_IdentityCtrl, pIdentity);

  int index = m_IdentityCtrl.AddString(g_UseDefaultIdentityText);
  // if no preferred identity defined, select the "<< Use Default >>" item
  if (!m_pND->m_IdentityName && index != CB_ERR)
    m_IdentityCtrl.SetCurSel(index);

  m_Initialised = TRUE;
  Validate();
	return TRUE;
}

LRESULT CAddNetworkDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CAddNetworkDlg::OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  if (Validate())
  {
    EndDialog(1);    
  }
	return 0;
}

LRESULT CAddNetworkDlg::OnSomethingChanged(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  // Isn't this the name of a song by Pulp ?  :)
  Validate();
  return 0;
}
