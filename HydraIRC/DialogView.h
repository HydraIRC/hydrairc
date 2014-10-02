#if !defined(AFX_DIALOGVIEW_H__20010419_45AA_056A_8DC7_0080AD509054__INCLUDED_)
#define AFX_DIALOGVIEW_H__20010419_45AA_056A_8DC7_0080AD509054__INCLUDED_

#pragma once


template<class T, class TBase = CWindow>
class CDialogView : public CDialogImpl<T, TBase>
{
public:
   HWND m_hWndFocus;

   BEGIN_MSG_MAP(CDialogView)
      MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
      MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
      MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
   END_MSG_MAP()

   LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
   {
      ATLASSERT(GetStyle() & WS_CHILD);
      ModifyStyle(WS_BORDER|WS_CAPTION, 0);
      ShowWindow(SW_NORMAL);
      bHandled = FALSE;
      return 0;
   }

   LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      HWND hWndFocus = ::GetFocus();
      if( hWndFocus!=NULL && IsChild(hWndFocus) ) m_hWndFocus = hWndFocus;
      return 0;
   }

   LRESULT OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
   {
      // Set focus to last known control
      if( !::IsWindow(m_hWndFocus) || !IsChild(m_hWndFocus) ) {
         bHandled = FALSE;
         return 0;
      }
      ::SetFocus(m_hWndFocus);
      return 0;
   }

   virtual BOOL PreTranslateMessage(MSG* pMsg)
   {
      HWND hWndFocus = ::GetFocus();
      if( hWndFocus==NULL || !IsChild(hWndFocus) ) return FALSE;
      if( GetWindow(GW_CHILD)==NULL ) return FALSE;      
      if( IsDialogMessage(pMsg) ) return TRUE;
      return FALSE;
   }
};

#endif // !defined(AFX_DIALOGVIEW_H__20010419_45AA_056A_8DC7_0080AD509054__INCLUDED_)

