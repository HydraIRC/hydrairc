
template <class T, int C = RGB(128, 128, 128), class TBase = CMDIWindow, class TWinTraits = CMDIChildWinTraits>
class ATL_NO_VTABLE FixedMDIChildWindowImpl : public CMDIChildWindowImpl<T, TBase, TWinTraits> {
public:

 	typedef FixedMDIChildWindowImpl<T, C, TBase, TWinTraits> thisClass;
	typedef CMDIChildWindowImpl<T, TBase, TWinTraits> baseClass;
	BEGIN_MSG_MAP(thisClass)
		MESSAGE_HANDLER(WM_FORWARDMSG, onForwardMsg)
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()
		
	// Fix window maximization state issues...code taken from www.codeproject.com, article by david bowen
	HWND Create(HWND hWndParent, ATL::_U_RECT rect = NULL, LPCTSTR szWindowName = NULL,
	DWORD dwStyle = 0, DWORD dwExStyle = 0,
	UINT nMenuID = 0, LPVOID lpCreateParam = NULL)
	{
		// NOTE: hWndParent is going to become m_hWndMDIClient
		//  in CMDIChildWindowImpl::Create
		ATLASSERT(::IsWindow(hWndParent));
		
		BOOL bMaximized = BOOLPREF(PREF_bAutoMaximize); // set the default
    // if we don't always maximzie the window, then check to see if the current window is maximized and
    // then open the new one in the same state
    if (!BOOLPREF(PREF_bAutoMaximize))
		  ::SendMessage(hWndParent, WM_MDIGETACTIVE, 0, (LPARAM)&bMaximized);
			  
		if(bMaximized == TRUE) {
			::SendMessage(hWndParent, WM_SETREDRAW, FALSE, 0);
		}
		
		HWND hWnd = baseClass::Create(hWndParent, rect, szWindowName, dwStyle, dwExStyle, nMenuID, lpCreateParam);
		
		if(bMaximized == TRUE) {
			::ShowWindow(hWnd, SW_SHOWMAXIMIZED);
			
			::SendMessage(hWndParent, WM_SETREDRAW, TRUE, 0);
			::RedrawWindow(hWndParent, NULL, NULL,
				RDW_INVALIDATE | RDW_ALLCHILDREN);
		}
		
		return hWnd;
	}
	
  LRESULT onForwardMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) 
  {
		return baseClass::PreTranslateMessage((LPMSG)lParam);
	}
};
