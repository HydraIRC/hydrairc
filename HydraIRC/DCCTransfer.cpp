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

/*	WARNING: Attention must be paid to the m_pServer variable
 *  you must check the server instance is still valid by checking that:
 *  g_ServerList.Find(m_pServer) >= 0
 *  This is important for reconnects.
 */

#include "StdAfx.h"
#include "HydraIRC.h"

void ExtractFileName (const char *path, char *dest)
{
  const char *src;

  src = path + strlen(path) - 1;

  while (src != path && *(src-1) != '/' 
         && *(src-1) != '\\' )
  {
    src--;
  }

  while (*src)
  {
    *dest++ = *src++;
  }
  *dest = 0;
}

CDCCTransfer::CDCCTransfer(IRCServer *pServer, char *OtherNick, char *FileName, unsigned long Address, int Port, unsigned long Size, int Flags)
{
  if (!pServer || !FileName)
    return; // TODO: hmm, need an exception..

  m_hFile = INVALID_HANDLE_VALUE;
  m_Status = DCCT_STATUS_UNKNOWN;
  // TODO: when DCC Xfers are stable, the pref, code and error messages for m_Verbose can be removed.
  m_Verbose = BOOLPREF(PREF_bDCCVerboseTransfers);
  m_StatusDirty = TRUE;
  m_pServer = pServer;
  m_ReadyToSend = TRUE; // TODO: check for removal of this code.
  m_FastSend = BOOLPREF(PREF_bDCCFastSends);
  m_NetworkID = pServer->m_pDetails->m_NetworkID;
  m_OtherNick = strdup(OtherNick);
  m_Address = Address;
  m_Port = Port;
  m_Size = Size;
  m_Type = Flags;
  m_LocalFileName = NULL;
  m_Retries = 0;
  // set the packet size, making sure it's not bigger than either of the buffer sizes.
  m_PacketSize = min(INTPREF(PREF_nDCCSendBufferSize),sizeof(m_SocketBuf));
  m_PacketSize = min(m_PacketSize,sizeof(m_FileBuf));
  m_pTransferSocket = NULL;
  m_pServerSocket = NULL;
  strncpy(m_IPAddressString,g_DefaultStrings[DEFSTR_Unknown],sizeof(m_IPAddressString));

  if (m_Type == DCC_RECEIVE)
  {
    // Set the remote file name
    char *FileNameCopy = strdup(FileName);
    if (!FileNameCopy)
      return; // TODO: hmm, need an exception..

    char *FileNamePtr = FileNameCopy;
    // skip leading quote
    while (*FileNamePtr == '\"' || *FileNamePtr == '\'')
      FileNamePtr++;

    // trip trailing quote
    int len = strlen(FileNamePtr);
    while (len-- >= 0 && (FileNamePtr[len] == '\"' || FileNamePtr[len] == '\''))
      FileNamePtr[len] = 0;

    m_FileName = strdup(FileNamePtr); // allocate a buffer the same size as FileNameCopy (as it'll only get smaller..)

    if (!m_FileName)
      return; // TODO: hmm, need an exception..

    // strip any leading paths.
    ExtractFileName(FileNamePtr,m_FileName);

    // replace any invalid characters with an _
    
    ReplaceInvalidFilenameCharacters(m_FileName,'_');

    free(FileNameCopy);

    char *Path;
      
    if (m_OtherNick && BOOLPREF(PREF_bCreateFoldersForDCCTransfers))
      Path = HydraIRC_BuildString(MAX_PATH,"%s\\%s",PATHPREF(PREF_pDCCDownloadsFolder),m_OtherNick);
    else
      Path = strdup(PATHPREF(PREF_pDCCDownloadsFolder));

    if (!Path)
      return; // TODO: hmm, need an exception..

    ReplaceString(&Path,"<HOMEDIR>",g_HomeDir);

    // create the path that the file is to be placed in.
    if (CreatePath(Path))
      sys_Printf(BIC_INFO,"Download path \"%s\" is OK\n",Path);
    else
      sys_Printf(BIC_ERROR,"Download path \"%s\" could not be created/accessed\n",Path);

    // Build the local file name
    m_LocalFileName = HydraIRC_BuildString(MAX_PATH,"%s\\%s",Path,m_FileName);

    free(Path);

    if (!m_LocalFileName)
      return; // TODO: hmm, need an exception..

    // TODO: remove bad ascii characters from m_LocalFileName here!
    // -> 0.3.135 - thought this is already done..

    //+ <gliptic> Check if we should auto accept or set in pending mode
    if (BOOLPREF(PREF_bDCCAutoAccept))
    {
      NotificationEvent_t *pLastNE = g_pNotificationManager->NotificationEvent(NE_DCCFILERECEIVE,m_NetworkID,m_OtherNick);
      if (pLastNE && pLastNE->m_Flags & NE_FLAG_LOGINEVENTLOG)
      {
        // add to event log
        g_pEventLogManager->BuildMessage(g_DefaultStrings[DEFSTR_DCC_Receive],m_OtherNick,m_FileName);
        g_pEventLogManager->AddItem(ELIT_DCC, ELID_DCC_Receive,m_OtherNick, NULL, m_pServer, NULL);
      }

      //Auto accept 
      Go(DCCT_STATUS_CONNECTING);
    }
    else
    {
      NotificationEvent_t *pLastNE = g_pNotificationManager->NotificationEvent(NE_DCCFILERECEIVE,m_NetworkID,m_OtherNick);
      if (pLastNE && pLastNE->m_Flags & NE_FLAG_LOGINEVENTLOG)
      {
        // add to event log
        g_pEventLogManager->BuildMessage(g_DefaultStrings[DEFSTR_DCC_ReceivePending],m_OtherNick,m_FileName);
        g_pEventLogManager->AddItem(ELIT_DCC, ELID_DCC_Receive,m_OtherNick, NULL, m_pServer, NULL);
      }

      //Set transfer in pending state
      Go(DCCT_STATUS_PENDING);
    }
    // </gliptic>


  }
  else // m_Type == DCC_SEND
  {
    // Build the remote file name (reverse of Receiving)
    m_FileName = strdup(GetFilePart(FileName));
    if (!m_FileName)
      return; // TODO: hmm, need an exception..

    // Set the local file name (reverse of Receiving)
    m_LocalFileName = strdup(FileName);
    if (!m_LocalFileName)
      return; // TODO: hmm, need an exception..

    // Replace bad/dis-allowed characters in the remote filename (like spaces) with dots ('.')
    for (int i = strlen(m_FileName)-1; i != 0; i--)
    {
      if (strchr(" &#$:",m_FileName[i]) > 0)
        m_FileName[i] = '.';
    }

    NotificationEvent_t *pLastNE = g_pNotificationManager->NotificationEvent(NE_DCCFILESEND,m_NetworkID,m_OtherNick);
    if (pLastNE && pLastNE->m_Flags & NE_FLAG_LOGINEVENTLOG)
    {
      // add to event log
      g_pEventLogManager->BuildMessage(g_DefaultStrings[DEFSTR_DCC_Send],m_FileName,m_OtherNick);
      g_pEventLogManager->AddItem(ELIT_DCC, ELID_DCC_Send,m_OtherNick, NULL, m_pServer, NULL);
    }

    Go(DCCT_STATUS_CONNECTING);

  }

}

