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

// PrefsDlg.h : interface of the CPrefsDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#define PREFSPAGE_NOTIMPLEMENTED  0
#define PREFSPAGE_IDENTITIES      1
#define PREFSPAGE_BUDDYGROUPS     2
#define PREFSPAGE_NOTIFICATIONS   3
#define PREFSPAGE_MISC            4
#define PREFSPAGE_LOGGING         5
#define PREFSPAGE_MESSAGES        6
#define PREFSPAGE_COMMANDPROFILES 7
#define PREFSPAGE_HIGHLIGHTING    8
#define PREFSPAGE_SELECTCHILD     9
//#define PREFSPAGE_FILTERS         10

class CPrefsDlg : 
  public CDialogImpl<CPrefsDlg>,
  public CDialogResize<CPrefsDlg>
{
private:
  CTreeViewCtrl	m_PrefsTree;
  CSimpleArray<CPrefsPage *> m_PrefsPages;
  CPrefsPage *m_pActivePage;

public:
	enum { IDD = IDD_PREFS };

  CDlgContainerCtrl m_CtrlContainer;
  CIdentitiesPage m_IdentitiesPage;  
  CNotImplementedPage m_NotImplementedPage;
  CNotificationsPage m_NotificationsPage;
  CBuddyGroupsPage m_BuddyGroupsPage;
  CMiscPage m_MiscPage;
  CMessagesPage m_MessagesPage;
  CLoggingPage m_LoggingPage;
  CCommandProfilesPage m_CommandProfilesPage;
  CHighlightingPage m_HighlightingPage;
  CSelectChildPage m_SelectChildPage;
  // TODO: Finish filters page.
  //CFiltersPage m_FiltersPage;

  CStatic m_OptionCtrl;
  CStatic m_InfoCtrl;

  int m_DefaultPage;

  //~CPrefsDlg( void );
  CPrefsDlg( int PageNum )
  {
    m_DefaultPage = PageNum;
    m_pActivePage = NULL;
  }
  void ShowPage( int PageNum );

	BEGIN_MSG_MAP(CPrefsDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    NOTIFY_HANDLER(IDC_PREFSTREE, TVN_SELCHANGING, OnTvnSelchangingPrefstree)
    NOTIFY_HANDLER(IDC_PREFSTREE, TVN_SELCHANGED, OnTreeSelectChange)    
		COMMAND_ID_HANDLER(IDOK, OnOKCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
    CHAIN_MSG_MAP(CDialogResize<CPrefsDlg>)
  END_MSG_MAP()

  BEGIN_DLGRESIZE_MAP(CPrefsDlg)
    DLGRESIZE_CONTROL(IDC_PREFSTREE           ,DLSZ_SIZE_Y)
    DLGRESIZE_CONTROL(IDC_STATIC_INFO         ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_PREFS_PLACEHOLDER   ,DLSZ_SIZE_X | DLSZ_SIZE_Y)

    DLGRESIZE_CONTROL(IDOK                    ,DLSZ_MOVE_X | DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDCANCEL                ,DLSZ_MOVE_X | DLSZ_MOVE_Y)
  END_DLGRESIZE_MAP()


// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnTreeSelectChange(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
  
  LRESULT OnTvnSelchangingPrefstree(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
};
