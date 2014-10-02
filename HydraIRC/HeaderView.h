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
