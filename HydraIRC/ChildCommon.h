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