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
#include "favoritesdlg.h"
#include ".\favoritesdlg.h"

// recursively add items from a favorites xml tree node to a treeview control at
// the specified location (hItem)
void PopulateFavoritesTree(CTreeViewCtrl *pTreeCtrl, CSimpleArray<TreeItemInfo *> &TIIList, HTREEITEM hItem,xmlNodePtr pNode, xmlNodePtr pSelectNode = NULL, HTREEITEM *hSelectItem = NULL)
{
  if (!pTreeCtrl)
    return;

  while (pNode)
  {
    if (pNode->type == XML_ELEMENT_NODE)
    {
      char *name = (char *)xmlGetProp(pNode,(xmlChar *)"name");

      if (stricmp((char*)pNode->name,"group") == 0)
      {
        TreeItemInfo *pNewTII = new TreeItemInfo(TII_GROUP,pNode);

        if (pNewTII)
        {
          HTREEITEM hNewItem=pTreeCtrl->InsertItem(TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM,
            (char *)name,
            ICON_CHANNELS,ICON_CHANNELS, // image & selected image numbers
            0,0, // unused
            (LPARAM)pNewTII,
            hItem,TVI_LAST);

          if (hNewItem)
          {
            if (pSelectNode && hSelectItem && pSelectNode == pNode)
              *hSelectItem = hNewItem;
            TIIList.Add(pNewTII);
            PopulateFavoritesTree(pTreeCtrl,TIIList,hNewItem,pNode->children,pSelectNode,hSelectItem);
          }
          else
            delete pNewTII;
        }
      }
      else if (stricmp((char*)pNode->name,"favorite") == 0)
      {
        char *type = (char *)xmlGetProp(pNode,(xmlChar *)"type");
        if (name && type)
        {
          int iconindex = 0;
          int itemtype = -1;
          switch (toupper(*type))
          {
            case 'C':
              iconindex = ICON_CHANNEL;
              itemtype = TII_IRCCHANNEL;
              break;
            case 'S':
              iconindex = ICON_SERVER;
              itemtype = TII_SERVER;
              break;
            case 'Q':
              iconindex = ICON_QUERY;
              itemtype = TII_QUERY;
              break;
          }

          TreeItemInfo *pNewTII = new TreeItemInfo(itemtype,pNode);

          if (pNewTII)
          {
            HTREEITEM hNewItem=pTreeCtrl->InsertItem(TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM,
              name,
              iconindex,iconindex, // image & selected image numbers
              0,0, // unused
              (LPARAM)pNewTII, 
              hItem,TVI_LAST);

            if (hNewItem)
            {
              if (pSelectNode && hSelectItem && pSelectNode == pNode)
                *hSelectItem = hNewItem;
              TIIList.Add(pNewTII);
            }
            else
              delete pNewTII;
          }

        }

        if (type) free(type);
      }
      if (name) free(name);
    }

    pNode = pNode->next;
  };

}

void ClearTree(CTreeViewCtrl *pTreeCtrl, CSimpleArray<TreeItemInfo *> &TIIList)
{
  if (pTreeCtrl)
    pTreeCtrl->DeleteAllItems();

  while (TIIList.GetSize() > 0)
  {
    delete TIIList[0];
    TIIList.RemoveAt(0);
  }
}

LRESULT CFavoritesDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  CenterWindow(GetParent());
  DlgResize_Init();

  m_TreeCtrl.Attach(GetDlgItem(IDC_FAVORITES_TREE));
  m_NoteCtrl.Attach(GetDlgItem(IDC_FAVORITES_NOTE));
	m_TreeCtrl.SetImageList(g_pMainWnd->m_images_serverlist, TVSIL_NORMAL);

  m_hRootItem=m_TreeCtrl.InsertItem(TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM,
    (char *)"Favorites",
    ICON_CHANNELS,ICON_CHANNELS, // image & selected image numbers
    0,0, // unused
    (LPARAM)NULL, // no TII for root item
    NULL,TVI_LAST);

  PopulateFavoritesTree(&m_TreeCtrl,m_TIIList,m_hRootItem,g_pPrefs->m_Favorites);

  m_TreeCtrl.Expand(m_hRootItem);

	return TRUE;
}

