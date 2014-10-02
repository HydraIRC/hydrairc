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

// Prefs_IdentitiesPage.h : interface of the CMiscPage class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

#define PII_BOOL    1
#define PII_INTEGER 2
#define PII_STRING  3
#define PII_PATH    4
#define PII_FONT    5
#define PII_COLOR   6

void CMiscPage::OnPageDisplay( void )
{
  HPROPERTY hItem;
  m_PropertyList.AddItem( PropCreateCategory("Strings") );
  for (int i = 0; i < PREF_STRING_MAX; i++)
  {
    hItem = m_PropertyList.AddItem( PropCreateSimple(PREF_STRING_NAME(i) , g_pNewPrefs->m_StringPrefs[i] ));
    if (hItem)
      m_PropertyList.SetItemData(hItem, MAKELPARAM(i,PII_STRING) );
  }
  m_PropertyList.AddItem( PropCreateCategory("Paths") );
  for (int i = 0; i < PREF_PATH_MAX; i++)
  {
    hItem = m_PropertyList.AddItem( PropCreateFileName(PREF_PATH_NAME(i) , g_pNewPrefs->m_PathPrefs[i] ));
    if (hItem)
      m_PropertyList.SetItemData(hItem, MAKELPARAM(i,PII_PATH) );
  }
  m_PropertyList.AddItem( PropCreateCategory("Numbers") );
  for (int i = 0; i < PREF_INT_MAX; i++)
  {
    hItem = m_PropertyList.AddItem( PropCreateSimple(PREF_INT_NAME(i) , (LONG)g_pNewPrefs->m_IntPrefs[i] ));
    if (hItem)
      m_PropertyList.SetItemData(hItem, MAKELPARAM(i,PII_INTEGER) );
  }
  m_PropertyList.AddItem( PropCreateCategory("Booleans") );
  for (int i = 0; i < PREF_BOOL_MAX; i++)
  {
    hItem = m_PropertyList.AddItem( PropCreateSimple(PREF_BOOL_NAME(i) , g_pNewPrefs->m_BoolPrefs[i] ? (bool)true : (bool)false )); // note the case of "bool"
    if (hItem)
      m_PropertyList.SetItemData(hItem, MAKELPARAM(i,PII_BOOL) );
  }
  m_PropertyList.AddItem( PropCreateCategory("Fonts") );
  for (int i = 0; i < PREF_FONT_MAX; i++)
  {
    char *FontInfo = HydraIRC_BuildString(128,"%s,%d,%s%s%s",
      g_pNewPrefs->m_FontPrefs[i].m_Name,
      g_pNewPrefs->m_FontPrefs[i].m_Size,
      g_pNewPrefs->m_FontPrefs[i].m_Flags == FIF_NONE ? "N" : 
        (g_pNewPrefs->m_FontPrefs[i].m_Flags & FIF_BOLD ? "B" : ""),
      g_pNewPrefs->m_FontPrefs[i].m_Flags & FIF_ITALIC ? "I" : "",
      g_pNewPrefs->m_FontPrefs[i].m_Flags & FIF_UNDERLINE ? "U" : "");

    if (FontInfo)
    {
      hItem = m_PropertyList.AddItem( PropCreateSimple(PREF_FONT_NAME(i), FontInfo ));
      if (hItem)
        m_PropertyList.SetItemData(hItem, MAKELPARAM(i,PII_FONT) );
      free(FontInfo);
    }
  }
  m_PropertyList.AddItem( PropCreateCategory("Colors") );
  for (int i = 0; i < PREF_COLOR_MAX; i++)
  {
    char *ColorInfo;

    if (g_pNewPrefs->m_ColorPrefs[i] != -1)
      ColorInfo = HydraIRC_BuildString(8,"#%06x",RGBVALTOCOLORREF(g_pNewPrefs->m_ColorPrefs[i]));
    else
      ColorInfo = strdup("System");

    if (ColorInfo)
    {
      hItem = m_PropertyList.AddItem( PropCreateSimple(PREF_COLOR_NAME(i) , ColorInfo ));
      if (hItem)
        m_PropertyList.SetItemData(hItem, MAKELPARAM(i,PII_COLOR) );
      free(ColorInfo);
    }
  }


  this->UpdateWindow();
}

void CMiscPage::OnPageDone( void )
{
  m_PropertyList.ResetContent();
}

