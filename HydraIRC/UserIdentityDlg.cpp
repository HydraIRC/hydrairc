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

// UserIdentityDlg.cpp : implementation of the CUserIdentityDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"
#include "useridentitydlg.h"


BOOL CUserIdentityDlg::Validate( void )
{
  BOOL Result;

  if (!m_Initialised)
    Result = FALSE;
  else
  {
    CopyWindowText(&m_IdentdOSCtrl,m_pIdentity->m_IdentdOS);
    CopyWindowText(&m_IdentdUserNameCtrl,m_pIdentity->m_IdentdUserName);

    Result = (CopyWindowText(&m_DescriptionCtrl,m_pIdentity->m_Description) &&
              CopyWindowText(&m_RealNameCtrl,m_pIdentity->m_RealName) &&
              CopyWindowText(&m_UserNameCtrl,m_pIdentity->m_UserName) &&
              m_NickListCtrl.GetCount() > 0);
  }

  m_OKCtrl.EnableWindow(Result);

  return Result;
}

LRESULT CUserIdentityDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());

  m_OKCtrl.Attach(GetDlgItem(ID_UI_OK));
  m_DescriptionCtrl.Attach(GetDlgItem(IDC_UI_DESCRIPTION));
  m_RealNameCtrl.Attach(GetDlgItem(IDC_UI_REALNAME));
  m_UserNameCtrl.Attach(GetDlgItem(IDC_UI_USERNAME));
  m_NickCtrl.Attach(GetDlgItem(IDC_UI_NICK));
  m_NickListCtrl.Attach(GetDlgItem(IDC_UI_NICKLIST));
  m_IdentdOSCtrl.Attach(GetDlgItem(IDC_UI_IDENTDOS));
  m_IdentdUserNameCtrl.Attach(GetDlgItem(IDC_UI_IDENTDUSERNAME));

  if (m_pIdentity->m_Description) m_DescriptionCtrl.SetWindowText(m_pIdentity->m_Description);
  if (m_pIdentity->m_RealName) m_RealNameCtrl.SetWindowText(m_pIdentity->m_RealName);
  if (m_pIdentity->m_UserName) m_UserNameCtrl.SetWindowText(m_pIdentity->m_UserName);
  if (m_pIdentity->m_IdentdOS) m_IdentdOSCtrl.SetWindowText(m_pIdentity->m_IdentdOS);
  if (m_pIdentity->m_IdentdUserName) m_IdentdUserNameCtrl.SetWindowText(m_pIdentity->m_IdentdUserName);

  for (int i = 0 ; i < m_pIdentity->m_NickList.GetSize() ; i ++)
  {
    m_NickListCtrl.AddString(m_pIdentity->m_NickList[i]);
  }

  m_Initialised = TRUE;
  Validate();
	return TRUE;
}

LRESULT CUserIdentityDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

void CUserIdentityDlg::OKClicked( void )
{
  if (Validate())
  {
    // copy the strings from nicklistctrl to the identity.
    while (m_pIdentity->m_NickList.GetSize() > 0)
    {
      free(m_pIdentity->m_NickList[0]);
      m_pIdentity->m_NickList.RemoveAt(0);
    }

    int len;
    char *tmpstr;
    while(m_NickListCtrl.GetCount() > 0)
    {
      len = m_NickListCtrl.GetTextLen(0);
      ATLASSERT(len);
      tmpstr = (char *)malloc(len+1);
      if(tmpstr)
      {
        m_NickListCtrl.GetText(0,tmpstr);
        m_pIdentity->m_NickList.Add(tmpstr);
      }
      m_NickListCtrl.DeleteString(0);
    }
    EndDialog(1);    
  }
}

LRESULT CUserIdentityDlg::OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  OKClicked();  
	return 0;
}

void CUserIdentityDlg::AddClicked( void )
{
  if (CopyWindowText(&m_NickCtrl,m_TmpNick))
  {
    stripwhitespace(m_TmpNick);
    char *tmp = m_TmpNick;
    while (*tmp == ' ') tmp++; // strip leading space.
    if (*tmp)
    {
      int itemnum = m_NickListCtrl.AddString(tmp);
      if (itemnum != LB_ERR)
        m_NickListCtrl.SetCurSel(itemnum);
    }
  }
  m_NickCtrl.SetFocus();
  m_NickCtrl.SetSel(0,-1);
  Validate();
}

LRESULT CUserIdentityDlg::OnBnClickedIdentitiesAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  AddClicked();
  return 0;
}

LRESULT CUserIdentityDlg::OnEnSetfocusUiNick(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  // TODO: both of these?
  SendDlgItemMessage(IDC_UI_ADD,BM_SETSTYLE,BS_DEFPUSHBUTTON,MAKELPARAM(TRUE,0));
  SendDlgItemMessage(ID_UI_OK  ,BM_SETSTYLE,BS_PUSHBUTTON   ,MAKELPARAM(TRUE,0));
  return 0;
}

LRESULT CUserIdentityDlg::OnEnChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  Validate();
  return 0;
}

LRESULT CUserIdentityDlg::OnBnClickedUiDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  int num = m_NickListCtrl.GetCurSel();
  if (num >= 0)
  {
    int itemcount = m_NickListCtrl.DeleteString(num);
    num = min(num,itemcount-1);
    if (itemcount)
      m_NickListCtrl.SetCurSel(num);
  }
  return 0;
}

// bleh, there must be a better way to swap two items..

LRESULT CUserIdentityDlg::OnBnClickedUiUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  int len,newnum;
  char *tmpstr;
  int num = m_NickListCtrl.GetCurSel();

  if (num >= 1) // not 0...
  {
    len = m_NickListCtrl.GetTextLen(num);
    ATLASSERT(len);
    tmpstr = (char *)malloc(len+1);
    if(tmpstr)
    {
      m_NickListCtrl.GetText(num,tmpstr);
      newnum = m_NickListCtrl.InsertString(num-1,tmpstr);
      m_NickListCtrl.DeleteString(num+1);
      m_NickListCtrl.SetCurSel(newnum);
      free(tmpstr);
    }
  }  
  return 0;
}

// bleh, there must be a better way to swap two items..
LRESULT CUserIdentityDlg::OnBnClickedUiDown(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  int len,newnum;
  char *tmpstr;
  int num = m_NickListCtrl.GetCurSel();

  if (num >= 0 && num < m_NickListCtrl.GetCount() - 1) // not < GetCount - 0
  {
    len = m_NickListCtrl.GetTextLen(num);
    ATLASSERT(len);
    tmpstr = (char *)malloc(len+1);
    if(tmpstr)
    {
      m_NickListCtrl.GetText(num,tmpstr);
      m_NickListCtrl.DeleteString(num);
      newnum = m_NickListCtrl.InsertString(num+1,tmpstr);            
      m_NickListCtrl.SetCurSel(newnum);
      free(tmpstr);
    }
  }  
  return 0;
}

LRESULT CUserIdentityDlg::OnBnClickedDefault(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& bHandled)
{
  if (::GetFocus() == m_NickCtrl.m_hWnd)
  {
    AddClicked();
  }
  else
  {
    OKClicked();
  }
  bHandled = TRUE;
  return 0;
}
