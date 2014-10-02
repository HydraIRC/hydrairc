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

// Utility.h
//
/////////////////////////////////////////////////////////////////////////////

void sys_Printf(int Contents, const char *format, ...);
int matchhostmask(char *mask, char *name);


// TreeItemInfo
//   used for storing information and pointers to other data when storing
//   hItem's in a treeview.  So that when you are given an hItem you can
//   find the information out quickly.
/////////////////////////////////////////////////////////////////////////////

// Tree Item Info Types (stored in the m_Type field)

// For ConnectDialog/Prefs
#define TII_NETWORK      1
#define TII_SERVER       2

// for UserList
#define TII_USERGROUP    10
#define TII_USER         11

// for ServerList
#define TII_IRCSERVER    20 // TODO: remove and use TII_SERVER
#define TII_CHANNELGROUP 21
#define TII_IRCCHANNEL   22 // TODO: rename to TII_CHANNEL for consistancy
#define TII_DCCCHATGROUP 23
#define TII_DCCCHAT      24
#define TII_QUERYGROUP   25
#define TII_QUERY        26

// for favorites
#define TII_GROUP        30

class TreeItemInfo
{ 
public:
  short m_Type; // see TII_* defines
  void *m_Data; // user data
  int m_Num;    // user data
  DWORD m_Flags; // user flags.

  // Note: unused member initialisers commented out for speed 
  // (we can often create hundreds of these, so we want to be
  // able to do it fast!)

  TreeItemInfo(short Type, void *Data, int Num, DWORD Flags)
  {
    m_Type = Type;
    m_Data = Data;
    m_Num = Num;
    m_Flags = Flags;
  }

  TreeItemInfo(short Type, void *Data, int Num)
  {
    m_Type = Type;
    m_Data = Data;
    m_Num = Num;
    //m_Flags = 0;
  }

  TreeItemInfo(short Type, void *Data)
  {
    m_Type = Type;
    m_Data = Data;
    //m_Num = 0;
    //m_Flags = 0;
  }

  TreeItemInfo(short Type, int Num)
  {
    m_Type = Type;
    //m_Data = NULL;
    m_Num = Num;
    //m_Flags = 0;
  }

  TreeItemInfo(short Type)
  {
    m_Type = Type;
    //m_Data = NULL;
    //m_Num = 0;
    //m_Flags = 0;
  }
}; 

// IRCUser
//   Holds basic information about an IRC user
// IRCUserList is a list of IRCUsers
// IRCUserGroup is a simple container (with support functions) for
//   an IRCUserList
// IRCUserGroupList is a simple container for IRCUserGroups
// 
// A channel has an IRCUserList member.
// A CUserListView has an IRCUserGroupList member.
/////////////////////////////////////////////////////////////////////////////

#define UG_ID_UNUSED -1
#define UG_ID_NORMAL 0
#define UG_ID_VOICE  1
#define UG_ID_HALFOP 2
#define UG_ID_OP     3
#define UG_ID_ADMIN  4
#define UG_ID_OWNER  5
#define UG_ID_CUSTOM 6

// Note: the icons in the userlist.bmp image appear in this order (with an offset)
// These must be in the order of privelidges.
#define USER_TYPE_NORMAL 0
#define USER_TYPE_VOICE  1
#define USER_TYPE_HALFOP 2
#define USER_TYPE_OP     3
#define USER_TYPE_ADMIN  4
#define USER_TYPE_OWNER  5

//#define IRCMODE(x) (1<<(int((x)-'a'+1))) //Generates a bitmask for a one character mode

#pragma warning(disable : 4293)
#define IRCMODE(x) (x >= 'a' ? (__int64(1)<<(int((x)-'a'+1))) : (__int64(1)<<(int((x)-'A'+27)))) //O.o
//#pragma warning(default : 4293) // doesn't work

#define IRCMODE_NORMAL (1<<0)
#define IRCMODE_VOICE  IRCMODE('v')
#define IRCMODE_HALFOP IRCMODE('h')
#define IRCMODE_OP     IRCMODE('o')
#define IRCMODE_ADMIN  IRCMODE('a')
#define IRCMODE_OWNER  IRCMODE('q')

