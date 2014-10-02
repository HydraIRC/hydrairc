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
  3.2) Will become the property of HydraIRC's author.

*/

// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CChildFrame;

typedef dockwins::CDockingFrameTraitsT< dockwins::CVC7LikeAutoHidePaneTraits, dockwins::CSimpleSplitterBar<3>,
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		WS_EX_APPWINDOW | WS_EX_WINDOWEDGE> CMainFrameTraits;

class CMainFrame : 
	public dockwins::CAutoHideMDIDockingFrameImpl<CMainFrame, CHidingMDIWindow, CMainFrameTraits, dockwins::CVC7LikeAutoHidePaneTraits>,
    public CUpdateUI<CMainFrame>,    
		public CMessageFilter, 
    public CIdleHandler
{
protected:
	typedef CMainFrame thisClass;
	typedef dockwins::CAutoHideMDIDockingFrameImpl<CMainFrame, CHidingMDIWindow, CMainFrameTraits, dockwins::CVC7LikeAutoHidePaneTraits> baseClass;

public:
#ifdef USE_XPCOMMANDBAR
  CMDICommandBarXPCtrl m_CmdBar;
  //CMDICommandBarCtrl m_CmdBar;
#else
#ifdef USE_TABBEDMDI
	CTabbedMDIClient<CDotNetTabCtrl<CTabViewTabItem> > m_tabbedClient;
	CTabbedMDICommandBarCtrl m_CmdBar;
#else
	CMDICommandBarCtrl m_CmdBar;
#endif

#endif // !USE_XPCOMMANDBAR

private:
  int m_StatusBarWidths[4];
  HWND createToolbar();
  void SetStatusPaneWidths(int* arrWidths, int nPanes);
  void UpdateStatusBar( void );
  char m_StatusBarText[SETSTATUSBARTEXT_BUFFER_SIZE];
  char m_StatusBarNick[SETSTATUSBARNICK_BUFFER_SIZE];
  time_t m_StatusBarTime;
  BOOL m_StatusBarDirty;
  int m_StatusBarIconID;
  int m_StatusBarNickIconID;
  IDManager m_FavoriteIDs;
  int m_FavoriteRootItems;

  void MinimizeToSystray( void );
  CTaskBarIcon m_TI;
  BOOL m_WasMaximized; // store the state of the window before hiding it

  // used to decide where to send the USERLIST context menu command messages
  HWND m_LastUserListMenuWindow;
  CUserListView *m_pLastUserListMenuView;

public:
  DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

  // Gui stuff
	sstate::CWindowStateMgr	m_stateMgr;

  CServerListView m_ServerListView;
  CUserListView m_UserListView;
  COutputView m_OutputView;
  CChannelMonitorView m_ChannelMonitorView;
  CServerMonitorView m_ServerMonitorView;

  CMultiPaneStatusBarCtrl m_Status;

  IDManager m_ToolbarUserIDs;
  CImageList m_images_toolbar;  
  IDManager m_ToolbarImageListIDs;

	CMainFrame()
		: m_ChannelMonitorView(IDI_CHANNELMONITOR,&g_EventManager),
      m_ServerMonitorView(IDI_SERVERMONITOR,&g_EventManager),
      m_OutputView(IDI_OUTPUT,&g_EventManager),
      // m_stateMgr(NULL,SW_SHOWMAXIMIZED), // SW_SHOWMAXIMIZED is ignored due to call to m_stateMgr->Initialise()
      m_ServerListView(IDI_SERVER),
      m_UserListView((IRCChannel *)NULL,IDI_USERLIST,&g_EventManager)
	{
    m_Timer = NULL;
    m_OkToQuit = 0;
    strcpy(m_StatusBarText,g_DefaultStrings[DEFSTR_DefaultStatusBarMessage]);
    m_StatusBarNick[0] = 0;
    m_StatusBarTime = time(NULL);
    m_StatusBarIconID = STATUS_ICON_NONE;
    m_StatusBarNickIconID = ICONID_USERS;
    m_StatusBarDirty = TRUE;
    m_FavoriteRootItems = 0;
    m_FavoriteIDs.SetIDRange(ID_FAVORITES_FIRST,ID_FAVORITES_LAST);
    m_ToolbarUserIDs.SetIDRange(ID_TOOLBARUSER_FIRST,ID_TOOLBARUSER_LAST);
    m_ToolbarImageListIDs.SetIDRange(1,255);
    m_LastUserListMenuWindow = NULL;
    m_pLastUserListMenuView = NULL;
	}

	~CMainFrame( void );

  // other stuff.
  CImageList m_images_serverlist; // public, because the serverlist class uses it..

  short m_OkToQuit;
  // Timer for the event manager
  INT_PTR m_Timer; // uses TID_MAINFRAME

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();
  virtual bool IsToolbarVisible(int ID) const;

  void UpdateGlobalUserList(IRCChannel *pChannel, BOOL Clear = FALSE, BOOL Refresh = FALSE);
  void ClearGlobalUserList( void );
  void DoPrefs( int PageNum = -1);
  void BuildFavoritesMenu( void );
  void RestoreWindow( void );
  void RefreshAllChildWindows( void ); // called by prefs and after some /set commands.
  void SetStatusBar( const int IconID, const char *message );
  void ChangeGUILayoutStyle(int LayoutStyle);

  void ToggleMonitoring(CChildFrame *pChildWnd);
  void ToggleTimeStamps(CChildFrame *pChild);
  void ToggleLogging(CChildFrame *pChild);

  void SetUserListMenuWindow(HWND hWnd)
    { m_LastUserListMenuWindow = hWnd; }

  void SetUserListMenuInstance(CUserListView *pUserListView)
    { m_pLastUserListMenuView = pUserListView; }

	BEGIN_MSG_MAP(CMainFrame)
    //sys_Printf(BIC_GUI,"message: %x - code: %x\n",uMsg, uMsg == WM_NOTIFY ? ((LPNMHDR)lParam)->code : 0);
	  MESSAGE_HANDLER(CWM_INITIALIZE, OnInitialize)		
    MESSAGE_HANDLER(WM_COPYDATA,OnCopyData)
    MESSAGE_HANDLER(WMU_WHERE_ARE_YOU, onWhereAreYou)
    MESSAGE_HANDLER(WM_IDENTDEVENT, g_pIdentd->OnIdentdEvent)
    MESSAGE_HANDLER(WM_TRANSFEREVENT, g_pTransfersManager->OnTransferEvent)
    MESSAGE_HANDLER(WM_PROCESSDCCS, g_pTransfersManager->OnProcessDCCs)
    MESSAGE_HANDLER(WM_REQUESTAUTOTILE,OnRequestAutoTile)
    MESSAGE_HANDLER(WM_REQUESTSCROLLTOEND,OnRequestScrollToEnd)
    MESSAGE_HANDLER(WM_CHILDFRAMECLOSED,OnChildFrameClosed)

    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	  MESSAGE_HANDLER(WM_CLOSE, OnClose)
    MESSAGE_HANDLER(WM_TIMER, OnTimer)
    MESSAGE_HANDLER(WM_SIZE, OnSize)
    MESSAGE_HANDLER(WM_SYSCOMMAND, OnSysCommand)

    // required by docking framework.
		MESSAGE_HANDLER(WM_SETTINGCHANGE, OnSettingChange)
		MESSAGE_HANDLER(WM_SYSCOLORCHANGE, OnSysColorChange)

    // misc
    COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
	  COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
    COMMAND_ID_HANDLER(ID_FILE_CLOSE, OnFileCloseWindow)
	  COMMAND_ID_HANDLER(ID_FILE_DISCONNECT, OnFileDisconnect)
    COMMAND_ID_HANDLER(ID_FILE_CONNECT, OnFileConnect)

    // view menu
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)

    // these turn on and off docking windows..
		COMMAND_TOGGLE_MEMBER_HANDLER(ID_VIEW_IRCSERVERLIST     ,m_ServerListView)
    COMMAND_TOGGLE_MEMBER_HANDLER(ID_VIEW_GLOBALUSERLIST    ,m_UserListView)
    COMMAND_TOGGLE_MEMBER_HANDLER(ID_VIEW_OUTPUT            ,m_OutputView)
    COMMAND_TOGGLE_MEMBER_HANDLER(ID_VIEW_CHANNELMONITOR    ,m_ChannelMonitorView)
    COMMAND_TOGGLE_MEMBER_HANDLER(ID_VIEW_SERVERMONITOR     ,m_ServerMonitorView)
    COMMAND_TOGGLE_MEMBER_HANDLER_PTR(ID_VIEW_EVENTLOG      ,g_pEventLogManager->m_pEventLogView)
    COMMAND_TOGGLE_MEMBER_HANDLER_PTR(ID_VIEW_PRIVMSGQUEUE  ,g_pPrivMsgQueueManager->m_pTextQueueView)
    COMMAND_TOGGLE_MEMBER_HANDLER_PTR(ID_VIEW_NOTICEQUEUE   ,g_pNoticeQueueManager->m_pTextQueueView)
    COMMAND_TOGGLE_MEMBER_HANDLER_PTR(ID_VIEW_URLCATCHER    ,g_pUrlCatcherManager->m_pTextQueueView)
    COMMAND_TOGGLE_MEMBER_HANDLER_PTR(ID_VIEW_TRANSFERS     ,g_pTransfersManager->m_pTransfersView)

    COMMAND_ID_HANDLER(ID_GUILAYOUT_LAYOUT1,OnViewGuiLayout);
    COMMAND_ID_HANDLER(ID_GUILAYOUT_LAYOUT2,OnViewGuiLayout);
    COMMAND_ID_HANDLER(ID_GUILAYOUT_LAYOUT3,OnViewGuiLayout);
    COMMAND_ID_HANDLER(ID_GUILAYOUT_LAYOUT4,OnViewGuiLayout);
    COMMAND_ID_HANDLER(ID_GUILAYOUT_LAYOUT5,OnViewGuiLayout);
    COMMAND_ID_HANDLER(ID_GUILAYOUT_HIDEALLDOCKERS,OnViewGuiLayout);

    // favorites menu
    COMMAND_ID_HANDLER(ID_FAVORITES_ORGANIZEFAVORITES, OnFavoritesOrganizeFavorites)
    COMMAND_ID_HANDLER(ID_FAVORITES_ADDTOFAVORITES, OnFavoritesAddToFavorites)
    COMMAND_RANGE_HANDLER(ID_FAVORITES_FIRST, ID_FAVORITES_LAST, OnFavoritesItemSelected)

    // help menu
    COMMAND_ID_HANDLER(ID_HELP_DOCUMENTATION,OnHelpDocumentation)
    COMMAND_ID_HANDLER(ID_HELP_ONLINEDOCUMENTATION,OnHelpOnlineDocumentation)
    COMMAND_ID_HANDLER(ID_HELP_RECENTCHANGES,OnHelpRecentChanges)
    COMMAND_ID_HANDLER(ID_HELP_HYDRAIRCONTHEWEB,OnHelpHydraIRCOnTheWeb)
    COMMAND_ID_HANDLER(ID_HELP_DONATE,OnHelpDonate)
    COMMAND_ID_HANDLER(ID_HELP_REPORTABUG,OnHelpReportABug)
    COMMAND_ID_HANDLER(ID_HELP_FORUMS,OnHelpForums)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnHelpAppAbout)

    // Windows menu
    COMMAND_ID_HANDLER(ID_WINDOW_CASCADE, OnWindowCascade)
		COMMAND_ID_HANDLER(ID_WINDOW_TILE_HORZ, OnWindowTileHorz)
    COMMAND_ID_HANDLER(ID_WINDOW_TILE_VERT, OnWindowTileVert)
    COMMAND_ID_HANDLER(ID_WINDOW_MINIMIZEALLWINDOWS, OnWindowMinimizeAllWindows)
    COMMAND_ID_HANDLER(ID_WINDOW_MINIMIZEACTIVEWINDOW, OnWindowMinimizeActiveWindow)
    
		//COMMAND_ID_HANDLER(ID_WINDOW_ARRANGE, OnWindowArrangeIcons)

    // toolbar and menus
    COMMAND_ID_HANDLER(ID_OPTIONS_TOGGLEINFO, OnToggleInfo)
    COMMAND_ID_HANDLER(ID_OPTIONS_TOGGLEHEADER, OnToggleHeader)
    COMMAND_ID_HANDLER(ID_OPTIONS_TOGGLEUSERLIST, OnToggleUserList)
    COMMAND_ID_HANDLER(ID_CHANNEL_MONITOR,  OnToggleChannelMonitor)
    COMMAND_ID_HANDLER(ID_CHANNEL_SHOWSERVER,  OnChannelShowServer)
    COMMAND_ID_HANDLER(ID_CHANNEL_PROPERTIES,  OnChannelProperties)    
    COMMAND_ID_HANDLER(ID_CHANNEL_CHANNELLIST, OnChannelChannelList)    

    // options menu
    COMMAND_ID_HANDLER(ID_OPTIONS_LOGGING, OnToggleOptionLogging)
    COMMAND_ID_HANDLER(ID_OPTIONS_TIMESTAMPS, OnToggleOptionTimeStamps)
    COMMAND_ID_HANDLER(ID_OPTIONS_AUDIOMUTE, OnToggleOptionsAudioMute)
    COMMAND_ID_HANDLER(ID_OPTIONS_PREFS, OnOptionsPrefs)

    COMMAND_RANGE_HANDLER(ID_TOOLBARUSER_FIRST, ID_TOOLBARUSER_LAST, OnToolbarUserItemSelected)

    // edit menu
    COMMAND_ID_HANDLER(ID_EDIT_CUT,OnEditCut)
    COMMAND_ID_HANDLER(ID_EDIT_COPY,OnEditCopy)
    COMMAND_ID_HANDLER(ID_EDIT_PASTE,OnEditPaste)
    COMMAND_ID_HANDLER(ID_EDIT_COPYBUFFER,OnEditCopyBuffer)
    COMMAND_ID_HANDLER(ID_EDIT_CLEARBUFFER,OnEditClearBuffer)

    // commands, only because of testxp's context menu handling
    COMMAND_RANGE_HANDLER(ID_EVENTLOGMENU_FIRST,ID_EVENTLOGMENU_LAST,OnEventLogCommands)
    COMMAND_RANGE_HANDLER(ID_TRANSFERSMENU_FIRST,ID_TRANSFERSMENU_LAST,OnTransfersCommands)
    COMMAND_RANGE_HANDLER(ID_SERVERLISTMENU_FIRST,ID_SERVERLISTMENU_LAST,OnServerListCommands)

    // need this one to forward the userlist command messages back to the right MDI child
    // if we don't do this they get sent to the active mdi window
    // by the macro CHAIN_MDI_CHILD_COMMANDS, the active mdi window
    // might not be the right one in the case of users
    // selecting items from the global user list while an MDI server window is active.
    COMMAND_RANGE_HANDLER(ID_USERLISTFIRST,ID_USERLISTLAST,OnUserListCommands)


    // systray icon
    TASKBAR_MESSAGE_HANDLER(m_TI, WM_LBUTTONUP, OnSystrayIconClick)

    // systray menu
    COMMAND_ID_HANDLER(ID_TASKBARMENU_RESTORE, OnTaskBarMenuRestore)

		CHAIN_MDI_CHILD_COMMANDS()
    CHAIN_MSG_MAP_MEMBER(m_TI)
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()

	BEGIN_UPDATE_UI_MAP(CMainFrame)
    UPDATE_ELEMENT(ID_VIEW_EVENTLOG, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
    UPDATE_ELEMENT(ID_VIEW_TRANSFERS, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
    UPDATE_ELEMENT(ID_VIEW_NOTICEQUEUE, UPDUI_MENUPOPUP)// | UPDUI_TOOLBAR)
    UPDATE_ELEMENT(ID_VIEW_PRIVMSGQUEUE, UPDUI_MENUPOPUP)// | UPDUI_TOOLBAR)
    UPDATE_ELEMENT(ID_VIEW_URLCATCHER, UPDUI_MENUPOPUP)// | UPDUI_TOOLBAR)
    UPDATE_ELEMENT(ID_VIEW_CHANNELMONITOR, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
    UPDATE_ELEMENT(ID_VIEW_SERVERMONITOR, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
    UPDATE_ELEMENT(ID_VIEW_OUTPUT, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
    UPDATE_ELEMENT(ID_VIEW_IRCSERVERLIST, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
    UPDATE_ELEMENT(ID_VIEW_GLOBALUSERLIST, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)

		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
    UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)

    UPDATE_ELEMENT(ID_EDIT_CUT, UPDUI_MENUPOPUP)
    UPDATE_ELEMENT(ID_EDIT_COPY, UPDUI_MENUPOPUP)
    UPDATE_ELEMENT(ID_EDIT_PASTE, UPDUI_MENUPOPUP)
    UPDATE_ELEMENT(ID_EDIT_COPYBUFFER, UPDUI_MENUPOPUP)
    UPDATE_ELEMENT(ID_EDIT_CLEARBUFFER, UPDUI_MENUPOPUP)

    UPDATE_ELEMENT(ID_FILE_CLOSE, UPDUI_MENUPOPUP)
    UPDATE_ELEMENT(ID_FILE_CONNECT, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
    UPDATE_ELEMENT(ID_FILE_DISCONNECT, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)

    UPDATE_ELEMENT(ID_CHANNEL_MONITOR, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
    UPDATE_ELEMENT(ID_CHANNEL_SHOWSERVER, UPDUI_MENUPOPUP)
    UPDATE_ELEMENT(ID_CHANNEL_PROPERTIES, UPDUI_MENUPOPUP)
    UPDATE_ELEMENT(ID_CHANNEL_CHANNELLIST, UPDUI_MENUPOPUP)

    UPDATE_ELEMENT(ID_OPTIONS_LOGGING, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
    UPDATE_ELEMENT(ID_OPTIONS_TIMESTAMPS, UPDUI_MENUPOPUP)
    UPDATE_ELEMENT(ID_OPTIONS_AUDIOMUTE, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
    UPDATE_ELEMENT(ID_OPTIONS_TOGGLEINFO, UPDUI_MENUPOPUP)
    UPDATE_ELEMENT(ID_OPTIONS_TOGGLEHEADER, UPDUI_MENUPOPUP)
    UPDATE_ELEMENT(ID_OPTIONS_TOGGLEUSERLIST, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
	END_UPDATE_UI_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

  // file menu
	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnFileCloseWindow(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnFileConnect(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnFileDisconnect(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

  // view menu
  LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnViewGuiLayout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);  

  // favorites menu
  LRESULT OnFavoritesOrganizeFavorites(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnFavoritesAddToFavorites(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnFavoritesItemSelected(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  
  // help menu
  LRESULT OnHelpAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnHelpHydraIRCOnTheWeb(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnHelpDonate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnHelpDocumentation(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnHelpOnlineDocumentation(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnHelpRecentChanges(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnHelpReportABug(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnHelpForums(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

  // window menu
	LRESULT OnWindowCascade(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnWindowTileHorz(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnWindowTileVert(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnWindowMinimizeAllWindows(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnWindowMinimizeActiveWindow(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

  // channel menu
  LRESULT OnChannelShowServer(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnChannelProperties(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnChannelChannelList(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

  // systray menu
  LRESULT OnTaskBarMenuRestore(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

  LRESULT OnToggleInfo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnToggleHeader(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnToggleUserList(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

  // channel related stuff
  LRESULT OnToggleChannelMonitor(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

  // server related stuff
  LRESULT OnToggleServerMonitor(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  
  // option menu
  LRESULT OnToggleOptionLogging(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnToggleOptionTimeStamps(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnToggleOptionsAudioMute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOptionsPrefs(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

  // Edit menu
  LRESULT OnEditCut(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnEditCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnEditPaste(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnEditClearBuffer(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);  
  LRESULT OnEditCopyBuffer(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);  

  // Toolbar user buttons
  LRESULT OnToolbarUserItemSelected(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

  // general stuff
  LRESULT OnEventLogCommands(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnTransfersCommands(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnServerListCommands(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnUserListCommands(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  


  LRESULT OnInitialize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
  LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
  LRESULT OnSystrayIconClick(LPARAM /*uMsg*/, BOOL& /*bHandled*/);
  LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnSettingChange(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnSysColorChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnRequestAutoTile(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
  LRESULT OnRequestScrollToEnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled);
  LRESULT OnChildFrameClosed(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled);
  
  LRESULT OnCopyData(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled);
	LRESULT onWhereAreYou(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) 
    { return WMU_WHERE_ARE_YOU;	}

  LRESULT OnEnLink(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
  void MDIActivate(HWND hWndChildToActivate);
};
