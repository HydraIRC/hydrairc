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

// UserList.cpp : implementation of the CUserListView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

// Forward declares
class IRCUser;
class IRCUserGroupList;

// order relates to USER_TYPE_* defines and NOT UG_ID_* defines.
char *g_UserGroupNames[6] =
{
  "Normal",
  "Voice",
  "Half-Ops",
  "Ops",
  "Admins",
  "Owner"
};

/*
void CUserListView::OnDocked(HDOCKBAR hBar,bool bHorizontal)
{
	DWORD dwStyle = GetWindowLong(GWL_STYLE)&(~WS_SIZEBOX);
	SetWindowLong( GWL_STYLE, dwStyle);

	baseClass::OnDocked(hBar,bHorizontal);
}

void CUserListView::OnUndocked(HDOCKBAR hBar)
{
	DWORD dwStyle = GetWindowLong(GWL_STYLE) | WS_SIZEBOX;
	SetWindowLong( GWL_STYLE , dwStyle);

	baseClass::OnUndocked(hBar);
}
*/

LRESULT CUserListView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(m_dwIcon), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	HWND tree = m_tree.Create(m_hWnd,NULL,NULL,
          TVS_HASBUTTONS |
					TVS_SHOWSELALWAYS | 
					//TVS_LINESATROOT | 
          TVS_HASLINES |
          //TVS_FULLROWSELECT |
					TVS_DISABLEDRAGDROP | TVS_INFOTIP |
					WS_CHILD | WS_VISIBLE);
	m_tree.SetImageList(g_UserListImages, TVSIL_NORMAL);
  m_tree.SetIndent(5);

  // TODO: add the groups of users that we were from out list of groups.

  if (m_pChannel) // skip addition of users if our list has not been pointed at a
                  // channel
  {
    for (int i = 0 ; i < m_pChannel->m_Users.m_UserList.GetSize() ; i ++)
    {
      Add(m_pChannel->m_Users.m_UserList[i]);
    }
  }

  m_tree.SelectItem(m_tree.GetRootItem());

  /*
  // FIXME: tooltips show immediately, not after a short delay
  int n;
  CToolTipCtrl pToolTip = m_tree.GetToolTips();
  if (pToolTip)
  {
    pToolTip.Activate(TRUE);
    
    n = pToolTip.GetDelayTime(TTDT_INITIAL);
    n = pToolTip.GetDelayTime(TTDT_AUTOPOP);
    n = pToolTip.GetDelayTime(TTDT_RESHOW);
  }
  */

  /*
  If an application processes this message, it should return zero to
  continue creation of the window. If the application returns –1,
  the window is destroyed and the CreateWindowEx or CreateWindow
  function returns a NULL handle.
  */
	return 0;

}

LRESULT CUserListView::OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
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

// Find a treeview group item.
// for valid GroupID's, see USER_TYPE_* defines in Server.h
HTREEITEM CUserListView::Find(int GroupID)
{
  HTREEITEM hItem;
  TVITEM item;
  TreeItemInfo *ptii;

  if (IsWindow())
  {
    hItem = m_tree.GetRootItem();
    if (hItem != NULL)
    {
      do
      {
    	  item.mask = TVIF_PARAM;
    	  item.lParam = 0;
    	  item.hItem = hItem; // we want more info about this tree node.
        if (m_tree.GetItem(&item) && item.lParam != NULL)
        {
          ptii = (TreeItemInfo*)item.lParam;

          if (ptii->m_Type == TII_USERGROUP && ptii->m_Num == GroupID)
            return hItem;
          
        }
        hItem = m_tree.GetNextSiblingItem(hItem);
      } while (hItem);
    }
  }
  return NULL;
}

