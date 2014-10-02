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

// MsgView.cpp : implementation of the CMsgView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

extern StringPrefItem_t g_MainPrefs_Strings[];

#define CUSTOMCOLORVAL(code) m_Colors[code - PREF_COLOR_FIRST]

CMsgView::CMsgView( void )
{
  m_ControlCodeMode = CTRLCODE_MODE_INTERPRET;
  m_MaxBufferLines = DEFALT_SCROLLBUFFER_MAXLINES;
  m_BufferLines = 0;
  m_FilterList = NULL;
  m_EnableTimestamps = TRUE;
  m_Dirty = FALSE;
  m_ContextMenuID = 0;
  m_ContextMenuWnd = NULL;  
  m_ScrollLockFlags = MBSL_UNLOCKED;
  time(&m_ScrollLockTime);
  memcpy(m_Colors,g_pPrefs->m_ColorPrefs,sizeof(m_Colors));

  m_TimeStampFormat = NULL;
}

CMsgView::~CMsgView( void )
{
  if (m_TimeStampFormat)
    free(m_TimeStampFormat);
}

void CMsgView::DisplayTimeStamp( const time_t *t )
{
  // Add the timestamp.
	CHARFORMAT2 fmt;
  fmt.dwMask = CFM_BOLD | CFM_COLOR | CFM_UNDERLINE | CFM_ITALIC;
  fmt.dwEffects = 0;
  fmt.crTextColor = CUSTOMCOLORVAL(item_timestamp);
  fmt.cbSize = sizeof(CHARFORMAT2);
  SetSelectionCharFormat(fmt);  

  struct tm *newtime;

  if (!t)
  {
    time_t now;
    time(&now);
    newtime = localtime(&now);
  }
  else
  {
    newtime = localtime(t);
  }


  char *timestampformat = m_TimeStampFormat;

  // if we have a custom time stamp format, use it, otherwise find a default from somewhere...
  if (!timestampformat)
  {
    if (g_pPrefs) // make sure we *reallllly* have a default
      timestampformat = STRINGPREF(PREF_sDefaultWindowTimestampFormat);
    else
      timestampformat = g_MainPrefs_Strings[PREF_sDefaultWindowTimestampFormat - PREF_STRING_FIRST].m_Value;
  }

  strftime(m_TimeStampBuffer,sizeof(m_TimeStampBuffer)-1,timestampformat,newtime);
  m_TimeStampBuffer[sizeof(m_TimeStampBuffer)-1] = 0;

  // add a space after the timestmap (if there's room)
  int len=strlen(m_TimeStampBuffer);
  if (len < sizeof(m_TimeStampBuffer)-1)
  {
    m_TimeStampBuffer[len++] = ' ';
    m_TimeStampBuffer[len] = 0;
  }

  ReplaceSel(m_TimeStampBuffer);
  SetSel(-1,-1);
}

BOOL CMsgView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

// public
void CMsgView::Put(const int Contents, const char *buf, const time_t *t /*= NULL*/)
{
  // first things first,  do we filter it ?
  if (ProcessSimpleFilter(m_FilterList,Contents))
    return; // yes!

  // is the output buffer auto-scroll locked?
  if (IsScrollLocked())
  {
    // if we can't allocate a bufferitem or a buffer copy then just fall through to PutNow()
    char *bufcpy = strdup(buf);
    if (bufcpy)
    {
      BufferItem *pBI = new BufferItem(Contents,bufcpy);
      if (pBI)
      {
        if (t)
        {
          CopyMemory(&pBI->m_Time, t, sizeof (time_t));
        }
        m_OutputQueue.Add(pBI);
        return; // all OK!
      }
      else
        free(bufcpy);
    }    
  }

  PutNow(Contents, buf, t);
}

