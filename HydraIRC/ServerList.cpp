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

// ServerList.cpp : implementation of the CServerListView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

#define MAX_SERVERLIST_ITEM_LEN 64
// sort order for server groups
#define GROUPORDER_ID_CHANNELS 0
#define GROUPORDER_ID_PM       1
#define GROUPORDER_ID_DCCCHATS 2
#define GROUPORDER_ID_QUERIES  3
// ... add more here

// STII = Serverlist Tree Item Info (Flags)
// we store a combination of these bits in each TreeItemInfo's m_Flags member
#define STII_NONE    (0)
//#define STII_DIRTY   (1<<0)

// STI = Serverlist Tree Item (Used by Adds Finds Removes and Updates)
#define STI_SERVER  1
#define STI_CHANNEL 2
#define STI_DCCCHAT 3
#define STI_QUERY   4

// this little helper class is used by Adds, Finds and Removes.
class STIInfo
{
public:
  void *m_ItemData;
  int m_ItemType;
  IRCServer *m_pServer;
  IRCChannel *m_pChannel;
  IRCQuery *m_pQuery;
  int m_iconindices[4];
  int m_TIIGroup;
  int m_TIIItem;
  int m_TreeSortValue;
  char *m_GroupName;
  char *m_ItemName;


  void BuildName( void )
  {
    if (m_ItemName) free(m_ItemName);
    m_ItemName = NULL;

    switch(m_ItemType)
    {
      case STI_SERVER:
        {
          IRCNetworkDetails *pND = GetNetworkDetails(m_pServer->m_pDetails->m_NetworkID);
          if (pND)
            m_ItemName = HydraIRC_BuildString(MAX_SERVERLIST_ITEM_LEN,"%s (%s)",pND->m_Name, m_pServer->m_pDetails->m_Name); 
          else
            m_ItemName = strdup(m_pServer->m_pDetails->m_Name);
        }
        break;
      case STI_CHANNEL:
        m_ItemName = strdup(m_pChannel->m_pProperties->m_Name);
        break;
      case STI_DCCCHAT:
        m_ItemName = strdup(m_pServer->m_OtherNick);
        break;
      case STI_QUERY:
        m_ItemName = strdup(m_pQuery->m_OtherNick);
        break;
    }
  }

  ~STIInfo( void )
  {
    if (m_ItemName) free(m_ItemName);
  }

  STIInfo( int ItemType, void *pItemData )
  {
    // default these two..
    m_ItemType = ItemType;
    m_ItemData = pItemData;
    m_iconindices[0] = ICON_CHANNELS;
    m_iconindices[1] = ICON_CHANNELS;
    m_ItemName = NULL; // initialise this one.
    
    switch(ItemType)
    {
      case STI_SERVER:
        m_pServer = (IRCServer *)pItemData;
        m_iconindices[2] = ICON_SERVER;
        m_iconindices[3] = ICON_SERVER;
        m_TIIItem = TII_IRCSERVER;
        break;
      case STI_CHANNEL:
        m_pChannel = (IRCChannel *)pItemData;
        m_iconindices[2] = ICON_CHANNEL;
        m_iconindices[3] = ICON_CHANNEL;
        m_TIIGroup = TII_CHANNELGROUP;
        m_TIIItem = TII_IRCCHANNEL;
        m_GroupName = "Channels";
        m_TreeSortValue = GROUPORDER_ID_CHANNELS;
        break;
      case STI_DCCCHAT:
        m_pServer = (IRCServer *)pItemData;
        m_iconindices[2] = ICON_DCCCHAT;
        m_iconindices[3] = ICON_DCCCHAT;
        m_TIIGroup = TII_DCCCHATGROUP;
        m_TIIItem = TII_DCCCHAT;
        m_GroupName = "DCC Chats";
        m_TreeSortValue = GROUPORDER_ID_DCCCHATS;
        break;
      case STI_QUERY:
        m_pQuery = (IRCQuery *)pItemData;
        m_iconindices[2] = ICON_QUERY;
        m_iconindices[3] = ICON_QUERY;
        m_TIIGroup = TII_QUERYGROUP;
        m_TIIItem = TII_QUERY;
        m_GroupName = "Queries";
        m_TreeSortValue = GROUPORDER_ID_QUERIES;
        break;
#ifdef DEBUG
      default:
        ATLASSERT(0); // Unknown Item!
#endif    
    }
    BuildName();
  }
};

void CServerListView::OnDocked(HDOCKBAR hBar,bool bHorizontal)
{
	DWORD dwStyle = GetWindowLong(GWL_STYLE)&(~WS_SIZEBOX);
	SetWindowLong( GWL_STYLE, dwStyle);

	baseClass::OnDocked(hBar,bHorizontal);
}
void CServerListView::OnUndocked(HDOCKBAR hBar)
{
	DWORD dwStyle = GetWindowLong(GWL_STYLE) | WS_SIZEBOX;
	SetWindowLong( GWL_STYLE , dwStyle);

	baseClass::OnUndocked(hBar);
}

