/*

  HydraIRC
  Copyright (C) 2002-2006 Dominic Clifton aka Hydra

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

// colors.cpp : colors'n'stuff
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

/* ReplaceString - This functin replaces ALL occurences of s1 in buf with s2
*
* returns -1 on error, or the amount of times s1 was replaced with s2
* buf may still contain instances of s1 if an error occurs.
* TODO: optimize memory allocation/deallocation (use a memory pool ?)
*/
int ReplaceString(char **buf,const char *s1,const char *s2)
{
  /*
  * Notes:
  * if s1 is not in buf, then buf is NOT modified.
  * If s2 is shorter than s1 then the amount of memory allocated for a new
  *   buffer is slightly larger than the actual required amount (the difference
  *   is the difference in length between s1 and s2)
  * recursive replacements are handled correctly
  * This routine has been tested fairly well.
  */


  if (!buf || !s1)
    return -1;

  int replacecount = 0;
  // a NULL s2 is ok.
  if (s2 == NULL) 
    s2 = "";

  int currentlen,s2len,s1len;
  currentlen = strlen(*buf);
  s1len = strlen(s1);
  if (s2)
    s2len = strlen(s2);
  else
    s2len = 0;

  char *newbuf = *buf;
  char *ptr,*left,*right;
  int checkstart = 0;
  int leftlen;
  while (ptr = strstr(newbuf + checkstart,s1))
  {
    leftlen = ptr-newbuf;
    if (newbuf == *buf)
    {
      newbuf = strdup(*buf);
      if (!newbuf)
      {
        replacecount = -1;
        break;
      }
    }
    left = newbuf;
    newbuf = (char *)malloc(currentlen + s2len + 1); // we allocate more bytes that we actually use..
    if (newbuf)
    {
      *(left+leftlen) = 0;    
      right = ptr+s1len;
      checkstart = leftlen + s2len; // avoid recursive replacements!
      //printf("Allocated %d bytes, but really, only %d bytes are required\n",currentlen + s2len + 1,strlen(left) + strlen(s2) + strlen(right) + 1);
      currentlen = _snprintf(newbuf,currentlen + s2len,"%s%s%s",left,s2,right);
      //printf("Current len is now %d\n",currentlen);
      replacecount++;
      free(left);
    }
    else // out of memory?
    {
      newbuf = left;
      replacecount = -1;
      break;
    }
  }

  if (newbuf && newbuf != *buf)
  {
    free(*buf);
    *buf = newbuf;
  }
  return replacecount;
}

/** GetFilePart - returns a pointer to the first character of the filename.
 *
 * give it "c:\path\filename.txt" and the return will point to "filename.txt"
 * uses \'s and /'s as path separators.
 */
char *GetFilePart(char *FullPathAndFileName)
{
  if (!FullPathAndFileName)
    return NULL;

  int i = strlen(FullPathAndFileName);

  while (i > 0) 
  {
    if (strchr("\\/",FullPathAndFileName[i-1]) != NULL)
      break;

    i --;
  }

  return FullPathAndFileName+i;
}

/** GetFilePart - returns the path of the file in a new string
 *
 * give it "c:\path\filename.txt" and it will return "c:\path"
 * uses \'s and /'s as path separators.
 * the returned value must be freed.
 */
char *GetPathPart(char *FullPathAndFileName)
{
  if (!FullPathAndFileName)
    return NULL;

  int i = strlen(FullPathAndFileName);

  while (i > 0) 
  {
    if (strchr("\\/",FullPathAndFileName[i-1]) != NULL)
      break;

    i --;
  }

  char *dest = NULL;

  if (i > 0)
    dest = (char *)malloc(i);

  if (dest)
  {
    strncpy(dest,FullPathAndFileName,i-1); // miss the trailing separator
    dest[i-1] = 0;
  }

  return dest;
}

