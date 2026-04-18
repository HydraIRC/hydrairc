/*

  HydraIRC
  Copyright (C) 2002-2026 Dominic Clifton aka Hydra

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
