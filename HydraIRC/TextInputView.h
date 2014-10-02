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