LRESULT CFavoritesDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  g_pPrefs->SavePrefs();

	EndDialog(wID);
	return 0;
}

LRESULT CFavoritesDlg::OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND hWndCtl, BOOL& /*bHandled*/)
{
  // Knowledge Base Q130692 workaround
  if (hWndCtl != GetDlgItem(IDOK))
    return 1;

  g_pPrefs->SavePrefs();

  EndDialog(1);    
	return 0;
}

LRESULT CFavoritesDlg::OnTvnEndlabeleditFavoritesTree(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
  LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);

  TVITEM Item;

	Item.mask = TVIF_PARAM;
  Item.hItem = pTVDispInfo->item.hItem;

  if (m_TreeCtrl.GetItem(&Item) && pTVDispInfo->item.mask & TVIF_TEXT && pTVDispInfo->item.pszText)
  {
    TreeItemInfo *pTII = (TreeItemInfo *)Item.lParam;
    if (pTII)
    {
      xmlNodePtr pNode = (xmlNodePtr)pTII->m_Data;
      if (pNode)
      {
        xmlSetProp(pNode,(xmlChar *)"name",(xmlChar *)pTVDispInfo->item.pszText);
        return TRUE;
      }
    }
  }

  return 0;
}

LRESULT CFavoritesDlg::OnTvnSelchangedFavoritesTree(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
  LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

  TreeItemInfo *pTII = (TreeItemInfo *)pNMTreeView->itemNew.lParam;

  if (!pTII)
  {
    // m_NoteCtrl.SetWindowText("");
    return 0;
  }
  xmlNodePtr pNode = (xmlNodePtr)pTII->m_Data;
  if (pNode)
  {
    char *name = (char *)xmlGetProp(pNode,(xmlChar *)"name");
    char *networkidstr = (char *)xmlGetProp(pNode,(xmlChar *)"networkid");
    int networkid = -1;
    if (networkidstr)
    {
      if (sscanf(networkidstr,"%d",&networkid) != 1)
        networkid = -1;
      free(networkidstr);
    }

    if (name)
    {
      char *note = NULL;
      switch(pTII->m_Type)
      {
        case TII_GROUP:
          note = HydraIRC_BuildString(MAX_PATH,"Group name: %s",name);
          break;
        case TII_SERVER:
          note = HydraIRC_BuildString(MAX_PATH,"Server name: %s (%s)",name, GetNetworkName(networkid) );
          break;
        case TII_IRCCHANNEL:
          note = HydraIRC_BuildString(MAX_PATH,"Channel name: %s (%s)",name, GetNetworkName(networkid) );
          break;
        case TII_QUERY:
          note = HydraIRC_BuildString(MAX_PATH,"Nick: %s (%s)",name, GetNetworkName(networkid) );
          break;
      }
      if (note)
      {
        m_NoteCtrl.SetWindowText(note);
        free(note);
      }
      free(name);
    }

  }
  return 0;
}

LRESULT CFavoritesDlg::OnTvnBeginlabeleditFavoritesTree(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
  LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);

  TVITEM Item;

	Item.mask = TVIF_PARAM;
  Item.hItem = pTVDispInfo->item.hItem;

  if (m_TreeCtrl.GetItem(&Item) && pTVDispInfo->item.mask & TVIF_TEXT && pTVDispInfo->item.pszText)
  {
    TreeItemInfo *pTII = (TreeItemInfo *)Item.lParam;
    if (pTII == NULL)
      return TRUE; // probably trying to edit the root item.
  }

  // Knowledge Base Q130692 workaround
  CEdit EditCtrl = m_TreeCtrl.GetEditControl();
  m_TVEdit.SubclassWindow(EditCtrl.m_hWnd);
  m_TVEdit.SetLimitText(MAX_PATH);

  return 0;
}