// copy the contents of an CSimpleAray<int> to a new SimpleFilter structure
// don't forget to free() the resulting SimpleFilter
int *CreateSimpleFilter(intarray& IA,int FilterType) // filter type should be FILTER_EXCLUDE or FILTER_INCLUDE
{
  int *SimpleFilter;
  int count = IA.GetSize();

  if (count <= 0)
    return NULL;

  SimpleFilter = (int *)malloc(sizeof(int) * (count + 2)); // + 2 for FILTER_<type> and FILTER_END terminator

  if (!SimpleFilter)
    return NULL;

  int i;
  int *pFilter = SimpleFilter;

  *pFilter = FilterType;
  pFilter++;

  for (i = 0; i < count; i++)
  {
    *pFilter = IA[i];
    pFilter++;
  }

  *pFilter = FILTER_END;

  
  return SimpleFilter;
}

// if filter list == NULL then result = FALSE (not filtered)
// returns TRUE if item was Filtered, FALSE otherwise
BOOL ProcessSimpleFilter(int *FilterList,int Value)
{
  if (FilterList)
  {
    BOOL found = FALSE;

    int *pFilter = FilterList;

    int FilterType = *pFilter++;

    while(*pFilter != FILTER_END)
    {
      if (*pFilter++ == Value)
      {
        found = TRUE;
        break;
      }
    }

    if ((FilterType == FILTER_EXCLUDE && found) ||
        (FilterType == FILTER_INCLUDE && !found))
    {
      return TRUE;
    }
  }
  return FALSE;
}

// Resolve a FQDN to an address, or convert a dotted IP to an address.
// *** NOT MULTI THREADED - CAN CAUSE DELAYS, USE SPARINGLY ***
DWORD ResolveFQDN(char *fqdn)
{
	DWORD Address = inet_addr(fqdn); // were we given an IP address ?
	if (Address == INADDR_NONE)
  {

    struct hostent *hp;
    hp = gethostbyname(fqdn);
    // leave Address as INADDR_NONE if gethostbyname fails.

    if (hp != NULL)
    {
      // Success!
      memcpy(&Address, hp->h_addr_list[0], sizeof(DWORD));
    }
  }
  return Address;
}

// get the address used to connect to the internet
// or, if a router is configured, get the address of that
// so that when we tell people out address, we actually give them
// the address of the router (which in turn has port forwarding
// enabled for the DCC ports..)
DWORD GetInternetIP( void )
{
  DWORD address;
  if (g_pPrefs)
  {
    char *RouterFQDN = STRINGPREF(PREF_sRouterAddress);
    if (RouterFQDN && *RouterFQDN)
    {
      address = ResolveFQDN(RouterFQDN);
      if (address != INADDR_NONE)
        return address;
    }
  }

  if (*g_FQDN)
  {
    address = ResolveFQDN(g_FQDN);
    if (address != INADDR_NONE)
      return address;
  }

	char buf[256];
	gethostname(buf, 255);
	hostent* he = gethostbyname(buf);
	if(he == NULL || he->h_addr_list[0] == 0)
		return 0;
	sockaddr_in dest;
	int i = 0;

	memcpy(&(dest.sin_addr), he->h_addr_list[i++], he->h_length);
  address = dest.sin_addr.S_un.S_addr;

  if (dest.sin_addr.s_net == 192 ||
      dest.sin_addr.s_net == 169 ||
      dest.sin_addr.s_net == 127 ||
      dest.sin_addr.s_net == 10 )
  {
    // find a better address, if we have one
		while(he->h_addr_list[i])
    {
			memcpy(&(dest.sin_addr), he->h_addr_list[i], he->h_length);
      if (dest.sin_addr.s_net != 192 &&
          dest.sin_addr.s_net != 169 &&
          dest.sin_addr.s_net != 127 &&
          dest.sin_addr.s_net != 10 )
      {
        // ok, this address isn't a known private network.
        address = dest.sin_addr.S_un.S_addr;
      }
 			i++;
   }

  }
  return address;
}

