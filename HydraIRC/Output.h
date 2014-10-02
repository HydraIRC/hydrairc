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

// Output.h : interface of the COutputView class
//
/////////////////////////////////////////////////////////////////////////////

class COutputView :
	public CListener, 
  public dockwins::CBoxedDockingWindowImpl< COutputView,CWindow,dockwins::CVC7LikeExBoxedDockingWindowTraits>
{
	typedef COutputView	thisClass;
	typedef dockwins::CBoxedDockingWindowImpl< COutputView,CWindow,dockwins::CVC7LikeExBoxedDockingWindowTraits> baseClass;

protected:
	DWORD m_dwIcon;

private:

#ifdef USE_XPCOMMANDBAR
public:
  CMDICommandBarXPCtrl m_CmdBar; // just for the nice XP-Look menus.
#endif

public:
	COutputView(DWORD dwIcon = IDI_OUTPUT,CEventManager *pEventManager = NULL);

	CMsgView	m_MsgView;

  DECLARE_WND_CLASS(_T("COutputView"))

	virtual void OnFinalMessage(HWND /*hWnd*/);

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
    COMMAND_HANDLER(IDC_OUTPUT_CLEARBUFFER, BN_CLICKED, m_MsgView.OnBnClickedClearBuffer)
    COMMAND_HANDLER(IDC_OUTPUT_COPYBUFFER, BN_CLICKED, m_MsgView.OnBnClickedCopyBuffer)

		CHAIN_MSG_MAP(baseClass)

	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

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

  //-DOCK void OnDocked(HDOCKBAR hBar,bool bHorizontal);
	//-DOCK void OnUndocked(HDOCKBAR hBar);

  void UpdateSettings( void );

  void OnEvent(int EventID, void *pData);

};
