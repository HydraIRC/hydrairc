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

/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

char *g_EventLogTypeStrings[ELIT_LAST] = 
{
  "Unknown",
  "Server",
  "DCC",
  "Channel",
  "User"
};

char *g_EventLogIDStrings[ELID_LAST] = 
{
  "Unknown",
  "Connect",
  "Disconnect",
  "LoggedIn",
  "Kicked",
  "Notice",
  "PrivMsg",
  "CTCP Sound",
  "CTCP Version",
  "CTCP Ping",
  "CTCP Time",
  "DCC Receive",
  "DCC Send",
  "DCC Receive Complete",
  "DCC Send Complete",
  "DCC Receive Failed",
  "DCC Send Failed",
  "Highlight",
  "Wallops",
  "Notification",
  "Quit",
  "Part",
  "Join",
};

CEventLogView::CEventLogView(EventLogManager *pEventLogMgr,  DWORD dwIcon, CEventManager *pEventManager)
  :m_dwIcon(dwIcon),
   m_pEventLogMgr(pEventLogMgr),
   CListener(pEventManager)
{
  m_FilterList = NULL; // nothing filtered!

  InitFilterGroups();

  //m_EventLogMenuIDs.SetIDRange(ID_EVENTLOGMENU_FIRST,ID_EVENTLOGMENU_LAST); // CHECK: done elsewhere now?
}

CEventLogView::~CEventLogView( void )
{  
#ifdef VERBOSE_DEBUG
  sys_Printf(BIC_FUNCTION,"CEventLogView::~CEventLogView() called\n");
#endif
  if (m_FilterList)
    free(m_FilterList);

  m_IntFilterList.RemoveAll();

  while (m_FilterListGroups.GetSize() > 0)
  {
    delete m_FilterListGroups[0];
    m_FilterListGroups.RemoveAt(0);
  }
}

LRESULT CEventLogView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(m_dwIcon), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

  RECT r;
  GetClientRect(&r);

  SetWindowText("Event Log");

  m_EventLogListCtrl.Create(m_hWnd,&r,NULL,
    WS_CHILD | WS_VISIBLE | 
    LVS_REPORT | LVS_ALIGNLEFT | /* LVS_NOSORTHEADER | */ WS_TABSTOP,
    WS_EX_CLIENTEDGE,
    IDC_TEXTQUEUE_LIST);

  m_EventLogListCtrl.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT,LVS_EX_FULLROWSELECT);

  m_EventLogListCtrl.AddColumn("Time",0);
  m_EventLogListCtrl.AddColumn("Event Type",1);
  m_EventLogListCtrl.AddColumn("Content",2);
  m_EventLogListCtrl.AddColumn("Source Type",3);
  m_EventLogListCtrl.AddColumn("Source",4);


  int count = m_pEventLogMgr->m_EventLog.GetSize();

  if (count == 0) // size the colums to the header text if no items..
  {
    m_EventLogListCtrl.SetColumnWidth(0,MAKELPARAM((int)LVSCW_AUTOSIZE_USEHEADER,0));
    m_EventLogListCtrl.SetColumnWidth(1,MAKELPARAM((int)LVSCW_AUTOSIZE_USEHEADER,0));
    m_EventLogListCtrl.SetColumnWidth(2,MAKELPARAM((int)LVSCW_AUTOSIZE_USEHEADER,0));
    m_EventLogListCtrl.SetColumnWidth(3,MAKELPARAM((int)LVSCW_AUTOSIZE_USEHEADER,0));
    m_EventLogListCtrl.SetColumnWidth(4,MAKELPARAM((int)LVSCW_AUTOSIZE_USEHEADER,0));
  }

  RefreshItems();

	// bHandled = FALSE; // TODO: Check
	return 0;
}