LRESULT CFavoritesDlg::OnBnClickedFavoritesNewfolder(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  HTREEITEM hItem = m_TreeCtrl.GetSelectedItem();
  xmlNodePtr pOtherNode = NULL;
  BOOL AddAsChild = FALSE;  


  // if we have no selected item or if the selected item is the root
  // then we just add a new sibling
  if (!hItem || hItem == m_hRootItem)
  {
    hItem = m_hRootItem;
    pOtherNode = g_pPrefs->m_Favorites;
  }
  else
  {

    // if it's not the root we have to figure out where to put the new folder
    TVITEM item;
    item.mask = TVIF_PARAM;
    item.hItem = hItem;
    if (!m_TreeCtrl.GetItem(&item))
      return 0;

    TreeItemInfo *pTII = (TreeItemInfo *)item.lParam;
    pOtherNode = (xmlNodePtr)pTII->m_Data;
    if (pTII->m_Type == TII_GROUP)
    {
      // the selected item is a group, so we add it as a child to the xml tree AND the treeview
      AddAsChild = TRUE;
    }
    else
    {
      // the selected item is not a group so we add it as the last item for the item's parent node
      // and we add it as a sibling to the selected item in the xml tree
      hItem = m_TreeCtrl.GetParentItem(hItem);
    }
  }

  //if (!hItem)
  //  return 0; // couldn't get parent or root item.

  char *NewFolderName = "New Folder";
  
  xmlNodePtr pNewNode = xmlNewNode(NULL,(xmlChar *)"group");
  if (pNewNode)
  {
    TreeItemInfo *pNewTII = new TreeItemInfo(TII_GROUP,pNewNode);
    if (!pNewTII)
    {
      xmlFreeNode(pNewNode);
      return 0;
    }

    // add to treeview
    HTREEITEM hNewItem=m_TreeCtrl.InsertItem(TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM,
      (char *)NewFolderName,
      ICON_CHANNELS,ICON_CHANNELS, // image & selected image numbers
      0,0, // unused
      (LPARAM)pNewTII,
      hItem,hItem ? TVI_LAST : TVI_ROOT);

    if (hNewItem)
    {
      m_TIIList.Add(pNewTII);
      // add to XML tree (only if added to treeview)
      xmlSetProp(pNewNode,(xmlChar *)"name",(xmlChar *)NewFolderName);

      if (g_pPrefs->m_Favorites)
      {
        if (AddAsChild)
          xmlAddChild(pOtherNode,pNewNode);
        else
          xmlAddSibling(pOtherNode,pNewNode);
      }
      else
        g_pPrefs->m_Favorites = pNewNode;

      m_TreeCtrl.EnsureVisible(hNewItem);
      m_TreeCtrl.SelectItem(hNewItem);
      m_TreeCtrl.EditLabel(hNewItem);
    }
    else
    {
      delete pNewTII;
      xmlFreeNode(pNewNode);
    }
  }

  return 0;
}

void CFavoritesDlg::DeleteSelectedItem( void )
{
  HTREEITEM hItem = m_TreeCtrl.GetSelectedItem();
  xmlNodePtr pNode = NULL;

  if (!hItem)
    return; // we need a selected item!

  TVITEM item;
  item.mask = TVIF_PARAM;
  item.hItem = hItem;
  if (!m_TreeCtrl.GetItem(&item))
    return;

  TreeItemInfo *pTII = (TreeItemInfo *)item.lParam;
  if (!pTII) // wtf?
    return;

  pNode = (xmlNodePtr)pTII->m_Data;


  // delete stuff
  xmlNodePtr pTemp = pNode->next;
  xmlNodePtr pCheck = pNode;
  xmlUnlinkNode(pNode);
  xmlFreeNode(pNode);
  m_TreeCtrl.DeleteItem(hItem);

  // if we deleted the last remaining item or first item in the favorites list, then set favorites accordingly
  if (g_pPrefs->m_Favorites == pCheck)
    g_pPrefs->m_Favorites = pTemp;

  // now you're probably wondering how we're going to delete all the associated
  // TreeItemInfo structs, well, that's easy.  We don't do anything here,  they're
  // only 16 bytes each, and if you look above you'll see we add them to m_TIIList
  // so we can free them all when we delete the dialog class.

  return;
}

LRESULT CFavoritesDlg::OnBnClickedFavoritesDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  DeleteSelectedItem();
  return 0;
}

CFavoritesDlg::CFavoritesDlg( void )
{
  m_bLDragging = FALSE;
}