// Holds the information about a user connected to an IRC server
// or a buddy.
class IRCUser
{
public:
  char *m_Nick;    // e.g. "Hydra"
  char *m_Address; // e.g. "user@host"
  int m_UserType;  // see USER_TYPE_* defines, defines which buddy group they're in, and
                   // what icon is used.
  __int64 m_UserModes; // Keeps a note of the IRC flags, as you can be +o AND +v
                   // see IRCMODE_* defines.

  int m_CustomGroupID; // not used when DEFINING buddies, only used by the userlists.

  IRCUser(char *Nick)
  {
    m_Address = NULL;
    m_Nick = strdup(Nick);
    m_UserType = -1;
    m_UserModes = IRCMODE_NORMAL;
    m_CustomGroupID = -1;
  }

  IRCUser( void )
  {
    m_Address = NULL;
    m_Nick = NULL;
    m_UserType = -1;
    m_UserModes = IRCMODE_NORMAL;
    m_CustomGroupID = -1;
  }

/*
  IRCUser(char *Nick, int UserType)
  {
    m_Address = NULL;
    m_Nick = strdup(Nick);
    m_UserType = UserType;    
    m_CustomGroupID = -1;
    SetModesFromType();
  }
*/
  IRCUser(char *Nick, __int64 UserModes)
  {
    m_Address = NULL;
    m_Nick = strdup(Nick);
    m_UserModes = UserModes;
    m_CustomGroupID = -1;
    SetTypeFromModes();
  }

/*
  IRCUser(char *Nick, char *Address, int UserType)
  {
    m_Address = strdup(Address);
    m_Nick = strdup(Nick);
    m_UserType = UserType;
    m_CustomGroupID = -1;
    SetModesFromType();
  }
*/

  IRCUser(char *Nick, char *Address, __int64 UserModes)
  {
    m_Address = strdup(Address);
    m_Nick = strdup(Nick);
    //m_UserType = UserType;
    m_UserModes = UserModes;
    m_CustomGroupID = -1;
    //SetModesFromType();
    SetTypeFromModes();
  }

  void Copy(IRCUser *pOther)
  {
    m_Address = strdup(pOther->m_Address);
    m_Nick = strdup(pOther->m_Nick);
    m_UserType = pOther->m_UserType;
    m_UserModes = pOther->m_UserModes;
    m_CustomGroupID = pOther->m_CustomGroupID;
  }

  // init from another IRCUser
  IRCUser(IRCUser *pOther)
  {
    Copy(pOther);
  }

  // DO NOT USE AFTER CALLING THIS.
  /*
  void Free( void )
  {
    if (m_Nick) free(m_Nick);
    if (m_Address) free(m_Address);
  }
  */

  ~IRCUser( void )
  {
#ifdef VERBOSE_DEBUG
    if (m_Nick) 
    {
      sys_Printf(BIC_FUNCTION,"Deleting IRCUser \"%s\"\n",m_Nick);
    }
#endif
    if (m_Nick) free(m_Nick);
    if (m_Address) free(m_Address);
    //Free();
  }

  BOOL AddressCompare(IRCUser *other)
  {
    ATLASSERT(other);
    return (BOOL)matchhostmask(m_Address,other->m_Address);
  }

  BOOL NickCompare(const char *Nick)
  {
    ATLASSERT(Nick);
    return (stricmp(m_Nick,Nick) == 0);
  }

  BOOL NickCompare(IRCUser *other)
  {
    ATLASSERT(other);
    return (NickCompare(other->m_Nick));
  }

  char *BuildMask( void )
  {
    return HydraIRC_BuildString(strlen(m_Nick)+1+strlen(m_Address)+1,"%s!%s",m_Nick,m_Address);
  }

  BOOL Compare(IRCUser *other)
  {
    ATLASSERT(other);
    return (BOOL) (matchhostmask(m_Nick,other->m_Nick) && matchhostmask(m_Address,other->m_Address));
    /*
    char *Mask = BuildMask();
    char *OtherMask = order->BuildMask();
    BOOL result (BOOL)hostmaskmatch(Mask,OtherMask);
    if (Mask) free(Mask);
    if (OtherMask) free(OtherMask);
    return result;
    */
  }

