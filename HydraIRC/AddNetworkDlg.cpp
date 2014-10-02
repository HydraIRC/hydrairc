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
