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

#define PLUGIN_API_CMDPROC 1

typedef BOOL (* PFN_PROCESSINPUT) (int ServerID, int ChannelID, char **Command);
typedef BOOL (* PFN_PROCESSCOMMAND) (int ServerID, int ChannelID, char **Command, char **Args);
typedef BOOL (* PFN_PROCESSCHANNELCOMMAND) (int ServerID, int ChannelID, char **Command, char **Args);
typedef BOOL (* PFN_PROCESSSERVERCOMMAND) (int ServerID, char **Command, char **Args);
typedef BOOL (* PFN_PROCESSDCCINPUT) (int ServerID, char **Command, char **Args);
typedef BOOL (* PFN_PROCESSQUERYINPUT) (int ServerID, char **Command, char **Args);

struct PluginCmdProcTable
{
  PFN_PROCESSINPUT m_pfnProcessInput;                   // *OPTIONAL*
  PFN_PROCESSCOMMAND m_pfnProcessCommand;               // *OPTIONAL*
  PFN_PROCESSCHANNELCOMMAND m_pfnProcessChannelCommand; // *OPTIONAL*
  PFN_PROCESSSERVERCOMMAND m_pfnProcessServerCommand;   // *OPTIONAL*
  PFN_PROCESSDCCINPUT m_pfnProcessDCCInput;							// *OPTIONAL*
  PFN_PROCESSQUERYINPUT m_pfnProcessQueryInput;					// *OPTIONAL*
};

// Note: these must be the same order as the function appear in the table.
enum PluginCmdProcTableOffsets
{
  PLUGIN_CMDPROC_PROCESSINPUT,
  PLUGIN_CMDPROC_PROCESSCOMMAND,
  PLUGIN_CMDPROC_PROCESSCHANNELCOMMAND,
  PLUGIN_CMDPROC_PROCESSSERVERCOMMAND,
  PLUGIN_CMDPROC_PROCESSDCCINPUT,
  PLUGIN_CMDPROC_PROCESSQUERYINPUT
};