  // made this public
  // must be accessable when user mode is changed by Channel
  void SetTypeFromModes( void )
  {
    if(m_UserModes & IRCMODE_OWNER)
		  m_UserType = USER_TYPE_OWNER;
    else if(m_UserModes & IRCMODE_ADMIN)
      m_UserType = USER_TYPE_ADMIN;
    else if(m_UserModes & IRCMODE_OP)
      m_UserType = USER_TYPE_OP;
    else if(m_UserModes & IRCMODE_HALFOP)
      m_UserType = USER_TYPE_HALFOP;
    else if(m_UserModes & IRCMODE_VOICE)
      m_UserType = USER_TYPE_VOICE;
    else if(m_UserModes & IRCMODE_NORMAL)
      m_UserType = USER_TYPE_NORMAL;
  }

private:
/*
  void SetModesFromType( void )
  {
    switch(m_UserType)
    {
      case USER_TYPE_NORMAL:
        m_UserModes = IRCMODE_NORMAL;
        break;
      case USER_TYPE_VOICE:
        m_UserModes = IRCMODE_VOICE;
        break;
      case USER_TYPE_HALFOP:
        m_UserModes = IRCMODE_HALFOP;
        break;
      case USER_TYPE_OP:
        m_UserModes = IRCMODE_OP;
        break;
      case USER_TYPE_ADMIN:
        m_UserModes = IRCMODE_ADMIN;
        break;
    }
  }
*/


};

typedef CSimpleArray <IRCUser *> IRCUserList;

/** Simple container for IRCUser classes
 *
 *  Name and GroupID *must* be valid when the group is created and must remain valid.
 *  NetworkID is -1 if unused.
 */
class IRCUserGroup
{
public:
  char *m_GroupName;
  int m_GroupID; // see UG_ID_* defines.
  int m_NetworkID; // or -1 if unused.
  IRCUserList m_UserList;

  IRCUserGroup( void )
  {
    m_GroupName = NULL;
    m_GroupID = UG_ID_UNUSED;
    m_NetworkID = NETWORK_ALL;
  }

  IRCUserGroup(char *Name, int GroupID)
  {
    m_GroupName = strdup(Name);
    m_GroupID = GroupID;
    m_NetworkID = NETWORK_ALL;
    //m_Flags = IRCUG_DEFAULTFLAGS;
  }

  IRCUserGroup(char *Name, int GroupID, int NetworkID)
  {
    m_GroupName = strdup(Name);
    m_GroupID = GroupID;
    m_NetworkID = NetworkID;
    //m_Flags = IRCUG_DEFAULTFLAGS;
  }

  void RemoveAll( void )
  {
    if (m_UserList.GetSize() > 0)
    {
      m_UserList.RemoveAll();
    }
  }

  void DeleteAll( void )
  {
    while (m_UserList.GetSize() > 0)
    {
      delete m_UserList[0];
      m_UserList.RemoveAt(0);
    }
  }

  ~IRCUserGroup( void )
  {
#ifdef VERBOSE_DEBUG
    if (m_GroupName)
    {
      sys_Printf(BIC_FUNCTION,"Deleting IRCUserGroup \"%s\" ID: %d Network: %d\n",m_GroupName, m_GroupID, m_NetworkID);
    }
#endif
    DeleteAll();
    if (m_GroupName) free(m_GroupName);
  }

  void AddUser(IRCUser *pUser)
  {
    m_UserList.Add(pUser);
  }

  void RemoveUser(IRCUser *pUser)
  {
    m_UserList.Remove(pUser);
  }

  void DeleteUser(IRCUser *pUser)
  {
    delete pUser;
    m_UserList.Remove(pUser);
  }

  IRCUser *FindUser(char *Nick)
  {
    for (int i = 0 ; i < m_UserList.GetSize(); i ++)
    {
      if (m_UserList[i]->NickCompare(Nick))
        return m_UserList[i];
    }
    return NULL;
  }

  // Currently Unused.
  /*
  IRCUser *FindUser(IRCUser *pUser)
  {
    for (int i = 0 ; i < m_UserList.GetSize(); i ++)
    {
      if (m_UserList[i]->Compare(pUser))
        return m_UserList[i];
    }
    return NULL;
  }
  */
};

class IRCUserGroupList
{
public:
  CSimpleArray<IRCUserGroup *> m_UserGroupList;

  void Add(IRCUserGroup *pGroup)
  {
    ATLASSERT(pGroup);
    m_UserGroupList.Add(pGroup);
  }

  void Remove(IRCUserGroup *pGroup)
  {
    ATLASSERT(pGroup);
    m_UserGroupList.Remove(pGroup);
  }

