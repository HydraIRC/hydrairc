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

// This file defines things that are common to the core *AND* the plugins

// BIC = Buffer Item Contents
#define BIC_UNKNOWN     0

// IRC Server/Channel Output Types
#define BIC_NOTICE      1
#define BIC_TOPIC       2
#define BIC_JOIN        3
#define BIC_KICK        4
#define BIC_MODE        5
#define BIC_NICK        6
#define BIC_PART        7
#define BIC_ACTION      8
#define BIC_QUIT        9
#define BIC_CONNECTING  10
#define BIC_CHANNELTEXT 11
#define BIC_QUERYTEXT   12
#define BIC_SERVERMSG   13
#define BIC_CTCP        14
#define BIC_OWNTEXT     15
#define BIC_WALLOPS     16

// HydraIRC output types
#define BIC_CONSOLEFIRST 100

#define BIC_FUNCTION     (BIC_CONSOLEFIRST + 0)
#define BIC_SOCKET       (BIC_CONSOLEFIRST + 1)
#define BIC_WARNING      (BIC_CONSOLEFIRST + 2)
#define BIC_ERROR        (BIC_CONSOLEFIRST + 3)
#define BIC_GUI          (BIC_CONSOLEFIRST + 4)
#define BIC_INFO         (BIC_CONSOLEFIRST + 5)
#define BIC_XML          (BIC_CONSOLEFIRST + 6)
#define BIC_CONSOLELAST  (BIC_CONSOLEFIRST + 7)

// Add more types here.

// Bitmask for HIRC_ChannelInfo_s.Mask to indicate members are valid
#define HIRC_CI_NAME          (1<<0)
#define HIRC_CI_TOPIC         (1<<1)

// Bitmask for HIRC_ServerInfo_s.Mask to indicate members are valid
#define HIRC_SI_NAME          (1<<0)
#define HIRC_SI_DESCRIPTION   (1<<1)
#define HIRC_SI_NICK          (1<<2)

// Child Window Types
#define CWTYPE_UNKNOWN 0
#define CWTYPE_SERVER  1
#define CWTYPE_CHANNEL 2
#define CWTYPE_DCCCHAT 3
#define CWTYPE_QUERY   4

// Object Types (for classes with an m_ObjectType member), currently only IRCCommon has this member.
#define OTYPE_DCCCHAT CWTYPE_DCCCHAT
#define OTYPE_SERVER CWTYPE_SERVER
#define OTYPE_CHANNEL CWTYPE_CHANNEL
#define OTYPE_QUERY CWTYPE_QUERY
#define OTYPE_MISC_FIRST OTYPE_QUERY
#define OTYPE_CHANNELWINDOWUSERLIST (OTYPE_MISC_FIRST+1)
#define OTYPE_GLOBALUSERLIST        (OTYPE_MISC_FIRST+2)


typedef struct HIRC_ChannelInfo_s
{
  int Mask;

  // these pointers are only valid directly after the call. if you want to use it later, copy the data!
  char *Name;
  char *Topic;
  // TODO: Add more things that plugins might need.
} HIRC_ChannelInfo_t;

typedef struct HIRC_ServerInfo_s
{
  int Mask;
  // these pointers are only valid directly after the call. if you want to use it later, copy the data!
  char *Name;
  char *Description;
  char *Nick;
  // TODO: Add more things that plugins might need.
} HIRC_ServerInfo_t;

typedef struct HIRC_WindiwInfo_s
{
  int WindowType; // see CWTYPE_*
  int WindowID;   // this will be the same as one of the items below
  int ServerID;
  int ChannelID;
  int QueryID;
  int DCCChatID;
  // TODO: Add more things that plugins might need.
} HIRC_WindowInfo_t;
