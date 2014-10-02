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

// TextQueueFrm.cpp : implementation of the CTextQueueFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

CTextQueueView::CTextQueueView( TextQueueManager *pTextQueueMgr, DWORD dwIcon, CEventManager *pEventManager)
  :m_dwIcon(dwIcon),
   m_pTextQueueMgr(pTextQueueMgr), // so we know who it belongs to.
   CListener(pEventManager)
{
  
}

CTextQueueView::~CTextQueueView( void )
{  
#ifdef VERBOSE_DEBUG
  sys_Printf(BIC_FUNCTION,"CTextQueueView::~CTextQueueView() called\n");
#endif
}

LRESULT CTextQueueView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(m_dwIcon), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

  RECT r;
  GetClientRect(&r);

  m_TransfersListCtrl.Create(m_hWnd,&r,NULL,
    WS_CHILD | WS_VISIBLE | 
    LVS_REPORT | LVS_ALIGNLEFT | LVS_NOSORTHEADER | WS_TABSTOP,
    WS_EX_CLIENTEDGE,
    IDC_TEXTQUEUE_LIST);

  m_TransfersListCtrl.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT,LVS_EX_FULLROWSELECT);

  // Add column headers
  m_TransfersListCtrl.AddColumn("Time",0);
  m_TransfersListCtrl.AddColumn("From",1);
  switch(m_pTextQueueMgr->m_QueueType)
  {
    case CWQUEUE_PRIVMSG:
    case CWQUEUE_NOTICE:
      m_TransfersListCtrl.AddColumn("Message",2);
      break;
    case CWQUEUE_URLCATCHER:
      m_TransfersListCtrl.AddColumn("URL/Address",2);
      m_TransfersListCtrl.AddColumn("Count",4);
      break;
  }
  m_TransfersListCtrl.AddColumn("Server",3);

  switch(m_pTextQueueMgr->m_QueueType)
  {
    case CWQUEUE_PRIVMSG:
      SetWindowText("Private messages");
      break;
    case CWQUEUE_NOTICE:
      SetWindowText("Notice messages");
      break;
    case CWQUEUE_URLCATCHER:
      SetWindowText("URL Catcher");
      break;
  }

  int count = m_pTextQueueMgr->m_TextQueue.GetSize();

  if (count == 0) // size the colums to the header text if no items..
  {
    m_TransfersListCtrl.SetColumnWidth(0,MAKELPARAM((int)LVSCW_AUTOSIZE_USEHEADER,0));
    m_TransfersListCtrl.SetColumnWidth(1,MAKELPARAM((int)LVSCW_AUTOSIZE_USEHEADER,0));
    m_TransfersListCtrl.SetColumnWidth(2,MAKELPARAM((int)LVSCW_AUTOSIZE_USEHEADER,0));
    m_TransfersListCtrl.SetColumnWidth(3,MAKELPARAM((int)LVSCW_AUTOSIZE_USEHEADER,0));

    switch(m_pTextQueueMgr->m_QueueType)
    {
      case CWQUEUE_URLCATCHER:
        m_TransfersListCtrl.SetColumnWidth(4,MAKELPARAM((int)LVSCW_AUTOSIZE_USEHEADER,0));
        break;
    }
  }

  for (int i = 0 ; i < count ; i++)
  {
    // add all the items, on the last item, size the columns to the data.
    AddItem(m_pTextQueueMgr->m_TextQueue[i],(i == count-1));
  }

	bHandled = FALSE;

  UpdateSettings();
	return 1;
}

LRESULT CTextQueueView::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
  RECT r;
  GetClientRect(&r);
  m_TransfersListCtrl.MoveWindow(&r);        
  bHandled = FALSE;
  return 1;
}

LRESULT CTextQueueView::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;  
  
	return 1;
}

void CTextQueueView::AutoSizeColumns( void )
{
  m_TransfersListCtrl.SetColumnWidth(0,MAKELPARAM((int)LVSCW_AUTOSIZE,0));
  m_TransfersListCtrl.SetColumnWidth(1,MAKELPARAM((int)LVSCW_AUTOSIZE,0));
  m_TransfersListCtrl.SetColumnWidth(2,MAKELPARAM((int)LVSCW_AUTOSIZE,0));
  m_TransfersListCtrl.SetColumnWidth(3,MAKELPARAM((int)LVSCW_AUTOSIZE,0));
  switch(m_pTextQueueMgr->m_QueueType)
  {
    case CWQUEUE_URLCATCHER:
      // don't auto size the Count column from the data.
      //m_TransfersListCtrl.SetColumnWidth(4,MAKELPARAM((int)LVSCW_AUTOSIZE,0));
      break;
  }
}

