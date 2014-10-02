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


// aboutdlg.cpp : implementation of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

LRESULT CAboutDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  CStatic m_AboutInfo;
	CenterWindow(GetParent());
  m_AboutInfo.Attach(GetDlgItem(IDC_ABOUT_VERSIONINFO));
  m_AboutInfo.SetWindowText("HydraIRC " VERSIONSTRING "\n"
                            "\n"
                            "Written by Dominic Clifton\n"
                            "(c) Copyright 2002-2006\n"
                            "\n"
                            "Most icons created by srobot (many thanks!)\n"
                            "Nice images created by digismack\n"
                            "\n"
                            "Special Thanks to Zann, Imajes, 'duckman, hdaalz, fryguy, \n"
							"digismack and the regular #HydraIRC idlers for continued support\n"
                            "\n"
                            "HydraIRC is Open-Source but NOT GPLd, see license for details!");
  m_AboutInfo.Detach();

	return TRUE;
}

LRESULT CAboutDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}
