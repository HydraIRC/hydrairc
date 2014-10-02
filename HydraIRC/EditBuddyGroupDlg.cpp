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

// EditBuddyGroupDlg.cpp : implementation of the CEditBuddyGroupDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

BOOL CEditBuddyGroupDlg::Validate( void )
{
  BOOL Result = FALSE;
  int itemnum;

  if (m_Initialised)
  {
    CopyWindowText(&m_NameCtrl,m_Name);
    itemnum = m_NetworkCtrl.GetCurSel();

    if (itemnum != CB_ERR)
      m_NetworkID = m_NetworkCtrl.GetItemData(itemnum);
    else
      m_NetworkID = NETWORK_ALL;

    stripwhitespace(m_Name);
    if (m_Name && *m_Name && *m_Name != ' ' && *m_Name != '\t' && 
        m_NetworkID >= NETWORK_ALL)
      Result = TRUE;
  }
  m_OKCtrl.EnableWindow(Result);

  return Result;
}

LRESULT CEditBuddyGroupDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  CenterWindow(GetParent());
  DlgResize_Init();

  m_NameCtrl.Attach(GetDlgItem(IDC_EBG_NAME));
  m_NetworkCtrl.Attach(GetDlgItem(IDC_EBG_NETWORK));
  m_OKCtrl.Attach(GetDlgItem(ID_EBG_OK));

  m_NetworkCtrl.AddString("All Networks");
  m_NetworkCtrl.SetItemData(0,-1);

  int i,itemnum;
  for (i = 0 ; i < g_NetworkList.GetSize() ; i ++)
  {
    itemnum = m_NetworkCtrl.AddString(g_NetworkList[i]->m_Name);
    if (itemnum != CB_ERR)
      m_NetworkCtrl.SetItemData(itemnum,g_NetworkList[i]->m_ID);
  }

  // Set the network control correctly..
  for (int i = 0 ; i < m_NetworkCtrl.GetCount() ; i ++)
  {
    // FIXME: hmm, GetItemData can return -1, but -1 is also valid data...
    // not a major problem though, so I'll leave it for now.
    if (m_NetworkCtrl.GetItemData(i) == m_NetworkID)
    {
      m_NetworkCtrl.SetCurSel(i);
      break;
    }
  }

  if (m_Name)
    m_NameCtrl.SetWindowText(m_Name);

  m_Initialised = TRUE;
  Validate();
	return TRUE;
}

LRESULT CEditBuddyGroupDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CEditBuddyGroupDlg::OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  if (Validate())
  {
    EndDialog(1);    
  }
	return 0;
}

LRESULT CEditBuddyGroupDlg::OnSomethingChanged(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  // Isn't this the name of a song by Pulp ?  :)
  Validate();
  return 0;
}