CDCCTransfer::~CDCCTransfer( void )
{
  if (IsActive())
  {
    Disconnect(g_DefaultStrings[DEFSTR_Cancelled]);
  }

  g_pTransfersManager->RemoveTransfer(this);
  if (m_OtherNick) free (m_OtherNick);
  if (m_FileName) free (m_FileName);
  if (m_LocalFileName) free(m_LocalFileName);
  if (m_pTransferSocket)
    delete m_pTransferSocket;
  if (m_pServerSocket)
    delete m_pServerSocket;
  if (m_hFile != INVALID_HANDLE_VALUE)
  {
    CloseHandle(m_hFile);
  }
}

void CDCCTransfer::Go( int NewStatus )
{
  // new status should be either DCCT_STATUS_CONNECTING or DCCT_STATUS_PENDING
  m_Status = NewStatus;
  if (m_Status == DCCT_STATUS_CONNECTING)
  {
    strcpy(m_StatusStr,"Connecting...");
  }
  else
  {
    strcpy(m_StatusStr,"Pending...");
  }
  m_hFile = INVALID_HANDLE_VALUE;
  m_FilePosition = 0;
  m_Sent = 0;
  m_CanRetry = FALSE;
  m_Received = 0;
  m_CheckTicks = 0;
  m_ResumeOffset = 0;
  m_Position = 0;
  m_RemotePosition = 0;
  time(&m_Time);
  time(&m_ResumeTime);

  if (m_Retries == 0)
    g_pTransfersManager->AddTransfer(this);

  BOOL Duplicate = g_pTransfersManager->IsDuplicate(this);
  if (Duplicate)
  {
    Disconnect("Duplicate transfer, ignored!");
    return;
  }

  if (m_Status == DCCT_STATUS_CONNECTING)
  {
    // connect!
    Connect();
  }
}

void CDCCTransfer::Retry( void )
{
  if (!m_CanRetry)
    return;

  m_Retries++;
  Go(DCCT_STATUS_CONNECTING);
}


void CDCCTransfer::UpdateStatus( void )
{
  m_StatusDirty = TRUE;
  g_pTransfersManager->UpdateStatus(this);
}

//+ <gliptic> Accept DCC transfer method
void CDCCTransfer::Accept( void )
{
  if(m_Status == DCCT_STATUS_PENDING)
  {
    strcpy(m_StatusStr, "Connecting...");
    m_Status = DCCT_STATUS_CONNECTING;
    Connect();
  }
}
// </gliptic> 

/** Connect (or reconnect)
 *
 *  to reconnect:
 *   receives: fill in m_Address and m_Port then call this.
 *   sends:    not implemented yet
 */

