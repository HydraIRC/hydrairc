/////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2001-2002 by David Yuheng Zhao
//
// Distribute and change freely, except: don't remove my name from the source 
//
// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// Partly based on a freeware MFC version from Dundas Software available at
// http://www.codeproject.com/docview/tabbedmdi.asp
//
// If you have any questions, I can be reached as follows:
//	yuheng_zhao@yahoo.com
//
/////////////////////////////////////////////////////////////////////////////
#ifndef __ATLMDI_TAB_H__
#define __ATLMDI_TAB_H__

#pragma once

namespace WTL
{
#define MDI_TABDEF_STYLE		TCS_RIGHTJUSTIFY
#define MDI_TAB_TIMER			9771

template<class TMDIWindow>
class CMDITabImpl : public CWindowImpl<CMDITabImpl,	
	CWindow, CWinTraits < WS_CHILD | WS_VISIBLE, 0> >
{
public:
	CMDITabImpl()
	{
		m_pParentFrame = NULL;
		m_bTimerDisabled = false;
	}
	~CMDITabImpl()
	{
		if (IsAttached())
			Detach();
	}
	inline BOOL IsAttached()
	{
		return m_tabctrl.IsWindow();
	}

	BOOL Attach(TMDIWindow* pFrame, DWORD dwStyle = MDI_TABDEF_STYLE)
	{
		if (IsAttached())
			return FALSE;

		m_pParentFrame = pFrame;

		if(!SubclassWindow(pFrame->m_hWndMDIClient))
		{
			ATLTRACE(_T("CMDITabImpl::Attach: failed to subclass MDI Client\n"));
			return FALSE;
		}
		
		dwStyle &= ~WS_POPUP;
		dwStyle |= TCS_FOCUSNEVER|WS_VISIBLE|WS_CHILD;
		RECT rc;
		memset(&rc, 0, sizeof(rc));
		m_tabctrl.m_pTabImpl = this;
		if(!m_tabctrl.Create(m_pParentFrame->m_hWnd, rc, NULL, dwStyle))
		{
			ATLTRACE(_T("CMDITabImpl::Attach: failed to create tab control\n"));
			return FALSE;
		}
		m_tabctrl.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT), FALSE);

		UpdateContents();

		SetTimer(MDI_TAB_TIMER, 500);
		return IsAttached();
	}
	
	void Detach()
	{
		KillTimer(MDI_TAB_TIMER);
		m_tabctrl.DestroyWindow();
		UnsubclassWindow();
		m_tabmap.RemoveAll();
		if (m_pParentFrame)
			m_pParentFrame->UpdateWindow();
	}
	
	void UpdateTabLayout(HWND hwndClient, RECT& rect)
	{
		if (m_tabctrl.IsWindow())
		{
			m_tabctrl.SetWindowPos(NULL, rect.left, rect.top, 
				rect.right-rect.left, rect.bottom-rect.top, SWP_NOZORDER); 
			m_tabctrl.AdjustRect(FALSE, &rect);
		}
		
		::SetWindowPos(hwndClient, NULL, rect.left, rect.top,
			rect.right - rect.left, rect.bottom - rect.top,
			SWP_NOZORDER | SWP_NOACTIVATE);
	}
	
	void UpdateContents()
	{
		if (!IsAttached())
			return;
		
		HWND hwndActive = m_pParentFrame->MDIGetActive();
		HWND hwndChild = ::GetWindow(hwndActive, GW_HWNDFIRST);
		while(hwndChild!=NULL)
		{
			if (m_tabmap.Lookup(hwndChild) == NULL)
				AddTab(hwndChild);
			else
			{
				if (hwndChild == hwndActive)
				{
					int pos = m_tabctrl.GetCurSel();
					HWND hwndCurrent = FindHwnd(pos);

					if (hwndCurrent!=hwndChild)
					{
						if (hwndCurrent!=NULL)
							UpdateTab(hwndCurrent, pos);
						UpdateTab(hwndChild, -1);
					}else
					{
						//UpdateTab(hwndCurrent, pos);
					}
				}
			}

			hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);
		}
	}
	
	void AddTab(HWND hwnd)
	{
		if (!IsAttached())
			return;
		TCITEM item;
		item.mask = TCIF_PARAM;
		item.lParam = (LPARAM)hwnd;
		int pos = m_tabctrl.GetItemCount();
		
		m_tabctrl.InsertItem(pos, &item);
		m_tabmap.Add(hwnd, 1);
		UpdateTab(hwnd, pos);

		if (m_tabctrl.GetItemCount()==1)
			m_pParentFrame->UpdateLayout(FALSE);
	}