  // remove and delete all the users from the groups and
  // remove and delete all the groups from the list
  void DeleteAll( void )
  {
    while (m_UserGroupList.GetSize() > 0)
    {
      m_UserGroupList[0]->DeleteAll();
      delete m_UserGroupList[0];
      m_UserGroupList.RemoveAt(0);
    }
  }

  // remove all users from the groups and
  // remove all the groups from the list (groups are not deleted!)
  // note: after a call to this it is safe to call
  // DeleteAll() *without* deleting the users themselves!
/*
  void RemoveAll( void )
  {
    while (m_UserGroupList.GetSize() > 0)
    {
      m_UserGroupList[0]->RemoveAll();
      m_UserGroupList.RemoveAt(0);
    }
  }
*/
  void RemoveAllUsers( void )
  {
    for (int i = 0 ; i < m_UserGroupList.GetSize() ; i++)
    {
      m_UserGroupList[i]->RemoveAll();
    }
  }

  IRCUserGroup *FindByGroupID(int ID)
  {
    IRCUserGroup *pGroup;
    for (int i = 0 ; i < m_UserGroupList.GetSize() ; i++)
    {
      pGroup = m_UserGroupList[i];
      if (pGroup->m_GroupID == ID)
        return pGroup;
    }
    return NULL;
  }

  IRCUserGroup *FindByGroupName(char *Name)
  {
    assert(Name);
    IRCUserGroup *pGroup;
    for (int i = 0 ; i < m_UserGroupList.GetSize() ; i++)
    {
      pGroup = m_UserGroupList[i];
      if (stricmp(pGroup->m_GroupName,Name) == 0)
        return pGroup;
    }
    return NULL;
  }

  IRCUserGroup *FindByGroupName(char *Name, int ID)
  {
    ATLASSERT(Name);
    IRCUserGroup *pGroup;
    for (int i = 0 ; i < m_UserGroupList.GetSize() ; i++)
    {
      pGroup = m_UserGroupList[i];
      if ((pGroup->m_GroupID == ID) && (stricmp(pGroup->m_GroupName,Name) == 0))
        return pGroup;
    }
    return NULL;
  }

  int FindUser(const char *Nick, int NetworkID)
  {
    IRCUserGroup *pGroup;
    IRCUser *pBuddy;
    for (int i = 0 ; i < m_UserGroupList.GetSize() ; i ++ )
    {
      pGroup = m_UserGroupList[i];
      if (NetworkID == pGroup->m_NetworkID || pGroup->m_NetworkID == -1) // -1 = cross-network group
      {
        for (int j = 0 ; j < pGroup->m_UserList.GetSize() ; j ++)
        {
          pBuddy = pGroup->m_UserList[j];

          if (pBuddy->NickCompare(Nick))
          {
            return pGroup->m_GroupID;
          }
        }
      }
    }
    return -1;
  }

  IRCUserGroup *FindUserGroup(const char *Nick,int NetworkID)
  {
    IRCUserGroup *pGroup;
    IRCUser *pBuddy;
    // we make two passes of the group list
    // the first time we match against custom groups
    // the second time we match against global groups too.
    // thus giving custom network specific groups a higer priority
    for (int pass = 1 ; pass <= 2 ; pass ++)
    {
      for (int i = 0 ; i < m_UserGroupList.GetSize() ; i ++ )
      {
        pGroup = m_UserGroupList[i];
        if (NetworkID == pGroup->m_NetworkID || ( pass == 2 && pGroup->m_NetworkID == -1)) // -1 = cross-network group
        {
          for (int j = 0 ; j < pGroup->m_UserList.GetSize() ; j ++)
          {
            pBuddy = pGroup->m_UserList[j];

            if (pBuddy->NickCompare(Nick))
            {
#ifdef VERBOSE_DEBUG
              sys_Printf(BIC_INFO,"Matched \"%s\" networkid: \"%d\" against group \"%s\" networkid: %d groupid: %d\n",Nick, NetworkID, pGroup->m_GroupName, pGroup->m_NetworkID, pGroup->m_GroupID);
#endif
              return pGroup;
            }
          }
        }
      }
    }
    return NULL;
  }