BOOL CMiscPage::OnPageValidate( void )
{
  BOOL OK = TRUE;
  for (int i = 0; i < PREF_STRING_MAX; i++)
  {
    HPROPERTY hFind = m_PropertyList.FindProperty(PREF_STRING_NAME(i));
    if (hFind)
    {
      CComVariant value;  
      BOOL Result = m_PropertyList.GetItemValue(hFind,&value);
      LPARAM lParam = m_PropertyList.GetItemData(hFind);
      if (HIWORD(lParam) == PII_STRING)
      {
        int ItemIndex = LOWORD(lParam);

        if (g_pNewPrefs->m_StringPrefs[ItemIndex]) free(g_pNewPrefs->m_StringPrefs[ItemIndex]);
        g_pNewPrefs->m_StringPrefs[ItemIndex] = strdup(CW2A(value.bstrVal));
      }
      else OK = FALSE;
    }
    else OK = FALSE;
  }

  for (int i = 0; i < PREF_PATH_MAX; i++)
  {
    HPROPERTY hFind = m_PropertyList.FindProperty(PREF_PATH_NAME(i));
    if (hFind)
    {
      CComVariant value;  
      BOOL Result = m_PropertyList.GetItemValue(hFind,&value);
      LPARAM lParam = m_PropertyList.GetItemData(hFind);
      if (HIWORD(lParam) == PII_PATH)
      {
        int ItemIndex = LOWORD(lParam);

        if (g_pNewPrefs->m_PathPrefs[ItemIndex]) free(g_pNewPrefs->m_PathPrefs[ItemIndex]);
        g_pNewPrefs->m_PathPrefs[ItemIndex] = strdup(CW2A(value.bstrVal));
        strippathseparator(g_pNewPrefs->m_PathPrefs[ItemIndex]);
      }
      else OK = FALSE;
    }
    else OK = FALSE;
  }

  for (int i = 0; i < PREF_INT_MAX; i++)
  {
    HPROPERTY hFind = m_PropertyList.FindProperty(PREF_INT_NAME(i));
    if (hFind)
    {
      CComVariant value;  
      BOOL Result = m_PropertyList.GetItemValue(hFind,&value);
      LPARAM lParam = m_PropertyList.GetItemData(hFind);
      if (HIWORD(lParam) == PII_INTEGER)
      {
        int ItemIndex = LOWORD(lParam);
        
        g_pNewPrefs->m_IntPrefs[ItemIndex] = value.intVal;
      }
      else OK = FALSE;
    }
    else OK = FALSE;
  }

  for (int i = 0; i < PREF_BOOL_MAX; i++)
  {
    HPROPERTY hFind = m_PropertyList.FindProperty(PREF_BOOL_NAME(i));
    if (hFind)
    {
      CComVariant value;  
      BOOL Result = m_PropertyList.GetItemValue(hFind,&value);
      LPARAM lParam = m_PropertyList.GetItemData(hFind);
      if (HIWORD(lParam) == PII_BOOL)
      {
        int ItemIndex = LOWORD(lParam);
        
        g_pNewPrefs->m_BoolPrefs[ItemIndex] = value.boolVal;
      }
      else OK = FALSE;
    }
    else OK = FALSE;
  }

  for (int i = 0; i < PREF_FONT_MAX; i++)
  {
    HPROPERTY hFind = m_PropertyList.FindProperty(PREF_FONT_NAME(i));
    if (hFind)
    {
      CComVariant value;  
      BOOL Result = m_PropertyList.GetItemValue(hFind,&value);
      LPARAM lParam = m_PropertyList.GetItemData(hFind);
      if (HIWORD(lParam) == PII_FONT)
      {
        int ItemIndex = LOWORD(lParam);

        g_pNewPrefs->UpdateFontPrefItem(&g_pNewPrefs->m_FontPrefs[ItemIndex],CW2A(value.bstrVal));
      }
      else OK = FALSE;
    }
    else OK = FALSE;
  }

  for (int i = 0; i < PREF_COLOR_MAX; i++)
  {
    HPROPERTY hFind = m_PropertyList.FindProperty(PREF_COLOR_NAME(i));
    if (hFind)
    {
      CComVariant value;  
      BOOL Result = m_PropertyList.GetItemValue(hFind,&value);
      LPARAM lParam = m_PropertyList.GetItemData(hFind);
      if (HIWORD(lParam) == PII_COLOR)
      {
        int ItemIndex = LOWORD(lParam);

        if (stricmp(CW2A(value.bstrVal),"System") == 0)
          g_pNewPrefs->m_ColorPrefs[ItemIndex] = -1;
        else
		{
          g_pNewPrefs->SetIntPref((int&)g_pNewPrefs->m_ColorPrefs[ItemIndex],CW2A(value.bstrVal));
		  g_pNewPrefs->m_ColorPrefs[ItemIndex] = RGBVALTOCOLORREF(g_pNewPrefs->m_ColorPrefs[ItemIndex]);
		}
			
      }
      else OK = FALSE;
    }
    else OK = FALSE;
  }

  return OK;
}

LRESULT CMiscPage::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  // the real init is done when the page is OnPageDisplay'd, as other preference items might
  // make a difference to the data
  DlgResize_Init(false,true,0);
  m_PropertyList.SubclassWindow(GetDlgItem(IDC_MISC_PROPERTYLIST));
  m_PropertyList.SetExtendedListStyle(PLS_EX_CATEGORIZED);
  return 0;
}

LRESULT CMiscPage::OnPinBrowse(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
{

  LPNMPROPERTYITEM nmp = (LPNMPROPERTYITEM)pnmh;

  USES_CONVERSION;
/*
  CComVariant value;  
  BOOL Result = m_PropertyList.GetItemValue(nmp->prop,&value);
  CFileDialog *pFileDialog = new CFileDialog(FALSE,NULL,CW2A(value.bstrVal));
  int result = pFileDialog->DoModal();
  if (result > 0)
  {
    CComVariant v(pFileDialog->m_szFileName);
    m_PropertyList.SetItemValue(nmp->prop, &v);
  }
  delete pFileDialog;
*/
  
  CComVariant value;  
  BOOL Result = m_PropertyList.GetItemValue(nmp->prop,&value);
  LPARAM lParam = m_PropertyList.GetItemData(nmp->prop);
  if (HIWORD(lParam) != PII_PATH)
    return 0;
  int ItemIndex = LOWORD(lParam);

  CFolderDialog *pFolderDialog = new CFolderDialog();

  // TODO: annoyingly, this has no effect
  //strcpy(pFolderDialog->m_szFolderPath,CW2A(value.bstrVal));
  strcpy(pFolderDialog->m_szFolderDisplayName,CW2A(value.bstrVal));
  int result = pFolderDialog->DoModal();
  if (result > 0)
  {
    CComVariant v(pFolderDialog->m_szFolderPath);
    m_PropertyList.SetItemValue(nmp->prop, &v);    
  }
  delete pFolderDialog;

  return 0;
}
