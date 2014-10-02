/*

  HydraIRC - plugin link-library

  TODO: Stick LGPL license here, for now we're using the HydraIRC Limited License

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

#ifdef PLUGIN_EXPORTS
#define PLUGIN_API __declspec(dllexport)
#else
#define PLUGIN_API __declspec(dllimport)
#endif

#define PLUGIN_REQUIRES 0
#define PLUGIN_PROVIDES 1

typedef struct APIDescriptor_s
{
  int m_API;
  void *m_pTable;
  int m_Size; // size of table.
  char *m_Name;
  int m_Type; // Either PLUGIN_PROVIDES or PLUGIN_REQUIRES
  BOOL m_TableInitDone;
} APIDescriptor_t;


class HydraIRCPlugin : 
  public CNode
{
public:
  BOOL m_Active; 
  char *m_Name; // only use alpha-numeric, -'s and _'s and NO SPACES
  char *m_Author;
  char *m_Info;

  // these are all filled in my HydraIRC, not the plugin itself, hands off!
  BOOL m_Resolved;
  char *m_FileName;
  CList m_APIList;
  HMODULE m_pDLL;

  // For testing core/dll memory allocation/deallocation
  void *m_AllocTest1, *m_AllocTest2;

  // these three are in the lib
  HydraIRCPlugin( void );
  ~HydraIRCPlugin( void );
  void Init( void );

  virtual BOOL RequestAPIs( void ) = 0; // You *must* implement this in your plugin, even if you don't use any API's (#@!?)

  // this one's in the lib
  BOOL AddAPI(int API, char *Name, int TableSize, int Type = PLUGIN_PROVIDES, void *pTable = NULL);

  // override if you need to do initialisation/destruction in your plugin
  virtual void OnActivate( void ) {};
  // NOTE: Do not use function calls from other plugins API's in this function, espescialy if the other API
  // is provided by another DLL plugin.
  virtual void OnDeactivate( void ) {}; 

  // override if your plugin needs to be able to cancel an unload/quit request (e.g. it's busy doing something)
  // note: don't actually do anything other than a) prompt the user and b) return true or false
  // all cleanup would should be done in OnDeactivate()
  // e.g.:  return(Busy ? AskUser("Is it ok to unload this plugin, even though it's busy Y/N") : TRUE)
  virtual BOOL OnRequestUnload( void ) { return TRUE; }; // return FALSE to cancel the request.

  // you must override this if your plugin PROVIDES an API
  // basically, fill out the function table pointed to by pAPID->m_pTable with
  // the respective function pointers from your plugin.
  virtual BOOL GetAPI(APIDescriptor_t *pAPID)  { return FALSE; };
};

#define NAME_HYDRAIRCPLUGIN_INIT "HydraIRCPlugin_Init"

typedef HydraIRCPlugin* (WINAPI *HYDRAIRCPLUGIN_DLL_INIT)(void);
