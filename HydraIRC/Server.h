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

// Server.h : comment here
//
/////////////////////////////////////////////////////////////////////////////

#define SVR_STATE_INITIALISING 0 // just after class creation
#define SVR_STATE_CONNECTING   1 // socket stuff is not setup yet, but window is.
#define SVR_STATE_CONNECTED    2 // socket connected
#define SVR_STATE_DISCONNECTED 3 // socket disconnected

enum OnTextInputFlags 
{
  OTIF_None = 0,
  OTIF_Parse = (1<<0),
  OTIF_StripWhitespace = (1<<0)
};

/* 
//unused for now
typedef struct IDStringPair_s
{
  int ID;
  char *Str;
} IDStringPair_t;
*/
class UserIdentity
{
public:
  int m_ID;                          // the unique identifier
  char *m_Description;               // as displayed in prefs
  char *m_RealName;
  char *m_UserName;
  CSimpleArray< char * > m_NickList;
  char *m_IdentdUserName;
  char *m_IdentdOS;

  char *GetNick(int Index) // Index starts at 0 for the first one.
  {
    if (Index < m_NickList.GetSize() && Index >= 0)
      return m_NickList[Index];
    else
      return "Unknown";
  }

  UserIdentity( void )
  {
#ifdef VERBOSE_DEBUG
    sys_Printf(BIC_FUNCTION,"new blank UserIdentity\n");
#endif
    Init();
  }

  void Init( void )
  {
    m_ID = -1; // 0 based index -1 == invalid.
    m_Description = NULL;
    m_RealName = NULL;
    m_UserName = NULL;
    m_IdentdUserName = NULL;
    m_IdentdOS = NULL;
  }

  void Clear( void )
  {
    if (m_Description) free (m_Description);
    if (m_RealName) free(m_RealName);
    if (m_UserName) free(m_UserName);
    if (m_IdentdUserName) free(m_IdentdUserName);
    if (m_IdentdOS) free (m_IdentdOS);
    
    while (m_NickList.GetSize() > 0)
    {
      free(m_NickList[0]);
      m_NickList.RemoveAt(0);
    }

    Init();
  }

  ~UserIdentity( void )
  {
#ifdef VERBOSE_DEBUG
    sys_Printf(BIC_FUNCTION,"Deleting Class Instance: UserIdentity\n");
#endif
    Clear();
  }

  // duplicate
  void Copy(UserIdentity *pOther)
  {
#ifdef VERBOSE_DEBUG
    sys_Printf(BIC_FUNCTION,"duplicating UserIdentity\n");
#endif
    Clear();
    m_ID = pOther->m_ID;
    m_Description = strdup(pOther->m_Description);
    m_RealName = strdup(pOther->m_RealName);
    m_UserName = strdup(pOther->m_UserName);
    m_IdentdUserName = strdup(pOther->m_IdentdUserName);
    m_IdentdOS = strdup(pOther->m_IdentdOS);
    for (int i = 0; i < pOther->m_NickList.GetSize(); i ++ )
    {
      m_NickList.Add(strdup(pOther->m_NickList[i]));
    }
  }

  UserIdentity(UserIdentity *pOther)
  {
    Init();
    Copy(pOther);
  }
};

enum IRCNetworkDetailsFlags 
{
  INDF_NONE       = 0,
  INDF_MASTERLIST = (1<<0), // INDF_MASTERLIST and INDF_CUSTOMLIST can be set at the same time
  INDF_CUSTOMLIST = (1<<1), // if they are both set this indicates that the item ORIGINATED from the supplied network list
};

class IRCNetworkDetails
{
public:
  char *m_Name;
  int m_ID;     // 0 = unknown/unbound network, created automatically.
  int m_Flags;  // see IRCNetworkDetailsFlags / INDF_*
  char *m_IdentityName; // NULL or name of a HydraIRC user identity

