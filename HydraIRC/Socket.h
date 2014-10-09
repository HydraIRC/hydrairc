/*

  HydraIRC
  Copyright (C) 2002-2014 Dominic Clifton aka Hydra

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

// Socket.h : 
//
/////////////////////////////////////////////////////////////////////////////

#define SOCK_ERR_NONE           0
#define SOCK_ERR_RESOLVE        1
#define SOCK_ERR_INVALID_SOCKET 2
#define SOCK_ERR_BIND_FAILED    3
#define SOCK_ERR_LISTEN_FAILED  4

class Socket
{
private:
  struct hostent *m_hp;
	struct sockaddr_in m_sin;
  HWND m_hWnd; // for event notification
  char m_Buf[8192];
  int m_MessageID;

public:
  SOCKET m_sock;

  Socket( HWND hWnd, int MessageID ) 
  { 
    m_hWnd = hWnd;
    m_MessageID = MessageID;
    m_sock = INVALID_SOCKET;
  }

  ~Socket( void ) 
  {
    CloseSocket();
  }

  SOCKET GetSocket( void )
  {
    return m_sock;
  }


  void CloseSocket( void )
  {
    if (m_sock != SOCKET_ERROR && m_sock != INVALID_SOCKET)
      closesocket(m_sock);
    m_sock = INVALID_SOCKET;
  }

  int Connect( char *Host, unsigned short Port);
  int Connect( DWORD Address, unsigned short Port);
  int Listen(unsigned short Port);
  int Accept(Socket *pOther, long Events);
  int Send( const char *format, ...);
  int SendRaw( const char *buffer, int bufferlen, int flags );
  int SendBuffer( const char *buffer ); // send null terminated buffer
  int Receive(LPSTR buffer, int len, int flags);
  unsigned long GetWaiting( void );
  char *GetWSASelectErrorString( int Error );
  struct sockaddr_in *GetAddress( void ) { return &m_sin; };

private:
  int ActualConnect( void );

};