public:
	void RemoveTab(HWND hwnd)
	{
		if (!IsAttached())
			return;
		TCITEM item;
		item.mask = TCIF_PARAM;

		int count = m_tabctrl.GetItemCount();
		for (int i=0; i<count; i++)
		{
			m_tabctrl.GetItem(i, &item);
			if (item.lParam == (LPARAM)hwnd)
			{
				m_tabctrl.DeleteItem(i);
				m_tabmap.Remove(hwnd);
				if (m_tabctrl.GetItemCount()==0)
					m_pParentFrame->UpdateLayout(FALSE);
				return;
			}
		}
	}
	
	void UpdateTab(HWND hwnd, int pos = -1)
	{
		if (!IsAttached())
			return;
		if (!::IsWindow(hwnd))  return;
		if (pos == -1) pos = FindTab(hwnd);
		if (pos == -1)
		{
			AddTab(hwnd);
			return;
		}
		TCHAR szText[MAX_PATH];
		TCITEM item;
		item.mask = TCIF_TEXT|TCIF_PARAM;
		item.pszText = szText;
		item.cchTextMax = MAX_PATH;
		
		m_tabctrl.GetItem(pos, &item);
		if (item.lParam == (LPARAM)hwnd)
		{
			TCHAR szWinText[MAX_PATH];
			::GetWindowText(hwnd, szWinText, MAX_PATH);
			TCITEM item;
			item.mask = TCIF_TEXT;
			item.pszText = szWinText;				
			m_tabctrl.SetItem(pos, &item);
			
			HWND hwndActive = m_pParentFrame->MDIGetActive();
			if (hwnd == hwndActive)
				m_tabctrl.SetCurSel(pos);
			
			return;
		}
	}
	
	int FindTab(HWND hwnd)
	{
		TCITEM item;
		item.mask = TCIF_PARAM;
		int count = m_tabctrl.GetItemCount();
		for (int i=0; i<count; i++)
		{
			m_tabctrl.GetItem(i, &item);
			if (item.lParam == (LPARAM)hwnd)
				return i;
		}
		return -1;
	}
	
	HWND FindHwnd(int i)
	{
		if (i<0)
			return NULL;
		TCITEM item;
		item.mask = TCIF_PARAM;
		item.lParam = NULL;
		m_tabctrl.GetItem(i, &item);
		return (HWND)item.lParam;
	}
	
public:
	BEGIN_MSG_MAP(CMDITabImpl)
		MESSAGE_HANDLER(WM_MDIACTIVATE, OnMDIActivate)
		MESSAGE_HANDLER(WM_MDIDESTROY, OnMDIDestroy)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
	END_MSG_MAP()	
		
	LRESULT OnMDIActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LRESULT res = DefWindowProc(uMsg, wParam, lParam);
		PostMessage(WM_TIMER,MDI_TAB_TIMER);
		return res;
	}
	
	LRESULT OnMDIDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		RemoveTab((HWND)wParam);
		LRESULT res = DefWindowProc(uMsg, wParam, lParam);
		HWND hwndActive = m_pParentFrame->MDIGetActive();
		if (hwndActive)
			UpdateTab(hwndActive);
		return res;
		
	}
	
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (wParam == MDI_TAB_TIMER && !m_bTimerDisabled)
			UpdateContents();
		return DefWindowProc(uMsg, wParam, lParam);
	}
private:
	class CMDITabCtrl :	public CWindowImpl<CMDITabCtrl, CTabCtrl>
	{	
	public:
		CMDITabImpl* m_pTabImpl;
		
		DECLARE_WND_SUPERCLASS( _T("MDITabCtrl"), CTabCtrl::GetWndClassName())	
		BEGIN_MSG_MAP(CMDITabCtrl)
			MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
			MESSAGE_HANDLER(WM_RBUTTONDOWN, OnRButtonDown)
		END_MSG_MAP()
		LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			m_pTabImpl->DisableTimer();
			LRESULT res = DefWindowProc(uMsg, wParam, lParam);
			TCHITTESTINFO hci;
			hci.pt.x = GET_X_LPARAM(lParam);
			hci.pt.y = GET_Y_LPARAM(lParam);
			hci.flags = TCHT_ONITEM;

			int pos = HitTest(&hci);
			if (pos>=0)
			{
				HWND hwnd = m_pTabImpl->FindHwnd(pos);
				HWND hwndActive = m_pTabImpl->MDIGetActive();
				if (hwnd!=hwndActive)
					m_pTabImpl->MDIActivate(hwnd);
			}
			m_pTabImpl->EnableTimer();
			return res;
		}

		LRESULT OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			LRESULT res = DefWindowProc(uMsg, wParam, lParam);
			return res;
		}
	};
public:
	HWND MDIGetActive()
	{
		return m_pParentFrame->MDIGetActive();
	}

	void MDIActivate(HWND hwnd)
	{
		m_pParentFrame->MDIActivate(hwnd);
	}
	void DisableTimer()
	{
		m_bTimerDisabled = true;
	}
	void EnableTimer()
	{
		m_bTimerDisabled = false;
		PostMessage(WM_TIMER,MDI_TAB_TIMER);
	}
private:
	bool m_bTimerDisabled;
	TMDIWindow* m_pParentFrame;
	CMDITabCtrl m_tabctrl;
	CSimpleMap<HWND, int> m_tabmap;
};
}
#endif