char *BuildSizeString(char *Buffer, int BufferSize, double Number)
{
  // TODO: make this accept format strings, like:
  // "<characters>%<formatcharacter><characters>"
  // Where possible "format characters" could be: 
  //  G (display round to nearest Giga*)
  //  M (display round to nearest Mega*)
  //  K (display round to nearest Kilo*)
  // Example format strings:
  // "[%GGB]", "%KKB/sec".
  // in these examples, only the % and the letter immediately following
  // it are replaced with the number.
  // example outputs for 5478934895 would be:
  // "[5.10GB]" and "5350522.35KB/Sec"
  // int the latter example, you still get a big number because a K
  // was the maximun unit allowed.

  if (Number > 1024 * 1024 * 1024)
    _snprintf(Buffer,BufferSize,"%.2fGB",Number / (double)(1024 * 1024 * 1024));
  else if (Number > 1024 * 1024)
    _snprintf(Buffer,BufferSize,"%.2fMB",Number / (double)(1024 * 1024));
  else
    _snprintf(Buffer,BufferSize,"%.2fKB",Number / 1024);
  return Buffer;
}

void OpenDirectoryForFile( char *PathAndFileName )
{
  char *FilePath = strdup(PathAndFileName);
  int len = strlen(FilePath);
  while (len-- >= 0 && FilePath[len] != '\\') 
    FilePath[len] = 0;
  ShellExecute(NULL,"open",FilePath,NULL,NULL,SW_SHOWNORMAL);
  free(FilePath);
}

////////////////

// utility functions for IntPair_t lookup tables.

void SetLookupID(IntPair_t *pTable, int ItemCount, int PrefID, int ObjectID)
{
  int i = 0;
  IntPair_t *pIP = pTable;

  while (i < ItemCount)
  {
    if (pIP->m_ID1 != PrefID)
    {
      i++;
      pIP++;
      continue;
    }

    pIP->m_ID2 = ObjectID;

    return;
  }
}

int GetLookupID(IntPair_t *pTable, int ItemCount, int PrefID)
{
  int i = 0;
  IntPair_t *pIP = pTable;

  while (i < ItemCount)
  {
    if (pIP->m_ID1 != PrefID)
    {
      i++;
      pIP++;
      continue;
    }

    return pIP->m_ID2;
  }
  return -1;
}

// make sure your output array is big enough to hold all the ouput groups from your regexp
// e.g.  1x char* for each {} pair in the regexp

int RegExpToArray(const char *Data, const char *RegExp, char **Output, BOOL CaseSensitive /*= TRUE*/)
{
  if (!Data || !RegExp || !Output)
    return -1;

  //sys_Printf(BIC_INFO,"PARSE: \"%s\" with \"%s\"",Data,RegExp);

  CAtlRegExp<> reRegExp;
  CAtlREMatchContext<> mcOutput;
  
  REParseError status = reRegExp.Parse( RegExp, CaseSensitive );
  if (!(REPARSE_ERROR_OK != status))
  {
    if (reRegExp.Match(Data, &mcOutput))
    {
      UINT i;
      for (i = 0 ; i < mcOutput.m_uNumGroups; i++)
      {
        const CAtlREMatchContext<>::RECHAR* szStart = 0;
        const CAtlREMatchContext<>::RECHAR* szEnd = 0;
        mcOutput.GetMatch(i, &szStart, &szEnd);

        int Length = szEnd - szStart;
        *Output = (char *)malloc(Length+1);
        if (*Output)
        {
          strncpy(*Output,szStart,Length);
          (*Output)[Length] = 0;
        }
        Output++;
      }
      return mcOutput.m_uNumGroups;
    }
  }
  return -1;
}

// converts "%data corruption and cra%shes suck" to "%%data corruption  and cra%%shes suck"
char *EscapePercentCharacters(char *dest,const char *source,int destbuffersize)
{
  if (!dest)
    return NULL;

  while (--destbuffersize > 1 && source && *source)
  {
    if (*source == '%' && destbuffersize)
	{
      *dest++ = '%'; // add another %
	  destbuffersize--;
	  *dest++ = *source++;
	}
	else
	{
	  *dest++ = *source++;
	}

    
  }
  *dest = 0;
  return dest;
}

