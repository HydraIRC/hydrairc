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

// Prefs_IdentitiesPage.h : interface of the CFiltersPage class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
class CFiltersPage : 
  public CDialogImpl<CFiltersPage>,
  public CDialogResize<CFiltersPage>,
  public CPrefsPage
{
private:
  CListViewCtrl m_UnusedListCtrl;
  CListViewCtrl m_UsedListCtrl;
  CComboBox m_ObjectCtrl;
  CButton m_IncludeCtrl;
  //CButton m_ExcludeCtrl; // only need one of these really.. ;)

public:
  enum { IDD = IDD_PREFS_FILTERS };

	BEGIN_MSG_MAP(CFiltersPage)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_HANDLER(IDC_FILTERS_OBJECT,     CBN_SELCHANGE,    OnCbnSelchangeObject)
    COMMAND_HANDLER(IDC_FILTERS_ADD,        BN_CLICKED,       OnBnClickedAdd)
    COMMAND_HANDLER(IDC_FILTERS_REMOVE,     BN_CLICKED,       OnBnClickedRemove)
    CHAIN_MSG_MAP(CDialogResize<CFiltersPage>)
    REFLECT_NOTIFICATIONS()
  END_MSG_MAP()

  BEGIN_DLGRESIZE_MAP(CFiltersPage)
    DLGRESIZE_CONTROL(IDC_FILTERS_OBJECT     ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_FILTERS_UNUSED     ,DLSZ_SIZE_Y)
    DLGRESIZE_CONTROL(IDC_FILTERS_USED       ,DLSZ_MOVE_X | DLSZ_SIZE_Y)
    DLGRESIZE_CONTROL(IDC_FILTERS_UNUSEDTEXT ,DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_FILTERS_USEDTEXT   ,DLSZ_MOVE_X | DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_FILTERS_ADD        ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_FILTERS_REMOVE     ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_FILTERS_INCLUDE    ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_FILTERS_EXCLUDE    ,DLSZ_SIZE_X)
  END_DLGRESIZE_MAP()

  // page common stuff
  void OnPageDisplay ( void );
  void OnPageDone ( void );
  BOOL OnPageValidate ( void );
  
  // page specific stuff
  void ClearLists( void );
  void PopulateLists( int Selection );

  LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);  
  //LRESULT OnPinBrowse(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/);
  LRESULT OnCbnSelchangeObject(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedRemove(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
