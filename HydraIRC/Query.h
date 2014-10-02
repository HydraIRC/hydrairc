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

// Query.h : definition of the IRCChannel class
//
/////////////////////////////////////////////////////////////////////////////

// Forward declares
class IRCServer;
class IRCUser; 

// Defines
#define QRY_STATE_INITIALISING 0 // just after class creation
#define QRY_STATE_CONNECTED    1 // channel joined
#define QRY_STATE_DISCONNECTED 2 // server disconnected

class IRCQuery : public IRCCommon,CListener
{
public:
  int m_QueryID;

  char *m_OtherNick; // this is the nick of the user that we're chatting to.
  char *m_Hostmask; // the hostmask of the user we're chatting to.
  int m_Status; // see QRY_STATE_* defines.
  IRCServer *m_pServer;  
  char *m_DescriptionString;

  IRCQuery(IRCServer *Server,const char *OtherNick, const char *Hostmask);
  ~IRCQuery( void );

  void OutputFormatter(int Contents,int IRCCommandID);

  void ToggleQueryWindow( void );    // creates the window if needed.
  void ShowQueryWindow( void ); // creates the window if needed.

  char *GetDescriptionString( void );
  void UpdateStatus( void );
  void UpdateInfo( void );
  short GetStatus( void ) { return m_Status; }
  void SetStatus( int Status ) { m_Status = Status; }

  // the other user might change their name..
  void ChangeUserNick(char *OtherNick);

  // we need to know about server disconnects and reconnects.
  void OnConnect( void );           // updates the status
  void OnDisconnect( void );        // updates the status


  // from CListener
  void OnEvent(int EventID, void *pData);

private:
  void Initialise( void );           // Called by Constructor.  
  char *BuildDescriptionString( void );
  void UpdateLogger( void );
};