  IRCUserGroup *ExportAll(IRCUserGroup *pExportGroup)
  {
    if (!pExportGroup)
      pExportGroup = new IRCUserGroup();

    if (!pExportGroup)
      return NULL;

    IRCUserGroup *pGroup;
    IRCUser *pBuddy;
    for (int i = 0 ; i < m_UserGroupList.GetSize() ; i ++ )
    {
      pGroup = m_UserGroupList[i];
      for (int j = 0 ; j < pGroup->m_UserList.GetSize() ; j ++)
      {
        pBuddy = pGroup->m_UserList[j];

        pExportGroup->AddUser(pBuddy);
      }
    }
    return pExportGroup;
  }

};

/** History manager
 *
 *  manages lines of text in a history buffer.
 *  it handles multiple calls to GetPreviousHistoryItem
 *  and GetNextHistoryItem and returns NULL if you're
 *  at the beginning or end of the list regardless of 
 *  how many times the user presses the up or down arrow
 *  (thus calling GetPreviousHistoryItem
 *  or GetNextHistoryItem lots of times).
 *
 *  repeat adds of the same item are ignored.
 *  but (correctly) adds of items that appear further
 *  back in the history are allowed.
 *
 *  Additionally you'll never get the same line of text
 *  if you alternate calls to GetPreviousHistoryItem
 *  and GetNextHistoryItem)
 *
 *  So basically, if you get a NULL back, you should make a
 *  "beep" noise, if not, display the text.
 */
class InputHistoryManager
{
private:
  int m_MaxLines;
  int m_CurrentHistoryItem;
  CSimpleArray<char *> m_HistoryBuffer;

public:
  InputHistoryManager( void )
  {
    m_MaxLines = 25;
    m_CurrentHistoryItem = 0;
  }

  ~InputHistoryManager( void )
  {
    m_MaxLines = 0;
    TrimBuffer();
  }


  void TrimBuffer( void )
  {
    while (m_HistoryBuffer.GetSize() > m_MaxLines)
    {
      free (m_HistoryBuffer[0]);
      m_HistoryBuffer.RemoveAt(0);
    }
    // only reset it if we trimmed the ones the current histort pointer was point to.
    if (m_CurrentHistoryItem >= m_HistoryBuffer.GetSize())
      ResetHistoryItemPointer();
  }

  // FIFO
  BOOL Add(char *Text)
  {
    // don't add the same item.
    int size = m_HistoryBuffer.GetSize();
    if (size > 0 && strcmp(m_HistoryBuffer[size-1],Text) == 0)
    {
      //ResetHistoryItemPointer();
      return FALSE;
    }

    char *copy = strdup(Text);
    if (!copy)
      return FALSE;

    m_HistoryBuffer.Add(copy);
    TrimBuffer();
    return TRUE;
  }

  void SetMaxLines( int MaxLines )
  {
    m_MaxLines = MaxLines;
    TrimBuffer();
  }

  BOOL IsFirst( void )
  {
    return (m_CurrentHistoryItem == m_HistoryBuffer.GetSize());
  }

  BOOL IsLast( void )
  {
    return (m_CurrentHistoryItem == 0);
  }

  void Prev( void )
  {
    m_CurrentHistoryItem++;
    if (m_CurrentHistoryItem >= m_HistoryBuffer.GetSize())
    {
      m_CurrentHistoryItem = m_HistoryBuffer.GetSize();
    }
  }

  void Next( void )
  {
    m_CurrentHistoryItem--;

    if (m_CurrentHistoryItem < 0)
    {
      m_CurrentHistoryItem = 0;
    }
  }

  char *GetPreviousHistoryItem( void )
  {
    m_CurrentHistoryItem++;
    if (m_CurrentHistoryItem >= m_HistoryBuffer.GetSize())
    {
      m_CurrentHistoryItem = m_HistoryBuffer.GetSize();
      return NULL;
    }

    return m_HistoryBuffer[m_CurrentHistoryItem];
  }

  char *GetNextHistoryItem( void )
  {
    m_CurrentHistoryItem--;

    if (m_CurrentHistoryItem < 0)
    {
      m_CurrentHistoryItem = 0;
      return NULL;
    }
    return m_HistoryBuffer[m_CurrentHistoryItem];
  }

  char *GetCurrentHistoryItem( void )
  {    
    if (m_CurrentHistoryItem >= 0 && m_CurrentHistoryItem < m_HistoryBuffer.GetSize())
      return m_HistoryBuffer[m_CurrentHistoryItem];
    else
      return NULL;
  }

