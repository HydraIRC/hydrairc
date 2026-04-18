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

// Prefs_LoggingPage.h : interface of the CLoggingPage class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
class CLoggingPage :
  public CDialogImpl<CLoggingPage>,
  public CDialogResize<CLoggingPage>,
  public CPrefsPage
{
private:
  CEdit m_LogRootFolderCtrl;
  CEdit m_ServerLogFormatCtrl;
  CEdit m_ChannelLogFormatCtrl;
  CEdit m_QueryLogFormatCtrl;
  CEdit m_DCCChatLogFormatCtrl;
  CEdit m_LogFileViewerCtrl;
  CButton m_EnableCtrl;
  CButton m_CreateNetworkFolderCtrl;
  CButton m_LogServerCtrl;
  CButton m_LogChannelCtrl;
  CButton m_LogQueryCtrl;
  CButton m_LogDCCChatCtrl;
  CButton m_ServerStripCodesCtrl;
  CButton m_ChannelStripCodesCtrl;
  CButton m_QueryStripCodesCtrl;
  CButton m_DCCChatStripCodesCtrl;

public:
  enum { IDD = IDD_PREFS_LOGGING };

	BEGIN_MSG_MAP(CLoggingPage)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_HANDLER(IDC_LOGGING_LOGVIEWERBROWSE, BN_CLICKED, OnBnClickedLoggingLogviewerbrowse)
    CHAIN_MSG_MAP(CDialogResize<CLoggingPage>)
    REFLECT_NOTIFICATIONS()
  END_MSG_MAP()

  BEGIN_DLGRESIZE_MAP(CLoggingPage)
    DLGRESIZE_CONTROL(IDC_LOGGING_LOGROOTFOLDER    ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_LOGGING_SERVERLOGFORMAT  ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_LOGGING_CHANNELLOGFORMAT ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_LOGGING_QUERYLOGFORMAT   ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_LOGGING_DCCCHATLOGFORMAT ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_LOGGING_LOGVIEWER        ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_LOGGING_LOGVIEWERBROWSE  ,DLSZ_MOVE_X)
  END_DLGRESIZE_MAP()

  // page common stuff
  void OnPageDisplay ( void );
  void OnPageDone ( void );
  BOOL OnPageValidate ( void );

  LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedLoggingLogviewerbrowse(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
