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

// TransfersFrm.cpp : implementation of the CTransfersView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

enum TransfersImageListIDs
{
  TILN_SEND = 0,
  TILN_SENDFAIL,
  TILN_SENDOK,
  TILN_RECEIVE,
  TILN_RECEIVEFAIL,
  TILN_RECEIVEOK,

  // add these to TILN_SEND or TILN_RECEIVE
  TILN_FAIL = 1,  
  TILN_OK = 2
};

CTransfersView::~CTransfersView( void )
{  
#ifdef VERBOSE_DEBUG
  sys_Printf(BIC_FUNCTION,"CTransfersView::~CTransfersView() called\n");
#endif
}

LRESULT CTransfersView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
  HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(m_dwIcon), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

  // load transfer images
	m_images_transfer.Create(16, 16, ILC_COLOR24 | ILC_MASK, 6, 0);
	ATLASSERT(m_images_transfer.m_hImageList);
	CBitmap bmp;
	bmp.LoadBitmap(IDB_TRANSFER);
	m_images_transfer.Add( bmp, RGB(255,0,255));

  RECT r;
  GetClientRect(&r);

  m_TransfersListCtrl.Create(m_hWnd,&r,NULL,
    WS_CHILD | WS_VISIBLE | 
    LVS_REPORT | LVS_ALIGNLEFT | LVS_NOSORTHEADER | WS_TABSTOP,
    WS_EX_CLIENTEDGE,
    IDC_TRANSFERS_LIST);

  m_TransfersListCtrl.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT,LVS_EX_FULLROWSELECT);

  m_TransfersListCtrl.SetImageList(m_images_transfer,LVSIL_SMALL);

  m_TransfersListCtrl.AddColumn("Mode",0);
  m_TransfersListCtrl.AddColumn("Filename",1);
  m_TransfersListCtrl.AddColumn("User",2);
  m_TransfersListCtrl.AddColumn("Status",3);
  m_TransfersListCtrl.AddColumn("Speed",4);
  m_TransfersListCtrl.AddColumn("ETA",5);
  m_TransfersListCtrl.AddColumn("Size",6);
  m_TransfersListCtrl.AddColumn("StartTime",7);
  m_TransfersListCtrl.AddColumn("IP",8);
  m_TransfersListCtrl.AddColumn("Server",9);

  m_TransfersListCtrl.SetColumnWidth(0,50);
  m_TransfersListCtrl.SetColumnWidth(1,125);
  m_TransfersListCtrl.SetColumnWidth(2,85);
  m_TransfersListCtrl.SetColumnWidth(3,165);
  m_TransfersListCtrl.SetColumnWidth(4,125);
  m_TransfersListCtrl.SetColumnWidth(5,100);
  m_TransfersListCtrl.SetColumnWidth(6,60);
  m_TransfersListCtrl.SetColumnWidth(7,145);
  m_TransfersListCtrl.SetColumnWidth(8,140);
  m_TransfersListCtrl.SetColumnWidth(9,200);

	bHandled = FALSE;
	return 1;
}

LRESULT CTransfersView::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
  RECT r;
  GetClientRect(&r);
  m_TransfersListCtrl.MoveWindow(&r);        
  bHandled = FALSE;
  return 1;
}

LRESULT CTransfersView::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;  
  
	return 1;
}

