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
