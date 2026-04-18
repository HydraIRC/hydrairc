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

// CTransfersFrm.h : interface of the CTransfersView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CDCCTransfer; // forward declare

enum TransferMenuIDs
{
  TRMI_Retry = ID_TRANSFERSMENU_FIRST,
  TRMI_Accept,
  TRMI_Cancel,
  TRMI_Remove,

  TRMI_CancelAllSends,
  TRMI_CancelAllReceives,
  TRMI_CancelAll,

  TRMI_RemoveAll,
  TRMI_RemoveAllCompleted,
  TRMI_RemoveAllFailed,

  TRMI_SelectAll,
  TRMI_SelectOnlyCompleted,
  TRMI_SelectOnlyFailed,
  TRMI_SelectNone,

  TRMI_LAST = ID_TRANSFERSMENU_LAST
};

#define TS_SNAPSHOTS_MAX 10

typedef struct TransferStats_s
{
  CDCCTransfer *pDCCTransfer;
  int Snapshots[TS_SNAPSHOTS_MAX]; // here we store per EV_TICK samples of bytes transferred since the last EV_TICK
  time_t SnapshotTimes[TS_SNAPSHOTS_MAX]; // and the times of the snapshots (only the first and last are used in the calculation of current speed)
  int LastPosition;
  short FirstSnapshot;
  // TODO: fserve/xdcc server scripts will probably need to have access to the current speed
  // so that can record speeds and drop connections that are under a user defined speed
  // so maybe we'll need to move more things here, or just add a plugin api call that
  // can return some related information
  // maybe we also need to give each transfer a specific id.

} TransferStats_t;


#define IDC_TRANSFERS_LIST 999

class CTransfersView :
  public CListener, 
	public dockwins::CBoxedDockingWindowImpl< CTransfersView,CWindow,dockwins::CVC7LikeExBoxedDockingWindowTraits>
{
	typedef CTransfersView	thisClass;
	typedef dockwins::CBoxedDockingWindowImpl< CTransfersView,CWindow,dockwins::CVC7LikeExBoxedDockingWindowTraits> baseClass;

protected:
	DWORD m_dwIcon;

public:
  CListViewCtrl m_TransfersListCtrl;
  CList m_TransferStats;

	CTransfersView(DWORD dwIcon = IDR_MAINFRAME,CEventManager *pEventManager = NULL)
		: m_dwIcon(dwIcon),
      CListener(pEventManager)
	{
	}

  ~CTransfersView( void );

  void AddItem(CDCCTransfer *pDCCTransfer);
  void RemoveItem(CDCCTransfer *pDCCTransfer);
  void UpdateItem(CDCCTransfer *pDCCTransfer);
  void ResetStats(CDCCTransfer *pDCCTransfer); // called when a transfer is actually started.
  TransferStats_t *GetStats(CDCCTransfer *pDCCTransfer);
  void UpdateStats(CDCCTransfer *pDCCTransfer);

  void UpdateSettings( void );
  void OnEvent(int EventID, void *pData);

	BEGIN_MSG_MAP(CTransfersView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
    MESSAGE_HANDLER(WM_SIZE, OnSize)
    NOTIFY_CODE_HANDLER(NM_CUSTOMDRAW, OnNmCustomDraw);
    NOTIFY_CODE_HANDLER(LVN_KEYDOWN, OnLvnKeyDown)
    NOTIFY_CODE_HANDLER(NM_DBLCLK, OnNmDblClick)
    NOTIFY_CODE_HANDLER(NM_RCLICK, OnNmRClick)
 		MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
    COMMAND_RANGE_HANDLER(ID_TRANSFERSMENU_FIRST, ID_TRANSFERSMENU_LAST, OnMenuItemSelected)
    CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()

  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
  LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);

  LRESULT OnLvnKeyDown(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled);
  LRESULT OnNmDblClick(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
  LRESULT OnNmRClick(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);

  LRESULT OnMenuItemSelected(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

  LRESULT OnNmCustomDraw(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

  LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
  {
    MINMAXINFO *MMI = (MINMAXINFO*)lParam;
    MMI->ptMinTrackSize.x=100;
    MMI->ptMinTrackSize.y=50;
    return 0;
  }

private:
  CImageList m_images_transfer;
  int FindItem(CDCCTransfer *pDCCTransfer);
  void DoAction( int Action );

};