void CTransfersView::AddItem(CDCCTransfer *pDCCTransfer)
{
  if (!pDCCTransfer)
    return;

  char SizeStr[11];
  BuildSizeString(SizeStr,sizeof(SizeStr)-1,(double)pDCCTransfer->m_Size);
  /*
  if (pDCCTransfer->m_Size > 1024 * 1024)
    _snprintf(SizeStr,10,"%.2fMB",(double)pDCCTransfer->m_Size / (double)(1024 * 1024));
  else
    _snprintf(SizeStr,10,"%.2fKB",(double)pDCCTransfer->m_Size / 1024);
  */

  tm *t = localtime(&pDCCTransfer->m_Time);

  /*
  char *DirectionStr;
  if (pDCCTransfer->m_Type == DCC_RECEIVE)
    DirectionStr = "Receive";
  else
    DirectionStr = "Send";
  */

  int itemnum = m_TransfersListCtrl.InsertItem(0,""/*DirectionStr*/,pDCCTransfer->m_Type == DCC_SEND ? TILN_SEND : TILN_RECEIVE);
  if (itemnum != -1)
  {
    m_TransfersListCtrl.AddItem(itemnum,1,pDCCTransfer->m_FileName);
    m_TransfersListCtrl.AddItem(itemnum,2,pDCCTransfer->m_OtherNick);
    m_TransfersListCtrl.AddItem(itemnum,3,pDCCTransfer->m_StatusStr);
    m_TransfersListCtrl.AddItem(itemnum,4,"");
    m_TransfersListCtrl.AddItem(itemnum,5,"");
    m_TransfersListCtrl.AddItem(itemnum,6,pDCCTransfer->m_Size > 0 ? SizeStr : "Unknown");
    m_TransfersListCtrl.AddItem(itemnum,7,stripcrlf(asctime(t)));
    m_TransfersListCtrl.AddItem(itemnum,8,pDCCTransfer->m_IPAddressString);
    if (pDCCTransfer->m_pServer && (g_ServerList.Find(pDCCTransfer->m_pServer) >= 0))
    {
      char *serverstr = HydraIRC_BuildString(128,"%s (%s)",
        pDCCTransfer->m_pServer->m_pDetails->m_Name,
        GetNetworkName(pDCCTransfer->m_pServer->m_pDetails->m_NetworkID));
      if (serverstr)
      {
        m_TransfersListCtrl.AddItem(itemnum,9,serverstr);
        free(serverstr);
      }
    }
    else
      m_TransfersListCtrl.AddItem(itemnum,9,"N/A");

    m_TransfersListCtrl.SetItemData(itemnum,(DWORD_PTR)pDCCTransfer);
  }

  TransferStats_t *pTS = (TransferStats_t *)malloc(sizeof (TransferStats_t));
  if (pTS)
  {
    ZeroMemory(pTS,sizeof (TransferStats_t));
    pTS->pDCCTransfer = pDCCTransfer;
    CNode *pNode = new CNode(pTS);
    m_TransferStats.AddTail(pNode);
    ResetStats(pDCCTransfer);
  }
}

int CTransfersView::FindItem(CDCCTransfer *pDCCTransfer)
{
  if (!::IsWindow(m_hWnd))
    return LB_ERR;

  CDCCTransfer *pFoundDCCTransfer;

  for (int i = 0 ; i < m_TransfersListCtrl.GetItemCount(); i++)
  {
    pFoundDCCTransfer = (CDCCTransfer *) m_TransfersListCtrl.GetItemData(i);
    if (pFoundDCCTransfer == pDCCTransfer)
    {
      return i;
    }
  }
  return LB_ERR;
}

void CTransfersView::RemoveItem(CDCCTransfer *pDCCTransfer)
{
  int Index = FindItem(pDCCTransfer);

  if (Index != LB_ERR)
    m_TransfersListCtrl.DeleteItem(Index);

  // remove the transfer stats from the list too.
  CNode *pNode;
  TransferStats_t *pTS;

  for (pNode = m_TransferStats.GetFirst() ; pNode->m_Next != NULL ; pNode = pNode->m_Next)
  {
    pTS = (TransferStats_t *)pNode->m_Data;
    if (pTS && pTS->pDCCTransfer == pDCCTransfer)
    {
      free(pTS);
      pNode->Delete();
      break;
    }
  }
}

