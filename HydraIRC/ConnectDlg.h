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

// Connectdlg.h : interface of the CConnectDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CConnectDlg :
  public CDialogImpl<CConnectDlg>,
  public CDialogResize<CConnectDlg>
{
private:

#ifdef USE_XPCOMMANDBAR
public:
  CMDICommandBarXPCtrl m_CmdBar; // just for the nice XP-Look menus.
#endif

  CSimpleArray<TreeItemInfo*> m_TIIList;
  CTreeViewCtrl	m_ServerTree;
  CEdit m_ServerURI;
  CEdit m_Password;
  CComboBox m_IdentityCtrl;

  BOOL m_PrefsChanged;
  IRCServerDetails *m_pServerDetails;
  UserIdentity *m_pIdentity;

  void UpdateControls( void );
  void OnOK( void );
  void ShowContextMenu( HTREEITEM selectitem);
  void CleanupServerTree( void );
  void RefreshServerTree( IRCServerDetails *pSelectServer, int NetworkID );

public:
	enum { IDD = IDD_CONNECT };

  CConnectDlg( IRCServerDetails *pServerDetails, UserIdentity *pUser);
  ~CConnectDlg( void );

  IRCNetworkDetails *AddNetwork( void );
  void EditNetwork( void );
  void DeleteNetwork( void );

  IRCServerDetails *AddServer( void );
  void EditServer( void );
  void DeleteServer( void );


	BEGIN_MSG_MAP(CConnectDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    NOTIFY_CODE_HANDLER(TVN_SELCHANGED, OnTreeSelectChange)    
		COMMAND_ID_HANDLER(IDOK, OnOKCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
    COMMAND_HANDLER(IDC_SERVERURI, EN_CHANGE, OnEnUpdateServeruri)
    COMMAND_HANDLER(IDC_SERVERURI, EN_SETFOCUS, OnEnSetfocusServeruri)
    NOTIFY_HANDLER(IDC_SERVERTREE, NM_DBLCLK, OnNMDblclkServertree)
    COMMAND_HANDLER(IDC_CONNECT_EDITIDENTITY, BN_CLICKED, OnBnClickedConnectEditidentity)
    NOTIFY_HANDLER(IDC_SERVERTREE, NM_RCLICK, OnNMRclickServertree)
    // context menus
    COMMAND_ID_HANDLER(ID_CONNECT_SC_CONNECT,OnContextMenuServerConnect)
    COMMAND_ID_HANDLER(ID_CONNECT_SC_ADD,OnContextMenuServerAdd)
    COMMAND_ID_HANDLER(ID_CONNECT_SC_EDIT,OnContextMenuServerEdit)
    COMMAND_ID_HANDLER(ID_CONNECT_SC_DELETE,OnContextMenuServerDelete)

    COMMAND_ID_HANDLER(ID_CONNECT_NC_ADD,OnContextMenuNetworkAdd)
    COMMAND_ID_HANDLER(ID_CONNECT_NC_ADDSERVER,OnContextMenuServerAdd)
    COMMAND_ID_HANDLER(ID_CONNECT_NC_EDIT,OnContextMenuNetworkEdit)
    COMMAND_ID_HANDLER(ID_CONNECT_NC_DELETE,OnContextMenuNetworkDelete)

    COMMAND_HANDLER(IDC_CONNECT_ADDNETWORK, BN_CLICKED, OnBnClickedConnectAddnetwork)
    COMMAND_HANDLER(IDC_CONNECT_ADDSERVER, BN_CLICKED, OnBnClickedConnectAddserver)
    CHAIN_MSG_MAP(CDialogResize<CConnectDlg>)
  END_MSG_MAP()

  BEGIN_DLGRESIZE_MAP(CConnectDlg)
    DLGRESIZE_CONTROL(IDC_SERVERTREE           ,DLSZ_SIZE_X | DLSZ_SIZE_Y)
    DLGRESIZE_CONTROL(IDC_CONNECT_SERVERFRAME  ,DLSZ_SIZE_X | DLSZ_SIZE_Y)
    DLGRESIZE_CONTROL(IDC_SERVERURI            ,DLSZ_MOVE_Y | DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_CONNECT_URILABEL     ,DLSZ_MOVE_Y)

    DLGRESIZE_CONTROL(IDC_CONNECT_ADDNETWORK   ,DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_CONNECT_ADDSERVER    ,DLSZ_MOVE_Y)
    
    DLGRESIZE_CONTROL(IDC_CONNECT_NOTELABEL    ,DLSZ_MOVE_Y)

    DLGRESIZE_CONTROL(IDC_CONNECT_PASSWORD     ,DLSZ_MOVE_Y | DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_CONNECT_PASSWORDLABEL,DLSZ_MOVE_Y)

    DLGRESIZE_CONTROL(IDC_CONNECT_IDENTITYFRAME,DLSZ_MOVE_Y | DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_CONNECT_IDENTITY     ,DLSZ_MOVE_Y | DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_CONNECT_EDITIDENTITY ,DLSZ_MOVE_X | DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDOK                     ,DLSZ_MOVE_X | DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDCANCEL                 ,DLSZ_MOVE_X | DLSZ_MOVE_Y)
  END_DLGRESIZE_MAP()


// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnTreeSelectChange(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
  LRESULT OnEnUpdateServeruri(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnEnSetfocusServeruri(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnNMDblclkServertree(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
  LRESULT OnBnClickedConnectEditidentity(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnNMRclickServertree(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
  LRESULT OnContextMenuServerConnect(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnContextMenuServerAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnContextMenuServerEdit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnContextMenuServerDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnContextMenuNetworkAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnContextMenuNetworkEdit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnContextMenuNetworkDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedConnectAddnetwork(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedConnectAddserver(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
