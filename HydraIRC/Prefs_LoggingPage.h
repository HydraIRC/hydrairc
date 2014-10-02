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