void CDCCTransfer::Connect( void )
{
  DWORD dwPtr;

  if (m_Status != DCCT_STATUS_CONNECTING && m_Status != DCCT_STATUS_RESUMING)
    Disconnect("Reconnecting");

  m_CheckTicks = 0; // reset our timeout counter.

  // Receiving ?
  if (m_Type == DCC_RECEIVE)
  {
    if (m_Status != DCCT_STATUS_RESUMING)
    {
      // check for existing file, and resume if possible
      m_hFile = CreateFile(m_LocalFileName,
                         GENERIC_WRITE,
                         0, // no sharing
                         NULL, // no security
                         OPEN_EXISTING, // existing file only
                         FILE_ATTRIBUTE_NORMAL, // normal file
                         NULL); // no template
      if (m_hFile != INVALID_HANDLE_VALUE)
      {
        // file exists, can can be resumed.

        // move to the end
        dwPtr = SetFilePointer (m_hFile, 0L, NULL, FILE_END);          
        if (dwPtr == INVALID_SET_FILE_POINTER) // Test for failure
        { 
          Disconnect("Resume Error: File Seek Error");
          return;
        }
        // get the current position.
        dwPtr = SetFilePointer(m_hFile, 0, NULL, FILE_CURRENT);
        if (dwPtr != INVALID_SET_FILE_POINTER) // Test for failure
        { 
          m_ResumeOffset = dwPtr;
        }
        else
        {
          m_ResumeOffset = 0; // reset it.
          Disconnect("Resume Error: File Position Error");
          return;
        }

        // resume offset already the same as file size?
        if (m_ResumeOffset == m_Size)
        {
          // Commented out, because if you queue something up, and the xdcc bot
          // sends you the file HydraIRC will not accept the x-fer, thus
          // making the bot wait for you to accept it.
          // Commenting this out just causes the x-fer to complete immediately.
          // but without stalling the bot/other user
          //Disconnect("Resume Error: File Already Complete!");
          //return;

          sys_Printf(BIC_WARNING,"Continuing resume, even though file is already complete!\n");
        }

        if (m_ResumeOffset > m_Size)
        {
          // TODO: rename local file and download again? make a pref?
          Disconnect("Resume Error: A larger file already exists");
          return;
        }

#ifdef DEBUG
        if (m_ResumeOffset == 0)
        {
          sys_Printf(BIC_WARNING,"Resuming at position 0!\n");
        }
#endif

        // if we have a server, and the server instance is still valid...
        if (m_pServer && (g_ServerList.Find(m_pServer) >= 0))
        {
          m_Status = DCCT_STATUS_RESUMING;
          m_pServer->CMD_CTCPRequest(m_OtherNick,"DCC RESUME %s %d %lu",
                          m_FileName,
                          m_Port,
                          m_ResumeOffset);
          /*
          m_pServer->Send("PRIVMSG %s :\001DCC RESUME %s %d %lu\001\r\n",
                          m_OtherNick,
                          m_FileName,
                          m_Port,
                          m_ResumeOffset);
          */
          strcpy(m_StatusStr,"Attempting Resume");
          UpdateStatus();
          return;
        }
        else
        {
          Disconnect("Resume Error: Can't send resume request!");
          return;
        }
        // we never get here.

      } // End: File Created, resume attempted.

      // if we get here, the file doesn't exist, so accept normally.
    }

    if (m_Status == DCCT_STATUS_RESUMING)
    {
      if (m_hFile == INVALID_HANDLE_VALUE)
      {
        Disconnect("Resume error: File already closed");
        return;
      }
      // move to the resume position, if we're not already there (we should be!)
      // get the current position.
      dwPtr = SetFilePointer(m_hFile, 0, NULL, FILE_CURRENT);
      if (dwPtr != m_ResumeOffset)
      {
        dwPtr = SetFilePointer (m_hFile, m_ResumeOffset, NULL, FILE_BEGIN); 
         
        if (dwPtr == INVALID_SET_FILE_POINTER) // Test for failure
        { 
          Disconnect("Resume error: File seek error");
          return;
        }
      }

      // set up the rest of the variables that are used when a file is resumed.
      m_Position = m_ResumeOffset;
      time(&m_ResumeTime);
    }

    g_pTransfersManager->m_pTransfersView->ResetStats(this);

    unsigned char *AddressDigits;
    AddressDigits = (unsigned char *)&m_Address;
    _snprintf(m_IPAddressString,sizeof(m_IPAddressString),"%d.%d.%d.%d:%d",AddressDigits[3],AddressDigits[2],AddressDigits[1],AddressDigits[0],(unsigned short)m_Port);
    sys_Printf(BIC_INFO,"DCC Connecting to %s\n",m_IPAddressString);

    m_pTransferSocket = new Socket(g_pMainWnd->m_hWnd,WM_TRANSFEREVENT);
    if (m_pTransferSocket && m_pTransferSocket->Connect(htonl(m_Address),m_Port) == SOCK_ERR_NONE)
    {
      m_Status = DCCT_STATUS_NEGOTIATE;
      _snprintf(m_StatusStr,sizeof(m_StatusStr), "Negotiating Link (%s)", m_IPAddressString);
    }
    else
    {
      _snprintf(m_StatusStr,sizeof(m_StatusStr), g_DefaultStrings[DEFSTR_ConnectFailed], m_IPAddressString);
      Disconnect(NULL);
    }
  }
  else // m_Type == DCC_SEND
  {
    if (m_ResumeOffset != 0)
    {
      // do we still have the server ?
      if (g_ServerList.Find(m_pServer) == -1)
      {
        // No.
        Disconnect("Disconnected from server!");
        return;
      }
      else
      {

        // tell the remote user to accept the file!
        m_pServer->CMD_CTCPRequest(m_OtherNick,"DCC ACCEPT %s %d %lu",           
          "file.ext", 
          m_Port, 
          m_ResumeOffset);
        /*
        m_pServer->Send("PRIVMSG %s :\001DCC ACCEPT %s %d %lu\001\r\n",
          m_OtherNick, 
          "file.ext", 
          m_Port, 
          m_ResumeOffset);
        */
        sprintf(m_StatusStr,"Accepted resume request");
      }
    }
    else
    {
      // get the size of the file.
      m_hFile = CreateFile(m_LocalFileName,
                          GENERIC_READ,
                          FILE_SHARE_READ,
                          NULL,
                          OPEN_EXISTING, // existing file only
                          FILE_ATTRIBUTE_NORMAL,
                          NULL);
      if (m_hFile != INVALID_HANDLE_VALUE)
      {
        m_Size = GetFileSize (m_hFile, NULL);
        CloseHandle(m_hFile);
        m_hFile = INVALID_HANDLE_VALUE;

        if (m_Size == INVALID_FILE_SIZE)
        {
          Disconnect("Error getting file size (Invalid Size)");
          return;
        }
      }
      else
      {
        Disconnect("Error getting file size (Invalid Handle)");
        return;
      }

      m_pServerSocket = new Socket(g_pMainWnd->m_hWnd,WM_TRANSFEREVENT);
      if (!m_pServerSocket)
      {
        Disconnect("Could not create server socket");
        return;
      }

      // find an available port to listen on...
      m_Port = INTPREF(PREF_nDCCSendFirstPort);

      CDCCTransfer *pOther;
      while ( TRUE )
      {      
        pOther = g_pTransfersManager->FindTransferByPort(m_Port,this);
        if ((pOther == NULL) &&               // no current transfer using this port
            (m_pServerSocket->Listen(m_Port) == SOCK_ERR_NONE)) // and the socket is not in use by something else..
          break; // found a port, continue!
        else
        {
          if (m_Port < INTPREF(PREF_nDCCSendLastPort) && m_Port < 65535)
            m_Port++;
          else
          {
            Disconnect("No free ports!");
            return;
          }
        }
      }
      sys_Printf(BIC_INFO,"DCC: File %s (%d KB) available on port %d\n",m_LocalFileName,m_Size / 1024,m_Port);

      // do we still have the server ?
      if (g_ServerList.Find(m_pServer) == -1)
      {
        // No.
        Disconnect("Disconnected from server!");
        return;
      }
      else
      {
        // tell the remote user to accept the file!
        m_pServer->CMD_CTCPRequest(m_OtherNick, "DCC SEND %s %ll %d %lu",          
          m_FileName, 
          htonl(GetInternetIP()), 
          m_Port, 
          m_Size);
        /*
        m_pServer->Send("PRIVMSG %s :\001DCC SEND %s %lu %d %lu\001\r\n",
          m_OtherNick, 
          m_FileName, 
          htonl(GetInternetIP()), 
          m_Port, 
          m_Size);
        */

        m_Status = DCCT_STATUS_NEGOTIATE;
        sprintf(m_StatusStr,"Listening on port %d",m_Port);
        // sit back and wait...
      }
    }
  }

  UpdateStatus();
}

