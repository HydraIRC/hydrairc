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

#pragma once

#define WMU_WHERE_ARE_YOU_MSG _T("WMU_HYDRAIRC_WHERE_ARE_YOU-{0E8530EE-FD38-4a8a-BB47-820B530D6EF7}")
const UINT WMU_WHERE_ARE_YOU = ::RegisterWindowMessage(_T("WMU_HYDRAIRC_WHERE_ARE_YOU_MSG"));

class SingleInstance
{
	DWORD  LastError;
	HANDLE hMutex;

public:
	SingleInstance(char* strMutexName)
	{
		// strMutexName must be unique
		hMutex = CreateMutex(NULL, FALSE, strMutexName);
		LastError = GetLastError();
	}

	~SingleInstance()
	{
		if(hMutex) {
			CloseHandle(hMutex);
			hMutex = NULL;
		}
	}

	BOOL IsAnotherInstanceRunning() { return (ERROR_ALREADY_EXISTS == LastError); }
};
