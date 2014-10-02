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

// Identd.cpp : implementation of the Identd class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

static TIMEVAL g_IdentTimeout = { 8, 0 }; // TODO: move this..

Identd::Identd( HWND hWnd )
{
  m_pSocket = NULL;
  m_hWnd = hWnd;
  Update();
}

Identd::~Identd( void )
{
  Disable();
}

void Identd::Enable( void )
{
  if (m_pSocket)
    return; // already enabled

  m_pSocket = new Socket(m_hWnd,WM_IDENTDEVENT);
  if (m_pSocket->Listen(113) == SOCK_ERR_NONE)
  {
    sys_Printf(BIC_INFO,"Identd: (AUTH) initialised on port 113\n");
  }
  else
  {
    sys_Printf(BIC_WARNING,"Identd: failed to initialise on port 113\n");
    delete m_pSocket;
    m_pSocket = NULL;
  }

}

void Identd::Disable( void )
{
  if (!m_pSocket)
    return;

  sys_Printf(BIC_INFO,"Identd: (AUTH) disabled\n");
  delete m_pSocket;
  m_pSocket = NULL;
}

// called when identd prefs are changed.

void Identd::Update( void )
{
  if (BOOLPREF(PREF_bIdentdEnabled))
    Enable();
  else
    Disable();
}

LRESULT Identd::OnIdentdEvent(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  int Event = WSAGETSELECTEVENT(lParam);
  int Error = WSAGETSELECTERROR(lParam);

  // TODO: use class Socket, not SOCKET

  SOCKET socket;
	SOCKADDR_IN acc_sin;	/* Accept socket address - internet style */
	int acc_sin_len;		  /* Accept socket address length */
	char buffer[1024];
	int local_port, remote_port;
  char *AddressStr = NULL;

  switch(Event)
  {
    case FD_ACCEPT:
			acc_sin_len = sizeof( acc_sin );
			socket = accept( m_pSocket->m_sock,(struct sockaddr FAR *) &acc_sin, (int FAR *) &acc_sin_len );

      AddressStr = GetIPv4AddressString(acc_sin.sin_addr.s_addr);
      if (!AddressStr) 
        break;

			if (socket == INVALID_SOCKET )
      {
        sys_Printf(BIC_WARNING,"Identd: Failed to accept connection from %s (%d)", AddressStr, WSAGetLastError());
				break;
			}

			if(GetLine(socket, buffer, sizeof(buffer)))
			{
        sys_Printf(BIC_WARNING,"Identd: read error from %s (%d) Buffer: \"%s\"\n", AddressStr, WSAGetLastError(),buffer);
        closesocket(socket);
				break;
			}

			if(sscanf(buffer, " %d , %d", &local_port, &remote_port) == 2) 
      {
				if(local_port < 1 || local_port > 65535 || remote_port < 1 || remote_port > 65535) 
				{
          sys_Printf(BIC_WARNING,"Identd: got bad port numbers from %s! Buffer: \"%s\"\n",AddressStr, buffer);
					sprintf(buffer, "%d , %d : ERROR : INVALID-PORT\r\n", local_port, remote_port);
				}
				else 
				{
          struct sockaddr_in *serveraddress;
          IRCServer *pServer;

          char *osinfo = STRINGPREF(PREF_sIdentdOS);
          char *username = STRINGPREF(PREF_sIdentdUserName);

          // see if we're connected to a server that has the same address as the
          // server sending this ident request

          for (int i = 0 ; i < g_ServerList.GetSize(); i++)
          {
            pServer = g_ServerList[i];

            if (pServer->GetStatus() == SVR_STATE_DISCONNECTED)
              continue;

            serveraddress = pServer->m_pSocket->GetAddress();
            if (serveraddress->sin_addr.s_addr == acc_sin.sin_addr.s_addr)
            {
              // reply with different identities depending on the User identity being used
              // by the server

              sys_Printf(BIC_INFO,"Identd: matched an incoming request to server: %s (%s)\n",pServer->m_pDetails->m_Name, AddressStr);
              // update the string pointers to use the ones from the server's associated user identity instead.
              if (pServer->m_pIdentity->m_IdentdUserName)
                username = pServer->m_pIdentity->m_IdentdUserName;
              else
                sys_Printf(BIC_WARNING,"Identd: No Identd Username configured in profile \"%s\"\n",pServer->m_pIdentity->m_Description);

              if (pServer->m_pIdentity->m_IdentdOS)
                osinfo = pServer->m_pIdentity->m_IdentdOS;
              else
                sys_Printf(BIC_WARNING,"Identd: No Identd OS configured in profile \"%s\"\n",pServer->m_pIdentity->m_Description);

              pServer->Printf(BIC_CONNECTING,"*** Identd: responding to %s with  \"%s\" - \"%s\"",AddressStr, username,osinfo);

              break; // don't try and match any more
            }
          }

          if (osinfo && username) // just to be on the safe side...
          {
            sys_Printf(BIC_INFO,"Identd: sending response to %s for request: \"%s\"\n",AddressStr, buffer);
            sprintf(buffer, "%d , %d : USERID : %s : %s\r\n", local_port, remote_port, osinfo, username);
          }
        }

        if (!send(socket, buffer, strlen(buffer), 0 ))
        {
          sys_Printf(BIC_WARNING,"Identd: failed response to %s %d\n", AddressStr, WSAGetLastError());
        }
      }
      else
      {
        sys_Printf(BIC_WARNING,"Identd: recieved malformed ident request from %s: \"%s\"\n", AddressStr, buffer);
      }
      closesocket(socket);
      break;

    case FD_CLOSE:
			socket = wParam;
			closesocket(socket);
      break;
  }
  SetString(AddressStr,NULL);
  bHandled = TRUE;
  return 0; // 1 ?

}

// TODO: Replace with my own code.
int Identd::GetLine(SOCKET s, char *buffer, int bufsize)
{
  char *pBuffer;
  int sizeleft;
  int retval;
  int iter;
  TIMEVAL to;
  fd_set fdset;

  *buffer = 0; // null terminate the buffer, in case of error
  pBuffer = buffer;
  sizeleft = bufsize;

  while(sizeleft) // until buffer full
  { 

	  // Check for timeout
	  to.tv_sec = g_IdentTimeout.tv_sec;
	  to.tv_usec = g_IdentTimeout.tv_usec;
	  FD_ZERO(&fdset);
	  FD_SET(s, &fdset);

	  retval = select(0, &fdset, 0, 0, &to);
	  if(retval != 1)
	    return(2); // timed out

	  retval = recv(s, pBuffer, sizeleft-1, 0); 
    // Get the next characters leaving space for the null terminator

    if(!retval || retval == SOCKET_ERROR) // connection closed or broken
      return(1);

    for(iter = 0; iter < retval; pBuffer++, sizeleft--, iter++)
    {
      if(*pBuffer == 10 || *pBuffer == 13) // EOL
      {
        *pBuffer = 13;
        *(pBuffer+1) = 0;
        return(0);
      }
    }
  }
  return(2); /* error - buffer full */
}