// Find a treeview user item.
// use Find(Nick,GroupType) if possible as it will be quicker on longer lists.
//HTREEITEM CUserListView::Find(char *Nick)
HTREEITEM CUserListView::Find(IRCUser *pUser)
{
  HTREEITEM hItem;
  HTREEITEM hChildItem;
  TVITEM item;
  TreeItemInfo *ptii;

  if (IsWindow())
  {
    hItem = m_tree.GetRootItem();
    do
    {
      item.mask = TVIF_CHILDREN;
      item.cChildren = 0;
      item.hItem = hItem; // we want more info about this tree node.
      if (m_tree.GetItem(&item))
      {
        if (item.cChildren > 0)
        {
          hChildItem = m_tree.GetChildItem(hItem);
          if (hChildItem)
          {
            // then search the group for a matching nick
            do
            {
              item.mask = TVIF_PARAM;
              item.lParam = 0;
              item.hItem = hChildItem; // we want more info about this tree node.
              if (m_tree.GetItem(&item) && item.lParam != NULL)
              {
                ptii = (TreeItemInfo*)item.lParam;

                if (ptii->m_Type == TII_USER && (IRCUser *)ptii->m_Data == pUser) // just compare the pointers
                  return hChildItem;
              }
              hChildItem = m_tree.GetNextSiblingItem(hChildItem);
            } while (hChildItem);

          }
        }
        
      }
      hItem = m_tree.GetNextSiblingItem(hItem);
    } while (hItem);
  }
  return NULL;
}

// find a treeview item for a given nick/usertype
// for valid UserType's, see USER_TYPE_* defines in Server.h
// HTREEITEM CUserListView::Find(char *Nick, int UserType)
HTREEITEM CUserListView::Find(IRCUser *pUser, int UserType)
{
  HTREEITEM hItem;
  TVITEM item;
  TreeItemInfo *ptii;

  // find the group first.
  hItem = Find(UserType);
  if (hItem != NULL)
  {
    hItem = m_tree.GetChildItem(hItem);
    // then search the group for a matching nick
    while (hItem)
    {
      item.mask = TVIF_PARAM;
      item.lParam = 0;
      item.hItem = hItem; // we want more info about this tree node.
      if (m_tree.GetItem(&item))
      {
        ptii = (TreeItemInfo*)item.lParam;

//        if (ptii->m_Type == TII_USER && stricmp( ((IRCUser *)ptii->m_Data)->m_Nick, Nick) == 0)
        if (ptii->m_Type == TII_USER && (IRCUser *)ptii->m_Data == pUser)
          return hItem;
        
      }
      hItem = m_tree.GetNextSiblingItem(hItem);
    }
  }
  return NULL;
}

void CUserListView::Select(IRCUser *pUser)
{
  if (!pUser)
    return;

  HTREEITEM hItem;

  hItem = Find(pUser,pUser->m_UserType);
  if (!hItem)
    hItem = Find(pUser);

  if (hItem)
  {
    m_tree.Select(hItem, TVGN_CARET);
  }
}

// you still need to delete pUser...
void CUserListView::Remove(IRCUser *pUser)
{
  IRCUser *pFoundUser;

  TVITEM item;
  TreeItemInfo *ptii;
  HTREEITEM hItem, hParent;
    
/*
  hItem = Find(pUser->m_Nick,pUser->m_UserType);
  if (!hItem)
    hItem = Find(pUser->m_Nick);
*/
  hItem = Find(pUser,pUser->m_UserType);
  if (!hItem)
    hItem = Find(pUser);

  if (hItem)
  {
    item.mask = TVIF_PARAM;
    item.lParam = 0;
    item.hItem = hItem; // we want more info about this tree node.
    if (m_tree.GetItem(&item))
    {
      ptii = (TreeItemInfo*)item.lParam;
      ATLASSERT(ptii->m_Type == TII_USER);
        
      pFoundUser = (IRCUser *)ptii->m_Data;
      hParent = m_tree.GetParentItem(hItem);

      IRCUserGroup *pGroup;
        
      if (m_ShowBuddyGroups && pFoundUser->m_CustomGroupID != -1)
        pGroup = m_UserGroups.FindByGroupID(UG_ID_CUSTOM + pFoundUser->m_CustomGroupID);
      else
        pGroup = m_UserGroups.FindByGroupID(pFoundUser->m_UserType);
        
      ATLASSERT(pGroup); // it's an error if we can't find the group, it's in there somewhere.

      // delete stuff..
      m_tree.DeleteItem(hItem);
      m_TIIList.Remove(ptii);
      delete ptii;

      pGroup->RemoveUser(pFoundUser);

      item.hItem = hParent; // we want more info about this tree node.
      item.mask = TVIF_PARAM | TVIF_CHILDREN;
      if (m_tree.GetItem(&item))
      {        
        if (item.cChildren == 0)
        {
          ptii = (TreeItemInfo*)item.lParam;
          m_tree.DeleteItem(hParent);
          m_TIIList.Remove(ptii);
          delete ptii;

          m_UserGroups.Remove(pGroup);
          delete pGroup;
        }
        else
        {
          // pGroup = (IRCUserGroup *)ptii->m_Data; // it's already set...

          if (BOOLPREF(PREF_bShowUserListGroupCounts))
          {
            int newnamelen = strlen(pGroup->m_GroupName) + 10;
            char *newname = (char *)malloc(newnamelen);
            _snprintf(newname,newnamelen-1,"%s (%d)",pGroup->m_GroupName,pGroup->m_UserList.GetSize());
            m_tree.SetItemText(hParent,newname);
            free(newname);
          }
        }
      }
      
    }
  }
}

