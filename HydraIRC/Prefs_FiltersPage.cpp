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

// Prefs_FiltersPage.h : interface of the CFiltersPage class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

typedef struct FilterTableItem_s {
  int m_ID;
  char *m_Name;
} FilterTableItem_t;

FilterTableItem_t g_FilterNamesTable[] =
{
  {BIC_UNKNOWN     ,"Unknown"},
  {BIC_NOTICE      ,"Notices"},
  {BIC_TOPIC       ,"Topic"},
  {BIC_JOIN        ,"Join"},
  {BIC_KICK        ,"Kick"},
  {BIC_MODE        ,"Mode"},
  {BIC_NICK        ,"Nick"},
  {BIC_PART        ,"Part"},
  {BIC_ACTION      ,"Action"},
  {BIC_QUIT        ,"Quit"},
  {BIC_CONNECTING  ,"Connect"},
  {BIC_CHANNELTEXT ,"ChannelText"},
  {BIC_QUERYTEXT   ,"QueryText"},
  {BIC_SERVERMSG   ,"Server"},
  {BIC_CTCP        ,"CTCP"}
};

void CFiltersPage::OnPageDisplay( void )
{
  m_ObjectCtrl.Attach(GetDlgItem(IDC_FILTERS_OBJECT));
  m_UnusedListCtrl.Attach(GetDlgItem(IDC_FILTERS_UNUSED));
  m_UsedListCtrl.Attach(GetDlgItem(IDC_FILTERS_USED));
  m_IncludeCtrl.Attach(GetDlgItem(IDC_FILTERS_INCLUDE));

  m_ObjectCtrl.AddString("Channel Windows");
  m_ObjectCtrl.AddString("Server Windows");
  m_ObjectCtrl.AddString("Channel Monitor");
  m_ObjectCtrl.AddString("Output Window");
  m_ObjectCtrl.SetCurSel(0);
  this->UpdateWindow();
}

void CFiltersPage::OnPageDone( void )
{
  while (m_ObjectCtrl.GetCount() > 0)
  {
    m_ObjectCtrl.DeleteString(0);
  }
  m_ObjectCtrl.Detach();

  ClearLists();
  m_UnusedListCtrl.Detach();
  m_UsedListCtrl.Detach();

  m_IncludeCtrl.Detach();
}

void CFiltersPage::ClearLists( void )
{
  while (m_UnusedListCtrl.GetItemCount() > 0)
  {
    m_UnusedListCtrl.DeleteAllItems();
  }
  while (m_UsedListCtrl.GetItemCount() > 0)
  {
    m_UsedListCtrl.DeleteAllItems();
  }
}

void CFiltersPage::PopulateLists( int Selection )
{
  ClearLists();
  switch(Selection)
  {
    case 0: // Channel Windows
      
      break;
  }

}

BOOL CFiltersPage::OnPageValidate( void )
{
  // don't need any validation.  user can't break anything!
  BOOL OK = TRUE;
  return OK;
}

LRESULT CFiltersPage::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  // the real init is done when the page is OnPageDisplay'd, as other preference items might
  // make a difference to the data

  DlgResize_Init(false,true,0);

  return 0;
}



LRESULT CFiltersPage::OnCbnSelchangeObject(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  int Sel = m_ObjectCtrl.GetCurSel();
  if (Sel != CB_ERR)
  {
    PopulateLists(Sel);
  }
  return 0;
}

LRESULT CFiltersPage::OnBnClickedAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  return 0;
}
LRESULT CFiltersPage::OnBnClickedRemove(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  return 0;
}
