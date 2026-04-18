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

// Transfers.h : interface of the TransfersManager class
//
/////////////////////////////////////////////////////////////////////////////

#define DCC_ANY     0
#define DCC_RECEIVE 1
#define DCC_SEND    2

class TransfersManager : 
  public CListener
{
private:
  int m_UpdateTicks;
public:
  CTransfersView *m_pTransfersView;
  CSimpleArray<CDCCTransfer *> m_TransferQueue;

  TransfersManager( CEventManager *pEventmanager );
  //TransfersManager( void );
  ~TransfersManager( void );
  //void DisplayTransfersWindow( void );
  //void CloseTransfersWindow( void );
  void AddTransfer(CDCCTransfer *pDCCTransfer);
  void RemoveTransfer(CDCCTransfer *pDCCTransfer);
  void UpdateStatus(CDCCTransfer *pDCCTransfer);
  void CheckAndUpdateAll( void );
  int GetActiveTransferCount( int Type ); // see DCC_* defines for Type
  void ReceiveFile(IRCServer *pServer,char *From, char *FileName, unsigned long Address, int Port, unsigned long Size);
  void Resume(IRCServer *pServer,char *From, int Port, long Offset);
  void QueueFileSend(IRCServer *pServer,char *To, char *FileName, int QueueFlags = 0 );
  LRESULT OnTransferEvent(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnProcessDCCs(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  void OnEvent(int EventID, void *pData);
  CDCCTransfer *FindTransferByPort(unsigned short Port, CDCCTransfer *pIgnore = NULL );

  BOOL IsDuplicate(CDCCTransfer *pNewTransfer);
};