void CDCCTransfer::Resume( long Offset)
{
  if (!(m_Status == DCCT_STATUS_NEGOTIATE  || m_Status == DCCT_STATUS_RESUMING)) 
    return;

  if (m_Type == DCC_SEND)
  {
    m_ResumeOffset = Offset;
    m_Position = m_ResumeOffset;
    m_Status = DCCT_STATUS_RESUMING;
    Connect();
  }
  else // DCC_RECEIVE
  {
    // TODO: notify user of Offset mismatch, or adjust accordingly?
    if (m_ResumeOffset == Offset)
    {
      Connect();
    }
  } 
}
void CDCCTransfer::Disconnect( char *Reason )
{
  NotificationEvent_t *pLastNE = NULL;

  // disconnect socket
  if (m_pTransferSocket)
  {
    m_pTransferSocket->CloseSocket();
    delete m_pTransferSocket;
    m_pTransferSocket = NULL;
  }  

  if (m_pServerSocket)
  {
    m_pServerSocket->CloseSocket();
    delete m_pServerSocket;
    m_pServerSocket = NULL;
  }  

  // close file
  if (m_hFile != INVALID_HANDLE_VALUE)
  {
    CloseHandle(m_hFile);
    m_hFile = INVALID_HANDLE_VALUE;
  }

  // update the status, remembering that the socket closes after we complete
  if (m_Status != DCCT_STATUS_COMPLETE)
  {
    if (m_Type == DCC_RECEIVE)
    {
      pLastNE = g_pNotificationManager->NotificationEvent(NE_DCCFILERECEIVEFAILED,m_NetworkID,m_OtherNick);
      switch (m_Status)
      {
        // we can't always retry a receive.
        case DCCT_STATUS_NEGOTIATE:
        case DCCT_STATUS_RESUMING:
          m_CanRetry = TRUE;
          break;        
      }
    }
    else
    {
      pLastNE = g_pNotificationManager->NotificationEvent(NE_DCCFILESENDFAILED,m_NetworkID,m_OtherNick);
      m_CanRetry = TRUE; // we can always retry a send!
    }
    m_Status = DCCT_STATUS_ERROR;
    if (Reason)
    {
      strcpy(m_StatusStr,Reason);
    }
  }
  else
  {
    // file completed!
    if (m_Type == DCC_RECEIVE)
      pLastNE = g_pNotificationManager->NotificationEvent(NE_DCCFILERECEIVECOMPLETE,m_NetworkID,m_OtherNick);
    else
      pLastNE = g_pNotificationManager->NotificationEvent(NE_DCCFILESENDCOMPLETE,m_NetworkID,m_OtherNick);
  }
  UpdateStatus();

  char message[4096];

  // you gotta love ternary operators for stuff like this:
  _snprintf(message,sizeof(message),"ctcp/dcc %s %s %s of %s %s%s (%s %ldKB of %ldKB)",
    m_Type == DCC_RECEIVE ? "Receive" : "Send",
    m_Type == DCC_RECEIVE ? "from" : "to",
    m_OtherNick,
    m_FileName,
    m_Status == DCCT_STATUS_COMPLETE ? "Completed" : "Failed: ",
    m_Status == DCCT_STATUS_COMPLETE ? "" : m_StatusStr,
    m_Type == DCC_RECEIVE ? "Received" : "Sent",
    (m_Position / 1024),
    (m_Size / 1024));
  
  // if we have a server, and the server instance is still valid...
  BOOL ServerStillValid = m_pServer && (g_ServerList.Find(m_pServer) >= 0);

  // display message in server window
  char percentescapedbuf[512*2 + 1];
  EscapePercentCharacters(percentescapedbuf,message,sizeof(percentescapedbuf));

  if (ServerStillValid)
  {
      m_pServer->Printf(BIC_CTCP,percentescapedbuf); // TODO: BIC_QUERYTEXT ?  BIC_TRANSFERINFO ???
  }

  if (pLastNE && pLastNE->m_Flags & NE_FLAG_LOGINEVENTLOG)
  {
    // add to event log
    g_pEventLogManager->AddItem(ELIT_DCC,
      m_Status == DCCT_STATUS_COMPLETE ? 
        (m_Type == DCC_RECEIVE ? ELID_DCC_ReceiveComplete : ELID_DCC_SendComplete)
        : 
        (m_Type == DCC_RECEIVE ? ELID_DCC_ReceiveFailed : ELID_DCC_SendFailed), 
      m_OtherNick, message, 
      ServerStillValid ? m_pServer : NULL, NULL);
  }

  // and also display to the active window, if the server isn't
  // the active window (or null). (to avoid printing it twice to the
  // same window)
  IRCServer *pActiveServer;
  if (g_pMainWnd)
  {
    pActiveServer = FindIRCServer(g_pMainWnd->MDIGetActive(),FALSE);
    if ((pActiveServer != m_pServer) || (m_pServer == NULL) || (pActiveServer == NULL))
      HydraIRC_ActiveWindowPrintf(BIC_CTCP,percentescapedbuf); // TODO: BIC_QUERYTEXT ?  BIC_TRANSFERINFO ???
  }

  // TODO: try and requeue/resend if failed?
}

