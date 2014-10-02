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