CFavoritesDlg::~CFavoritesDlg( void )
{
  ClearTree(NULL,m_TIIList);
}

LRESULT CFavoritesDlg::OnTvnBegindragFavoritesTree(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
  LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
  // TODO: Add your control notification handler code here

	m_hitemDrag = pNMTreeView->itemNew.hItem;
	
  TVITEM item;
  item.hItem = m_hitemDrag;
  item.mask = TVIF_PARAM;
  if (!m_TreeCtrl.GetItem(&item))
    return 0;

  TreeItemInfo *pTII = (TreeItemInfo *)item.lParam;
  if (!pTII)
    return 0;

  m_pDragSourceNode = (xmlNodePtr)pTII->m_Data;
  if (!m_pDragSourceNode)
    return 0; // ? wtf!?

  m_hitemDrop = NULL;

	m_TreeCtrl.SelectItem( m_hitemDrag );

	// get the image list for dragging 
	m_DragImage = m_TreeCtrl.CreateDragImage(m_hitemDrag);  
	// CreateDragImage() returns NULL if no image list 
	// associated with the tree view control 
	if( m_DragImage.IsNull() )
		return 0;

	m_bLDragging = TRUE;
	m_DragImage.BeginDrag(0, CPoint(15, 15));
	POINT pt = pNMTreeView->ptDrag;
	m_TreeCtrl.ClientToScreen( &pt );
  //::MapWindowPoints(HWND_DESKTOP, m_hWnd, &pt, 1);
	m_DragImage.DragEnter(NULL, pt);
	SetCapture();

  return 0;
}

LRESULT CFavoritesDlg::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  bHandled = FALSE;

	if (m_bLDragging)
	{
    TVHITTESTINFO ht = {0};
    DWORD dwpos = GetMessagePos();

    ht.pt.x = GET_X_LPARAM(dwpos);
    ht.pt.y = GET_Y_LPARAM(dwpos);
    m_DragImage.DragMove(ht.pt);
    ::MapWindowPoints(HWND_DESKTOP, m_TreeCtrl.m_hWnd, &ht.pt, 1);

    //m_TreeCtrl.ClientToScreen(&ht.pt);
    m_TreeCtrl.HitTest(&ht);
        

    m_hitemDrop = NULL;

    if(TVHT_ONITEM  & ht.flags)
    {
      m_DragImage.DragShowNolock(FALSE);

      if (ht.hItem == m_hRootItem)
        m_hitemDrop = m_hRootItem;
      else
      {     
        TVITEM item;
        item.hItem = ht.hItem;
        item.mask = TVIF_PARAM;
        if (m_TreeCtrl.GetItem(&item))
        {
          TreeItemInfo *pTII = (TreeItemInfo *)item.lParam;
          if (!pTII)
            return 0;

          if (pTII->m_Type == TII_GROUP)
          {
            m_hitemDrop = ht.hItem;
          }
          else
            m_hitemDrop = m_TreeCtrl.GetParentItem(ht.hItem);
        }
      }

      // don't allow an item to be moved into a parent of itself.
      HTREEITEM htiParent = m_hitemDrop;
		  while( (htiParent = m_TreeCtrl.GetParentItem( htiParent )) != NULL ) 
		  {
			  if( htiParent == m_hitemDrag) 
        {
          m_hitemDrop = NULL;
          break;
        }
		  }

      // don't allow drag into own folder or into itself
      if (m_hitemDrag == m_hitemDrop || m_hitemDrop == m_TreeCtrl.GetParentItem(m_hitemDrag))
        m_hitemDrop = NULL;

      m_TreeCtrl.SelectItem(m_hitemDrop);

      m_DragImage.DragShowNolock(TRUE);
    }
	}

  return 0;
}

