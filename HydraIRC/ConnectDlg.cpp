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

// Connectdlg.cpp : implementation of the CConnectDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"
#include "connectdlg.h"
#include ".\connectdlg.h"

void CConnectDlg::UpdateControls( void )
{
  TreeItemInfo *pTII;
  IRCServerDetails *pServerDetails;
  TVITEM item;
	item.mask = TVIF_PARAM;
	item.hItem = m_ServerTree.GetSelectedItem();

  if (m_ServerTree.GetItem(&item))
  {
    pTII = (TreeItemInfo*)item.lParam;
/*
    if (pTII->m_Type == TII_NETWORK)
    {
      // user clicked on a server
      // TODO: select first server in network.
    }
*/
    if (pTII->m_Type == TII_SERVER)
    {
      // user clicked on a server
      pServerDetails = (IRCServerDetails *)pTII->m_Data;
#ifdef DEBUG
      sys_Printf(BIC_INFO,"Selected: \"%s (%s)\"\n",pServerDetails->m_Description, pServerDetails->m_Name);
#endif

      // update our info (this must be done *BEFORE* we update the URI)
      m_pServerDetails->Copy(pServerDetails);

      // Fill in the URI with the server's info.
      char *newuri = pServerDetails->BuildURI();
      if (newuri)
      {
        m_ServerURI.SetWindowText(newuri);      
        free(newuri);      
      }
      m_Password.SetWindowText(pServerDetails->m_Password);

      // select the last user identity for the network.

      UserIdentity *pIdentity = g_pPrefs->GetPreferredIdentity(pServerDetails->m_NetworkID, 
        GPIF_UseDefault | GPIF_UseFirst);

      if (pIdentity)
        m_IdentityCtrl.SelectString(0,pIdentity->m_Description);
      else
        m_IdentityCtrl.SetCurSel(0);

    }

  }
}

