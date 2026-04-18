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