LRESULT CFavoritesDlg::OnMouseButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  bHandled = FALSE;

	if (m_bLDragging)
	{
    m_bLDragging = FALSE;
		m_DragImage.DragLeave(m_TreeCtrl.m_hWnd);
		m_DragImage.EndDrag();
		ReleaseCapture();

		m_DragImage.Destroy();

    // Remove drop target highlighting
		m_TreeCtrl.SelectItem(NULL);

    if (!m_hitemDrop)
      return 0;

    m_TreeCtrl.Expand( m_hitemDrop, TVE_EXPAND ) ;

    TVITEM item;
    item.hItem = m_hitemDrop;
    item.mask = TVIF_PARAM;
    if (m_TreeCtrl.GetItem(&item))
    {
      xmlNodePtr pDestNode = NULL;
      if (m_hRootItem != m_hitemDrop) // not the root...
      {
        TreeItemInfo *pTII = (TreeItemInfo *)item.lParam;
        if (!pTII)
          return 0;

        pDestNode = (xmlNodePtr)pTII->m_Data;

        if (!pDestNode)
          return 0;
      }
      /*
      char *name = (char *)xmlGetProp(pDestNode,(xmlChar *)"name");

      if (!name)
        return 0;

      free(name);
      */
      // ok, here's the tricky bit.  we move move the source item, and any
      // children to the new location in the treeview, then we must
      // move the xml data to it's new location in the tree too.

      // unlink the xml node
      xmlNodePtr pTemp = m_pDragSourceNode->next;
      xmlNodePtr pCheck = m_pDragSourceNode;
      xmlUnlinkNode(m_pDragSourceNode);

      // delete the drag source item from it's old location
      m_TreeCtrl.DeleteItem(m_hitemDrag); // we don't bother freeing the associated TII's, instead we do it when the dialog closes...

      // if we unlinked the first item in the favorites list, then set favorites accordingly
      if (g_pPrefs->m_Favorites == pCheck)
        g_pPrefs->m_Favorites = pTemp;


      if (m_hRootItem != m_hitemDrop) // not at the root...
      {
        xmlAddChild(pDestNode,m_pDragSourceNode);
      }
      else
      {
        // special case for moving an item into the root.
        xmlAddSibling(g_pPrefs->m_Favorites,m_pDragSourceNode);
      }

      HTREEITEM hNewDest = NULL;
        
      PopulateFavoritesTree(&m_TreeCtrl,m_TIIList,m_hitemDrop,m_pDragSourceNode,m_pDragSourceNode,&hNewDest);

      // hNewDest should be pointing at m_hitemDrop's new location

      if (hNewDest)
        m_TreeCtrl.SelectItem(hNewDest);

    }
  }
  return 0;
}

LRESULT CFavoritesDlg::OnTvnKeydownFavoritesTree(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& bHandled)
{
  LPNMTVKEYDOWN pTVKeyDown = reinterpret_cast<LPNMTVKEYDOWN>(pNMHDR);
  // TODO: Add your control notification handler code here
  bHandled = FALSE;
  if (pTVKeyDown->wVKey == VK_DELETE)
  {
    DeleteSelectedItem();
    return 0;
  }
  return 0;
}

/********************************************************************************************/

CFavoritesAddDlg::CFavoritesAddDlg( xmlNodePtr pNewNode )
{
  // default add to the root.
  m_pDestNode = g_pPrefs->m_Favorites;
  m_pNewNode = pNewNode;
}

CFavoritesAddDlg::~CFavoritesAddDlg( void )
{
  ClearTree(NULL,m_TIIList);
}

char *str_Server = "Server";
char *str_Channel = "Channel";
char *str_Query = "Query";