LRESULT CConnectDlg::OnTreeSelectChange(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
{
  // when a user clicks on an item in the server treeview, we get a message
  // the message contains a pointer to the new and old selections
  // but doesn't contain the lParam, which we use to find out what the user clicked on
  // (because we don't know what kind of item (server or network) the user selected.)
  // we then get the TreeItemInfo struct back, check that and then finally act accordingly.

  /*
  TVITEM item;
	item.mask = TVIF_PARAM;
  LPNMTREEVIEW selectioninfo = (LPNMTREEVIEW)pnmh;  
  item.hItem = selectioninfo->itemNew.hItem;
  if (m_ServerTree.GetItem(&item))
  ...
  */

  // note: code move to UpdateControls() after above comment was written
  UpdateControls();

  //bHandled = FALSE;
  return 0;
}

void CConnectDlg::CleanupServerTree( void )
{
  while (m_TIIList.GetSize() > 0)
  {
    delete m_TIIList[0];
    m_TIIList.RemoveAt(0);
  }
}

void CConnectDlg::RefreshServerTree( IRCServerDetails *pSelectServer, int NetworkID )
{
  // use one or the other parameter, not both
  // use NULL or NETWORK_UNKNOWN for the unused item

  TreeItemInfo *pTII = NULL;
  HTREEITEM hItem,hSelectItem = NULL,hItemParent;
  IRCNetworkDetails *pNetworkDetails;

  m_ServerTree.DeleteAllItems();
  CleanupServerTree();
  // check the network list from start to end
  for (int n = 0 ; n < g_NetworkList.GetSize() ; n++ )
  {
    pNetworkDetails = g_NetworkList[n];

    // add the network
    // create a info structure, so we can find out what the user clicked on.
    pTII = new TreeItemInfo(TII_NETWORK,(void *)pNetworkDetails);
    m_TIIList.Add(pTII);

    hItemParent=m_ServerTree.InsertItem(TVIF_TEXT | TVIF_PARAM,
                                        pNetworkDetails->m_Name,
                                        0,0, // image & selected image numbers
                                        0,0, // unused
                                        (LPARAM)pTII,
                                        TVI_ROOT,TVI_SORT);
    hItem = TVI_LAST;

    // select this one?
    if (NetworkID == pNetworkDetails->m_ID)
      hSelectItem = hItemParent;

    // and add the servers to the tree as siblings

    for (int s = 0 ; s < g_ServerDetailsList.GetSize() ; s++ )
    {
      // add if this server is network we're looping though
      if (g_ServerDetailsList[s]->m_NetworkID == pNetworkDetails->m_ID)
      {
        // create a info structure, so we can find out what the user clicked on.
        pTII = new TreeItemInfo(TII_SERVER,(void *)g_ServerDetailsList[s]);
        m_TIIList.Add(pTII);

        hItem=m_ServerTree.InsertItem(TVIF_TEXT | TVIF_PARAM,
                                      g_ServerDetailsList[s]->m_Name,
                                      0,0, // image & selected image numbers
                                      0,0, // unused
                                      (LPARAM)pTII,
                                      hItemParent,TVI_SORT);
        if (hItem != NULL && 
            (
             (g_ServerDetailsList[s] == pSelectServer) 
             ||
             (pSelectServer && pSelectServer->m_Name && 
              stricmp(g_ServerDetailsList[s]->m_Name,pSelectServer->m_Name) == 0 && 
              g_ServerDetailsList[s]->m_Port == pSelectServer->m_Port)
            )
           )
        {
          // if we try and select this item now and then call, EnsureVisible() 
          // it doesn't scroll the item into view.
          // (something to do with still adding items I guess...)
          // so we save it for later, and then select it when we're done adding..
          hSelectItem = hItem;
        }
      }
    }
  }

  if (hSelectItem) // got something to select?
  {
    m_ServerTree.Expand(m_ServerTree.GetParentItem(hSelectItem));
    m_ServerTree.EnsureVisible(hSelectItem);
    m_ServerTree.SelectItem(hSelectItem);
  }
}

LRESULT CConnectDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{  
	CenterWindow(GetParent());

  DlgResize_Init();

#ifdef USE_XPCOMMANDBAR
	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
  m_CmdBar.Prepare();
#endif

  m_ServerURI.Attach(GetDlgItem(IDC_SERVERURI));
  m_ServerURI.Clear();

  m_Password.Attach(GetDlgItem(IDC_CONNECT_PASSWORD));
  m_Password.Clear();

  if (m_pServerDetails->m_Password)
    m_Password.SetWindowText(m_pServerDetails->m_Password);

  char *TempURI = NULL;
  if (m_pServerDetails->m_Name)
  {
    TempURI = m_pServerDetails->BuildURI();
  }

  if (TempURI)
  {
    m_ServerURI.AppendText(TempURI);
    free(TempURI);
  }
  else
  {
    m_ServerURI.AppendText("IRC://");
  }

  m_ServerTree.Attach(GetDlgItem(IDC_SERVERTREE));

  // add the list of identities to the identity selector.
  m_IdentityCtrl.Attach(GetDlgItem(IDC_CONNECT_IDENTITY));

  PopulateIdentityComboCtrl(m_IdentityCtrl, m_pIdentity);

  RefreshServerTree(m_pServerDetails,NETWORK_UNKNOWN);
	return TRUE;
}

LRESULT CConnectDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  EndDialog(0);    
  return 0;
}

void CConnectDlg::OnOK( void )
{
  char *uri = NULL;

  CopyWindowText(&m_ServerURI,uri);

  if (m_pServerDetails->ParseURI(uri))
  {
    int itemnum = m_IdentityCtrl.GetCurSel();     
    UserIdentity *pIdentity = (UserIdentity *) m_IdentityCtrl.GetItemData(itemnum);

    if (pIdentity && (int)pIdentity != -1)
    {
      if (!m_pServerDetails->m_Description)
        m_pServerDetails->m_Description = strdup(g_DefaultServerDetails->m_Description);
      
      CopyWindowText(&m_Password,m_pServerDetails->m_Password);

      m_pIdentity->Clear();
      m_pIdentity->Copy(pIdentity);
      // All OK!
	    EndDialog(1);    

    }
  }
  if (uri) free(uri);
}