LRESULT CServerListView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(m_dwIcon), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	HWND tree = m_tree.Create(m_hWnd,NULL,NULL,
					TVS_HASBUTTONS | TVS_SHOWSELALWAYS | 
					TVS_HASLINES | // TVS_LINESATROOT | 
					TVS_DISABLEDRAGDROP | TVS_INFOTIP |
					WS_CHILD | WS_VISIBLE);
  m_tree.SendMessage(CCM_SETVERSION,5,0); // to fix clipped text
	m_tree.SetImageList(g_pMainWnd->m_images_serverlist, TVSIL_NORMAL);
  m_pToolTip = m_tree.GetToolTips();

  UpdateSettings();

  /*
  // FIXME: tooltips show immediately, not after a short delay
  // Hmm, it seems that these are actually "INFOTIPS" and not "TOOLTIPS"
  // any they don't work the same way, not even in IE6's history panel...
  if (m_pToolTip)
  {
    int n;

    // this correctly disables the tooltips, so at least we know pToolTip is valid...
    // pToolTip.Activate(FALSE); 
    
    // verify the delay values are correct (they are...)
    n = pToolTip.GetDelayTime(TTDT_INITIAL);
    n = pToolTip.GetDelayTime(TTDT_AUTOPOP);
    n = pToolTip.GetDelayTime(TTDT_RESHOW);
    

    // how about disabling hot tracking?  no, still n effect
    TOOLINFO ToolInfo;
    ToolInfo.hwnd = pToolTip.m_hWnd;
    ToolInfo.cbSize = sizeof(ToolInfo);
    pToolTip.TrackActivate(&ToolInfo,FALSE);
  }
  */

	return 0;
}
LRESULT CServerListView::OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	if(wParam != SIZE_MINIMIZED )
	{
		RECT rc;
		GetClientRect(&rc);
		::SetWindowPos(m_tree.m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top ,SWP_NOZORDER | SWP_NOACTIVATE);
	}
	bHandled = FALSE;
  return bHandled ? 0 : 1; // WM_SIZE - If an application processes this message, it should return zero.
}

HTREEITEM CServerListView::FindItem(int ItemType, void *pItemData)
{
  STIInfo STII(ItemType,pItemData);

  HTREEITEM hServerItem;
  TVITEM item;
  TreeItemInfo *pTII;
  if (IsWindow())
  {
    if (ItemType == STI_SERVER)
    {
      // Server's go in the root of the tree...

      // Find the server that the channel is in.
      hServerItem = m_tree.GetRootItem();
      do
      {
        item.mask = TVIF_PARAM;
        item.lParam = 0;
        item.hItem = hServerItem; // we want more info about this tree node.
        if (m_tree.GetItem(&item))
        {
          pTII = (TreeItemInfo *)item.lParam;
          if (pTII->m_Type == STII.m_TIIItem && pTII->m_Data == STII.m_ItemData)
          {
            return hServerItem;
          }
        }
        hServerItem = m_tree.GetNextSiblingItem(hServerItem);
      } while (hServerItem);
    } // End: Server
    else
    {
      HTREEITEM hChildItem;
      HTREEITEM hSubChildItem;

      switch (ItemType)
      {
        case STI_DCCCHAT:
          hServerItem = FindItem(STI_SERVER,(void *)STII.m_pServer->m_pOtherServer);
          break;
        case STI_QUERY:
          hServerItem = FindItem(STI_SERVER,(void *)STII.m_pQuery->m_pServer);
          break;
        case STI_CHANNEL:
          hServerItem = FindItem(STI_SERVER,(void *)STII.m_pChannel->m_pServer);
          break;
      }
      
      if (hServerItem)
      {
        // get the first window group item.
        if (hChildItem = m_tree.GetChildItem(hServerItem))
        {
          do
          {
            // see if it's the the right group item.
            item.mask = TVIF_PARAM;
            item.hItem = hChildItem; // we want more info about this tree node.
            if (m_tree.GetItem(&item))
            {
              pTII = (TreeItemInfo *)item.lParam;

              if (pTII->m_Type == STII.m_TIIGroup)
              {
                // find the item itself
                if (hSubChildItem = m_tree.GetChildItem(hChildItem))
                {
                  do
                  {
                    item.mask = TVIF_PARAM;
                    item.hItem = hSubChildItem; // we want more info about this tree node.
                    if (m_tree.GetItem(&item))
                    {                  
                      pTII = (TreeItemInfo *)item.lParam;
                      // check for a match
                      if (pTII->m_Type == STII.m_TIIItem && pTII->m_Data == STII.m_ItemData)
                      {
                        return hSubChildItem;
                      }
                    }
                    hSubChildItem = m_tree.GetNextSiblingItem(hSubChildItem);
                  } while (hSubChildItem);
                }
              }
              else
              {
                // check for a match
                if (pTII->m_Type == STII.m_TIIItem && pTII->m_Data == STII.m_ItemData)
                {
                  return hChildItem;
                }
              }
            }
            hChildItem = m_tree.GetNextSiblingItem(hChildItem);
          } while (hChildItem);
        }
      }
    }
  }
  return NULL;
}

