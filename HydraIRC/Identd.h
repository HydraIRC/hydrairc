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