LRESULT CConnectDlg::OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  OnOK();
	return 0;
}

CConnectDlg::CConnectDlg( IRCServerDetails *pServerDetails, UserIdentity *pIdentity)
{
  m_PrefsChanged = FALSE;
  m_pServerDetails = NULL;
  m_pIdentity = NULL;

  if (!pServerDetails || !pIdentity)
    return; // TODO, exception!
  m_pServerDetails = pServerDetails;
  m_pIdentity = pIdentity;
}

CConnectDlg::~CConnectDlg( void )
{
  CleanupServerTree();
  if (m_PrefsChanged)
    g_pPrefs->SavePrefs();
}

LRESULT CConnectDlg::OnEnUpdateServeruri(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  // if the user edits the uri directly the description and other server details
  // no longer apply, so we erase them.

  char *uri, *newuri = m_pServerDetails->BuildURI();
  int len = m_ServerURI.GetWindowTextLength();

  uri = (char *)malloc(len + 1);

  if (newuri && uri)
  {
    m_ServerURI.GetWindowText(uri,len + 1); // includes space for null terminator
    if (stricmp(newuri,uri) != 0)
    {
      if (m_pServerDetails->m_Description)
        free(m_pServerDetails->m_Description);
      m_pServerDetails->m_Description = NULL;

      if (m_pServerDetails->m_Password)
        free(m_pServerDetails->m_Password);
      m_pServerDetails->m_Password = NULL;

      m_pServerDetails->m_NetworkID = NETWORK_UNKNOWN;
    }
  }

  if (newuri) free(newuri);      
  if (uri) free(uri);

  return 0;
}

LRESULT CConnectDlg::OnEnSetfocusServeruri(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  m_ServerURI.SetSel(-1,-1);
  return 0;
}

LRESULT CConnectDlg::OnNMDblclkServertree(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
  // Comments out by BM on 15/08/2002
  /* 
  TVHITTESTINFO ht = {0};

  DWORD dwpos = GetMessagePos();

  ht.pt.x = GET_X_LPARAM(dwpos);
  ht.pt.y = GET_Y_LPARAM(dwpos);
  // TODO: check ok on multi-monitor system
  ::MapWindowPoints(HWND_DESKTOP, pNMHDR->hwndFrom, &ht.pt, 1);

  TreeView_HitTest(pNMHDR->hwndFrom, &ht);
      
  if(TVHT_ONITEM & ht.flags)
  {
    OnOK();
  }  
  */

	HTREEITEM item = m_ServerTree.GetSelectedItem();
	if(m_ServerTree.GetChildItem(item) == NULL) // no child items means it's a server item.
	{
		OnOK();
  }  

  return 0;
}

LRESULT CConnectDlg::OnBnClickedConnectEditidentity(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  int itemnum = m_IdentityCtrl.GetCurSel();     
  UserIdentity *pOldIdentity = (UserIdentity *)m_IdentityCtrl.GetItemData(itemnum);
  ATLASSERT (pOldIdentity);
  UserIdentity *pNewIdentity = new UserIdentity(pOldIdentity);

  // edit a copy of the old one.
  CUserIdentityDlg Dlg(pNewIdentity);
  int result = Dlg.DoModal();

  if (result == 1)
  {
    g_pPrefs->m_UserIdentitiesList.Remove(pOldIdentity);
    g_pPrefs->m_UserIdentitiesList.Add(pNewIdentity);

    m_PrefsChanged = TRUE;

    // we have to update the list too..
    m_IdentityCtrl.ResetContent();
    for (int i = 0 ; i < g_pPrefs->m_UserIdentitiesList.GetSize() ; i++ )
    {
      itemnum = m_IdentityCtrl.AddString(g_pPrefs->m_UserIdentitiesList[i]->m_Description);
      m_IdentityCtrl.SetItemDataPtr(itemnum,g_pPrefs->m_UserIdentitiesList[i]);
      if (pNewIdentity == g_pPrefs->m_UserIdentitiesList[i])
        m_IdentityCtrl.SetCurSel(itemnum);
    }    
    
    delete pOldIdentity;
  }
  else
  {
    delete pNewIdentity;
  }
  return 0;
}