  void ResetHistoryItemPointer( void )
  {
    m_CurrentHistoryItem = m_HistoryBuffer.GetSize();
  }
};

/* Example history manager test, simulating history item navigation.
 * each call to Get* is corresponds to an up or down arrow key..

  InputHistoryManager Test;
  char *tmpstr;

  
  Test.Add("1");
  Test.Add("2");
  Test.Add("3");
  Test.Add("4");
  Test.Add("4"); // doesn't get added, it matched the last item
  Test.Add("5");
  Test.SetMaxLines(3);

  tmpstr = Test.GetNextHistoryItem(); // returns "5"
  tmpstr = Test.GetNextHistoryItem(); // returns "4"
  tmpstr = Test.GetNextHistoryItem(); // returns "3"
  tmpstr = Test.GetNextHistoryItem(); // returns NULL

  Test.ResetHistoryItemPointer();
  tmpstr = Test.GetNextHistoryItem();     // returns "5"
  tmpstr = Test.GetPreviousHistoryItem(); // returns NULL
  tmpstr = Test.GetNextHistoryItem();     // returns "5"

  Test.Add("6");
  tmpstr = Test.GetNextHistoryItem();     // returns "6"
  tmpstr = Test.GetNextHistoryItem();     // returns "5"
  tmpstr = Test.GetNextHistoryItem();     // returns "4"
  tmpstr = Test.GetNextHistoryItem();     // returns NULL
  tmpstr = Test.GetPreviousHistoryItem(); // returns "5"
  tmpstr = Test.GetNextHistoryItem();     // returns "4"
  tmpstr = Test.GetPreviousHistoryItem(); // returns "5"
  tmpstr = Test.GetPreviousHistoryItem(); // returns "6"
  tmpstr = Test.GetPreviousHistoryItem(); // returns NULL
  tmpstr = Test.GetPreviousHistoryItem(); // returns NULL
  tmpstr = Test.GetNextHistoryItem();     // returns "6"
  tmpstr = Test.GetNextHistoryItem();     // returns "5" 
  tmpstr = Test.GetNextHistoryItem();     // returns "4"
  tmpstr = Test.GetNextHistoryItem();     // returns NULL
*/

class BufferItem
{
public:
  // TODO: use the high 16 bits for flags
  int m_Contents; // see BIC_* defines
  time_t m_Time;  
  char *m_Buffer;

  BufferItem( int Contents, char *Buffer)
  {
    m_Contents = Contents;
    m_Buffer = Buffer;
    time(&m_Time);
  }

  ~BufferItem( void )
  {
    if (m_Buffer) free(m_Buffer);
  }

};

/*
  CCommandProfile

  Holds a name and a null terminated list of \n (or \r) seperated commands.

  Example usage:

  CCommandProfile *pProfile = new CCommandProfile("Test1",
    "\n\n\nCommand 1\n\r\n\r\rCommand 2\n\rA Long Command (3)\r\nCommand 4\rCommand 5\n\n\n");

  char *Command = pCommandProfile->GetFirstCommand();
  while (Command)
  {
    printf("\"%s\"\n",Command);
    Command = pCommandProfile->GetNextCommand();
  };
  delete pCommandProfile;

  This will print out the following:

  "Command 1"
  "Command 2"
  "A Long Command (3)"
  "Command 4"
  "Command 5"

  Note that all \r's and \n's are stripped.

*/
class CCommandProfile
{
protected:
  char *m_Handle;
  char *m_Position;

public:
  char *m_Commands;
  char *m_Name;

  CCommandProfile( void )
  {
    m_Name = NULL;
    m_Commands = NULL;
    m_Handle = NULL;
    m_Position = NULL;
  }

  CCommandProfile( char *Name, char *Commands)
  {
    m_Name = strdup(Name);
    m_Commands = strdup(Commands);
    m_Handle = NULL;
    m_Position = NULL;
  }

  ~CCommandProfile( void )
  {
    if (m_Name) free(m_Name);
    if (m_Commands) free(m_Commands);
    if (m_Handle) free(m_Handle);
  }

  CCommandProfile( CCommandProfile *pOther)
  {
    m_Name = NULL;
    m_Commands = NULL;
    m_Handle = NULL;
    m_Position = NULL;
    Copy(pOther);
  }