LRESULT CFavoritesAddDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  CenterWindow(GetParent());
  DlgResize_Init();

  m_TreeCtrl.Attach(GetDlgItem(IDC_FAVORITESADD_TREE));
  //m_TypeCtrl.Attach(GetDlgItem(IDC_FAVORITESADD_TYPE));
  m_NameCtrl.Attach(GetDlgItem(IDC_FAVORITESADD_NAME));

  m_TreeCtrl.SetImageList(g_pMainWnd->m_images_serverlist, TVSIL_NORMAL);

  m_hRootItem=m_TreeCtrl.InsertItem(TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM,
    (char *)"Favorites",
    ICON_CHANNELS,ICON_CHANNELS, // image & selected image numbers
    0,0, // unused
    (LPARAM)NULL, // no TII for root item
    NULL,TVI_LAST);

  PopulateFavoritesTree(&m_TreeCtrl,m_TIIList,m_hRootItem,g_pPrefs->m_Favorites);

  m_TreeCtrl.Expand(m_hRootItem);
  m_TreeCtrl.SelectItem(m_hRootItem);

  /*
  m_TypeCtrl.AddString(str_Server);
  m_TypeCtrl.AddString(str_Channel);
  m_TypeCtrl.AddString(str_Query);
  */

  char *name = (char *)xmlGetProp(m_pNewNode,(xmlChar *)"name");
  m_NameCtrl.SetWindowText(name);
  m_NameCtrl.SetLimitText(512);
  if (name) free(name);

  /*
  char *type = (char *)xmlGetProp(m_pNewNode,(xmlChar *)"type");
  if (type)
  {
    switch (toupper(*type)) 
    {
      case 'S' :
        m_TypeCtrl.SelectString(0,str_Server);
        break;
      case 'C' :
        m_TypeCtrl.SelectString(0,str_Channel);
        break;
      case 'Q' :
        m_TypeCtrl.SelectString(0,str_Query);
        break;
    }
    free(type);
  }
  */

	return TRUE;
}

LRESULT CFavoritesAddDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CFavoritesAddDlg::OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND hWndCtl, BOOL& /*bHandled*/)
{
  // Knowledge Base Q130692 workaround
  if (hWndCtl != GetDlgItem(IDOK))
    return 1;


  char namebuf[512];
  m_NameCtrl.GetWindowText(namebuf,sizeof(namebuf)-1);
  namebuf[sizeof(namebuf)-1] = 0;
  xmlSetProp(m_pNewNode,(xmlChar *)"name",(xmlChar *)namebuf);

  /*
  char *type;
  type = (char *)xmlGetProp(m_pNewNode,(xmlChar *)"type");
  if (type)
  {
    if (stricmp(type,str_Server) == 0)
    {
      *type++ = 'S';
      *type = 0;
    }
    else if (stricmp(type,str_Channel) == 0)
    {
      *type++ = 'C';
      *type = 0;
    }
    else if (stricmp(type,str_Query) == 0)
    {
      *type++ = 'Q';
      *type = 0;
    }

    free(type);
  }
  */


  if (g_pPrefs->m_Favorites)
  {
    if (m_pDestNode == g_pPrefs->m_Favorites)
      xmlAddSibling(m_pDestNode,m_pNewNode);
    else
      xmlAddChild(m_pDestNode,m_pNewNode);
  }
  else
    g_pPrefs->m_Favorites = m_pNewNode;

  g_pPrefs->SavePrefs();

  EndDialog(1);    
	return 0;
}

LRESULT CFavoritesAddDlg::OnTvnSelchangedFavoritesTree(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
  LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

  TreeItemInfo *pTII = (TreeItemInfo *)pNMTreeView->itemNew.lParam;

  // root item has no TII.
  if (!pTII)
  {
    m_pDestNode = g_pPrefs->m_Favorites;
  }
  else
  {
    // if not a group item, use it's parent
    if (pTII->m_Type != TII_GROUP)
    {
      HTREEITEM hParent = m_TreeCtrl.GetParentItem(pNMTreeView->itemNew.hItem);
      TVITEM item;
      item.hItem = hParent;
      item.mask = TVIF_PARAM;
      if (m_TreeCtrl.GetItem(&item))
      {
        pTII = (TreeItemInfo *)item.lParam;
      }

      // TODO: Maybe we should select the hParent item?

    }

    if (pTII)
      m_pDestNode = (xmlNodePtr)pTII->m_Data;
    else
      m_pDestNode = g_pPrefs->m_Favorites;

  }

  return 0;
}

