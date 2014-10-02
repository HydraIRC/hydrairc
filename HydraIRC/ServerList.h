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
