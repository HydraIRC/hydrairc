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

// EditNotificationDlg.cpp : implementation of the CEditNotificationDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"
#include ".\editnotificationdlg.h"


BOOL CEditNotificationDlg::Validate( void )
{
  BOOL Result;
  int itemnum;

  if (m_Updating)
    return FALSE;

  if (!m_Initialised)
    Result = FALSE;
  else
  {
    m_pNPI->m_Flags = 0;
    if (m_FlashTaskBarCtrl.GetCheck()) m_pNPI->m_Flags |= NE_FLAG_FLASHTASKBAR;
    if (m_LogInEventLogCtrl.GetCheck()) m_pNPI->m_Flags |= NE_FLAG_LOGINEVENTLOG;
    if (!m_EnabledCtrl.GetCheck()) m_pNPI->m_Flags |= NE_FLAG_DISABLED;
    if (m_HasSoundCtrl.GetCheck())
    {
      if (m_SystemSoundCtrl.GetCheck()) m_pNPI->m_Flags |= NE_FLAG_SYSTEMSOUND;
      if (m_SoundFileCtrl.GetCheck()) m_pNPI->m_Flags |= NE_FLAG_SOUNDFILE;
    }

    CopyWindowText(&m_FileNameCtrl,m_pNPI->m_FileName);
    CopyWindowText(&m_MatchStringCtrl,m_pNPI->m_MatchString);

    m_pNPI->m_HighlightFlags = HIGHLIGHT_NONE;
    if (m_HighlightWholeLineCtrl.GetCheck()) m_pNPI->m_HighlightFlags |= HIGHLIGHT_WHOLELINE;
    if (m_HighlightCaseSensitiveCtrl.GetCheck()) m_pNPI->m_HighlightFlags |= HIGHLIGHT_CASESENSITIVE;

    // TODO: replace this section with a decent color picker control
    char *numstr = NULL;
    int rgb;
    BOOL OK;
    rgb = -1;
    CopyWindowText(&m_ForegroundColorCtrl,numstr);
    OK = g_pPrefs->SetIntPref(rgb,numstr);
    if (!OK || rgb < 0 || rgb > 0xffffff)
      rgb = -1;
    m_pNPI->m_ForegroundColor = RGBVALTOCOLORREF(rgb);

    rgb = -1;
    CopyWindowText(&m_BackgroundColorCtrl,numstr);
    OK = g_pPrefs->SetIntPref(rgb,numstr);
    if (!OK || rgb < 0 || rgb > 0xffffff)
      rgb = -1;
    m_pNPI->m_BackgroundColor = RGBVALTOCOLORREF(rgb);

    SetString(numstr,NULL);
    // TODO: end of section to replace

    itemnum = m_SelectSystemSoundCtrl.GetCurSel();
    if (itemnum != CB_ERR)
      m_pNPI->m_SoundID = m_SelectSystemSoundCtrl.GetItemData(itemnum);
    else
      m_pNPI->m_SoundID = -1;

    itemnum = m_EventCtrl.GetCurSel();
    if (itemnum != CB_ERR)
      m_pNPI->m_EventCode = m_EventCtrl.GetItemData(itemnum);
    else
      m_pNPI->m_EventCode = -1;

    Result = TRUE; 
    if ((m_pNPI->m_Flags & NE_FLAG_SOUNDFILE) && (m_pNPI->m_FileName == NULL)) // enabled, needs valid data.
      Result = FALSE;
    if ((m_pNPI->m_Flags & NE_FLAG_SYSTEMSOUND) && (m_pNPI->m_SoundID < 0)) // enabled, needs valid data.
      Result = FALSE;

    if (m_HasSoundCtrl.GetCheck())
    {
      m_FileNameCtrl.EnableWindow((m_pNPI->m_Flags & NE_FLAG_SOUNDFILE) > 0);
      m_SelectSystemSoundCtrl.EnableWindow((m_pNPI->m_Flags & NE_FLAG_SYSTEMSOUND) > 0);
      m_SystemSoundCtrl.EnableWindow(TRUE);
      m_SoundFileCtrl.EnableWindow(TRUE);
      m_BrowseCtrl.EnableWindow((m_pNPI->m_Flags & NE_FLAG_SOUNDFILE) > 0);
    }
    else
    {
      m_FileNameCtrl.EnableWindow(FALSE);
      m_SelectSystemSoundCtrl.EnableWindow(FALSE);
      m_SystemSoundCtrl.EnableWindow(FALSE);
      m_SoundFileCtrl.EnableWindow(FALSE);
      m_BrowseCtrl.EnableWindow(FALSE);
    }

    if (m_pNPI->m_EventCode == NE_HIGHLIGHT)
    {
      m_MatchStringCtrl.EnableWindow(TRUE);
      m_ForegroundColorCtrl.EnableWindow(TRUE);
      m_BackgroundColorCtrl.EnableWindow(TRUE);
      m_PickForegroundColorCtrl.EnableWindow(TRUE);
      m_PickBackgroundColorCtrl.EnableWindow(TRUE);
      m_HighlightWholeLineCtrl.EnableWindow(TRUE);
      m_HighlightCaseSensitiveCtrl.EnableWindow(TRUE);
      //m_LogInEventLogCtrl.EnableWindow(TRUE);
      if (m_pNPI->m_MatchString == NULL ||
          (m_pNPI->m_BackgroundColor == -1 && m_pNPI->m_ForegroundColor == -1))
        Result = FALSE; // need matchstring and at least one color!
    }
    else
    {
      m_MatchStringCtrl.EnableWindow(FALSE);
      m_ForegroundColorCtrl.EnableWindow(FALSE);
      m_BackgroundColorCtrl.EnableWindow(FALSE);
      m_PickForegroundColorCtrl.EnableWindow(FALSE);
      m_PickBackgroundColorCtrl.EnableWindow(FALSE);
      m_HighlightWholeLineCtrl.EnableWindow(FALSE);
      m_HighlightCaseSensitiveCtrl.EnableWindow(FALSE);
      //m_LogInEventLogCtrl.EnableWindow(FALSE);
    }
  }
  m_OKCtrl.EnableWindow(Result);

  return Result;
}

