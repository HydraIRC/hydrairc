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
