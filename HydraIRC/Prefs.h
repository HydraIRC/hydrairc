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

// Prefs.h
//
/////////////////////////////////////////////////////////////////////////////

// forward declare.
class UserIdentity;
class Prefs;

enum GetPreferredIdentityFlags {
  GPIF_None = 0,
  GPIF_UseDefault = (1<<0),
  GPIF_UseFirst = (1<<1)
};

enum IgnoreInfoFlags 
{
  IIF_NONE                 = 0,
  IIF_DISABLED             = (1<<0), // i.e.  Don't ignore, this allows us to toggle them on/off
  IIF_IGNORECHANNELLIST    = (1<<1), // i.e.  Ignore the channel list
};

class IgnoreInfo
{
public:
  CSimpleArray< char * > m_ChannelNameList; // list of channels to ignore user in. (with #%! prefixes)
  char *m_Nick; // Only used for display purposes, no matching is done
  char *m_HostMask;
  char *m_Notes; // comment field
  int m_NetworkID;
  int m_Flags; // see IIF_* / IgnoreInfoFlags

  IgnoreInfo( )
  {
    Init();
  }

  void Init( void )
  {
    m_Nick = NULL;
    m_HostMask = NULL;
    m_Notes = NULL;
    m_NetworkID = NETWORK_ALL;    
    m_Flags = IIF_NONE;
  }

  IgnoreInfo( char *Nick, char *HostMask, char *Notes, int NetworkID, int Flags)
  {
    Init();
    SetString(m_Nick,Nick);
    SetString(m_HostMask,HostMask);
    SetString(m_Notes,Notes);
    m_NetworkID = NetworkID;
    m_Flags = Flags;

    // Generate a basic mask from the nick if no mask is specified
    if (!HostMask)
      m_HostMask = HydraIRC_BuildString(strlen(Nick)+3,"%s!*@*",Nick);
  }

  IgnoreInfo( IgnoreInfo *pOther)
  {
    Init();
    Copy(pOther);
  }

  void Copy( IgnoreInfo *pOther)
  {
    Clear();

    SetString(m_Nick,pOther->m_Nick);
    SetString(m_HostMask,pOther->m_HostMask);
    SetString(m_Notes,pOther->m_Notes);
    m_NetworkID = pOther->m_NetworkID;
    m_Flags = pOther->m_Flags;

    int i;
    for (i = 0; i < pOther->m_ChannelNameList.GetSize(); i++)
    {
      m_ChannelNameList.Add(strdup(pOther->m_ChannelNameList[i]));
    }
  }

  ~IgnoreInfo( )
  {
    Clear();
  }

  void Clear( void )
  {
    SetString(m_Nick,NULL);
    SetString(m_HostMask,NULL);
    SetString(m_Notes,NULL);
    m_NetworkID = NETWORK_ALL;
    m_Flags = IIF_NONE;

    char *ChannelName;

    while (m_ChannelNameList.GetSize() > 0)
    {
      ChannelName = m_ChannelNameList[0];
      m_ChannelNameList.RemoveAt(0);
      free(ChannelName);
    }
  }

  BOOL IgnoringChannel(char *ChannelName)
  {
    int i;
    for (i = 0 ; i < m_ChannelNameList.GetSize(); i++ )
    {
      if (stricmp(ChannelName,m_ChannelNameList[i]) == 0)
        return TRUE;
    }
    return FALSE;
  }

  BOOL AddChannel(char *ChannelName)
  {
    if (IgnoringChannel(ChannelName))
      return FALSE; // don't add twice!

    m_ChannelNameList.Add(strdup(ChannelName));
    return TRUE;
  }

  BOOL RemoveChannel(char *ChannelName)
  {
    int i;
    for (i = 0 ; i < m_ChannelNameList.GetSize(); i++ )
    {
      char *CurrentChannelName = m_ChannelNameList[i];

      if (stricmp(ChannelName,CurrentChannelName) == 0)
      {
        free(CurrentChannelName);
        m_ChannelNameList.RemoveAt(i);
        Validate();
        return TRUE;
      }
    }
    return FALSE;
  }

  int ChannelCount( void )
  {
    return m_ChannelNameList.GetSize();
  }

  void Validate( void ) // call after removing channels manually
  {
    if (ChannelCount() == 0) 
      m_Flags |= IIF_IGNORECHANNELLIST;
  }
};


