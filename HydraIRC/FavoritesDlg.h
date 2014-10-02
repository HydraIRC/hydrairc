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

// FavoritesDlg.h : interface of the CFavoritesDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

template <class T>
class CTVEditT : 
      public CWindowImpl<CTVEditT<T> ,CEdit>
{
public:

  BEGIN_MSG_MAP(CTVEditT< T >)
    MESSAGE_HANDLER(WM_GETDLGCODE , OnWMGETDLGCODE )
  END_MSG_MAP()

  LRESULT OnWMGETDLGCODE( UINT uMsg,
                  WPARAM wParam,
                  LPARAM lParam,
                  BOOL& bHandled)
  {
    return (DLGC_WANTALLKEYS);
  }

};

typedef CTVEditT<CWindow>		CTVEdit;


class CFavoritesDlg : 
  public CDialogImpl<CFavoritesDlg>,
  public CDialogResize<CFavoritesDlg>
{
private:
  // Controls
  CTreeViewCtrl m_TreeCtrl;
  CTVEdit m_TVEdit;
  CStatic m_NoteCtrl;
  //CButton m_AddCtrl;
  //CButton m_DeleteCtrl;
  //CButton m_NewFolderCtrl;  
  CSimpleArray<TreeItemInfo *> m_TIIList;

  // for dragging and dropping...
	CImageList      m_DragImage;
	BOOL            m_bLDragging;
	HTREEITEM       m_hitemDrag,m_hitemDrop,m_hRootItem;
	HCURSOR         m_dropCursor,m_noDropCursor;
  xmlNodePtr      m_pDragSourceNode;

public:
	enum { IDD = IDD_FAVORITES };

  CFavoritesDlg( void );
  ~CFavoritesDlg( void );

  void DeleteSelectedItem( void );
    
	BEGIN_MSG_MAP(CFavoritesDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOKCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
    MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
    MESSAGE_HANDLER(WM_LBUTTONUP, OnMouseButtonUp)
    NOTIFY_HANDLER(IDC_FAVORITES_TREE, TVN_ENDLABELEDIT, OnTvnEndlabeleditFavoritesTree)
    NOTIFY_HANDLER(IDC_FAVORITES_TREE, TVN_SELCHANGED, OnTvnSelchangedFavoritesTree)
    NOTIFY_HANDLER(IDC_FAVORITES_TREE, TVN_BEGINLABELEDIT, OnTvnBeginlabeleditFavoritesTree)
    COMMAND_HANDLER(IDC_FAVORITES_NEWFOLDER, BN_CLICKED, OnBnClickedFavoritesNewfolder)
    COMMAND_HANDLER(IDC_FAVORITES_DELETE, BN_CLICKED, OnBnClickedFavoritesDelete)
    NOTIFY_HANDLER(IDC_FAVORITES_TREE, TVN_BEGINDRAG, OnTvnBegindragFavoritesTree)
    NOTIFY_HANDLER(IDC_FAVORITES_TREE, TVN_KEYDOWN, OnTvnKeydownFavoritesTree)
    CHAIN_MSG_MAP(CDialogResize<CFavoritesDlg>)
  END_MSG_MAP()

  BEGIN_DLGRESIZE_MAP(CFavoritesDlg)
    DLGRESIZE_CONTROL(IDC_FAVORITES_TREE      ,DLSZ_SIZE_X | DLSZ_SIZE_Y)
    DLGRESIZE_CONTROL(IDOK                    ,DLSZ_MOVE_X | DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_FAVORITES_NOTE      ,DLSZ_MOVE_Y | DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_FAVORITES_NEWFOLDER ,DLSZ_MOVE_Y )
    DLGRESIZE_CONTROL(IDC_FAVORITES_DELETE    ,DLSZ_MOVE_Y )
  END_DLGRESIZE_MAP()


// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

  LRESULT OnTvnEndlabeleditFavoritesTree(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
  LRESULT OnTvnSelchangedFavoritesTree(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
  LRESULT OnTvnBeginlabeleditFavoritesTree(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
  LRESULT OnBnClickedFavoritesNewfolder(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedFavoritesDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnTvnKeydownFavoritesTree(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);

  // drag and drop...
  LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnMouseButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnTvnBegindragFavoritesTree(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);

};

class CFavoritesAddDlg : 
  public CDialogImpl<CFavoritesAddDlg>,
  public CDialogResize<CFavoritesAddDlg>
{
private:
  // Controls
  CTreeViewCtrl m_TreeCtrl;
  CTVEdit m_TVEdit;
  CEdit m_NameCtrl;
  //CComboBox m_TypeCtrl;
  CSimpleArray<TreeItemInfo *> m_TIIList;

	HTREEITEM       m_hRootItem;
  xmlNodePtr      m_pDestNode,m_pNewNode;

public:
	enum { IDD = IDD_FAVORITESADD };

  CFavoritesAddDlg( xmlNodePtr pNewNode );
  ~CFavoritesAddDlg( void );
    
	BEGIN_MSG_MAP(CFavoritesAddDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOKCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
    NOTIFY_HANDLER(IDC_FAVORITESADD_TREE, TVN_SELCHANGED, OnTvnSelchangedFavoritesTree)
    COMMAND_HANDLER(IDC_FAVORITESADD_NEWFOLDER, BN_CLICKED, OnBnClickedFavoritesaddNewfolder)
    NOTIFY_HANDLER(IDC_FAVORITESADD_TREE, TVN_BEGINLABELEDIT, OnTvnBeginlabeleditFavoritesaddTree)
    NOTIFY_HANDLER(IDC_FAVORITESADD_TREE, TVN_ENDLABELEDIT, OnTvnEndlabeleditFavoritesaddTree)
    CHAIN_MSG_MAP(CDialogResize<CFavoritesAddDlg>)
  END_MSG_MAP()

  BEGIN_DLGRESIZE_MAP(CFavoritesAddDlg)
    DLGRESIZE_CONTROL(IDC_FAVORITESADD_TREE      ,DLSZ_SIZE_X | DLSZ_SIZE_Y)
    DLGRESIZE_CONTROL(IDOK                       ,DLSZ_MOVE_X)
    DLGRESIZE_CONTROL(IDCANCEL                   ,DLSZ_MOVE_X)
    DLGRESIZE_CONTROL(IDC_FAVORITESADD_NAME      ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_FAVORITESADD_NEWFOLDER ,DLSZ_MOVE_X)
  END_DLGRESIZE_MAP()


// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

  LRESULT OnTvnSelchangedFavoritesTree(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);

  LRESULT OnBnClickedFavoritesaddNewfolder(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnTvnBeginlabeleditFavoritesaddTree(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
  LRESULT OnTvnEndlabeleditFavoritesaddTree(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
};
