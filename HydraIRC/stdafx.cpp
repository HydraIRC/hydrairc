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

// stdafx.cpp : source file that includes just the standard includes
//	HydraIRC.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#if (_ATL_VER < 0x0700)
#include <atlimpl.cpp>
#endif //(_ATL_VER < 0x0700)

//#include<DockImpl.cpp>

// We've specified _TABBEDMDI_MESSAGES_EXTERN_REGISTER
// (see TabbedMDI.h)
#ifdef USE_TABBEDMDI
#if (_MSC_VER < 1300)
  RegisterTabbedMDIMessages g_RegisterTabbedMDIMessages;
#endif
#endif
