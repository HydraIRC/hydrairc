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

#include "stdafx.h"
#include "HydraIRC.h"
#include ".\prefs_loggingpage.h"

void CLoggingPage::OnPageDisplay( void )
{
  // attach the controls
  m_EnableCtrl.Attach(GetDlgItem(IDC_LOGGING_ENABLE));
  m_CreateNetworkFolderCtrl.Attach(GetDlgItem(IDC_LOGGING_CREATENETWORKFOLDER));

  m_LogRootFolderCtrl.Attach(GetDlgItem(IDC_LOGGING_LOGROOTFOLDER));

  m_ServerLogFormatCtrl.Attach(GetDlgItem(IDC_LOGGING_SERVERLOGFORMAT));
  m_ChannelLogFormatCtrl.Attach(GetDlgItem(IDC_LOGGING_CHANNELLOGFORMAT));
  m_QueryLogFormatCtrl.Attach(GetDlgItem(IDC_LOGGING_QUERYLOGFORMAT));
  m_DCCChatLogFormatCtrl.Attach(GetDlgItem(IDC_LOGGING_DCCCHATLOGFORMAT));

  m_LogServerCtrl.Attach(GetDlgItem(IDC_LOGGING_LOGSERVER));
  m_LogChannelCtrl.Attach(GetDlgItem(IDC_LOGGING_LOGCHANNEL));
  m_LogQueryCtrl.Attach(GetDlgItem(IDC_LOGGING_LOGQUERY));
  m_LogDCCChatCtrl.Attach(GetDlgItem(IDC_LOGGING_LOGDCCCHAT));

  m_ServerStripCodesCtrl.Attach(GetDlgItem(IDC_LOGGING_SERVERSTRIPCODES));
  m_ChannelStripCodesCtrl.Attach(GetDlgItem(IDC_LOGGING_CHANNELSTRIPCODES));
  m_QueryStripCodesCtrl.Attach(GetDlgItem(IDC_LOGGING_QUERYSTRIPCODES));
  m_DCCChatStripCodesCtrl.Attach(GetDlgItem(IDC_LOGGING_DCCCHATSTRIPCODES));

  m_LogFileViewerCtrl.Attach(GetDlgItem(IDC_LOGGING_LOGVIEWER));

  // set the values
  m_EnableCtrl.SetCheck(g_pNewPrefs->m_BoolPrefs[PREF_bLogging - PREF_BOOL_FIRST]);
  m_CreateNetworkFolderCtrl.SetCheck(g_pNewPrefs->m_BoolPrefs[PREF_bCreateNetworkLogFolder - PREF_BOOL_FIRST]);

  m_LogRootFolderCtrl.SetWindowText(g_pNewPrefs->m_PathPrefs[PREF_pLogRootFolder - PREF_PATH_FIRST]);

  m_ServerLogFormatCtrl.SetWindowText(g_pNewPrefs->m_StringPrefs[PREF_sServerLogFormat - PREF_STRING_FIRST]);
  m_ChannelLogFormatCtrl.SetWindowText(g_pNewPrefs->m_StringPrefs[PREF_sChannelLogFormat - PREF_STRING_FIRST]);
  m_QueryLogFormatCtrl.SetWindowText(g_pNewPrefs->m_StringPrefs[PREF_sQueryLogFormat - PREF_STRING_FIRST]);
  m_DCCChatLogFormatCtrl.SetWindowText(g_pNewPrefs->m_StringPrefs[PREF_sDCCChatLogFormat - PREF_STRING_FIRST]);

  m_LogServerCtrl.SetCheck(g_pNewPrefs->m_BoolPrefs[PREF_bServerLogging - PREF_BOOL_FIRST]);
  m_LogChannelCtrl.SetCheck(g_pNewPrefs->m_BoolPrefs[PREF_bChannelLogging - PREF_BOOL_FIRST]);
  m_LogQueryCtrl.SetCheck(g_pNewPrefs->m_BoolPrefs[PREF_bQueryLogging - PREF_BOOL_FIRST]);
  m_LogDCCChatCtrl.SetCheck(g_pNewPrefs->m_BoolPrefs[PREF_bDCCChatLogging - PREF_BOOL_FIRST]);

  m_ServerStripCodesCtrl.SetCheck(g_pNewPrefs->m_BoolPrefs[PREF_bStripCodesFromServerLogs - PREF_BOOL_FIRST]);
  m_ChannelStripCodesCtrl.SetCheck(g_pNewPrefs->m_BoolPrefs[PREF_bStripCodesFromChannelLogs - PREF_BOOL_FIRST]);
  m_QueryStripCodesCtrl.SetCheck(g_pNewPrefs->m_BoolPrefs[PREF_bStripCodesFromQueryLogs - PREF_BOOL_FIRST]);
  m_DCCChatStripCodesCtrl.SetCheck(g_pNewPrefs->m_BoolPrefs[PREF_bStripCodesFromDCCChatLogs - PREF_BOOL_FIRST]);

  m_LogFileViewerCtrl.SetWindowText(g_pNewPrefs->m_StringPrefs[PREF_sLogFileViewer - PREF_STRING_FIRST]);

  this->UpdateWindow();
}