void CServerListView::Select(IRCServer *pServer)
{
  HTREEITEM hItem;
  if (pServer->m_IsDirect)
    hItem = FindItem(STI_DCCCHAT,(void *)pServer);
  else
    hItem = FindItem(STI_SERVER,(void *)pServer);

  if (hItem)
  {
    m_tree.SelectItem(hItem);
  }
}

void CServerListView::Select(IRCChannel *pChannel)
{
  HTREEITEM hItem = FindItem(STI_CHANNEL,(void *)pChannel);
  if (hItem)
  {
    m_tree.SelectItem(hItem);
    g_pMainWnd->UpdateGlobalUserList(pChannel);
  }
}

void CServerListView::Select(IRCQuery *pQuery)
{
  HTREEITEM hItem = FindItem(STI_QUERY,(void *)pQuery);
  if (hItem)
  {
    m_tree.SelectItem(hItem);
  }
}

void CServerListView::RemoveItem(int ItemType, void *pItemData)
{
  HTREEITEM hItem;
  HTREEITEM hParentItem;
  TVITEM item;
  TreeItemInfo *pTII = NULL;

  hItem = FindItem(ItemType,pItemData);
  if (hItem)
  {
    // save the parent item for later!
    hParentItem = m_tree.GetParentItem(hItem);

    // delete the item
    item.mask = TVIF_PARAM;
    item.hItem = hItem; // we want more info about this tree node.
    if (m_tree.GetItem(&item))
    {      
      m_TIIList.Remove((TreeItemInfo *)item.lParam);
      delete (TreeItemInfo *)item.lParam;
    }
    m_tree.DeleteItem(hItem);

    if (ItemType != STI_SERVER) 
    {
      // now, if that was the last item deleted, we need to delete
      // the item's group tree item, so query the group tree item.

      item.mask = TVIF_CHILDREN | TVIF_PARAM;
      item.hItem = hParentItem; // we want more info about this tree node.
      if (m_tree.GetItem(&item))
      {
        pTII = (TreeItemInfo *)item.lParam;
        // if the parent item is a group item, and the group has no children, then remove the group icon.
        if (pTII && 
            (pTII->m_Type == TII_CHANNELGROUP ||
             pTII->m_Type == TII_DCCCHATGROUP ||
             pTII->m_Type == TII_QUERYGROUP) && 
            item.cChildren == 0)
        {
          m_TIIList.Remove(pTII);
          delete pTII;
          m_tree.DeleteItem(hParentItem);
        }
      }
    }
  }
#ifdef DEBUG
  else
  {
    // Note: Sometimes we are allowed to fail to find the item, that is
    // this routine is called in various cleanups even though the item
    // may not have been added to the list.  e.g. ~IRCChannel()

    // ATLASSERT(0); // couldn't find the item we're trying to remove!
  }
#endif
}

void CServerListView::Remove(IRCServer *pServer)
{
  if (pServer->m_IsDirect)
    RemoveItem(STI_DCCCHAT,(void *)pServer);
  else
    RemoveItem(STI_SERVER,(void *)pServer);
}

void CServerListView::Remove(IRCQuery *pQuery)
{
  RemoveItem(STI_QUERY,(void *)pQuery);
}

void CServerListView::Remove(IRCChannel *pChannel)
{
  RemoveItem(STI_CHANNEL,(void *)pChannel);
}

