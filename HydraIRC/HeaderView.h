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

#pragma once

class CIRCEditCtrl :
  public CWindowImpl<CIRCEditCtrl, CRichEditCtrl>,
  public CRichEditCommands<CIRCEditCtrl>
{
private:
  int m_ControlCodeMode;
  COLORREF m_Colors[16];

public:

  DECLARE_WND_SUPERCLASS(NULL, CRichEditCtrl::GetWndClassName())

  void PutIRCText( char *Message, COLORREF Foreground, COLORREF Background);

	BEGIN_MSG_MAP(CIRCEditCtrl)
    MESSAGE_HANDLER(WM_PASTE, OnPaste)
    CHAIN_MSG_MAP_ALT(CRichEditCommands<CIRCEditCtrl>, 1)
	END_MSG_MAP()

  CIRCEditCtrl() :
    m_ControlCodeMode(CTRLCODE_MODE_INTERPRET)
  {
  }

  ~CIRCEditCtrl()
  {
  }

  void SetColors( COLORREF *NewColors)
  {
    CopyMemory(m_Colors, NewColors, sizeof(m_Colors));
  }

  LRESULT OnPaste(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
  {
    bHandled = TRUE;
    return 0; // WM_PASTE - This message does not return a value
  }
};

class CHeaderView :
  public CWindowImpl<CHeaderView>
{
public:
  CHeaderView(void);
  ~CHeaderView(void);

	BEGIN_MSG_MAP(CHeaderView)
		MESSAGE_HANDLER(WM_CREATE,             OnCreate)
		MESSAGE_HANDLER(WM_SIZE,               OnSize)
    MESSAGE_HANDLER(WM_COMMAND,            OnCommand)
    MESSAGE_HANDLER(WM_COPY,               OnEditCopy)
    NOTIFY_CODE_HANDLER(EN_LINK,           ::OnEnLink)  // for doubleclickable url's
	END_MSG_MAP()

	LRESULT OnCommand(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
  LRESULT OnEditCopy(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
  {
    if (::IsWindow(m_EditCtrl))
      m_EditCtrl.SendMessage(WM_COPY,0,0);
    return 0; // WM_COPY - no return value
  }

  // the handler for these are included in the parent window's message map, as that's where the messages are sent.
  LRESULT OnEnLink(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);

  int GetLineCount( void )
  {
    return m_EditCtrl.GetLineCount();
  }

  void SetMessageWindow( HWND hWnd) { m_MessageWindow = hWnd; }

  // set and update directly
  void SetBackColor( COLORREF NewColor )
  {
    m_EditCtrl.SetBackgroundColor(NewColor);
  }

  void UpdateHeader(char *NewMessage);
  void SetColors( COLORREF *pColors ); // stores new color values
  void UpdateSettings( void );         // updates uses new values

private:
  HWND m_MessageWindow;
  char *m_CurrentHeader;

  CIRCEditCtrl m_EditCtrl;
  CButton m_ButtonCtrl;

  COLORREF m_TextColor;
  COLORREF m_BackColor;
};