void CConnectDlg::ShowContextMenu( HTREEITEM selectitem )
{
  TreeItemInfo *pTII;
  IRCServerDetails *pServerDetails;
  IRCNetworkDetails *pNetworkDetails;
  TVITEM item;

  if (!selectitem)
    selectitem = m_ServerTree.GetSelectedItem();

	item.mask = TVIF_PARAM;
	item.pszText = NULL;
  item.hItem = selectitem;

  if (m_ServerTree.GetItem(&item))
  {
    pTII = (TreeItemInfo*)item.lParam;
    if (pTII->m_Type == TII_NETWORK)
    {
      // user clicked on a network
      pNetworkDetails = (IRCNetworkDetails *)pTII->m_Data;
#ifdef DEBUG
      sys_Printf(BIC_INFO,"Selected: \"%s\"\n",pNetworkDetails->m_Name);
#endif

      CMenuHandle menuContext;
			menuContext.LoadMenu(IDR_CONNECT_SERVERCONTEXT);
			CMenuHandle menuPopup(menuContext.GetSubMenu(1)); // network menu is second submenu
      // note: no default item for this menu

      int Flags = MF_BYCOMMAND;
      if (pNetworkDetails->m_Flags & INDF_CUSTOMLIST)
        Flags |= MF_ENABLED;
      else
        Flags |= MF_GRAYED;
      menuPopup.EnableMenuItem(ID_CONNECT_NC_DELETE,Flags);

      // Display the menu at the current mouse location.
      POINT pt;
      ::GetCursorPos(&pt);      
      //g_pMainWnd->m_CmdBar.TrackPopupMenuEx(menuPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, m_hWnd);
#ifdef USE_XPCOMMANDBAR
      m_CmdBar.TrackPopupMenu(menuPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y);
#else
      ::TrackPopupMenu(menuPopup, 0, pt.x, pt.y, 0, m_hWnd, NULL);
#endif


    }
    else if (pTII->m_Type == TII_SERVER)
    {
      // user clicked on a server
      pServerDetails = (IRCServerDetails *)pTII->m_Data;
#ifdef DEBUG
      sys_Printf(BIC_INFO,"Selected: \"%s (%s)\"\n",pServerDetails->m_Description, pServerDetails->m_Name);
#endif

      CMenuHandle menuContext;
			menuContext.LoadMenu(IDR_CONNECT_SERVERCONTEXT);
			CMenuHandle menuPopup(menuContext.GetSubMenu(0)); // server menu is first submenu
      // Make first menu-item the default (bold font)      
      menuPopup.SetMenuDefaultItem(0,TRUE);

      int Flags = MF_BYCOMMAND;
      if (pServerDetails->m_Flags & ISDF_CUSTOMLIST)
        Flags |= MF_ENABLED;
      else
        Flags |= MF_GRAYED;
      menuPopup.EnableMenuItem(ID_CONNECT_SC_DELETE,Flags);

      // Display the menu at the current mouse location.
      POINT pt;
      ::GetCursorPos(&pt);      
      //g_pMainWnd->m_CmdBar.TrackPopupMenuEx(menuPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, m_hWnd);
#ifdef USE_XPCOMMANDBAR
      m_CmdBar.TrackPopupMenu(menuPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y);
#else
      ::TrackPopupMenu(menuPopup, 0, pt.x, pt.y, 0, m_hWnd, NULL);
#endif
    }
  }  
}

