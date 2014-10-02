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
