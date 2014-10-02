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

// ChannelMonitorView.h : interface of the CChannelMonitorView class
//
/////////////////////////////////////////////////////////////////////////////

// forward declare
class IRCChannel;
class CMainFrame;

class CChannelMonitorView :
	public CListener, 
  public dockwins::CBoxedDockingWindowImpl< CChannelMonitorView,CWindow,dockwins::CVC7LikeExBoxedDockingWindowTraits>
//  public dockwins::CBoxedDockingWindowImpl< CChannelMonitorView,CWindow,dockwins::COutlookLikeExBoxedDockingWindowTraits>

{
	typedef CChannelMonitorView	thisClass;
	typedef dockwins::CBoxedDockingWindowImpl< CChannelMonitorView,CWindow,dockwins::CVC7LikeExBoxedDockingWindowTraits> baseClass;
//	typedef dockwins::CBoxedDockingWindowImpl< CChannelMonitorView,CWindow,dockwins::COutlookLikeExBoxedDockingWindowTraits> baseClass;

protected:
	DWORD m_dwIcon;

private:
  void Message(IRCChannel *pChannel, char *Buffer);

#ifdef USE_XPCOMMANDBAR
public:
  CMDICommandBarXPCtrl m_CmdBar; // just for the nice XP-Look menus.
#endif

public:
	CChannelMonitorView(DWORD dwIcon = IDR_MAINFRAME,CEventManager *pEventManager = NULL)
		: m_dwIcon(dwIcon),
      CListener(pEventManager)
	{
	}

  DECLARE_WND_CLASS(_T("CChannelMonitorView"))

	BEGIN_MSG_MAP(thisClass)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
    NOTIFY_CODE_HANDLER(EN_MSGFILTER, m_MsgView.OnMsgFilter) // for text selections
    NOTIFY_CODE_HANDLER(EN_LINK,      ::OnEnLink)  // for doubleclickable url's

    // context menu
#ifdef USE_XPCOMMANDBAR
    MESSAGE_HANDLER(WM_MSGVIEWCONTEXTMENU, OnMsgViewContextMenu);
#endif
    COMMAND_HANDLER(IDC_CHANNELMONITOR_CLEARBUFFER, BN_CLICKED, m_MsgView.OnBnClickedClearBuffer)
    COMMAND_HANDLER(IDC_CHANNELMONITOR_COPYBUFFER, BN_CLICKED, m_MsgView.OnBnClickedCopyBuffer)
    COMMAND_HANDLER(IDC_CHANNELMONITOR_STOPALLMONITORING, BN_CLICKED, OnBnClickedStopAllMonitoring)

		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()

  CMsgView	m_MsgView;
  CSimpleArray<IRCChannel *> m_ChannelList;

  BOOL IsMonitoring(IRCChannel *pChannel);
  void AddChannel(IRCChannel *pChannel);
  void RemoveChannel(IRCChannel *pChannel);
  void Put(IRCChannel *pChannel, BufferItem *pBI);
  
// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

  LRESULT OnBnClickedStopAllMonitoring(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

#ifdef USE_XPCOMMANDBAR
  LRESULT OnMsgViewContextMenu(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
#endif

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
  LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
  {
    MINMAXINFO *MMI = (MINMAXINFO*)lParam;
    MMI->ptMinTrackSize.x=100;
    MMI->ptMinTrackSize.y=50;
    return 0;
  }
  
  void OnDocked(HDOCKBAR hBar,bool bHorizontal);
	void OnUndocked(HDOCKBAR hBar);
  void UpdateSettings( void );
  void OnEvent(int EventID, void *pData);
};
