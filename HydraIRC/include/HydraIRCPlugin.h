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
