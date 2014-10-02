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

#include "stdafx.h"
#include "HydraIRC.h"

// CInfoView

BOOL CInfoView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}


LRESULT CInfoView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
  bHandled = FALSE;
  SetAutoURLDetect(TRUE);
  SetEventMask(ENM_MOUSEEVENTS | ENM_KEYEVENTS | ENM_SELCHANGE | ENM_LINK | ENM_SCROLL | ENM_SCROLLEVENTS);
  UpdateSettings();
  return 0;
}

void CInfoView::Put(CString Message)
{
  SetSel(0,-1);
  ReplaceSel(Message);
  SetSel(-1,-1); 
  SendMessage(WM_VSCROLL, SB_TOP, NULL);
}

LRESULT CInfoView::OnMsgFilter(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
  bHandled = FALSE;
  MSGFILTER *MsgFilter = (MSGFILTER *)pnmh;  

  switch (MsgFilter->msg)
  {
    case WM_LBUTTONUP:
      {
        if (pnmh->hwndFrom == m_hWnd)
        {
          CHARRANGE cr;
          GetSel(cr);
          if (cr.cpMin != cr.cpMax)
          {
            SendMessage(WM_COPY,0,0);
          }
          SetSel(cr.cpMax,cr.cpMax);
          Put("Copied selection to clipboard\n");
        }        
      }  
      break;
#ifdef DEBUG
    default:
      {
        CString Message;
        Message.Format("child message: %d - %d\n",idCtrl,pnmh->code);        
        OutputDebugString(Message);
        Message.Format("msg: %d - %x\n",MsgFilter->msg, MsgFilter->msg);
        OutputDebugString(Message);
      }
#endif
  }
  return 0;// TODO: Check
}

// Call this function after you create the window or after an prefs update
void CInfoView::UpdateSettings( void )
{
  
  SetFont(GetAppFont(PREF_fInfoFont)); // TODO: new font pref needed
  SetBackgroundColor(m_BackColor);

  SetSel(0,-1);
  CHARFORMAT2 fmt;
  GetDefaultCharFormat(fmt);  
  fmt.dwEffects = 0;
  fmt.dwMask = CFM_COLOR | CFM_BACKCOLOR;
  fmt.crTextColor = m_TextColor;
	fmt.crBackColor = m_BackColor;
  fmt.cbSize = sizeof(CHARFORMAT2);
  SetSelectionCharFormat(fmt);
}

// call UpdateSettings() to have the changes take effect.
void CInfoView::SetColors( COLORREF *pColors )
{  
  // extract just the colors we need from the array of colors.
  m_TextColor = pColors[item_infotext - PREF_COLOR_FIRST];
  m_BackColor = pColors[item_infobackground - PREF_COLOR_FIRST];
}