  IRCNetworkDetails(char *Name, int ID, char *IdentityName, int Flags)
  {
#ifdef VERBOSE_DEBUG
    sys_Printf(BIC_FUNCTION,"new IRCNetworkDetails\n");
#endif
    m_Name = strdup(Name);
    m_ID = ID;
    m_IdentityName = strdup(IdentityName);
    m_Flags = Flags;
  }
  IRCNetworkDetails( void )
  {
#ifdef VERBOSE_DEBUG
    sys_Printf(BIC_FUNCTION,"new blank IRCNetworkDetails\n");
#endif
    Init();
  }

  void Init( void )
  {
    m_Name = NULL;
    m_IdentityName = NULL;
    m_ID = 0;
    m_Flags = INDF_NONE;
  }

  void Clear( void )
  {
    if (m_Name) free(m_Name);
    if (m_IdentityName) free(m_IdentityName);
    Init();
  }

  ~IRCNetworkDetails( void )
  {
#ifdef VERBOSE_DEBUG
    sys_Printf(BIC_FUNCTION,"Deleting Class Instance: IRCNetworkDetails\n");
#endif
    Clear();
  }

  IRCNetworkDetails(IRCNetworkDetails *pOther)
  {
    Init();
    Copy(pOther);
  }

  void Copy(IRCNetworkDetails *pOther)
  {
    m_Name = strdup(pOther->m_Name);
    m_IdentityName = strdup(pOther->m_IdentityName);
    m_ID = pOther->m_ID;
    m_Flags = pOther->m_Flags;
  }

  void RemoveCustomStatus( void )
  {
    if (! (m_Flags & INDF_CUSTOMLIST))
      return;
    m_Flags &= ~INDF_CUSTOMLIST;
    if (m_IdentityName) free(m_IdentityName);
    m_IdentityName = NULL;
  }
};

enum IRCServerDetailsFlags 
{
  ISDF_NONE       = 0,
  ISDF_MASTERLIST = (1<<0), // ISDF_MASTERLIST and ISDF_CUSTOMLIST can be set at the same time
  ISDF_CUSTOMLIST = (1<<1), // if they are both set this indicates that the item ORIGINATED from the supplied server list
};

enum PriorityNumbers // see enum g_PriorityNumbersNames
{
  PRI_DISABLED = 0,
  PRI_LOWEST,
  PRI_LOW,
  PRI_NORMAL,
  PRI_HIGH,
  PRI_HIGHEST
};

class IRCServerDetails // this should be renamed to "IRCHostDetails" as we use it for DCCChats too!
{
public:
  char *m_Name;
  char *m_Description;
  char *m_Password;       // NULL for DCC chats.
  char *m_URIParams;
  int m_Flags;            // see IRCServerDetailsFlags / ISDF_*
  int m_Priority;         // see PRI_*
  unsigned short m_Port;
  unsigned int m_NetworkID; // relates to network in g_NetworkList with matching m_ID.

  void Init( void )
  {
    m_Name = NULL;
    m_Description = NULL;
    m_Password = NULL;
    m_URIParams = NULL;
    m_Port = 0;
    m_Flags = ISDF_NONE;
    m_Priority = 0;
    m_NetworkID = NETWORK_UNKNOWN;    
  }

  IRCServerDetails(char *URI)
  {
    Init();
    ParseURI(URI);
  }

