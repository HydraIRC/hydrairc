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
