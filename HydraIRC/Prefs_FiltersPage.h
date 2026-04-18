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
