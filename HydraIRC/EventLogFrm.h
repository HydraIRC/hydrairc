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
  3.2) Will become the property of HydraIRC's author.

*/

// CNotificationFrm.h : interface of the CEventLog class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#define IDC_EVENTLOGVIEW_LIST 998

class EventLogManager;
class EventLogItem;

enum EventLogFilterGroupIDs
{
  ELFI_NONE = 0,
  ELFI_FIRST,
  ELFI_Notices = ELFI_FIRST, // marker
  ELFI_Messages,
  ELFI_Highlights,
  ELFI_Connects,
  ELFI_Disconnects,
  ELFI_Transfers,
  ELFI_CTCPs,
  ELFI_Others,
  ELFI_LAST = ELFI_Others // marker
};

enum EventLogMenuIDs
{
  ELMI_Action = ID_EVENTLOGMENU_FIRST,
  ELMI_CopyTime,
  ELMI_CopyContent,
  ELMI_Delete,
  ELMI_DeleteAll,
  ELMI_SelectAll,

  ELMI_FIRSTSHOWACTION, // marker
  ELMI_ShowNone = ELMI_FIRSTSHOWACTION,
  ELMI_ShowAll,
  ELMI_FIRSTSHOWFILTER = ELMI_ShowAll + ELFI_FIRST,
  ELMI_ShowNotices = ELMI_FIRSTSHOWFILTER,  // these *** MUST *** be in the same order and include all of EventLogFilterGroupIDs
  ELMI_ShowMessages,
  ELMI_ShowHighlights,
  ELMI_ShowConnects,
  ELMI_ShowDisconnects,
  ELMI_ShowTransfers,
  ELMI_ShowCTCPs,
  ELMI_ShowOther,
  ELMI_LASTSHOWACTION = ELMI_ShowOther, // marker

  ELMI_LAST = ID_EVENTLOGMENU_LAST
};

class EventLogFilterGroup
{
public:
  int m_FilterID;
  intarray m_EventLogIDList;

  void AddList(int *IDList) // list ends with -1
  {
    int *pID = IDList;

    while (*pID != -1)
    {
      m_EventLogIDList.Add(*pID);
      pID++;
    }
  }

  EventLogFilterGroup(int FilterID, int *IDList)
  {
    m_FilterID = FilterID;
    AddList(IDList);
  }

  ~EventLogFilterGroup()
  {
    m_EventLogIDList.RemoveAll();
  }
};


class CEventLogView :
  public CListener, 
	public dockwins::CBoxedDockingWindowImpl< CEventLogView,CWindow,dockwins::CVC7LikeExBoxedDockingWindowTraits>
{
	typedef CEventLogView	thisClass;
	typedef dockwins::CBoxedDockingWindowImpl< CEventLogView,CWindow,dockwins::CVC7LikeExBoxedDockingWindowTraits> baseClass;

protected:
	DWORD m_dwIcon;

private:

  intarray m_IntFilterList;
  int *m_FilterList; // a SimpleFilter Note: when NULL nothing is filtered!
  CSimpleArray<EventLogFilterGroup *> m_FilterListGroups;
  
  intarray m_ActiveFilterGroupIDs; // contains a list of EventLogFilterGroupIDs
                                   // TODO: need to update this list when/if we
                                   // allow the capability to add/remove individual
                                   // EventLogIDs to the m_IntFilterList
                                   
  
  //IDManager m_EventLogMenuIDs;
  CListViewCtrl m_EventLogListCtrl;

  void DoDefaultAction( EventLogItem *pELI );
  void DoAction( int Action );

  void InitFilterGroups( void );
  void RefreshItems( void );

public:
  EventLogManager *m_pEventLogMgr;
 
  CEventLogView( EventLogManager *pEventLogMgr, DWORD dwIcon = IDR_MAINFRAME, CEventManager *pEventManager = NULL );
  ~CEventLogView( void );

  void AddItem(EventLogItem *pELI, BOOL AutoSize);
  void RemoveItem(EventLogItem *pELI);
  void AutoSizeColumns( void );

  void UpdateSettings( void );
  void OnEvent(int EventID, void *pData);


	BEGIN_MSG_MAP(CEventLogView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
    MESSAGE_HANDLER(WM_SIZE, OnSize)
    NOTIFY_CODE_HANDLER(NM_CUSTOMDRAW, OnNmCustomDraw);
    NOTIFY_CODE_HANDLER(LVN_KEYDOWN, OnLvnKeyDown)
    NOTIFY_CODE_HANDLER(NM_DBLCLK, OnNmDblClick)
    NOTIFY_CODE_HANDLER(NM_RCLICK, OnNmRClick)
    MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
    COMMAND_RANGE_HANDLER(ID_EVENTLOGMENU_FIRST, ID_EVENTLOGMENU_LAST, OnMenuItemSelected)

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

};
