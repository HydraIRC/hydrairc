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

// IRC.h : interface of the CTextInputView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

// forward declares
class IRCServer;

class CTextInputView : 
   public CWindowImpl<CTextInputView, CRichEditCtrl>,
   public CRichEditCommands<CTextInputView>
{
public:
  DECLARE_WND_SUPERCLASS(NULL, CRichEditCtrl::GetWndClassName())

  // for the text history.
  InputHistoryManager m_History;
  char *m_UndoBuffer;

private:
  COLORREF m_TextColor;
  COLORREF m_BackColor;

public:
  CTextInputView( void )
  {
    m_History.SetMaxLines(INTPREF(PREF_nMaxInputHistoryLines));
    m_TextColor = COLORVAL(item_textinputnormaltext);
    m_BackColor = COLORVAL(item_textinputbackground);
  }

  ~CTextInputView( void )
  {
  }

  void SetColors( COLORREF *pColors );
  void UpdateSettings( void );

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CTextInputView)
    MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
    MESSAGE_HANDLER(WM_CHAR, OnChar)
    MESSAGE_HANDLER(WM_SETFOCUS, OnFocus)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)    
    MESSAGE_HANDLER(WM_PASTE, OnPaste)    
 		CHAIN_MSG_MAP_ALT(CRichEditCommands<CTextInputView>, 1)
	END_MSG_MAP()

  LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnPaste(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

  // TODO: InputHistoryBuffer m_History;
  HWND m_MessageWindow;
  void SetMessageWindow(HWND hWnd);
  short GetRequiredHeight( int WindowType );
};
