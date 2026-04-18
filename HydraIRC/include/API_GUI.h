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

#define PLUGIN_API_GUI 2

typedef BOOL (* PFN_ONCOMMANDMESSAGE) (WORD wID);

struct PluginGUITable
{
  PFN_ONCOMMANDMESSAGE m_pfnOnCommandMessage;
};

// Note: these must be the same order as the function appear in the table.
enum PluginGUITableOffsets
{
  PLUGIN_GUI_ONCOMMANDMESSAGE,
};
