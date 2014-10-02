// Original Code by Nenad Stefanovic [nenads@windows.microsoft.com] from the WTL Mailing List
// Revision 1 - 16/12/2003

class CFixedMDICommandBarCtrl : 
        public CMDICommandBarCtrlImpl<CFixedMDICommandBarCtrl>
{
public:
        DECLARE_WND_SUPERCLASS(_T("WTL_FixedMDICommandBar"),GetWndClassName())

        typedef CMDICommandBarCtrlImpl<CFixedMDICommandBarCtrl> _baseClass;
        typedef CCommandBarCtrlImpl<CFixedMDICommandBarCtrl> _baseBaseClass;

        BEGIN_MSG_MAP(CFixedMDICommandBarCtrl)
                CHAIN_MSG_MAP(_baseClass)
        ALT_MSG_MAP(1)   // Parent window messages
                CHAIN_MSG_MAP_ALT(_baseClass, 1)
        ALT_MSG_MAP(2)   // MDI client window messages
                CHAIN_MSG_MAP_ALT(_baseClass, 2)
        ALT_MSG_MAP(3)   // Message hook messages
                MESSAGE_RANGE_HANDLER(0, 0xFFFF, OnAllHookMessages)
                CHAIN_MSG_MAP_ALT(_baseBaseClass, 3)
        END_MSG_MAP()

        LRESULT OnAllHookMessages(UINT uMsg, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
        {
                bHandled = FALSE;
                if(uMsg == WM_MDIGETACTIVE || uMsg == WM_MDISETMENU)
                        return 1;

                BOOL bMaximized = FALSE;
                HWND hWndChild = (HWND)::SendMessage(m_wndMDIClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMaximized);
                bool bMaxOld = m_bChildMaximized;
                m_bChildMaximized = (hWndChild != NULL && bMaximized);
                HICON hIconOld = m_hIconChildMaximized;

                if(m_bChildMaximized)
                {
                        if(m_hWndChildMaximized != hWndChild)
                        {
                                ATL::CWindow wnd = m_hWndChildMaximized = hWndChild;
                                m_hIconChildMaximized = wnd.GetIcon(FALSE);
                                if(m_hIconChildMaximized == NULL)
                                {
                                        m_hIconChildMaximized = wnd.GetIcon(TRUE);
                                        if(m_hIconChildMaximized == NULL)
                                        {
                                                // no icon set with WM_SETICON, get the class one // need conditional code because types don't match in winuser.h 
#ifdef _WIN64
                                                m_hIconChildMaximized = (HICON)::GetClassLongPtr(wnd, GCLP_HICONSM); 
#else
                                                m_hIconChildMaximized = (HICON)LongToHandle(::GetClassLongPtr(wnd, GCLP_HICONSM)); 
#endif
                                        }
                                }
                        }
                }
                else
                {
                        m_hWndChildMaximized = NULL;
                        m_hIconChildMaximized = NULL;
                }

/*fix*/                if(bMaxOld != m_bChildMaximized)
                {
#ifdef _CMDBAR_EXTRA_TRACE
                        ATLTRACE2(atlTraceUI, 0, "MDI CmdBar - All messages hook change: m_bChildMaximized = %s\n", m_bChildMaximized ? "true" : "false");
#endif
                        // assuming we are in a rebar, change our size to accomodate new state
                        // we hope that if we are not in a rebar, nCount will be 0
                        int nCount = (int)::SendMessage(GetParent(), RB_GETBANDCOUNT, 0, 0L);
                        int cxDiff = (m_bChildMaximized ? 1 : -1) * (m_cxLeft + m_cxRight);
                        for(int i = 0; i < nCount; i++)
                        {
#if (_WIN32_IE >= 0x0500)
                                REBARBANDINFO rbi = { sizeof(REBARBANDINFO), RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_STYLE };
                                ::SendMessage(GetParent(), RB_GETBANDINFO, i, (LPARAM)&rbi);
                                if(rbi.hwndChild == m_hWnd)
                                {
                                        if((rbi.fStyle &
RBBS_USECHEVRON) != 0)
                                        {
                                                rbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE;
                                                rbi.cxMinChild += cxDiff;
                                                rbi.cxIdeal += cxDiff;

                                                ::SendMessage(GetParent(), RB_SETBANDINFO, i, (LPARAM)&rbi);
                                        }
                                        break;
                                }
#elif (_WIN32_IE >= 0x0400)
                                REBARBANDINFO rbi = { sizeof(REBARBANDINFO), RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_IDEALSIZE };
                                ::SendMessage(GetParent(), RB_GETBANDINFO, i, (LPARAM)&rbi);
                                if(rbi.hwndChild == m_hWnd)
                                {
                                        rbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE;
                                        rbi.cxMinChild += cxDiff;
                                        rbi.cxIdeal += cxDiff;
                                        ::SendMessage(GetParent(), RB_SETBANDINFO, i, (LPARAM)&rbi);
                                        break;
                                }
#else //(_WIN32_IE < 0x0400)
                                REBARBANDINFO rbi = { sizeof(REBARBANDINFO), RBBIM_CHILD | RBBIM_CHILDSIZE };
                                ::SendMessage(GetParent(), RB_GETBANDINFO, i, (LPARAM)&rbi);
                                if(rbi.hwndChild == m_hWnd)
                                {
                                        rbi.fMask = RBBIM_CHILDSIZE;
                                        rbi.cxMinChild += cxDiff;
                                        ::SendMessage(GetParent(), RB_SETBANDINFO, i, (LPARAM)&rbi);
                                        break;
                                }
#endif //(_WIN32_IE < 0x0400)
                       }
/*fix*/                }
/*fix*/                if(bMaxOld != m_bChildMaximized || hIconOld != m_hIconChildMaximized)
/*fix*/                {
                        // force size change and redraw everything
                        RECT rect = { 0 };
                        GetWindowRect(&rect);
                        ::MapWindowPoints(NULL, GetParent(), (LPPOINT)&rect, 2);
                        SetRedraw(FALSE);
                        SetWindowPos(NULL, 0, 0, 1, 1, SWP_NOZORDER | SWP_NOMOVE);
                        SetWindowPos(NULL, &rect, SWP_NOZORDER | SWP_NOMOVE);
                        SetRedraw(TRUE);
                        RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW);
                }

                return 1;
        }
};