/*  Check the transfer's not timed out.
 *
 *  this get's called once a second, unless the main app is really busy
 *  in which case it might be longer.
 */
void CDCCTransfer::Check( void )
{
  // only check for timeouts if we're connected...
  if (m_Status == DCCT_STATUS_CONNECTED) 
  {
    m_CheckTicks++;
    if (m_CheckTicks > INTPREF(PREF_nDCCTransferTimeout)) 
    {
      Disconnect("Timeout!");
    }
    else
    {
      // Prevent Stalls.
      ReadTransferSocket();
      WriteTransferSocket();
    }
  }
  // ... or if we're resuming...
  if (m_Status == DCCT_STATUS_RESUMING) 
  {
    m_CheckTicks++;
    // waiting for response from remote host, after we tried a resume.
    if (m_CheckTicks > INTPREF(PREF_nDCCResumeTimeout))
    {
      Disconnect("Resume Failed");
    }
  }
  // ... or if we're waiting for a remote user...
  if (m_Status == DCCT_STATUS_NEGOTIATE) 
  {
    m_CheckTicks++;
    // waiting for response from remote host, after we tried a resume.
    if (m_CheckTicks > INTPREF(PREF_nDCCNegotiateTimeout))
    {
      Disconnect("Negotiate Failed");
    }
  }
}