char *PickRandomString(char **strings,int count)
{
  int index = rand() % count;
  return strings[index];
}

// if the destination is set, it is freed, then a copy of Value is taken if value is set
// otherwise Dest is set to NULL (so you can do SetString(MyString,NULL) to free it...)
// Note: order of copy/free is important as we can pass Dest as Value too
// so we copy to temp, then free dest, then set test to temp
// Note: Value is never fred, caller needs to make it's freed when necessary
void SetString(char* &Dest, char *Value)
{
  char *temp;

  if (Value)
    temp = strdup(Value);
  else
    temp = NULL;

  if (Dest)
    free(Dest);
  
  Dest = temp;
}

void FreeBuffers(char **buffers, int buffercount)
{
  if (!*buffers)
    return;

  for (int i= 0; i < buffercount; i++ )
  {
    if (*buffers)
    {
      free(*buffers);
      *buffers = NULL;
    }
    buffers++;
  }
}

char *AliasProcessor(char *Input,char *Alias, int InputTokenBuffers, char EscapeCharacter ,char **SubsitutionTable)
{
  char **InputTokens; // the amount of max params + 1, e.g. 4 = $1, $2, $3 and $rest
  char *InputRest;
  int InputTokenCount;

  if (!Input)
    return NULL;

  int tmpbuffersize = strlen(Input)+1+strlen(Alias)+1; // as large as both + a space and a terminator.
  char *tmpbuffer = (char *)malloc(tmpbuffersize);
  char *Output = strdup(Alias);

  InputTokens = (char **)malloc(sizeof(char *) * InputTokenBuffers);

  if (!InputTokens || !Output || !tmpbuffer)
  {
    if (InputTokens) free(InputTokens);
    SetString(Output,NULL);
    SetString(tmpbuffer,NULL);
    return NULL;
  }

  InputTokenCount = GetTokens(Input, InputTokens, InputTokenBuffers, &InputRest);

  char argname[10];

  // note: we do it in reverse order otherwise when we do "$1" we would also do "$10", so we replace the "$10"'s first..
  for (int i = InputTokenBuffers-1 ; i >= 1 ; i --)
  {
    // $n- = argments from n to end of line
    _snprintf(argname,sizeof(argname),"%c%d-",EscapeCharacter,i);

    tmpbuffer[0] = 0;
    for (int j = i; j < InputTokenBuffers; j++)
    {
      strcat(tmpbuffer,UseEmptyString(InputTokens[j]));
      strcat(tmpbuffer," ");
    }
    strcat(tmpbuffer,UseEmptyString(InputRest));
    stripwhitespace(tmpbuffer);

    ReplaceString(&Output,argname,tmpbuffer);

    // $n = argument n
    _snprintf(argname,sizeof(argname),"%c%d",EscapeCharacter,i);
    ReplaceString(&Output,argname,InputTokens[i]);
  }

  // $- = rest of line
  _snprintf(argname,sizeof(argname),"%c-",EscapeCharacter);
  ReplaceString(&Output,argname,InputRest);

  // $$ = $ character
  argname[0] = EscapeCharacter;
  argname[1] = EscapeCharacter;
  argname[2] = 0;
  ReplaceString(&Output,argname,argname+1); // use the latter half of the argname string ;)

  if (SubsitutionTable)
  {
    char *S1,*S2;
    do 
    {
      S1 = *SubsitutionTable++;
      S2 = *SubsitutionTable++;
      if (S1)
      {
        char *tmpstr = (char *)malloc(strlen(S1)+2);
        if (tmpstr)
        {
          strcpy(tmpstr+1,S1);
          tmpstr[0] = EscapeCharacter; // add the $ prefix..
          ReplaceString(&Output,tmpstr,S2); // use the latter half of the argname string ;)
          free(tmpstr);
        }
      }
    } while(S1);
  }

  free(tmpbuffer);
  FreeBuffers(InputTokens, InputTokenBuffers);
  return Output;
}