// hmm, make it a member? probably...
static int CALLBACK UserGroupSorter(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
  // lParamSort contains a pointer to the tree window.
  // CTreeViewCtrl *pTree = (CTreeViewCtrl*) lParamSort;
  TreeItemInfo *pTII1 = (TreeItemInfo *)lParam1;
  TreeItemInfo *pTII2 = (TreeItemInfo *)lParam2;

  if (pTII1->m_Type == TII_USERGROUP && pTII2->m_Type == TII_USERGROUP)
  {
    if (pTII1->m_Num > pTII2->m_Num)
      return -1;
    else if (pTII1->m_Num < pTII2->m_Num)
      return 1;
    else
      return 0;    
  }
  return 0;
}

void CUserListView::Update(IRCUser *pUser)
{
  Remove(pUser);
  Add(pUser);
}

// Add an item to the list, creating groups for the item as required.
// returns NULL or a valid HTREEITEM.
HTREEITEM CUserListView::Add(IRCUser *pUser)
{
  HTREEITEM hLastFirstVisibleItem; //heh, what a great variable name

  // remember what we're looking at so that we can move back to it after the add.
  hLastFirstVisibleItem = m_tree.GetFirstVisibleItem();

  // add an item
  
  HTREEITEM hItem;
  TreeItemInfo *pTII;
  IRCUserGroup *pGroup;
  
  //sys_Printf(BIC_NOTICE, "Adding user %s, type: %d", pUser->m_Nick, pUser->m_UserType);

  // if a custom group is specified for the user, add them to it.
  if (m_ShowBuddyGroups && pUser->m_CustomGroupID != -1)
  {
    // Find the group's HTREEITEM (using our custom group...)
    hItem = Find(UG_ID_CUSTOM + pUser->m_CustomGroupID);
  }
  else
  {
    // Find the group's HTREEITEM
    hItem = Find(pUser->m_UserType);
  }
  if (hItem == NULL)
  {
    // User group not created yet, so create it.
    // we make a new user group, that this class manages
    // based on the information in pUser

    int GroupID;
    //int GroupFlags = IRCUG_DEFAULTFLAGS;
    char *GroupName;
    IRCUserGroup *pSearchGroup;

    // Get the group name.
    if (m_ShowBuddyGroups && pUser->m_CustomGroupID != -1)
    {
      pSearchGroup = g_pPrefs->m_CustomUserGroups.FindByGroupID(pUser->m_CustomGroupID);
      if (pSearchGroup)
      {
        GroupName = pSearchGroup->m_GroupName;
        //GroupFlags = pSearchGroup->m_Flags;
      }
      else
      {
        ATLASSERT(pSearchGroup);
        GroupName = "unknown group";
      }
    }
    else
    {
      switch (pUser->m_UserType)
      {
        case USER_TYPE_NORMAL:
        case USER_TYPE_VOICE:
        case USER_TYPE_HALFOP:
        case USER_TYPE_OP:
        case USER_TYPE_ADMIN:
        case USER_TYPE_OWNER:
          GroupName = g_UserGroupNames[pUser->m_UserType];
          break;
      }
    }

    // create a custom group id for ourselves.
    // (USER_TYPE_* and UG_ID_* only match numerically by conincidence)
    if (m_ShowBuddyGroups && pUser->m_CustomGroupID != -1)
    {
      GroupID = UG_ID_CUSTOM + pUser->m_CustomGroupID;
    }
    else
    {
      switch (pUser->m_UserType)
      {
        case USER_TYPE_NORMAL:
          GroupID = UG_ID_NORMAL;
          break;
        case USER_TYPE_VOICE:
          GroupID = UG_ID_VOICE;
          break;
        case USER_TYPE_HALFOP:
          GroupID = UG_ID_HALFOP;
          break;
        case USER_TYPE_OP:
          GroupID = UG_ID_OP;
          break;
        case USER_TYPE_ADMIN:
          GroupID = UG_ID_ADMIN;
          break;
        case USER_TYPE_OWNER:
          GroupID = UG_ID_OWNER;
          break;
      }
    }

    // Create a new user group to store a refernce to the pUser in.    
    pGroup = new IRCUserGroup(GroupName, GroupID);
    //pGroup->m_Flags = GroupFlags;
    // Add our newly created group to the class's grouplist
    m_UserGroups.Add(pGroup);

    pTII = new TreeItemInfo(TII_USERGROUP, (void *)pGroup, GroupID);
    m_TIIList.Add(pTII);
    hItem = m_tree.InsertItem(TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM,
                                  GroupName,
                                  0,0, // image & selected image numbers
                                  0,0, // unused
                                  (LPARAM)pTII,
                                  TVI_ROOT,TVI_LAST);
    // Sort the group list.
    TVSORTCB tvs;
    tvs.hParent = TVI_ROOT;
    tvs.lpfnCompare = UserGroupSorter;
    tvs.lParam = (LPARAM)&m_tree;
    m_tree.SortChildrenCB(&tvs);
  }

  if (hItem) // got somewhere to put out new node ?
  {
    TVITEM item;    
    item.mask = TVIF_PARAM | TVIF_STATE | TVIF_CHILDREN;
    item.hItem = hItem;
    item.lParam = 0;
    if (m_tree.GetItem(&item) && item.lParam != NULL)
    {
      pTII = (TreeItemInfo *)item.lParam;

      // Add the user to out own internal list of users.
      pGroup = (IRCUserGroup *)pTII->m_Data;
      pGroup->AddUser(pUser);

      // Add the user to the tree under the group icon
      pTII = new TreeItemInfo(TII_USER,(void *)pUser);
      m_TIIList.Add(pTII);
      
      long iIcon;
      
      switch (pUser->m_UserType)
      {
        case USER_TYPE_NORMAL:
          iIcon = 2;
          break;
        case USER_TYPE_VOICE:
          iIcon = 3;
          break;
        case USER_TYPE_HALFOP:
          iIcon = 4;
          break;
        case USER_TYPE_OP:
          iIcon = 5;
          break;
        case USER_TYPE_ADMIN:
          iIcon = 5;
          break;
        case USER_TYPE_OWNER:
          iIcon = 5;
          break;
      }
      
      /*
      HTREEITEM hNewItem = m_tree.InsertItem(TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM,
                                    pUser->m_Nick,
                                    2+pUser->m_UserType,2+pUser->m_UserType, // image & selected image numbers
                                    0,0, // unused
                                    (LPARAM)pTII,
                                    hItem,TVI_SORT); // insert alphabetically
      */
      
      HTREEITEM hNewItem = m_tree.InsertItem(TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM,
                                    pUser->m_Nick,
                                    iIcon, iIcon, // image & selected image numbers
                                    0,0, // unused
                                    (LPARAM)pTII,
                                    hItem,TVI_SORT); // insert alphabetically

      // todo: delay the processing of group counts until after a channel
      // has been joined to speed up joining large channels

      if (BOOLPREF(PREF_bShowUserListGroupCounts))
      {
        int newnamelen = strlen(pGroup->m_GroupName) + 10;
        char *newname = (char *)malloc(newnamelen);
        _snprintf(newname,newnamelen-1,"%s (%d)",pGroup->m_GroupName,pGroup->m_UserList.GetSize());
        m_tree.SetItemText(hItem,newname);
        free(newname);
      }

      // Expand the parent, if we should..

      // only expand if this is the first user we're adding
      if (item.cChildren == 0)
      {
        // only expand if not already expanded
        // REMOVE: this next "if" is not really needed.
        if (!(item.state & TVIS_EXPANDED))
        {
          // if we're supposed to expand, then do so.
          //if (pGroup->m_Flags & IRCUG_EXPAND || pGroup->m_Flags & IRCUG_EXPANDED)
          //{
            m_tree.Expand(hItem);
          //}
        }
      }
/*
      // attempt at drawing a box round the tree item.. (failed..)
      RECT r;
      m_tree.GetItemRect(hItem,&r,TRUE);
      CPaintDC dc(m_tree.m_hWnd);
	    //RECT rect;
      //GetClientRect(&rect);
      //dc.FillRect(&r,COLOR_APPWORKSPACE);
      //dc.FillRect(&r, COLOR_3DFACE);
      dc.DrawEdge(&r, EDGE_RAISED, BF_LEFT | BF_RIGHT);
      //::InvalidateRect(m_tree.m_hWnd,&r,FALSE);
*/

      // go back to where we were before we added this user
      if (m_tree.GetFirstVisibleItem() != hLastFirstVisibleItem)
      {
        m_tree.EnsureVisible(hLastFirstVisibleItem);

        // *sigh* EnsureVisible scrolls the user left to the right a bit,
        // even if it doesn't actually need to (bleh)
        // so we scroll the window back again..
        m_tree.SendMessage(WM_HSCROLL,SB_LEFT,(LPARAM)m_tree.m_hWnd); // todo: check lparam value
        // Todo: if the user had scrolled to the right on purpose
        // we would be resetting the scroll back to the left.
        // so we need to get the Horizontal scroll position before
        // calling EnsureVisible() and then set it back again after.
      }
      return hNewItem;
    }
  }
  return NULL;
}


