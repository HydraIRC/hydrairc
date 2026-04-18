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

// ChildCommon.h : comment here
//
// These routines are common to all MDI child windows.
/////////////////////////////////////////////////////////////////////////////

class CChildCommon : 
#ifdef USE_TABBEDMDI
  public CTabbedMDIChildWindowImpl<CChildCommon>
#else
  public FixedMDIChildWindowImpl<CChildCommon>
#endif
{
protected:
#ifdef USE_TABBEDMDI
  typedef CTabbedMDIChildWindowImpl<CChildCommon> baseClass;
#else
  typedef FixedMDIChildWindowImpl<CChildCommon> baseClass;
#endif
  
public:
  short m_WindowType;

	DECLARE_FRAME_WND_CLASS("MDICHILD", IDI_SERVER)
  //DECLARE_FRAME_WND_CLASS(NULL, IDR_MDICHILD)

  BOOL IsActive( void );
  void HideWindow( void );
  void ActivateWindow( void );

#ifdef USE_TABBEDMDI // this was done in fixedmdi

  // override the Create() method to automaximize the window if the pref is set
	HWND Create(HWND hWndParent, ATL::_U_RECT rect = NULL, LPCTSTR szWindowName = NULL,
	DWORD dwStyle = 0, DWORD dwExStyle = 0,
	UINT nMenuID = 0, LPVOID lpCreateParam = NULL)
	{
		BOOL bMaximized = BOOLPREF(PREF_bAutoMaximize); // set the default
    if (bMaximized)
      dwStyle |= WS_MAXIMIZE;

    HWND hWnd = baseClass::Create(hWndParent, rect, szWindowName, dwStyle, dwExStyle, nMenuID, lpCreateParam);
		return hWnd;
  }
#endif
};