LRESULT CConnectDlg::OnNMRclickServertree(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
  TVHITTESTINFO ht = {0};

  DWORD dwpos = GetMessagePos();

  ht.pt.x = GET_X_LPARAM(dwpos);
  ht.pt.y = GET_Y_LPARAM(dwpos);
  // TODO: check ok on multi-monitor system
  ::MapWindowPoints(HWND_DESKTOP, pNMHDR->hwndFrom, &ht.pt, 1);

  TreeView_HitTest(pNMHDR->hwndFrom, &ht);
      
  if(TVHT_ONITEM & ht.flags)
  {
    m_ServerTree.SelectItem(ht.hItem);
    ShowContextMenu(ht.hItem);
  }  
  return 0;
}

// context menu

LRESULT CConnectDlg::OnContextMenuServerConnect(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  OnOK();
  return TRUE; // ?
}

LRESULT CConnectDlg::OnContextMenuServerAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  AddServer();
  return TRUE; // ?
}

LRESULT CConnectDlg::OnContextMenuServerEdit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  EditServer();
  return TRUE; // ?
}

LRESULT CConnectDlg::OnContextMenuServerDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  DeleteServer();
  return TRUE; // ?
}

LRESULT CConnectDlg::OnContextMenuNetworkAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  AddNetwork();
  return TRUE; // ?
}

LRESULT CConnectDlg::OnContextMenuNetworkEdit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  EditNetwork();
  return TRUE; // ?
}

LRESULT CConnectDlg::OnContextMenuNetworkDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  DeleteNetwork();
  return TRUE; // ?
}

// buttons

LRESULT CConnectDlg::OnBnClickedConnectAddnetwork(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  AddNetwork();
  return 0;;
}

LRESULT CConnectDlg::OnBnClickedConnectAddserver(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  AddServer();
  return 0; // ?
}

void CConnectDlg::EditNetwork( void )
{
  // show edit server dialog and update tree item from new details

  TreeItemInfo *pTII;
  IRCNetworkDetails *pND,*pNewND;
  TVITEM item;

	item.mask = TVIF_PARAM;
	item.pszText = NULL;
  item.hItem = m_ServerTree.GetSelectedItem();

  if (m_ServerTree.GetItem(&item))
  {
    pTII = (TreeItemInfo*)item.lParam;
  }

  if (!pTII || pTII->m_Type != TII_NETWORK)
    return; // How'd we get here?

  pND = (IRCNetworkDetails *)pTII->m_Data;

  pNewND = new IRCNetworkDetails(pND);

  if (!pNewND)
    return;

  // edit a copy of the old one.
  CAddNetworkDlg Dlg(pNewND);
  int result = Dlg.DoModal();

  if (result == 1)
  {
    // make sure we save it into the user's profile if it's been edited!
    pNewND->m_Flags |= INDF_CUSTOMLIST;

    // find the list index of the old network details instance
    int Index = g_NetworkList.Find(pND);
    if (Index >= 0)
    {
      g_NetworkList[Index] = pNewND;

      // update the item then
      pTII->m_Data = (void *)pNewND;
      m_ServerTree.SetItemText(item.hItem,pNewND->m_Name);
      m_ServerTree.SortChildren(m_ServerTree.GetParentItem(item.hItem));
      m_ServerTree.EnsureVisible(item.hItem);

      // delete the old instance
      delete pND;

      UpdateControls(); // show the changes in the connect dialog (if needed)
      m_PrefsChanged = TRUE;
    }
    else
      delete pNewND; // cleanup if we fail.
    
    // OK
  }
  else
  {
    // Cancel
    delete pNewND;
  }  
}