  BOOL ParseURI(char *URI)
  {
    Clear();
    int port = 6667; // TODO: replace with user preference or a define.
    char *servername;
    char *portnumstr;
    int n,p;

    BOOL Result = FALSE;

    if (!URI)
      return FALSE;

    n = strlen(URI);
    // it's not gonna be longer..
    servername = (char*)malloc(n+1);  // include space for the null terminator
    portnumstr = (char*)malloc(n+1);        // include space for the null terminator

    if (servername && portnumstr) // alloc ok ?
    {
      // strip IRC://
      if (strnicmp(URI,"IRC://",6) == 0)
      {
        // TODO: eventually we'll support connecting to a default server when the
        // user enters just "IRC://" or "IRC://:6668" or "IRC:///#hydrairc"
        n = 6;
      }
      else
      {
        n = 0;
      }


      // extract the servername
      p = 0;
      while (URI[n] != 0 && (isdigit(URI[n]) || isalpha(URI[n]) || URI[n] == '.' || URI[n] == '-'))
      {
        servername[p++] = URI[n++];
      }
      servername[p] = 0; // null terminator
      
      if (URI[n] == ':')
      {
        n++;
        // extract the port number
        p = 0;
        while (URI[n] != 0 && isdigit(URI[n]))
        {
          portnumstr[p++] = URI[n++];
        }
        portnumstr[p] = 0; // null terminator
        port = atol(portnumstr);
      }
      // valid port number
      if (port <= 0 || port >= 65535) 
        port = 6667;

      SetString(m_Name,servername);
      m_Port = port;
      if (URI[n] == '/')
        SetString(m_URIParams,URI+n+1);
      else if (URI[n] == ',') // supports "irc://<servername>:<port>,<password>" too..
        SetString(m_Password,URI+n+1);

      // TODO: decode URIParams!! (e.g. %xx)

      if (*servername)
        Result = TRUE;
    }
    if (servername) free(servername);
    if (portnumstr) free(portnumstr);

    return Result;
  }

  IRCServerDetails()
  {
#ifdef VERBOSE_DEBUG
    sys_Printf(BIC_FUNCTION,"new blank IRCServerDetails\n");
#endif
    Init();
  }

  BOOL IsValid( void )
  {
    // copy the name to the description if no description is set
    if (!m_Description && m_Name)
      m_Description = strdup(m_Name);
      
    return ((BOOL) (m_Name && *m_Name));
  }

  void Clear( void )
  {
    if (m_Name) free(m_Name);
    if (m_Description) free(m_Description);
    if (m_Password) free(m_Password);
    if (m_URIParams) free(m_URIParams);
    Init();
  }
  ~IRCServerDetails()
  {
#ifdef VERBOSE_DEBUG
    sys_Printf(BIC_FUNCTION,"Deleting Class Instance: IRCServerDetails\n");
#endif
    Clear();
  }

  // init from another
  IRCServerDetails( IRCServerDetails *pOther )
  {
    Init();
    Copy(pOther);
  }

  // duplicate
  void Copy( IRCServerDetails *pOther )
  {
#ifdef VERBOSE_DEBUG
    sys_Printf(BIC_FUNCTION,"duplicating IRCServerDetails\n");
#endif
    Clear();
    m_Name = strdup(pOther->m_Name);
    m_Description = strdup(pOther->m_Description);
    m_Password = strdup(pOther->m_Password);
    m_URIParams = strdup(pOther->m_URIParams);
    m_Port = pOther->m_Port;
    m_Priority = pOther->m_Priority;
    m_NetworkID = pOther->m_NetworkID;
    m_Flags = pOther->m_Flags;
  }

  char *BuildURI( void ) // free the string this returns.
  {
    if (!m_Name)
      return NULL;

    char uristr[10];
    sprintf(uristr,"%d",m_Port);

    int len = 6 + strlen(m_Name) +         // "IRC://<servername>"
              1 + strlen(uristr) +         // ":<port>"
              1;                           // "\0"

    if (m_URIParams)
      len += 1 + strlen(m_URIParams);      // "/<params>"

    char *newuri = (char *)malloc(len);
    if (newuri)
    {
      sprintf(newuri,"IRC://%s:%s%s%s",m_Name,uristr,m_URIParams ? "/" : "", m_URIParams ? m_URIParams : "");
      ATLASSERT(strlen(newuri) != len); // double check length
    }

    return newuri;
  }
};

enum IRCServerStatusFlags 
{
  ISSF_NONE            = 0,
  ISSF_RETRYCONNECT    = (1<<0),
  ISSF_RETRYINGCONNECT = (1<<1),
  ISSF_RETRYFAILED     = (1<<2),
  ISSF_LOGGEDIN        = (1<<3),
  ISSF_WASLOGGEDIN     = (1<<4),
};

