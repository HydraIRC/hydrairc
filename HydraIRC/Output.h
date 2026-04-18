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
