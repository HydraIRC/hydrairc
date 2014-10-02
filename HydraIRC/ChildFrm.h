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

// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

void sys_Printf(int Contents, const char *format, ...);

extern CMainFrame *g_pMainWnd;

#pragma once

// Forward Defines.
class IRCServer; 
class IRCChannel;
class IRCQuery;

class CChildFrame :
  public CChildCommon,
  public CListener,
  public CDropFilesHandler<CChildFrame>
{
private:
  int m_LastDirtyStatus; // used when updating GUI elements, like serverlist
  IRCUser *m_SelectedUserListUser;  //used for userlist context menus.
  WindowPrefs *m_pWindowPrefs; // null until GetWindowPrefs is called
  int m_LastSize; // SIZE_*, see WM_SIZE Notification
  short m_InfoUpdateTicks;
  CSimpleArray<CTimerCommand*> m_TimerCommands;

public:

  // from top to bottom
  CHeaderView m_HeaderView;
  CMsgView m_MsgView;
  CUserListView m_UserListView;
  CInfoView m_InfoView;
  CTextInputView m_TextInputView;

  // from top to bottom
  CHydraHorSplitterWindow m_HorizSplitter1; // used by HeaderView and m_VertSplitter1
  CHydraHorSplitterWindow m_HorizSplitter2; // used by m_VertSplitter1 and InputView
  CHydraSplitterWindow m_VertSplitter1;     // used by MessageView and m_HorizSplitter3
  CHydraHorSplitterWindow m_HorizSplitter3; // used by Info and UserList views

  CHydraPaneContainer m_UserListPane;
  CHydraPaneContainer m_InfoPane;

  //CChildSplitter *m_pSplitter;

  IRCServer *m_pServer;
  IRCChannel *m_pChannel;
  IRCQuery *m_pQuery;
  char *m_StatusStr; 
  BOOL m_FirstFocus;

  // there are unused color items in here which are not relevant to child windows
  COLORREF m_ColorPrefs[PREF_COLOR_MAX];

  // ID's used when calling plugins (todo: use a union?)
  int m_ChannelID;
  int m_ServerID;
  int m_QueryID;

  int m_ScrollTickCount;
  void CChildFrame::OnEvent(int EventID, void *pData);

  CChildFrame(CEventManager *pEventManager, IRCServer *pServer);
  CChildFrame(CEventManager *pEventManager, IRCServer *pServer, IRCChannel *pChannel);
  CChildFrame(CEventManager *pEventManager, IRCServer *pServer, IRCQuery *pQuery);
  ~CChildFrame();

  void GetWindowPrefs( void );
  void Initialise( void );  

  void ActivateTextInputPane( void );

  void ToggleUserList( void );
  void ShowUserList( void );
  void HideUserList( void );
  BOOL IsUserListVisible( void );

  void ToggleHeader( void );
  void ShowHeader( void );
  void HideHeader( void );
  BOOL IsHeaderVisible( void );

  void ToggleInfo( void );
  void ShowInfo( void );
  void HideInfo( void );
  BOOL IsInfoVisible( void );

  BOOL CanHaveInfo( void )
  {
    return (m_WindowType == CWTYPE_SERVER || 
            m_WindowType == CWTYPE_CHANNEL || 
            m_WindowType == CWTYPE_QUERY || 
            m_WindowType == CWTYPE_DCCCHAT) ? TRUE : FALSE;
  }

  BOOL CanHaveHeader( void )
  {
    return (m_WindowType == CWTYPE_SERVER || 
            m_WindowType == CWTYPE_CHANNEL || 
            m_WindowType == CWTYPE_QUERY || 
            m_WindowType == CWTYPE_DCCCHAT) ? TRUE : FALSE;
  }

  BOOL CanHaveUserList( void )
  {
    return (m_WindowType == CWTYPE_SERVER || 
            m_WindowType == CWTYPE_CHANNEL || 
            m_WindowType == CWTYPE_QUERY || 
            m_WindowType == CWTYPE_DCCCHAT) ? TRUE : FALSE;
  }

	BEGIN_MSG_MAP(CChildFrame)
    //sys_Printf(BIC_GUI,"message: 0x%08x - code: 0x08%x\n",uMsg, uMsg == WM_NOTIFY ? ((LPNMHDR)lParam)->code : 0);
    //if (uMsg == WM_NOTIFY) sys_Printf(BIC_GUI,"notify message: %x - code: %x\n",uMsg, ((LPNMHDR)lParam)->code);

    // custom messages
    MESSAGE_HANDLER(WM_TEXTINPUT,          OnTextInput) // sent by the textinputview
    MESSAGE_HANDLER(WM_HEADERCHANGED,      OnHeaderChanged) // sent by the headerview
    MESSAGE_HANDLER(WM_UPDATEINFOPANE,     OnUpdateInfoPane) // sent by anything
    MESSAGE_HANDLER(WM_NETEVENT,           OnNetEvent) // sent by m_pSocket
    MESSAGE_HANDLER(WM_DNSEVENT,           OnDNSEvent) // sent by m_pSocket
    MESSAGE_HANDLER(WM_BUFFERWAITING,      OnBufferWaiting) // sent by AddToBuffer()
    MESSAGE_HANDLER(WM_PROCESSQUEUEWAITING,OnProcessQueue)    
#ifdef USE_TABBEDMDI
    MESSAGE_HANDLER(WM_UPDATETAB,          OnUpdateTab) // sent by anything
#endif

    // pane messages
    COMMAND_ID_HANDLER(ID_PANE_CLOSE,      OnPaneClose) // sent by pane windows

    // Windows Platform SDK Messages
    NOTIFY_CODE_HANDLER(EN_MSGFILTER,      OnEnMsgFilter) // for text selections    
    NOTIFY_CODE_HANDLER(EN_LINK,           ::OnEnLink)  // for doubleclickable url's
    MESSAGE_HANDLER(WM_CREATE,             OnCreate)
		MESSAGE_HANDLER(WM_CLOSE,              OnClose)
    MESSAGE_HANDLER(WM_SIZE,               OnSize)
    MESSAGE_HANDLER(WM_SYSCOMMAND,         OnSysCommand)
    MESSAGE_HANDLER(WM_SETFOCUS,           OnSetFocus)
    MESSAGE_HANDLER(WM_MDIACTIVATE,        OnMDIActivate)
    MESSAGE_HANDLER(WM_FORWARDMSG,         OnForwardMsg)
    // handle mouse activate correctly (Daniel Bowen, see WTL Mailing list - digest 718)
		MESSAGE_HANDLER(WM_MOUSEACTIVATE,      OnMouseActivate)
    MESSAGE_HANDLER(WM_COPY,               OnEditCopy)

    // context menu
    COMMAND_ID_HANDLER(IDC_CHILDFRAME_CLEARBUFFER, m_MsgView.OnBnClickedClearBuffer)
    COMMAND_ID_HANDLER(IDC_CHILDFRAME_COPYBUFFER,  m_MsgView.OnBnClickedCopyBuffer)
    COMMAND_ID_HANDLER(ID_CHANNEL_MONITOR,         g_pMainWnd->OnToggleChannelMonitor)
    COMMAND_ID_HANDLER(ID_SERVER_MONITOR,          g_pMainWnd->OnToggleServerMonitor)
    COMMAND_ID_HANDLER(ID_CHANNEL_PROPERTIES,      g_pMainWnd->OnChannelProperties)
    COMMAND_ID_HANDLER(ID_CHANNEL_SHOWSERVER,      g_pMainWnd->OnChannelShowServer)
    COMMAND_ID_HANDLER(ID_CHANNEL_CHANNELLIST,     g_pMainWnd->OnChannelChannelList)
    COMMAND_ID_HANDLER(ID_OPTIONS_TOGGLEINFO,      g_pMainWnd->OnToggleInfo)
    COMMAND_ID_HANDLER(ID_OPTIONS_TOGGLEHEADER,    g_pMainWnd->OnToggleHeader)
    COMMAND_ID_HANDLER(ID_OPTIONS_TOGGLEUSERLIST,  g_pMainWnd->OnToggleUserList)
    COMMAND_ID_HANDLER(ID_OPTIONS_LOGGING,         g_pMainWnd->OnToggleOptionLogging)
    COMMAND_ID_HANDLER(ID_OPTIONS_TIMESTAMPS,      g_pMainWnd->OnToggleOptionTimeStamps)
    COMMAND_ID_HANDLER(IDC_CHILDFRAME_VIEWLOGFILE, OnViewLogFile)
    COMMAND_ID_HANDLER(IDC_CHILDFRAME_CYCLE,       OnCycleChannel)
    COMMAND_ID_HANDLER(IDC_CHILDFRAME_DCCSEND,     OnDCCSend)
    COMMAND_ID_HANDLER(IDC_CHILDFRAME_DCCCHAT,     OnDCCChat)
    COMMAND_ID_HANDLER(IDC_CHILDFRAME_WHOIS,       OnWhoIs)
    COMMAND_RANGE_HANDLER(IDC_CHILDFRAME_VERBOSELEVEL_LEVEL0,IDC_CHILDFRAME_VERBOSELEVEL_LEVEL3,OnSetVerboseLevel)
    COMMAND_RANGE_HANDLER(ID_USERLISTFIRST,ID_USERLISTLAST,OnUserListContext)

    //CHAIN_CLIENT_COMMANDS()
    CHAIN_MSG_MAP(baseClass)
//+ <gliptic> Drag and drop files
    CHAIN_MSG_MAP(CDropFilesHandler<CChildFrame>)
// </gliptic>
		//-CHAIN_MSG_MAP(CMDIChildWindowImpl<CChildFrame>)

	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

  // Custom Messages for HydraIRC
  LRESULT OnTextInput(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnHeaderChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnPaneClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnUpdateInfoPane(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
#ifdef USE_TABBEDMDI
  LRESULT OnUpdateTab(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
#endif
  LRESULT OnUserListContext(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnSetVerboseLevel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnViewLogFile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnCycleChannel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnDCCSend(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnDCCChat(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnWhoIs(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

  LRESULT OnNetEvent(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
  LRESULT OnDNSEvent(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
  LRESULT OnBufferWaiting(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
  LRESULT OnProcessQueue(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);

  // Windows Platform SDK messages
  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
  LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnForwardMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
  LRESULT OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
  LRESULT OnMDIActivate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
  LRESULT OnEnMsgFilter(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);  
  LRESULT OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  //LRESULT OnEnSelChange(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

  LRESULT OnEditCopy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

//+ <gliptic> Drag and drop files
  BOOL IsReadyForDrop(void);
  BOOL HandleDroppedFile(LPCTSTR szBuff);
  void EndDropFiles(void);
// </gliptic>

  void DoContextMenu( void );
  void DoUserListContextMenu(TreeItemInfo *pTII, CUserListView *pUserListView);
  void DoUserListDoubleClick(TreeItemInfo *pTII);

  void SetVerboseLevel(int VerboseLevel);

  void UpdateSettings( BOOL IgnoreWindowPrefs );
  void UpdateStatus( void );
  void UpdateInfo( void );
#ifdef USE_TABBEDMDI
  void UpdateTab( void );
#endif
  void ProcessBuffer( void );  
	virtual void OnFinalMessage(HWND /*hWnd*/);

/* BEGIN_CEL_PATCH */
	void CheckPluginProcessing(char *Buffer, DWORD Mode);	
/* END_CEL_PATCH */
	
  void ProcessTextInput(char *Buffer, BOOL Parse);
  IRCServer *GetServer( void );

  void AddTimerCommand(int Delay, char *CommandProfileName);
  CTimerCommand *GetTimerCommand(char *CommandProfileName);

private:
  void InitialiseWindows( void );

  BOOL ProcessCommand(char *Command, char *Args);

  // Sends output to the default message buffer associated with this instance.
  void Put(const int Contents, char *buffer);
  void Printf(const int Contents, const char *format, ...);

public:
  // IRC Commands (that the user can use as commands, e,g /msg)
  // some have matching functions in IRCServer that do the actual
  // socket sending,  these are just input/arg parsers.
  void CMD_CTCPRequest    (char *Args);
  void CMD_Action         (const char *Msg );
  void CMD_Notice         (char *Args );
  void CMD_PrivMsg        (char *Args );
  void CMD_PrivMsg        (const char *To, const char *Msg);
  void CMD_Part           (char *Args );
  void CMD_Cycle          ( void );
  void CMD_Help           (const char *Args );
  void CMD_Quit           (char *Args );
  void CMD_Exit           (char *Args );
  void CMD_Join           (char *Args, int Flags = CCF_NONE );
  void CMD_Query          (char *Args );
  void CMD_Server         (char *Args, BOOL NewWindow = FALSE );
  void CMD_Raw            (char *Args );
  void CMD_Exec           (char *Args );
  void CMD_Timer          (char *Args );
  void CMD_StopTimer      (char *Args );
  void CMD_Nick           (char *Args );
  void CMD_GetNick        (char *Args );
  void CMD_Monitor        (const char *Args );
  void CMD_Logging        (const char *Args );
  void CMD_AudioMute      (const char *Args );
  void CMD_Timestamps     (char *Args );
  void CMD_Set            (char *Args );
  void CMD_DCC            (char *Args );
  void CMD_DCCSend        (char *Args );
  void CMD_DCCChat        (char *Args );
  void CMD_UnloadPlugin   (char *Args );
  void CMD_RefreshPlugins (const char *Args );
  void CMD_ListPlugins    (const char *Args );
  void CMD_Ignore         (const char *Args );
  void CMD_Clear          (const char *Args );
  void CMD_SaveTheme      (char *Args );
  void CMD_Window         (const char *Args );
/* BEGIN_CEL_PATCH */
	void CMD_CLEAR          (void );
/* END_CEL_PATCH */

protected:
  void ResizeHeader( void );

};
