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
