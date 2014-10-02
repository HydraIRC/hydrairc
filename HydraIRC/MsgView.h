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

// MsgView.h : interface of the CMsgView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#define MBSL_UNLOCKED  0
#define MBSL_ACTIVE   (1<<0)  // scroll lock is active
#define MBSL_TIMED    (1<<1)  // if set, scroll lock will be deactivated after a time period, m_ScrollLockTime will be valid.

class CMsgView : public CWindowImpl<CMsgView, CRichEditCtrl>, 
                 public CRichEditCommands<CMsgView>,
                 public CListener
{
private:
  UINT m_ContextMenuID;
  HWND m_ContextMenuWnd;

  int m_ControlCodeMode;
  BOOL m_EnableTimestamps;
  BOOL m_Dirty;

  BOOL m_ScrollLockFlags;
  time_t m_ScrollLockTime; // the time that the scroll lock was activated, only valid when MBSL_TIMED is set
  // The send queue, only used when Flood Control activates.
  CSimpleArray<BufferItem *> m_OutputQueue; // when scroll lock is active, the output queue is buffered here.
  
  int m_MaxBufferLines;
  int m_BufferLines;
  COLORREF m_Colors[PREF_COLOR_MAX];
  int *m_FilterList; // "NULL" - OR - "FILTER_EXCLUDE | FILTER_INCLUDE, filternum, filternum,...,FILTER_END" - see BIC_* defines.

  // use a buffer twice the size of the length of the text range we need
  // to receive due to a bug/issue with Windows 2000 Chinese 
  char m_BufferTrimBuf[2048+2];

  char m_TimeStampBuffer[MAX_TIMESTAMP_LEN];
  char *m_TimeStampFormat;

public:
	DECLARE_WND_SUPERCLASS(NULL, CRichEditCtrl::GetWndClassName())

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CMsgView)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)  
    MESSAGE_HANDLER(WM_SIZE, OnSize)  
    MESSAGE_HANDLER(WM_REQUESTSCROLLTOEND,OnRequestScrollToEnd)
/*
    MESSAGE_HANDLER(WM_MENUCOMMAND,OnMenuCommand)

    COMMAND_HANDLER(IDC_SERVERMONITOR_CLEARBUFFER, BN_CLICKED, OnBnClickedClearBuffer)
    COMMAND_HANDLER(IDC_SERVERMONITOR_COPYBUFFER, BN_CLICKED, OnBnClickedCopyBuffer)

    COMMAND_HANDLER(IDC_CHANNELMONITOR_CLEARBUFFER, BN_CLICKED, OnBnClickedClearBuffer)
    COMMAND_HANDLER(IDC_CHANNELMONITOR_COPYBUFFER, BN_CLICKED, OnBnClickedCopyBuffer)
*/
		CHAIN_MSG_MAP_ALT(CRichEditCommands<CMsgView>, 1)
		//DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

  //LRESULT OnMenuCommand(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnRequestScrollToEnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

  // the handler for these are included in the parent window's message map, as that's where the messages are sent.
  LRESULT OnEnLink(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
  LRESULT OnMsgFilter(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);

  // context menu stuff
  LRESULT OnBnClickedClearBuffer(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedCopyBuffer(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

  void Put(const int Contents, const char *buf, const time_t *t  = NULL );
  void DisplayTimeStamp( const time_t *t );
  void ScrollToEnd( void );

  CMsgView( void );
  ~CMsgView( void );

  void SetColors( COLORREF *pColors );
  void SetFont(FontPrefItem_t *pFPI);
  void ClearBuffer( void );
  void SetContextMenu(UINT MenuResourceID, HWND MessageWindow);
  void SetControlCodeMode( int Mode ) { m_ControlCodeMode = Mode; }
  int GetControlCodeMode( void ) { return m_ControlCodeMode; }
  BOOL GetTimeStamps( void ) { return m_EnableTimestamps; }
  void SetTimeStamps( BOOL Timestamps ) { m_EnableTimestamps = Timestamps; }
  void SetTimeStampFormat( char *FormatStr ) { SetString(m_TimeStampFormat,FormatStr); }
  void SetMaxBufferLines( int MaxLines ) { m_MaxBufferLines = MaxLines; }
  void SetFilterList( int *FilterList ) { m_FilterList = FilterList; }  
  int *GetFilterList( void ) { return m_FilterList; }  

  BOOL IsDirty( void ) { return m_Dirty; }

  BOOL SetScrollLock(int NewFlags);
  BOOL IsScrollLocked( void );

  BOOL IsLink(CHARRANGE cr);
  BOOL GetLinkRange(CHARRANGE cr, CHARRANGE& linkcr);

  void ProcessOutputQueue( void );

  void ShowScrollbar( void );

  // from CListener
  void OnEvent(int EventID, void *pData);

private:
  void PutNow(const int Contents, const char *buf, const time_t *t = NULL);

};