void CServerListView::AddItem(int ItemType, void *pItemData)
{
  STIInfo STII(ItemType,pItemData);

  if (ItemType == STI_SERVER)
  {
    // Server's go in the root of the tree...
    TreeItemInfo *pNewTII = new TreeItemInfo(STII.m_TIIItem,STII.m_ItemData,0,STII_NONE);
    m_TIIList.Add(pNewTII);
    HTREEITEM hItem=m_tree.InsertItem(TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM,
      STII.m_ItemName,
      STII.m_iconindices[2],STII.m_iconindices[3], // image & selected image numbers
      0,0, // unused
      (LPARAM)pNewTII, //pServer,
      TVI_ROOT,TVI_SORT);
  }
  else
  {
    TVITEM item;
    HTREEITEM hServerItem;
    HTREEITEM hGroupItem;
    HTREEITEM hItem;
    HTREEITEM hNewLocation;
    item.pszText = NULL;
    item.iImage = 0;
    item.iSelectedImage = 0;
    item.state = 0;
    item.stateMask = 0;
    item.lParam = 0;  
    TreeItemInfo *pTII;

    switch (ItemType)
    {
      case STI_DCCCHAT:
        hServerItem = FindItem(STI_SERVER,(void *)STII.m_pServer->m_pOtherServer);
        break;
      case STI_QUERY:
        hServerItem = FindItem(STI_SERVER,(void *)STII.m_pQuery->m_pServer);
        break;
      case STI_CHANNEL:
        hServerItem = FindItem(STI_SERVER,(void *)STII.m_pChannel->m_pServer);
        break;
    }

    if (hServerItem)
    {
      item.mask = TVIF_CHILDREN;
      item.hItem = hServerItem; // we want more info about this tree node.
      if (m_tree.GetItem(&item))
      {
        // when we find the right place to put the item, we set it here.
        // when searching 
        hNewLocation = NULL;

        if (item.cChildren > 0)
        {
          // find the DCCChats/Channels/Queries child window group tree item
          if (hGroupItem = m_tree.GetChildItem(hServerItem))
          {
            do
            {
              item.mask = TVIF_PARAM;
              item.hItem = hGroupItem; // we want more info about this tree node.
              if (m_tree.GetItem(&item))
              {
                pTII = (TreeItemInfo *)item.lParam;
                if (pTII->m_Type == STII.m_TIIGroup)
                {
                  // found the channels tree view item under the server.
                  hNewLocation = hGroupItem;
                  break;
                }
              }
              hGroupItem = m_tree.GetNextSiblingItem(hGroupItem);
            } while (hGroupItem);
          }
        }

        if (hNewLocation == NULL)
        {
          // TODO: Add a pref for this
          if (!BOOLPREF(PREF_bServerListChannelGroups) && ItemType == STI_CHANNEL)
          {
            // add channels directly under the server node.
            hNewLocation = hServerItem;
          }
          else
          {
            // if there are no other windows of this kind open
            // So we add a group for them to appear under.

            // TODO: Insert groups alphabetcially (or in a proper defined order ?)
            TreeItemInfo *pNewTII = new TreeItemInfo(STII.m_TIIGroup,NULL,STII.m_TreeSortValue,STII_NONE);
            m_TIIList.Add(pNewTII);
            hNewLocation=m_tree.InsertItem(TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM,
              STII.m_GroupName,
              STII.m_iconindices[0],STII.m_iconindices[1], // image & selected image numbers
              0,0, // unused
              (LPARAM)pNewTII,
              hServerItem,hServerItem);
          }
        }

        if (hNewLocation != NULL)
        {
          TreeItemInfo *pNewTII = new TreeItemInfo(STII.m_TIIItem, STII.m_ItemData,0,STII_NONE);
          m_TIIList.Add(pNewTII);
          hItem=m_tree.InsertItem(TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM,
            STII.m_ItemName,
            STII.m_iconindices[2],STII.m_iconindices[3], // image & selected image numbers
            0,0, // unused
            (LPARAM)pNewTII,
            hNewLocation,TVI_SORT);
          m_tree.Expand(hServerItem); // expand the server
          m_tree.Expand(hNewLocation); // expand the group
          //m_tree.SelectItem(hItem);

        }
#ifdef DEBUG
        else 
        {
          ATLASSERT(0); // unable to find the right place to put the icon
        }
#endif
      }
    }
  }
}

void CServerListView::Add(IRCChannel *pChannel)
{
  AddItem(STI_CHANNEL,(void *)pChannel);
}

void CServerListView::Add(IRCQuery *pQuery)
{
  AddItem(STI_QUERY,(void *)pQuery);
}

void CServerListView::Add(IRCServer *pServer)
{
  if (pServer->m_IsDirect)
    AddItem(STI_DCCCHAT,(void *)pServer);
  else
    AddItem(STI_SERVER,(void *)pServer);
}

void CServerListView::UpdateItem(int ItemType, void *pItemData)
{
  HTREEITEM hItem;
  TVITEM item;
  char ItemName[MAX_SERVERLIST_ITEM_LEN+1];

  hItem = FindItem(ItemType,pItemData);
  if (hItem)
  {
    // delete the item
    item.mask = TVIF_TEXT | TVIF_PARAM;
    item.hItem = hItem; // we want more info about this tree node.
    item.pszText = ItemName;
    item.cchTextMax = MAX_SERVERLIST_ITEM_LEN; // TODO: check +1 ? for null terminator...
    if (m_tree.GetItem(&item))
    { 
      TreeItemInfo *pTII = (TreeItemInfo *)item.lParam;
#ifdef DEBUG
      ATLASSERT(pTII->m_Data == pItemData); // Double check we're attempting to update the right item
#endif

      // Set the item's dirty flag.
      //pTII->m_Flags |= STII_DIRTY;
      // and after all that, lol.. instead we just set a flag for the whole list
      // not individial items
      SetDirty();

      STIInfo *pSTI = new STIInfo(ItemType,pItemData);
      if (pSTI)
      {
        if (strncmp(pSTI->m_ItemName,item.pszText,MAX_SERVERLIST_ITEM_LEN) != 0)
        {
          // item name is different, update it!
          strncpy(ItemName,pSTI->m_ItemName,MAX_SERVERLIST_ITEM_LEN);
          ItemName[MAX_SERVERLIST_ITEM_LEN] = 0; // truncate!
          m_tree.SetItemText(hItem,ItemName);
          m_tree.SortChildren(m_tree.GetParentItem(hItem));
        }
        delete (pSTI);
      }
    }
  }
#ifdef DEBUG
  else
  {
    ATLASSERT(0); // couldn't find the item we're trying to update!
  }
#endif
}

