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
