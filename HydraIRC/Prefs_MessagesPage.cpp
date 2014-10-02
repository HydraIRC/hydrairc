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

// Prefs_MessagesPage.h : interface of the CMessagesPage class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

void CMessagesPage::OnPageDisplay( void )
{
  // attach the controls
  m_MaxPrivMsgLogAgeMinsCtrl.Attach(GetDlgItem(IDC_MESSAGES_MAXPRIVMSGLOGAGEMINS));
  m_MaxPrivMsgLogItemsCtrl.Attach(GetDlgItem(IDC_MESSAGES_MAXPRIVMSGLOGITEMS));
  m_MaxNoticeLogAgeMinsCtrl.Attach(GetDlgItem(IDC_MESSAGES_MAXNOTICELOGAGEMINS));
  m_MaxNoticeLogItemsCtrl.Attach(GetDlgItem(IDC_MESSAGES_MAXNOTICELOGITEMS));
  m_QueryNotifyReactivateDelayCtrl.Attach(GetDlgItem(IDC_MESSAGES_QUERYNOTIFYREACTIVATEDELAY));

  m_CreateQueryWindowsAutomaticallyCtrl.Attach(GetDlgItem(IDC_MESSAGES_CREATEQUERYWINDOWSAUTOMATICALLY));
  m_CreateQueryWindowsHiddenCtrl.Attach(GetDlgItem(IDC_MESSAGES_CREATEQUERYWINDOWSHIDDEN));
  m_RestrictQueriesToQueryWindowsCtrl.Attach(GetDlgItem(IDC_MESSAGES_RESTRICTMESSAGES));
  m_RestrictNoticesToQueryWindowsCtrl.Attach(GetDlgItem(IDC_MESSAGES_RESTRICTNOTICES));
  m_ForwardNoticesCtrl.Attach(GetDlgItem(IDC_MESSAGES_FORWARDNOTICES));
  m_ForwardMessagesCtrl.Attach(GetDlgItem(IDC_MESSAGES_FORWARDMESSAGES));
  m_CrossNetworkNoticeForwardingCtrl.Attach(GetDlgItem(IDC_MESSAGES_CROSSNETWORKNOTICEFORWARDING));
  m_ChannelMemberNoticesOnlyCtrl.Attach(GetDlgItem(IDC_MESSAGES_CHANNELMEMBERNOTICESONLY));

  m_RemovePrivMsgItemsCtrl.Attach(GetDlgItem(IDC_MESSAGES_REMOVEPRIVMSGITEMS));
  m_RemoveNoticeItemsCtrl.Attach(GetDlgItem(IDC_MESSAGES_REMOVENOTICEITEMS));

  // set the values (I couldn't be arsed to convert numbers to strings and use SetWindowText()...)
  SetDlgItemInt(IDC_MESSAGES_MAXPRIVMSGLOGITEMS,g_pNewPrefs->m_IntPrefs[PREF_nMaxPrivMsgLogItems - PREF_INT_FIRST],FALSE);
  SetDlgItemInt(IDC_MESSAGES_MAXPRIVMSGLOGAGEMINS,g_pNewPrefs->m_IntPrefs[PREF_nMaxPrivMsgLogAgeMins - PREF_INT_FIRST],FALSE);
  SetDlgItemInt(IDC_MESSAGES_MAXNOTICELOGITEMS,g_pNewPrefs->m_IntPrefs[PREF_nMaxNoticeLogItems - PREF_INT_FIRST],FALSE);
  SetDlgItemInt(IDC_MESSAGES_MAXNOTICELOGAGEMINS,g_pNewPrefs->m_IntPrefs[PREF_nMaxNoticeLogAgeMins - PREF_INT_FIRST],FALSE);
  SetDlgItemInt(IDC_MESSAGES_QUERYNOTIFYREACTIVATEDELAY,g_pNewPrefs->m_IntPrefs[PREF_nQueryNotifyReactivateDelay - PREF_INT_FIRST],FALSE);

  m_CrossNetworkNoticeForwardingCtrl.SetCheck(g_pNewPrefs->m_BoolPrefs[PREF_bCrossNetworkNoticeForwarding - PREF_BOOL_FIRST]);
  m_CreateQueryWindowsAutomaticallyCtrl.SetCheck(g_pNewPrefs->m_BoolPrefs[PREF_bCreateQueryWindowsAutomatically - PREF_BOOL_FIRST]);
  m_CreateQueryWindowsHiddenCtrl.SetCheck(g_pNewPrefs->m_BoolPrefs[PREF_bCreateQueryWindowsHidden - PREF_BOOL_FIRST]);
  m_RestrictQueriesToQueryWindowsCtrl.SetCheck(g_pNewPrefs->m_BoolPrefs[PREF_bRestrictQueriesToQueryWindows - PREF_BOOL_FIRST]);
  m_RestrictNoticesToQueryWindowsCtrl.SetCheck(g_pNewPrefs->m_BoolPrefs[PREF_bRestrictNoticesToQueryWindows - PREF_BOOL_FIRST]);
  m_ForwardNoticesCtrl.SetCheck(g_pNewPrefs->m_BoolPrefs[PREF_bForwardNotices - PREF_BOOL_FIRST]);
  m_ForwardMessagesCtrl.SetCheck(g_pNewPrefs->m_BoolPrefs[PREF_bForwardMessages - PREF_BOOL_FIRST]);
  m_ChannelMemberNoticesOnlyCtrl.SetCheck(g_pNewPrefs->m_BoolPrefs[PREF_bChannelMemberNoticesOnly - PREF_BOOL_FIRST]);

  m_RemovePrivMsgItemsCtrl.SetCheck(g_pNewPrefs->m_BoolPrefs[PREF_bRemoveOldPrivmsgLogItems - PREF_BOOL_FIRST]);
  m_RemoveNoticeItemsCtrl.SetCheck(g_pNewPrefs->m_BoolPrefs[PREF_bRemoveOldNoticeLogItems - PREF_BOOL_FIRST]);

  this->UpdateWindow();
}

