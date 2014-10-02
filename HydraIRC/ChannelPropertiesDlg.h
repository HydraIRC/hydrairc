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

// Connectdlg.h : interface of the CConnectDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

//#include "GUIControls.h"

class CChannelProperties; // forward define

class CChannelPropertiesDlg :
  public CDialogImpl<CChannelPropertiesDlg>,
  public CDialogResize<CChannelPropertiesDlg>
{
private:
  //CUsefulEdit m_TopicCtrl;
  CEdit m_TopicCtrl;
  CEdit m_KeyStringCtrl;
  CEdit m_LimitStringCtrl;
  CButton m_LimitCtrl;
  CButton m_KeyCtrl;
  CButton m_InviteCtrl;

  IRCChannel *m_pChannel;
  CChannelProperties *m_pProperties; // this stores the data that we change..

public:
	enum { IDD = IDD_CHANNELPROPERTIES };

  void OnOK( void );
  CChannelPropertiesDlg( IRCChannel *pChannel, CChannelProperties *pProperties);
  ~CChannelPropertiesDlg( void );

	BEGIN_MSG_MAP(CChannelPropertiesDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOKCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
    CHAIN_MSG_MAP(CDialogResize<CChannelPropertiesDlg>)
  END_MSG_MAP()

  BEGIN_DLGRESIZE_MAP(CChannelPropertiesDlg)
    DLGRESIZE_CONTROL(IDC_CHANNELPROPERTIES_SETTINGSFRAME ,DLSZ_SIZE_X | DLSZ_SIZE_Y)
    DLGRESIZE_CONTROL(IDC_CHANNELPROPERTIES_TOPIC         ,DLSZ_SIZE_X | DLSZ_SIZE_Y)

    DLGRESIZE_CONTROL(IDC_CHANNELPROPERTIES_KEYSTRING     ,DLSZ_SIZE_X | DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_CHANNELPROPERTIES_KEY           ,DLSZ_MOVE_Y)   
    DLGRESIZE_CONTROL(IDC_CHANNELPROPERTIES_LIMIT         ,DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_CHANNELPROPERTIES_LIMITSTRING   ,DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_CHANNELPROPERTIES_INVITE        ,DLSZ_MOVE_Y)

    DLGRESIZE_CONTROL(IDOK                    ,DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDCANCEL                ,DLSZ_MOVE_Y)
  END_DLGRESIZE_MAP()


// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
