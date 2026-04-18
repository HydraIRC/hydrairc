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

// Prefs_IdentitiesPage.h : interface of the CNotificationsPage class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
class CNotificationsPage : 
  public CDialogImpl<CNotificationsPage>,
  public CDialogResize<CNotificationsPage>,
  public CPrefsPage
{
private:
  CListViewCtrl m_EventsCtrl;
  CComboBox m_GroupCtrl;
public:
  enum { IDD = IDD_PREFS_NOTIFICATIONS };

	BEGIN_MSG_MAP(CNotificationsPage)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_HANDLER(IDC_NOTIFICATIONS_GROUP, CBN_SELCHANGE, OnCbnSelchangeNotificationsGroup)
    NOTIFY_HANDLER(IDC_NOTIFICATIONS_EVENTS, NM_RCLICK, OnNMRclickNotificationsEvents)
    NOTIFY_HANDLER(IDC_NOTIFICATIONS_EVENTS, NM_DBLCLK, OnNMDblclkNotificationsEvents)
    COMMAND_HANDLER(IDC_NOTIFICATIONS_NEW, BN_CLICKED, OnBnClickedNotificationsNew)
    COMMAND_HANDLER(IDC_NOTIFICATIONS_DELETE, BN_CLICKED, OnBnClickedNotificationsDelete)
    COMMAND_HANDLER(IDC_NOTIFICATIONS_EDIT, BN_CLICKED, OnBnClickedNotificationsEdit)
    CHAIN_MSG_MAP(CDialogResize<CNotificationsPage>)
    REFLECT_NOTIFICATIONS()
  END_MSG_MAP()

  BEGIN_DLGRESIZE_MAP(CNotificationsPage)
    DLGRESIZE_CONTROL(IDC_NOTIFICATIONS_GROUP   ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_NOTIFICATIONS_EVENTS  ,DLSZ_SIZE_X | DLSZ_SIZE_Y)
    DLGRESIZE_CONTROL(IDC_STATIC_INFO           ,DLSZ_SIZE_X | DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_NOTIFICATIONS_NEW     ,DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_NOTIFICATIONS_DELETE  ,DLSZ_SIZE_X | DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_NOTIFICATIONS_EDIT    ,DLSZ_MOVE_X | DLSZ_MOVE_Y)
  END_DLGRESIZE_MAP()

  // page common stuff
  void OnPageDisplay ( void );
  void OnPageDone ( void );
  BOOL OnPageValidate ( void );
  
  // page specific stuff
  void UpdateEventView( int ItemNum );
  void CNotificationsPage::EditEvent(int ItemNum);

  LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);  
  LRESULT OnCbnSelchangeNotificationsGroup(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

  LRESULT OnNMRclickNotificationsEvents(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
  LRESULT OnNMDblclkNotificationsEvents(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
  LRESULT OnBnClickedNotificationsNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedNotificationsDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedNotificationsEdit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
