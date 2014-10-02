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
