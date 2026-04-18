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

// PrefsPage.h : interface of the CPrefsPage class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CPrefsPage
{
private:
  char *m_Name;
  char *m_Info;

public:
  void SetDetails(char *Name, char *Info)
  {
    m_Name = Name;
    m_Info = Info;
  }

  char *GetName( void ) { return m_Name; }
  char *GetInfo( void ) { return m_Info; }
  virtual void OnPageDisplay( void ) = 0;
  virtual void OnPageDone( void ) = 0;
  virtual BOOL OnPageValidate( void ) = 0;

};

class CNotImplementedPage : 
  public CDialogImpl<CNotImplementedPage>,
  public CDialogResize<CNotImplementedPage>,
  public CPrefsPage
{
public:
  enum { IDD = IDD_PREFS_NOTIMPLEMENTED };

	BEGIN_MSG_MAP(CNotImplementedPage)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    CHAIN_MSG_MAP(CDialogResize<CNotImplementedPage>)
  END_MSG_MAP()

  BEGIN_DLGRESIZE_MAP(CNotImplementedPage)
    DLGRESIZE_CONTROL(IDC_STATIC_INFO           ,DLSZ_SIZE_X)
  END_DLGRESIZE_MAP()

  LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  {
    DlgResize_Init(false,true,0);
    return 0;
  }

   void OnPageDisplay ( void ) {}
   void OnPageDone ( void ) {}
   BOOL OnPageValidate ( void ) { return TRUE; }
//   LRESULT OnStnClickedStaticInfo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

class CSelectChildPage : 
  public CDialogImpl<CSelectChildPage>,
  public CDialogResize<CSelectChildPage>,
  public CPrefsPage
{
public:
  enum { IDD = IDD_PREFS_SELECTCHILD };

	BEGIN_MSG_MAP(CSelectChildPage)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    CHAIN_MSG_MAP(CDialogResize<CSelectChildPage>)
  END_MSG_MAP()

  BEGIN_DLGRESIZE_MAP(CSelectChildPage)
    DLGRESIZE_CONTROL(IDC_STATIC_INFO           ,DLSZ_SIZE_X)
  END_DLGRESIZE_MAP()

  LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  {
    DlgResize_Init(false,true,0);
    return 0;
  }

   void OnPageDisplay ( void ) {}
   void OnPageDone ( void ) {}
   BOOL OnPageValidate ( void ) { return TRUE; }
//   LRESULT OnStnClickedStaticInfo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
