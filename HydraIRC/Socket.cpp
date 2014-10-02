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

// Socket.cpp : 
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

// Socket errors and descriptions are here:
// ms-help://MS.MSDNVS/winsock/errors_3wc2.htm#_win32_wsaeconnreset_2
char *Socket::GetWSASelectErrorString( int Error )
{
  switch(Error)
  {
    // FD_CONNECT
    case WSAEAFNOSUPPORT: return "Addresses in the specified family cannot be used with this socket";
    case WSAECONNREFUSED: return "The attempt to connect was forcefully rejected";
    case WSAENETUNREACH:  return "The network cannot be reached from this host at this time.";
    case WSAEFAULT:       return "The namelen parameter is incorrect";
    case WSAEINVAL:       return "The socket is already bound to an address";
    case WSAEISCONN:      return "The socket is already connected";
    case WSAEMFILE:       return "No more file descriptors are available";
    case WSAENOBUFS:      return "No buffer space is available. The socket cannot be connected";
    case WSAENOTCONN:     return "The socket is not connected";
    case WSAETIMEDOUT:    return "Attempt to connect timed out without establishing a connection";
    // FD_CLOSE
    case WSAENETDOWN:     return "The network subsystem has failed";
    case WSAECONNRESET:   return "The connection was reset by the remote side";
    case WSAECONNABORTED: return "The connection was terminated due to a time-out or other failure";
    // Others
    case WSAENOTSOCK:     return "Socket already closed";
    case WSAEHOSTUNREACH: return "No route to host";

  }
  return "Unknown Error";
}

int Socket::ActualConnect( void ) // TODO: add "events" param
{
  m_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (m_sock == INVALID_SOCKET) 
  {
    return SOCK_ERR_INVALID_SOCKET;
  }

  WSAAsyncSelect(m_sock, m_hWnd, m_MessageID, FD_CLOSE | FD_READ | FD_CONNECT | FD_ACCEPT);

  connect(m_sock, (struct sockaddr *)&m_sin, sizeof(m_sin));
  int result = WSAGetLastError(); // clear the WSAWOULDBLOCK error

  return SOCK_ERR_NONE;
}

int Socket::Connect( char *Host, unsigned short Port) // TODO: add "events" param
{
	ZeroMemory(&m_sin, sizeof(m_sin));
	m_sin.sin_family = AF_INET;
	m_sin.sin_port = htons(Port);
	m_sin.sin_addr.s_addr = inet_addr(Host);
	if (m_sin.sin_addr.s_addr == INADDR_NONE)
	{
		m_hp = gethostbyname(Host);
		if (m_hp == NULL)
    {
      return SOCK_ERR_RESOLVE;
    }
		memcpy(&m_sin.sin_addr, m_hp->h_addr_list[0], sizeof(DWORD));    
	}
  return ActualConnect();
}

int Socket::Connect( DWORD Address, unsigned short Port) // TODO: add "events" param
{
  ZeroMemory(&m_sin, sizeof(m_sin));
  m_sin.sin_family = AF_INET;
  m_sin.sin_port = htons(Port);
  //memcpy(&m_sin.sin_addr, Address, sizeof(DWORD));    
  m_sin.sin_addr.s_addr = Address;
  return ActualConnect();
}

int Socket::Listen(unsigned short Port)
{
	ZeroMemory(&m_sin, sizeof(m_sin));
	m_sin.sin_family = AF_INET;
  m_sin.sin_port = htons(Port);
	m_sin.sin_addr.s_addr = INADDR_ANY;

  m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == INVALID_SOCKET) 
  {
    return SOCK_ERR_INVALID_SOCKET;
  }
	
  if (bind(m_sock, (struct sockaddr *)&m_sin, sizeof(m_sin)) == SOCKET_ERROR) 
  {
    sys_Printf(BIC_SOCKET,"Failed to bind (%d)\n", WSAGetLastError());
    return SOCK_ERR_BIND_FAILED;
  }

	if (listen(m_sock, SOMAXCONN ) < 0)
  {
    sys_Printf(BIC_SOCKET,"Failed to listen (%d)\n", WSAGetLastError());
    return SOCK_ERR_LISTEN_FAILED;
  }

  WSAAsyncSelect(m_sock, m_hWnd, m_MessageID, FD_ACCEPT | FD_CLOSE);

  return SOCK_ERR_NONE;
}

int Socket::SendBuffer( const char *buffer )
{
  if (m_sock == INVALID_SOCKET) 
    return SOCKET_ERROR;

  sys_Printf(BIC_SOCKET,"Sending \"%s\"\n",buffer);
  return send(m_sock, buffer, lstrlen(buffer), 0);
}

int Socket::SendRaw( const char *buffer, int bufferlen, int flags )
{
  if (m_sock == INVALID_SOCKET) 
    return SOCKET_ERROR;

  //sys_Printf("SENDING RAW: \"%d\" bytes\n",bufferlen);
  return send(m_sock, buffer, bufferlen, flags);
}


int Socket::Send( const char *format, ...)
{
  if (m_sock == INVALID_SOCKET) 
    return SOCKET_ERROR;

  if (format)
	{
		va_list args;
		
		//ZeroMemory(&m_Buf, sizeof(m_Buf));
		va_start(args, format);
		_vsnprintf(m_Buf, sizeof(m_Buf), format, args);
		va_end(args);
    m_Buf[sizeof(m_Buf) - 1] = 0;

		return SendBuffer(m_Buf);
	}
	return 0;
}

int Socket::Receive(LPSTR buffer, int len, int flags)
{
  return recv(m_sock, buffer, len, flags);
}

unsigned long Socket::GetWaiting( void )
{
  unsigned long bytes = -1;
  ioctlsocket(m_sock, FIONREAD, &bytes);
  return bytes;
}

BOOL Socket::Accept(Socket *pOther, long Events)
{
	int acc_sin_len = sizeof( m_sin );
	m_sock = accept( pOther->m_sock,(struct sockaddr FAR *) &m_sin, (int FAR *) &acc_sin_len );

  if (m_sock == INVALID_SOCKET)
    return FALSE;

  WSAAsyncSelect(m_sock, m_hWnd, m_MessageID, Events);
    return TRUE;
}
