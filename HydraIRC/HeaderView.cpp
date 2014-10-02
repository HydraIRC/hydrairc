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

/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

CHeaderView::CHeaderView(void)
 : m_MessageWindow(NULL),
   m_CurrentHeader(NULL)
{
  // TODO: add new prefs items for these.
  m_TextColor = COLORVAL(item_headertext);
  m_BackColor = COLORVAL(item_headerbackground);
}

CHeaderView::~CHeaderView(void)
{
  SetString(m_CurrentHeader,NULL);
}

LRESULT CHeaderView::OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	if(wParam != SIZE_MINIMIZED )
	{
		RECT rc;
		GetClientRect(&rc);
    rc.right -= 20;
		m_EditCtrl.SetWindowPos(NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top ,SWP_NOZORDER | SWP_NOACTIVATE);

		GetClientRect(&rc);
    rc.left = rc.right - 20;
    m_ButtonCtrl.SetWindowPos(NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top ,SWP_NOZORDER | SWP_NOACTIVATE);

	}
  ::SendMessage(m_MessageWindow,WM_HEADERCHANGED,(WPARAM)0,(LPARAM)NULL);
	bHandled = FALSE;
	return 1; // TODO: CHECK!
}

LRESULT CHeaderView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  RECT rc;
  GetClientRect(&rc);
  rc.right -= 20;

  const DWORD dwTopicStyle = WS_CHILD | WS_VISIBLE 
                         | ES_CENTER | ES_READONLY | ES_MULTILINE                          
                         | WS_VSCROLL | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
              dwTopicExStyle = 0;

  m_EditCtrl.Create(m_hWnd, &rc, NULL, dwTopicStyle, dwTopicExStyle);
  m_EditCtrl.SetAutoURLDetect(TRUE);
  m_EditCtrl.SetEventMask(ENM_LINK);
  /*
  m_EditCtrl.ReplaceSel("The professional IRC Client | 0.3.100 | www.hydrairc.com | Say hi to new op, `duckman");
  ::SendMessage(m_MessageWindow,WM_HEADERCHANGED,(WPARAM)0,(LPARAM)NULL);
  */
  
  GetClientRect(&rc);

  const DWORD dwButtonStyle = BS_FLAT | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
  rc.left = rc.right - 20;
  m_ButtonCtrl.Create(m_hWnd,&rc,_T("  X  "),dwButtonStyle,0);
  m_ButtonCtrl.SetFont ( AtlGetStockFont ( DEFAULT_GUI_FONT ) );

  UpdateSettings();
  return 0;
}

LRESULT CHeaderView::OnCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{	
	if((HWND)lParam == m_ButtonCtrl.m_hWnd)
  {
    LRESULT Result = MessageBox("Close header pane?","Question",MB_YESNO | MB_ICONQUESTION);
    if (Result == IDYES)
		  return ::SendMessage(GetParent(), WM_COMMAND, ID_PANE_CLOSE, (LPARAM)m_hWnd);
  }
	bHandled = FALSE;
	return 1;
}


void CHeaderView::UpdateHeader(char *NewMessage)
{
  SetString(m_CurrentHeader,NewMessage);
  if (!m_CurrentHeader)
    return;

  m_EditCtrl.SetSel(0,-1);
  m_EditCtrl.ReplaceSel("");
  m_EditCtrl.PutIRCText(m_CurrentHeader, m_TextColor, m_BackColor);
  ::SendMessage(m_MessageWindow,WM_HEADERCHANGED,(WPARAM)0,(LPARAM)NULL);
}

COLORREF *PrefsColorsToIRCEditCtrlColors(COLORREF *PrefsColors)
{
  int AllocSize = 16 * sizeof(COLORREF);

  COLORREF *Destination = (COLORREF *)malloc(AllocSize);
  if (!Destination)
    return NULL;

  CopyMemory(Destination, PrefsColors, AllocSize);
  
  return Destination;
}

