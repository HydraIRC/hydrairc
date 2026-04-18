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
