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

/////////////////////////////////////////////////////////////////////////////

#ifndef TCN_INITIALIZE
#define TCN_INITIALIZE TCN_FIRST-10
#define TCN_INSERTITEM TCN_FIRST-11
#define TCN_DELETEITEM TCN_FIRST-12
#endif // TCN_INITIALIZE

template <class T, class TBase = CWindow, class TWinTraits = CControlWinTraits>
class ATL_NO_VTABLE CDlgContainerImpl : 
   public CWindowImpl< T, TBase, TWinTraits >
{
public:
   CSimpleArray<HWND> m_aViews;
   int m_iCurPos;
   HWND m_hWndClient;

   CDlgContainerImpl()
      : m_iCurPos(-1), m_hWndClient(NULL)
   {
   }

   // Operations

   BOOL SubclassWindow(HWND hWnd)
   {
      ATLASSERT(m_hWnd == NULL);
      ATLASSERT(::IsWindow(hWnd));
      BOOL bRet = CWindowImpl< T, TBase, TWinTraits >::SubclassWindow(hWnd);
      if( bRet ) _Init();
      return bRet;
   }

   BOOL PreTranslateMessage(MSG* pMsg)
   {
      if( m_hWndClient ) {
         TCHAR szClassName[7];
   	   ::GetClassName(m_hWndClient, szClassName, 7);
         if( ::lstrcmp(_T("#32770"), szClassName)==0 ) return ::IsDialogMessage(m_hWndClient, pMsg);
      }
      return FALSE;
   }

   int AddItem(HWND hWnd)
   {
      ATLASSERT(::IsWindow(hWnd));
      // Add view
      m_aViews.Add(hWnd);
      CWindow wnd = hWnd;
      // Initially we hide the view
      wnd.ShowWindow(SW_HIDE);
      // FIX: Important; see Q149501
      wnd.ModifyStyleEx(0, WS_EX_CONTROLPARENT);
      // Notify owner
      NMHDR nmh = { m_hWnd, GetDlgCtrlID(), TCN_INSERTITEM };
      ::SendMessage(GetParent(), WM_NOTIFY, nmh.idFrom, (LPARAM)&nmh);
      // Return new position
      return m_aViews.GetSize()-1;
   }
   BOOL RemoveItem(HWND hWnd)
   {
      ATLASSERT(::IsWindow(hWnd));
      int iPos = m_aViews.Find(hWnd);
      return RemoveItem(iPos);
   }
   BOOL RemoveItem(int iPos)
   {
      if( iPos<0 || iPos>=m_aViews.GetSize() ) return NULL;
      // Notify owner
      NMHDR nmh = { m_hWnd, GetDlgCtrlID(), TCN_DELETEITEM };
      ::SendMessage(GetParent(), WM_NOTIFY, nmh.idFrom, (LPARAM)&nmh);
      // Remove view
      return m_aViews.RemoveAt(iPos);
   }
   HWND GetItem(int iPos) const
   {
      if( iPos<0 || iPos>=m_aViews.GetSize() ) return NULL;
      return m_aViews[iPos];
   }
   int GetItemCount() const
   {
      return m_aViews.GetSize();
   }
   BOOL SetCurSel(int iPos)
   {
      if( iPos<0 || iPos>=m_aViews.GetSize() ) return FALSE;
      if( iPos==m_iCurPos ) return TRUE; // Already selected
      // Ask user if it's OK to change selection...
      NMHDR nmh = { m_hWnd, GetDlgCtrlID(), TCN_SELCHANGING };
      LRESULT lRes = ::SendMessage(GetParent(), WM_NOTIFY, nmh.idFrom, (LPARAM)&nmh);
      if( lRes!=0 ) return FALSE; // User declined
      // Assign new state
      HWND hWndActive = GetItem(m_iCurPos);
      HWND hWndNew = GetItem(iPos);
      m_iCurPos = iPos;      
      m_hWndClient = hWndNew;
      // Hide old view
      if( hWndActive ) ::SetWindowPos(hWndActive, NULL, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_HIDEWINDOW);
      // Resize new view into place
      BOOL bDummy;
      OnSize(0, 0, 0, bDummy);
      if( hWndNew ) ::SetWindowPos(hWndNew, NULL, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
      // Send notification
      nmh.code = TCN_SELCHANGE;
      ::SendMessage(GetParent(), WM_NOTIFY, nmh.idFrom, (LPARAM)&nmh);
      return TRUE;
   }
   int GetCurSel() const
   {
      return m_iPos;
   }

   // Message map and handlers

   BEGIN_MSG_MAP(CDlgContainerImpl)
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
      MESSAGE_HANDLER(WM_SIZE, OnSize)
      CHAIN_CLIENT_COMMANDS()
   END_MSG_MAP()

   LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      _Init();
      return 0;
   }
   LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      return TRUE; // View fills entire client area
   }
   LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      if( m_hWndClient==NULL ) return 0;
      RECT rc;
      GetClientRect(&rc);
      ::SetWindowPos(m_hWndClient, NULL, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, SWP_NOACTIVATE | SWP_NOZORDER);
      return 0;
   }

   // Implementation

   void _Init()
   {
      // FIX: Important; see Q149501
      ModifyStyleEx(0, WS_EX_CONTROLPARENT);
      // This is a little WTL subclass helper notification
      NMHDR nmh = { m_hWnd, GetDlgCtrlID(), TCN_INITIALIZE };
      ::SendMessage(GetParent(), WM_NOTIFY, nmh.idFrom, (LPARAM)&nmh);
   }
};

class CDlgContainerCtrl : public CDlgContainerImpl<CDlgContainerCtrl>
{
public:
   DECLARE_WND_CLASS(_T("WTL_DlgContainer"))
};