LRESULT CUserListView::OnTVNSelectChanged(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
{
  TVITEM item;
  LPNMTREEVIEW selectioninfo = (LPNMTREEVIEW)pnmh;

  item.mask = TVIF_PARAM | TVIF_CHILDREN;
  item.hItem = selectioninfo->itemNew.hItem;
  if (m_tree.GetItem(&item))
  {
    // TODO: stuff
  }
  return 0;
}

LRESULT CUserListView::OnTVNItemExpanded(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
{
  TVITEM item;
  TreeItemInfo *pTII;
  LPNMTREEVIEW selectioninfo = (LPNMTREEVIEW)pnmh;
  //char buffer[20]; //remove

  item.mask = TVIF_PARAM | TVIF_CHILDREN;
  item.hItem = selectioninfo->itemNew.hItem;
	
  if (m_tree.GetItem(&item) && item.lParam != NULL)
  {
    pTII = (TreeItemInfo *)item.lParam;
    IRCUserGroup *pGroup = (IRCUserGroup *)pTII->m_Data;

    if (item.cChildren > 0)
    {
      if (selectioninfo->action == TVE_EXPAND)
      {
        m_tree.SetItemImage(selectioninfo->itemNew.hItem,1,1);
        //pGroup->m_Flags |= IRCUG_EXPANDED;
      }
      if (selectioninfo->action == TVE_COLLAPSE)
      {
        m_tree.SetItemImage(selectioninfo->itemNew.hItem,0,0);
        //pGroup->m_Flags &= ~IRCUG_EXPANDED;
      }
    }
  }

  return 0;
}


LRESULT CUserListView::OnTVNGetInfoTip(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
{
  char buffer[400];
  TVITEM item;
  buffer[0] = 0;  
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
        // TODO: stuff
      }
      else
      {
        strncpy(lpGetInfoTip->pszText,item.pszText,item.cchTextMax-1); // -1 really needed ?
      }
    }
  }
  return 0;
}