void CServerListView::Update(IRCServer *pServer)
{
  if (pServer->m_IsDirect)
    UpdateItem(STI_DCCCHAT,(void *)pServer);
  else
    UpdateItem(STI_SERVER,(void *)pServer);
}

void CServerListView::Update(IRCQuery *pQuery)
{
  UpdateItem(STI_QUERY,(void *)pQuery);
}

void CServerListView::Update(IRCChannel *pChannel)
{
  UpdateItem(STI_CHANNEL,(void *)pChannel);
}


LRESULT CServerListView::OnContextMenu(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
  RECT rc;
  UINT Flags;
  HTREEITEM hItem;
  TreeItemInfo *pTII = NULL;
  IRCChannel *pChannel = NULL;
  IRCServer *pServer = NULL;

	POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

  // mouse click location

	m_tree.GetClientRect(&rc);
	m_tree.ScreenToClient(&pt); 

  hItem = m_tree.HitTest(pt,&Flags);

  if (hItem != NULL)
  {
    TVITEM item;
    item.mask = TVIF_PARAM;
	  item.lParam = 0;  
    item.hItem = hItem; // we want more info about this tree node.
    if (m_tree.GetItem(&item))
    {
      pTII = (TreeItemInfo *)item.lParam;
      m_SelectedTreeItem = pTII;
      switch (pTII->m_Type)
      {
        case TII_IRCCHANNEL:
          // add channel context menu items
          pChannel = (IRCChannel *)pTII->m_Data;
          break;
        case TII_IRCSERVER:
          // add server context menu items
          pServer = (IRCServer *)pTII->m_Data;
          break;
      }
    }
  }

  // build the menu.

  if (pTII)
  {
    // only show context menus for these tree items...
    if (pTII->m_Type == TII_IRCCHANNEL || pTII->m_Type == TII_IRCSERVER ||
        pTII->m_Type == TII_QUERY || pTII->m_Type == TII_DCCCHAT)
    {
      CMenuHandle Menu;
      Menu.CreatePopupMenu();

      switch(pTII->m_Type)
      {
        case TII_IRCSERVER:
          if (pServer)
          {
            if (pServer->GetStatus() == SVR_STATE_DISCONNECTED)
              Menu.AppendMenu(MF_STRING,SLMI_Connect,   _T("&Connect"));

            if (pServer->GetStatus() == SVR_STATE_CONNECTED)
            {
              Menu.AppendMenu(MF_STRING,SLMI_Reconnect, _T("&Reconnect"));
              Menu.AppendMenu(MF_STRING,SLMI_Disconnect,_T("&Disconnect"));
              Menu.AppendMenu(MF_STRING,SLMI_ListChannels,_T("Channel &List"));
            }
          }
          break;
      }

      if (pTII->m_Type == TII_IRCCHANNEL || pTII->m_Type == TII_IRCSERVER ||
          pTII->m_Type == TII_QUERY || pTII->m_Type == TII_DCCCHAT)
      {
        Menu.AppendMenu(MF_STRING,SLMI_Logging,_T("Lo&gging"));
      }

      if (pTII->m_Type == TII_IRCCHANNEL || pTII->m_Type == TII_IRCSERVER)
      {
        Menu.AppendMenu(MF_STRING,SLMI_Monitoring,_T("&Monitoring"));
      }

      switch(pTII->m_Type)
      {
        case TII_IRCCHANNEL:
          if (pChannel)
          {
            Menu.AppendMenu(MF_STRING,SLMI_Cycle,     _T("C&ycle"));
            Menu.AppendMenu(MF_STRING,SLMI_Properties,_T("&Properties"));
          }
          break;
      }

      Menu.AppendMenu(MF_STRING,SLMI_Close,_T("C&lose"));


      ::GetCursorPos(&pt);      
      g_pMainWnd->m_CmdBar.TrackPopupMenu(Menu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y);
      //::TrackPopupMenu(Menu, 0, pt.x, pt.y, 0, m_hWnd, NULL);
    }
  }

  return 0; // WM_CONTEXTMENU - No return value
}