void CEventLogView::RefreshItems( void )
{
  // removes all items and re-adds them,
  // often used after changing the contents of m_InfFilterList

  m_EventLogListCtrl.DeleteAllItems();

  int count = m_pEventLogMgr->m_EventLog.GetSize();
  // add all the items, on the last item, size the columns to the data.
  for (int i = 0 ; i < count ; i++)
  {    
    AddItem(m_pEventLogMgr->m_EventLog[i],(i == count-1));
  }


}

LRESULT CEventLogView::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
  RECT r;
  GetClientRect(&r);
  m_EventLogListCtrl.MoveWindow(&r);        
  bHandled = FALSE;
  return 0;
}

LRESULT CEventLogView::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;  
  
	return 0;
}

void CEventLogView::AutoSizeColumns( void )
{
  m_EventLogListCtrl.SetColumnWidth(0,MAKELPARAM((int)LVSCW_AUTOSIZE,0));
  m_EventLogListCtrl.SetColumnWidth(1,MAKELPARAM((int)LVSCW_AUTOSIZE,0));
  m_EventLogListCtrl.SetColumnWidth(2,MAKELPARAM((int)LVSCW_AUTOSIZE,0));
  m_EventLogListCtrl.SetColumnWidth(3,MAKELPARAM((int)LVSCW_AUTOSIZE,0));
  m_EventLogListCtrl.SetColumnWidth(4,MAKELPARAM((int)LVSCW_AUTOSIZE,0));
}

void CEventLogView::AddItem(EventLogItem *pELI, BOOL AutoSize)
{
  if (!::IsWindow(m_EventLogListCtrl))
    return;

  // don't add if we're filtering this type!
  if (ProcessSimpleFilter(m_FilterList,pELI->m_ID))
    return;

  // not filtered, add it!

  char numstr[10];
  tm *t = localtime(&pELI->m_Time);

  int itemnum = m_EventLogListCtrl.InsertItem(0,GetTimestamp(t));
  if (itemnum != -1)
  {
/*
  m_EventLogListCtrl.AddColumn("Time",0);
  m_EventLogListCtrl.AddColumn("ID",4);
  m_EventLogListCtrl.AddColumn("Message",2);
  m_EventLogListCtrl.AddColumn("Server",3);
  m_EventLogListCtrl.AddColumn("Source",1);
*/
    _snprintf(numstr,sizeof(numstr)-1,"%d",pELI->m_ID);
    m_EventLogListCtrl.AddItem(itemnum,1,pELI->m_ID < ELID_LAST ? g_EventLogIDStrings[pELI->m_ID] : numstr);
    m_EventLogListCtrl.AddItem(itemnum,2,pELI->m_Message);
    m_EventLogListCtrl.AddItem(itemnum,3,g_EventLogTypeStrings[pELI->m_Type]);

    if (stricmp(g_EventLogTypeStrings[pELI->m_Type],"Server") == 0)
      m_EventLogListCtrl.AddItem(itemnum,4,pELI->m_ServerName);
    else if (stricmp(g_EventLogTypeStrings[pELI->m_Type],"DCC") == 0)
      m_EventLogListCtrl.AddItem(itemnum,4,pELI->m_From);    
    else if (stricmp(g_EventLogTypeStrings[pELI->m_Type],"Channel") == 0)
      m_EventLogListCtrl.AddItem(itemnum,4,pELI->m_ChannelName);
    else if (stricmp(g_EventLogTypeStrings[pELI->m_Type],"User") == 0)
      m_EventLogListCtrl.AddItem(itemnum,4,pELI->m_From);
    else if (stricmp(g_EventLogTypeStrings[pELI->m_Type],"Unknown") == 0)
      m_EventLogListCtrl.AddItem(itemnum,4,"Unknown");
    else
      m_EventLogListCtrl.AddItem(itemnum,4,"Unknown");

    m_EventLogListCtrl.SetItemData(itemnum,(DWORD_PTR)pELI);
  }
  if (AutoSize)
    AutoSizeColumns();
}

