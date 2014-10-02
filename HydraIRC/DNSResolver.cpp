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