/*  Prefs collections

 the default for channel prefs and the like is to inherit.
 when prefs are loaded from disk for a particular channel the
 m_InheritFromDefault is set to FALSE if the pref is found for a channel
 then, if the user changes the default prefs, all the ones that are inherited
 are updated from the defaults.
 also, if the user changes the actual channel prefs, the m_InheritFromDefault
 is set to FALSE.
 when saving the prefs for channels only the prefs with a FALSE
 m_InheritFromDefault are saved to the channel prefs file.

enum PrefType {
  PT_BOOL,
  PT_INT,
  PT_STRING,
  PT_PATH,
  PT_COLOR,
  PT_FONT
};

typedef struct BoolPrefItem_s
{
  int m_PrefID;
  BOOL m_Bool;
  BOOL m_InheritFromDefault; 
} BoolPrefItem_t;

typedef struct IntPrefItem_s
{
  int m_PrefID;
  int m_Int;
  int m_Max;
  BOOL m_InheritFromDefault;
} IntPrefItem_t;

typedef struct PathPrefItem_s
{
  int m_PrefID;
  char *m_Path;
  BOOL m_InheritFromDefault;
} PathPrefItem_t;

typedef struct StringPrefItem_s
{
  int m_PrefID;
  char *m_String;
  BOOL m_InheritFromDefault;
} StringPrefItem_t;

typedef struct ColorPrefItem_s
{
  int m_PrefID;
  COLORREF m_ColorRef;
  BOOL m_InheritFromDefault;
} ColorPrefItem_t;

typedef struct FontPrefItem_s
{
  int m_PrefID;
  char *m_Name;
  int m_Size;
  // int m_Script; // TODO: Add this!
  int m_Flags;
  BOOL m_InheritFromDefault;
} FontPrefItem_t;

*/

typedef struct BoolPrefItem_s
{
  int m_PrefID;
  BOOL m_Value;
} BoolPrefItem_t;

typedef struct IntPrefItem_s
{
  int m_PrefID;
  int m_Value;
  int m_Max; // TODO: currently ignored.
} IntPrefItem_t;

typedef struct StringPrefItem_s
{
  int m_PrefID;
  char *m_Value;
} StringPrefItem_t;

typedef struct ColorPrefItem_s
{
  int m_PrefID;
  COLORREF m_Value;
} ColorPrefItem_t;

typedef struct FontPrefItem_s
{
  int m_PrefID;
  char *m_Name;
  int m_Size;
  // int m_Script; // ZANN_FONTS
  int m_Flags;
} FontPrefItem_t;

/* // PrefsCollections

typedef struct PrefsCollection_s
{
  BOOL *m_pBools;
  BoolPrefItem_t *m_pBoolsTable;

  int *m_pInts;
  IntPrefItem_t *m_pIntsTable;

  char **m_pPaths;
  PathPrefItem_t *m_pPathsTable;

  char **m_pStrings;
  StringPrefItem_t *m_pStringsTable;

  FontPrefInfo_t *m_pFonts;
  FontPrefItem_t *m_pFontsTable;

  COLORREF *m_pColors;
  ColorPrefItem_t *m_pColorsTable;

  //unsigned char m_StaticFlags; // set a PCOL_SF_* bit to indicate the pointer to the array should NOT be freed.
} PrefsCollection_t;

// forward declare.
class Prefs;

class ChannelPrefs
{
public:
  char *m_ChannelName;
  int m_NetworkID;
  PrefsCollection_t m_PC;
  Prefs *m_pPrefs;

  ChannelPrefs( Prefs *pPrefs, char *ChannelName, int NetworkID );
  ChannelPrefs( Prefs *pPrefs, ChannelPrefs *pOther );
  ~ChannelPrefs( void );
  void Initialise( void );
};

*/

// easy access macros
#define INTPREF(x) (g_pPrefs->m_IntPrefs[x - PREF_INT_FIRST])
#define FONTPREF(x) (&g_pPrefs->m_FontPrefs[x - PREF_FONT_FIRST])
#define BOOLPREF(x) (g_pPrefs->m_BoolPrefs[x - PREF_BOOL_FIRST])
#define STRINGPREF(x) (g_pPrefs->m_StringPrefs[x - PREF_STRING_FIRST])
#define PATHPREF(x) (g_pPrefs->m_PathPrefs[x - PREF_PATH_FIRST])
#define COLORVAL(x) (g_pPrefs->m_ColorPrefs[x - PREF_COLOR_FIRST])
#define COLORPREF(x) RGBVALTOCOLORREF(COLORVAL(x)) // hehe, thank god for macro expansion.. :)

