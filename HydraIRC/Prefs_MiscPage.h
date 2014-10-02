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

// Prefs_IdentitiesPage.h : interface of the CMiscPage class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
class CMiscPage : 
  public CDialogImpl<CMiscPage>,
  public CDialogResize<CMiscPage>,
  public CPrefsPage
{
private:
  CPropertyListCtrl m_PropertyList;
public:
  enum { IDD = IDD_PREFS_MISC };

	BEGIN_MSG_MAP(CMiscPage)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    NOTIFY_HANDLER(IDC_MISC_PROPERTYLIST, PIN_BROWSE, OnPinBrowse)
    CHAIN_MSG_MAP(CDialogResize<CMiscPage>)
    REFLECT_NOTIFICATIONS()
  END_MSG_MAP()

  BEGIN_DLGRESIZE_MAP(CMiscPage)
    DLGRESIZE_CONTROL(IDC_MISC_PROPERTYLIST     ,DLSZ_SIZE_X | DLSZ_SIZE_Y)
  END_DLGRESIZE_MAP()

  // page common stuff
  void OnPageDisplay ( void );
  void OnPageDone ( void );
  BOOL OnPageValidate ( void );
  
  // page specific stuff

  LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);  
  LRESULT CMiscPage::OnPinBrowse(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/);
};