// Call this function after you create the window or after an prefs update
void CHeaderView::UpdateSettings( void )
{
  COLORREF *IRCColors = PrefsColorsToIRCEditCtrlColors(g_pPrefs->m_ColorPrefs);
  if (IRCColors)
  {
    m_EditCtrl.SetColors(IRCColors);
    free (IRCColors);
  }
  m_EditCtrl.SetFont(GetAppFont(PREF_fHeaderFont)); // TODO: new font pref needed
  m_EditCtrl.SetBackgroundColor(m_BackColor);

  if (m_CurrentHeader)
    UpdateHeader(m_CurrentHeader);
  else
  {
    m_EditCtrl.SetSel(0,-1);
    CHARFORMAT2 fmt;
    m_EditCtrl.GetDefaultCharFormat(fmt);  
    fmt.dwEffects = 0;
    fmt.dwMask = CFM_COLOR | CFM_BACKCOLOR;
    fmt.crTextColor = m_TextColor;
	  fmt.crBackColor = m_BackColor;
    fmt.cbSize = sizeof(CHARFORMAT2);
    m_EditCtrl.SetSelectionCharFormat(fmt);
  }
}

// call UpdateSettings() to have the changes take effect.
void CHeaderView::SetColors( COLORREF *pColors )
{  
  // extract just the colors we need from the array of colors.
  m_TextColor = pColors[item_headertext - PREF_COLOR_FIRST];
  m_BackColor = pColors[item_headerbackground - PREF_COLOR_FIRST];
}