#define PREF_STRING_NAME(i) (g_PrefsStringIDNames[PREF_STRING_FIRST + i])
#define PREF_PATH_NAME(i) (g_PrefsStringIDNames[PREF_PATH_FIRST + i])
#define PREF_INT_NAME(i) (g_PrefsStringIDNames[PREF_INT_FIRST + i])
#define PREF_BOOL_NAME(i) (g_PrefsStringIDNames[PREF_BOOL_FIRST + i])
#define PREF_FONT_NAME(i) (g_PrefsStringIDNames[PREF_FONT_FIRST + i])
#define PREF_COLOR_NAME(i) (g_PrefsStringIDNames[PREF_COLOR_FIRST + i])
extern char *g_PrefsStringIDNames[];

class WindowPositions
{
public:
  int x;
  int y;
  int w;
  int h;

  WindowPositions( void )
  {
    x = 0;
    y = 0;
    w = 0;
    h = 0;
  }

  WindowPositions( WindowPositions *pOther)
  {
    Copy(pOther);
  }
  
  void Copy( WindowPositions *pOther)
  {
    x = pOther->x;
    y = pOther->y;
    w = pOther->w;
    h = pOther->h;
  }

  void Set(int nx,int ny,int nw,int nh)
  {
    x = nx;
    y = ny;
    w = nw;
    h = nh;
  }

};

/** Holds information for a window, 
 *
 * Colors, size and position.
 *
 * Each CChildFrame window has a m_pWindowPrefs which is either NULL if the
 * window has no prefs, or it points to a current instance of a WindowPrefs class
 * When prefs is updated, all the windowprefs class instances are duplicated to the new
 * Prefs class instance.  If the user clicks ok then then the g_pPrefs is updated
 * and points to the new updated instance of the Prefs class, then an EV_PREFSCHANGED
 * event is fired,  this causes all listeners to ask the new prefs instance
 * for their new copies of their WindowPrefs class instances, they then go about 
 * updating things based on the new prefs as usual.
 */
#define WPF_NONE          (0)
#define WPF_HAVEPOSITIONS (1<<0)
#define WPF_HAVECOLORS    (1<<1)

class WindowPrefs
{
public:
  int m_NetworkID; // defaults to NETWORK_UNKNOWN
  short m_WindowType; // see CWTYPE_* defines.
  char *m_WindowName; // NULL for servers, channel name for channels, nick for Query windows.
  // there are unused color items in here which are not relevant to child windows
  COLORREF m_ColorPrefs[PREF_COLOR_MAX];

  int m_Flags; // see WPF_* defines.

  WindowPositions m_WindowPositions;

  WindowPrefs(Prefs *pPrefs); // for initialising
  WindowPrefs(WindowPrefs *pOther); // for duplicating
  ~WindowPrefs( void );

};

/** Loads and saves global prefs and manages user profile data.
 *
 *  The prefs class stores user profile settings *only*
 *  other more global settings (like servers/networks) are
 *  loaded and saved by the prefs class, but not managed by it.
 */
class Prefs
{
public: 
  BOOL m_BoolPrefs[PREF_BOOL_MAX];
  int m_IntPrefs[PREF_INT_MAX];
  char *m_StringPrefs[PREF_STRING_MAX];
  char *m_PathPrefs[PREF_PATH_MAX];
  FontPrefItem_t m_FontPrefs[PREF_FONT_MAX];
  CSimpleArray<CCommandProfile *> m_CommandProfileList;
  CSimpleArray<WindowPrefs *> m_WindowPrefsList;

  CSimpleArray<IgnoreInfo *> m_IgnoreList;

  xmlNodePtr m_Favorites;
  COLORREF m_ColorPrefs[PREF_COLOR_MAX];

  
  // PrefsCollection_t m_PC; // PrefsCollections
  // CSimpleArray<ChannelPrefs *> m_ChannelPrefList; // PrefsCollections

  CSimpleArray<UserIdentity *> m_UserIdentitiesList;
  CSimpleArray<NotificationPrefInfo_t *> m_NotificationPrefList;

  IRCUserGroupList m_CustomUserGroups;

  // Init, Load and Save
  Prefs(void);          // init and load prefs
  Prefs(Prefs *pOther); // init from another instance
  ~Prefs(void);
  void Initialise( void );

  /* // PrefsCollections
  void InitPrefsCollection(PrefsCollection_t *pPC, PrefsCollection_t *pParent);
  */

  // loading
  void LoadPrefs( void );
  OutputThemeItem_t *LoadThemeByName(char *ThemeName);
  ColorPrefItem_t *XML_LoadColorScheme(xmlNodePtr pNode);
  void ConvertColorSchemeToPrefs(ColorPrefItem_t *pScheme, COLORREF *pColorPrefs);
  ColorPrefItem_t *LoadColorSchemeByName(char *SchemeName);

