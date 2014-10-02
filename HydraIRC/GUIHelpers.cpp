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
  3.2) Will become the property of HydraIRC's author.

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