void CTransfersView::UpdateItem(CDCCTransfer *pDCCTransfer)
{
  if (!pDCCTransfer->m_StatusDirty)
    return;

  int Index = FindItem(pDCCTransfer);

  if (Index == LB_ERR)
    return;

  char StatusStr[128];
  char SpeedStr[128];
  char ETAStr[128];

  double diff;
  double Speed;
  double CurrentSpeed;
  int h,m,s;
  time_t now;
  time(&now);
  
  strcpy(ETAStr,"N/A");
  strcpy(SpeedStr,"N/A");
  // we always have a StatusStr by the time we come to update the columns

  int len = 0;
  switch(pDCCTransfer->m_Status)
  {
    case DCCT_STATUS_CONNECTED:
      {
        // Build SpeedStr

        // calculate current x-fer speed.

        CurrentSpeed = 0; // Note: CurrentSpeed is in KB/Sec
        TransferStats_t *pTS = GetStats(pDCCTransfer);
        if (pTS)
        {
          int i;
          long total = 0;
          for (i = pTS->FirstSnapshot; i < TS_SNAPSHOTS_MAX ; i ++)
            total += pTS->Snapshots[i];      

          diff = difftime(pTS->SnapshotTimes[TS_SNAPSHOTS_MAX-1],pTS->SnapshotTimes[pTS->FirstSnapshot]);
          if (diff > 0 && total > 0)
          {
            CurrentSpeed = ((double)total / 1024 ) / (diff + 1);
          }

          if (CurrentSpeed > 1024)
            len = _snprintf(SpeedStr,sizeof(SpeedStr),"%.2fMB/s ", CurrentSpeed / 1024);
          else
            len = _snprintf(SpeedStr,sizeof(SpeedStr),"%.2fKB/s ", CurrentSpeed);

          // Here's how we can do it without the "SnapshotTimes", but it's not as accurate
          // if HydraIRC's process is stalled for some reason.
          /*
          int snapshots = (TS_SNAPSHOTS_MAX - pTS->FirstSnapshot);
          if (snapshots > 0)
            CurrentSpeed = ((double)total / 1024) / snapshots;
          else
            CurrentSpeed = 0;

          if (CurrentSpeed > 1024)
            len += _snprintf(SpeedStr+len,sizeof(SpeedStr)-len,"[%.2fMB/s] ", CurrentSpeed / 1024);
          else
            len += _snprintf(SpeedStr+len,sizeof(SpeedStr)-len,"[%.2fKB/s] ", CurrentSpeed);
          */
        }

        // calculate average x-fer speed.

        diff = difftime(now,pDCCTransfer->m_ResumeTime);
        Speed = 0; // Note: Speed is in KB/sec

        if (diff > 0) // avoid divide by 0 error.
        {
          // bytes transferred / time since resume started
          Speed = ((pDCCTransfer->m_Position - pDCCTransfer->m_ResumeOffset) / 1024) / diff;

        }  
        if (Speed > 1024)
          len += _snprintf(SpeedStr+len,sizeof(SpeedStr)-len,"(%.2fMB/s)", Speed / 1024);
        else
          len += _snprintf(SpeedStr+len,sizeof(SpeedStr)-len,"(%.2fKB/s)", Speed);

#ifdef DEBUG        
        if (Speed < 0 || CurrentSpeed < 0)
        {
          sys_Printf(BIC_ERROR,"Bleh!");
        }
#endif
        // Build StatusStr

        len = _snprintf(StatusStr,sizeof(StatusStr),"%s",
          pDCCTransfer->m_Type == DCC_RECEIVE ? "Received" : "Sent");

        if (pDCCTransfer->m_Position > (1024 * 1024))
          len += _snprintf(StatusStr+len,sizeof(StatusStr)-len," %.2f MB", (double)pDCCTransfer->m_Position / (1024 * 1024));
        else
          len += _snprintf(StatusStr+len,sizeof(StatusStr)-len," %.2f KB", (double)pDCCTransfer->m_Position / 1024);

        // calculate percent and ETAs, but only if we know the size
        if (pDCCTransfer->m_Size > 0)
        {
          // calculate percentage
          double Percent = ((double) pDCCTransfer->m_Position / (double)pDCCTransfer->m_Size) * 100;

          // don't show 100% unless it's *really* done!
          if (Percent ==  100 && pDCCTransfer->m_Position < pDCCTransfer->m_Size)
            Percent = 99.99f;
          len += _snprintf(StatusStr+len,sizeof(StatusStr)-len," - %.2f%%",Percent);

          // Build ETAStr

          // calculate eta
          len = 0; // used for appending strings - important, we might not get pTS (we should though...)
          int ETASecs;

          if (Speed == 0 || CurrentSpeed == 0)
          {
            _snprintf(ETAStr,sizeof(ETAStr),"Stalled");
          }
          else
          {
            if (pTS)
            {            
              ETASecs = int (((double)(pDCCTransfer->m_Size - pDCCTransfer->m_Position) / 1024) / CurrentSpeed);

              s = ETASecs;
              m = s / 60;
              s = s - (m * 60); 
              h = m / 60; 
              m = m - 60 * h;

              len = _snprintf(ETAStr,sizeof(ETAStr),"%d:%02d:%02d ",h,m,s);
            }

            ETASecs = int (((double)(pDCCTransfer->m_Size - pDCCTransfer->m_Position) / 1024) / Speed);

            s = ETASecs;
            m = s / 60;
            s = s - (m * 60); 
            h = m / 60; 
            m = m - 60 * h;

            _snprintf(ETAStr+len,sizeof(ETAStr)-len,"(%d:%02d:%02d)",h,m,s);
          }

        }

        //sys_Printf(BIC_INFO,"%.2f %.2f %s\n",CurrentSpeed, Speed, StatusStr);

      }
      break;
    case DCCT_STATUS_COMPLETE:
      {
        // show the time taken since the resume was started in the ETA column
        diff = difftime(now,pDCCTransfer->m_ResumeTime);
        if (diff == 0) 
          diff++; // always show a speed, even if the x-fer took <1 second!

        s = (int)diff;
        m = s / 60;
        s = s - (m * 60); 
        h = m / 60; 
        m = m - 60 * h;

        _snprintf(ETAStr,sizeof(ETAStr),"[%d:%02d:%02d]",h,m,s);

        // calculate the average speed of the transfer

        Speed = 0; // Note: Speed is in KB/sec

        // bytes transferred / time since resume started
        Speed = ((pDCCTransfer->m_Position - pDCCTransfer->m_ResumeOffset) / 1024) / diff;

        if (Speed > 1024)
          _snprintf(SpeedStr,sizeof(SpeedStr),"[%.2fMB/s]", Speed / 1024);
        else
          _snprintf(SpeedStr,sizeof(SpeedStr),"[%.2fKB/s]", Speed);
      }
      // And follow through...
    default:
      {
        strcpy(StatusStr,pDCCTransfer->m_StatusStr);

        sys_Printf(BIC_INFO,"DCC Transfer Status Changed: %s (%d) %s %s POS: %ld OUT: %ld IN: %ld\n",
          pDCCTransfer->m_StatusStr,
          pDCCTransfer->m_Status,
          pDCCTransfer->m_OtherNick,
          pDCCTransfer->m_Type == DCC_RECEIVE ? "RECEIVE" : "SEND",
          pDCCTransfer->m_Position,
          pDCCTransfer->m_Sent,
          pDCCTransfer->m_Received);
          
      }
      break;
  }

  int ImageIndex = pDCCTransfer->m_Type == DCC_SEND ? TILN_SEND : TILN_RECEIVE;
  switch(pDCCTransfer->m_Status)
  {
    case DCCT_STATUS_COMPLETE:
      ImageIndex += TILN_OK;
      break;
    case DCCT_STATUS_ERROR:
      ImageIndex += TILN_FAIL;
      break;
  }

  m_TransfersListCtrl.SetItem(Index,0,LVIF_IMAGE,NULL,ImageIndex,0,0,NULL);
  m_TransfersListCtrl.SetItem(Index,8,LVIF_TEXT,pDCCTransfer->m_IPAddressString,0,0,0,0);
  m_TransfersListCtrl.SetItem(Index,3,LVIF_TEXT,StatusStr,0,0,0,0);
  m_TransfersListCtrl.SetItem(Index,4,LVIF_TEXT,SpeedStr,0,0,0,0);
  m_TransfersListCtrl.SetItem(Index,5,LVIF_TEXT,ETAStr,0,0,0,0);

  if (pDCCTransfer->m_Type == DCC_SEND)
  {
    char SizeStr[10];
    BuildSizeString(SizeStr,10,(double)pDCCTransfer->m_Size);
    // it's unlikely, but the file size of the local file may have
    // changed between the time it was first offered and now.
    m_TransfersListCtrl.SetItem(Index,6,LVIF_TEXT,pDCCTransfer->m_Size > 0 ? SizeStr : "Unknown",0,0,0,0);
  }

  pDCCTransfer->m_StatusDirty = FALSE; // reset our flag.

  // redraw this item (including all unmodified sub items, so that the color changes)
  m_TransfersListCtrl.RedrawItems(Index,Index);
}

