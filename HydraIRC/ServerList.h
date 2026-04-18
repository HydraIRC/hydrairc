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

// CServerListView.h
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(CServerListView_H_INCLUDED_)
#define CServerListView_H_INCLUDED_

#pragma once

class IRCQuery;
class IRCServer;

enum ServerListMenuIDs
{
  SLMI_Connect = ID_SERVERLISTMENU_FIRST,
  SLMI_Reconnect,
  SLMI_Disconnect,
  SLMI_ListChannels,
  SLMI_Close,
  SLMI_AddToFavorites,
  SLMI_Cycle,
  SLMI_Logging,
  SLMI_Monitoring,
  SLMI_Properties
};

class CServerListView :
	public CListener, 
  public dockwins::CBoxedDockingWindowImpl< CServerListView,CWindow,dockwins::CVC7LikeBoxedDockingWindowTraits>
{
	typedef CServerListView	thisClass;
	typedef dockwins::CBoxedDockingWindowImpl< CServerListView,CWindow,dockwins::CVC7LikeBoxedDockingWindowTraits> baseClass;

protected:
	DWORD m_dwIcon;
  CToolTipCtrl m_pToolTip;

private:
  BOOL m_Dirty;
  TreeItemInfo *m_SelectedTreeItem; // used when dealing with context menus, must be checked for validity before using.

public:
  CSimpleArray<TreeItemInfo*> m_TIIList;
	CTreeViewCtrl	m_tree;
	CImageList		m_images;

	CServerListView(DWORD dwIcon = IDR_MAINFRAME)
		:m_dwIcon(dwIcon)
	{
    CListener::Start(&g_EventManager);
    m_Dirty = FALSE;
    m_SelectedTreeItem = NULL;
	}
  ~CServerListView( void );
  void UpdateSettings( void );

  DECLARE_WND_CLASS(_T("CServerListView"))

	BEGIN_MSG_MAP(thisClass)
    //sys_Printf(BIC_GUI,"message: %x - code: %x\n",uMsg, uMsg == WM_NOTIFY ? ((LPNMHDR)lParam)->code : 0);
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
    MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu);
    NOTIFY_CODE_HANDLER(NM_SETFOCUS, OnNmSetFocus);
    NOTIFY_CODE_HANDLER(NM_CLICK, OnNmClick);
    NOTIFY_CODE_HANDLER(NM_CUSTOMDRAW, OnNmCustomDraw);
    NOTIFY_CODE_HANDLER(TVN_GETINFOTIP, OnTvnGetInfoTip);
    COMMAND_RANGE_HANDLER(ID_SERVERLISTMENU_FIRST, ID_SERVERLISTMENU_LAST, OnMenuItemSelected)
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()

  void OnDocked(HDOCKBAR hBar,bool bHorizontal);
	void OnUndocked(HDOCKBAR hBar);
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
  LRESULT OnContextMenu(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTvnGetInfoTip(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled);
  LRESULT OnNmClick(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled);
  LRESULT OnNmSetFocus(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled);
  LRESULT OnNmCustomDraw(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
  LRESULT OnMenuItemSelected(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

  //woohoo overloading...

  void Add(IRCServer *pServer);
  void Add(IRCChannel *pChannel);
  void Add(IRCQuery *pQuery);

  void Remove(IRCServer *pServer);
  void Remove(IRCChannel *pChannel);
  void Remove(IRCQuery *pQuery);
  
  void Select(IRCServer *pServer);
  void Select(IRCChannel *pChannel);
  void Select(IRCQuery *pQuery);

  void Update(IRCServer *pServer);
  void Update(IRCQuery *pQuery);
  void Update(IRCChannel *pChannel);

  // from CListener
  void OnEvent(int EventID, void *pData);

private:
  void AddItem(int ItemType, void *pItemData);
  HTREEITEM FindItem(int ItemType, void *pItemData);
  void RemoveItem(int ItemType, void *pItemData);
  void UpdateItem(int ItemType, void *pItemData);

  //HTREEITEM Find(IRCServer *pServer);
  //HTREEITEM Find(IRCChannel *pChannel);
  void OnItemClicked(HTREEITEM hItem);

  void CheckDirtyStatus( void );
  void SetDirty( void ) { m_Dirty = TRUE; }

  void DoAction( int Action );

  BOOL IsValidTreeItem(TreeItemInfo *pTII)
  {
    return (BOOL)(m_TIIList.Find(pTII) >= 0);
  }
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(CServerListView_H_INCLUDED_)