CUserListView::~CUserListView( void )
{
#ifdef VERBOSE_DEBUG
  sys_Printf(BIC_FUNCTION,"CUserListView::~CUserListView\n");
#endif
  RemoveAll();
}

void CUserListView::RemoveAll( void )
{
#ifdef VERBOSE_DEBUG
  sys_Printf(BIC_FUNCTION,"CUserListView::RemoveAll\n");
#endif
  // free the lists of users, but don't free the users themselves
  // (so remove the users from the lists, then delete the groups and lists of groups)
  m_UserGroups.RemoveAllUsers();
  m_UserGroups.DeleteAll();
  while (m_TIIList.GetSize() > 0)
  {
    delete m_TIIList[0];
    m_TIIList.RemoveAt(0);
  }
  if (::IsWindow(m_tree.m_hWnd))
    m_tree.DeleteAllItems();
}

void CUserListView::Refresh( void )
{
  // export all users from all groups to a single group
  IRCUserGroup *pAllUsers = m_UserGroups.ExportAll(NULL);
  if (!pAllUsers)
    return;

  // remove all the users from all the groups, and the groups
  // but don't don't delete the user instances themselves.
  RemoveAll();

  // then add the users back to the list, using now-current settings/prefs
  while (pAllUsers->m_UserList.GetSize() > 0)
  {
    Add(pAllUsers->m_UserList[0]);
    pAllUsers->RemoveUser(pAllUsers->m_UserList[0]);
  }

  delete pAllUsers;
}