// private
void CMsgView::PutNow(const int Contents, const char *buf, const time_t *t /*= NULL*/)
{

  if (!buf || !*buf)
    return;

  // we've NOT filtered it, so that means it's going to be displayed...
  m_Dirty = TRUE;

	CHARFORMAT2 fmt;
	long effects = 0;
	int i, si, cfi,cbi;
	char sfgcol[9]; // can hold "0\0" - "16777215\0" in decimal (HEX 0x00FFFFFF)
	char sbgcol[9];
  int colornum,linecolor;
  int len = lstrlen(buf);

  char *sputbuf=strdup(buf);

  if (!sputbuf)
    return;

  //ATLASSERT(!(len>=512));

  SetRedraw(FALSE);

  /*
  SCROLLINFO ScrollInfo;
  ZeroMemory(&ScrollInfo, sizeof(SCROLLINFO));
  ScrollInfo.fMask = SIF_POS | SIF_RANGE; // SIF_ALL;
  GetScrollInfo(SB_VERT,&ScrollInfo); // returns 0,  i.e.  doesn't work, bah!
  // also, see the end of this function for another commented out line that uses this info..
  */
  POINT p = PosFromChar(GetWindowTextLength()); // so do it the slow way :(
	CRect r;
	GetClientRect(r);

  sputbuf[0] = 0; // null terminate

  SetSel(-1,-1); // deselect and move to end
  GetDefaultCharFormat(fmt);

  switch (Contents)
  {
    // Console Colors
    case BIC_XML:
    case BIC_ERROR:
      linecolor = CUSTOMCOLORVAL(item_error);
      break;
    case BIC_WARNING:
      linecolor = CUSTOMCOLORVAL(item_warning);
      break;
    case BIC_INFO:
      linecolor = CUSTOMCOLORVAL(item_info);
      break;

    // Server/Channel/Item Colors
    case BIC_SERVERMSG: // use same color as BIC_NOTICE
    case BIC_NOTICE:
      linecolor = CUSTOMCOLORVAL(item_notice);
      break;
    case BIC_TOPIC:
      linecolor = CUSTOMCOLORVAL(item_topic);
      break;
    case BIC_JOIN:
      linecolor = CUSTOMCOLORVAL(item_join);
      break;
    case BIC_KICK:
      linecolor = CUSTOMCOLORVAL(item_kick);
      break;
    case BIC_MODE:
      linecolor = CUSTOMCOLORVAL(item_mode);
      break;
    case BIC_NICK:
      linecolor = CUSTOMCOLORVAL(item_nick);
      break;
    case BIC_PART:
      linecolor = CUSTOMCOLORVAL(item_part);
      break;
    case BIC_ACTION:
      linecolor = CUSTOMCOLORVAL(item_action);
      break;
    case BIC_QUIT:
      linecolor = CUSTOMCOLORVAL(item_quit);
      break;
    case BIC_CONNECTING:
      linecolor = CUSTOMCOLORVAL(item_connecting);
      break;
    case BIC_QUERYTEXT:
      linecolor = CUSTOMCOLORVAL(item_querytext);
      break;
    case BIC_CTCP:
      linecolor = CUSTOMCOLORVAL(item_ctcp);
      break;
    case BIC_OWNTEXT:
	    linecolor = CUSTOMCOLORVAL(item_owntext);
  	  break;
    case BIC_WALLOPS:
	    linecolor = CUSTOMCOLORVAL(item_wallops);
  	  break;
    case BIC_CHANNELTEXT:
    case BIC_UNKNOWN:
    default:
	    linecolor = CUSTOMCOLORVAL(item_normal);
  }

#ifdef ZANN_FONTS
  // try setting CFM_CHARSET directly.
  fmt.dwMask = CFM_BOLD | CFM_COLOR | CFM_BACKCOLOR | CFM_UNDERLINE | CFM_ITALIC | CFM_CHARSET;
  fmt.bCharSet = RUSSIAN_CHARSET;
  strcpy(fmt.szFaceName,"Verdana");
#else
	fmt.dwMask = CFM_BOLD | CFM_COLOR | CFM_BACKCOLOR | CFM_UNDERLINE | CFM_ITALIC;
#endif

	fmt.dwEffects = effects;
  fmt.crTextColor = linecolor;
	fmt.crBackColor = CUSTOMCOLORVAL(item_background);  
	fmt.cbSize = sizeof(CHARFORMAT2);
  SetSelectionCharFormat(fmt);  

  // remember some stuff for highlighting
  BOOL HighlightMode;
	COLORREF LastTextColor = fmt.crTextColor;
	COLORREF LastBackColor = fmt.crBackColor;

#ifdef ZANN_FONTS
  HDC hDC = ::GetWindowDC(m_hWnd);
  int result = GetTextCharset(hDC); // returns 0 
  SelectObject(hDC,g_Fonts[FONT_SERVEROUTPUTFONT].Font);
  result = GetTextCharset(hDC); // returns 204
  ::ReleaseDC(m_hWnd,hDC);
#endif

  ReplaceSel("\n");
  SetSel(-1,-1);
  m_BufferLines++;

  if (m_EnableTimestamps)
  {
    DisplayTimeStamp(t);

    // reset the color back after the timestamp
    fmt.dwMask = CFM_BOLD | CFM_COLOR | CFM_UNDERLINE | CFM_ITALIC;
    fmt.dwEffects = 0;
    fmt.crTextColor = linecolor;
    fmt.cbSize = sizeof(CHARFORMAT2);
    SetSelectionCharFormat(fmt);  
  }

  si = 0;
	for (i = 0; i < len; i++)
	{
    if (m_ControlCodeMode == CTRLCODE_MODE_DISPLAY)
    {
      if (buf[i] == '\r')
        continue;
      sputbuf[si] = buf[i];
      si++;
      sputbuf[si] = 0; // null terminate

      // count the lines
      if (buf[i] == '\n')
      {
        if (sputbuf[0])
        {
          ReplaceSel(sputbuf);
          SetSel(-1,-1);
        }
        si = 0;
        sputbuf[si] = 0; // null terminate
        m_BufferLines++;

        if (m_EnableTimestamps)
        {

          DisplayTimeStamp(t);

          // reset the color back after the timestamp
          fmt.dwMask = CFM_BOLD | CFM_COLOR | CFM_UNDERLINE | CFM_ITALIC;
          fmt.dwEffects = 0;
          fmt.crTextColor = linecolor;
          fmt.cbSize = sizeof(CHARFORMAT2);
          SetSelectionCharFormat(fmt);  
        }
      }
    }
    else
    {
      switch(buf[i])
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
          if (i < lstrlen(buf)-1)
          {
            if (isdigit((unsigned char)buf[i+1]))
            {
              sfgcol[0] = buf[i+1];
              i++;

              if (i < lstrlen(buf)-1)
              {
                if (isdigit((unsigned char)buf[i+1]))
                {
                  sfgcol[1] = buf[i+1];
                  i++;

                  // check for invalid color number, step back a character if it's not valid
                  // we re-check the colornum below, and if it is invalid (like 99) we
                  // reset the colors
                  colornum = atoi(sfgcol);
                  if ((colornum <0 || colornum >15) && colornum !=99)
                  {
                    sfgcol[1] = 0;
                    i--;
                  }
                }
              }
            }
          }

          // if we've got a , then get the bg color
          if (i < lstrlen(buf)-1 && buf[i+1] == ',')
          {
            i++;

            if (i < lstrlen(buf)-1)
            {
              if (isdigit((unsigned char)buf[i+1]))
              {
                sbgcol[0] = buf[i+1];
                i++;
                if (i < lstrlen(buf)-1)
                {
                  if (isdigit((unsigned char)buf[i+1]))
                  {
                    sbgcol[1] = buf[i+1];
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
                fmt.crTextColor = CUSTOMCOLORVAL(PREF_COLOR_FIRST + colornum);
              }
              else // probably color code 99, reset
              {
                fmt.crTextColor = linecolor;
              }
            }
            if (sbgcol[0])
            {
              fmt.dwMask |= CFM_BACKCOLOR;
              colornum = atoi(sbgcol);

              if (colornum >= 0 && colornum <= 15)
              {                  
                fmt.crBackColor = CUSTOMCOLORVAL(PREF_COLOR_FIRST + colornum);
              }
              else // probably color code 99, so reset
              {
                fmt.crBackColor = CUSTOMCOLORVAL(item_background);
              }
            }
            if (!sfgcol[0] && !sbgcol[0])
            {
              // didn't get colors, behave as if this was ctrl+o
              fmt.cbSize = sizeof(CHARFORMAT2);
              fmt.dwMask = CFM_COLOR | CFM_BACKCOLOR;
              fmt.dwEffects = 0;
              fmt.crTextColor = linecolor;
              fmt.crBackColor = CUSTOMCOLORVAL(item_background);
            } 
            SetSelectionCharFormat(fmt);
          }
          break;

        case '\007': // 16 million color (HydraIRC Highlighting) - hex
        case '\004': // 16 million color - hex

          if (buf[i] == '\007')
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
          while ((i < lstrlen(buf)-1) && (isxdigit((unsigned char)buf[i+1])) && (cfi < 6))
          {
						sfgcol[cfi++] = buf[i+1];
						i++;
          }
          if (cfi != 6)
            i-=cfi; //step back if we've not got 6 hex digits

          // if we've got a ',' then get the bg color
          cbi = 0;
          if (i < lstrlen(buf)-1 && buf[i+1] == ',')
          {
            i++;
            // get the bg color
            while ((i < lstrlen(buf)-1) && (isxdigit((unsigned char)buf[i+1])) && (cbi < 6))
            {
						    sbgcol[cbi++] = buf[i+1];
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
                  fmt.crBackColor = CUSTOMCOLORVAL(item_background);
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
	              fmt.crTextColor = linecolor;
	              fmt.crBackColor = CUSTOMCOLORVAL(item_background);
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
	        fmt.crTextColor = linecolor;
	        fmt.crBackColor = CUSTOMCOLORVAL(item_background);
          SetSelectionCharFormat(fmt);

          break;
			  case '\006':
			  case '\023':
			  case '\022':
				  break;

			  default:
				  {
					  sputbuf[si] = buf[i];
					  si++;
            sputbuf[si] = 0; // null terminate

            // count the lines, add a timestamp if needed.
            if (buf[i] == '\n')
            {
              if (sputbuf[0])
              {
                ReplaceSel(sputbuf);
                SetSel(-1,-1);
              }
              si = 0;
              sputbuf[si] = 0; // null terminate
              m_BufferLines++;
              if (m_EnableTimestamps)
              {

                DisplayTimeStamp(t);

                // reset the color back after the timestamp
                fmt.dwMask = CFM_COLOR;
                fmt.crTextColor = linecolor;
                fmt.cbSize = sizeof(CHARFORMAT2);
                SetSelectionCharFormat(fmt);  
              }
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

#if 0
  // TODO: remove, testing
  POINT p2 = PosFromChar(GetWindowTextLength());
  CRect r2;
  GetClientRect(r2);

  // Bah! p2 != p in output/channel monitor windows, but it does in
  // server/channel windows.
#endif

  int CharsToGet = ((sizeof(m_BufferTrimBuf) / 2) - 1); // (2048+2) / 2 - 1 = 1024

  // remove lines of text until m_BufferLines < m_MaxBufferLines
  // note: we can't use LineCount() because we're using wordwrap.
  while (m_BufferLines >= m_MaxBufferLines)
  {
    // There is a bug in EM_GETTEXTRANGE, If you tell it to get 1024 characters
    // from the start of the buffer (0-1023) and give it a pointer to a
    // buffer that is 1024+1 bytes long (for data + null terminator)
    // sometimes it will give you more than 1024 bytes.
    // I've a feeling richedit is converting the buffer to unicode when
    // it shouldn't be.  so to avoid this we use a buffer that is twice as
    // large as the amount of data we want (1024+1 * 2 = 2050)
    // see definition of m_BufferTrimBuf in MsgView.h

    // To reproduce this bug run HydraIRC on Windows 2000 (SP3 used) with the
    // your Locale set to "Chinese (Hong Kong)" and set "Max Scroll Buffer Lines" to 20.
    // then connect to irc.chatcafe.net - the MOTD contains chinese characters.
    // see "HydraIRC/Misc/Microsoft-Bugs/GetTextRangeBug-Annotated.png"

#ifdef DEBUG
    // for debug builds we log occurences of this error to the log file.
    // we can't log them to the output window because that sometimes causes another
    // error to occur and we end up with a stack-overflow as this is called recursivly
    // by sys_Printf()

    // Safety Check!
    m_BufferTrimBuf[sizeof(m_BufferTrimBuf)-1] = 0; // set a null terminator at end of buffer
    int receivedbytes = GetTextRange(0,CharsToGet - 1,m_BufferTrimBuf); // -1 because first char is 0, not 1
    // check null terminator still present at end of buffer.
    ATLASSERT(m_BufferTrimBuf[sizeof(m_BufferTrimBuf)-1] == 0); // Huuuuge buffer overflow (twice the amount of requested data received!)

    // we can check to see if bytes received is more than bytes requested...

    if (receivedbytes > CharsToGet && BOOLPREF(PREF_bConsoleLogging))
      sys_Logf(BIC_WARNING,"Microsoft richedit EM_GETTEXTRANGE buffer overflow bug avoided! Received: %d > Requested: %d !!\n",receivedbytes,CharsToGet);
#else
    GetTextRange(0,CharsToGet - 1,m_BufferTrimBuf); // -1 because first char is 0, not 1
#endif

    // look for a \r (not a \n..)
    i = 0;
    while (m_BufferTrimBuf[i] && m_BufferTrimBuf[i] != '\r' && i < CharsToGet ) i ++;

    // delete from the start to the \r and the \r itself.
    SetSel(0,i+1);
    ReplaceSel("");
    SetSel(-1,-1);

    // decrement our counter.
    m_BufferLines --;
  }

  // TODO: this should work, why not ?
  // So we have to use this workaround, which is slower, and doesn't work
  // correctly for all windows. (see definition of p2 and r2 above)
  //if (ScrollInfo.nPos == ScrollInfo.nMax) 
  if (r.PtInRect(p))
  {
    m_Dirty = FALSE;
    SendMessage(WM_VSCROLL, SB_BOTTOM, NULL);    
  }

  SetRedraw(TRUE);
  RedrawWindow();

  free(sputbuf);
}

void CMsgView::ScrollToEnd( void )
{
  SetRedraw(TRUE);
  SetSel(-1,-1); // deselect and move to end

  /* TODO: this doesn't work ??  why ??
  SCROLLINFO ScrollInfo;
  ScrollInfo.fMask = SIF_ALL;
  GetScrollInfo(SB_VERT,&ScrollInfo);
  */
  // This causes the text in the window to align to the bottom
  // of the window edge.
  // if this is NOT done, the text remains where it is when the window is made larger
  // and the scrollbar position remains at the bottom even though view of the text
  // is not at the bottom!
  // also, the WM_VSCROLL must be sent after the following two commands
  // if you do it the other way round it only works half the time
  // Thanks again microsoft!
  RedrawWindow();
  ScrollCaret();
  SendMessage(WM_VSCROLL, SB_BOTTOM, NULL);
  m_Dirty = FALSE;
}


void CMsgView::ShowScrollbar( void )
{
  //SendMessage(EM_SHOWSCROLLBAR, (WPARAM)SB_VERT, (LPARAM)TRUE);
}

void CMsgView::SetColors( COLORREF *pColors )
{ 
  memcpy(m_Colors,pColors,sizeof(m_Colors));
  SetBackgroundColor(CUSTOMCOLORVAL(item_background));
}

LRESULT CMsgView::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
  bHandled = FALSE;
  ScrollToEnd();
  return bHandled ? 0 : 1; // WM_SIZE - If an application processes this message, it should return zero.
}

LRESULT CMsgView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
  PARAFORMAT2 pf;
  pf.cbSize = sizeof(pf);
  pf.dwMask = PFM_ALL2;
  GetParaFormat(pf);
  pf.dwMask = PFM_OFFSET;
  pf.dxOffset = 200;
  SetParaFormat(pf);
  SetAutoURLDetect(TRUE);
  SetEventMask(ENM_MOUSEEVENTS | ENM_KEYEVENTS | ENM_SELCHANGE | ENM_LINK | ENM_SCROLL | ENM_SCROLLEVENTS);
  SetBackgroundColor(CUSTOMCOLORVAL(item_background));
  //SetEditStyle(SES_EXTENDBACKCOLOR,SES_EXTENDBACKCOLOR);
  bHandled =FALSE;

  char BlankLines[1025]; // +1 for terminator
  int LineCount = min(sizeof(BlankLines) -1 , m_MaxBufferLines);

  BlankLines[LineCount] = 0;
  for (int i = 0; i < LineCount; i ++)
  {
    BlankLines[i] = '\n';
  }
  m_BufferLines = LineCount;

  SetSel(0,-1);
  ReplaceSel(BlankLines);
  SetSel(-1,-1);

  // note that we don't start the lister until we've got an output window!
  // not using this now.
  CListener::Start(&g_EventManager);

  /*
  If an application processes this message, it should return zero to
  continue creation of the window. If the application returns –1,
  the window is destroyed and the CreateWindowEx or CreateWindow
  function returns a NULL handle.
  */
	return 0;
}

// When the user drags a selection, and then releases the mouse
// we copy the select to the clipboard and then set the focus back to the
// input view
LRESULT CMsgView::OnMsgFilter(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
  bHandled = FALSE;
#ifdef DEBUG
  //sys_Printf(BIC_GUI,"child message: %d - %d\n",idCtrl,pnmh->code);
  //sys_Printf(BIC_GUI,"msg: %d - %x\n",msgfilter->msg, msgfilter->msg);
/*
  WM_KEYUP
  WM_LBUTTONUP
  WM_MOUSEMOVE
*/
#endif
  MSGFILTER *msgfilter = (MSGFILTER *)pnmh;
  
  switch(msgfilter->msg)
  {
    /* note: Switched back to using EN_LINK again

    case WM_LBUTTONDBLCLK:
      {
        SetScrollLock(MBSL_UNLOCKED);

        if (pnmh->hwndFrom == m_hWnd)
        {
          // Did the user double click on a link?
          CHARRANGE cr;
          CHARRANGE linkcr;

          GetSel(cr);
          if (GetLinkRange(cr,linkcr))
          {
            char *LinkText = (char *)malloc((linkcr.cpMax - linkcr.cpMin) + 1);
            if (LinkText)
            {
              GetTextRange(linkcr.cpMin,linkcr.cpMax,LinkText);

              // TODO: move to App->LaunchURL() or something.
              sys_Printf(BIC_INFO,"URL Doubleclicked \"%s\"\n",LinkText);
              ShellExecute(NULL,"open",LinkText,NULL,NULL,SW_SHOWNORMAL);

              free(LinkText);
            }

            SetSel(cr.cpMax,cr.cpMax);  
          }
        }
      }
      break;
    */
    case WM_LBUTTONDOWN:
      {
        // lock window
        SetScrollLock(MBSL_ACTIVE | MBSL_TIMED);
      }
      break;
    case WM_LBUTTONUP:
      {
        if (pnmh->hwndFrom == m_hWnd)
        {
          CHARRANGE cr;
          GetSel(cr);
          if (cr.cpMin != cr.cpMax)
          {
            SendMessage(WM_COPY,0,0);
          }
          SetSel(cr.cpMax,cr.cpMax);
        }
#ifdef DEBUG
        else
          sys_Printf(BIC_WARNING,"LBUTTONUP from other window?!\n");
#endif
        // unlock window
        SetScrollLock(MBSL_UNLOCKED);

      }
      break;
    case WM_RBUTTONUP:
      {
        bHandled = TRUE;

        // Owner drawn context menus don't work right (the WM_COMMAND messages are not received
        // by the correct window) if we try and use the main window's m_Cmdbar to draw the menu
        // so when using USE_XPCOMMANDBAR we send a message to the window that wants to handle
        // the context menu, then it uses it's own m_CmdBar instance to handle the drawing of
        // the menus.
#ifdef USE_XPCOMMANDBAR
        if (m_ContextMenuWnd)
        {
          ::SendMessage(m_ContextMenuWnd,WM_MSGVIEWCONTEXTMENU,m_ContextMenuID,NULL);
          break;
        }
#endif
        if (!m_ContextMenuID)
          break;

        // Display the menu at the current mouse location.
				CMenu menuContext;
				menuContext.LoadMenu(m_ContextMenuID);        
				CMenuHandle menuPopup(menuContext.GetSubMenu(0));        
        
        POINT pt;
        ::GetCursorPos(&pt);      
        //g_pMainWnd->m_CmdBar.TrackPopupMenuEx(menuPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,m_ContextMenuWnd);
        ::TrackPopupMenu(menuPopup, 0, pt.x, pt.y, 0, m_ContextMenuWnd, NULL); // send messages to specified window
      }
      break;
#ifdef DEBUG
    case WM_VSCROLL:
      {
        //sys_Printf(BIC_INFO,"Scroll Msg: 0x%08x wParam: 0x%08x lParam: 0x%08x\n",msgfilter->msg,msgfilter->wParam,msgfilter->lParam);
      }
      break;
    default:
      {
        //sys_Printf(BIC_INFO,"Other Msg: 0x%08x wParam: 0x%08x lParam: 0x%08x\n",msgfilter->msg,msgfilter->wParam,msgfilter->lParam);
      }
#endif
  }
  return 0;// TODO: Check
}

// we need to scroll to end.
LRESULT CMsgView::OnRequestScrollToEnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
  bHandled = TRUE;
#ifdef DEBUG
  sys_Printf(BIC_INFO,"WM_REQUESTSCROLLTOEND received\n");
#endif
  ScrollToEnd();
  return 0; // Custom Message
}

void CMsgView::SetFont(FontPrefItem_t *pFPI)
{
  CHARFORMAT2 fmt;        
  strcpy(fmt.szFaceName,pFPI->m_Name);
  fmt.yHeight = 20 * pFPI->m_Size; // convert point to twip (20*72=1440 units per inch)
#ifdef ZANN_FONTS
  fmt.dwMask = CFM_FACE | CFM_CHARSET | CFM_SIZE;  // twip
  fmt.bCharSet = RUSSIAN_CHARSET;
#else
  fmt.dwMask = CFM_FACE | CFM_SIZE;  // twip
#endif
  fmt.cbSize = sizeof(CHARFORMAT2);
  SetSel(0,-1);
  SetSelectionCharFormat(fmt);  
  SetSel(-1,-1);
  SetDefaultCharFormat(fmt);
}

void CMsgView::SetContextMenu(UINT MenuResourceID, HWND MessageWindow)
{
  // a MenuResourceID of 0 == no menu.
  // MessageWindow is where all the menu notifications are sent to.
  m_ContextMenuID = MenuResourceID;
  m_ContextMenuWnd = MessageWindow;
}

void CMsgView::ClearBuffer( void )
{
  SetSel(0,-1);
  ReplaceSel("");
  m_BufferLines=0;
}

LRESULT CMsgView::OnBnClickedClearBuffer(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled)
{
  ClearBuffer();
  return 0;
}

LRESULT CMsgView::OnBnClickedCopyBuffer(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled)
{
  SetSel(0,-1);
  Copy();
  SetSel(-1,-1);
  return 0;
}

// returns TRUE if flags set correctly, FLASE if not locked, or already locked.
BOOL CMsgView::SetScrollLock(int NewFlags)
{
  if (NewFlags & MBSL_ACTIVE)
  {
    if (!(m_ScrollLockFlags & MBSL_ACTIVE))
    {
      m_ScrollLockFlags = MBSL_ACTIVE;
      
      if (NewFlags & MBSL_TIMED)
      {
        m_ScrollLockFlags |= MBSL_TIMED;
        time(&m_ScrollLockTime);
      }
#ifdef DEBUG
      sys_Printf(BIC_WARNING,"Locked\n");
#endif
      return TRUE;
    }
  }
  else
  {
    if (m_ScrollLockFlags & MBSL_ACTIVE)
    {
      m_ScrollLockFlags = MBSL_UNLOCKED;
#ifdef DEBUG
      sys_Printf(BIC_WARNING,"Unlocked\n");
#endif
      ProcessOutputQueue(); // dump the contents of the output buffer now!
      return TRUE;
    }
  }
  return FALSE; // not locked, or already locked.
}

BOOL CMsgView::IsLink(CHARRANGE cr)
{
  CHARFORMAT2 fmt;
  // workaround, can't get a the format of the selection if not chars are selected.
  // but if we select the next character we might scroll to the bottom, so we need to
  // see if the next character is visible first

  POINT p = PosFromChar(cr.cpMax+1);
	CRect r;
	GetClientRect(r);
  if (r.PtInRect(p))
  {
    SetSel(cr.cpMax,cr.cpMax+1);  
    int mask = GetSelectionCharFormat(fmt);
    SetSel(cr.cpMax,cr.cpMax);


    // if we're over a link..
    if (mask & CFM_LINK && (fmt.dwEffects & CFE_LINK))
    {
      return TRUE;
    }
  }
  return FALSE;
}

BOOL CMsgView::GetLinkRange(CHARRANGE cr, CHARRANGE& linkcr)
{
  if (!IsLink(cr))
    return FALSE;

  // Find the start and end of the link.

  CHARFORMAT2 fmt;

  linkcr.cpMin = cr.cpMin;
  linkcr.cpMax = cr.cpMax+1; // select at least one character
  int mask;

  // find start (max len 512)
  while ((linkcr.cpMax - linkcr.cpMin) < 512 && (linkcr.cpMin > 0))
  {
    SetSel(linkcr.cpMin-1,linkcr.cpMax+1);  
    mask = GetSelectionCharFormat(fmt);


    // if we're over a link..
    if (mask & CFM_LINK && (fmt.dwEffects & CFE_LINK))
    {
      linkcr.cpMin--;
    }
    else
      break;
  }
  // find end (max len 512)
  while ((linkcr.cpMax - linkcr.cpMin) < 512)
  {
    SetSel(linkcr.cpMin,linkcr.cpMax+1);
    mask = GetSelectionCharFormat(fmt);


    // if we're over a link..
    if (mask & CFM_LINK && (fmt.dwEffects & CFE_LINK))
    {
      linkcr.cpMax++;
    }
    else
      break;
  }
  return TRUE;
}

BOOL CMsgView::IsScrollLocked( void )
{
  return (m_ScrollLockFlags & MBSL_ACTIVE ? TRUE : FALSE);
}

void CMsgView::ProcessOutputQueue( void )
{
  int OutputQueueItems;
  
  if (!IsScrollLocked())
  {
    while (OutputQueueItems = m_OutputQueue.GetSize() > 0)
    {
      BufferItem *pBI = m_OutputQueue[0];
      PutNow(pBI->m_Contents, pBI->m_Buffer, &pBI->m_Time);
      delete pBI;
      m_OutputQueue.RemoveAt(0);
    }
  }
}

void CMsgView::OnEvent(int EventID, void *pData)
{
  switch(EventID)
  {
    case EV_TICK:      
      // check for a timed scroll lock, and unlock if required.
      if ((m_ScrollLockFlags & (MBSL_ACTIVE | MBSL_TIMED)) == (MBSL_ACTIVE | MBSL_TIMED))
      {
        time_t now;
        time(&now);

        double diff = difftime(now,m_ScrollLockTime);

        if (diff >= INTPREF(PREF_nScrollLockTimeout))
        {
#ifdef DEBUG
          sys_Printf(BIC_WARNING,"Unlocking Timed Lock\n");
#endif

          SetScrollLock(MBSL_UNLOCKED);
        }
      }
      break;
  }
}

/*
LRESULT CMsgView::OnMenuCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  if (!lParam)
    return 0;

  int ID = ::GetMenuItemID((HMENU)lParam, wParam);

  sys_Printf(BIC_INFO,"0x%08ld\n", ID);

  return 0; // No return value.
}
*/