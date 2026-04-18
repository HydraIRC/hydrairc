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

// Transfers.cpp : implementation of the TransfersManager class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

TransfersManager::TransfersManager( CEventManager *pEventmanager )
  : CListener(pEventmanager)
{
  m_pTransfersView = new CTransfersView(IDI_TRANSFERS, &g_EventManager);
  m_UpdateTicks = 0;
}

TransfersManager::~TransfersManager( void )
{
  // delete all CDCCTransfers instances, which
  // closes all socket connections and files.
  while (m_TransferQueue.GetSize() > 0)
  {
    delete m_TransferQueue[0];
  }

  delete m_pTransfersView;
}

void TransfersManager::ReceiveFile(IRCServer *pServer,char *From, char *FileName, unsigned long Address, int Port, unsigned long Size)
{
  // TODO: check we're not receiving the same file from someone else
  // we might be if the user queues themselves up on several XDCC bots.
  // if we are already receiving it, we just do nothing, and the remote
  // host will eventually time out.
  CDCCTransfer *pDCCT = new CDCCTransfer(pServer,From,FileName,Address,Port,Size,DCC_RECEIVE);
}

void TransfersManager::QueueFileSend(IRCServer *pServer,char *To, char *FileName, int QueueFlags /* = 0 */)
{
  // TODO: actually queue it!  for now we just send ASAP.
  // The queue can have various critera, e.g:
  // 1) max sends allowed
  // 2) don't send if outgoing bytes per second is more than x
  // 3) send x files at a time to a given user
  // the flags paramater is to allow the user/scripts to override/adjust the queue positioning

  CDCCTransfer *pDCCT = new CDCCTransfer(pServer,To,FileName,0,0,0,DCC_SEND);
}

void TransfersManager::Resume(IRCServer *pServer,char *From, int Port, long Offset)
{
  CDCCTransfer *pDCCTransfer;
  for (int i = 0 ; i < m_TransferQueue.GetSize() ; i ++)
  {
    pDCCTransfer = m_TransferQueue[i];
    if (pDCCTransfer->m_Port == Port)
    {
      pDCCTransfer->Resume(Offset);
    }
  }
}

void TransfersManager::AddTransfer(CDCCTransfer *pDCCTransfer)
{
  if (!pDCCTransfer)
    return;

  m_TransferQueue.Add(pDCCTransfer);
  m_pTransfersView->AddItem(pDCCTransfer);
}

void TransfersManager::RemoveTransfer(CDCCTransfer *pDCCTransfer)
{
  if (!pDCCTransfer)
    return;
  
  m_pTransfersView->RemoveItem(pDCCTransfer);
  m_TransferQueue.Remove(pDCCTransfer);
}

LRESULT TransfersManager::OnTransferEvent(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  // TODO: something
  int Event = WSAGETSELECTEVENT(lParam);
  int Err = WSAGETSELECTERROR(lParam);

  // Forward the net event to the right transfer
  CDCCTransfer *pDCCTransfer;
  for (int i = 0 ; i < m_TransferQueue.GetSize() ; i ++)
  {
    pDCCTransfer = m_TransferQueue[i];
    if (pDCCTransfer->m_pTransferSocket != NULL && 
        (SOCKET)wParam == pDCCTransfer->m_pTransferSocket->GetSocket())
    {
      pDCCTransfer->OnDCCTransferSocketEvent(Event,Err);
      break;
    }
    if (pDCCTransfer->m_pServerSocket != NULL && 
        (SOCKET)wParam == pDCCTransfer->m_pServerSocket->GetSocket())
    {
      pDCCTransfer->OnDCCServerSocketEvent(Event,Err);
      break;
    }
  }

  bHandled = TRUE;
  return 0;
}

void TransfersManager::UpdateStatus(CDCCTransfer *pDCCTransfer)
{
  m_pTransfersView->UpdateItem(pDCCTransfer);
}

void TransfersManager::CheckAndUpdateAll( void )
{
  CDCCTransfer *pDCCTransfer;
  for (int i = 0 ; i < m_TransferQueue.GetSize(); i ++)
  {
    pDCCTransfer = m_TransferQueue[i];
    pDCCTransfer->Check();
    m_pTransfersView->UpdateStats(pDCCTransfer); // stats, not status :)

    // Update every X seconds.
    m_UpdateTicks++;
    if (m_UpdateTicks >= INTPREF(PREF_nTransferViewUpdateFrequency))
    {
      m_UpdateTicks = 0;
      m_pTransfersView->UpdateItem(pDCCTransfer);
    }
  }
}

LRESULT TransfersManager::OnProcessDCCs(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  int ActiveDCCSends = 0;
  CDCCTransfer *pDCCTransfer;
  for (int i = 0 ; i < m_TransferQueue.GetSize(); i ++)
  {
    pDCCTransfer = m_TransferQueue[i];
    if (pDCCTransfer->ProcessFastDCCSend())    
      ActiveDCCSends++;
  }
  
  if (ActiveDCCSends != 0)
    g_pMainWnd->PostMessage(WM_PROCESSDCCS,0,0); // come back here soon!

  bHandled = TRUE;
  return 0;
}

int TransfersManager::GetActiveTransferCount( int Type )
{
  CDCCTransfer *pDCCTransfer;
  int Count = 0;
  for (int i = 0 ; i < m_TransferQueue.GetSize(); i ++)
  {
    pDCCTransfer = m_TransferQueue[i];
    if (pDCCTransfer->IsActive() &&
        (Type == DCC_ANY || pDCCTransfer->m_Type == Type))
      Count ++;
  }
  return Count;
}

void TransfersManager::OnEvent(int EventID, void *pData)
{
  switch(EventID)
  {
    case EV_TICK:
      {
        CheckAndUpdateAll();
      }
      break;
  }
}

// return a pointer to an active transfer using this port, or NULL
// used when doing send resumes and allocating a port.
CDCCTransfer *TransfersManager::FindTransferByPort(unsigned short Port, CDCCTransfer *pIgnore /* = NULL */)
{
  CDCCTransfer *pDCCTransfer;
  for (int i = 0 ; i < m_TransferQueue.GetSize() ; i ++)
  {
    pDCCTransfer = m_TransferQueue[i];

    if (pDCCTransfer == pIgnore)
      continue;

    if (pDCCTransfer->m_Port == Port &&  // correct port?
        pDCCTransfer->IsActive()) // active?
      return pDCCTransfer;
  }
  return NULL;
}

BOOL TransfersManager::IsDuplicate(CDCCTransfer *pNewTransfer)
{
  CDCCTransfer *pDCCTransfer;
  for (int i = 0 ; i < m_TransferQueue.GetSize() ; i ++)
  {
    pDCCTransfer = m_TransferQueue[i];

    if (pDCCTransfer == pNewTransfer)
      continue; // ignore ourself

    if (pNewTransfer->m_Type != DCC_RECEIVE)
      continue; // ignore anything other than dcc receives.

    if (pDCCTransfer->m_Type != pNewTransfer->m_Type)
      continue; // ignore transfers of different types

    // check for same local filename..
    if (stricmp(pDCCTransfer->m_LocalFileName,pNewTransfer->m_LocalFileName) == 0)
    {
      // see if other transfer is currently active
      if (!pDCCTransfer->IsActive())
        continue; // ignore this one, it's not active.

      // ooh!  this transfer is active, the new one must be a duplicate
      return TRUE;
    }
  }
  return FALSE;
}