int GetTokens(char *buffer,char **buffers,int numbuffers, char **extra)
{
  // always clear the buffer pointers...
  int i = 0;
  for (i = 0 ; i < numbuffers; i ++)
  {
    buffers[i] = NULL;
  }

  if (extra)
  {
    *extra = NULL;
  }

  char *output = strdup(buffer); // can't ever overflow as the longest token can only be as long as the input string.
  char	*pOutput = output;
  char *emptystring = "\0";

  if (!buffer || !output)
    return 0;

  int currentbuffer = 0;
  char	quoteChar = '\0';
  int quoteStart = 0;
  int index = 0;
  BOOL skipWhitespace;

  while (buffer[index] && currentbuffer < numbuffers)
  {
    skipWhitespace = FALSE;

    switch (buffer[index])
    {
    case '"':
    case '\'':
      {
        *pOutput++ = buffer[index];
        *pOutput = '\0';

        if (!quoteChar)
        {
          // start quoted section only if quote is first character or quote follows whitespace
          if (index == 0 || (index>0 && (buffer[index-1] == '\t' || buffer[index-1] == ' ')))
          {
            quoteChar = buffer[index];
            quoteStart = index;
          }
        }
        else
        {
          // end quoted section only if quote is last character in line or is followed by whitespace 
          if (quoteChar == buffer[index] && (buffer[index+1]=='\0' || buffer[index+1]==' ' || buffer[index+1]=='\t'))
          {
            quoteChar = 0;
            *buffers = strdup(output);
            buffers++;
            currentbuffer++;

            pOutput = output;
            *pOutput = '\0';
            skipWhitespace = TRUE;
          }
        }
        break;
      }
    case ' ':
    case '\t':
      {
        if (!quoteChar)
        {
          if (*output)
          {
            *buffers = strdup(output);
            buffers++;
            currentbuffer++;
            pOutput = output;
            *pOutput = '\0';
            skipWhitespace = TRUE;
          }
        }
        else
        {
          *pOutput++ = buffer[index];
          *pOutput = '\0';
        }
        break;
      }
    default:
      {
        *pOutput++ = buffer[index];
        *pOutput = '\0';
        break;
      }
    }

    index++;

    if (skipWhitespace)
    {
      while (isspace ((unsigned char)buffer[index]))
      {
        index++;
      }
    }

    if (!buffer[index] && quoteChar)
    {
      // reached the end of the string while in a quote

      // add the quote char to the output buffer.
      //*pOutput++ = quoteChar;
      //*pOutput = '\0';

      // skip back to the character after the quote char
      //pOutput = output + quoteStart + 1;
      pOutput = pOutput - (index - quoteStart) + 1;
      *pOutput = 0;
      index = quoteStart+1;
      quoteChar = 0;

      skipWhitespace = FALSE;
    }
  }

  if (*output)
  {
    *buffers = strdup(output);
    currentbuffer++;
    buffers++;
  }

  if (extra)
  {
    if (currentbuffer == numbuffers)
    {
      *extra = strdup (buffer + index);
    }
    else
    {
      *extra = strdup(emptystring);
    }
  }

  // create empty null terminated strings for any remaining unused token buffers
  while (currentbuffer < numbuffers ) // -1 as index starts from 0
  {
    *buffers = strdup(emptystring);
    currentbuffer++;
    buffers++;
  }            

  free(output);

  return currentbuffer;
}

BOOL Clipboard_Copy(int format, char *data, int datalength)
{
  BOOL Result = FALSE;
  HGLOBAL hglbCopy;
  LPTSTR  lptstrCopy;
  if (OpenClipboard(NULL))
  {

    hglbCopy = GlobalAlloc(GMEM_MOVEABLE, datalength);

    if (hglbCopy)
    {
      // Empty the clipboard first!
      EmptyClipboard();

      // Lock the handle and copy the text to the buffer. 

      lptstrCopy = (LPTSTR) GlobalLock(hglbCopy); 
      memcpy(lptstrCopy, data, datalength); 
      GlobalUnlock(hglbCopy); 

      // Place the handle on the clipboard. 

      SetClipboardData(format, hglbCopy); 
      
      Result = TRUE;
    }
    CloseClipboard();
  }
  return Result;
}