void CUserListView::SetShowBuddyGroups(BOOL ShowBuddyGroups)
{
  BOOL DoRefresh = m_ShowBuddyGroups != ShowBuddyGroups;
  m_ShowBuddyGroups = ShowBuddyGroups;
  if (DoRefresh)
    Refresh();
}

void CUserListView::UpdateSettings( void )
{
  if (g_pPrefs)
  {
    // if this instance is a docking window, get the colors from the prefs.
    if (this == &g_pMainWnd->m_UserListView)
    {
      SetColors(g_pPrefs->m_ColorPrefs);
    }

    m_tree.SetFont(GetAppFont(PREF_fUserListFont));
    m_tree.SetBkColor(m_BackColor);
    m_tree.SetTextColor(m_TextColor);
  }
  m_tree.RedrawWindow();
}

LRESULT CUserListView::OnNMDblclk(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& bHandled)
{
  if (!(m_pChannel && m_pChannel->m_pChildWnd && m_pChannel->m_pChildWnd->IsWindow()))
    return 0;  
      
  RECT rc;
  UINT Flags;
  HTREEITEM hItem;
  TreeItemInfo *pTII;

  DWORD dwpos = GetMessagePos();
	POINT pt = 
  { 
    GET_X_LPARAM(dwpos) ,
    GET_Y_LPARAM(dwpos)
  };

  // mouse click location

	m_tree.GetClientRect(&rc);
	m_tree.ScreenToClient(&pt); 

  hItem = m_tree.HitTest(pt,&Flags);

  if (hItem != NULL)
  {
    m_tree.SelectItem(hItem);
    TVITEM item;
    item.mask = TVIF_PARAM;
	  item.lParam = 0;  
    item.hItem = hItem; // we want more info about this tree node.
    if (m_tree.GetItem(&item))
    {
      pTII = (TreeItemInfo *)item.lParam;
      CHILDFRAMEPTR(m_pChannel->m_pChildWnd)->DoUserListDoubleClick(pTII);
    }
  }

  bHandled = FALSE;

  return 0;
}

