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