void CConnectDlg::EditServer( void )
{
  // show edit server dialog and update tree item from new details

  TreeItemInfo *pTII;
  IRCServerDetails *pSD,*pNewSD;
  TVITEM item;

	item.mask = TVIF_PARAM;
	item.pszText = NULL;
  item.hItem = m_ServerTree.GetSelectedItem();

  if (m_ServerTree.GetItem(&item))
  {
    pTII = (TreeItemInfo*)item.lParam;
  }

  if (!pTII || pTII->m_Type != TII_SERVER)
    return; // How'd we get here?

  pSD = (IRCServerDetails *)pTII->m_Data;
  pNewSD = new IRCServerDetails(pSD);

  if (!pNewSD)
    return;

  // edit a copy of the old one.
  CEditServerDetailsDlg Dlg(pNewSD);
  int result = Dlg.DoModal();

  if (result == 1)
  {
    // make sure we save it into the user's profile if it's been edited!
    pNewSD->m_Flags |= ISDF_CUSTOMLIST;

    int Index = g_ServerDetailsList.Find(pSD);
    if (Index >= 0)
    {
      g_ServerDetailsList[Index] = pNewSD;
      if (pNewSD->m_NetworkID != pSD->m_NetworkID) // server moved networks?
      {
        RefreshServerTree(pNewSD,NETWORK_UNKNOWN);
      }
      else
      {
        // no, just update the item then
        pTII->m_Data = (void *)pNewSD;
        m_ServerTree.SetItemText(item.hItem,pNewSD->m_Name);
        m_ServerTree.SortChildren(m_ServerTree.GetParentItem(item.hItem));
        m_ServerTree.EnsureVisible(item.hItem);
      }
      delete pSD;

      UpdateControls(); // show the changes int eh connect dialog.

      // Save prefs, server list changed!
      m_PrefsChanged = TRUE;
    }
    else
      delete pNewSD; // cleanup if we fail.
    
    // OK
  }
  else
  {
    // Cancel
    delete pNewSD;
  }  
}


IRCNetworkDetails *CConnectDlg::AddNetwork( void )
{
  BOOL OK = FALSE;
  IRCNetworkDetails *pNewND = new IRCNetworkDetails();

  if (pNewND)
  {      
    CAddNetworkDlg Dlg(pNewND);
    int result = Dlg.DoModal();

    if (result == 1)
    {
      // Assign a new network id in the user-supplied network id range.
      // TODO: create a define for 1 and 10000

      int id;
      id = 1; // start from 1.
      // note: 10000 is an arbritary number set by me.
      // hydrairc's supplied list of network's start with id's >= 10000
      while (GetNetworkDetails(id) != NULL && id < 10000)
        id++;

      if (id < 10000)
      {
        pNewND->m_ID = id;
        pNewND->m_Flags = INDF_CUSTOMLIST;
        g_NetworkList.Add(pNewND);
        RefreshServerTree(NULL, id);
        OK = TRUE;
      }
    }
  }

  if (!OK) // user cancelled dialog or no id's left.
  {
    delete pNewND;
    return NULL;
  }
  else
    return pNewND;
}

IRCServerDetails *CConnectDlg::AddServer( void )
{
  // show edit server dialog and update tree item from new details

  // get the the network details for the currently selected item


  TreeItemInfo *pTII;
  IRCNetworkDetails *pND = NULL;
  TVITEM item;
  HTREEITEM SelectedItem = m_ServerTree.GetSelectedItem();
  HTREEITEM ParentItem = SelectedItem ? m_ServerTree.GetParentItem(SelectedItem) : NULL;

  if (SelectedItem != NULL)
  {
	  item.mask = TVIF_PARAM;
	  item.pszText = NULL;
    item.hItem = SelectedItem;

    if (m_ServerTree.GetItem(&item))
    {
      pTII = (TreeItemInfo*)item.lParam;
    }

    if (ParentItem && pTII && pTII->m_Type == TII_SERVER)
    {
      pND = (IRCNetworkDetails *)pTII->m_Data;
      item.hItem = ParentItem;
      if (m_ServerTree.GetItem(&item))
      {
        pTII = (TreeItemInfo*)item.lParam;
        // and follow through to the check below..
      }
    }

    if (pTII && pTII->m_Type == TII_NETWORK)
    {
      pND = (IRCNetworkDetails *)pTII->m_Data;
    }
  }


  IRCServerDetails *pNewSD;

  pNewSD = new IRCServerDetails(g_DefaultServerDetails);

  if (!pNewSD)
    return 0; // ?

  pNewSD->m_Flags |= ISDF_CUSTOMLIST; // it's a new server that the user created!
  if (pND)
    pNewSD->m_NetworkID = pND->m_ID;

  // edit it
  CEditServerDetailsDlg Dlg(pNewSD);
  int result = Dlg.DoModal();

  if (result == 1)
  {
    // OK
    g_ServerDetailsList.Add(pNewSD);
    RefreshServerTree(pNewSD, NETWORK_UNKNOWN);
    // Save prefs, server list changed!
    m_PrefsChanged = TRUE;
    return pNewSD;
  }
  else
  {
    // Cancel
    delete pNewSD;
    return NULL;
  }  
}

