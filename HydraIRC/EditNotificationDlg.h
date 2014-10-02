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
  3.2) Will become the property of HydraIRC's author.

*/

// EditNotificationDlg.h : interface of the CEditNotificationDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CEditNotificationDlg : 
  public CDialogImpl<CEditNotificationDlg>,
  public CDialogResize<CEditNotificationDlg>
{
private:
  NotificationPrefInfo_t *m_pNPI;
  BOOL m_Initialised;
  BOOL m_Updating;

  // Controls
  CEdit m_FileNameCtrl;
  CEdit m_MatchStringCtrl;
  CComboBox m_SelectSystemSoundCtrl;
  CComboBox m_EventCtrl;
  CButton m_OKCtrl;
  CButton m_HasSoundCtrl;
  CButton m_BrowseCtrl;
  CButton m_EnabledCtrl;
  CButton m_SystemSoundCtrl;
  CButton m_SoundFileCtrl;
  CButton m_FlashTaskBarCtrl;
  CButton m_HighlightWholeLineCtrl;
  CButton m_HighlightCaseSensitiveCtrl;
  CButton m_LogInEventLogCtrl;

  // TODO: find a color picker control and use..
  CEdit m_ForegroundColorCtrl;
  CEdit m_BackgroundColorCtrl;
  CButton m_PickForegroundColorCtrl;
  CButton m_PickBackgroundColorCtrl;

  CFileDialog *m_pFileDialog;

public:
	enum { IDD = IDD_EDITNOTIFICATION };

  CEditNotificationDlg(NotificationPrefInfo_t *pNPI)
  {
    m_pNPI = pNPI;
    m_pFileDialog = new CFileDialog(TRUE,_TEXT("*.wav"));
    m_Initialised = FALSE;
    m_Updating = FALSE;
  }

  ~CEditNotificationDlg( void )
  {
    delete m_pFileDialog;
  }

  BOOL CEditNotificationDlg::Validate( void );
  void CEditNotificationDlg::UpdateColors( void );
  
	BEGIN_MSG_MAP(CEditNotificationDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOKCmd)
		COMMAND_ID_HANDLER(ID_EN_OK, OnOKCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
    COMMAND_HANDLER(IDC_EN_EVENT, CBN_SELCHANGE, OnSomethingChanged)
    COMMAND_HANDLER(IDC_EN_SYSTEMSOUND, BN_CLICKED, OnSomethingChanged)
    COMMAND_HANDLER(IDC_EN_FILENAME, EN_CHANGE, OnSomethingChanged)
    COMMAND_HANDLER(IDC_EN_MATCHSTRING, EN_CHANGE, OnSomethingChanged)
    COMMAND_HANDLER(IDC_EN_FOREGROUNDCOLOR, EN_CHANGE, OnSomethingChanged) // TODO: change when color picker is used
    COMMAND_HANDLER(IDC_EN_BACKGROUNDCOLOR, EN_CHANGE, OnSomethingChanged) // TODO: change when color picker is used
    COMMAND_HANDLER(IDC_EN_HIGHLIGHTWHOLELINE, BN_CLICKED, OnSomethingChanged)
    COMMAND_HANDLER(IDC_EN_HIGHLIGHTCASESENSITIVE, BN_CLICKED, OnSomethingChanged)
    COMMAND_HANDLER(IDC_EN_LOGINEVENTLOG, BN_CLICKED, OnSomethingChanged)
    COMMAND_HANDLER(IDC_EN_SOUNDFILE, BN_CLICKED, OnSomethingChanged)
    COMMAND_HANDLER(IDC_EN_FLASHTASKBAR, BN_CLICKED, OnSomethingChanged)
    COMMAND_HANDLER(IDC_EN_BROWSE, BN_CLICKED, OnBnClickedEnBrowse)
    COMMAND_HANDLER(IDC_EN_HASSOUND, BN_CLICKED, OnSomethingChanged)
    COMMAND_HANDLER(IDC_EN_SELECTSYSTEMSOUND, CBN_SELCHANGE, OnSomethingChanged)
    COMMAND_HANDLER(IDC_EN_PICKBGCOLOR, BN_CLICKED, OnBnClickedEnPickbgcolor)
    COMMAND_HANDLER(IDC_EN_PICKFGCOLOR, BN_CLICKED, OnBnClickedEnPickfgcolor)
    CHAIN_MSG_MAP(CDialogResize<CEditNotificationDlg>)
  END_MSG_MAP()

  BEGIN_DLGRESIZE_MAP(CEditNotificationDlg)
    DLGRESIZE_CONTROL(IDC_EN_EVENT            ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_EN_ENABLED          ,DLSZ_MOVE_X)
    DLGRESIZE_CONTROL(IDC_EN_SELECTSYSTEMSOUND,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_EN_FILENAME         ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_EN_BROWSE           ,DLSZ_MOVE_X)

    DLGRESIZE_CONTROL(IDC_EN_AUDIBLERECTANGLE ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_EN_VISUALRECTANGLE  ,DLSZ_SIZE_X)

    DLGRESIZE_CONTROL(IDC_EN_MATCHSTRING             ,DLSZ_SIZE_X)

    DLGRESIZE_CONTROL(ID_EN_OK               ,DLSZ_MOVE_X | DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDCANCEL                ,DLSZ_MOVE_X | DLSZ_MOVE_Y)
  END_DLGRESIZE_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnSomethingChanged(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedEnBrowse(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedEnPickbgcolor(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedEnPickfgcolor(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