LRESULT CTransfersView::OnLvnKeyDown(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
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
      case 'R':
        {
          CDCCTransfer *pDCCTransfer;
          for (int i = 0 ; i < m_TransfersListCtrl.GetItemCount(); i ++)  
          {
            if (LVIS_SELECTED & m_TransfersListCtrl.GetItemState(i,LVIS_SELECTED))
            {
              pDCCTransfer = (CDCCTransfer *)m_TransfersListCtrl.GetItemData(i);

              if (!pDCCTransfer)
                continue;

              pDCCTransfer->Retry(); // if the transfer can be retried, then it will be..
            }
          }
        }
        break;
      case VK_DELETE:
        {
          CDCCTransfer *pDCCTransfer;
          for (int i = 0 ; i < m_TransfersListCtrl.GetItemCount(); i ++)  
          {
            if (LVIS_SELECTED & m_TransfersListCtrl.GetItemState(i,LVIS_SELECTED))
            {
              pDCCTransfer = (CDCCTransfer *)m_TransfersListCtrl.GetItemData(i);

              if (!pDCCTransfer)
                continue;

              if ((pDCCTransfer->m_Status == DCCT_STATUS_ERROR) ||
                  (pDCCTransfer->m_Status == DCCT_STATUS_COMPLETE))
              {
                // Note: deleting an instance of a CDCCTransfer calls
                // the transfer manager's RemoveTransfer, which in turn
                // calls this->RemoveItem, so me must delete the item First
                // so that the order if the list processing isn't broken.
                m_TransfersListCtrl.DeleteItem(i);
                delete pDCCTransfer;

                i --; // step back one.
              }
              else
              {
                // active dcc, just cancel it.
                pDCCTransfer->Disconnect(g_DefaultStrings[DEFSTR_Cancelled]);
              }
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


TransferStats_t *CTransfersView::GetStats(CDCCTransfer *pDCCTransfer)
{
  CNode *pNode;
  TransferStats_t *pTS;

  for (pNode = m_TransferStats.GetFirst() ; pNode->m_Next != NULL ; pNode = pNode->m_Next)
  {
    pTS = (TransferStats_t *)pNode->m_Data;
    if (pTS && pTS->pDCCTransfer == pDCCTransfer)
      return pTS;
  }
  return NULL;
}

void CTransfersView::UpdateStats(CDCCTransfer *pDCCTransfer)
{
  TransferStats_t *pTS = GetStats(pDCCTransfer);
  if (!pTS)
    return;

  // Shuffle the Snapshots.  FIFO.
  for (int i = 1 ; i < TS_SNAPSHOTS_MAX ; i ++) // note, starting at 1, not 0
  {
    pTS->Snapshots[i-1] = pTS->Snapshots[i];
    pTS->SnapshotTimes[i-1] = pTS->SnapshotTimes[i];
  }

  pTS->Snapshots[TS_SNAPSHOTS_MAX-1] = pDCCTransfer->m_Position - pTS->LastPosition;
  pTS->LastPosition = pDCCTransfer->m_Position;
  time(&(pTS->SnapshotTimes[TS_SNAPSHOTS_MAX-1]));

  // point to the first snapshot
  if (pTS->FirstSnapshot > 0) 
    pTS->FirstSnapshot--;

}

void CTransfersView::ResetStats(CDCCTransfer *pDCCTransfer)
{
  TransferStats_t *pTS = GetStats(pDCCTransfer);
  if (!pTS)
    return;

  pTS->LastPosition = pDCCTransfer->m_ResumeOffset;
  pTS->FirstSnapshot = TS_SNAPSHOTS_MAX - 1;
  // set the snapshot times.
  for (int i = 0; i < TS_SNAPSHOTS_MAX; i ++)
  {
    pTS->Snapshots[i] = 0;
    pTS->SnapshotTimes[i] = pDCCTransfer->m_ResumeTime;
  }
}

// TODO: add this functionality to the item's context menu.
LRESULT CTransfersView::OnNmDblClick(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
{
  LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE) pnmh;

  if (lpnmitem->iItem != -1)
  {
    CDCCTransfer *pDCCTransfer;
    pDCCTransfer = (CDCCTransfer *) m_TransfersListCtrl.GetItemData(lpnmitem->iItem);

    if (pDCCTransfer)      
    {
      if (pDCCTransfer->m_Type == DCC_RECEIVE)
      {
        if (pDCCTransfer->m_Status == DCCT_STATUS_COMPLETE)
        {
          // Completed Transfer

          // When a user double clicks the file, open the file, or the directory it's in
          if (BOOLPREF(PREF_bDCCReceiveDoubleClickOpensFile))
          {
            ShellExecute(NULL,"open",pDCCTransfer->m_LocalFileName,NULL,NULL,SW_SHOWNORMAL);
          }
          else // open the directory.
          {
            OpenDirectoryForFile(pDCCTransfer->m_LocalFileName);
          }
        }
        else
        {
          // Incomplete Transfer, open the directory
          OpenDirectoryForFile(pDCCTransfer->m_LocalFileName);
        }
      }
      // don't do anything for sends.
    }
  }

  return 0;
}

LRESULT CTransfersView::OnNmCustomDraw(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
  LPNMLVCUSTOMDRAW pNMH = (LPNMLVCUSTOMDRAW)pnmh;

  switch(pNMH->nmcd.dwDrawStage)
  {
    case CDDS_PREPAINT:
      return CDRF_NOTIFYITEMDRAW;

    case CDDS_ITEMPREPAINT:
      {
        CDCCTransfer *pDCCTransfer = (CDCCTransfer *)pNMH->nmcd.lItemlParam;

        //sys_Printf(BIC_INFO,"pNMH->nmcd.dwItemSpec == 0x%08x pNMH->nmcd.lItemlParam == 0x%08x\n",pNMH->nmcd.dwItemSpec, pNMH->nmcd.lItemlParam);

        // if these are the same then we're being told about column headers, if not
        // then we're being told about an item and dwItemSpec is the item index
        // gee thanks for the docs Microsoft, NOT!

        if ((DWORD) pNMH->nmcd.dwItemSpec == (DWORD) pNMH->nmcd.lItemlParam)
          return NULL; // don't care about the column headers

        if (!pDCCTransfer)
          return NULL;//CDRF_DODEFAULT ?

        switch(pDCCTransfer->m_Status)
        {
          case DCCT_STATUS_ERROR:
            pNMH->clrText = COLORVAL(item_transferlisterrortext);
            break;
          case DCCT_STATUS_COMPLETE:
            pNMH->clrText = COLORVAL(item_transferlistcompletetext);
            break;
          default:
            pNMH->clrText = COLORVAL(item_transferlistnormaltext);
            break;
        }
        pNMH->clrTextBk = COLORVAL(item_transferlistbackground);

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


LRESULT CTransfersView::OnNmRClick(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
{
  /*
  int MenuID = -1;

  LVHITTESTINFO ht = {0};
  DWORD dwpos = GetMessagePos();
  POINT pt;
  pt.x = GET_X_LPARAM(dwpos);
  pt.y = GET_Y_LPARAM(dwpos);
  ht.pt.x = pt.x;
  ht.pt.y = pt.y;
  ::MapWindowPoints(HWND_DESKTOP, m_TransfersListCtrl.m_hWnd, &ht.pt, 1);
  m_TransfersListCtrl.HitTest(&ht);

  if(LVHT_ONITEM & ht.flags)
  {
    CDCCTransfer *pDCCTransfer = (CDCCTransfer *)m_TransfersListCtrl.GetItemData(ht.iItem);
    if (pDCCTransfer)
    {
      if (pDCCTransfer->m_Type == DCC_RECEIVE)
        MenuID = IDR_DCCRECEIVE_CONTEXT;
      else
        MenuID = IDR_DCCSEND_CONTEXT;
    }
  }

  // Display the menu at the current mouse location.
	CMenuHandle menuContext;
	menuContext.LoadMenu(MenuID);
	CMenuHandle menuPopup(menuContext.GetSubMenu(0));
  //menuPopup.SetMenuDefaultItem(0,TRUE);
  g_pMainWnd->m_CmdBar.TrackPopupMenu(menuPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y);
  */

  CDCCTransfer *pDCCTransfer;

  CMenuHandle Menu;
  Menu.CreatePopupMenu();

  // count how many selected items we have and make a note of
  // what type all the slected items are, and if we have more than
  // one type selected. (e.g. all DCC Sends, or mixed Sends and Receives)
  // and if they're all the same status, and what that status is.
  // and if they can all be retried.

  BOOL First = TRUE;
  int DCCType = DCC_ANY;
  int DCCStatus = DCCT_STATUS_UNKNOWN;
  BOOL DCCCanRetry = FALSE;

  int SelectedCount = 0;
  for (int i = 0 ; i < m_TransfersListCtrl.GetItemCount(); i ++)  
  {
    if (LVIS_SELECTED & m_TransfersListCtrl.GetItemState(i,LVIS_SELECTED))
    {
      SelectedCount++;
      pDCCTransfer = (CDCCTransfer *)m_TransfersListCtrl.GetItemData(i);

      if (!pDCCTransfer)
        continue;

      if (First)
      {
        // make a note of the first selected type
        DCCType = pDCCTransfer->m_Type;
        DCCStatus = pDCCTransfer->m_Status;
        DCCCanRetry = pDCCTransfer->m_CanRetry;
        First = FALSE;
      }
      else
      {
        // can they all be retried?
        if (pDCCTransfer->m_CanRetry != DCCCanRetry)

        // more than one type selected?
        if (pDCCTransfer->m_Type != DCCType)
        {
          DCCType = DCC_ANY;
          DCCStatus = DCCT_STATUS_UNKNOWN; 
        }
        else
        {
          // same type, but more than one status selected?
          if (pDCCTransfer->m_Status != DCCStatus)
            DCCStatus = DCCT_STATUS_UNKNOWN; 
        }
      }
    }
  }

  // Build the menu adding items based on what's selected
  if (SelectedCount > 0)
  {
//+ <gliptic> Accept option
    if (pDCCTransfer->m_Status == DCCT_STATUS_PENDING)
      Menu.AppendMenu(MF_STRING,TRMI_Accept,_T("&Accept"));
// </gliptic>

    if (!((pDCCTransfer->m_Status == DCCT_STATUS_ERROR) ||
         (pDCCTransfer->m_Status == DCCT_STATUS_COMPLETE)))
      Menu.AppendMenu(MF_STRING,TRMI_Cancel,_T("&Cancel"));

    if (DCCCanRetry)
      Menu.AppendMenu(MF_STRING,TRMI_Retry,_T("&Retry\tR"));

    Menu.AppendMenu(MF_STRING,TRMI_Remove,_T("R&emove"));

    Menu.AppendMenu(MF_SEPARATOR);
  }

  Menu.AppendMenu(MF_STRING,TRMI_CancelAllSends,    _T("Cancel All Sends"));
  Menu.AppendMenu(MF_STRING,TRMI_CancelAllReceives, _T("Cancel All Receives"));
  Menu.AppendMenu(MF_STRING,TRMI_CancelAll,         _T("Cancel All"));
  Menu.AppendMenu(MF_SEPARATOR);

  Menu.AppendMenu(MF_STRING,TRMI_RemoveAll,         _T("Remove All"));
  Menu.AppendMenu(MF_STRING,TRMI_RemoveAllCompleted,_T("Remove All Completed"));
  Menu.AppendMenu(MF_STRING,TRMI_RemoveAllFailed,   _T("Remove All Failed"));
  Menu.AppendMenu(MF_SEPARATOR);

  Menu.AppendMenu(MF_STRING,TRMI_SelectAll,         _T("Select All\tCtrl+A"));
  Menu.AppendMenu(MF_STRING,TRMI_SelectOnlyCompleted,_T("Select Only Completed"));
  Menu.AppendMenu(MF_STRING,TRMI_SelectOnlyFailed,   _T("Select Only Failed"));
  Menu.AppendMenu(MF_STRING,TRMI_SelectNone,        _T("Select &None"));

  POINT pt;
  ::GetCursorPos(&pt);      
  g_pMainWnd->m_CmdBar.TrackPopupMenu(Menu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y);
  //::TrackPopupMenu(Menu, 0, pt.x, pt.y, 0, m_hWnd, NULL);

  return bHandled ? 1 : 0; // NM_RCLICK - Return nonzero to not allow the default processing, or zero to allow the default processing
  // or in otherwords, return zero to allow the default processing
  // or 1 if handled.
}

LRESULT CTransfersView::OnMenuItemSelected(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
  DoAction(wID);
  return 0;
}

void CTransfersView::DoAction( int Action )
{
  CDCCTransfer *pDCCTransfer;
  for (int i = 0 ; i < m_TransfersListCtrl.GetItemCount(); i ++)  
  {
    pDCCTransfer = (CDCCTransfer *)m_TransfersListCtrl.GetItemData(i);

    if (!pDCCTransfer)
      continue;

    if (LVIS_SELECTED & m_TransfersListCtrl.GetItemState(i,LVIS_SELECTED))
    {
      // actions for selected items
      switch (Action)
      {
        // reset the selection status for these three items.
        case TRMI_SelectNone:
        case TRMI_SelectOnlyCompleted:
        case TRMI_SelectOnlyFailed:
          m_TransfersListCtrl.SetItemState(i, 0, LVIS_SELECTED);
          break;

        case TRMI_Retry:
          pDCCTransfer->Retry();
          break;

        case TRMI_Accept:
          pDCCTransfer->Accept();
          break;

        case TRMI_Cancel:
          if (!((pDCCTransfer->m_Status == DCCT_STATUS_ERROR) ||
               (pDCCTransfer->m_Status == DCCT_STATUS_COMPLETE)))
          {
            // active dcc, just cancel it.
            pDCCTransfer->Disconnect(g_DefaultStrings[DEFSTR_Cancelled]);
          }
          break;

        case TRMI_Remove:
          if ((pDCCTransfer->m_Status == DCCT_STATUS_ERROR) ||
              (pDCCTransfer->m_Status == DCCT_STATUS_COMPLETE))
          {
            // Note: deleting an instance of a CDCCTransfer calls
            // the transfer manager's RemoveTransfer, which in turn
            // calls this->RemoveItem, so me must delete the item First
            // so that the order if the list processing isn't broken.
            m_TransfersListCtrl.DeleteItem(i);
            delete pDCCTransfer;
            i --; // step back one.
          }
          break;
      }
    }
    else
    {
      // do actions for non-selected items
      switch (Action)
      {
        case TRMI_SelectAll:
          m_TransfersListCtrl.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
          break;
      }
    }

    // do actions for selected/non-selected items
    switch (Action)
    {
      case TRMI_SelectOnlyCompleted:
        if (pDCCTransfer->m_Status == DCCT_STATUS_COMPLETE)
          m_TransfersListCtrl.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
        break;

      case TRMI_SelectOnlyFailed:
        if (pDCCTransfer->m_Status == DCCT_STATUS_ERROR)
          m_TransfersListCtrl.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
        break;

      case TRMI_CancelAll:
          if (!((pDCCTransfer->m_Status == DCCT_STATUS_ERROR) ||
               (pDCCTransfer->m_Status == DCCT_STATUS_COMPLETE)))
          {
            // active dcc, just cancel it.
            pDCCTransfer->Disconnect(g_DefaultStrings[DEFSTR_Cancelled]);
          }
          break;
      case TRMI_CancelAllSends:
          if (!((pDCCTransfer->m_Status == DCCT_STATUS_ERROR) ||
               (pDCCTransfer->m_Status == DCCT_STATUS_COMPLETE)) &&
               (pDCCTransfer->m_Type == DCC_SEND ))
          {
            // active dcc, just cancel it.
            pDCCTransfer->Disconnect(g_DefaultStrings[DEFSTR_Cancelled]);
          }
          break;
      case TRMI_CancelAllReceives:
          if (!((pDCCTransfer->m_Status == DCCT_STATUS_ERROR) ||
               (pDCCTransfer->m_Status == DCCT_STATUS_COMPLETE)) &&
               (pDCCTransfer->m_Type == DCC_RECEIVE ))
          {
            // active dcc, just cancel it.
            pDCCTransfer->Disconnect(g_DefaultStrings[DEFSTR_Cancelled]);
          }
          break;

      case TRMI_RemoveAll:
        if ((pDCCTransfer->m_Status == DCCT_STATUS_ERROR) ||
            (pDCCTransfer->m_Status == DCCT_STATUS_COMPLETE))
        {
          m_TransfersListCtrl.DeleteItem(i); // order is important, see TRMI_Remove above
          delete pDCCTransfer;
          i --; // step back one.
        }
        break;
      case TRMI_RemoveAllCompleted:
        if (pDCCTransfer->m_Status == DCCT_STATUS_COMPLETE)
        {
          m_TransfersListCtrl.DeleteItem(i); // order is important, see TRMI_Remove above
          delete pDCCTransfer;
          i --; // step back one.
        }
        break;
      case TRMI_RemoveAllFailed:
        if (pDCCTransfer->m_Status == DCCT_STATUS_ERROR)
        {
          m_TransfersListCtrl.DeleteItem(i); // order is important, see TRMI_Remove above
          delete pDCCTransfer;
          i --; // step back one.
        }
        break;
    }
  }

}

void CTransfersView::OnEvent(int EventID, void *pData)
{
  switch(EventID)
  {
    case EV_PREFSCHANGED:
      UpdateSettings();
      break;
  }
}

void CTransfersView::UpdateSettings( void )
{
  if (g_pPrefs)
  {

    m_TransfersListCtrl.SetBkColor(COLORVAL(item_transferlistbackground));    
  }
}
