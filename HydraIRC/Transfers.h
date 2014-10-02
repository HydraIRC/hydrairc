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