  void Clear( void )
  {
    if (m_Commands) free(m_Commands);
    m_Commands = NULL;
    if (m_Name) free(m_Name);
    m_Name = NULL;
    if (m_Handle) free(m_Handle);
    m_Handle = NULL;
    m_Position = NULL;
  }

  void Copy(CCommandProfile *pOther)
  {
    Clear();
    m_Commands = strdup(pOther->m_Commands);
    m_Name = strdup(pOther->m_Name);
  }

  char *GetFirstCommand( void )
  {
    if (m_Handle) free(m_Handle);
    m_Handle = NULL;
    if (!m_Commands)
      return NULL;

    m_Handle = strdup(m_Commands);
    m_Position = m_Handle;
    return GetNextCommand();
  }

  char *GetNextCommand( void )
  {
    if (!m_Handle)
      return NULL; // user must call GetFirstCommand first!

    // skip adjacent \n's and \r's
    while (*m_Position && (*m_Position == '\n' || *m_Position == '\r'))
    {
      m_Position++;
    }


    char *Start = m_Position;

    while (*m_Position && *m_Position != '\n' && *m_Position != '\r')
    {
      m_Position++;
    }

    if (*m_Position)
      *m_Position++ = 0; // null terminate Start string, and move the position forward if we're not at the end of the string

    if (Start == m_Position)
    {
      // no more commands!
      GarbageCollect();

      return NULL;
    }
    return Start;
  }

  // you can call this to save memory after calling GetFirstCommand() if say, you one want to retreive the first
  // couple commands
  void GarbageCollect( void )
  {
    if (m_Handle) free(m_Handle);
    m_Handle = NULL;
    m_Position = NULL;
  }
};

class CListener;

class CTimerCommand {

public:
  int m_Delay;
  char *m_CommandProfileName;
  CListener *m_pListener;

public:
  CTimerCommand(int Delay, char *CommandProfileName, CListener *pListener) {
    m_Delay = Delay;
    m_CommandProfileName = strdup(CommandProfileName);
    m_pListener = pListener;
  }

  ~CTimerCommand() {
    if (m_CommandProfileName)
      free(m_CommandProfileName);
  }
};


// Functions defined in Utility.cpp
int ReplaceString(char **buf,const char *s1,const char *s2); // replace all occurences of s1 in buf with s2, reallocating as needed
int GetTokens(char *buffer,char **buffers,int numbuffers, char **extra);
void FreeBuffers(char **buffers, int buffercount);
char *AliasProcessor(char *Input,char *Alias, int InputTokenBuffers, char EscapeCharacter ,char **SubsitutionTable);
char *GetFilePart(char *FullPathAndFileName); // return pointer to filename part of a path + filename string, or NULL
DWORD GetInternetIP( void );
char *BuildSizeString(char *Buffer, int BufferSize, double Number);
void OpenDirectoryForFile( char *PathAndFileName );

#define FILTER_EXCLUDE  1
#define FILTER_INCLUDE  2
#define FILTER_END      -1
int *CreateSimpleFilter(intarray& IA,int FilterType); // filter type should be FILTER_EXCLUDE or FILTER_INCLUDE
BOOL ProcessSimpleFilter(int *FilterList,int Value);

////////////////

// utility functions for lookup tables.

typedef struct IDPair_s
{
  int m_ID1,m_ID2;
} IntPair_t;

void SetLookupID(IntPair_t *pTable, int ItemCount, int PrefID, int ObjectID);
int GetLookupID(IntPair_t *pTable, int ItemCount, int PrefID);

// make sure your output array is big enough to hold all the ouput groups from your regexp
// e.g.  1x char* for each {} pair in the regexp
int RegExpToArray(const char *Data, const char *RegExp, char **Output, BOOL CaseSensitive = TRUE);

char *EscapePercentCharacters(char *dest,const char *source,int destbuffersize);
char *PickRandomString(char **strings,int count);

// if the destination is set, it is freed, then a copy of Value is taken
void SetString(char* &Dest, char *Value);
char *GetPathPart(char *FullPathAndFileName);
BOOL Clipboard_Copy(int format, char *data, int datalength);
char *GetIPv4AddressString(DWORD Address);
LRESULT OnEnLink(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
char *ReplaceInvalidFilenameCharacters(char *FileName,char Replace);