void CTextQueueView::AddItem(TextQueueItem *pTQI, BOOL AutoSize)
{
  tm *t = localtime(&pTQI->m_Time);
  //int itemnum = m_TransfersListCtrl.InsertItem(0,stripcrlf(asctime(t)));

  int itemnum = m_TransfersListCtrl.InsertItem(0,GetTimestamp(t));
  if (itemnum != -1)
  {
    m_TransfersListCtrl.AddItem(itemnum,1,pTQI->m_From);
    m_TransfersListCtrl.AddItem(itemnum,2,pTQI->m_Message);
    m_TransfersListCtrl.AddItem(itemnum,3,pTQI->m_ServerName);
    m_TransfersListCtrl.SetItemData(itemnum,(DWORD_PTR)pTQI);

    switch(m_pTextQueueMgr->m_QueueType)
    {
      case CWQUEUE_URLCATCHER:
        URLCatcherQueueItem *pUCQI = (URLCatcherQueueItem *)pTQI;
        char number[10];
        _snprintf(number,sizeof(number)-1,"%d",pUCQI->m_Seen);
        m_TransfersListCtrl.AddItem(itemnum,4, number);
        break;
    }

  }
  if (AutoSize)
    AutoSizeColumns();
}

void CTextQueueView::UpdateItem(TextQueueItem *pTQI)
{
  LVFINDINFO lvFindInfo;
  lvFindInfo.flags = LVFI_PARAM;
  lvFindInfo.lParam = (LPARAM)pTQI;
  int itemnum = m_TransfersListCtrl.FindItem(&lvFindInfo,-1); // -1 = start from beginning

  if (itemnum >= 0)
  {
    // update item accordingly

    switch(m_pTextQueueMgr->m_QueueType)
    {
      case CWQUEUE_URLCATCHER:
        URLCatcherQueueItem *pUCQI = (URLCatcherQueueItem *)pTQI;
        char number[10];
        _snprintf(number,sizeof(number)-1,"%d",pUCQI->m_Seen);
        m_TransfersListCtrl.SetItem(itemnum,4 , LVIF_TEXT , number, 0, 0, 0, NULL);
        break;
    }    
  }
}

void CTextQueueView::RemoveItem(TextQueueItem *pTQI)
{
  TextQueueItem *pFoundTQI;

  for (int i = 0 ; i < m_TransfersListCtrl.GetItemCount(); i++)
  {
    pFoundTQI = (TextQueueItem *) m_TransfersListCtrl.GetItemData(i);
    if (pFoundTQI == pTQI)
    {
      m_TransfersListCtrl.DeleteItem(i);
      return;
    }
  }
}

LRESULT CTextQueueView::OnLvnKeyDown(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
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
        for (int i = 0 ; i < m_TransfersListCtrl.GetItemCount(); i ++)  
          m_TransfersListCtrl.SetItemState(i,LVIS_SELECTED, LVIS_SELECTED);
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
        {
          TextQueueItem * pTQI;
          for (int i = 0 ; i < m_TransfersListCtrl.GetItemCount(); i ++)  
          {
            if (LVIS_SELECTED & m_TransfersListCtrl.GetItemState(i,LVIS_SELECTED))
            {
              pTQI = (TextQueueItem *)m_TransfersListCtrl.GetItemData(i);

              if (!pTQI)
                continue;

              m_TransfersListCtrl.DeleteItem(i);
              m_pTextQueueMgr->m_TextQueue.Remove(pTQI);
              delete pTQI;
              i --; // step back one.
            }
          }
        }
        break;
      default:
        bHandled = FALSE;
    }
  }
  return 0;
}

