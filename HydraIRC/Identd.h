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

// Identd.h : declaration of the Identd class
//
/////////////////////////////////////////////////////////////////////////////

class Identd
{
protected:
  //WSADATA wsa;
  //BOOL m_WSAInitOK;
private:
  Socket *m_pSocket;
  char *m_OSInfo;
  char *m_UserName;
  HWND m_hWnd;
public:  

  Identd(HWND hWnd);
  ~Identd(void);

  void Update( void ); // call when identd related prefs hae changed

  void Enable( void );
  void Disable( void );

  LRESULT OnIdentdEvent(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  int GetLine(SOCKET s, char *buffer, int bufsize);
};
