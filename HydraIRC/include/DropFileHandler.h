#ifndef __DROPFILEHANDLER_H__
#define __DROPFILEHANDLER_H__

// Helper class for CDropFilesHandler, or any other handler for WM_DROPFILES.
// This one is a template to avoid compiling when not used.
template <class CHAR> class CDrag
{
	HDROP m_hd;
	bool bHandled;
	CHAR m_buff[MAX_PATH + 5]; // DragQueryFile() wants LPTSTR
public:
	// Constructor, destructor
	CDrag(WPARAM wParam)
	{
		m_hd = (HDROP) wParam; 
		bHandled = false;
		m_buff[0] = '\0';
	}
	~CDrag()
	{
		if (bHandled)
			::DragFinish(m_hd);
	}
	// Helper function, detects if the message is meant for 'this' window.
	BOOL IsInClientRect(HWND hw)
	{
		POINT p;
		::DragQueryPoint(m_hd, &p);
		RECT rc;
		::GetClientRect(hw, &rc);
		return ::PtInRect(&rc, p);
	}
	UINT GetNumFiles(void)
	{
		return ::DragQueryFile(m_hd, 0xffffFFFF, NULL, 0);
	}
	UINT DragQueryFile(UINT iFile)
	{
		bHandled = true;
		return ::DragQueryFile(m_hd, iFile, m_buff, MAX_PATH);
	}
	// CString overload for DragQueryFile (not used here, might be used by a handler
	// which is implemented outside CDropFilesHandler<t>.
#ifdef _WTL_USE_CSTRING
	UINT DragQueryFile(UINT iFile, CString &cs)
	{
		bHandled = true;
		UINT ret = ::DragQueryFile(m_hd, iFile, m_buff, MAX_PATH);
		cs = m_buff;
		return ret;
	}
	inline operator CString() const { return CString(m_buff); }
#endif
	// Other string overloads might come handy...

	// This class can 'be' the currently held file's path.
	inline operator const CHAR *() const { return m_buff; }
};

template <class T> class CDropFilesHandler
{
	public:
	// We'll use a message map here, so future implementations can, if necessary, handle
	// more than one message.
	BEGIN_MSG_MAP(CDropFilesHandler<T>)
		MESSAGE_HANDLER(WM_DROPFILES, OnDropFiles) 
	END_MSG_MAP()
	// WM_DROPFILES handler: it calls a T function, with the path to one dropped file each time.
	LRESULT OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		ATLTRACE("*** Got WM_DROPFILE! *******\n");
        T* pT = static_cast<T*>(this);
		// Your class should implement a boolean function. It should return TRUE only
		// if initialization (whatever is tried) was successful.
		if (!pT->IsReadyForDrop())
		{
			bHandled = FALSE;
			return 0;
		}
		CDrag<CHAR> cd(wParam);
		// Your class should have a public member of type HWND (like CWindow does).
		if (cd.IsInClientRect(pT->m_hWnd))
		{
			UINT nfiles = cd.GetNumFiles();
			if (0 < nfiles)
			{
				for (UINT i = 0; i < nfiles; i++)
				{
					if (0 < cd.DragQueryFile(i))
					{
						ATLTRACE("   Dropped one! %s\n", (LPCTSTR) cd);
						// Your class should return TRUE unless no more files are wanted.
						if (!pT->HandleDroppedFile((LPCTSTR) cd))
							i = nfiles + 10; // Break the loop
					}
					pT->EndDropFiles();
				} // for
			}
		}
		else 
			bHandled = FALSE;
		return 0;
	}
	// For frame windows:
	void RegisterDropHandler(void)
	{
        T* pT = static_cast<T*>(this);
		ATLASSERT(::IsWindow(pT->m_hWnd));
		CWindow wnd;
		wnd.Attach(pT->m_hWnd);
		wnd.ModifyStyleEx(0, WS_EX_ACCEPTFILES);
	}

};

/*

Usage:

class CMyDlg : public CDialogImpl<CMyDlg>,
               public CDropFilesHandler<CMyDlg> // Inherit from the template, as usual...
{
public:
	enum { IDD = IDD_MYDIALOG }; // Make sure this one has EXSTYLE WS_EX_ACCEPTFILES defined.

	BEGIN_MSG_MAP(CInsertObjectDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog) // and others...
		...
		CHAIN_MSG_MAP(CDropFilesHandler<CMyDlg>)
	END_MSG_MAP()

    // You can use this function to initialize variables needed for one drop's batch.
	BOOL IsReadyForDrop(void)
	{
		return m_bIsReadyForDrop; // initialized in OnInitDialog, false in constructor.
	}
	BOOL HandleDroppedFile(LPCTSTR szBuff)
	{
		// Implement this.
	    // Return TRUE unless you're done handling files (e.g., if you want 
		// to handle only the first relevant file, and you have already found it).
		ATLTRACE("%s\n", szBuff);
		return TRUE;
	}
	void EndDropFiles(void)
	{
		// Sometimes, if your file handling is not trivial, you might want to add all
		// file names to some container, and do the handling afterwards, in a worker
		// thread. If so, this function is your choice.

		// After handling, get ready for another round.
		m_bIsReadyForDrop = true;
	}
private:
	BOOL m_bIsReadyForDrop;  
*/

#endif //__DROPFILEHANDLER_H__