void CLoggingPage::OnPageDone( void )
{
  m_LogRootFolderCtrl.Detach();
  m_ServerLogFormatCtrl.Detach();

  m_ChannelLogFormatCtrl.Detach();

  m_QueryLogFormatCtrl.Detach();
  m_DCCChatLogFormatCtrl.Detach();
  m_EnableCtrl.Detach();
  m_CreateNetworkFolderCtrl.Detach();

  m_LogServerCtrl.Detach();
  m_LogChannelCtrl.Detach();
  m_LogQueryCtrl.Detach();
  m_LogDCCChatCtrl.Detach();

  m_ServerStripCodesCtrl.Detach();
  m_ChannelStripCodesCtrl.Detach();
  m_QueryStripCodesCtrl.Detach();
  m_DCCChatStripCodesCtrl.Detach();

  m_LogFileViewerCtrl.Detach();
}

BOOL CLoggingPage::OnPageValidate( void )
{
  BOOL OK;

  g_pNewPrefs->m_BoolPrefs[PREF_bLogging - PREF_BOOL_FIRST] = m_EnableCtrl.GetCheck();
  g_pNewPrefs->m_BoolPrefs[PREF_bCreateNetworkLogFolder - PREF_BOOL_FIRST] = m_CreateNetworkFolderCtrl.GetCheck();

  OK = (CopyWindowText(&m_LogRootFolderCtrl,g_pNewPrefs->m_PathPrefs[PREF_pLogRootFolder - PREF_PATH_FIRST]) &&
        CopyWindowText(&m_ServerLogFormatCtrl,g_pNewPrefs->m_StringPrefs[PREF_sServerLogFormat - PREF_STRING_FIRST]) &&
        CopyWindowText(&m_ChannelLogFormatCtrl,g_pNewPrefs->m_StringPrefs[PREF_sChannelLogFormat - PREF_STRING_FIRST]) &&
        CopyWindowText(&m_QueryLogFormatCtrl,g_pNewPrefs->m_StringPrefs[PREF_sQueryLogFormat - PREF_STRING_FIRST]) &&
        CopyWindowText(&m_DCCChatLogFormatCtrl,g_pNewPrefs->m_StringPrefs[PREF_sDCCChatLogFormat - PREF_STRING_FIRST]) &&
        CopyWindowText(&m_LogFileViewerCtrl,g_pNewPrefs->m_StringPrefs[PREF_sLogFileViewer - PREF_STRING_FIRST]) );

  g_pNewPrefs->m_BoolPrefs[PREF_bServerLogging - PREF_BOOL_FIRST] = m_LogServerCtrl.GetCheck();
  g_pNewPrefs->m_BoolPrefs[PREF_bChannelLogging - PREF_BOOL_FIRST] = m_LogChannelCtrl.GetCheck();
  g_pNewPrefs->m_BoolPrefs[PREF_bQueryLogging - PREF_BOOL_FIRST] = m_LogQueryCtrl.GetCheck();
  g_pNewPrefs->m_BoolPrefs[PREF_bDCCChatLogging - PREF_BOOL_FIRST] = m_LogDCCChatCtrl.GetCheck();

  g_pNewPrefs->m_BoolPrefs[PREF_bStripCodesFromServerLogs - PREF_BOOL_FIRST] = m_ServerStripCodesCtrl.GetCheck();
  g_pNewPrefs->m_BoolPrefs[PREF_bStripCodesFromChannelLogs - PREF_BOOL_FIRST] = m_ChannelStripCodesCtrl.GetCheck();
  g_pNewPrefs->m_BoolPrefs[PREF_bStripCodesFromQueryLogs - PREF_BOOL_FIRST] = m_QueryStripCodesCtrl.GetCheck();
  g_pNewPrefs->m_BoolPrefs[PREF_bStripCodesFromDCCChatLogs - PREF_BOOL_FIRST] = m_DCCChatStripCodesCtrl.GetCheck();

  strippathseparator(g_pNewPrefs->m_PathPrefs[PREF_pLogRootFolder - PREF_PATH_FIRST]);

  if (OK) // all string pointers are valid..
  {
    // disable logging if we've got blank strings..
    if (!*g_pNewPrefs->m_PathPrefs[PREF_sServerLogFormat - PREF_PATH_FIRST]) g_pNewPrefs->m_BoolPrefs[PREF_bServerLogging - PREF_BOOL_FIRST] = FALSE;
    if (!*g_pNewPrefs->m_PathPrefs[PREF_sChannelLogFormat - PREF_PATH_FIRST]) g_pNewPrefs->m_BoolPrefs[PREF_bChannelLogging - PREF_BOOL_FIRST] = FALSE;
    if (!*g_pNewPrefs->m_PathPrefs[PREF_sQueryLogFormat - PREF_PATH_FIRST]) g_pNewPrefs->m_BoolPrefs[PREF_bQueryLogging - PREF_BOOL_FIRST] = FALSE;
    if (!*g_pNewPrefs->m_PathPrefs[PREF_sDCCChatLogFormat - PREF_PATH_FIRST]) g_pNewPrefs->m_BoolPrefs[PREF_bDCCChatLogging - PREF_BOOL_FIRST] = FALSE;
  }

  return OK;
}

