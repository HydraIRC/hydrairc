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

// GUIHelpers.cpp : misc functions for using/populating/managing GUI controls
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

// populate an identity control with the list of identities, select the identity
// passed if set, otherwise select the default identity (if set)
void PopulateIdentityComboCtrl(CComboBox &IdentityCtrl, UserIdentity *pSelectIdentity /* = NULL */)
{
  int itemnum;
  
  char *selectname = NULL;

  UserIdentity *pIdentity;

  UserIdentity *pDefaultIdentity = g_pPrefs->FindIdentityByDescripton(STRINGPREF(PREF_sDefaultIdentity));
  if (pDefaultIdentity)
  {
    itemnum = IdentityCtrl.AddString(pDefaultIdentity->m_Description);
    IdentityCtrl.SetItemDataPtr(itemnum,pDefaultIdentity);
    selectname = pDefaultIdentity->m_Description; // select this one, unless we're supposed to select something else...
  }

  for (int i = 0 ; i < g_pPrefs->m_UserIdentitiesList.GetSize() ; i++ )
  {
    pIdentity = g_pPrefs->m_UserIdentitiesList[i];

    // check we've not already added this one.
    if (pIdentity != pDefaultIdentity)
    {
      itemnum = IdentityCtrl.AddString(pIdentity->m_Description);
      IdentityCtrl.SetItemDataPtr(itemnum,pIdentity);
      // select this one?
      if (pSelectIdentity && pSelectIdentity->m_Description && stricmp(pIdentity->m_Description,pSelectIdentity->m_Description) == 0)
        selectname = pIdentity->m_Description;
    }
  }

  if (selectname) // this should always be set, as we can't get to the connect dialog without an identity...
    IdentityCtrl.SelectString(0,selectname);
  else
    IdentityCtrl.SetCurSel(0);
}