class IRCServer :
  public IRCCommon,CListener
{
protected:
  //WSADATA wsa;
  //BOOL m_WSAInitOK;

  // The input buffer
  // IRC Spec says MAX 512 chars per line, +1 for null terminator.
  char m_SendBuf[513]; // IRC Spec says MAX 512 chars per line, +1 for null terminator.

  // this points to the response from the irc server that the server is currently
  // Parse()'ing.   Only valid when the Parse() routine is executing and NULL otheriwse.
  char *m_CurrentResponse; 
    
  // Server modes
  char *m_UserModes;
  char *m_Prefixes;
  char *m_ChannelModes[4];
  char *m_ParamChannelModes;
  char *m_ParamClearChannelModes;
  // End Server modes

  // The socket receive buffer;
  char m_SocketBuf[8192];
  char *m_SocketBufPtr;

  // The process queue
  CSimpleArray<char *> m_ProcessQueue;

  // The send queue, only used when Flood Control activates.
  CSimpleArray<char *> m_SendQueue;

  // these command profiles are run when the user is logged in, and then
  // they are removed from the list and freed
  CSimpleArray<CCommandProfile *> m_ExecOnLoginList;

  char *m_DescriptionString;

  // Server status, see SVR_STATE_* defines.
  short m_Status;

  short m_StatusFlags;

  // Store a copy of the active nick here, Sometimes a server forces a 
  // nick change, that would be reflected here.
  char *m_Nick; // default's to the nick from m_pUser
  char *m_LastNick; // when we've sucessfully logged in or set a nick, we set that here, NULL otherwise.

  // 0 based index, incremented when we try a different nick form the user's list.
  // this relates to the order of nicks in the UserIdentity->m_NickList
  // it can also be higher than the amount of nicks in the list
  // when this happens a nick is generated by HydraIRC automatically. "[n]<PreferredNick>"
  int m_NickIndex;


                        
  int m_ConnectAttempt; // reset when disconnected
  unsigned int m_LoggedInCount; // never reset

  int m_FloodControlCounter;

  NotificationEvent_t *m_pLastNE; // temporary variable, frequently used.

  // times
  time_t m_ConnectAttemptTime; // time server attempted to make a connection (used for DCC chat timeouts)
  time_t m_CreateTime; // time server instance was created (for stats)
  

private:

  void Initialise( BOOL ConnectNow );
  void UpdateLogger( void );
  char *BuildDescriptionString( void );

public:
  const char *m_Variables[VID_LAST];
  Socket *m_pSocket;
  Socket *m_pServerSocket; // used only when initiating DCC Chats
  int m_ServerID; // converted to ServerID and DCCChatID for plugins.
  int m_EventManagerID;

  // Channel List window handle
  CChannelListDlg *m_pChannelListDlg; // null if not open.

  // DCC Chat stuff
  BOOL m_IsDirect; // TRUE for DCC chats
  BOOL m_Listen;   // TRUE for DCC chat requesting, false for accepting.
  IRCServer *m_pOtherServer; // when a dcc chat is created, we store a pointer to the server
                             // that originated the dcc chat, killing a Server kills it's DCC Chats.
                             // it'd be way to complex otherwise!
  char *m_OtherNick; // this is the nick of the user that we're chatting to.
  // End: DCC Chat stuff

  // when we are logged in, we set this to a copy of m_Nick and then after a reconnect
  // we look for Quits with this nick, and if we see one we try and change the current nick
  // back to this one.
  char *m_ReclaimNick; 

  // This is that the user WANTS to connect to
  IRCServerDetails *m_pDetails;
  // Then we update this after we connect with the info returned by the server
  // IRCServerDetails *m_pConnectDetails; // Unused for now

  // We store a copy of the user's original identity details here  
  UserIdentity *m_pIdentity;

  // an array of channels that we're in.
  CSimpleArray<IRCChannel*> m_ChannelList;

  // an array of DCC chats instigated by this server
  CSimpleArray<IRCServer*> m_DCCChatList;

  // an array of DCC chats instigated by this server
  CSimpleArray<IRCQuery*> m_QueryList;

  // Functions

  ~IRCServer();

  // Init from a connect dialog or on startup.
  IRCServer(IRCServerDetails *pServerDetails, UserIdentity *pUserIdentity = NULL , BOOL Connect = TRUE );

  // Init a new server using the user details from a current server session.
  // e.g. when a user types /connect <host>
  // or when we start/accept a dcc chat
  // specify the first three parameters to create a new server instance, or 5 (or more) for a DCC chat instance
  IRCServer(IRCServerDetails *pServerDetails, IRCServer *pOther, char *OtherNick = NULL, BOOL Direct = FALSE, BOOL Listen = FALSE);

  char *GetDescriptionString( void );
  BOOL IsLoggedIn( void ) { return ((m_StatusFlags & ISSF_LOGGEDIN) && (m_Status == SVR_STATE_CONNECTED)); };

  BOOL ParseURIParams( char *URIParams);
  void ExecCommandProfile( CCommandProfile *pProfile, BOOL WaitForLogin );

  // Connecting
  void Connect( void );
  void ActualConnect( DWORD Address );
  void Disconnect( BOOL Reconnect = TRUE, BOOL SendQuit = TRUE );
  void Reconnect( void );

  // Events (triggered by code)
  void OnConnect( void );
  void OnDisconnect( void );  
  void OnLoggedIn( void );

  // Events (triggered by WM_USER messages)
  void OnNetEvent(int Event, int Error);
  void OnServerSocket(int Event, int Error);
  void OnProcessQueue( void );

  //void ClearBuffer( void );
  COLORREF GetNickHighlightColorCode( char *Nick ); //TODO: move to prefs

  short GetStatus( void )
    { return m_Status; }
  void SetNick( char *Nick ); // this doesn't send a NICK message to the server.
  char *GetNick( void )
    { return m_Nick; }
  const char* GetPrefixedNick(const char* Nick, __int64 Modes);

  // channel stuff
  IRCChannel *CreateChannel(char *Args, int Flags = CCF_NONE);
  IRCChannel *FindChannel(IRCChannel *pChannel);
  IRCChannel *FindChannel(const char *ChannelName);

  // query stuff
  IRCQuery *CreateQuery(const char *OtherNick, const char *Hostmask);
  IRCQuery *FindQuery(const char *OtherNick);

  // broadcast text to all channels on this server
  void ChannelBroadcast(int Contents, const char *format, ...);

  // forward a messge to the active window, prefs are used to deterimine whether to
  // forward the messages based on the Contents value.
  void ForwardMessage(const int Contents, const char *Message);

  // these send data to the server, via floodcontrol and the socket.
  BOOL IsFloodCausingCommand(const char *Buffer);
  BOOL Send(const char *format, ...);
  void SendBuffer(const char *Buffer);
  void ProcessSendQueue( void );

  // from CListener
  void OnEvent(int EventID, void *pData);

  // Commands (displays output where appropriate, and sends commands via the socket)
  BOOL CMD_PrivMsg(const char *To,const char *Msg); // sends query/channel text (and does local echo)
  BOOL CMD_Notice(const char *To,const char *Msg);  // sends notice text (and does local echo)
  BOOL CMD_Query(const char *To, const char *Msg);  // creates a query window too
  BOOL CMD_Action(const char *To, const char *Msg);
  BOOL CMD_CTCPRequest(const char *to, const char *format, ...);
  BOOL CMD_CTCPReply(const char *to, const char *format, ...);
  BOOL CMD_Nick(const char *Args); 
  BOOL CMD_Part(const char *Channel,const char *Msg);
  void CMD_Quit(const char *Msg);
  BOOL CMD_Raw(const char *Command );

  // Server nick prefixes
  char GetPrefixForMode(char Mode);
  char GetModeForPrefix(char Prefix);
  char GetNickPrefix(__int64 Mode);

  void UpdateStatus( void );
  void UpdateInfo( void );
  void UpdateUserIdentity( void );
  void ResetNick( void );

  void ShowChannelListDialog( void );

private:
  void InitVariables( void );
  // any calls to this outside of Parse() should be preceded with a call to InitVariables()
  void OutputFormatter(int Contents,int IRCCommandID);
  void Parse( char *processstr );  
  void ProcessChannelModeString(IRCChannel *pChannel, char *Modes);
  BOOL CheckIgnore(char *HostMask, char *Target);
};
