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

// 
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

enum ChannelListSortModes {
  CLSM_NAME,
  CLSM_NAMEREVERSE,
  CLSM_USERS,
  CLSM_USERSREVERSE,
  CLSM_TOPIC,
  CLSM_TOPICREVERSE,
};

class CChannelListDlg : 
  public CDialogImpl<CChannelListDlg>,
  public CDialogResize<CChannelListDlg>
{
private:
  BOOL m_Initialised;
  int m_CurrentSortMode;
  BOOL m_OnListStartCalled; //see CChannelListDlg::AddChannel()

  // Controls
  CEdit m_SearchTextCtrl;
  CEdit m_UsersMinCtrl;
  CEdit m_UsersMaxCtrl;
  CComboBox m_SearchSelectCtrl;
  CButton m_JoinCtrl;
  CButton m_PopulateCtrl;
  
  CButton m_ShowAllCtrl;
  CButton m_ShowFavoritesCtrl;

  CListViewCtrl m_ChannelListCtrl;

  CProgressBarCtrl m_ProgressCtrl;

  IRCServer *m_pServer;

  void DeleteChannelList( void );
  void EnableControls( BOOL Enable );

public:
  CSimpleArray<CChannelProperties *> m_ChannelList;

	enum { IDD = IDD_CHANNELLIST };

  CChannelListDlg(IRCServer *pServer)
  {
    m_Initialised = FALSE;
    m_pServer = pServer;
    m_CurrentSortMode = CLSM_NAME;
    m_OnListStartCalled = FALSE;
  }

  ~CChannelListDlg();

  void AddChannel( char *ChannelName, int Users, char *Topic);
  void OnListStart( void );
  void OnListEnd( void );
  
	BEGIN_MSG_MAP(CChannelListDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(ID_CHANNELLIST_JOIN, OnJoinCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		
    COMMAND_HANDLER(ID_CHANNELLIST_SEARCHTEXT,  EN_CHANGE,     OnSomethingChanged)
    COMMAND_HANDLER(ID_CHANNELLIST_USERSMIN,    EN_CHANGE,     OnSomethingChanged)
    COMMAND_HANDLER(ID_CHANNELLIST_USERSMAX,    EN_CHANGE,     OnSomethingChanged)
    
    COMMAND_HANDLER(ID_CHANNELLIST_POPULATE, BN_CLICKED, OnBnClickedChannellistPopulate)
    NOTIFY_HANDLER(ID_CHANNELLIST_LIST, NM_DBLCLK, OnNMDblclkChannellistList)
    NOTIFY_HANDLER(ID_CHANNELLIST_LIST, LVN_COLUMNCLICK, OnLvnColumnclickChannellistList)
    CHAIN_MSG_MAP(CDialogResize<CChannelListDlg>)
  END_MSG_MAP()

  BEGIN_DLGRESIZE_MAP(CChannelListDlg)
    DLGRESIZE_CONTROL(ID_CHANNELLIST_SELECTGROUP ,DLSZ_SIZE_X)    
    DLGRESIZE_CONTROL(ID_CHANNELLIST_LIST        ,DLSZ_SIZE_X | DLSZ_SIZE_Y)    
        
    DLGRESIZE_CONTROL(ID_CHANNELLIST_POPULATE   ,DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(ID_CHANNELLIST_PROGRESS   ,DLSZ_SIZE_X | DLSZ_MOVE_Y)    
    DLGRESIZE_CONTROL(ID_CHANNELLIST_JOIN       ,DLSZ_MOVE_X | DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDCANCEL                  ,DLSZ_MOVE_X | DLSZ_MOVE_Y)
  END_DLGRESIZE_MAP()


// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnJoinCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnSomethingChanged(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedChannellistPopulate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnNMDblclkChannellistList(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
  LRESULT OnLvnColumnclickChannellistList(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
};