void CEventLogView::RemoveItem(EventLogItem *pELI)
{
  EventLogItem *pFoundNVI;

  for (int i = 0 ; i < m_EventLogListCtrl.GetItemCount(); i++)
  {
    pFoundNVI = (EventLogItem *) m_EventLogListCtrl.GetItemData(i);
    if (pFoundNVI == pELI)
    {
      m_EventLogListCtrl.DeleteItem(i);
      return;
    }
  }
}

LRESULT CEventLogView::OnLvnKeyDown(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
{
  NMLVKEYDOWN *keyinfo = (NMLVKEYDOWN *) pnmh;

  bHandled = FALSE;
  int virtkey = GetKeyState(VK_CONTROL);
#ifdef DEBUG
  //sys_Printf(BIC_GUI,"key down message: '%c' 0x%02x 0x%08x\n",wParam,wParam,virtkey);
#endif
  if (virtkey & 0x10000) // Control Key Pressed ?
  {
    bHandled = TRUE;
    switch(keyinfo->wVKey)//wParam)
    {
      case 'A': // select all
        DoAction(ELMI_SelectAll);
        break;
      default:
        bHandled = FALSE;
    }
  }
  else // control key NOT pressed.
  {
    bHandled = TRUE;
    switch(keyinfo->wVKey)//wParam)
    {
      case VK_DELETE:
        DoAction(ELMI_Delete);
        break;
      default:
        bHandled = FALSE;
    }
  }
  return 0;
}

LRESULT CEventLogView::OnNmRClick(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
{
  EventLogItem *pELI;

  /*
  LVHITTESTINFO ht = {0};
  DWORD dwpos = GetMessagePos();
  ht.pt.x = GET_X_LPARAM(dwpos);
  ht.pt.y = GET_Y_LPARAM(dwpos);
  // TODO: check ok on multi-monitor system
  ::MapWindowPoints(HWND_DESKTOP, pnmh->hwndFrom, &ht.pt, 1);
  ListView_HitTest(pnmh->hwndFrom, &ht);
  if(LVHT_ONITEM  & ht.flags)
  {
    pELI = (EventLogItem *)m_EventLogListCtrl.GetItemData(ht.iItem);
    if (!pELI)
      return 0;
  }
  */

  CMenuHandle Menu;
  Menu.CreatePopupMenu();

  /*
  MENUINFO MenuInfo;
  MenuInfo.dwMenuData = (ULONG_PTR) pELI;
  MenuInfo.fMask = MIM_MENUDATA;
  Menu.SetMenuInfo(&MenuInfo);
  */

  BOOL First = TRUE;
  int Type = ELIT_UNKNOWN;
  int SelectedCount = 0;
  for (int i = 0 ; i < m_EventLogListCtrl.GetItemCount(); i ++)  
  {
    if (LVIS_SELECTED & m_EventLogListCtrl.GetItemState(i,LVIS_SELECTED))
    {
      SelectedCount++;
      pELI = (EventLogItem *)m_EventLogListCtrl.GetItemData(i);

      if (!pELI)
        continue;

      if (First)
      {
        // make a note of the first selected type
        Type = pELI->m_Type;
        First = FALSE;
      }
      else
      {
        // more than one type selected?
        if (pELI->m_Type != Type)
          Type = ELIT_UNKNOWN;
      }
    }
  }

  if (SelectedCount > 0)
  {
    BOOL Added = TRUE;
    if (Type == ELIT_UNKNOWN)
    {
      Menu.AppendMenu(MF_STRING,ELMI_Action,_T("Do Default Action"));
    }
    else
    {
      switch(pELI->m_Type)
      {
        case ELIT_SERVER:
          Menu.AppendMenu(MF_STRING,ELMI_Action,_T("Show Server Window"));
          break;
        case ELIT_DCC:
          Menu.AppendMenu(MF_STRING,ELMI_Action,_T("Show DCC Window"));
          break;
        case ELIT_CHANNEL:
          Menu.AppendMenu(MF_STRING,ELMI_Action,_T("Show Channel Window"));
          break;
        case ELIT_USER:
          Menu.AppendMenu(MF_STRING,ELMI_Action,_T("Show Query Window"));
          break;
        default:
          Added = FALSE;
      }
    }
    if (Added)
    {
      Menu.SetMenuDefaultItem(0,TRUE);
      Menu.AppendMenu(MF_SEPARATOR);
    }
    Menu.AppendMenu(MF_STRING,ELMI_CopyTime,_T("Copy Time"));
    Menu.AppendMenu(MF_STRING,ELMI_CopyContent,_T("Copy Content"));
    Menu.AppendMenu(MF_SEPARATOR);
    Menu.AppendMenu(MF_STRING,ELMI_Delete,_T("Delete\tDel"));
  }
  Menu.AppendMenu(MF_STRING,ELMI_DeleteAll,_T("Delete All"));
  Menu.AppendMenu(MF_STRING,ELMI_SelectAll,_T("Select All\tCtrl+A"));
  Menu.AppendMenu(MF_SEPARATOR);
  Menu.AppendMenu(MF_STRING,ELMI_ShowAll        ,_T("Show All"));
  Menu.AppendMenu(MF_STRING,ELMI_ShowNone       ,_T("Show None"));
  Menu.AppendMenu(MF_SEPARATOR);

  UINT Flags;

  Flags = (m_ActiveFilterGroupIDs.Find(ELFI_Notices) >= 0 ? MF_UNCHECKED : MF_CHECKED);
  Menu.AppendMenu(MF_STRING | Flags,ELMI_ShowNotices    ,_T("Show Notices"));

  Flags = (m_ActiveFilterGroupIDs.Find(ELFI_Messages) >= 0 ? MF_UNCHECKED : MF_CHECKED);
  Menu.AppendMenu(MF_STRING | Flags,ELMI_ShowMessages   ,_T("Show Messages"));

  Flags = (m_ActiveFilterGroupIDs.Find(ELFI_Highlights) >= 0 ? MF_UNCHECKED : MF_CHECKED);
  Menu.AppendMenu(MF_STRING | Flags,ELMI_ShowHighlights ,_T("Show Highlights"));

  Flags = (m_ActiveFilterGroupIDs.Find(ELFI_Connects) >= 0 ? MF_UNCHECKED : MF_CHECKED);
  Menu.AppendMenu(MF_STRING | Flags,ELMI_ShowConnects   ,_T("Show Connects"));

  Flags = (m_ActiveFilterGroupIDs.Find(ELFI_Disconnects) >= 0 ? MF_UNCHECKED : MF_CHECKED);
  Menu.AppendMenu(MF_STRING | Flags,ELMI_ShowDisconnects,_T("Show Disconnects"));

  Flags = (m_ActiveFilterGroupIDs.Find(ELFI_Transfers) >= 0 ? MF_UNCHECKED : MF_CHECKED);
  Menu.AppendMenu(MF_STRING | Flags,ELMI_ShowTransfers  ,_T("Show Transfers"));

  Flags = (m_ActiveFilterGroupIDs.Find(ELFI_CTCPs) >= 0 ? MF_UNCHECKED : MF_CHECKED);
  Menu.AppendMenu(MF_STRING | Flags,ELMI_ShowCTCPs      ,_T("Show CTCPs"));

  Flags = (m_ActiveFilterGroupIDs.Find(ELFI_Others) >= 0 ? MF_UNCHECKED : MF_CHECKED);
  Menu.AppendMenu(MF_STRING | Flags,ELMI_ShowOther      ,_T("Show Others"));

  

//MF_CHECKED
//MF_UNCHECKED

  POINT pt;
  ::GetCursorPos(&pt);      
  g_pMainWnd->m_CmdBar.TrackPopupMenu(Menu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y);
  //::TrackPopupMenu(Menu, 0, pt.x, pt.y, 0, m_hWnd, NULL);

  return bHandled ? 1 : 0; // NM_RCLICK - Return nonzero to not allow the default processing, or zero to allow the default processing
  // or in otherwords, return zero to allow the default processing
  // or 1 if handled.
}

LRESULT CEventLogView::OnMenuItemSelected(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
  DoAction(wID);
  return 0;
}

void CEventLogView::InitFilterGroups( void )
{
  EventLogFilterGroup *pELFG;

  int IL_Notices[] = {ELID_Notice, -1};
  pELFG = new EventLogFilterGroup(ELFI_Notices, IL_Notices );
  if (pELFG)
    m_FilterListGroups.Add(pELFG);

  int IL_Messages[] = {ELID_PrivMsg, -1};
  pELFG = new EventLogFilterGroup(ELFI_Messages, IL_Messages );
  if (pELFG)
    m_FilterListGroups.Add(pELFG);

  int IL_Highlights[] = {ELID_Highlight, -1};
  pELFG = new EventLogFilterGroup(ELFI_Highlights, IL_Highlights );
  if (pELFG)
    m_FilterListGroups.Add(pELFG);

  int IL_Connects[] = {ELID_Connect, ELID_LoggedIn, -1};
  pELFG = new EventLogFilterGroup(ELFI_Connects, IL_Connects );
  if (pELFG)
    m_FilterListGroups.Add(pELFG);

  int IL_Disconnects[] = {ELID_Disconnect, -1};
  pELFG = new EventLogFilterGroup(ELFI_Disconnects, IL_Disconnects );
  if (pELFG)
    m_FilterListGroups.Add(pELFG);

  int IL_Transfers[] = {ELID_DCC_Receive, ELID_DCC_Send, ELID_DCC_ReceiveComplete, ELID_DCC_ReceiveFailed, ELID_DCC_SendComplete, ELID_DCC_SendFailed, -1};
  pELFG = new EventLogFilterGroup(ELFI_Transfers, IL_Transfers );
  if (pELFG)
    m_FilterListGroups.Add(pELFG);

  int IL_CTCPs[] = {ELID_CTCP_Sound, ELID_CTCP_Version, ELID_CTCP_Ping, ELID_CTCP_Time, -1};
  pELFG = new EventLogFilterGroup(ELFI_CTCPs, IL_CTCPs );
  if (pELFG)
    m_FilterListGroups.Add(pELFG);

  int IL_Others[] = {ELID_Unknown, ELID_Kicked, ELID_Wallops, ELID_Notification, ELID_Quit, ELID_Part, ELID_Join, -1};
  pELFG = new EventLogFilterGroup(ELFI_Others, IL_Others );
  if (pELFG)
    m_FilterListGroups.Add(pELFG);
  
}

void CEventLogView::DoAction( int Action )
{
  EventLogItem *pELI;
  int i;

  if (Action >= ELMI_FIRSTSHOWACTION && Action <= ELMI_LASTSHOWACTION)
  {
    // filter/unfilter event log items.

    // convert action id into a "EventLogFilterGroupIDs"
    // (see EventLogFilterIDs and EventLogMenuIDs)
    int FilterGroupID = Action - ELMI_FIRSTSHOWFILTER + ELFI_FIRST;

    sys_Printf(BIC_INFO,"FilterGroupID = %d, Action = %d\n", FilterGroupID, Action);

    if (FilterGroupID >= ELFI_FIRST && FilterGroupID <= ELFI_LAST)
    {
      // enable/disable specific filters.

      int index;
      EventLogFilterGroup *pELFG;
      BOOL Found = FALSE;
      
      // get a handle to the filter list group specified
      for (i = 0; i < m_FilterListGroups.GetSize() && !Found; i++)
      {
        pELFG = m_FilterListGroups[i];
        if (FilterGroupID == pELFG->m_FilterID)
          Found = TRUE;
      }
     
      if (Found) // known group? It should always be known but check anyway in case of low memory
      {
        
        index = m_ActiveFilterGroupIDs.Find(FilterGroupID);
        if (index >=0)
        {
          m_ActiveFilterGroupIDs.RemoveAt(index);

          // remove the list of EventLog ID's from the current m_IntFilterList

          for (i = 0; i < pELFG->m_EventLogIDList.GetSize(); i++)
          {
            index = m_IntFilterList.Find(pELFG->m_EventLogIDList[i]);
            if (index >=0)
            {
              m_IntFilterList.RemoveAt(index);
            }
          }
        }
        else
        {
          // we didn't find FilterID in the list of active groups then we
          // need to add the eventlogid's to the m_intfilterlist
          // add add the filterid group to the active group list

          m_ActiveFilterGroupIDs.Add(FilterGroupID);

          for (i = 0; i < pELFG->m_EventLogIDList.GetSize(); i++)
          {
            m_IntFilterList.Add(pELFG->m_EventLogIDList[i]);
          }
        }

      }      
    }
    else
    {
      switch (Action)
      {
        case ELMI_ShowAll:
          // remove everything
          m_IntFilterList.RemoveAll();
          m_ActiveFilterGroupIDs.RemoveAll();
          break;

        case ELMI_ShowNone:
          // remove everything...
          m_IntFilterList.RemoveAll();
          m_ActiveFilterGroupIDs.RemoveAll();

          // ... then add everything (easier to code..)
          for (i = ELID_FIRST; i < ELID_LAST; i++)
          {
            m_IntFilterList.Add(i);
          }
          for (i = 0; i < m_FilterListGroups.GetSize(); i++)
          {
            m_ActiveFilterGroupIDs.Add(m_FilterListGroups[i]->m_FilterID);
          }
          break;
      }
    }

    // TODO: Call ValidateActiveFilterGroupsList()
    // which needs to check each active groups's id list to make sure
    // they're in the current m_IntFilterList and if one or more of the groups
    // id list values is NOT in the m_IntFilterList then move the group
    // from m_ActiveFilterGroupIDs

    if (m_FilterList)
      free(m_FilterList);

    m_FilterList = CreateSimpleFilter(m_IntFilterList, FILTER_EXCLUDE);

    RefreshItems();
  }
  else
  {
    // process actions for each event log item.

    for (int i = 0 ; i < m_EventLogListCtrl.GetItemCount(); i ++)  
    {
      pELI = (EventLogItem *)m_EventLogListCtrl.GetItemData(i);

      if (!pELI)
        continue;

      if (LVIS_SELECTED & m_EventLogListCtrl.GetItemState(i,LVIS_SELECTED))
      {
        char m_CopyItem[512];
        switch (Action)
        {
          case ELMI_Action:
            DoDefaultAction(pELI);
            break;
          case ELMI_CopyTime:
            m_EventLogListCtrl.GetItemText(i,0,m_CopyItem,512);
            Clipboard_Copy(CF_TEXT, m_CopyItem, strlen(m_CopyItem) + 1);
            break;
          case ELMI_CopyContent:
            m_EventLogListCtrl.GetItemText(i,2,m_CopyItem,512);
            Clipboard_Copy(CF_TEXT, m_CopyItem, strlen(m_CopyItem) + 1);
            break;
          case ELMI_Delete:
            m_EventLogListCtrl.DeleteItem(i);
            m_pEventLogMgr->m_EventLog.Remove(pELI);
            delete pELI;
            i --; // step back one.
            break;
        }
      }
      else
      {
        // do actions for non-selected items
        switch (Action)
        {
          case ELMI_SelectAll:
            m_EventLogListCtrl.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
            break;
        }
      }
      // do actions for selected/non-selected items
      switch (Action)
      {
        case ELMI_DeleteAll:
          m_EventLogListCtrl.DeleteItem(i);
          m_pEventLogMgr->m_EventLog.Remove(pELI);
          delete pELI;
          i --; // step back one.
          break;
      }
    }
  }
}

void CEventLogView::DoDefaultAction( EventLogItem *pELI )
{
  if (!pELI)
    return;
  // server still valid and on same network ?
  if (pELI && g_ServerList.Find(pELI->m_pServer) >= 0 && pELI->m_pServer->m_pDetails->m_NetworkID == pELI->m_NetworkID)
  {
    switch (pELI->m_Type)
    {
      case ELIT_USER:
        {
          pELI->m_pServer->CreateQuery(pELI->m_From,NULL);
          IRCQuery *pQuery = pELI->m_pServer->FindQuery(pELI->m_From);
    
          // and show it!
    
          if (pQuery)
            pQuery->ShowQueryWindow();
        }
        break;
      case ELIT_SERVER:
      case ELIT_DCC:
        {      
          if (pELI->m_pServer && pELI->m_pServer->m_pChildWnd)
            pELI->m_pServer->m_pChildWnd->ActivateWindow();
            //g_pMainWnd->MDIActivate(pELI->m_pServer->m_pChildWnd->m_hWnd);
        }
        break;
      case ELIT_CHANNEL:
        {
          IRCChannel *pChannel = 
            pELI->m_pServer->FindChannel(pELI->m_pChannel);
          if (!pChannel)
            pChannel = pELI->m_pServer->CreateChannel(pELI->m_ChannelName);

          // and show it!
    
          if (pChannel && pChannel->m_pChildWnd)
            pELI->m_pChannel->m_pChildWnd->ActivateWindow();
            //g_pMainWnd->MDIActivate(pELI->m_pChannel->m_pChildWnd->m_hWnd);
        }
        break;
    }
  }
  else
  {
    sys_Printf(BIC_INFO,"Server doesn't exist, or is connected to a different network\n");
    // TODO: for a future version..
    // We could search for a server connected to the same network, or search for
    // a server with the same servername as the one we stored.
    // too much work for now.
  }

}

// TODO: add this functionality to the item's context menu.
LRESULT CEventLogView::OnNmDblClick(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
{
  LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE) pnmh;

  if (lpnmitem->iItem != -1)
  {
    EventLogItem *pELI;
    pELI = (EventLogItem *) m_EventLogListCtrl.GetItemData(lpnmitem->iItem);

    DoDefaultAction(pELI);
  }

  return 0;
}

LRESULT CEventLogView::OnNmCustomDraw(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
  LPNMLVCUSTOMDRAW pNMH = (LPNMLVCUSTOMDRAW)pnmh;
  
  switch(pNMH->nmcd.dwDrawStage)
  {
    case CDDS_PREPAINT:
      return CDRF_NOTIFYITEMDRAW;
      
    case CDDS_ITEMPREPAINT:
      {
        EventLogItem *pELI;
        pELI = (EventLogItem *)pNMH->nmcd.lItemlParam;

        //sys_Printf(BIC_INFO,"pNMH->nmcd.dwItemSpec == 0x%08x pNMH->nmcd.lItemlParam == 0x%08x\n",pNMH->nmcd.dwItemSpec, pNMH->nmcd.lItemlParam);

        // if these are the same then we're being told about column headers, if not
        // then we're being told about an item and dwItemSpec is the item index
        // gee thanks for the docs Microsoft, NOT!

        if ((DWORD) pNMH->nmcd.dwItemSpec == (DWORD) pNMH->nmcd.lItemlParam)
          return NULL; // don't care about the column headers

        if (!pELI)
          return NULL;//CDRF_DODEFAULT ?

        pNMH->clrText = COLORVAL(item_eventlogtext);
        pNMH->clrTextBk = COLORVAL(item_eventlogbackground);

        return NULL;//CDRF_NEWFONT;
      }
      break;
  }
  // shouldn't get here the return value is just to shut the compiler up.
#ifdef DEBUG
  ATLASSERT(0); 
#endif
  return NULL;
}


void CEventLogView::OnEvent(int EventID, void *pData)
{
  switch(EventID)
  {
    case EV_PREFSCHANGED:
      UpdateSettings();
      break;
  }
}

void CEventLogView::UpdateSettings( void )
{
  if (g_pPrefs)
  {    
    m_EventLogListCtrl.SetBkColor(COLORVAL(item_eventlogbackground));    
  }
}

