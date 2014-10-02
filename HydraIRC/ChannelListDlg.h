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
