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