void CDCCTransfer::OnConnect( void )
{
  DWORD dwPtr;

  m_Status = DCCT_STATUS_CONNECTED;

  unsigned char *AddressDigits = (unsigned char *)&m_pTransferSocket->GetAddress()->sin_addr;
  _snprintf(m_IPAddressString,sizeof(m_IPAddressString),"%d.%d.%d.%d:%d",AddressDigits[0],AddressDigits[1],AddressDigits[2],AddressDigits[3],(unsigned short)m_Port);
  sys_Printf(BIC_INFO,"DCC Connected to %s\n",m_IPAddressString);

  strcpy(m_StatusStr,"Connected");
  UpdateStatus();

  if (m_Type == DCC_RECEIVE)
  {
    // file will be at the correct position to start resuming from
    // if a resume is required and m_hFile will not be invalid.
    // otherwise, create the file
    if (m_hFile == INVALID_HANDLE_VALUE)
    {
      m_hFile = CreateFile(m_LocalFileName,
                          GENERIC_WRITE,
                          FILE_SHARE_READ,
                          NULL,
                          OPEN_ALWAYS,
                          FILE_ATTRIBUTE_NORMAL,
                          NULL);
      if (m_hFile == INVALID_HANDLE_VALUE)
      {
        Disconnect("Can't create file!");
        // todo: meaningful error messages - see ms-help://MS.VSCC.2003/MS.MSDNQTR.2004OCT.1033/debug/base/retrieving_the_last_error_code.htm
        return;
      }
    }
    else
    {
      g_pEventLogManager->BuildMessage(g_DefaultStrings[DEFSTR_DCC_ReceiveResumed],m_FileName,m_OtherNick,m_ResumeOffset);
      g_pEventLogManager->AddItem(ELIT_DCC, ELID_DCC_Receive,m_OtherNick, NULL, m_pServer, NULL);
    }
  }
  else // m_Type == DCC_SEND
  {
    g_pEventLogManager->BuildMessage(g_DefaultStrings[DEFSTR_DCC_SendAccepted],m_FileName,m_OtherNick);
    g_pEventLogManager->AddItem(ELIT_DCC, ELID_DCC_Send,m_OtherNick, NULL, m_pServer, NULL);

    m_hFile = CreateFile(m_LocalFileName,
                        GENERIC_READ,
                        FILE_SHARE_READ,
                        NULL,
                        OPEN_EXISTING, // existing file only
                        FILE_ATTRIBUTE_NORMAL,
                        NULL);
    if (m_hFile == INVALID_HANDLE_VALUE)
    {
      Disconnect("Can't open local file!");
      return;
    }

    if (m_ResumeOffset != 0)
    {
      // move to the resume position
      dwPtr = SetFilePointer (m_hFile, m_ResumeOffset, NULL, FILE_BEGIN); 
        
      if (dwPtr == INVALID_SET_FILE_POINTER || dwPtr != m_ResumeOffset) // Test for failure
      { 
        Disconnect("Resume error: File seek error");
        return;
      }

      m_Position = m_ResumeOffset;
      m_FilePosition = m_ResumeOffset;
      time(&m_ResumeTime);

      g_pTransfersManager->m_pTransfersView->ResetStats(this);

      g_pEventLogManager->BuildMessage(g_DefaultStrings[DEFSTR_DCC_SendResumed],m_FileName,m_OtherNick,m_ResumeOffset);
      g_pEventLogManager->AddItem(ELIT_DCC, ELID_DCC_Send,m_OtherNick, NULL, m_pServer, NULL);

    }

  }
}



void CDCCTransfer::OnDCCServerSocketEvent(int Event, int Error)
{
  // Todo: display more info about which transfer had a problem
  if (Error != 0)
    sys_Printf(BIC_WARNING,"Transfer Socket Error: (%d : %s)\n", Error, m_pServerSocket->GetWSASelectErrorString(Error));

  switch(Event)
  {
    case FD_ACCEPT:
      m_pTransferSocket = new Socket(g_pMainWnd->m_hWnd,WM_TRANSFEREVENT);
      if (m_pTransferSocket)
      {
        if (m_pTransferSocket->Accept(m_pServerSocket,FD_CLOSE | FD_READ | FD_CONNECT | FD_WRITE))
        {
          m_pServerSocket->CloseSocket(); // we don't want any more connections
          strcpy(m_StatusStr,"Socket connection accepted");
          UpdateStatus();
        }
        else
        {
          Disconnect("Couldn't accept socket");
          return;
        }
      }
      else
      {
        Disconnect("Couldn't create socket");
        return;
      }
      break;
    case FD_CLOSE: // TODO: check this code...
#ifdef DEBUG
      sys_Printf(BIC_INFO,"Server socket close notification, m_Status = %d\n",m_Status);
#endif
      if (m_pServerSocket->GetSocket() != INVALID_SOCKET)
        m_pServerSocket->CloseSocket();
      break;

#ifdef DEBUG
    default:
      sys_Printf(BIC_INFO,"unprocessed: OnDCCServerSocketEvent(%d,%d), m_Status = %d\n",Event, Error, m_Status);
#endif

  }
}

void CDCCTransfer::OnDCCTransferSocketEvent(int Event, int Error)
{
  // Todo: display more info about which transfer had a problem
  if (Error != 0)
    sys_Printf(BIC_WARNING,"Transfer Socket Error: (%d : %s)\n", Error, m_pTransferSocket->GetWSASelectErrorString(Error));

  switch(Event)
  {
    case FD_ACCEPT:
//#ifdef DEBUG
      if (m_Verbose)
        sys_Printf(BIC_INFO,"FD_ACCEPT Notification, m_Position = %d\n",m_Position);
//#endif
      // follow through..
    case FD_CONNECT:
      if (m_pTransferSocket->GetSocket() == INVALID_SOCKET || Error != 0)
      {
        _snprintf(m_StatusStr,sizeof(m_StatusStr), g_DefaultStrings[DEFSTR_ConnectFailed], m_IPAddressString);
        Disconnect(NULL);
      }
      else
      {
        OnConnect();
      }
      break;
    case FD_CLOSE:
      _snprintf(m_StatusStr,sizeof(m_StatusStr), g_DefaultStrings[DEFSTR_ConnectionClosed], m_IPAddressString);
      Disconnect(NULL);
      break;
    case FD_WRITE:
      {
//#ifdef DEBUG
        if (m_Verbose)
          sys_Printf(BIC_INFO, "FD_WRITE Notification, m_Position = %d, m_RemotePosition = %d\n",m_Position,m_RemotePosition);
//#endif
        if (m_Type == DCC_SEND)
        {

          // this is the fist FD_WRITE we've received...
          if (m_Status != DCCT_STATUS_CONNECTED)
            OnConnect();

          WriteTransferSocket();
        }
      }
      break;
    case FD_READ:
      {
//#ifdef DEBUG
        if (m_Verbose)
          sys_Printf(BIC_INFO, "FD_READ Notification, m_Position = %d, m_RemotePosition = %d\n",m_Position,m_RemotePosition);
//#endif
        ReadTransferSocket();
      }
      break;
#ifdef DEBUG
    default:
      sys_Printf(BIC_INFO,"unprocessed: OnDCCTransferSocketEvent(%d,%d), m_Status = %d\n",Event, Error, m_Status);
#endif

  }
}