// Duplicated from C:FavoritesDlg, TODO: use a derived class?
LRESULT CFavoritesAddDlg::OnBnClickedFavoritesaddNewfolder(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  HTREEITEM hItem = m_TreeCtrl.GetSelectedItem();
  xmlNodePtr pOtherNode = NULL;
  BOOL AddAsChild = FALSE;  


  // if we have no selected item or if the selected item is the root
  // then we just add a new sibling
  if (!hItem || hItem == m_hRootItem)
  {
    hItem = m_hRootItem;
    pOtherNode = g_pPrefs->m_Favorites;
  }
  else
  {

    // if it's not the root we have to figure out where to put the new folder
    TVITEM item;
    item.mask = TVIF_PARAM;
    item.hItem = hItem;
    if (!m_TreeCtrl.GetItem(&item))
      return 0;

    TreeItemInfo *pTII = (TreeItemInfo *)item.lParam;
    pOtherNode = (xmlNodePtr)pTII->m_Data;
    if (pTII->m_Type == TII_GROUP)
    {
      // the selected item is a group, so we add it as a child to the xml tree AND the treeview
      AddAsChild = TRUE;
    }
    else
    {
      // the selected item is not a group so we add it as the last item for the item's parent node
      // and we add it as a sibling to the selected item in the xml tree
      hItem = m_TreeCtrl.GetParentItem(hItem);
    }
  }

  //if (!hItem)
  //  return 0; // couldn't get parent or root item.

  char *NewFolderName = "New Folder";
  
  xmlNodePtr pNewNode = xmlNewNode(NULL,(xmlChar *)"group");
  if (pNewNode)
  {
    TreeItemInfo *pNewTII = new TreeItemInfo(TII_GROUP,pNewNode);
    if (!pNewTII)
    {
      xmlFreeNode(pNewNode);
      return 0;
    }

    // add to treeview
    HTREEITEM hNewItem=m_TreeCtrl.InsertItem(TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM,
      (char *)NewFolderName,
      ICON_CHANNELS,ICON_CHANNELS, // image & selected image numbers
      0,0, // unused
      (LPARAM)pNewTII,
      hItem,hItem ? TVI_LAST : TVI_ROOT);

    if (hNewItem)
    {
      m_TIIList.Add(pNewTII);
      // add to XML tree (only if added to treeview)
      xmlSetProp(pNewNode,(xmlChar *)"name",(xmlChar *)NewFolderName);

      if (g_pPrefs->m_Favorites)
      {
        if (AddAsChild)
          xmlAddChild(pOtherNode,pNewNode);
        else
          xmlAddSibling(pOtherNode,pNewNode);
      }
      else
        g_pPrefs->m_Favorites = pNewNode;

      m_TreeCtrl.EnsureVisible(hNewItem);
      m_TreeCtrl.SelectItem(hNewItem);
      m_TreeCtrl.EditLabel(hNewItem);
    }
    else
    {
      delete pNewTII;
      xmlFreeNode(pNewNode);
    }
  }

  return 0;
}

LRESULT CFavoritesAddDlg::OnTvnBeginlabeleditFavoritesaddTree(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
  LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);

  TVITEM Item;

	Item.mask = TVIF_PARAM;
  Item.hItem = pTVDispInfo->item.hItem;

  if (m_TreeCtrl.GetItem(&Item) && pTVDispInfo->item.mask & TVIF_TEXT && pTVDispInfo->item.pszText)
  {
    TreeItemInfo *pTII = (TreeItemInfo *)Item.lParam;
    if (pTII == NULL)
      return TRUE; // probably trying to edit the root item.
  }

  // Knowledge Base Q130692 workaround
  CEdit EditCtrl = m_TreeCtrl.GetEditControl();
  m_TVEdit.SubclassWindow(EditCtrl.m_hWnd);
  m_TVEdit.SetLimitText(MAX_PATH);

  return 0;
}

LRESULT CFavoritesAddDlg::OnTvnEndlabeleditFavoritesaddTree(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
  LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);

  TVITEM Item;

	Item.mask = TVIF_PARAM;
  Item.hItem = pTVDispInfo->item.hItem;

  if (m_TreeCtrl.GetItem(&Item) && pTVDispInfo->item.mask & TVIF_TEXT && pTVDispInfo->item.pszText)
  {
    TreeItemInfo *pTII = (TreeItemInfo *)Item.lParam;
    if (pTII)
    {
      xmlNodePtr pNode = (xmlNodePtr)pTII->m_Data;
      if (pNode)
      {
        xmlSetProp(pNode,(xmlChar *)"name",(xmlChar *)pTVDispInfo->item.pszText);
        return TRUE;
      }
    }
  }

  return 0;
}
