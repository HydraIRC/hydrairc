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

#pragma once

template<class T, class TBase = CEdit, class TWinTraits = CControlWinTraits>
class ATL_NO_VTABLE CUsefulEditImpl	: public CWindowImpl<T, TBase, TWinTraits>
{
public:
  BOOL m_IgnoreInput;

  DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())
	BEGIN_MSG_MAP(CUsefulEdit)
    MESSAGE_HANDLER(WM_CHAR, OnChar)
  END_MSG_MAP()


  LRESULT OnChar(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
  {
    //bHandled is TRUE to start with..
    if (!m_IgnoreInput)
      bHandled = FALSE;
    return FALSE;
  } 

	CUsefulEditImpl()
  {
    m_IgnoreInput = FALSE;
  }

  void IgnoreInput(BOOL Ignore = TRUE)
  {
    m_IgnoreInput = Ignore;
  }

};

class CUsefulEdit : public CUsefulEditImpl<CUsefulEdit>
{
public:
	DECLARE_WND_CLASS(_T("WTL_UsefulEdit"));
};
