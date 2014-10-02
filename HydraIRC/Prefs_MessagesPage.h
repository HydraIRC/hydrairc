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

#pragma once
class CMessagesPage :
  public CDialogImpl<CMessagesPage>,
  public CDialogResize<CMessagesPage>,
  public CPrefsPage
{
private:
  CEdit m_MaxPrivMsgLogAgeMinsCtrl;
  CEdit m_MaxPrivMsgLogItemsCtrl;
  CEdit m_MaxNoticeLogAgeMinsCtrl;
  CEdit m_MaxNoticeLogItemsCtrl;

  CEdit m_QueryNotifyReactivateDelayCtrl;

  CButton m_CreateQueryWindowsAutomaticallyCtrl;
  CButton m_CreateQueryWindowsHiddenCtrl;
  CButton m_RestrictQueriesToQueryWindowsCtrl;
  CButton m_RestrictNoticesToQueryWindowsCtrl;
  CButton m_ForwardNoticesCtrl;
  CButton m_ForwardMessagesCtrl;
  CButton m_CrossNetworkNoticeForwardingCtrl;
  CButton m_ChannelMemberNoticesOnlyCtrl;
  CButton m_RemovePrivMsgItemsCtrl;
  CButton m_RemoveNoticeItemsCtrl;

public:
  enum { IDD = IDD_PREFS_MESSAGES };

	BEGIN_MSG_MAP(CMessagesPage)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    CHAIN_MSG_MAP(CDialogResize<CMessagesPage>)
    REFLECT_NOTIFICATIONS()
  END_MSG_MAP()

  BEGIN_DLGRESIZE_MAP(CMessagesPage)
    // We don't need to resize or move anything
    //DLGRESIZE_CONTROL(IDC_MESSAGES_???    ,DLSZ_SIZE_X)
  END_DLGRESIZE_MAP()

  // page common stuff
  void OnPageDisplay ( void );
  void OnPageDone ( void );
  BOOL OnPageValidate ( void );

  LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};