void CDCCTransfer::ReadTransferSocket( void )
{
  int BytesRead;
  DWORD BytesWritten;
  unsigned long cbLen;
  unsigned long Bytes;
  
  // Set this now, something's changed.
  // Note: we do this *without* calling UpdateStatus() because
  // we don't want to call UpdateStatus() each time we receive
  // a bit of data.
  m_StatusDirty = TRUE;

  if (m_Type == DCC_RECEIVE)
  {
    cbLen = m_pTransferSocket->GetWaiting();
    cbLen = min(sizeof(m_SocketBuf),cbLen);

    BytesRead = m_pTransferSocket->Receive(m_SocketBuf,cbLen,0);

    // we don't do any null terminating, it's a data stream...

    if (BytesRead == SOCKET_ERROR)
    {
      int Error = WSAGetLastError();
      if (m_Verbose)
        sys_Printf(BIC_WARNING,"Socket error while receiving: (%d : %s)\n", Error, m_pTransferSocket->GetWSASelectErrorString(Error));
      return;
    }
    else
    {
      if (BytesRead > 0)
      {
        // we got some data!
        m_CheckTicks = 0; // reset our timeout counter.

        // write to the file and check for error.
        LockFile(m_hFile, m_Position, 0, BytesRead, 0);
        WriteFile(m_hFile, m_SocketBuf, BytesRead, &BytesWritten, NULL);
        UnlockFile(m_hFile, m_Position, 0, BytesRead, 0);

        if (BytesRead != BytesWritten)
        {
          sys_Printf(BIC_ERROR,"File write error (Bytes Written: %d != Bytes to Write: %d)\n", BytesWritten, BytesRead);
          Disconnect("File write error");
        }
        else
        {
          m_Received += BytesRead;
          m_Position += BytesRead;

          Bytes = htonl(m_Position);

          BytesWritten = m_pTransferSocket->SendRaw((char *)&Bytes, sizeof(Bytes), 0);
          if (BytesWritten == SOCKET_ERROR)
          {
            int Error = WSAGetLastError();
            if (m_Verbose)
              sys_Printf(BIC_WARNING,"Socket error while sending (1): (%d : %s)\n", Error, m_pTransferSocket->GetWSASelectErrorString(Error));
            // carry on regardless of error.
          }
          else
          {
            m_Sent += BytesWritten;
          }



          if (m_Position == m_Size)
          {
            CloseHandle(m_hFile);
            m_hFile = INVALID_HANDLE_VALUE;

            m_Status = DCCT_STATUS_COMPLETE;
            strcpy(m_StatusStr,"Complete");
            UpdateStatus();

            // Note: socket is disconnected by remote host.
            // so we'll receive a FD_CLOSE in a moment...
          }
        }
      }
    }
  }
  else // m_Type == DCC_SEND
  {    
    int LoopCount = 0; // make sure we exit this loop every so often
    // as the dcc transfers run in the same thread as the rest of the app

    //do
    //{
      cbLen = m_pTransferSocket->GetWaiting();
      cbLen = min(sizeof(m_SocketBuf),cbLen);

      if (cbLen < sizeof(DWORD))
      {
        //if (m_Verbose)
        //  sys_Printf(BIC_SOCKET,"Bytes waiting = %d (< %d) (\n",cbLen, sizeof(DWORD));

        //g_pMainWnd->PostMessage(WM_PROCESSDCCS,0,0);
        return;
      }
      else
      {
        if (m_Verbose)
          sys_Printf(BIC_SOCKET,"Bytes Waiting: %d\n", cbLen);
      }

      BytesRead = m_pTransferSocket->Receive(m_SocketBuf,sizeof(DWORD),0);

      // we don't do any null terminating, it's a data stream...

      if (BytesRead == SOCKET_ERROR)
      {
        int Error = WSAGetLastError();
        if (m_Verbose)
          sys_Printf(BIC_SOCKET,"Socket error while receiving: (%d : %s)\n", Error, m_pTransferSocket->GetWSASelectErrorString(Error));
        return;
      }
      else
      {
        m_Received += BytesRead;

        if (BytesRead == 0)
        {
          if (m_Verbose)
            sys_Printf(BIC_WARNING,"0 byte socket read with %d bytes waiting\n",cbLen);
        }
        else
        {
          // we got some data!
          m_CheckTicks = 0; // reset our timeout counter.
          
          // check file position.
          m_RemotePosition = htonl(*((DWORD*)m_SocketBuf));

          if (m_RemotePosition == m_Size)
          {
            m_Status = DCCT_STATUS_COMPLETE;
            strcpy(m_StatusStr,"Complete");
            //UpdateStatus();
            Disconnect(NULL);
            return;
          }
          else
          {
            if (m_RemotePosition != m_Position)
            {
              //if (m_Verbose)
              //  sys_Printf(BIC_WARNING,"m_RemotePosition (%d) != m_Position (%d)\n",m_RemotePosition,m_Position);
            }
            else
            {
              //if (m_Verbose)
              //  sys_Printf(BIC_INFO,"m_RemotePosition (%d) == m_Position (%d)\n",m_RemotePosition,m_Position);
              m_ReadyToSend = TRUE;
            }
            WriteTransferSocket();
          }
        }
      }
      /*
      if (cbLen > sizeof(DWORD))
      {
        if (m_Verbose)
          sys_Printf(BIC_INFO,"Large DCC Receive Buffer!, parsing again! (%d)\n",cbLen);
      }
      */
    //} while (cbLen > sizeof(DWORD) && LoopCount++ < 1000); // deal with max 1000 * m_PacketSize bytes per call.
    //} while (0);

    //g_pMainWnd->PostMessage(WM_PROCESSDCCS,0,0);

  } // END: m_Type == DCC_SEND
}

