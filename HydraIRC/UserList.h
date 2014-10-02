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
