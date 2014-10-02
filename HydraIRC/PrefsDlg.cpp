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

// PrefsDlg.cpp : implementation of the CPrefsDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"
#include "prefsdlg.h"

void CPrefsDlg::ShowPage( int PageNum )
{
  if (m_pActivePage)
  {
    if (m_pActivePage->OnPageValidate())
    {
      m_pActivePage->OnPageDone();
    }
    else
    {
      // TODO: select the still active page's (m_pActivePage) corresponding item
      // in the treeview.
      return;
    }
  }

  m_CtrlContainer.SetCurSel(PageNum);
  
  m_pActivePage = m_PrefsPages[PageNum];

  m_OptionCtrl.SetWindowText(m_pActivePage->GetName());
  m_InfoCtrl.SetWindowText(m_pActivePage->GetInfo());
    

  // TODO: select the corresponding item in the treeview.
  // e.g. if PageNum is set to PREFSPAGE_IDENTITIES then select/highlight "User Identities"

  m_pActivePage->OnPageDisplay();
}

LRESULT CPrefsDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{  
  HTREEITEM hItem,hParent,hChild,hItemToSelect;

	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

  CenterWindow(GetParent());
  DlgResize_Init();

  m_OptionCtrl.Attach(GetDlgItem(IDC_STATIC_OPTION));
  m_InfoCtrl.Attach(GetDlgItem(IDC_STATIC_INFO));
  m_PrefsTree.Attach(GetDlgItem(IDC_PREFSTREE));

  // Add the items in whatever order we please...

  hParent=m_PrefsTree.InsertItem(TVIF_TEXT | TVIF_PARAM, "Users and Identification", 0, 0, 0, 0, PREFSPAGE_SELECTCHILD, TVI_ROOT,TVI_LAST);    
  hChild=m_PrefsTree.InsertItem(TVIF_TEXT | TVIF_PARAM, "User Identities", 0, 0, 0, 0, PREFSPAGE_IDENTITIES, hParent, TVI_LAST);
  hItemToSelect = hChild;
  hChild=m_PrefsTree.InsertItem(TVIF_TEXT | TVIF_PARAM, "IdentD Server", 0, 0, 0, 0, PREFSPAGE_NOTIMPLEMENTED, hParent,TVI_LAST);
  m_PrefsTree.Expand(hParent,TVE_EXPAND);

  hParent=m_PrefsTree.InsertItem(TVIF_TEXT | TVIF_PARAM, "Display", 0, 0, 0, 0, PREFSPAGE_SELECTCHILD, TVI_ROOT,TVI_LAST);
  hChild=m_PrefsTree.InsertItem(TVIF_TEXT | TVIF_PARAM, "Server List", 0, 0, 0, 0, PREFSPAGE_NOTIMPLEMENTED, hParent,TVI_LAST);
  hChild=m_PrefsTree.InsertItem(TVIF_TEXT | TVIF_PARAM, "User List", 0, 0, 0, 0, PREFSPAGE_NOTIMPLEMENTED, hParent,TVI_LAST);
  hChild=m_PrefsTree.InsertItem(TVIF_TEXT | TVIF_PARAM, "Channel Monitor", 0, 0, 0, 0, PREFSPAGE_NOTIMPLEMENTED, hParent,TVI_LAST);
  hChild=m_PrefsTree.InsertItem(TVIF_TEXT | TVIF_PARAM, "Server Windows", 0, 0, 0, 0, PREFSPAGE_NOTIMPLEMENTED, hParent,TVI_LAST);
  hChild=m_PrefsTree.InsertItem(TVIF_TEXT | TVIF_PARAM, "Channel Windows", 0, 0, 0, 0, PREFSPAGE_NOTIMPLEMENTED, hParent,TVI_LAST);
  hChild=m_PrefsTree.InsertItem(TVIF_TEXT | TVIF_PARAM, "Query Windows", 0, 0, 0, 0, PREFSPAGE_NOTIMPLEMENTED, hParent,TVI_LAST);
  m_PrefsTree.Expand(hParent,TVE_EXPAND);

  

  hParent=m_PrefsTree.InsertItem(TVIF_TEXT | TVIF_PARAM, "Notifications and Alerts", 0, 0, 0, 0, PREFSPAGE_SELECTCHILD, TVI_ROOT, TVI_LAST);
  hChild=m_PrefsTree.InsertItem(TVIF_TEXT | TVIF_PARAM, "Messages", 0, 0, 0, 0, PREFSPAGE_MESSAGES, hParent,TVI_LAST);
  hChild=m_PrefsTree.InsertItem(TVIF_TEXT | TVIF_PARAM, "User Groups", 0, 0, 0, 0, PREFSPAGE_BUDDYGROUPS, hParent, TVI_LAST);
  hChild=m_PrefsTree.InsertItem(TVIF_TEXT | TVIF_PARAM, "Notifications", 0, 0, 0, 0, PREFSPAGE_NOTIFICATIONS, hParent, TVI_LAST);
  m_PrefsTree.Expand(hParent,TVE_EXPAND);

  hParent=m_PrefsTree.InsertItem(TVIF_TEXT | TVIF_PARAM, "Logs and History", 0, 0, 0, 0, PREFSPAGE_SELECTCHILD, TVI_ROOT, TVI_LAST);
  hChild=m_PrefsTree.InsertItem(TVIF_TEXT | TVIF_PARAM, "Logging", 0, 0, 0, 0, PREFSPAGE_LOGGING, hParent,TVI_LAST);
  m_PrefsTree.Expand(hParent,TVE_EXPAND);

  hItem=m_PrefsTree.InsertItem(TVIF_TEXT | TVIF_PARAM, "Scripts", 0, 0, 0, 0, PREFSPAGE_COMMANDPROFILES, TVI_ROOT,TVI_LAST);

  hParent=m_PrefsTree.InsertItem(TVIF_TEXT | TVIF_PARAM, "Advanced", 0, 0, 0, 0, PREFSPAGE_SELECTCHILD, TVI_ROOT,TVI_LAST);
  hChild=m_PrefsTree.InsertItem(TVIF_TEXT | TVIF_PARAM, "All Settings", 0, 0, 0, 0, PREFSPAGE_MISC, hParent,TVI_LAST);

  // TODO: Finish filters page.
  //hItem=m_PrefsTree.InsertItem(TVIF_TEXT | TVIF_PARAM, "Filters", 0, 0, 0, 0, PREFSPAGE_FILTERS, TVI_ROOT,TVI_LAST);
  // TODO: Ditch the highlighting page...  not used!
  //hItem=m_PrefsTree.InsertItem(TVIF_TEXT | TVIF_PARAM, "Highlighting", 0, 0, 0, 0, PREFSPAGE_HIGHLIGHTING, TVI_ROOT,TVI_LAST);
  /*
  hItem=m_PrefsTree.InsertItem(TVIF_TEXT | TVIF_PARAM, "Display", 0, 0, 0, 0, PREFSPAGE_NOTIMPLEMENTED, TVI_ROOT,TVI_LAST);

  hParent=m_PrefsTree.InsertItem(TVIF_TEXT | TVIF_PARAM, "Networks", 0, 0, 0, 0, PREFSPAGE_NOTIMPLEMENTED, TVI_ROOT,TVI_LAST);
  hItem=m_PrefsTree.InsertItem(TVIF_TEXT | TVIF_PARAM, "Servers", 0, 0, 0, 0, PREFSPAGE_NOTIMPLEMENTED, hParent,TVI_LAST);
  m_PrefsTree.Expand(hParent,TVE_EXPAND);
  */

  m_CtrlContainer.SubclassWindow(GetDlgItem(IDC_PREFS_PLACEHOLDER));

  m_NotImplementedPage.Create(m_CtrlContainer);
  m_NotImplementedPage.SetDetails("","");
  m_PrefsPages.Add(&m_NotImplementedPage);
  m_CtrlContainer.AddItem(m_NotImplementedPage); // 0

  m_IdentitiesPage.Create(m_CtrlContainer);
  m_IdentitiesPage.SetDetails("User Identities","Personal information");
  m_PrefsPages.Add(&m_IdentitiesPage);
  m_CtrlContainer.AddItem(m_IdentitiesPage); // 1

  m_BuddyGroupsPage.Create(m_CtrlContainer);
  m_BuddyGroupsPage.SetDetails("User Groups","Add users to groups");
  m_PrefsPages.Add(&m_BuddyGroupsPage);
  m_CtrlContainer.AddItem(m_BuddyGroupsPage); // 2

  m_NotificationsPage.Create(m_CtrlContainer);
  m_NotificationsPage.SetDetails("Notifications","Notifications");
  m_PrefsPages.Add(&m_NotificationsPage);
  m_CtrlContainer.AddItem(m_NotificationsPage); // 3

  m_MiscPage.Create(m_CtrlContainer);
  m_MiscPage.SetDetails("Misc","Temporary Page");
  m_PrefsPages.Add(&m_MiscPage);
  m_CtrlContainer.AddItem(m_MiscPage); // 4

  m_LoggingPage.Create(m_CtrlContainer);
  m_LoggingPage.SetDetails("Logging","Text loggings settings");
  m_PrefsPages.Add(&m_LoggingPage);
  m_CtrlContainer.AddItem(m_LoggingPage); // 5

  m_MessagesPage.Create(m_CtrlContainer);
  m_MessagesPage.SetDetails("Messages","Message and Notice settings");
  m_PrefsPages.Add(&m_MessagesPage);
  m_CtrlContainer.AddItem(m_MessagesPage); // 6

  m_CommandProfilesPage.Create(m_CtrlContainer);
  m_CommandProfilesPage.SetDetails("Command Profiles","Named groups of commands");
  m_PrefsPages.Add(&m_CommandProfilesPage);
  m_CtrlContainer.AddItem(m_CommandProfilesPage); // 7

  m_HighlightingPage.Create(m_CtrlContainer);
  m_HighlightingPage.SetDetails("Highlighting","Highlighting prefs");
  m_PrefsPages.Add(&m_HighlightingPage);
  m_CtrlContainer.AddItem(m_HighlightingPage); // 8

  // then add the page classes to the list of pages.
  m_SelectChildPage.Create(m_CtrlContainer);
  m_SelectChildPage.SetDetails("","");
  m_PrefsPages.Add(&m_SelectChildPage);
  m_CtrlContainer.AddItem(m_SelectChildPage); // 9

  /* TODO: Finish filters page.
  m_FiltersPage.Create(m_CtrlContainer);
  m_FiltersPage.SetDetails("Filters","Show or limit channel information");
  m_PrefsPages.Add(&m_FiltersPage);
  m_CtrlContainer.AddItem(m_FiltersPage); // 10
  */

  // and select a page...
  if (m_DefaultPage == -1) // this lets us jump to a particular page..
  {
    m_PrefsTree.SelectItem(m_PrefsTree.GetRootItem());
  }
  else
  {
    m_PrefsTree.SelectItem(hItemToSelect);
    ShowPage(m_DefaultPage);
  }
  return TRUE;
}

LRESULT CPrefsDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  // end the current page
  m_pActivePage->OnPageDone();
  EndDialog(0);    
  return 0;
}

LRESULT CPrefsDlg::OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  // Validate the current page!
  if (m_pActivePage->OnPageValidate())
  {
    m_pActivePage->OnPageDone();
    EndDialog(1);    
  }
	return 0;
}

LRESULT CPrefsDlg::OnTvnSelchangingPrefstree(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
  LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
  // validate the current dialog...
  
  if (!m_pActivePage)
    return 0; // OK, allow change..

  if (m_pActivePage->OnPageValidate())
  {
    m_pActivePage->OnPageDone();
    m_pActivePage = NULL;
    return 0; // OK, allow change..
  }
  else
    return 1; // PAGE NOT VALIDATED!  Disallow change!
  
}

LRESULT CPrefsDlg::OnTreeSelectChange(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
{
  // when a user clicks on an item in the server treeview, we get a message
  // the message contains a pointer to the new and old selections

  LPNMTREEVIEW selectioninfo = (LPNMTREEVIEW)pnmh;  

  ShowPage((int) m_PrefsTree.GetItemData( selectioninfo->itemNew.hItem ));

  bHandled = FALSE;
  return 0;
}