char *GetIPv4AddressString(DWORD Address)
{
  unsigned char *AddressDigits;
  AddressDigits = (unsigned char *)&Address;
  return HydraIRC_BuildString(13,"%d.%d.%d.%d",AddressDigits[0],AddressDigits[1],AddressDigits[2],AddressDigits[3]);
}

// this function is called by different instances of RichEdit controls.
LRESULT OnEnLink(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
  bHandled = FALSE;
  if (!::IsWindow(pnmh->hwndFrom))
    return bHandled ? 0 : 1;

  ENLINK *link = (ENLINK *)pnmh;

  if ((BOOLPREF(PREF_bActivateLinkOnDoubleClick) && link->msg == WM_LBUTTONDBLCLK) ||
      (!BOOLPREF(PREF_bActivateLinkOnDoubleClick) && link->msg == WM_LBUTTONUP) )
  {
    CRichEditCtrl EditCtrl;
    EditCtrl.Attach(pnmh->hwndFrom);
    bHandled = TRUE;


    //SetScrollLock(MBSL_UNLOCKED);
    char *LinkText = (char *)malloc((link->chrg.cpMax - link->chrg.cpMin) + 1);
    if (LinkText)
    {
      int Len = EditCtrl.GetTextRange(link->chrg.cpMin,link->chrg.cpMax,LinkText);
      if (Len)
      {
        // TODO: move to App->LaunchURL() or something.
        sys_Printf(BIC_INFO,"URL Doubleclicked \"%s\"\n",LinkText);
        ShellExecute(NULL,NULL,LinkText,NULL,NULL,SW_SHOWNORMAL);
      }
      free(LinkText);
    }
    EditCtrl.Detach();
  }
  /*
    EN_LINK
      If you return zero, the control proceeds with its normal handling of the message.
      If you return a nonzero value, the control does not handle the message.
  */
  return bHandled ? 0 : 1;
}

char *ReplaceInvalidFilenameCharacters(char *FileName,char Replace)
{
  char *ptr = FileName;
  while (*ptr)
  {
    if (strchr("\\|<>*?/:",*ptr) != NULL)
      *ptr = '_';
    ptr++;
  }
  return FileName;
}


/*
 * r_match()
 * Iterative matching function, rather than recursive.
 * Written by Douglas A Lewis (dalewis@acsu.buffalo.edu)
 */

// TODO: replace this code with something very similar but not GNU licensed
// taken from http://www.ircnet.ru/files/match.c

int matchhostmask(char *mask, char *name)
{
	unsigned char                   *m;
	unsigned char                   *n;
	char                            *ma;
	char                            *na;
	int                              wild;

  if (!mask || !name)
    return 0;

	m = (u_char *) mask;
	n = (u_char *) name;
	ma = mask;
	na = name;
	wild = 0;

	while (1) {
		if (*m == '*') {
			while (*m == '*')
				m++;
			wild = 1;
			ma = (char *) m;
			na = (char *) n;
		}
		if (!*m) {
			if (!*n)
				return 0;
			for (m--; (m > (u_char *) mask) && (*m == '?'); m--) ;
			if (!wild)
				return 1;
			m = (u_char *) ma;
			n = (u_char *)++ na;
		} else if (!*n) {
			while (*m == '*')
				m++;
			return (*m != 0);
		}
		if ((tolower(*m) != tolower(*n)) && (*m != '?')) {
			if (!wild)
				return 1;
			m = (u_char *) ma;
			n = (u_char *)++ na;
		} else {
			if (*m)
				m++;
			if (*n)
				n++;
		}
	}
}
