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