// TODO: add this functionality to the item's context menu.
LRESULT CTextQueueView::OnNmDblClick(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
{
  LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE) pnmh;

  if (lpnmitem->iItem != -1)
  {
    TextQueueItem *pTQI;
    pTQI = (TextQueueItem *) m_TransfersListCtrl.GetItemData(lpnmitem->iItem);

    if (m_pTextQueueMgr->m_QueueType == CWQUEUE_PRIVMSG ||
        m_pTextQueueMgr->m_QueueType == CWQUEUE_NOTICE)
    {
      // server still valid and on same network ?
      if (g_ServerList.Find(pTQI->m_pServer) >= 0 && pTQI->m_pServer->m_pDetails->m_NetworkID == pTQI->m_NetworkID)
      {
        IRCQuery *pQuery = pTQI->m_pServer->CreateQuery(pTQI->m_From,NULL);
        if (!pQuery)
          pQuery = pTQI->m_pServer->FindQuery(pTQI->m_From);

        // and show it!

        if (pQuery)
          pQuery->ShowQueryWindow();
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
    else if (m_pTextQueueMgr->m_QueueType == CWQUEUE_URLCATCHER)
    {
      ShellExecute(NULL, NULL, pTQI->m_Message, 0, 0, SW_SHOW);

      URLCatcherQueueItem *pUCQI = (URLCatcherQueueItem *)pTQI;

      if (!(pUCQI->m_Flags & UCQIF_VISITED))
      {
        pUCQI->m_Flags |= UCQIF_VISITED;
        UpdateItem(pTQI);
      }
    }
  }

  return 0;
}

LRESULT CTextQueueView::OnNmCustomDraw(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
  LPNMLVCUSTOMDRAW pNMH = (LPNMLVCUSTOMDRAW)pnmh;
  
  switch(pNMH->nmcd.dwDrawStage)
  {
    case CDDS_PREPAINT:
      return CDRF_NOTIFYITEMDRAW;
      
    case CDDS_ITEMPREPAINT:
      if (m_pTextQueueMgr->m_QueueType == CWQUEUE_URLCATCHER)
        return CDRF_NOTIFYSUBITEMDRAW;
      // follow through if not the url catcher...
    case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
      {
        TextQueueItem *pTQI;
        pTQI = (TextQueueItem *)pNMH->nmcd.lItemlParam;

        //sys_Printf(BIC_INFO,"pNMH->nmcd.dwItemSpec == 0x%08x pNMH->nmcd.lItemlParam == 0x%08x\n",pNMH->nmcd.dwItemSpec, pNMH->nmcd.lItemlParam);

        // if these are the same then we're being told about column headers, if not
        // then we're being told about an item and dwItemSpec is the item index
        // gee thanks for the docs Microsoft, NOT!

        if ((DWORD) pNMH->nmcd.dwItemSpec == (DWORD) pNMH->nmcd.lItemlParam)
          return NULL; // don't care about the column headers

        if (!pTQI)
          return NULL;//CDRF_DODEFAULT ?

        switch (m_pTextQueueMgr->m_QueueType)
        {
          case CWQUEUE_URLCATCHER:  

            if (pNMH->iSubItem == 2) // "URL/Address column"
            {
              URLCatcherQueueItem *pUCQI = (URLCatcherQueueItem *)pTQI;

              if (pUCQI->m_Flags & UCQIF_VISITED)
                pNMH->clrText = COLORVAL(item_urlcatchervisitedurltext);
              else
                pNMH->clrText = COLORVAL(item_urlcatcherurltext);
            }
            else
              pNMH->clrText = COLORVAL(item_urlcatchertext);
            
            pNMH->clrTextBk = COLORVAL(item_urlcatcherbackground);
            break;

          case CWQUEUE_PRIVMSG:
            pNMH->clrText = COLORVAL(item_querylogtext);
            pNMH->clrTextBk = COLORVAL(item_querylogbackground);
            break;
          case CWQUEUE_NOTICE:
            pNMH->clrText = COLORVAL(item_noticelogtext);
            pNMH->clrTextBk = COLORVAL(item_noticelogbackground);
            break;
        }
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

void CTextQueueView::OnEvent(int EventID, void *pData)
{
  switch(EventID)
  {
    case EV_PREFSCHANGED:
      UpdateSettings();
      break;
  }
}

void CTextQueueView::UpdateSettings( void )
{
  if (g_pPrefs)
  {
    switch (m_pTextQueueMgr->m_QueueType)
    {
      case CWQUEUE_URLCATCHER:
        m_TransfersListCtrl.SetBkColor(COLORVAL(item_urlcatcherbackground));    
        break;
      case CWQUEUE_PRIVMSG:
        m_TransfersListCtrl.SetBkColor(COLORVAL(item_querylogbackground));    
        break;
      case CWQUEUE_NOTICE:
        m_TransfersListCtrl.SetBkColor(COLORVAL(item_noticelogbackground));    
        break;

    }
  }
}
