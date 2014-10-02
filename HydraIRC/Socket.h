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
