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

// PluginManager.cpp : definition of the CPluginManager class
//
/////////////////////////////////////////////////////////////////////////////

class CBasicPlugin : 
  public HydraIRCPlugin 
{
public:
  CBasicPlugin(void);
  //~CBasicPlugin( void) { };

  BOOL RequestAPIs( void );
  BOOL GetAPI(APIDescriptor_t *pAPID);
  BOOL OnRequestUnload( void );
};

class CPluginManager
{
public:
  CList m_Plugins;

  CPluginManager( void );
  ~CPluginManager( void );

  HydraIRCPlugin *CPluginManager::FindPluginByName(const char *PluginName);

  void LoadPlugin(const char *FileName ); // relative path+filename to g_HomeDir

  void RefreshPlugins( void );
  void ListPlugins( void );

  BOOL Unload( HydraIRCPlugin *pPlugin,const BOOL Force, const BOOL IgnoreInUse);
  int UnloadByName(const char *PluginName, const BOOL Force, const BOOL IgnoreInUse);
  BOOL RequestUnload( HydraIRCPlugin *pPlugin, const BOOL IgnoreInUse );

  BOOL InUse( HydraIRCPlugin *pPlugin);

  void Shutdown( void );
  BOOL RequestShutdown( void );

  BOOL Resolve(APIDescriptor_t *pAPID);
  FUNCPTR GetFunction(int API, int TableOffset,HydraIRCPlugin **pPlugin, CNode **pNode );

#ifdef DEBUG
  void *m_AllocTest1;
  void *m_AllocTest2;
#endif

private:
  // here are the plugins created by the core.
  CBasicPlugin *m_pCorePlugin;
};

// Export these functions to the core,  if it's defined in PluginManager.cpp
// and not listed here then the core doesn't use the function.
void HydraIRC_ActiveWindowPrintf(int Contents, const char *format, ...);
char *HydraIRC_BuildString(int MaxSize, const char *format, ...);
BOOL HydraIRC_NewServer(char *URI);