void CEditNotificationDlg::UpdateColors( void )
{
  char numstr[10];

  m_Updating = TRUE;

  if (m_pNPI->m_ForegroundColor == -1)
    strcpy(numstr,"Not Set");
  else
    _snprintf(numstr,9,"#%06x",RGBVALTOCOLORREF(m_pNPI->m_ForegroundColor)); // convert from COLORVAL to RGB
  m_ForegroundColorCtrl.SetWindowText(numstr);

  if (m_pNPI->m_BackgroundColor == -1)
    strcpy(numstr,"Not Set");
  else
    _snprintf(numstr,9,"#%06x",RGBVALTOCOLORREF(m_pNPI->m_BackgroundColor)); // convert from COLORVAL to RGB
  m_BackgroundColorCtrl.SetWindowText(numstr);

  m_Updating = FALSE;
  Validate();
}

LRESULT CEditNotificationDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());
  DlgResize_Init();

  m_FileNameCtrl.Attach(GetDlgItem(IDC_EN_FILENAME));
  m_MatchStringCtrl.Attach(GetDlgItem(IDC_EN_MATCHSTRING));
  m_SelectSystemSoundCtrl.Attach(GetDlgItem(IDC_EN_SELECTSYSTEMSOUND));
  m_EventCtrl.Attach(GetDlgItem(IDC_EN_EVENT));
  m_OKCtrl.Attach(GetDlgItem(ID_EN_OK));
  m_HasSoundCtrl.Attach(GetDlgItem(IDC_EN_HASSOUND));
  m_BrowseCtrl.Attach(GetDlgItem(IDC_EN_BROWSE));
  m_EnabledCtrl.Attach(GetDlgItem(IDC_EN_ENABLED));
  m_FlashTaskBarCtrl.Attach(GetDlgItem(IDC_EN_FLASHTASKBAR));
  m_SystemSoundCtrl.Attach(GetDlgItem(IDC_EN_SYSTEMSOUND));
  m_SoundFileCtrl.Attach(GetDlgItem(IDC_EN_SOUNDFILE));
  m_HighlightWholeLineCtrl.Attach(GetDlgItem(IDC_EN_HIGHLIGHTWHOLELINE));
  m_HighlightCaseSensitiveCtrl.Attach(GetDlgItem(IDC_EN_HIGHLIGHTCASESENSITIVE));
  m_LogInEventLogCtrl.Attach(GetDlgItem(IDC_EN_LOGINEVENTLOG));
  m_ForegroundColorCtrl.Attach(GetDlgItem(IDC_EN_FOREGROUNDCOLOR));
  m_BackgroundColorCtrl.Attach(GetDlgItem(IDC_EN_BACKGROUNDCOLOR));
  m_PickForegroundColorCtrl.Attach(GetDlgItem(IDC_EN_PICKFGCOLOR));
  m_PickBackgroundColorCtrl.Attach(GetDlgItem(IDC_EN_PICKBGCOLOR));

  m_EnabledCtrl.SetCheck((m_pNPI->m_Flags & NE_FLAG_DISABLED) == 0);
  m_FlashTaskBarCtrl.SetCheck((m_pNPI->m_Flags & NE_FLAG_FLASHTASKBAR) > 0);
  m_LogInEventLogCtrl.SetCheck((m_pNPI->m_Flags & NE_FLAG_LOGINEVENTLOG) > 0);
  m_SystemSoundCtrl.SetCheck((m_pNPI->m_Flags & NE_FLAG_SYSTEMSOUND) > 0);
  m_SoundFileCtrl.SetCheck((m_pNPI->m_Flags & NE_FLAG_SOUNDFILE) > 0);
  m_HasSoundCtrl.SetCheck((m_pNPI->m_Flags & (NE_FLAG_SYSTEMSOUND | NE_FLAG_SOUNDFILE)) > 0);

  m_HighlightWholeLineCtrl.SetCheck((m_pNPI->m_HighlightFlags & HIGHLIGHT_WHOLELINE) > 0);
  m_HighlightCaseSensitiveCtrl.SetCheck((m_pNPI->m_HighlightFlags & HIGHLIGHT_CASESENSITIVE) > 0);
  
  if (m_pNPI->m_FileName) m_FileNameCtrl.SetWindowText(m_pNPI->m_FileName);
  if (m_pNPI->m_MatchString) m_MatchStringCtrl.SetWindowText(m_pNPI->m_MatchString);

  UpdateColors();

  int itemnum,i,firstnum;

  if (m_pNPI->m_GroupID == -2)
    firstnum = 0;
  else
    firstnum = NE_GROUPSTART; 

  for (i = firstnum ; i < NE_LAST ; i ++)
  {
    // REMOVEME: don't add highlight to normaly buddy groups (yet)

    //if (m_pNPI->m_GroupID == -2 || (m_pNPI->m_GroupID == -2 && i != NE_HIGHLIGHT))
    //{
      itemnum = m_EventCtrl.AddString(g_NotificationEventNames[i]);
      if (itemnum != CB_ERR)
        m_EventCtrl.SetItemData(itemnum,i);
    //}
  }

  // new event or existing event ?
  // if it's an existing event then we don't let the user change the event.
  if (m_pNPI->m_EventCode == -1)
  {
    // select the first one, enable the control
    m_EventCtrl.SetCurSel(0);
    m_EventCtrl.EnableWindow(TRUE);
  }
  else
  {
    // select the correct one, and disable the control
    i = 0;
    while (i < m_EventCtrl.GetCount() && 
            m_EventCtrl.GetItemData(i) != m_pNPI->m_EventCode)
      i++;

    if (i < m_EventCtrl.GetCount())
      m_EventCtrl.SetCurSel(i);

    m_EventCtrl.EnableWindow(FALSE);
  }

  for (i = 0 ; i < SID_LAST ; i ++)
  {
    itemnum = m_SelectSystemSoundCtrl.AddString(g_SoundIDDisplayNames[i]);
    if (itemnum != CB_ERR)
      m_SelectSystemSoundCtrl.SetItemData(itemnum,i);
  }

  // set the current selection, bearing in mind the combo box
  // data items are sorted alphabetically.
  if ((m_pNPI->m_Flags & NE_FLAG_SYSTEMSOUND) > 0)
  {
    i = 0;
    while (i < m_SelectSystemSoundCtrl.GetCount() && 
           m_SelectSystemSoundCtrl.GetItemData(i) != m_pNPI->m_SoundID)
      i++;

    if (i < m_SelectSystemSoundCtrl.GetCount())
      m_SelectSystemSoundCtrl.SetCurSel(i);
    
  }

  m_Initialised = TRUE;
  Validate();
	return TRUE;
}

