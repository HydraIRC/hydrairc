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

// CDNSResolver.h : implements a non-blocking DNS Resolver
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class DNSResolveItem
{
public:
  char *m_fqdn;
  HWND m_hWnd; // where to send the information back to.
  DWORD m_address;

  DNSResolveItem(char *fqdn, HWND hWnd)
  {
    m_fqdn = strdup(fqdn);
    m_hWnd = hWnd;
    m_address = 0;
  }

  ~DNSResolveItem()
  {
    if (m_fqdn) free(m_fqdn);
  }
};

class CDNSResolver : public CMessageMap//, Thread
{
public:
  CDNSResolver(void);
  ~CDNSResolver(void);

  // Call this to kick of a DNS Resolution attempt.
  // the window passed here as a parameter will receive a WM_DNSEVENT message
  // with an lParam pointing to a DNSResolveItem which must then be delete()'d
  void StartResolve(char *fqdn, HWND hWnd);

	BEGIN_MSG_MAP(CDNSResolver)
  THREAD_MSG_MAP(THID_DNSRESOLVER)
    MESSAGE_HANDLER(WM_DNSEVENT, OnDNSEvent)
  END_MSG_MAP()

  CAsyncMessagePump m_Thread1;

	LRESULT OnDNSEvent(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
};
