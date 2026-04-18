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