void CServerListView::OnItemClicked(HTREEITEM hItem)
{
  TVITEM item;
  item.mask = TVIF_PARAM;
  item.hItem = hItem;
  item.lParam = 0;
  if (m_tree.GetItem(&item))
  {
    if (item.lParam)
    {
      TreeItemInfo *pTII;

      pTII = (TreeItemInfo *)item.lParam;
      switch (pTII->m_Type)
      {
        case TII_QUERY:
          {
            IRCQuery *pQuery = (IRCQuery *)pTII->m_Data;
            pQuery->ShowQueryWindow(); // calling a different show window, this one creates the window if needed...
          }
          break;
        case TII_IRCCHANNEL:
          {
            IRCChannel *pChannel = (IRCChannel *)pTII->m_Data;
            if (pChannel->m_pChildWnd)
            {
              pChannel->m_pChildWnd->ActivateWindow();
              //::ShowWindow(pChannel->m_pChildWnd->m_hWnd,SW_SHOW);
              //g_pMainWnd->MDIActivate(pChannel->m_pChildWnd->m_hWnd);              
            }
          }
          break;
        case TII_DCCCHAT:
        case TII_IRCSERVER:
          {
            IRCServer *pServer = (IRCServer *)pTII->m_Data;
            if (pServer->m_pChildWnd)
            {
              pServer->m_pChildWnd->ActivateWindow();
              //::ShowWindow(pServer->m_pChildWnd->m_hWnd,SW_SHOW);
              //g_pMainWnd->MDIActivate(pServer->m_pChildWnd->m_hWnd);
            }
          }
          break;
      }
    }
  }
}