void CConnectDlg::DeleteServer( void )
{
  TreeItemInfo *pTII;
  IRCServerDetails *pSD;
  TVITEM item;

	item.mask = TVIF_PARAM;
	item.pszText = NULL;
  item.hItem = m_ServerTree.GetSelectedItem();

  if (m_ServerTree.GetItem(&item))
  {
    pTII = (TreeItemInfo*)item.lParam;
  }

  if (!pTII || pTII->m_Type != TII_SERVER)
    return; // ? // How'd we get here?

  pSD = (IRCServerDetails *)pTII->m_Data;

  if (!(pSD->m_Flags & ISDF_CUSTOMLIST))
    return; // can't delete these!!

  // select the parent of this item
  m_ServerTree.SelectItem(m_ServerTree.GetParentItem(item.hItem));

  // delete the item and it's data from everywhere
  m_ServerTree.DeleteItem(item.hItem);
  m_TIIList.Remove(pTII);
  delete pTII;
  g_ServerDetailsList.Remove(pSD);
  delete pSD;

  m_PrefsChanged = TRUE;
}

void CConnectDlg::DeleteNetwork( void )
{
  TreeItemInfo *pTII;
  IRCNetworkDetails *pND;
  TVITEM item;

	item.mask = TVIF_PARAM;
	item.pszText = NULL;
  item.hItem = m_ServerTree.GetSelectedItem();

  if (m_ServerTree.GetItem(&item))
  {
    pTII = (TreeItemInfo*)item.lParam;
  }

  if (!pTII || pTII->m_Type != TII_NETWORK)
    return; // ? // How'd we get here?

  pND = (IRCNetworkDetails *)pTII->m_Data;

  if (!(pND->m_Flags & INDF_CUSTOMLIST))
    return; // can't delete these!!

  if ((pND->m_Flags & (INDF_CUSTOMLIST | INDF_MASTERLIST)) == (INDF_CUSTOMLIST | INDF_MASTERLIST) ) // both flags set?
  {
    // this item ORIGINATED from the list supplied with hydrairc, as such we can't
    // delete it from the list, so we just clear the custom fields then it won't
    // be saved in the users profile any more.
    pND->RemoveCustomStatus();
    m_PrefsChanged = TRUE;
    return;
  }

  // select the parent of this item
  m_ServerTree.SelectItem(m_ServerTree.GetParentItem(item.hItem));

  // delete the network item and it's servers from everywhere
  m_ServerTree.DeleteItem(item.hItem);
  m_TIIList.Remove(pTII);
  delete pTII;
  // note: the TreeItemInfo structures for the server items are now orphaned, they'll be free'd when the dialog is closed.

  int id;
  for (id = 0; id < g_ServerDetailsList.GetSize(); id++)
  {
    IRCServerDetails *pSD = g_ServerDetailsList[id];
    if (pSD->m_NetworkID == pND->m_ID)
    {
      g_ServerDetailsList.Remove(pSD);
      delete pSD;
      id--; // step back one
    }
  }
  g_NetworkList.Remove(pND);
  delete pND;
  m_PrefsChanged = TRUE;
}    
