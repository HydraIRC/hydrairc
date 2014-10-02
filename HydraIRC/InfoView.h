// WorkView.h : Declaration of the CInfoView

#pragma once

// CInfoView

class CInfoView : public CWindowImpl<CInfoView, CRichEditCtrl>, 
                  public CRichEditCommands<CInfoView>
{ 

public:
  DECLARE_WND_SUPERCLASS(NULL, CRichEditCtrl::GetWndClassName())  

  virtual BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CInfoView)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)      
		CHAIN_MSG_MAP_ALT(CRichEditCommands<CInfoView>, 1)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

  CInfoView()
  {
    // TODO: add new prefs items for these.
    m_TextColor = COLORVAL(item_infotext);
    m_BackColor = COLORVAL(item_infobackground);
  }

  void UpdateSettings( void );
  void SetColors( COLORREF *pColors );

// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnMsgFilter(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);

public:
  void Put(CString Message);

private:
  COLORREF m_TextColor;
  COLORREF m_BackColor;
};