LRESULT CServerListView::OnTvnGetInfoTip(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
{
  char buffer[400];
  TVITEM item;
  LPNMTVGETINFOTIPA lpGetInfoTip = (LPNMTVGETINFOTIP)pnmh;      

  if (lpGetInfoTip->hItem)
  {
	  item.mask = TVIF_PARAM | TVIF_TEXT;
	  item.pszText = buffer;
    item.cchTextMax = 399;
  	item.lParam = 0;
    item.hItem = lpGetInfoTip->hItem;
    if (m_tree.GetItem(&item))
    {
      if (item.lParam)
      {
        TreeItemInfo *pTII;

        pTII = (TreeItemInfo *)item.lParam;
        switch (pTII->m_Type)
        {
          case TII_IRCCHANNEL:
            strncpy(lpGetInfoTip->pszText,((IRCChannel *)pTII->m_Data)->GetDescriptionString(),lpGetInfoTip->cchTextMax-1);
            return 0;
            break;
          case TII_IRCSERVER:
            strncpy(lpGetInfoTip->pszText,((IRCServer *)pTII->m_Data)->GetDescriptionString(),lpGetInfoTip->cchTextMax-1);
            return 0;
            break;
        }
      }
      // still here ?  ok, just use the item's text then...
      strncpy(lpGetInfoTip->pszText,item.pszText,lpGetInfoTip->cchTextMax-1);
    }
  }
  return 0;
}

CServerListView::~CServerListView( void )
{
  // cleanup the treeiteminfo's
  while (m_TIIList.GetSize() > 0)
  {
    delete m_TIIList[0];
    m_TIIList.RemoveAt(0);
  }
}

LRESULT CServerListView::OnNmClick(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
{
  TVHITTESTINFO ht = {0};

  DWORD dwpos = GetMessagePos();

  ht.pt.x = GET_X_LPARAM(dwpos);
  ht.pt.y = GET_Y_LPARAM(dwpos);
  // TODO: check ok on multi-monitor system
  // TODO: use ClientToScreen() instead
  ::MapWindowPoints(HWND_DESKTOP, pnmh->hwndFrom, &ht.pt, 1);

  m_tree.HitTest(&ht);
  //TreeView_HitTest(pnmh->hwndFrom, &ht);
      
  if(TVHT_ONITEM  & ht.flags)
  {
    OnItemClicked(ht.hItem);
  }
  else
  {
    bHandled = FALSE;
  }
  return 0;

}

LRESULT CServerListView::OnNmSetFocus(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
{
#ifdef VERBOSE_DEBUG
  sys_Printf(BIC_FUNCTION,"CServerListView::OnNmSetFocus\n");
#endif
  ::SetFocus(g_pMainWnd->MDIGetActive());
  bHandled = TRUE;
  return 1;
}

LRESULT CServerListView::OnNmCustomDraw(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
  LPNMTVCUSTOMDRAW pNMH = (LPNMTVCUSTOMDRAW)pnmh;

  switch(pNMH->nmcd.dwDrawStage)
  {
    case CDDS_PREPAINT:
      return CDRF_NOTIFYITEMDRAW;

    case CDDS_ITEMPREPAINT:
      {
        BOOL FontChanged = FALSE;
        COLORREF NewForeground = -1;
        COLORREF NewBackground = -1;

        TreeItemInfo *pTII = (TreeItemInfo *)pNMH->nmcd.lItemlParam;

        if (!pTII)
          return NULL;

        int DirtyStatus;

        switch(pTII->m_Type)
        {
          case TII_IRCSERVER:
            {
              COLORREF NewServerForeground = -1;

              IRCServer *pServer = (IRCServer *)pTII->m_Data;
              switch (pServer->GetStatus())
              {
                case SVR_STATE_DISCONNECTED:
                  NewForeground = COLORVAL(item_serverlistdisconnected);
                  break;
                case SVR_STATE_CONNECTING:
                  NewForeground = COLORVAL(item_serverlistconnecting);
                  break;
                case SVR_STATE_CONNECTED:
                  if (!pServer->IsLoggedIn())
                    NewForeground = COLORVAL(item_serverlistconnecting);
                  else
                    NewForeground = COLORVAL(item_serverlistconnected);
                  break;
              }              

              DirtyStatus = pServer->GetDirtyStatus();
              if (DirtyStatus & DIRTY_RECENT)
              {
                NewServerForeground = COLORVAL(item_serverlistserverrecenttext);
                NewBackground = COLORVAL(item_serverlistserverrecentbackground);
                SelectObject(pNMH->nmcd.hdc,GetAppFont(PREF_fServerListRecentFont));
                FontChanged=TRUE;
              }
              else
              {
                if (DirtyStatus & DIRTY_TRUE)
                {
                  NewServerForeground = COLORVAL(item_serverlistserverdirtytext);
                  NewBackground = COLORVAL(item_serverlistserverdirtybackground);
                  SelectObject(pNMH->nmcd.hdc,GetAppFont(PREF_fServerListDirtyFont));
                  FontChanged=TRUE;
                }
              }
              if (NewServerForeground != -1)
                NewForeground = NewServerForeground;
            }
            break;
          case TII_DCCCHAT:
            {
              IRCServer *pServer = (IRCServer *)pTII->m_Data;

              DirtyStatus = pServer->GetDirtyStatus();
              if (DirtyStatus & DIRTY_RECENT)
              {
                NewForeground = COLORVAL(item_serverlistdccchatrecenttext);
                NewBackground = COLORVAL(item_serverlistdccchatrecentbackground);
                SelectObject(pNMH->nmcd.hdc,GetAppFont(PREF_fServerListRecentFont));
                FontChanged=TRUE;
              }
              else
              {
                if (DirtyStatus & DIRTY_TRUE)
                {
                  NewForeground = COLORVAL(item_serverlistdccchatdirtytext);
                  NewBackground = COLORVAL(item_serverlistdccchatdirtybackground);
                  SelectObject(pNMH->nmcd.hdc,GetAppFont(PREF_fServerListDirtyFont));
                  FontChanged=TRUE;
                }
              }
            }
            break;

          case TII_IRCCHANNEL:
            {
              IRCChannel *pChannel = (IRCChannel *)pTII->m_Data;
              DirtyStatus = pChannel->GetDirtyStatus();
              if (DirtyStatus & DIRTY_RECENT)
              {
                NewForeground = COLORVAL(item_serverlistchannelrecenttext);
                NewBackground = COLORVAL(item_serverlistchannelrecentbackground);
                SelectObject(pNMH->nmcd.hdc,GetAppFont(PREF_fServerListRecentFont));
                FontChanged=TRUE;
              }
              else
              {
                if (DirtyStatus & DIRTY_TRUE)
                {
                  NewForeground = COLORVAL(item_serverlistchanneldirtytext);
                  NewBackground = COLORVAL(item_serverlistchanneldirtybackground);
                  SelectObject(pNMH->nmcd.hdc,GetAppFont(PREF_fServerListDirtyFont));
                  FontChanged=TRUE;
                }
              }
            }
            break;
          case TII_QUERY:
            {
              IRCQuery *pQuery = (IRCQuery *)pTII->m_Data;
              DirtyStatus = pQuery->GetDirtyStatus();
              if (DirtyStatus & DIRTY_RECENT)
              {
                NewForeground = COLORVAL(item_serverlistqueryrecenttext);
                NewBackground = COLORVAL(item_serverlistqueryrecentbackground);
                SelectObject(pNMH->nmcd.hdc,GetAppFont(PREF_fServerListRecentFont));
                FontChanged=TRUE;
              }
              else
              {
                if (DirtyStatus & DIRTY_TRUE)
                {
                  NewForeground = COLORVAL(item_serverlistquerydirtytext);
                  NewBackground = COLORVAL(item_serverlistquerydirtybackground);
                  SelectObject(pNMH->nmcd.hdc,GetAppFont(PREF_fServerListDirtyFont));
                  FontChanged=TRUE;
                }
              }
            }
            break;
        }

        if (NewForeground != -1)
          pNMH->clrText = NewForeground;
        // use a selected background color, if the item is selected
        if (pNMH->nmcd.uItemState & CDIS_SELECTED)
          NewBackground = COLORVAL(item_serverlistselectedbackground);
        if (NewBackground != -1)
          pNMH->clrTextBk = NewBackground;
        return FontChanged ? CDRF_NEWFONT : NULL;
      }
  }
  // shouldn't get here the return value is just to shut the compiler up.
#ifdef DEBUG
  ATLASSERT(0); 
#endif
  return NULL;
}

void CServerListView::CheckDirtyStatus( void )
{
  //
/*
  // Parse the list of pTII items, if any has a dirty flag then redraw the window.
  TreeItemInfo *pTII;
  int i;
  BOOL Dirty = FALSE;
  
  for (i = 0; i < m_TIIList.GetSize(); i++)
  {
    pTII = m_TIIList[i];
    switch (pTII->m_Type) // we're only interested in some of the items in the treeview.
    {
      case TII_IRCSERVER:
      case TII_IRCCHANNEL:
      case TII_QUERY:
      case TII_DCCCHAT:
        if (pTII->m_Flags & STII_DIRTY)
        {
          Dirty = TRUE; // we need to redraw.
          pTII->m_Flags &= ~STII_DIRTY; // flip the flag.
        }
        break;
    }
  }

  if (Dirty)
    m_tree.RedrawWindow();
*/
  if (m_Dirty)
  {
    m_tree.RedrawWindow();
    m_Dirty = FALSE;
  }
}

void CServerListView::OnEvent(int EventID, void *pData)
{
  switch (EventID)
  {
    case EV_TICK:
      CheckDirtyStatus();
      break;

    case EV_PREFSCHANGED: 
      CServerListView::UpdateSettings();
      break;
  }
}

void CServerListView::UpdateSettings( void )
{
  if (g_pPrefs)
  {
    m_tree.SetBkColor(COLORVAL(item_serverlistbackground));
    m_tree.SetTextColor(COLORVAL(item_serverlistnormaltext));
    m_tree.SetFont(GetAppFont(PREF_fServerListFont));

    if (m_pToolTip)
      m_pToolTip.Activate(BOOLPREF(PREF_bServerListToolTips));
  }
  m_tree.RedrawWindow();
}

LRESULT CServerListView::OnMenuItemSelected(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
  DoAction(wID);
  return 0;
}

void CServerListView::DoAction( int Action )
{
  if (!IsValidTreeItem(m_SelectedTreeItem))
    return;

  switch (m_SelectedTreeItem->m_Type)
  {
    case TII_IRCSERVER:
      {
        IRCServer *pServer = (IRCServer *)m_SelectedTreeItem->m_Data;
        switch( Action )
        {
          case SLMI_Connect:
            pServer->Connect();
            break;
          case SLMI_Disconnect:
            pServer->Disconnect(FALSE);
            break;
          case SLMI_Reconnect:
            pServer->Disconnect(FALSE);
            pServer->Reconnect();
            break;
          case SLMI_Close:
            if (::IsWindow(pServer->m_pChildWnd->m_hWnd))
            CHILDFRAMEPTR(pServer->m_pChildWnd)->PostMessage(WM_CLOSE);
            break;
          case SLMI_Logging:
            g_pMainWnd->ToggleLogging(CHILDFRAMEPTR(pServer->m_pChildWnd));
            break;
          case SLMI_Monitoring:
            g_pMainWnd->ToggleMonitoring(CHILDFRAMEPTR(pServer->m_pChildWnd));
          case SLMI_ListChannels:
            pServer->ShowChannelListDialog();           
            break;
        }
      }
      break;
    case TII_IRCCHANNEL:
      {
        IRCChannel *pChannel = (IRCChannel *)m_SelectedTreeItem->m_Data;
        switch( Action )
        {
          case SLMI_Close:
            if (::IsWindow(pChannel->m_pChildWnd->m_hWnd))
            CHILDFRAMEPTR(pChannel->m_pChildWnd)->PostMessage(WM_CLOSE);
            break;
          case SLMI_Logging:
            g_pMainWnd->ToggleLogging(CHILDFRAMEPTR(pChannel->m_pChildWnd));
            break;
          case SLMI_Monitoring:
            g_pMainWnd->ToggleMonitoring(CHILDFRAMEPTR(pChannel->m_pChildWnd));
            break;
          case SLMI_Properties:
            pChannel->DoPropertiesDlg();
            break;
          case SLMI_Cycle:
            pChannel->Cycle();
            break;
        }
      }
      break;
    case TII_DCCCHAT:
      {
        IRCServer *pServer = (IRCServer *)m_SelectedTreeItem->m_Data;
        switch( Action )
        {
          case SLMI_Close:
            if (::IsWindow(pServer->m_pChildWnd->m_hWnd))
            CHILDFRAMEPTR(pServer->m_pChildWnd)->PostMessage(WM_CLOSE);
            break;
          case SLMI_Logging:
            g_pMainWnd->ToggleLogging(CHILDFRAMEPTR(pServer->m_pChildWnd));
            break;
        }
      }
      break;

    case TII_QUERY:
      {
        IRCQuery *pQuery = (IRCQuery *)m_SelectedTreeItem->m_Data;
        switch( Action )
        {
          case SLMI_Close:
            if (pQuery->m_pChildWnd)
            {
              if (::IsWindow(pQuery->m_pChildWnd->m_hWnd))
              CHILDFRAMEPTR(pQuery->m_pChildWnd)->PostMessage(WM_CLOSE);
            }
            else
              delete pQuery;

            break;
          case SLMI_Logging:
            g_pMainWnd->ToggleLogging(CHILDFRAMEPTR(pQuery->m_pChildWnd));
            break;
        }
      }
      break;
  }
  m_SelectedTreeItem = NULL;
}
