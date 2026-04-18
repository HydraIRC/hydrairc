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

#define VERSIONNUM "v0.3.169.1"
#define VERSIONDATE "(18/April/2026)"

#ifdef RELEASE_VERSION
#define VERSIONSTRING_EXTRA_1 ""
#else
#define VERSIONSTRING_EXTRA_1 "-Test"
#endif

#ifdef USE_XPCOMMANDBAR
#define VERSIONSTRING_EXTRA_2 "-XP"
#else
#define VERSIONSTRING_EXTRA_2 ""
#endif

#ifdef DEBUG
#define VERSIONSTRING_EXTRA_3 "-Debug"
#else
#define VERSIONSTRING_EXTRA_3 ""
#endif

#define VERSIONSTRING VERSIONNUM VERSIONSTRING_EXTRA_1 VERSIONSTRING_EXTRA_2 VERSIONSTRING_EXTRA_3 " " VERSIONDATE

/*
#ifndef RELEASE_VERSION
#ifndef USE_XPCOMMANDBAR
#define VERSIONSTRING VERSIONNUM "-Test " VERSIONDATE
#else
#define VERSIONSTRING VERSIONNUM "-Test-XP " VERSIONDATE
#endif
#else
#ifndef USE_XPCOMMANDBAR
#define VERSIONSTRING VERSIONNUM " " VERSIONDATE
#else
#define VERSIONSTRING VERSIONNUM "-XP " VERSIONDATE
#endif
#endif
*/

#define AUTHORSTRING "Dominic Clifton aka Hydra"