void CMessagesPage::OnPageDone( void )
{
  m_MaxPrivMsgLogAgeMinsCtrl.Detach();
  m_MaxPrivMsgLogItemsCtrl.Detach();
  m_MaxNoticeLogAgeMinsCtrl.Detach();
  m_MaxNoticeLogItemsCtrl.Detach();
  m_QueryNotifyReactivateDelayCtrl.Detach();

  m_CreateQueryWindowsAutomaticallyCtrl.Detach();
  m_CreateQueryWindowsHiddenCtrl.Detach();
  m_RestrictQueriesToQueryWindowsCtrl.Detach();
  m_RestrictNoticesToQueryWindowsCtrl.Detach();
  m_ForwardNoticesCtrl.Detach();
  m_ForwardMessagesCtrl.Detach();
  m_CrossNetworkNoticeForwardingCtrl.Detach();
  m_ChannelMemberNoticesOnlyCtrl.Detach();

  m_RemovePrivMsgItemsCtrl.Detach();
  m_RemoveNoticeItemsCtrl.Detach();

}

BOOL CMessagesPage::OnPageValidate( void )
{
  // don't need any validation.  user can't break anything!
  BOOL OK = TRUE;


  // Get the values from the controls and store them in the prefs
  char *NumStr = NULL; // important init!

  BOOL Check = FALSE;
  if (Check = (CopyWindowText(&m_MaxPrivMsgLogItemsCtrl,NumStr) && *NumStr))
    g_pNewPrefs->m_IntPrefs[PREF_nMaxPrivMsgLogItems - PREF_INT_FIRST] = atoi(NumStr);
  if (!Check) OK = FALSE;
  if (Check = (CopyWindowText(&m_MaxPrivMsgLogAgeMinsCtrl,NumStr) && *NumStr))
    g_pNewPrefs->m_IntPrefs[PREF_nMaxPrivMsgLogAgeMins - PREF_INT_FIRST] = atoi(NumStr);
  if (!Check) OK = FALSE;
  if (Check = (CopyWindowText(&m_MaxNoticeLogItemsCtrl,NumStr) && *NumStr))
    g_pNewPrefs->m_IntPrefs[PREF_nMaxNoticeLogItems - PREF_INT_FIRST] = atoi(NumStr);
  if (!Check) OK = FALSE;
  if (Check = (CopyWindowText(&m_MaxNoticeLogAgeMinsCtrl,NumStr) && *NumStr))
    g_pNewPrefs->m_IntPrefs[PREF_nMaxNoticeLogAgeMins - PREF_INT_FIRST] = atoi(NumStr);
  if (!Check) OK = FALSE;
  if (Check = (CopyWindowText(&m_QueryNotifyReactivateDelayCtrl,NumStr) && *NumStr))
    g_pNewPrefs->m_IntPrefs[PREF_nQueryNotifyReactivateDelay - PREF_INT_FIRST] = atoi(NumStr);
  if (!Check) OK = FALSE;

  if (NumStr) free(NumStr);

  g_pNewPrefs->m_BoolPrefs[PREF_bCreateQueryWindowsAutomatically - PREF_BOOL_FIRST] = m_CreateQueryWindowsAutomaticallyCtrl.GetCheck();
  g_pNewPrefs->m_BoolPrefs[PREF_bCreateQueryWindowsHidden - PREF_BOOL_FIRST] = m_CreateQueryWindowsHiddenCtrl.GetCheck();
  g_pNewPrefs->m_BoolPrefs[PREF_bRestrictQueriesToQueryWindows - PREF_BOOL_FIRST] = m_RestrictQueriesToQueryWindowsCtrl.GetCheck();
  g_pNewPrefs->m_BoolPrefs[PREF_bRestrictNoticesToQueryWindows - PREF_BOOL_FIRST] = m_RestrictNoticesToQueryWindowsCtrl.GetCheck();
  g_pNewPrefs->m_BoolPrefs[PREF_bForwardNotices - PREF_BOOL_FIRST] = m_ForwardNoticesCtrl.GetCheck();
  g_pNewPrefs->m_BoolPrefs[PREF_bForwardMessages - PREF_BOOL_FIRST] = m_ForwardMessagesCtrl.GetCheck();
  g_pNewPrefs->m_BoolPrefs[PREF_bCrossNetworkNoticeForwarding - PREF_BOOL_FIRST] = m_CrossNetworkNoticeForwardingCtrl.GetCheck();
  g_pNewPrefs->m_BoolPrefs[PREF_bChannelMemberNoticesOnly - PREF_BOOL_FIRST] = m_ChannelMemberNoticesOnlyCtrl.GetCheck();

  g_pNewPrefs->m_BoolPrefs[PREF_bRemoveOldPrivmsgLogItems - PREF_BOOL_FIRST] = m_RemovePrivMsgItemsCtrl.GetCheck();
  g_pNewPrefs->m_BoolPrefs[PREF_bRemoveOldNoticeLogItems - PREF_BOOL_FIRST] = m_RemoveNoticeItemsCtrl.GetCheck();

  return OK;
}

LRESULT CMessagesPage::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  // the real init is done when the page is OnPageDisplay'd, as other preference items might
  // make a difference to the data

  DlgResize_Init(false,true,0);

  return 0;
}