BOOL CDCCTransfer::ProcessFastDCCSend( void )
{
  if (m_Type == DCC_RECEIVE)
    return FALSE;

  if (m_Status != DCCT_STATUS_CONNECTED)
    return FALSE;

  ReadTransferSocket();
  WriteTransferSocket();

  return TRUE;
}

void CDCCTransfer::WriteTransferSocket( void )
{
  if (m_Type == DCC_RECEIVE) // we only do sending in here.
    return;

  if (m_Verbose)
    sys_Printf(BIC_INFO,"SendNextBlock: m_RemotePosition = %d, m_Position = %d, m_Sent = %d\n",m_RemotePosition,m_Position,m_Sent);

  if (m_hFile == INVALID_HANDLE_VALUE)
  {
    if (m_Verbose)
      sys_Printf(BIC_WARNING,"SendNextBlock: file is not open\n");
    return;
  }

  if (!m_ReadyToSend)
  {
    if (m_Verbose)
      sys_Printf(BIC_WARNING,"SendNextBlock: NOT ready to send\n");
    return;
  }

  // TODO:  Move this bit of code to Socket::IsReadyToWrite()
  fd_set socketlist;
  socketlist.fd_count = 1;
  socketlist.fd_array[0] = m_pTransferSocket->GetSocket();

  timeval socketwaittime = {0};

  int result = select(NULL,NULL,&socketlist,NULL,&socketwaittime);
  // TODO: end of bit to move

  if (result != 1)
  {
    if (m_Verbose)
      sys_Printf(BIC_WARNING,"SendNextBlock: Write socket not ready!\n");

    // TODO: FIXME!
    //g_pMainWnd->PostMessage(WM_PROCESSDCCS,0,0);
    return;
  }

  // Read From File

  DWORD BytesRead;

  if (m_Position == m_FilePosition && m_Position != m_Size) // we need to read more data from the file
  {
    // don't try and read past the end of the file (as we already told the other
    // host how big the file is).

    DWORD BytesToRead = min(m_PacketSize, m_Size - m_FilePosition);

    if (ReadFile(m_hFile, m_FileBuf, BytesToRead, &BytesRead, NULL))
    {
      m_FilePosition += BytesRead;
    }
    else
    {
      Disconnect("File read error");
      return;
    }
  }

  // Write To Socket

  int BytesWritten, BytesToSend = m_FilePosition - m_Position;

  //if (m_Verbose)
  //  sys_Printf(BIC_INFO,"SendNextBlock: BytesToSend == %d\n",BytesToSend);

  if (BytesToSend == 0)
  {
    return;
  }

  BytesWritten = m_pTransferSocket->SendRaw(m_FileBuf,BytesToSend,0);

  if (BytesWritten == SOCKET_ERROR)
  {
    int Error = WSAGetLastError();
    sys_Printf(BIC_SOCKET,"Socket error while sending (2): (%d : %s)\n", Error, m_pTransferSocket->GetWSASelectErrorString(Error));
    return;
  }

  if (BytesToSend != BytesWritten)
  {
    char *MessageStr = "Read/Send size mismatch while sending";
    Disconnect(MessageStr);
    sys_Printf(BIC_ERROR,"%s %s to %s: %d != %d\n",MessageStr, m_FileName, m_OtherNick, BytesRead, BytesWritten);
    return;
  }

  m_Sent += BytesWritten;    
  m_Position += BytesWritten;

  // now wait for the remote end to tell us how much data they've received.
  if (!m_FastSend)
    m_ReadyToSend = FALSE;

}

BOOL CDCCTransfer::IsActive( void )
{
  return ((BOOL)(m_Status != DCCT_STATUS_ERROR &&
                 m_Status != DCCT_STATUS_COMPLETE));
}