  // saving
  void SavePrefs( void );
  BOOL SaveTheme(char *ThemeName, char *FileName);

  // Utility
  WindowPrefs *GetWindowPrefs(int WindowType, int NetworkID, char *WindowName);
  BOOL DeleteWindowPrefs(int WindowType, int NetworkID, char *WindowName);

  // BOOL SetPref(PrefsCollection_t *pPC, char *pref, char *value); // PrefsCollections
  BOOL UpdateFontPrefItem(FontPrefItem_t *pFPI,char *FontPref);
  BOOL SetBooleanPref(char *value, BOOL *pref);
  BOOL SetIntPref(int& pref, char *value);
  BOOL SetPref(char *pref, char *value); 
  void UpdatePref(char *pref); // call to make HydraIRC take notice of the pref that was changed.

  UserIdentity *GetPreferredIdentity(int NetworkID, int Flags);
  UserIdentity *FindIdentityByDescripton(char *Description);
  UserIdentity *FindIdentityByID(int ID);
  int ObtainNewIdentityID( void );
  CCommandProfile *FindCommandProfile(char *Name);

  /* PrefsCollections
  BOOL GetBooleanPref(PrefsCollection_t *pPC, int PrefID);
  COLORREF GetColorPref(PrefsCollection_t *pPC, int PrefID);
  PrefsCollection_t *Prefs::GetChannelPrefsCollection(char *ChannelName, int NetworkID);
  */
private:
  // Init
  void Copy(Prefs *pOther);
  void SetupPrefs( void );

  // Utility
  
  // int GetPrefIndexFromID(PrefsCollection_t *pPC, int PrefType, int PrefID); // PrefsCollections

  // loading
  // FileName is optional, it loads from "Profile_<ProfileName>.xml" if omitted
  void LoadProfile( char *ProfileName, char *FileName = NULL );

  // these are global prefs
  void XML_LoadServerList(xmlNodePtr pNode,int DefaultFlags);
  void XML_LoadNetworkList(xmlNodePtr pNetworkNode,int DefaultFlags);
  void XML_SaveServerList(xmlNodePtr pParentNode,int Flags);
  void XML_SaveNetworkList(xmlNodePtr pParentNode,int Flags);

  OutputThemeItem_t *XML_LoadTheme(xmlNodePtr pNode);

  // user profile can override default for these..
  // void LoadMiscPrefs(PrefsCollection_t *pPC, xmlNodePtr pMiscNode); // PrefsCollections
  // void LoadColors(PrefsCollection_t *pPC, xmlNodePtr pNode); // PrefsCollections
  void XML_LoadMiscPrefs(xmlNodePtr pMiscNode); 
  void XML_LoadColors(xmlNodePtr pNode, COLORREF *pColorPrefs);
  void XML_LoadFonts(xmlNodePtr pFontNode);

  // these are user profile prefs
  void XML_LoadUserIdentities(xmlNodePtr pNode);
  BOOL XML_LoadNicks(xmlNodePtr pNode, UserIdentity *pUserIdentity);
  void XML_LoadBuddies(xmlNodePtr pNode, IRCUserGroup *pGroup);
  void XML_LoadBuddyGroups(xmlNodePtr pServerNode);
  void XML_LoadGeneralEventNotifications(xmlNodePtr pNode,int GroupID);
  void XML_LoadGeneralNotifications(xmlNodePtr pNode);
  void XML_LoadNotifications(xmlNodePtr pNode);
  void XML_LoadCommandProfiles(xmlNodePtr Node);
  void XML_LoadFavorites(xmlNodePtr pNode);
  void XML_LoadIgnores(xmlNodePtr pNode);
  void XML_LoadWindowPrefs(xmlNodePtr pNode);

  /* // PrefsCollections
  void LoadChannelPrefsCollections(xmlNodePtr pNode, ChannelPrefs *pChannelPrefs);
  void LoadChannelPrefs(xmlNodePtr pNode);
  */

  // saving
  // FileName is optional, it saves to "Profile_<ProfileName>.xml" if omitted
  void SaveProfile(char *ProfileName, char *FileName = NULL);

  // these are global prefs
  // TODO: add more here..
  void XML_SaveTheme(xmlNodePtr pParentNode, OutputThemeItem_t *pTheme);

