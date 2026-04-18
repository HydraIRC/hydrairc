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
