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

// CTextQueueFrm.h : interface of the CTextQueueFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#define IDC_TEXTQUEUE_LIST 999

class TextQueueManager;
class TextQueueItem;

class CTextQueueView :
  public CListener, 
	public dockwins::CBoxedDockingWindowImpl< CTextQueueView,CWindow,dockwins::CVC7LikeExBoxedDockingWindowTraits>
{
	typedef CTextQueueView	thisClass;
	typedef dockwins::CBoxedDockingWindowImpl< CTextQueueView,CWindow,dockwins::CVC7LikeExBoxedDockingWindowTraits> baseClass;

protected:
	DWORD m_dwIcon;

public:
  CListViewCtrl m_TransfersListCtrl;
  TextQueueManager *m_pTextQueueMgr;
 
  CTextQueueView::CTextQueueView( TextQueueManager *pTextQueueMgr, DWORD dwIcon, CEventManager *pEventManager = NULL);
  ~CTextQueueView( void );

  void AddItem(TextQueueItem *pTQI, BOOL AutoSize = TRUE);
  void UpdateItem(TextQueueItem *pTQI);
  void RemoveItem(TextQueueItem *pTQI);
  void AutoSizeColumns( void );

  void UpdateSettings( void );
  void OnEvent(int EventID, void *pData);


	BEGIN_MSG_MAP(CTextQueueView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
    MESSAGE_HANDLER(WM_SIZE, OnSize)
    NOTIFY_CODE_HANDLER(NM_CUSTOMDRAW, OnNmCustomDraw);
    NOTIFY_CODE_HANDLER(LVN_KEYDOWN, OnLvnKeyDown)
    NOTIFY_CODE_HANDLER(NM_DBLCLK, OnNmDblClick)
    MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
    CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()

  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
  LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);

  LRESULT OnLvnKeyDown(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled);
  LRESULT OnNmDblClick(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);

  LRESULT OnNmCustomDraw(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

  LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
  {
    MINMAXINFO *MMI = (MINMAXINFO*)lParam;
    MMI->ptMinTrackSize.x=100;
    MMI->ptMinTrackSize.y=50;
    return 0;
  }

};