void CIRCEditCtrl::PutIRCText( char *Message,COLORREF Foreground, COLORREF Background)
{
	CHARFORMAT2 fmt;
	long effects = 0;
	int i, si, cfi,cbi;
	char sfgcol[9]; // can hold "0\0" - "16777215\0" in decimal (HEX 0x00FFFFFF)
	char sbgcol[9];
  int colornum;

  int len = lstrlen(Message);
  char *sputbuf=strdup(Message);

  if (!sputbuf)
    return;

  sputbuf[0] = 0; // null terminate


  SetSel(-1,-1); // deselect and move to end
  GetDefaultCharFormat(fmt);
  fmt.dwMask = CFM_BOLD | CFM_COLOR | CFM_BACKCOLOR | CFM_UNDERLINE | CFM_ITALIC;
	fmt.dwEffects = effects;
  fmt.crTextColor = Foreground;
	fmt.crBackColor = Background;  
	fmt.cbSize = sizeof(CHARFORMAT2);
  SetSelectionCharFormat(fmt);  

  // remember some stuff for highlighting
  BOOL HighlightMode;
	COLORREF LastTextColor = fmt.crTextColor;
	COLORREF LastBackColor = fmt.crBackColor;

  si = 0;
	for (i = 0; i < len; i++)
	{
    if (m_ControlCodeMode == CTRLCODE_MODE_DISPLAY)
    {
      if (Message[i] == '\r')
        continue;
      sputbuf[si] = Message[i];
      si++;
      sputbuf[si] = 0; // null terminate

      // count the lines
      if (Message[i] == '\n')
      {
        if (sputbuf[0])
        {
          ReplaceSel(sputbuf);
          SetSel(-1,-1);
        }
        si = 0;
        sputbuf[si] = 0; // null terminate
      }
    }
    else
    {
      switch(Message[i])
		  {
        // bold
			  case '\002':
          if (m_ControlCodeMode == CTRLCODE_MODE_STRIP) break;

				  if (sputbuf[0])
				  {
            ReplaceSel(sputbuf);
            //ScrollCaret();
            SetSel(-1,-1);
				  }
				  //ZeroMemory(&sbuf, sizeof(sbuf));
				  si = 0;
          sputbuf[si] = 0; // null terminate

          //GetDefaultCharFormat(fmt);
				  fmt.cbSize = sizeof(CHARFORMAT2);
				  fmt.dwMask = CFM_BOLD;
					effects ^= CFE_BOLD;
				  fmt.dwEffects = effects;
          SetSelectionCharFormat(fmt);
          break;

        // color (ctrl+k)
			  case '\003':
				  if (sputbuf[0])
				  {
            ReplaceSel(sputbuf);
            //ScrollCaret();
            SetSel(-1,-1);
				  }
          si = 0;
          sputbuf[si] = 0; // null terminate
				  ZeroMemory(&sfgcol, sizeof(sfgcol));
				  ZeroMemory(&sbgcol, sizeof(sbgcol));

          // get the fg color
          if (i < lstrlen(Message)-1)
          {
            if (isdigit((unsigned char)Message[i+1]))
            {
              sfgcol[0] = Message[i+1];
              i++;

              if (i < lstrlen(Message)-1)
              {
                if (isdigit((unsigned char)Message[i+1]))
                {
                  sfgcol[1] = Message[i+1];
                  i++;

                  // check for invalid color number, step back a character if it's not valid
                  colornum = atoi(sfgcol);
                  if ((colornum <0 || colornum >15) && colornum != 99)
                  {
                    sfgcol[1] = 0;
                    i--;
                  }
                }
              }
            }
          }

          // if we've got a , then get the bg color
          if (i < lstrlen(Message)-1 && Message[i+1] == ',')
          {
            i++;

            if (i < lstrlen(Message)-1)
            {
              if (isdigit((unsigned char)Message[i+1]))
              {
                sbgcol[0] = Message[i+1];
                i++;
                if (i < lstrlen(Message)-1)
                {
                  if (isdigit((unsigned char)Message[i+1]))
                  {
                    sbgcol[1] = Message[i+1];
                    i++;

                    // check for invalid color number, step back a character if it's not valid
                    colornum = atoi(sbgcol);
                    if ((colornum <0 || colornum >15) && colornum != 99)
                    {
                      sbgcol[1] = 0;
                      i--;
                    }
                  }
                }
              }
            }
          }

          if (m_ControlCodeMode == CTRLCODE_MODE_INTERPRET)
          {
            if (sfgcol[0])
            {
              fmt.cbSize = sizeof(CHARFORMAT2);
              fmt.dwMask = CFM_COLOR;
              fmt.dwEffects = effects;
              colornum = atoi(sfgcol);
              if (colornum >= 0 && colornum <= 15)
              {
                fmt.crTextColor = m_Colors[colornum];
              }
              else // assume color code 99
              {
                fmt.crTextColor = Foreground;
              }
            }
            if (sbgcol[0])
            {
              fmt.dwMask |= CFM_BACKCOLOR;
              colornum = atoi(sbgcol);
              if (colornum >= 0 && colornum <= 15)
              {                  
                fmt.crBackColor = m_Colors[colornum];
              }
              else // assume color code 99
              {
                fmt.crBackColor = Background;
              }
            }
            if (!sfgcol[0] && !sbgcol[0])
            {
              // didn't get colors, behave as if this was ctrl+o
              fmt.cbSize = sizeof(CHARFORMAT2);
              fmt.dwMask = CFM_COLOR | CFM_BACKCOLOR;
              fmt.dwEffects = 0;
              fmt.crTextColor = Foreground;
              fmt.crBackColor = Background;
            }
            SetSelectionCharFormat(fmt);
          }
          break;

        case '\007': // 16 million color (HydraIRC Highlighting) - hex
        case '\004': // 16 million color - hex

          if (Message[i] == '\007')
            HighlightMode = TRUE;
          else
            HighlightMode = FALSE;

				  if (sputbuf[0])
				  {
            ReplaceSel(sputbuf);
            SetSel(-1,-1);
				  }
          si = 0;
          sputbuf[si] = 0; // null terminate
				  ZeroMemory(&sfgcol, sizeof(sfgcol));
				  ZeroMemory(&sbgcol, sizeof(sbgcol));

          // get the fg color
				  cfi = 0;
          while ((i < lstrlen(Message)-1) && (isxdigit((unsigned char)Message[i+1])) && (cfi < 6))
          {
						sfgcol[cfi++] = Message[i+1];
						i++;
          }
          if (cfi != 6)
            i-=cfi; //step back if we've not got 6 hex digits

          // if we've got a ',' then get the bg color
          cbi = 0;
          if (i < lstrlen(Message)-1 && Message[i+1] == ',')
          {
            i++;
            // get the bg color
            while ((i < lstrlen(Message)-1) && (isxdigit((unsigned char)Message[i+1])) && (cbi < 6))
            {
						    sbgcol[cbi++] = Message[i+1];
						    i++;
            }
            if (cbi != 6)
              i-=cbi; //step back if we've not got 6 hex digits
          }

          if (m_ControlCodeMode == CTRLCODE_MODE_INTERPRET || HighlightMode )
          {
				    fmt.cbSize = sizeof(CHARFORMAT2);
				    fmt.dwMask = CFM_COLOR;
				    fmt.dwEffects = effects;


            if (cfi == 6 || cbi == 6)
            {
              if (HighlightMode)
              {
                LastTextColor = fmt.crTextColor;
                LastBackColor = fmt.crBackColor;
              }
              if (cfi == 6)
				      {
                if (sscanf(sfgcol,"%x",&colornum) == 1 && colornum >= 0 && colornum <= 0xffffff)
                {
                  // RGB(0,0,255) == 0xff0000 not 0x0000ff
                  // so we swap the bytes.
                  fmt.crTextColor = RGBVALTOCOLORREF(colornum);
                }
              }

              if (cbi == 6)
              {
                fmt.dwMask |= CFM_BACKCOLOR;                
                if (sscanf(sbgcol,"%x",&colornum) == 1 && colornum >= 0 && colornum <= 0xffffff)
                {                  
                  fmt.crBackColor = RGBVALTOCOLORREF(colornum);
                }
                else
                {
                  fmt.crBackColor = Background;
                }
              }
              SetSelectionCharFormat(fmt);
            }
            else
            {              
              // didn't get colors, behave as if this was ctrl+o
	            fmt.cbSize = sizeof(CHARFORMAT2);
	            fmt.dwMask = CFM_BOLD | CFM_COLOR | CFM_BACKCOLOR | CFM_UNDERLINE | CFM_ITALIC;
	            fmt.dwEffects = 0;
              if (HighlightMode)
              {
	              fmt.crTextColor = LastTextColor;
	              fmt.crBackColor = LastBackColor;
              }
              else
              {
	              fmt.crTextColor = Foreground;
	              fmt.crBackColor = Background;
              }
              SetSelectionCharFormat(fmt);
            }

				  }
          break;
        // underline
			  case '\037':
          if (m_ControlCodeMode == CTRLCODE_MODE_STRIP) break;
				  if (sputbuf[0])
				  {
            ReplaceSel(sputbuf);
            SetSel(-1,-1);
				  }
				  si = 0;
          sputbuf[si] = 0; // null terminate
				  fmt.cbSize = sizeof(CHARFORMAT2);
				  fmt.dwMask = CFM_UNDERLINE;
          effects ^= CFE_UNDERLINE;
				  fmt.dwEffects = effects;
          SetSelectionCharFormat(fmt);
				  break;

        // italics
			  case '\035':
          if (m_ControlCodeMode == CTRLCODE_MODE_STRIP) break;
				  if (sputbuf[0])
				  {
            ReplaceSel(sputbuf);
            SetSel(-1,-1);
				  }
				  si = 0;
          sputbuf[si] = 0; // null terminate
				  fmt.cbSize = sizeof(CHARFORMAT2);
				  fmt.dwMask = CFM_ITALIC;
          effects ^= CFE_ITALIC;
				  fmt.dwEffects = effects;
          SetSelectionCharFormat(fmt);
				  break;

        // reverse
			  case '\026':
          if (m_ControlCodeMode == CTRLCODE_MODE_STRIP) break;
				  if (sputbuf[0])
				  {
            ReplaceSel(sputbuf);
            SetSel(-1,-1);
				  }
				  si = 0;
          sputbuf[si] = 0; // null terminate
				  fmt.cbSize = sizeof(CHARFORMAT2);
				  fmt.dwMask = CFM_COLOR | CFM_BACKCOLOR;

          // swap the colors.
          colornum = fmt.crBackColor;
          fmt.crBackColor = fmt.crTextColor;
          fmt.crTextColor = colornum;
          
          SetSelectionCharFormat(fmt);
          break;
        // normal
			  case '\017':
          if (m_ControlCodeMode == CTRLCODE_MODE_STRIP) break;
				  if (sputbuf[0])
				  {
            ReplaceSel(sputbuf);
            SetSel(-1,-1);
				  }
				  si = 0;
          sputbuf[si] = 0; // null terminate
	        fmt.cbSize = sizeof(CHARFORMAT2);
	        fmt.dwMask = CFM_BOLD | CFM_COLOR | CFM_BACKCOLOR | CFM_UNDERLINE | CFM_ITALIC;
	        fmt.dwEffects = 0;
	        fmt.crTextColor = Foreground;
	        fmt.crBackColor = Background;
          SetSelectionCharFormat(fmt);

          break;
			  case '\006':
			  case '\023':
			  case '\022':
				  break;

			  default:
				  {
					  sputbuf[si] = Message[i];
					  si++;
            sputbuf[si] = 0; // null terminate

            // count the lines, add a timestamp if needed.
            if (Message[i] == '\n')
            {
              if (sputbuf[0])
              {
                ReplaceSel(sputbuf);
                SetSel(-1,-1);
              }
              si = 0;
              sputbuf[si] = 0; // null terminate
            }
				  }
				  break;
		  }
    }
  }
  if (sputbuf[0])
  {
    ReplaceSel(sputbuf);
    SetSel(-1,-1);
  }

  free (sputbuf);
}