LRESULT CUserListView::OnContextMenu(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
  if (!(m_pChannel && m_pChannel->m_pChildWnd && m_pChannel->m_pChildWnd->IsWindow()))
    return 0;  

  RECT rc;
  UINT Flags;
  HTREEITEM hItem;
  TreeItemInfo *pTII;

	POINT pt = 
  { 
    GET_X_LPARAM(lParam) , 
    GET_Y_LPARAM(lParam)
  };

  // mouse click location

	m_tree.GetClientRect(&rc);
	m_tree.ScreenToClient(&pt); 

  hItem = m_tree.HitTest(pt,&Flags);

  if ((hItem != NULL) && ((Flags & TVHT_ONITEM) > 0))
  {
    m_tree.SelectItem(hItem);
    TVITEM item;
    item.mask = TVIF_PARAM;
	  item.lParam = 0;  
    item.hItem = hItem; // we want more info about this tree node.
    if (m_tree.GetItem(&item))
    {
      pTII = (TreeItemInfo *)item.lParam;
      CHILDFRAMEPTR(m_pChannel->m_pChildWnd)->DoUserListContextMenu(pTII,this);
    }
  }
  else
  {
    CHILDFRAMEPTR(m_pChannel->m_pChildWnd)->DoUserListContextMenu(NULL,this);
  }

  bHandled = FALSE;
  return 0;
}

void CUserListView::OnEvent(int EventID, void *pData)
{
  switch(EventID)
  {
    case EV_PREFSCHANGED:
      UpdateSettings();
      break;
  }
}

LRESULT CUserListView::OnNmCustomDraw(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
  LPNMTVCUSTOMDRAW pNMH = (LPNMTVCUSTOMDRAW)pnmh;

  switch(pNMH->nmcd.dwDrawStage)
  {
    case CDDS_PREPAINT:
      return CDRF_NOTIFYITEMDRAW;

    case CDDS_ITEMPREPAINT:
      {
        COLORREF NewBackground = -1;

        //sys_Printf(BIC_INFO,"%d\n",pNMH->nmcd.uItemState);

        // use a selected background color, if the item is selected
        if (pNMH->nmcd.uItemState & CDIS_SELECTED)
        {
          NewBackground = m_SelectedBackColor;
        }
        if (NewBackground != -1)
          pNMH->clrTextBk = NewBackground;
        return NULL;// CDRF_NEWFONT
      }
  }
  // shouldn't get here the return value is just to shut the compiler up.
#ifdef DEBUG
  ATLASSERT(0); 
#endif
  return NULL;
}

void CUserListView::SetColors( COLORREF *pColors )
{
  if (this == &g_pMainWnd->m_UserListView)
  {
    // extract just the colors we need from the array of colors.
    m_TextColor = pColors[item_globaluserlistnormaltext - PREF_COLOR_FIRST];
    m_BackColor = pColors[item_globaluserlistbackground - PREF_COLOR_FIRST];
    m_SelectedBackColor = pColors[item_globaluserlistselectedbackground - PREF_COLOR_FIRST];
  }
  else
  {
    // extract just the colors we need from the array of colors.
    m_TextColor = pColors[item_userlistnormaltext - PREF_COLOR_FIRST];
    m_BackColor = pColors[item_userlistbackground - PREF_COLOR_FIRST];
    m_SelectedBackColor = pColors[item_userlistselectedbackground - PREF_COLOR_FIRST];
  }

}

LRESULT CUserListView::OnUserListContext(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
  // handle window related items here, failing that, pass back to the window that owns this list of users.
  switch (wID)
  {
    case ID_USERLIST_SHOWBUDDYGROUPS:
      m_ShowBuddyGroups = !m_ShowBuddyGroups;
      Refresh();
      break;
    default:
      if (m_pChannel && m_pChannel->m_pChildWnd)
      {
        return CHILDFRAMEPTR(m_pChannel->m_pChildWnd)->OnUserListContext(wNotifyCode, wID, hWndCtl, bHandled);
      }
  }
  return 0; // doesn't matter
}