class CMySplitter : public CWindowImpl< CMySplitter, CWindow,
			CWinTraits < WS_CHILD | WS_VISIBLE> >
{
public:
	enum Orientation {
		ORIENTATION_VERT,
		ORIENTATION_HORIZ
	};
private:
	RECT			m_rcLocation;	// dimensions of splitter itself 
	RECT			m_rcBoundingBox;// dimensions of workspace being split
	int 			m_CenterPoint;	// offset from 0 in parent's co-ordinates for center of splitter 
	Orientation		m_Orientation;	// whether splitter is vert or horiz
	HWND			m_hParentWnd;	// Handle to parent window
	bool			m_bBeingMoved;	// Are we in the middle of a move operation - 
									// WM_MOUSEMOVE handling is only done when true
	int				m_parentCoord;	// Location within parent of splitter
	
public:

	BEGIN_MSG_MAP(CMySplitter)
	MESSAGE_HANDLER(WM_PAINT, OnPaint)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
	MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
	END_MSG_MAP()
	
	CMySplitter():m_CenterPoint(0)
	{
		m_bBeingMoved=false;

	}

	// We wish to use a specific class cursor (IDC_SIZEALL),
	// so we define our own implementation of GetWndClassInfo
	static CWndClassInfo& GetWndClassInfo() { 
		static CWndClassInfo wc = { 
			{ sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW|\
				CS_DBLCLKS, StartWindowProc, 0, 0, NULL,
				NULL, NULL, (HBRUSH)(COLOR_WINDOW + 1),
				NULL, TEXT("MYSPLITTER"), NULL }, 
				NULL, NULL, IDC_SIZEALL, TRUE, 0, _T("")
			}; 
		return wc; 
	}

	#define MY_SPLITTER_HALFSIZE 3 // splitter width is twice this size

	void GetFirstPane(RECT &rcPane)
	{
		if (m_Orientation == ORIENTATION_VERT)
		{
			rcPane.left = m_rcBoundingBox.left;
			rcPane.top = m_rcBoundingBox.top;
			rcPane.right = m_CenterPoint-MY_SPLITTER_HALFSIZE;
			rcPane.bottom = m_rcBoundingBox.bottom;
		} 
		else // ORIENTATION_HORIZ
		{
			rcPane.left = m_rcBoundingBox.left;
			rcPane.top = m_rcBoundingBox.top;
			rcPane.right = m_rcBoundingBox.right;
			rcPane.bottom = m_CenterPoint-MY_SPLITTER_HALFSIZE;
		}
	}

	void GetSecondPane(RECT &rcPane)
	{
		if (m_Orientation == ORIENTATION_VERT)
		{
			rcPane.left = m_CenterPoint+MY_SPLITTER_HALFSIZE;
			rcPane.top = m_rcBoundingBox.top;
			rcPane.right = m_rcBoundingBox.right;
			rcPane.bottom = m_rcBoundingBox.bottom;
		} 
		else // ORIENTATION_HORIZ
		{
			rcPane.left = m_rcBoundingBox.left;
			rcPane.top = m_CenterPoint+MY_SPLITTER_HALFSIZE;
			rcPane.right = m_rcBoundingBox.right;
			rcPane.bottom = m_rcBoundingBox.bottom;
		}
	}
				
	void Init(HWND hParentWnd, Orientation orientation, int CenterPoint, RECT &rcBoundingBox)
	{
		m_Orientation = orientation;
		m_rcBoundingBox = rcBoundingBox;
		m_hParentWnd = hParentWnd;
		m_CenterPoint = CenterPoint;
		GenerateLocationRect();
		Create(m_hParentWnd, m_rcLocation, TEXT("Splitter"),0,0,0);
	}

	void SetBoundingBox(RECT &rcBoundingBox)
	{
		m_rcBoundingBox = rcBoundingBox;
		GenerateLocationRect();
		MoveWindow(&m_rcLocation, FALSE);
	}

	void GenerateLocationRect()
	{
		// based on the center point and orientation, generate the 
		// co-ordinates of the rectangle for the splitter
		if (m_Orientation == ORIENTATION_VERT)
		{
			m_rcLocation.left = m_CenterPoint-MY_SPLITTER_HALFSIZE;
			m_rcLocation.top = m_rcBoundingBox.top;
			m_rcLocation.right = m_CenterPoint+MY_SPLITTER_HALFSIZE;
			m_rcLocation.bottom = m_rcBoundingBox.bottom;

		}
		else // ORIENTATION_HORIZ
		{
			m_rcLocation.left = m_rcBoundingBox.left;
			m_rcLocation.top = m_CenterPoint-MY_SPLITTER_HALFSIZE;
			m_rcLocation.right = m_rcBoundingBox.right;
			m_rcLocation.bottom = m_CenterPoint+MY_SPLITTER_HALFSIZE;
		}
	}

	void FillSolidRect(HDC hDc, int x, int y, int x2, int y2, int index)
	{
		::SetBkColor(hDc, ::GetSysColor(index));
		RECT rect;
		rect.left = x;
		rect.top = y;
		rect.right = x2;
		rect.bottom = y2;
		::ExtTextOut(hDc, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
	}

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
	PAINTSTRUCT ps;
	::BeginPaint(m_hWnd, &ps);
	HDC hdc = ::GetDC(m_hWnd);
	
	OutputDebugString(TEXT("Entering OnPaint for splitter\n"));
		RECT rect;
		if (!::GetClientRect(m_hWnd, &rect))
			OutputDebugString(TEXT("Error\n"));
	COLORREF clrBtnFace, clrBtnShadow, clrBtnHilite;
	COLORREF clrBtnText, clrWindowFrame;

	clrBtnFace = ::GetSysColor(COLOR_BTNFACE);
	clrBtnShadow = ::GetSysColor(COLOR_BTNSHADOW);
	clrBtnHilite = ::GetSysColor(COLOR_BTNHIGHLIGHT);
	clrBtnText = ::GetSysColor(COLOR_BTNTEXT);
	clrWindowFrame = ::GetSysColor(COLOR_WINDOWFRAME);

#define CX_BORDER 2
#define CY_BORDER 2

	FillSolidRect(hdc, rect.left, rect.top, rect.right, rect.bottom, COLOR_BTNFACE);
	FillSolidRect(hdc, rect.left, rect.top, rect.right, CY_BORDER, COLOR_BTNHIGHLIGHT);
	FillSolidRect(hdc, rect.left, rect.top, CX_BORDER, rect.bottom, COLOR_BTNHIGHLIGHT);
	FillSolidRect(hdc, rect.right-CX_BORDER, rect.top, rect.right, rect.bottom, COLOR_BTNSHADOW);
	FillSolidRect(hdc, rect.left, rect.bottom-CY_BORDER, rect.right, rect.bottom, COLOR_BTNSHADOW);

	::EndPaint(m_hWnd, &ps);
	return 0;
	}

	bool PtInSplitter(int x, int y)
	{
		return true;
	}
	
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		OutputDebugString(_T("Entering Splitter's OnLButtonDown\n"));

		SetCapture();
		m_bBeingMoved = true;
		HDC hdc = ::GetDC(m_hParentWnd);
		SetROP2(hdc, R2_NOT);
		::Rectangle(hdc, 
			m_rcLocation.left, m_rcLocation.top, 
			m_rcLocation.right, m_rcLocation.bottom);
		SetROP2(hdc, R2_BLACK);
		::ReleaseDC(m_hParentWnd, hdc);
		m_parentCoord = m_CenterPoint-MY_SPLITTER_HALFSIZE;
		return 0;
	}

	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
	if (m_bBeingMoved)
		{
		OutputDebugString(_T("Entering Splitter's OnLButtonMove (during move)\n"));
			HDC hdc = ::GetDC(m_hParentWnd);
			SetROP2(hdc, R2_NOT);
			// erase old rectangle
			Rectangle(hdc, m_rcLocation.left, m_rcLocation.top, m_rcLocation.right, m_rcLocation.bottom);
			// update position
			if (m_Orientation == ORIENTATION_VERT)
			{
				short pos = (short)LOWORD(lParam);
				int delta = pos+m_parentCoord-m_CenterPoint;
				m_rcLocation.left += delta;
				m_rcLocation.right += delta;
				m_CenterPoint = pos+m_parentCoord; // x position
			}
		else // ORIENTATION_HORIZ
			{
				short pos = (short)HIWORD(lParam);
				int delta = pos+m_parentCoord-m_CenterPoint;
				m_rcLocation.top += delta;
				m_rcLocation.bottom += delta;
				m_CenterPoint = pos+m_parentCoord; // x position
			}

			// draw new rectangle
			Rectangle(hdc, m_rcLocation.left, m_rcLocation.top, m_rcLocation.right, m_rcLocation.bottom);
			SetROP2(hdc, R2_BLACK);
			::ReleaseDC(m_hParentWnd, hdc);
		}
		return 0;
	}

	LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
	OutputDebugString(_T("Entering Splitter's OnLButtonUp\n"));
	m_bBeingMoved = false;
	// update position
	if (m_Orientation == ORIENTATION_VERT)
	{
		short x = LOWORD(lParam); // x position
		m_CenterPoint = x+m_parentCoord; // x position
	}
	else // ORIENTATION_HORIZ
	{
		short y = HIWORD(lParam); // x position
		m_CenterPoint = y+m_parentCoord; // x position
	}

	HDC hdc = ::GetDC(m_hParentWnd);
	SetROP2(hdc, R2_NOT);
	// erase old rectangle
	Rectangle(hdc, m_rcLocation.left, m_rcLocation.top, m_rcLocation.right, m_rcLocation.bottom);
	SetROP2(hdc, R2_BLACK);
	::ReleaseDC(m_hParentWnd, hdc);
	ReleaseCapture();

	GenerateLocationRect();
	MoveWindow(&m_rcLocation, FALSE);
	
	// In splitter window, force draw of splitter in new location
	InvalidateRect(NULL, FALSE);
	// In parent, force removal of old splitter, and refresh and extra space
	::InvalidateRect(m_hParentWnd, NULL, TRUE); 
	return 0;
	}
};
