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

template< class TTabCtrl >

class CHydraMDITabOwner :
public CMDITabOwner<TTabCtrl>
{
  public:
    // Expose the type of tab control
    typedef TTabCtrl TTabCtrl;

  protected:
    typedef CMDITabOwner<TTabCtrl> baseClass;
    typedef CHydraMDITabOwner<TTabCtrl> thisClass;

    // Message Handling
  public:
    DECLARE_WND_CLASS(_T("HydraMdiTabOwner"))

    BEGIN_MSG_MAP(CMDITabOwner)
      NOTIFY_CODE_HANDLER(NM_CLICK, OnClick)
      CHAIN_MSG_MAP(baseClass)
    END_MSG_MAP()

  LRESULT OnClick(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
  {
    bHandled = TRUE;

    NMCTCITEM* pnmTC = (NMCTCITEM*)pnmh;
    if(pnmTC)
    {
      if(pnmTC->iItem >= 0)
      {
        if(pnmTC->iItem != 0)
        {
          m_TabCtrl.MoveItem(pnmTC->iItem, 0U, false, false);
        }

        m_TabCtrl.SetFocus();
        m_TabCtrl.SetCurSel(0U);
      }
    }

    // Return TRUE so that default handling doesn't occur
    // (we've already taken care of it)
    return TRUE;
  }
};