  // user profile can override default for these..
  void XML_SaveFonts(xmlNodePtr pParentNode);
  void XML_SaveMiscPrefs(xmlNodePtr pParentNode);
  void XML_SaveColors(xmlNodePtr pParentNode, COLORREF *ColorPrefs);

  // these are user profile prefs
  void XML_SaveGeneralNotifications(xmlNodePtr pParentNode);
  void XML_SaveNotifications(xmlNodePtr pParentNode);
  void XML_SaveBuddyGroups(xmlNodePtr pParentNode);
  void XML_SaveIdentities(xmlNodePtr pParentNode);
  void XML_SaveCommandProfiles(xmlNodePtr pParentNode);
  void XML_SaveFavorites(xmlNodePtr pParentNode);
  void XML_SaveIgnores(xmlNodePtr pParentNode);
  void XML_SaveWindowPrefs(xmlNodePtr pParentNode);
};


// active settings are preferences that are not saved.

// easy access macros
//#define INTACTIVESETTING(x) (g_pActiveSettings->m_IntSettings[x])
//#define BOOLACTIVESETTING(x) (g_pActiveSettings->m_BoolSettings[x])
#define PATHACTIVESETTING(x) (g_pActiveSettings->m_PathSettings[x])

/*
  // boolean active setting are currently not used as of 0.3.090

enum boolactivesettings {
  //AS_bSomeSetting,
  BOOLAS_LAST
};

*/

enum pathactivesettings {
  AS_pLastDCCSend,
  PATHAS_LAST
};

/** ActiveSettings hold prefs items that are NOT saved on exit.
 *
 *  They are often initilised after a prefs load, and can be changed
 *  at any point during the programs execution.
 */
class ActiveSettings
{
public:
  //BOOL m_BoolSettings[BOOLAS_LAST];

  char *m_PathSettings[PATHAS_LAST];

  ActiveSettings( void )
  {
    ZeroMemory(m_PathSettings,sizeof(m_PathSettings));
  }

  ~ActiveSettings( void )
  {
    for (int i = 0; i < PATHAS_LAST; i ++)
      SetString(m_PathSettings[i],NULL);
  }
};

// theme stuff

class ThemeHandle
{
public:
  int m_RefCount;
  OutputThemeItem_t *m_ThemeItems;
  char *m_Name;

  ThemeHandle( void )
  {
    m_RefCount = 0;
  }

  ~ThemeHandle( void )
  {
    if (m_ThemeItems && m_ThemeItems != g_DefaultOutputTheme)
    {
      OutputThemeItem_t *pItem = m_ThemeItems;

      while (pItem->m_WindowType != -1)
      {
        if (pItem->m_FormatStr)
          free(pItem->m_FormatStr);
        pItem++;
      }

      free(m_ThemeItems);
    }
    if (m_Name)
      free(m_Name);
  }

  BOOL Dec( void )
  {
    m_RefCount--;

    if (m_RefCount == 0)
    {
      delete this;
      return TRUE; // if deleted object, return TRUE;
    }
#ifdef DEBUG
    if (m_RefCount < 0)
      ATLASSERT(0);
#endif
    return FALSE;
  }

  void Inc( void )
  {
    m_RefCount++;
  }

};

class ThemeManager
{
private:
  CSimpleArray<ThemeHandle*> m_ThemeList;

  ThemeHandle *FindTheme(char *ThemeName);
  ThemeHandle *LoadTheme(char *ThemeName);

public:
  ThemeHandle *GetThemeHandle(char *ThemeName)
  {
    // if it's not loaded, load it
    // otherwise, return a handle to it

    ThemeHandle *pTheme = FindTheme(ThemeName);

    if (!pTheme)
    {
      pTheme = LoadTheme(ThemeName);
    }

    if (!pTheme)
      return NULL;

    pTheme->Inc();

    return pTheme;
  }

  void ReleaseThemeHandle(ThemeHandle *pTheme)
  {
    // decrease the refcount, and if object was then deleted we remove it from out list too.
    if (pTheme->Dec())
      m_ThemeList.Remove(pTheme);
  }

  ~ThemeManager( void )
  {
    ThemeHandle *pTheme;
    while (m_ThemeList.GetSize() > 0) // this should always be 
    {
      pTheme = m_ThemeList[0];
      m_ThemeList.Remove(pTheme);
#ifdef DEBUG
      sys_Printf(BIC_INFO,"REFCOUNT for object at 0x%08x != 0 (%d)\n",pTheme,pTheme->m_RefCount);
      ATLASSERT(0);
#endif
      delete pTheme; // delete it anyway.
    }
  }
};
