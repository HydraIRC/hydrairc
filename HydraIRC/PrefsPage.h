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