LRESULT CEditNotificationDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CEditNotificationDlg::OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  if (Validate())
  {
    EndDialog(1);    
  }
	return 0;
}

LRESULT CEditNotificationDlg::OnSomethingChanged(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  // Isn't this the name of a song by Pulp ?  :)
  Validate();
  return 0;
}


LRESULT CEditNotificationDlg::OnBnClickedEnBrowse(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  if (m_pFileDialog->DoModal() == 1)
  {
    m_FileNameCtrl.SetWindowText(m_pFileDialog->m_szFileName);
    g_pNotificationManager->PlaySoundFile(m_pFileDialog->m_szFileName);
    Validate();
  }
  return 0;
}

BOOL PickColor(COLORREF& Color)
{
  COLORREF NewColor = Color;
  if (NewColor == -1)
    NewColor = 0x000000;
  CColorDialog dlg(NewColor);
  int result = dlg.DoModal();
  if (result == 1)
  {
    Color = dlg.m_cc.rgbResult;
    return TRUE;
  }
    return FALSE;
}

LRESULT CEditNotificationDlg::OnBnClickedEnPickbgcolor(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  PickColor(m_pNPI->m_BackgroundColor);
  UpdateColors();
  return 0;
}

LRESULT CEditNotificationDlg::OnBnClickedEnPickfgcolor(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  PickColor(m_pNPI->m_ForegroundColor);
  UpdateColors();
  return 0;
}
