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

// Channel.h : definition of the IRCChannel class
//
/////////////////////////////////////////////////////////////////////////////

// Forward declares
class IRCServer;
class IRCUser; 

// Defines
#define CHN_STATE_INITIALISING 0 // just after class creation
#define CHN_STATE_CONNECTING   1 // command issued to server, waiting for response and names list
#define CHN_STATE_CONNECTED    2 // channel joined
#define CHN_STATE_DISCONNECTED 3 // server disconnected
#define CHN_STATE_KICKED       4 // kicked!
#define CHN_STATE_BANNED       5 // banned!
#define CHN_STATE_ERROR        6 // other error!

enum ChannelFlags 
{
  CHN_FLAG_NONE    = 0,  
  CHN_FLAG_CYCLING = (1<<0)
};

class CChannelProperties
{
public:
  char *m_Name; // channel name
  char *m_Topic; // can be NULL if no topic
  char *m_Key;   // keep for rejoins, can be NULL
  int m_Modes;   // TODO: mode flags
  int m_Limit;
  int m_Users;

  CChannelProperties( void )
  {
    Initialise();
  }

  void Initialise( void )
  {
    m_Topic = NULL;
    m_Key = NULL;
    m_Name = NULL;
    m_Modes = 0;
    m_Limit = 0;
    m_Users = 0;
  }

  CChannelProperties( char *ChannelName, int Users, char *Topic)
  {
    Initialise();
    m_Name = strdup(ChannelName);
    m_Users = Users;
    m_Topic = strdup(Topic);
  }

  CChannelProperties( CChannelProperties *pOther)
  {
    m_Topic = strdup(pOther->m_Topic);
    m_Key = strdup(pOther->m_Key);
    m_Name = strdup(pOther->m_Name);
    m_Modes = pOther->m_Modes;
    m_Limit = pOther->m_Limit;
    m_Users = pOther->m_Users;
  }

  ~CChannelProperties( void )
  {
    if (m_Key) free(m_Key);
    if (m_Topic) free(m_Topic);    
    if (m_Name) free(m_Name);
  }

  // Set or clear the topic
  void SetTopic( char *Topic )
  {
    SetString(m_Topic, Topic);
  }

  // set or clear the key
  void SetKey( char *Key )
  {
    SetString(m_Key, Key);
  }

  void SetName( char *Name )
  {
    SetString(m_Name, Name);
  }

};

class IRCChannel : public IRCCommon,CListener
{
public:
  int m_ChannelID;
  CChannelProperties *m_pProperties;
  CChannelPropertiesDlg *m_pPropertiesDlg;

  char *m_KickedBy;
  char *m_KickReason;
  char *m_BannedBy;

  int m_Status; // see CHN_STATE_* defines.
  int m_Flags; // see CHN_FLAG_*
  int m_JoinErrorReason; // irc numeric containing join fail reason.
  BOOL m_HaveNames; // if FALSE it means we're waiting for a an end-of-/names message
  //BOOL m_HasUserList; // false if the child windows doesn't have a CUserListView instance
  IRCServer *m_pServer;  
  char *m_DescriptionString;
  // PrefsCollection_t *m_pPC; // PrefsCollections

  void OutputFormatter(int Contents,int IRCCommandID);

  IRCChannel(IRCServer *Server,char *ChannelName, char *Key);
  ~IRCChannel( void );

  char *GetDescriptionString( void );
  void UpdateStatus( void );
  void UpdateInfo( void );
  short GetStatus( void ) { return m_Status; }
  void SetStatus( int Status ) { m_Status = Status; }

  void Join( void );                 // tells the server to join the channel
  void OnJoin( void );               // displays the gui and kicks off events
  void OnKicked( char *By, char *Reason );
  void OnCycle( void );
  void OnBanned( void );
  void OnJoinError( int Reason );
  void OnDisconnect( void );         // updates the status
  void ReJoin( void );

  void Cycle( void );

  void RemoveUser( char *User );     // removes and deletes the user
  void RemoveUser( IRCUser *pUser ); // removes and deletes the user

  void UpdateUser(IRCUser *pUser);

  void ChangeUserNick(IRCUser *pUser, char *NewNick);
  void ChangeUserMode(IRCUser *pUser, char *NewMode);
  void ChangeChannelMode(char *NewMode, char *Param);

  BOOL CompleteNick(char *Nick, char **Dest,BOOL Forwards = TRUE, BOOL AllowNext = FALSE);

  void AddUser( char *User, char *Host );
  void AddUser( IRCUser *pUser);
  void AddUsers( char *Users );
  void SetTopic( char *Topic );
  void InitialiseGUI( void );        // Initialise's the GUI.
  void DoPropertiesDlg( void );

  // from CListener
  void OnEvent(int EventID, void *pData);

private:
  void Initialise( void );           // Called by Constructor.
  char *BuildDescriptionString( void );
  void UpdateLogger( void );
};
