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

// CDNSResolver.cpp : implements a threaded DNS Resolver
//
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "HydraIRC.h"

CDNSResolver::CDNSResolver(void)
{
  m_Thread1.Start(this, THID_DNSRESOLVER);
}

CDNSResolver::~CDNSResolver(void)
{
  m_Thread1.Stop();
}

LRESULT CDNSResolver::OnDNSEvent(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
  bHandled = TRUE;

  DNSResolveItem *pDNSRI = (DNSResolveItem *)lParam;
  ATLASSERT(pDNSRI);

  // resolve the address! :)

  struct hostent *hp;
  hp = gethostbyname(pDNSRI->m_fqdn);

  // Signal the calling window
  if (hp != NULL)
  {
    // Success!
    memcpy(&pDNSRI->m_address, hp->h_addr_list[0], sizeof(DWORD));
  }

  // ok, some time might have passed, and the user may have closed the window
  // so check to see if the window still exists before attempting to connect.

  if (IsWindow(pDNSRI->m_hWnd))
  {
    ::PostMessage(pDNSRI->m_hWnd,WM_DNSEVENT,(hp != NULL),(LPARAM) pDNSRI);
  }
  else
  {
    // window was deleted, so we can free this...
    delete pDNSRI;
  }

  return FALSE;
}

void CDNSResolver::StartResolve(char *fqdn, HWND hWnd)
{
  // Note: pDNSRI must be freed by the calling thread when it gets this
  // back from WM_DNSEVENT message's LPARAM

  DNSResolveItem *pDNSRI = new DNSResolveItem(fqdn, hWnd);

  // Post a message to the DNSResolver thread's message queue.
  // this will then (eventually) trigger a call to OnDNSEvent which does
  // the work.
  BOOL Success = m_Thread1.PostMessage(WM_DNSEVENT,NULL,(LPARAM) pDNSRI);
  ATLASSERT(Success);
}
