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

// CUserListView.h
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(CUserListView_H_INCLUDED_)
#define CUserListView_H_INCLUDED_

#pragma once

// Forward declares
class IRCUser;
class IRCUserGroupList;
class IRCChannel;
class IRCQuery;
class IRCServer;

// Declaration

class CUserListView :
	public CListener, 
	public dockwins::CBoxedDockingWindowImpl< CUserListView,CWindow,dockwins::CVC7LikeBoxedDockingWindowTraits>
{
	typedef CUserListView	thisClass;
	typedef dockwins::CBoxedDockingWindowImpl< CUserListView,CWindow,dockwins::CVC7LikeBoxedDockingWindowTraits> baseClass;

protected:
	DWORD m_dwIcon;

private:
  CSimpleArray <TreeItemInfo *> m_TIIList; // keeps track of all TreeItemInfo objects.
  IRCUserGroupList m_UserGroups;

  COLORREF m_TextColor;
  COLORREF m_BackColor;
  COLORREF m_SelectedBackColor;

  BOOL m_ShowBuddyGroups;

public:
  int m_Width; // store the width here TODO: move to protected

  CTreeViewCtrl	m_tree;
  IRCChannel    *m_pChannel; // so we can get the users for the list!
  int m_ObjectType;

  CUserListView(IRCChannel *pChannel, DWORD dwIcon = IDR_MAINFRAME,CEventManager *pEventManager = NULL)
		: m_dwIcon(dwIcon),
      CListener(pEventManager),
      m_pChannel(pChannel),
      m_ShowBuddyGroups(TRUE)
  {
    m_ObjectType = OTYPE_GLOBALUSERLIST;
    if (!g_pPrefs)
      ATLASSERT(0);

    m_TextColor = COLORVAL(item_userlistnormaltext);
    m_BackColor = COLORVAL(item_userlistbackground);
    m_SelectedBackColor = COLORVAL(item_userlistselectedbackground);
  }
  ~CUserListView( void );

  DECLARE_WND_CLASS(_T("CUserListView"))


	BEGIN_MSG_MAP(thisClass)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
    MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
    MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)

    NOTIFY_CODE_HANDLER(NM_CUSTOMDRAW, OnNmCustomDraw)
    NOTIFY_CODE_HANDLER(TVN_GETINFOTIP, OnTVNGetInfoTip)
    NOTIFY_CODE_HANDLER(TVN_SELCHANGED, OnTVNSelectChanged)
    NOTIFY_CODE_HANDLER(TVN_ITEMEXPANDED, OnTVNItemExpanded)
    NOTIFY_CODE_HANDLER(NM_DBLCLK, OnNMDblclk)
    COMMAND_RANGE_HANDLER(ID_USERLISTFIRST,ID_USERLISTLAST,OnUserListContext)
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()

  //void OnDocked(HDOCKBAR hBar,bool bHorizontal);
	//void OnUndocked(HDOCKBAR hBar);

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
  LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
  {
    MINMAXINFO *MMI = (MINMAXINFO*)lParam;
    MMI->ptMinTrackSize.x=50;
    MMI->ptMinTrackSize.y=50;
    return 0;
  }

  LRESULT OnContextMenu(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTVNGetInfoTip(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled);
  LRESULT OnTVNSelectChanged(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled);
  LRESULT OnTVNItemExpanded(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled);
  LRESULT OnNMDblclk(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
  LRESULT OnNmCustomDraw(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
  LRESULT OnUserListContext(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

  HTREEITEM Find(int GroupID);
  //HTREEITEM Find(char *Nick, int UserType); // quicker if you know the user type.
  //HTREEITEM Find(char *Nick);
  HTREEITEM Find(IRCUser *pUser, int UserType);
  HTREEITEM Find(IRCUser *pUser);
  HTREEITEM Add(IRCUser *User);
  void Remove(IRCUser *User);
  void RemoveAll( void );
  void Update(IRCUser *pUser); // this just Remove()'s and Add()'s them
  void Refresh( void ); // refreshes tree and moves users to appropriate groups

  void Select(IRCUser *pUser);

  void UpdateSettings( void );
  void SetColors( COLORREF *pColors );

  void OnEvent(int EventID, void *pData);

  // TODO: replace m_pChannel and pChannel with a userlist management class
  void SetSource( IRCChannel *pChannel) { m_pChannel = pChannel; }

  void SetShowBuddyGroups(BOOL ShowBuddyGroups);
  BOOL GetShowBuddyGroups() { return m_ShowBuddyGroups; }
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(CUserListView_H_INCLUDED_)