LRESULT CLoggingPage::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  // the real init is done when the page is OnPageDisplay'd, as other preference items might
  // make a difference to the data

  DlgResize_Init(false,true,0);

  return 0;
}

LRESULT CLoggingPage::OnBnClickedLoggingLogviewerbrowse(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  char *FolderName;
  CFileDialog *pFileDialog;

  // open the file dialog in the last place that was used
  
  // TODO: move this bit of code to utilities.cpp
  FolderName = strdup(g_pNewPrefs->m_StringPrefs[PREF_sLogFileViewer - PREF_STRING_FIRST]);
  if (!FolderName)
    FolderName = strdup("C:\\Program Files\\");

  pFileDialog = new CFileDialog(TRUE,NULL,FolderName,OFN_FILEMUSTEXIST,"*.exe");
  if (pFileDialog)
  {
    int result = pFileDialog->DoModal();
    if (result == IDOK)
    {
      // remember filename
      SetString(g_pNewPrefs->m_StringPrefs[PREF_sLogFileViewer - PREF_STRING_FIRST],pFileDialog->m_szFileName);

      // update the GUI
      m_LogFileViewerCtrl.SetWindowText(pFileDialog->m_szFileName);
    }
    delete pFileDialog;
  }
  SetString(FolderName,NULL);

  return 0;
}
