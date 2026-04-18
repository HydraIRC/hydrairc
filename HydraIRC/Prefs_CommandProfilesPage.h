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

// Prefs_CommandProfilesPage.h : interface of the CCommandProfilesPage class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class  CCommandProfilesPage : 
  public CDialogImpl<CCommandProfilesPage>,
  public CDialogResize<CCommandProfilesPage>,
  public CPrefsPage
{
private:
  CListBox m_ProfilesListCtrl;
  CEdit m_CommandsCtrl;
  CEdit m_ProfileNameCtrl;
  CButton m_AddCtrl;
  CButton m_DeleteCtrl;
  CButton m_CopyCtrl;
  CButton m_RenameCtrl;

  int m_LastItemNum;

public:
  enum { IDD = IDD_PREFS_COMMANDPROFILES };  

	BEGIN_MSG_MAP(CCommandProfilesPage)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_HANDLER(IDC_COMMANDPROFILES_ADD,        BN_CLICKED,       OnBnClickedAdd)
    COMMAND_HANDLER(IDC_COMMANDPROFILES_DELETE,     BN_CLICKED,       OnBnClickedDelete)
    COMMAND_HANDLER(IDC_COMMANDPROFILES_COPY,       BN_CLICKED,       OnBnClickedCopy)
    COMMAND_HANDLER(IDC_COMMANDPROFILES_RENAME,     BN_CLICKED,       OnBnClickedRename)
    COMMAND_HANDLER(IDC_COMMANDPROFILES_PROFILES, LBN_SELCHANGE, OnLbnSelchangeCommandprofilesProfiles)
    COMMAND_HANDLER(IDC_COMMANDPROFILES_NAME, EN_SETFOCUS, OnEnSetfocusCommandprofilesName)
    CHAIN_MSG_MAP(CDialogResize<CCommandProfilesPage>)
    REFLECT_NOTIFICATIONS()
  END_MSG_MAP()

  BEGIN_DLGRESIZE_MAP(CCommandProfilesPage)
    DLGRESIZE_CONTROL(IDC_COMMANDPROFILES_COMMANDS   ,DLSZ_SIZE_X | DLSZ_SIZE_Y)
    DLGRESIZE_CONTROL(IDC_COMMANDPROFILES_PROFILES   ,DLSZ_SIZE_Y)
  END_DLGRESIZE_MAP()

  // page common stuff
  void OnPageDisplay ( void );
  void OnPageDone ( void );
  BOOL OnPageValidate ( void );
  
  // page specific stuff
  void ResetNameCtrl( void );
  void SaveCurrentCommandProfile( void );
  void ShowItem(int ItemNum);
  void AddItem(CCommandProfile *pCommandProfile);
  void ClearLists( void );

  LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);  
  //LRESULT OnPinBrowse(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/);
  LRESULT OnCbnSelchangeObject(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedRename(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnLbnSelchangeCommandprofilesProfiles(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnEnSetfocusCommandprofilesName(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
