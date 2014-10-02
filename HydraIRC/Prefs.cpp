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

// Prefs.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

// enables saving all all preferences, even if they're the same as the defaults
//#define COMPLETE_PREFS

// enables extra debug output when loading/saving prefs.
//#define PREF_DEBUG

// Prefs Items
//
/////////////////////////////////////////////////////////////////////////////

BoolPrefItem_t g_MainPrefs_Bools[PREF_BOOL_MAX+1] = // we don't need to specify the size really but it can help to track problems +1 is for list terminator
{
  {PREF_bPromptOnServerClose,                    TRUE},
  {PREF_bPromptOnDisconnect,                     TRUE},
  {PREF_bShowChannelListDialogOnConnect,         TRUE},
  {PREF_bShowChannelWindowUserListPaneByDefault, TRUE},
  {PREF_bShowChannelWindowHeaderPaneByDefault,   TRUE},
  {PREF_bShowChannelWindowInfoPaneByDefault,     TRUE},
  {PREF_bShowServerWindowUserListPaneByDefault,  FALSE},
  {PREF_bShowServerWindowHeaderPaneByDefault,    TRUE},
  {PREF_bShowServerWindowInfoPaneByDefault,      TRUE},
  {PREF_bShowQueryWindowUserListPaneByDefault,   TRUE},
  {PREF_bShowQueryWindowHeaderPaneByDefault,     FALSE},
  {PREF_bShowQueryWindowInfoPaneByDefault,       TRUE},
  {PREF_bShowDCCChatWindowUserListPaneByDefault, TRUE},
  {PREF_bShowDCCChatWindowHeaderPaneByDefault,   TRUE},
  {PREF_bShowDCCChatWindowInfoPaneByDefault,     TRUE},
  {PREF_bMonitorChannelsOnJoin,               TRUE},
  {PREF_bMonitorServersByDefault,             TRUE},
  {PREF_bShowChannelMonitorMessageOnStatusBar,TRUE},
  {PREF_bRememberAudioMute,                   FALSE},
  {PREF_bAudioMute,                           FALSE},
  {PREF_bAutoTile,                            FALSE},
  {PREF_bAutoMaximize,                        FALSE},
  {PREF_bHideNotMinimize,                     FALSE},
  {PREF_bRemoveOldPrivmsgLogItems,            TRUE},
  {PREF_bRemoveOldNoticeLogItems,             TRUE},
  {PREF_bRemoveOldURLCatcherItems,            TRUE},
  {PREF_bRemoveOldEventLogItems,              TRUE},
  {PREF_bChannelMemberNoticesOnly,            TRUE},

  {PREF_bOutputWindowTimestamps,              TRUE},
  {PREF_bChannelMonitorTimestamps,            TRUE},
  {PREF_bServerMonitorTimestamps,             TRUE},

  {PREF_bServerWindowTimestamps,              TRUE},
  {PREF_bChannelWindowTimestamps,             TRUE},
  {PREF_bQueryWindowTimestamps,               TRUE},
  {PREF_bDCCChatWindowTimestamps,             TRUE},

  {PREF_bShowUserListGroupCounts,             TRUE},
  {PREF_bSendKeepAliveMessages,               TRUE},
  {PREF_bShowPings,                           FALSE},
  {PREF_bShowPongs,                           FALSE},
  {PREF_bShowMOTD,                            TRUE},
  {PREF_bIgnoreCTCP,                          FALSE},
  {PREF_bConsoleLogging,                      FALSE},
  {PREF_bSocketLogging,                       FALSE},
  {PREF_bCrossNetworkNoticeForwarding,        TRUE},
  {PREF_bMinimizeToSysTray,                   FALSE},
  {PREF_bCreateQueryWindowsAutomatically,     TRUE},
  {PREF_bCreateQueryWindowsHidden,            FALSE},
  {PREF_bRestrictQueriesToQueryWindows,       TRUE},
  {PREF_bRestrictNoticesToQueryWindows,       TRUE},
  {PREF_bForwardNotices,                      TRUE},
  {PREF_bForwardMessages,                     TRUE},
  {PREF_bForwardCTCPs,                        TRUE},
  {PREF_bForwardServerNotices,                TRUE},
  {PREF_bDCCFastSends,                        TRUE},
  {PREF_bDCCVerboseTransfers,                 FALSE},
  {PREF_bDCCReceiveDoubleClickOpensFile,      FALSE},
  {PREF_bDCCAutoAccept,                       TRUE},
  {PREF_bCreateFoldersForDCCTransfers,        FALSE},
  {PREF_bIdentdEnabled,                       TRUE},
  {PREF_bSelectiveFloodControl,               FALSE},
  {PREF_bStrictChannelCharacters,             FALSE},
  {PREF_bServerListToolTips,                  TRUE},
  {PREF_bServerListChannelGroups,             FALSE},
  {PREF_bLogging,                             FALSE},
  {PREF_bCreateNetworkLogFolder,              TRUE},
  {PREF_bServerLogging,                       TRUE},
  {PREF_bChannelLogging,                      TRUE},
  {PREF_bQueryLogging,                        TRUE},
  {PREF_bDCCChatLogging,                      TRUE},
  {PREF_bStripCodesFromServerLogs,            TRUE},
  {PREF_bStripCodesFromChannelLogs,           TRUE},
  {PREF_bStripCodesFromQueryLogs,             TRUE},
  {PREF_bStripCodesFromDCCChatLogs,           TRUE},
  {PREF_bStripCodesFromServerWindows,         FALSE},
  {PREF_bStripCodesFromChannelWindows,        FALSE},
  {PREF_bStripCodesFromQueryWindows,          FALSE},
  {PREF_bStripCodesFromDCCChatWindows,        FALSE},
  {PREF_bShowPrivateMessagesOnStatusBar,      TRUE},
  {PREF_bCTCPUseERRMSGReply,                  TRUE},
  {PREF_bRejoinChannelOnKick,                 TRUE},
  {PREF_bUseGradientFilledPanes,              TRUE},
  {PREF_bClearDirtyFlagOnDeactivate,          TRUE},
  {PREF_bShowNickPrefixes,                    TRUE},
  {PREF_bShowBuddyGroupsInUserList,           TRUE},
  {PREF_bActivateLinkOnDoubleClick,           FALSE},
  {PREF_END_OF_LIST,0}
};

IntPrefItem_t g_MainPrefs_Ints[PREF_INT_MAX+1] =  // we don't need to specify the size really but it can help to track problems +1 is for list terminator
{
  {PREF_nDefaultUserListWidth,              125,    ~0L},
  {PREF_nDefaultInfoHeight,                 100,    ~0L},
  {PREF_nMaxHeaderLines,                    3,      ~15L},
  {PREF_nMaxScrollBufferLines,              500,    ~0L},
  {PREF_nMaxPrivMsgLogAgeMins,              4320,   ~0L},
  {PREF_nMaxNoticeLogAgeMins,               4320,   ~0L},
  {PREF_nMaxURLCatcherAgeMins,              4320,   ~0L},
  {PREF_nMaxEventLogAgeMins,                4320,   ~0L},
  {PREF_nMaxPrivMsgLogItems,                300,    ~0L},
  {PREF_nMaxNoticeLogItems,                 300,    ~0L},
  {PREF_nMaxURLCatcherItems,                300,    ~0L},
  {PREF_nMaxEventLogItems,                  300,    ~0L},
  {PREF_nReconnectRetries1,                 10,     ~0L},
  {PREF_nReconnectDelay1,                   30,     ~0L},
  {PREF_nReconnectRetries2,                 100,    ~0L},
  {PREF_nReconnectDelay2,                   300,    ~0L},
  {PREF_nKeepAliveDelay,                    60,     ~0L},
  {PREF_nTransferViewUpdateFrequency,       1,      ~0L},
  {PREF_nInactivityScrollDelay,             60,     ~0L},
  {PREF_nScrollLockTimeout,                 60,     ~0L},
  {PREF_nFloodControlThreshold,             5,      15L},
  {PREF_nFloodControlItemsPerSecond,        2,      10L},
  {PREF_nQueryNotifyReactivateDelay,        300,    ~0L},
  {PREF_nDCCTransferTimeout,                300,    ~0L},
  {PREF_nDCCResumeTimeout,                  30,     ~0L},
  {PREF_nDCCNegotiateTimeout,               180,    ~0L},
  {PREF_nDCCSendFirstPort,                  3072,   ~65535L},
  {PREF_nDCCSendLastPort,                   4096,   ~65535L},
  {PREF_nDCCSendBufferSize,                 1024,   8192L},
  {PREF_nLogBufferSize,                     16384,  65536},   // 16k        - 65k
  {PREF_nLogFlushInterval,                  15,     3600L},   // 15 seconds - 1 hour
  {PREF_nMaxClipboardPasteLines,            10,     1000L},   // lines of text
  {PREF_nMaxInputHistoryLines,              25,     250L},
  {PREF_nServerListServerRecentSeconds,     900,    ~0L},     // 15 mins
  {PREF_nServerListDCCChatRecentSeconds,    900,    ~0L},     // 15 mins
  {PREF_nServerListChannelRecentSeconds,    900,    ~0L},     // 15 mins
  {PREF_nServerListQueryRecentSeconds,      900,    ~0L},     // 15 mins
  {PREF_nServerConnectTimeout,              120,    ~0L},     // 2 mins
  {PREF_nDCCChatConnectTimeout,             120,    ~0L},     // 2 mins
  {PREF_nDCCChatFirstPort,                  3072,   ~65535L},
  {PREF_nDCCChatLastPort,                   4096,   ~65535L},
  {PREF_nChannelRejoinDelay,                60,     ~0L},     // 1 min
  {PREF_KickRejoinDelay,                    10,      ~0L},    // 10 seconds
  {PREF_END_OF_LIST,0}
};

StringPrefItem_t g_MainPrefs_Strings[PREF_STRING_MAX+1] =  // we don't need to specify the size really but it can help to track problems +1 is for list terminator
{
  {PREF_sIdentdOS,               "Win32"    }, // TODO: determine OS and fill in the default automatically
  {PREF_sIdentdUserName,         "HydraIRC" },
  {PREF_sDefaultTimestampFormat, "%Y-%m-%d %H:%M:%S" },
  {PREF_sDefaultWindowTimestampFormat, "[%H:%M]" },
  {PREF_sDefaultIdentity,        ""},
  {PREF_sServerLogFormat,        "[<TIMESTAMP=%Y-%m-%d %H:%M:%S>] <SERVEROUTPUT>" },
  {PREF_sChannelLogFormat,       "[<TIMESTAMP=%Y-%m-%d %H:%M:%S>] <CHANNELOUTPUT>" },
  {PREF_sQueryLogFormat,         "[<TIMESTAMP=%Y-%m-%d %H:%M:%S>] <QUERYOUTPUT>" }, //TODO: define variable
  {PREF_sDCCChatLogFormat,       "[<TIMESTAMP=%Y-%m-%d %H:%M:%S>] <DCCCHATOUTPUT>" }, //TODO: define variable
  {PREF_sRouterAddress,          ""},
  {PREF_sDefaultTheme,           "Default"},
  {PREF_sNickCompletionSuffix,   ","},
  {PREF_sChannelBookmark1,       "#hydrairc"},
  {PREF_sLogFileViewer,          "notepad.exe"},
  {PREF_sChannelBookmark2,       ""},
  {PREF_sChannelBookmark3,       ""},
  {PREF_sChannelBookmark4,       ""},
  {PREF_sChannelBookmark5,       ""},
  {PREF_sChannelBookmark6,       ""},
  {PREF_sChannelBookmark7,       ""},
  {PREF_sChannelBookmark8,       ""},
  {PREF_sChannelBookmark9,       ""},
  {PREF_END_OF_LIST,NULL}
};

StringPrefItem_t g_MainPrefs_Paths[PREF_PATH_MAX+1] = // we don't need to specify the size really but it can help to track problems +1 is for list terminator
{
  // TODO: For MS Logo compliance we need to default these to
  // "...\My Documents\HydraIRC\<Bleh>\" instead of "<apppath>\<bleh>"
  {PREF_pDCCDownloadsFolder, "<HOMEDIR>\\Downloads"},
  {PREF_pDCCUploadsFolder,   "<HOMEDIR>\\Downloads"}, // TODO: Add <MYDOCUMENTS>
  {PREF_pLogRootFolder,      "<HOMEDIR>\\Logs"},      // TODO: Add <MYDOCUMENTS>
  {PREF_END_OF_LIST,NULL}
};

FontPrefItem_t g_MainPrefs_Fonts[PREF_FONT_MAX+1] =  // we don't need to specify the size really but it can help to track problems +1 is for list terminator
{
  {PREF_fServerInputFont,      "Verdana", 9, FIF_NONE},
  {PREF_fServerOutputFont,     "Verdana", 9, FIF_NONE},
  {PREF_fChannelInputFont,     "Verdana", 9, FIF_NONE},
  {PREF_fChannelOutputFont,    "Verdana", 9, FIF_NONE},
  {PREF_fOutputWindowFont,     "Verdana", 9, FIF_NONE},
  {PREF_fChannelMonitorFont,   "Verdana", 9, FIF_NONE},
  {PREF_fServerMonitorFont,    "Verdana", 9, FIF_NONE},
  {PREF_fDCCChatInputFont,     "Verdana", 9, FIF_NONE},
  {PREF_fDCCChatOutputFont,    "Verdana", 9, FIF_NONE},
  {PREF_fQueryInputFont,       "Verdana", 9, FIF_NONE},
  {PREF_fQueryOutputFont,      "Verdana", 9, FIF_NONE},
  {PREF_fServerListFont,       "Verdana", 9, FIF_NONE},
  {PREF_fServerListDirtyFont,  "Verdana", 9, FIF_UNDERLINE},
  {PREF_fServerListRecentFont, "Verdana", 9, FIF_UNDERLINE},
  {PREF_fUserListFont,         "Verdana", 9, FIF_NONE},
  {PREF_fHeaderFont,           "Verdana", 9, FIF_NONE},
  {PREF_fInfoFont,             "Verdana", 8, FIF_NONE},

  {PREF_END_OF_LIST,NULL}
};

ColorPrefItem_t g_MainPrefs_Colors[PREF_COLOR_MAX+1] =  // we don't need to specify the size really but it can help to track problems +1 is for list terminator
{
  {color_white,                                    RGB(255,255,255)},
  {color_black,                                    RGB(  0,  0,  0)},
  {color_darkblue,                                 RGB(  0,  0,128)},
  {color_darkgreen,                                RGB(  0,146,  0)},
  {color_red,                                      RGB(255,  0,  0)},
  {color_brown,                                    RGB(128,  0,  0)},
  {color_purple,                                   RGB(156,  0,156)},
  {color_orange,                                   RGB(252,128,  0)},
  {color_yellow,                                   RGB(255,255,  0)},
  {color_green,                                    RGB(  0,252,  0)},
  {color_darkcyan,                                 RGB(  0,146,146)},
  {color_cyan,                                     RGB(  0,255,255)},
  {color_blue,                                     RGB(  0,  0,252)},
  {color_pink,                                     RGB(255,  0,255)},
  {color_gray,                                     RGB(128,128,128)},
  {color_lightgray,                                RGB(170,170,170)},

  // items..

  {item_background,                                RGB(255,255,255)},
  {item_normal,                                    RGB( 64, 64, 64)},
  {item_notice,                                    RGB(128,  0,  0)},
  {item_topic,                                     RGB(  0,146,  0)},
  {item_join,                                      RGB(  0,146,  0)},
  {item_kick,                                      RGB(192,  0,  0)},
  {item_mode,                                      RGB(  0,146,  0)},
  {item_nick,                                      RGB(  0,146,  0)},
  {item_part,                                      RGB(128,  0,  0)},
  {item_action,                                    RGB(156,  0,156)},
  {item_ctcp,                                      RGB(192,  0,192)},
  {item_quit,                                      RGB(128,  0,  0)},
  {item_connecting,                                RGB(  0,  0,128)},
  {item_querytext,                                 RGB(  0,192,192)},
  {item_owntext,                                   RGB(  0,  0,128)},
  {item_wallops,                                   RGB(192,  0,  0)},

  {item_info,                                      RGB(  0,128,  0)},
  {item_warning,                                   RGB(128,128,  0)},
  {item_error,                                     RGB(128,  0,  0)},

  {item_timestamp,                                 RGB(128,128,128)},



  {item_userlistbackground,                        RGB(255,255,255)},
  {item_userlistnormaltext,                        RGB(  0,  0,  0)},
  {item_userlistselectedbackground,                -1 /*RGB( 10, 36,106)*/},  // -1 = use system defaults

  {item_textinputbackground,                       RGB(255,255,255)},
  {item_textinputnormaltext,                       RGB(  0,  0,  0)},

  {item_globaluserlistbackground,                  RGB(255,255,255)},
  {item_globaluserlistnormaltext,                  RGB(  0,  0,  0)},
  {item_globaluserlistselectedbackground,          -1 /*RGB( 10, 36,106)*/},  // -1 = use system defaults

  {item_serverlistbackground,                      RGB(255,255,255)},
  {item_serverlistnormaltext,                      RGB(  0,  0,  0)},
  {item_serverlistselectedbackground,              -1 /*RGB(212,208,200)*/},  // -1 = use system defaults

  {item_serverlistdisconnected,                    RGB(128,  0,  0)},
  {item_serverlistconnecting,                      RGB(128,128,  0)},
  {item_serverlistconnected,                       RGB(  0,128,  0)},
  {item_serverlistserverdirtytext,                 RGB(  0,  0,128)},
  {item_serverlistserverdirtybackground,           RGB(255,255,255)},
  {item_serverlistdccchatdirtytext,                RGB(  0,  0,128)},
  {item_serverlistdccchatdirtybackground,          RGB(255,255,255)},
  {item_serverlistchanneldirtytext,                RGB(  0,  0,128)},
  {item_serverlistchanneldirtybackground,          RGB(255,255,255)},
  {item_serverlistquerydirtytext,                  RGB(  0,  0,128)},
  {item_serverlistquerydirtybackground,            RGB(255,255,255)},
  {item_serverlistserverrecenttext,                 RGB(255,  0,  0)},
  {item_serverlistserverrecentbackground,           RGB(255,255,255)},
  {item_serverlistdccchatrecenttext,                RGB(255,  0,  0)},
  {item_serverlistdccchatrecentbackground,          RGB(255,255,255)},
  {item_serverlistchannelrecenttext,                RGB(255,  0,  0)},
  {item_serverlistchannelrecentbackground,          RGB(255,255,255)},
  {item_serverlistqueryrecenttext,                  RGB(255,  0,  0)},
  {item_serverlistqueryrecentbackground,            RGB(255,255,255)},

  // background is not reversed for these items:

  {item_transferlisterrortext,                     RGB(147,  0,  0)},
  {item_transferlistcompletetext,                  RGB(  0,147,  0)},
  {item_transferlistnormaltext,                    RGB(  0,  0,  0)},
  {item_transferlistbackground,                    RGB(255,255,255)},

  {item_eventlogtext,                              RGB(  0,  0,  0)},
  {item_eventlogbackground,                        RGB(255,255,255)},

  {item_urlcatchertext,                            RGB(  0,  0,  0)},
  {item_urlcatcherurltext,                         RGB(  0,  0,255)}, // use internet explorer defaults.
  {item_urlcatchervisitedurltext,                  RGB(128,  0,128)}, // use internet explorer defaults.
  {item_urlcatcherbackground,                      RGB(255,255,255)},

  {item_noticelogtext,                             RGB(  0,  0,  0)},
  {item_noticelogbackground,                       RGB(255,255,255)},

  {item_querylogtext,                              RGB(  0,  0,  0)},
  {item_querylogbackground,                        RGB(255,255,255)},

  {item_headertext,                                RGB(  0,  0,  0)},
  {item_headerbackground,                          RGB(0xF7,0xF3,0xE9)},
  
  {item_infotext,                                  RGB(  0,  0,  0)},
  {item_infobackground,                            RGB(0xF7,0xF3,0xE9)},

  {PREF_END_OF_LIST,0}
};

/*
ColorPrefItem_t g_ChannelPrefs_Colors[] =
{
  {color_white,                                    0,},
  {color_black,                                    0,},
  {color_darkblue,                                 0,},
  {color_darkgreen,                                0,},
  {color_red,                                      0,},
  {color_brown,                                    0,},
  {color_purple,                                   0,},
  {color_orange,                                   0,},
  {color_yellow,                                   0,},
  {color_green,                                    0,},
  {color_darkcyan,                                 0,},
  {color_cyan,                                     0,},
  {color_blue,                                     0,},
  {color_pink,                                     0,},
  {color_gray,                                     0,},
  {color_lightgray,                                0,},

  // items..

  {item_background,                                0,},
  {item_normal,                                    0,},
  {item_notice,                                    0,},
  {item_topic,                                     0,},
  {item_join,                                      0,},
  {item_kick,                                      0,},
  {item_mode,                                      0,},
  {item_nick,                                      0,},
  {item_part,                                      0,},
  {item_action,                                    0,},
  {item_ctcp,                                      0,},
  {item_quit,                                      0,},
  {item_connecting,                                0,},
  {item_querytext,                                 0,},

  {item_info,                                      0,},
  {item_warning,                                   0,},
  {item_error,                                     0,},

  {item_timestamp,                                 0,},

  // gui colors.

  {item_userlistbackground,                        0,},
  {item_userlistnormaltext,                        0,},
  {item_textinputbackground,                       0,},
  {item_textinputnormaltext,                       0,},
  {PREF_END_OF_LIST,0}
};
*/

/* TODO: move to new prefs stuff

#ifdef ZANN_FONTS
#define FONT_SCRIPTS_LAST 20
int g_FontPrefScriptIDs[FONT_SCRIPTS_LAST] = {
  ANSI_CHARSET,
  DEFAULT_CHARSET,
  SYMBOL_CHARSET,
  SHIFTJIS_CHARSET,
  HANGEUL_CHARSET,
  HANGUL_CHARSET,
  GB2312_CHARSET,
  CHINESEBIG5_CHARSET,
  OEM_CHARSET,
  JOHAB_CHARSET,
  HEBREW_CHARSET,
  ARABIC_CHARSET,
  GREEK_CHARSET,
  TURKISH_CHARSET,
  VIETNAMESE_CHARSET,
  THAI_CHARSET,
  EASTEUROPE_CHARSET,
  RUSSIAN_CHARSET,
  MAC_CHARSET,
  BALTIC_CHARSET
};

char *g_FontPrefScriptNames[FONT_SCRIPTS_LAST] = {
"ANSI",
"BALTIC",
"CHINESEBIG5",
"DEFAULT",
"EASTEUROPE",
"GB2312",
"GREEK",
"HANGUL",
"MAC",
"OEM",
"RUSSIAN",
"SHIFTJIS",
"SYMBOL",
"TURKISH",
"VIETNAMESE",
"THAI"
"EASTEUROPE",
"RUSSIAN",
"MAC",
"BALTIC"
};
#endif
*/

// these must be the same order as the enum boolprefs in Prefs.h
char *g_PrefsStringIDNames[PREFSTRINGID_LAST] = 
{
  // bools

  "Prompt On Server Close",
  "Prompt On Disconnect",
  "Show Channel List Dialog On Connect",
  "Show Channel Window User List Pane By Default",
  "Show Channel Window Header Pane By Default",
  "Show Channel Window Info Pane By Default",
  "Show Server Window User List Pane By Default",
  "Show Server Window Header Pane By Default",
  "Show Server Window Info Pane By Default",
  "Show Query Window User List Pane By Default",
  "Show Query Window Header Pane By Default",
  "Show Query Window Info Pane By Default",
  "Show DCCChat Window User List Pane By Default",
  "Show DCCChat Window Header Pane By Default",
  "Show DCCChat Window Info Pane By Default",
  "Monitor Channels On Join",
  "Monitor Servers By Default",
  "Show Channel Monitor Message On Status Bar",
  "Remember Audio Mute",
  "Audio Mute",
  "Auto Tile",
  "Auto Maximize",
  "Hide Not Minimize",
  "Remove Old Privmsg Log Items",
  "Remove Old Notice Log Items",
  "Remove Old URLCatcher Items",
  "Remove Old Event Log Items",
  "Channel Member Notices Only",

  "Output Window Timestamps",
  "Channel Monitor Timestamps",
  "Server Monitor Timestamps",

  "Server Window Timestamps",
  "Channel Window Timestamps",
  "Query Window Timestamps",
  "DCC Chat Window Timestamps",

  "Show User List Group Counts",
  "Send Keep Alive Messages",
  "Show Pings",  
  "Show Pongs",  
  "Show MOTD",
  "Ignore CTCP",
  "Console Logging",
  "Socket Logging",
  "Cross Network Notice Forwarding",
  "Minimize To SysTray",
  "Create Query Windows Automatically",
  "Create Query Windows Hidden",
  "Restrict Queries To Query Windows",
  "Restrict Notices To Query Windows",
  "Forward Notices",
  "Forward Messages",
  "Forward CTCPs",
  "Forward Server Notices",
  "DCC Fast Sends",
  "DCC Verbose Transfers",
  "DCC Receive Double Click Opens File",
  "DCC Auto Accept",
  "Create Folders For DCC Transfers",
  "Identd Enabled",
  "Selective Flood Control",
  "Strict Channel Characters",
  "Server List Tool Tips",
  "Server List Channel Groups",
  "Logging",
  "Create Network Log Folder",
  "Server Logging",
  "Channel Logging",
  "Query Logging",
  "DCCChat Logging",
  "Strip Codes From Server Logs",
  "Strip Codes From Channel Logs",
  "Strip Codes From Query Logs",
  "Strip Codes From DCCChat Logs",
  "Strip Codes From Server Windows",
  "Strip Codes From Channel Windows",
  "Strip Codes From Query Windows",
  "Strip Codes From DCCChat Windows",
  "Show Private Messages On StatusBar",
  "CTCP Use ERRMSG Reply",
  "Rejoin Channel On Kick",
  "Use Gradient Filled Panes",
  "Clear Dirty Flag On Deactivate",
  "Show Nick Prefixes",
  "Show Buddy Groups In User List",
  "Activate Link On Double-Click",


  // ints

  "Default User List Width",
  "Default Info Height",
  "Max Header Lines",
  "Max Scroll Buffer Lines",
  "Max PrivMsg Log Age Mins",
  "Max Notice Log Age Mins",
  "Max URLCatcher Age Mins",
  "Max Event Log Age Mins",
  "Max PrivMsg Log Items",
  "Max Notice Log Items",
  "Max URLCatcher Log Items",
  "Max Event Log Items",
  "Reconnect Retries1",
  "Reconnect Delay1 Seconds",
  "Reconnect Retries2",
  "Reconnect Delay2 Seconds",
  "Keep Alive Delay",
  "Transfer View Update Frequency Seconds",
  "Inactivity Scroll Delay Seconds",
  "Scroll Lock Timeout Seconds",
  "Flood Control Threshold Lines",
  "Flood Control Lines Per Second",
  "Query Notify Reactivate Delay Seconds",
  "DCC Transfer Timeout Seconds",
  "DCC Resume Timeout Seconds",
  "DCC Negotiate Timeout Seconds",
  "DCC Send First Port",
  "DCC Send Last Port",
  "DCC Send Buffer Size",
  "Log Buffer Size",
  "Log Flush Interval Seconds",
  "Max Clipboard Paste Lines",
  "Max Input History Lines",
  "Server List Server Recent Seconds",
  "Server List DCCChat Recent Seconds",
  "Server List Channel Recent Seconds",
  "Server List Query Recent Seconds",
  "Server Connect Timeout Seconds",
  "DCC Chat Connect Timeout Seconds",
  "DCC Chat First Port",
  "DCC Chat Last Port",
  "Channel Rejoin Delay Seconds",
  "Kick Rejoin Delay Seconds",


  // strings

  "Identd OS",
  "Identd UserName",
  "Default Timestamp Format",
  "Default Window Timestamp Format",
  "Default Identity",
  "Server Log Format",
  "Channel Log Format",
  "Query Log Format",
  "DCCChat Log Format",
  "External Address",
  "Default Theme",
  "Nick Completion Suffix",
  "Log File Viewer",
  "Channel Bookmark 1",
  "Channel Bookmark 2",
  "Channel Bookmark 3",
  "Channel Bookmark 4",
  "Channel Bookmark 5",
  "Channel Bookmark 6",
  "Channel Bookmark 7",
  "Channel Bookmark 8",
  "Channel Bookmark 9",


  // paths

  "DCC Downloads Folder",
  "DCC Uploads Folder",
  "Logs Folder",


  // fonts

  "Server Input",
  "Server Output",
  "Channel Input",
  "Channel Output",
  "Output Window",
  "Channel Monitor",
  "Server Monitor",
  "DCCChat Input",
  "DCCChat Output",
  "Query Input",
  "Query Output",
  "Server List",
  "Server List Dirty",
  "Server List Recent",
  "User List",
  "Header",
  "Info",


  // msgview colors

  "Color_white",          
  "Color_black",          
  "Color_darkblue",       
  "Color_darkgreen",
  "Color_red",
  "Color_brown",
  "Color_purple",
  "Color_orange",
  "Color_yellow",
  "Color_green",
  "Color_darkcyan",
  "Color_cyan",
  "Color_blue",
  "Color_pink",
  "Color_gray",
  "Color_lightgray",

  "Item_background",      
  "Item_normal",          
  "Item_notice",          
  "Item_topic",           
  "Item_join",            
  "Item_kick",            
  "Item_mode",            
  "Item_nick",            
  "Item_part",            
  "Item_action",          
  "Item_ctcp",            
  "Item_quit",            
  "Item_connecting",      
  "Item_querytext",       
  "Item_owntext",
  "Item_wallops",

  "Item_info",            
  "Item_warning",         
  "Item_error",           

  "Item_timestamp",

  // gui colors

  "Item_userlistbackground",
  "Item_userlistnormaltext",
  "Item_userlistselectedbackground",

  "Item_textinputbackground",
  "Item_textinputnormaltext",

  "Item_globaluserlistbackground",
  "Item_globaluserlistnormaltext",
  "Item_globaluserlistselectedbackground",

  "Item_serverlistbackground",
  "Item_serverlistnormaltext",
  "Item_serverlistselectedbackground",

  "Item_serverlistdisconnected",
  "Item_serverlistconnecting",
  "Item_serverlistconnected",
  "Item_serverlistserverdirtytext", 
  "Item_serverlistserverdirtybackground", 
  "Item_serverlistdccchatdirtytext", 
  "Item_serverlistdccchatdirtybackground", 
  "Item_serverlistchanneldirtytext", 
  "Item_serverlistchanneldirtybackground", 
  "Item_serverlistquerydirtytext", 
  "Item_serverlistquerydirtybackground",
  "Item_serverlistserverrecenttext", 
  "Item_serverlistserverrecentbackground", 
  "Item_serverlistdccchatrecenttext", 
  "Item_serverlistdccchatrecentbackground", 
  "Item_serverlistchannelrecenttext", 
  "Item_serverlistchannelrecentbackground", 
  "Item_serverlistqueryrecenttext", 
  "Item_serverlistqueryrecentbackground",

  "Item_transferlisterrortext",
  "Item_transferlistcompletetext",
  "Item_transferlistnormaltext",
  "Item_transferlistbackground",

  "Item_eventlogtext",
  "Item_eventlogbackground",

  "Item_urlcatchertext",
  "Item_urlcatcherurltext",
  "Item_urlcatchervisitedurltext",
  "Item_urlcatcherbackground",

  "Item_noticelogtext",
  "Item_noticelogbackground",

  "Item_querylogtext",
  "Item_querylogbackground",

  "Item_headertext",
  "Item_headerbackground",

  "Item_infotext",
  "Item_infobackground",

};


// Prefs Utils
//
/////////////////////////////////////////////////////////////////////////////
/*
void EscapeString(char *dest, int destlen, char *source)
{
  if (dest)
    *dest = 0; // null terminate output buffer.

  if (!dest || !source || destlen <= 0)
    return;

  int dp,x;

  dp = 0;

  while (*source && dp < (destlen-4))  // -4 so there is always room to add "\xxx"
  {
    // do we need to escape it?
    if (*source == '\\')
    {
      // turn it from "\" into "\\"
      x = _snprintf(dest,destlen-dp,"\\\\",*source++);
      dp += x;
      dest += x;
    }
    else if (!isprint(*source) || *source == '<' || *source == '>' || *source == '&')
    {
      // convert it to a simple c-style literal string (\ooo)
      x = _snprintf(dest,destlen-dp,"\\%03o",*source++);
      dp += x;
      dest += x;
    }
    else
    {
      *dest++ = *source++;
      dp++;
    }
  }
  *dest = 0;
}

void UnEscapeString(char *dest, int destlen, char*source)
{
  if (dest)
    *dest = 0; // null terminate output buffer.

  if (!dest || !source || destlen <= 0)
    return;

  int dp;

  dp = 0;

  while (*source && dp < destlen)
  {
    if (*source == '\\')
    {
      source++;
      if (*source)
      {
        if (*source == '\\')
        {
          *dest++ = *source++;
          dp++;
        }
        else
        {
          int oct,success;
          success = sscanf(source,"%3o",&oct);
          if (success == 1)
          {
            *dest++ = (char)oct;
            source += 3;
            dp++;
          }
        }
      }
    }
    else
    {
      *dest++ = *source++;
      dp++;
    }
  }
  *dest = 0;
}
*/

void EscapeString(char *dest, int destlen, char *source)
{
  if (dest)
    *dest = 0; // null terminate output buffer.

  if (!dest || !source || destlen <= 0)
    return;

  int dp,x;

  dp = 0;

  while (*source && dp < destlen)
  {
    // do we need to escape it?
    if (*source == '\\')
    {
      // turn it from "\" into "\\"
      x = _snprintf(dest,destlen-dp,"\\\\",*source++);
      dp += x;
      dest += x;
    }
    else if (!isprint((unsigned char)*source) || *source == '<' || *source == '>' || *source == '&')
    {
      // convert it to a simple c-style literal string (\ooo)
      x = _snprintf(dest,destlen-dp,"\\%03o",(unsigned char)(*source++));
      dp += x;
      dest += x;
    }
    else
    {
      *dest++ = *source++;
      dp++;
    }
  }
  *dest = 0;
}

void UnescapeString(char *dest, int destlen, char*source)
{
  if (dest)
    *dest = 0; // null terminate output buffer.

  if (!dest || !source || destlen <= 0)
    return;

  int dp;

  dp = 0;

  while (*source && dp < destlen)
  {
    if (*source == '\\')
    {
      source++;
      if (*source)
      {
        if (*source == '\\')
        {
          *dest++ = *source++;
          dp++;
        }
        else
        {
          int oct,success,count;
          count = 0;
          while (source[count] && isdigit(source[count]) && count <3)
            count++;

          if (count == 3)
          {
            success = sscanf(source,"%3o",&oct);
            if (success == 1)
            {
              *dest++ = (char)oct;
              source += 3;
              dp++;
            }
          }
          else
          {
            source += count; // skip this
          }
        }
      }
    }
    else
    {
      *dest++ = *source++;
      dp++;
    }
  }
  *dest = 0;
}


void HandleXMLError( void* ctxt, const char* text, ... )
{
  va_list argptr;
  static char buf[8192];

  va_start (argptr,text);
  _vsnprintf (buf, sizeof(buf),text, argptr);
  va_end (argptr);
  buf[sizeof(buf) - 1] = 0;


  // split up the buffer into seperate lines.
  char *ptr, *start;

  ptr = buf;
  start = buf;

  while (*ptr)
  {
    if (*ptr == '\n' || !*(ptr+1))
    {
      *ptr++ = 0;

      sys_Printf(BIC_XML,"%s\n",start);

      start = ptr;
    }
    else
    {
      ptr++;
    }
  };
}


/** set an integer preference item
 *
 * We leave the preference's value unchanged if the supplied value
 * is invalid.
 * returns TRUE if the preference was set (not changed)
 */
BOOL Prefs::SetIntPref(int& pref, char *value)
{

  if (!value || !*value) // return on null or empty string
    return FALSE;

  // Color in HTML hex notation "#xxxxxx"
  if (*value == '#')
  {
    int newval;
    if (1 == sscanf(value,"#%6x",&newval))
    {
      pref = newval;
      return TRUE;
    }
    else
      return FALSE;
  }
  else // plain old number "n"
  {
    // validate the value (make sure it's a number in string format)
    for (int i = 0 ; value[i] != 0 ; i++)
    {
      if (!(isdigit(value[i]) || ((i == 0) && (value[i] == '-')) ) )
        return FALSE;
    }

    pref = atoi(value);
  }
  return TRUE;
}

/** set a boolean preference item
 *
 * We leave the preference's value unchanged if the supplied value
 * is invalid.
 * returns TRUE if the preference was set (not changed)
 * TRUE = "1", "T*", "Y*"  FALSE = "0", "F*", "N*"
 */
BOOL Prefs::SetBooleanPref(char *value, BOOL *pref)
{
  if (!value || !*value) // return on null or empty string
    return FALSE;

  if ((value[0] == '0') || (tolower(value[0]) == 'n') || (tolower(value[0]) == 'f'))
  {
    *pref = FALSE;
    return TRUE;
  }
  if ((value[0] == '1') || (tolower(value[0]) == 'y') || (tolower(value[0]) == 't'))
  {
    *pref = TRUE;
    return TRUE;
  }
  return FALSE;
}

/*

PrefsCollection_t *Prefs::GetChannelPrefsCollection(char *ChannelName, int NetworkID)
{
  ChannelPrefs *pChannelPrefs;
  for (int i = 0 ; i < m_ChannelPrefList.GetSize() ; i++ )
  {
    pChannelPrefs = m_ChannelPrefList[i];

    if (pChannelPrefs->m_NetworkID == NetworkID && stricmp(pChannelPrefs->m_ChannelName,ChannelName) == 0)
    {
      return &pChannelPrefs->m_PC;
    }
  }
  return &m_PC;
}

BOOL Prefs::GetBooleanPref(PrefsCollection_t *pPC, int PrefID)
{
  BOOL *pBool;
  BoolPrefItem_t *pBPI;
  

  for (int pass = 0 ; pass < 2 ; pass ++)
  {
    // use the defaults if we weren't given a prefs collection or the prefs collection doesn't contain booleans
    // or we didn't find the pref in the pref collection we were given.
    if (pass == 2 || !pPC || !pPC->m_pBools)
      pPC = &m_PC;

    if (pPC)
    {
      pBool = pPC->m_pBools;
      pBPI = pPC->m_pBoolsTable;

      // find out item, icrementing the pointer to the prefs table and to the actual value.
      while (pBPI->m_PrefID != PrefID && pBPI->m_PrefID != PREF_END_OF_LIST) 
      {
        pBPI++;
        pBool++;
      }
      
      if (pBPI->m_PrefID == PrefID)
      {
        return *pBool;
      }
    }
  }
  ATLASSERT(0); // should not get here - maybe you specified a bad PrefID ?
  return FALSE; // shut compiler up.
}

COLORREF Prefs::GetColorPref(PrefsCollection_t *pPC, int PrefID)
{
  COLORREF *pColor;
  ColorPrefItem_t *pCPI;
  
  for (int pass = 0 ; pass < 2 ; pass ++)
  {
    // use the defaults if we weren't given a prefs collection or the prefs collection doesn't contain booleans
    // or we didn't find the pref in the pref collection we were given.
    if (pass == 2 || !pPC || !pPC->m_pColors)
      pPC = &m_PC;

    if (pPC)
    {
      pColor = pPC->m_pColors;
      pCPI = pPC->m_pColorsTable;

      // find out item, icrementing the pointer to the prefs table and to the actual value.
      while (pCPI->m_PrefID != PrefID && pCPI->m_PrefID != PREF_END_OF_LIST) 
      {
        pCPI++;
        pColor++;
      }
      
      if (pCPI->m_PrefID == PrefID)
      {
        return *pColor;
      }
    }
  }
  ATLASSERT(0); // should not get here - maybe you specified a bad PrefID ?
  return FALSE; // shut compiler up.
}

int Prefs::GetPrefIndexFromID(PrefsCollection_t *pPC, int PrefType, int PrefID)
{
  int index = 0;
  switch(PrefType)
  {
    case PT_BOOL:
      {
        ATLASSERT(pPC->m_pBoolsTable && pPC->m_pBools); // we MUST have these!

        BoolPrefItem_t *pBPI = pPC->m_pBoolsTable;
        
        while (pBPI->m_PrefID != PrefID && pBPI->m_PrefID != PREF_END_OF_LIST) 
        {
          pBPI++; // find our item!
          index ++;
        }
        return index;
      }
      break;
    case PT_INT:
      {
        ATLASSERT(pPC->m_pIntsTable && pPC->m_pInts); // we MUST have these!

        IntPrefItem_t *pIPI = pPC->m_pIntsTable;
        
        while (pIPI->m_PrefID != PrefID && pIPI->m_PrefID != PREF_END_OF_LIST) 
        {
          pIPI++; // find our item!
          index ++;
        }
        return index;
      }
      break;
    case PT_STRING:
      {
        ATLASSERT(pPC->m_pStringsTable && pPC->m_pStrings); // we MUST have these!

        StringPrefItem_t *pSPI = pPC->m_pStringsTable;
        
        while (pSPI->m_PrefID != PrefID && pSPI->m_PrefID != PREF_END_OF_LIST) 
        {
          pSPI++; // find our item!
          index ++;
        }
        return index;
      }
      break;
    case PT_PATH:
      {
        ATLASSERT(pPC->m_pPathsTable && pPC->m_pPaths); // we MUST have these!

        PathPrefItem_t *pPPI = pPC->m_pPathsTable;
        
        while (pPPI->m_PrefID != PrefID && pPPI->m_PrefID != PREF_END_OF_LIST) 
        {
          pPPI++; // find our item!
          index ++;
        }
        return index;
      }
      break;
    case PT_FONT:
      {
        ATLASSERT(pPC->m_pFontsTable && pPC->m_pFonts); // we MUST have these!

        FontPrefItem_t *pFPI = pPC->m_pFontsTable;
        
        while (pFPI->m_PrefID != PrefID && pFPI->m_PrefID != PREF_END_OF_LIST) 
        {
          pFPI++; // find our item!
          index ++;
        }
        return index;
      }
      break;
    case PT_COLOR:
      {
        ATLASSERT(pPC->m_pColorsTable && pPC->m_pColors); // we MUST have these!

        ColorPrefItem_t *pCPI = pPC->m_pColorsTable;
        
        while (pCPI->m_PrefID != PrefID && pCPI->m_PrefID != PREF_END_OF_LIST) 
        {
          pCPI++; // find our item!
          index ++;
        }
        return index;
      }
      break;
  }

  return -1; // shut the compiler up.
}
*/

BOOL Prefs::UpdateFontPrefItem(FontPrefItem_t *pFPI,char *FontPref)
{
  if (!pFPI || !FontPref || !*FontPref)
    return FALSE;

  char *fontinfostr = strdup(FontPref);

  if (!fontinfostr)
    return FALSE;

  // FONT NAME
  char *tmpstr = strtok(fontinfostr,",");
  int tmpnum;
  if (tmpstr)
  {
    if (*tmpstr != '-')
    {
      if (pFPI->m_Name) 
        free(pFPI->m_Name);
      pFPI->m_Name = strdup(tmpstr);
    }
    // SIZE
    tmpstr = strtok(NULL,",");
    if (tmpstr)
    {
      if (sscanf(tmpstr,"%d",&tmpnum) == 1)
      {
        if (tmpnum > 0)
          pFPI->m_Size = tmpnum;
      }
    }

    // STYLES
    tmpstr = strtok(NULL,",");
    if (tmpstr)
    {
      strupr(tmpstr);
      if (strchr(tmpstr,'N') > 0)
        pFPI->m_Flags = 0; // reset all flags.
      if (strchr(tmpstr,'B') > 0)
        pFPI->m_Flags |= FIF_BOLD;
      if (strchr(tmpstr,'I') > 0)
        pFPI->m_Flags |= FIF_ITALIC;
      if (strchr(tmpstr,'U') > 0)
        pFPI->m_Flags |= FIF_UNDERLINE;
    }

  }
  free(fontinfostr);

  return TRUE;
}

// BOOL Prefs::SetPref(PrefsCollection_t *pPC, char *pref, char *value) // prefscollections
BOOL Prefs::SetPref(char *pref, char *value)
{
  if (!(pref && *pref && value))// && *value))
    return FALSE;

  BOOL done = FALSE;
  BOOL success = FALSE;
  int i;//,prefindex;
  char *prefcpy = strdup(pref);
  char *base = strtok(prefcpy,"/\\");
  char *prefname = strtok(NULL,"");

  if (!prefname)
  {
    prefname = base;
    base = "misc";
  }

  if (stricmp(base,"colors") == 0)
  {
    for (i = 0 ; i < PREF_COLOR_MAX && !done ; i ++)
    {
      if (stricmp(prefname,PREF_COLOR_NAME(i)) == 0)
      {
        /*
        prefindex = GetPrefIndexFromID(pPC,PT_COLOR,PREF_COLOR_FIRST + i);
        if (prefindex != -1)
        {
          done = SetIntPref(value,(int *)(&pPC->m_pColors[prefindex]));
          if (done)
            pPC->m_pColors[prefindex] = RGBVALTOCOLORREF(pPC->m_pColors[prefindex]);
        }
        */
        done = SetIntPref((int&)m_ColorPrefs[i],value);
        if (done)
          m_ColorPrefs[i] = RGBVALTOCOLORREF(m_ColorPrefs[i]);
        success = done;
      }
    }
  }
  else if (stricmp(base,"font") == 0)
  {
    for (i = 0 ; i < PREF_FONT_MAX && !done ; i ++)
    {
      if (stricmp(prefname,PREF_FONT_NAME(i)) == 0)
      {
        success = UpdateFontPrefItem(&m_FontPrefs[i],value);
        done = TRUE;
      }
    }
  }
  else if (stricmp(base,"misc") == 0)
  {
    for (i = 0 ; i < PREF_BOOL_MAX && !done ; i ++)
    {
      
      if (stricmp(prefname,PREF_BOOL_NAME(i)) == 0)
      {
        /*
        prefindex = GetPrefIndexFromID(pPC,PT_BOOL,PREF_BOOL_FIRST + i);
        if (prefindex != -1)
        {
          success = SetBooleanPref(value,&pPC->m_pBools[prefindex]);
          done = TRUE;
        }
        */
        success = SetBooleanPref(value,&m_BoolPrefs[i]);
        done = TRUE;
      }
    }
    if (!done)
    {
      for (i = 0 ; i < PREF_INT_MAX && !done ; i ++)
      {
        if (stricmp(prefname,PREF_INT_NAME(i)) == 0)
        {
          /*
          prefindex = GetPrefIndexFromID(pPC,PT_INT,PREF_INT_FIRST + i);
          if (prefindex != -1)
          {
            success = SetIntPref(value,&pPC->m_pInts[prefindex]);
            done = TRUE;
          }
          */
          success = SetIntPref(m_IntPrefs[i],value);
          done = TRUE;
        }
      }
      if (!done)
      {
        for (i = 0 ; i < PREF_STRING_MAX && !done ; i ++)
        {
          if (stricmp(prefname,PREF_STRING_NAME(i)) == 0)
          {
            /*
            prefindex = GetPrefIndexFromID(pPC,PT_STRING,PREF_STRING_FIRST + i);
            if (prefindex != -1)
            {
              // free the old one
              if (pPC->m_pStrings[prefindex]) 
                free(pPC->m_pStrings[prefindex]);

              // save the new one
              pPC->m_pStrings[prefindex] = value;
              done = TRUE;
              success = TRUE;
            }
            */
            char *valuecopy = strdup(value);
            if (valuecopy)
            {
              // free the old one
              if (m_StringPrefs[i]) 
                free(m_StringPrefs[i]);

              // save the new one
              m_StringPrefs[i] = valuecopy;
              done = TRUE;
              success = TRUE;
            }
          }
        }
        if (!done)
        {
          for (i = 0 ; i < PREF_PATH_MAX && !done ; i ++)
          {
            if (stricmp(prefname,PREF_PATH_NAME(i)) == 0)
            {
              /*
              prefindex = GetPrefIndexFromID(pPC,PT_PATH,PREF_PATH_FIRST + i);
              if (prefindex != -1)
              {
                // free the old one
                if (pPC->m_pPaths[prefindex]) 
                  free(pPC->m_pPaths[prefindex]);

                // save the new one
                pPC->m_pPaths[prefindex] = value;
                done = TRUE;
                success = TRUE;
              }
              */
              done = TRUE;
              if (*value)
              {
                char *valuecopy = strdup(value);
                if (valuecopy)
                {
                  // free the old one
                  if (m_PathPrefs[i]) 
                    free(m_PathPrefs[i]);

                  // save the new one
                  m_PathPrefs[i] = valuecopy;                
                  success = TRUE;
                }
              }
            }
          }
        }
      }
    }
  }
  free(prefcpy);
  return success;
}


// call to make HydraIRC take notice of the pref that was changed.
void Prefs::UpdatePref(char *pref)
{
  if (!(pref && *pref))
    return;

  char *prefcpy = strdup(pref);
  char *base = strtok(prefcpy,"/\\");
  char *prefname = strtok(NULL,"");

  if (!prefname)
  {
    prefname = base;
    base = "misc";
  }

  // from here on, use prefname and base

  if ((stricmp(base,"font") == 0) || (stricmp(base,"colors") == 0) || (stricmp(prefname,"colorscheme") == 0))
  {
    SetupFonts();
    g_pMainWnd->RefreshAllChildWindows();
    g_pMainWnd->m_ServerListView.UpdateSettings();
    g_pMainWnd->m_ChannelMonitorView.UpdateSettings();
    g_pMainWnd->m_ServerMonitorView.UpdateSettings();
    g_pMainWnd->m_OutputView.UpdateSettings();
    g_pMainWnd->m_UserListView.UpdateSettings();
    g_pTransfersManager->m_pTransfersView->UpdateSettings();
    g_pNoticeQueueManager->m_pTextQueueView->UpdateSettings();
    g_pPrivMsgQueueManager->m_pTextQueueView->UpdateSettings();
    g_pUrlCatcherManager->m_pTextQueueView->UpdateSettings();
    g_pEventLogManager->m_pEventLogView->UpdateSettings();
  }
  if (stricmp(base,"misc") == 0)
  {
    if ((stricmp(prefname,PREF_INT_NAME(PREF_nMaxInputHistoryLines)) == 0) ||
        (stricmp(prefname,PREF_INT_NAME(PREF_nDefaultUserListWidth)) == 0)  )
    {
      g_pMainWnd->RefreshAllChildWindows();
    }
    if (stricmp(prefname,PREF_BOOL_NAME(PREF_bIdentdEnabled)) == 0)
    {
      if (g_pIdentd) // TODO: we shouldn't be here if this don't exist..
        g_pIdentd->Update();
    }
  }

  free(prefcpy);
}

int GetNotificationEventCode(char *str)
{
  for (int i = 0 ; i < NE_LAST ; i++)
  {
    if (stricmp(g_NotificationEventNames[i],str) == 0)
      return i;
  }
  return -1;
}

UserIdentity *Prefs::GetPreferredIdentity(int NetworkID, int Flags)
{
  // select the last user identity for the network.
  char *pPreferredIdentity;
  UserIdentity *pIdentity = NULL;
  IRCNetworkDetails *pNetworkDetails = GetNetworkDetails(NetworkID);
  if (pNetworkDetails)
  {
    pPreferredIdentity = pNetworkDetails->m_IdentityName;

    if (pPreferredIdentity)
      pIdentity = FindIdentityByDescripton(pPreferredIdentity);
  }
  if (!pIdentity && (Flags & GPIF_UseDefault))
  {
    pIdentity = FindIdentityByDescripton(m_StringPrefs[PREF_sDefaultIdentity - PREF_STRING_FIRST]);
  }
  if (!pIdentity && (Flags & GPIF_UseFirst))
  {
    pIdentity = m_UserIdentitiesList[0];
  }
  return pIdentity;
}

UserIdentity *Prefs::FindIdentityByDescripton(char *Description)
{
  if (!Description || !*Description)
    return NULL;

  for (int i = 0 ; i < m_UserIdentitiesList.GetSize() ; i++ )
  {
    if (stricmp(m_UserIdentitiesList[i]->m_Description,Description) == 0)
      return m_UserIdentitiesList[i];
  }
  return NULL;
}

UserIdentity *Prefs::FindIdentityByID(int ID)
{
  for (int i = 0 ; i < m_UserIdentitiesList.GetSize() ; i++ )
  {
    if (m_UserIdentitiesList[i]->m_ID == ID)
      return m_UserIdentitiesList[i];
  }
  return NULL;
}

int Prefs::ObtainNewIdentityID( void )
{
  // we start from 1, not 0.  m_ID is not related to the position
  // of the identity in the m_IdentiyList memeber.
  int num = 1; 

  while (FindIdentityByID(num)) num++;
  return num;
}

// LOADING
//
/////////////////////////////////////////////////////////////////////////////

void Prefs::XML_LoadGeneralEventNotifications(xmlNodePtr pNode,int GroupID)
{
  BOOL Done;
  char *prop,*Name;
  int EventCode;
  NotificationPrefInfo_t *pNPI;

  while (pNode != NULL)
  {
    if (pNode->type == XML_ELEMENT_NODE && stricmp((char*)pNode->name,"event") == 0)
    {
      Done = FALSE;
      Name = (char*)xmlGetProp(pNode, (xmlChar*)"name");
      if (Name)
      {
        EventCode = GetNotificationEventCode(Name);
        if (EventCode != -1)
        {
          pNPI = (NotificationPrefInfo_t *)malloc(sizeof(NotificationPrefInfo_t));
          if (pNPI)
          {
            ZeroMemory(pNPI, sizeof(NotificationPrefInfo_t));
            pNPI->m_ForegroundColor = -1;
            pNPI->m_BackgroundColor = -1;
            

            pNPI->m_EventCode = EventCode;
            pNPI->m_GroupID = GroupID;

            prop = (char*)xmlGetProp(pNode, (xmlChar*)"soundid");
            pNPI->m_SoundID = prop ? atoi(prop) : -1;
            if (prop) free(prop);


            prop = (char*)xmlGetProp(pNode, (xmlChar*)"flags");
            pNPI->m_Flags = prop ? atoi(prop) : 0;
            if (prop) free(prop);
            pNPI->m_FileName = (char*)xmlGetProp(pNode, (xmlChar*)"soundfilename");

            prop = (char*)xmlGetProp(pNode, (xmlChar*)"highlightflags");
            pNPI->m_HighlightFlags = prop ? atoi(prop) : 0;
            if (prop) free(prop);

            // TODO: remove this for 0.4
            // convert HIGHLIGHT_RESERVED to NE_FLAG_LOGINEVENTLOG
            // HIGHLIGHT_RESERVED was HIGHLIGHT_LOGINEVENTLOG in 0.3.133
            if (pNPI->m_HighlightFlags & HIGHLIGHT_RESERVED)
            {
              pNPI->m_Flags |= NE_FLAG_LOGINEVENTLOG;
              pNPI->m_HighlightFlags ^= HIGHLIGHT_RESERVED;
            }


            BOOL Sucess;
            prop = (char*)xmlGetProp(pNode, (xmlChar*)"foregroundcolor");
            if (prop)
            {
              Sucess = SetIntPref((int&)pNPI->m_ForegroundColor,prop);
              if (Sucess)
                pNPI->m_ForegroundColor = RGBVALTOCOLORREF(pNPI->m_ForegroundColor); // convert from COLORVAL to RGB
              free(prop);
            }

            prop = (char*)xmlGetProp(pNode, (xmlChar*)"backgroundcolor");
            if (prop)
            {
              Sucess = SetIntPref((int&)pNPI->m_BackgroundColor,prop);
              if (Sucess)
                pNPI->m_BackgroundColor = RGBVALTOCOLORREF(pNPI->m_BackgroundColor); // convert from COLORVAL to RGB
              free(prop);
            }

            pNPI->m_MatchString = (char*)xmlGetProp(pNode, (xmlChar*)"matchstring");


            // validate it

            // all flags are ok by themselves,
            // but some flags need data memebers too.
            BOOL ok = TRUE; 
            if ((pNPI->m_Flags & NE_FLAG_SOUNDFILE) && (pNPI->m_FileName == NULL)) // enabled, needs valid data.
              ok = FALSE;
            if ((pNPI->m_Flags & NE_FLAG_SYSTEMSOUND) && (pNPI->m_SoundID < 0)) // enabled, needs valid data.
              ok = FALSE;
            if (ok)
            {
              m_NotificationPrefList.Add(pNPI);
            }
            else
            {
              sys_Printf(BIC_XML,"Error parsing <event>, Name = \"%s\"\n",g_NotificationEventNames[EventCode]);
              free(pNPI);
            }
            Done = TRUE;
          }
        }
      }
      if (!Done)
        sys_Printf(BIC_XML,"Error parsing <event>, Name = \"%s\"\n",UseEmptyString(Name));

      if (Name) free(Name);
    }
    pNode = pNode->next;
  }
}

void Prefs::XML_LoadGeneralNotifications(xmlNodePtr pNode)
{  
  char *prop;
  int GroupID;

  while (pNode != NULL)
  {
    if (pNode->type == XML_ELEMENT_NODE && stricmp((char*)pNode->name,"group") == 0)
    {
      prop = (char*)xmlGetProp(pNode, (xmlChar*)"id");
      if (prop)
      {
        GroupID = atoi(prop);
        free(prop);
      }

      if (GroupID >= -2)
      {
        XML_LoadGeneralEventNotifications(pNode->children,GroupID);
      }
    }
    pNode = pNode->next;
  }
}

void Prefs::XML_LoadNotifications(xmlNodePtr pNode)
{  
  while (pNode != NULL)
  {
    if (pNode->type == XML_ELEMENT_NODE)
    {
      if (stricmp((char*)pNode->name,"general") == 0)
        XML_LoadGeneralNotifications(pNode->children);
    }
    pNode = pNode->next;
  }
}

BOOL Prefs::XML_LoadNicks(xmlNodePtr pNode, UserIdentity *pUserIdentity)
{  
  char *Nick;

  while (pNode != NULL)
  {
    if (pNode->type == XML_ELEMENT_NODE && stricmp((char*)pNode->name,"nick") == 0)
    {
      // Get the properties!
      Nick = (char*)xmlGetProp(pNode, (xmlChar*)"name");
      
      if (Nick && *Nick)
      {
        pUserIdentity->m_NickList.Add(Nick);

#ifdef PREF_DEBUG
        sys_Printf(BIC_XML,"Nick, Name = \"%s\"\n",Nick);
#endif
      }
      else
      {
        sys_Printf(BIC_XML,"Error parsing <nick>, Name = \"%s\"\n",Nick);
        if (Nick) free(Nick); // free the empty string
      }
    }
    pNode = pNode->next;
  }
  return (BOOL(pUserIdentity->m_NickList.GetSize() > 0));
}

void Prefs::XML_LoadUserIdentities(xmlNodePtr pNode)
{  
  char *Description, *RealName, *UserName, *IDstr, *IdentOS, *IdentUserName;
  int ID;

  while (pNode != NULL)
  {
    if (pNode->type == XML_ELEMENT_NODE && stricmp((char *)pNode->name,"identity") == 0)
    {
      // Get the properties!
      Description   = (char*)xmlGetProp(pNode, (xmlChar*)"name");
      RealName      = (char*)xmlGetProp(pNode, (xmlChar*)"realname");
      UserName      = (char*)xmlGetProp(pNode, (xmlChar*)"username");
      IDstr         = (char*)xmlGetProp(pNode, (xmlChar*)"id");
      IdentOS       = (char*)xmlGetProp(pNode, (xmlChar*)"identdos");
      IdentUserName = (char*)xmlGetProp(pNode, (xmlChar*)"identdusername");

      if (Description && RealName && UserName && IDstr && ((ID = atoi(IDstr)) > 0))
      {
        free(IDstr); // Not needed anymore, use ID
        // TODO: verify ID is unique
        UserIdentity *pUserIdentity = new UserIdentity();

        if (pUserIdentity)
        {
#ifdef PREF_DEBUG
          sys_Printf(BIC_XML,"UserIdentity, Name = \"%s\"\n",Description);
#endif
          pUserIdentity->m_Description = Description;
          pUserIdentity->m_RealName = RealName;
          pUserIdentity->m_UserName = UserName;
          pUserIdentity->m_ID = ID;

          // These were null, prior to 0.323
          pUserIdentity->m_IdentdOS = IdentOS; 
          pUserIdentity->m_IdentdUserName = IdentUserName;

          if (XML_LoadNicks(pNode->children,pUserIdentity))
          {
            m_UserIdentitiesList.Add(pUserIdentity);
          }
          else
          {
            delete pUserIdentity;
          }
        }
      }
      else
      {
        sys_Printf(BIC_XML,"Error parsing <identity>, Name = \"%s\"\n",UseEmptyString(Description));

        if (Description) free(Description);
        if (RealName) free(RealName);
        if (UserName) free(UserName);
        if (IDstr) free(IDstr);
        if (IdentOS) free(IdentOS);
        if (IdentUserName) free(IdentUserName);
      }
    }
    pNode = pNode->next;
  }
}

void Prefs::XML_LoadBuddies(xmlNodePtr pNode, IRCUserGroup *pGroup)
{  
  char *BuddyName;

  while (pNode != NULL)
  {
    if (pNode->type == XML_ELEMENT_NODE && stricmp((char*)pNode->name,"buddy") == 0)
    {
      // Get the properties!
      BuddyName = (char*)xmlGetProp(pNode, (xmlChar*)"name");
      
      if (BuddyName)
      {
        IRCUser *pUser = new IRCUser(BuddyName);

        if (pUser)
        {
          pUser->m_CustomGroupID = pGroup->m_GroupID;
#ifdef PREF_DEBUG
          sys_Printf(BIC_XML,"Buddy, Name = \"%s\"\n",pUser->m_Nick);
#endif
          pGroup->AddUser(pUser);
        }
        free(BuddyName);
      }
      else
      {
        sys_Printf(BIC_XML,"Error parsing <buddy>\n");
      }
    }
    pNode = pNode->next;
  }
}

void Prefs::XML_LoadBuddyGroups(xmlNodePtr pNode)
{  
  char *GroupName, *GroupID, *NetworkID;//, *Flagsstr;

  while (pNode != NULL)
  {
    if (pNode->type == XML_ELEMENT_NODE && stricmp((char *)pNode->name,"group") == 0)
    {
      // Get the properties!
      GroupName = (char*)xmlGetProp(pNode, (xmlChar*)"name");
      GroupID   = (char*)xmlGetProp(pNode, (xmlChar*)"id");
      NetworkID = (char*)xmlGetProp(pNode, (xmlChar*)"network");
      //Flagsstr  = (char*)xmlGetProp(pNode, (xmlChar*)"flags");
      
      if (GroupName && GroupID && NetworkID)
      {
        // TODO: verify GroupID is unique

        IRCUserGroup *pGroup = new IRCUserGroup(GroupName,atoi(GroupID),atoi(NetworkID));

        if (pGroup)
        {
#ifdef PREF_DEBUG
          sys_Printf(BIC_XML,"BuddyGroup, Name = \"%s\"\n",pGroup->m_GroupName);
#endif
          // Override the default flags
          //if (Flagsstr)
          //  pGroup->m_Flags = atoi(Flagsstr);

          m_CustomUserGroups.Add(pGroup);

          XML_LoadBuddies(pNode->children,pGroup);

        }
      }
      else
      {
        sys_Printf(BIC_XML,"Error parsing <group>, Name = \"%s\"\n",UseEmptyString(GroupName));
      }
      if (GroupName) free(GroupName);
      if (GroupID) free(GroupID);
      if (NetworkID) free(NetworkID);
      // if (Flagstr) free(Flagstr);
    }
    pNode = pNode->next;
  }
}

void Prefs::XML_LoadCommandProfiles(xmlNodePtr pNode)
{  
  char *name,*commands;
  while (pNode != NULL)
  {
    if (pNode->type == XML_ELEMENT_NODE && stricmp((char*)pNode->name,"commandprofile") == 0)
    {
      name = (char*)xmlGetProp(pNode, (xmlChar*)"name");
      commands = (char*)xmlGetProp(pNode, (xmlChar*)"commands");

      if (commands)
      {
	    int bufferSize = strlen(commands) * 2 + 1;
        char *UnescapedString = (char *)malloc(bufferSize);
		if (UnescapedString) {
			UnescapeString(UnescapedString,bufferSize,commands);
		}
		free(commands);
		commands = UnescapedString;
      }

      if (name && commands)
      {

        CCommandProfile *pCommandProfile = new CCommandProfile();
        if (pCommandProfile)
        {
          pCommandProfile->m_Name = name;
          pCommandProfile->m_Commands = commands;

          m_CommandProfileList.Add(pCommandProfile);
        }
      }
      else
      {
        if (name) free(name);
        if (commands) free(commands);
      }
    }
    pNode = pNode->next;
  }
}

void Prefs::XML_LoadIgnores(xmlNodePtr pNode)
{  
  char *prop, *Nick, *HostMask, *Notes;
  int NetworkID = -1, Flags = IIF_NONE;
  xmlNodePtr pChildNode;
  IgnoreInfo *pIgnoreInfo = NULL;

  while (pNode != NULL)
  {
    if (pNode->type == XML_ELEMENT_NODE && stricmp((char*)pNode->name,"ignore") == 0)
    {
      // Get the properties!
      Nick = (char *)xmlGetProp(pNode, (xmlChar*)"nick");
      HostMask = (char *)xmlGetProp(pNode, (xmlChar*)"hostmask");
      Notes = (char *)xmlGetProp(pNode, (xmlChar*)"notes");

      prop = (char*)xmlGetProp(pNode, (xmlChar*)"networkid");
      if (prop)
      {
        NetworkID = atoi(prop);
        free(prop);
      }
      prop = (char*)xmlGetProp(pNode, (xmlChar*)"flags");
      if (prop)
      {
        Flags = atoi(prop);
        free(prop);
      }

      if (Nick && *Nick)
        pIgnoreInfo = new IgnoreInfo(Nick, HostMask, Notes, NetworkID, Flags);

      if (pIgnoreInfo)
      {
        pChildNode = pNode->children;

        while (pChildNode != NULL)
        {
          if (pChildNode->type == XML_ELEMENT_NODE)
          {
#ifdef PREF_DEBUG
            sys_Printf(BIC_XML,"pChildNode->name = \"%s\"\n",pChildNode->name);
#endif
            if (stricmp((char*)pChildNode->name,"channel") == 0)
            {
              prop = (char*)xmlGetProp(pChildNode, (xmlChar*)"name");
              if (prop)
              {
                pIgnoreInfo->AddChannel(strdup(prop));
                free(prop);
              }
            }
          }
          pChildNode = pChildNode->next;
        }
        pIgnoreInfo->Validate();
        m_IgnoreList.Add(pIgnoreInfo);
      }
      else
      {
        sys_Printf(BIC_XML,"Error parsing <ignores>, Nick = \"%s\"\n",UseEmptyString(Nick));
      }
      if (Nick) free(Nick);
      if (HostMask) free(HostMask);
    }
    pNode = pNode->next;
  }
}

void Prefs::XML_LoadWindowPrefs(xmlNodePtr pNode)
{
  char *prop;
  xmlNodePtr pChildNode;
  WindowPrefs *pWindowPrefs;

  while (pNode != NULL)
  {
    if (pNode->type == XML_ELEMENT_NODE && stricmp((char*)pNode->name,"windowpref") == 0)
    {
      pWindowPrefs = new WindowPrefs(this);

      if (pWindowPrefs)
      {
        prop = (char*)xmlGetProp(pNode, (xmlChar*)"type");
        if (prop)
        {
          pWindowPrefs->m_WindowType = atoi(prop);
          free(prop);
        } 
        prop = (char*)xmlGetProp(pNode, (xmlChar*)"networkid");
        if (prop)
        {
          pWindowPrefs->m_NetworkID = atoi(prop);
          free(prop);
        } 
        prop = (char*)xmlGetProp(pNode, (xmlChar*)"flags");
        if (prop)
        {
          pWindowPrefs->m_Flags = atoi(prop);
          free(prop);
        } 
        prop = (char*)xmlGetProp(pNode, (xmlChar*)"positions");
        if (prop)
        {
          int x,y,w,h;
          if (sscanf(prop,"%d,%d,%d,%d",&x,&y,&w,&h) == 4)
            pWindowPrefs->m_WindowPositions.Set(x,y,w,h);
          free(prop);
        } 

        switch(pWindowPrefs->m_WindowType)
        {
          case CWTYPE_CHANNEL:
          case CWTYPE_QUERY:
            pWindowPrefs->m_WindowName = (char*)xmlGetProp(pNode, (xmlChar*)"name");
            break;
        }

        pChildNode = pNode->children;

        while (pChildNode != NULL)
        {
          if (pChildNode->type == XML_ELEMENT_NODE)
          {
#ifdef PREF_DEBUG
            sys_Printf(BIC_XML,"pChildNode->name = \"%s\"\n",pChildNode->name);
#endif
            if (stricmp((char*)pChildNode->name,"colors") == 0)
            {
              XML_LoadColors(pChildNode->children, pWindowPrefs->m_ColorPrefs);
            }
          }
          pChildNode = pChildNode->next;
        }

        // TODO: some validation before adding.
        m_WindowPrefsList.Add(pWindowPrefs);
      }

      
    }
    pNode = pNode->next;
  }
}

void Prefs::XML_LoadFonts(xmlNodePtr pNode)
{  
  char *prop,*Description;
  int FontID;
  while (pNode != NULL)
  {
    if (pNode->type == XML_ELEMENT_NODE && stricmp((char*)pNode->name,"font") == 0)
    {
      // Get the properties!
      FontID = -1;
      Description = (char *)xmlGetProp(pNode, (xmlChar*)"description");
      if (Description)
      {
        for (int i = 0 ; i < PREF_FONT_MAX ; i++)
        {
          if (stricmp(Description,PREF_FONT_NAME(i)) == 0)
          {
            FontID = i;
            break;
          }
        }
      }
      if (FontID != -1)
      {
        prop = (char*)xmlGetProp(pNode, (xmlChar*)"name");
        if (prop)
        {
          free(m_FontPrefs[FontID].m_Name); // free the old one.
          m_FontPrefs[FontID].m_Name = prop;

          // only bother with the size if we got a font name
          // we can live with the default/last used size if needed.
          prop = (char*)xmlGetProp(pNode, (xmlChar*)"size");
          if (prop)
          {
            m_FontPrefs[FontID].m_Size = atoi(prop);
            free(prop);
          }

          BOOL tmpbool;
          m_FontPrefs[FontID].m_Flags = FIF_NONE;

          prop = (char*)xmlGetProp(pNode, (xmlChar*)"bold");
          if (prop)
          {
            SetBooleanPref(prop,&tmpbool);
            if (tmpbool) 
              m_FontPrefs[FontID].m_Flags |= FIF_BOLD;
            free(prop);
          }

          prop = (char*)xmlGetProp(pNode, (xmlChar*)"italic");
          if (prop)
          {
            SetBooleanPref(prop,&tmpbool);
            if (tmpbool) 
              m_FontPrefs[FontID].m_Flags |= FIF_ITALIC;
            free(prop);
          }

          prop = (char*)xmlGetProp(pNode, (xmlChar*)"underline");
          if (prop)
          {
            SetBooleanPref(prop,&tmpbool);
            if (tmpbool) 
              m_FontPrefs[FontID].m_Flags |= FIF_UNDERLINE;
            free(prop);
          }

#ifdef ZANN_FONTS
          prop = (char*)xmlGetProp(pNode, (xmlChar*)"script");
          if (prop)
          {
            for (i = 0 ; i < FONT_SCRIPTS_LAST; i++)
            {
              if (stricmp(prop,g_FontPrefScriptNames[i]) == 0)
              {
                m_FontPrefs[FontID].m_Script = g_FontPrefScriptIDs[i];
                break;
              }
            }
            free(prop);
          }
#endif
        }
      }
      else
      {
        sys_Printf(BIC_XML,"Error parsing <FontList>, Description = \"%s\"\n",UseEmptyString(Description));
      }
      if (Description) free(Description);
    }
    pNode = pNode->next;
  }
}

void Prefs::XML_LoadServerList(xmlNodePtr pNode,int DefaultFlags)
{  
  char *prop;

  while (pNode != NULL)
  {
    if (pNode->type == XML_ELEMENT_NODE && stricmp((char*)pNode->name,"server") == 0)
    {
      IRCServerDetails *pServerDetails = new IRCServerDetails;

      if (pServerDetails)
      {
        // Get the properties!
        pServerDetails->m_Name = (char*)xmlGetProp(pNode, (xmlChar*)"name");
        pServerDetails->m_Description = (char*)xmlGetProp(pNode, (xmlChar*)"description");
        pServerDetails->m_Password = (char*)xmlGetProp(pNode, (xmlChar*)"password"); // TODO: encrypt/decrypt
        prop = (char*)xmlGetProp(pNode, (xmlChar*)"priority");
        if (prop) 
        {
          pServerDetails->m_Priority  = atoi(prop);
          pServerDetails->m_Priority = max(min(pServerDetails->m_Priority,PRI_DISABLED),PRI_HIGHEST);
          free(prop);
        }
        else
          pServerDetails->m_Priority = PRI_NORMAL;

        prop = (char*)xmlGetProp(pNode, (xmlChar*)"port");
        if (prop) 
        {
          pServerDetails->m_Port = atoi(prop);
          free(prop);
        }
        prop = (char*)xmlGetProp(pNode, (xmlChar*)"network");
        if (prop)
        {
          pServerDetails->m_NetworkID = atoi(prop);
          free(prop);
        }
        prop = (char*)xmlGetProp(pNode, (xmlChar*)"flags");
        if (prop)
        {
          pServerDetails->m_Flags = atoi(prop);
          free(prop);
        }        


        if (pServerDetails->m_Description && pServerDetails->m_Name &&
            pServerDetails->m_Port)
        {
          if (DefaultFlags & ISDF_CUSTOMLIST)
          {
            // remove duplicates that are already loaded and use the
            // new details instead (allow profiles to override hydrairc.xml)
            IRCServerDetails *pOther;
            int i;
            for (i = 0; i < g_ServerDetailsList.GetSize(); i++ )
            {
              pOther = g_ServerDetailsList[i];

              // a comparison of port and address == duplicate
              // because the one in the user's profile will generally
              // have MORE information (or at least, up-to-date information)
              // e.g.  password change, different priority, etc.
              if (pOther->m_Port == pServerDetails->m_Port &&
                  stricmp(pOther->m_Name,pServerDetails->m_Name) == 0)
              {
                // this server is in the list already
                g_ServerDetailsList.Remove(pOther);
                delete pOther;

                break; // don't look for any more
              }
            }
          }
          pServerDetails->m_Flags |= DefaultFlags; // add the default flags

#ifdef PREF_DEBUG
          sys_Printf(BIC_XML,"Server, Name = \"%s\"\n",pServerDetails->m_Name);
#endif
          g_ServerDetailsList.Add(pServerDetails); // TODO: insert sorted by description (or name?), check for duplicates
        }
        else
        {
          sys_Printf(BIC_XML,"Error parsing <ServerList>, Name = \"%s\"\n",UseEmptyString(pServerDetails->m_Name));
          delete pServerDetails;
        }
      }
    }

    pNode = pNode->next;
  }
}

void Prefs::XML_LoadNetworkList(xmlNodePtr pNode, int DefaultFlags)
{  
  char *prop;

  while (pNode != NULL)
  {
    if (pNode->type == XML_ELEMENT_NODE && stricmp((char*)pNode->name,"network") == 0)
    {
      IRCNetworkDetails *pNetworkDetails = new IRCNetworkDetails;

      if (pNetworkDetails)
      {
        // Get the properties!
        pNetworkDetails->m_Name = (char*)xmlGetProp(pNode, (xmlChar*)"name");
        pNetworkDetails->m_IdentityName = (char*)xmlGetProp(pNode, (xmlChar*)"preferredidentityname");
        prop = (char*)xmlGetProp(pNode, (xmlChar*)"id");
        if (prop)
        {
          pNetworkDetails->m_ID = atoi(prop);
          free(prop);
        }        
        prop = (char*)xmlGetProp(pNode, (xmlChar*)"flags");
        if (prop)
        {
          pNetworkDetails->m_Flags = atoi(prop);
          free(prop);
        }        

        if (pNetworkDetails->m_Name)
        {
          if (DefaultFlags & INDF_CUSTOMLIST)
          {
            // remove duplicates that are already loaded and use the
            // new details instead (allow profiles to override hydrairc.xml)
            IRCNetworkDetails *pOther;
            int i;
            for (i = 0; i < g_NetworkList.GetSize(); i++ )
            {
              pOther = g_NetworkList[i];

              // a comparison of ID and NAME == duplicate

              if (pOther->m_ID == pNetworkDetails->m_ID &&
                  stricmp(pOther->m_Name,pNetworkDetails->m_Name) == 0)
              {
                // this network is in the list already
                g_NetworkList.Remove(pOther);
                delete pOther;

                break; // don't look for any more
              }
            }
          }

          pNetworkDetails->m_Flags |= DefaultFlags; // add the default flags
          // verify NetworkID is unique
          if (GetNetworkDetails(pNetworkDetails->m_ID) != NULL)
          {
            sys_Printf(BIC_WARNING,"Network, Name = \"%s\" already present in HydraIRC.xml, ignoring!\n",pNetworkDetails->m_Name);
            delete pNetworkDetails;
          }
          else
          {
#ifdef PREF_DEBUG
            sys_Printf(BIC_XML,"Network, Name = \"%s\"\n",pNetworkDetails->m_Name);
#endif
            g_NetworkList.Add(pNetworkDetails); // TODO: insert sorted
          }
        }
        else
        {
          sys_Printf(BIC_XML,"Error parsing <NetworkList>, Name = \"%s\"\n",UseEmptyString(pNetworkDetails->m_Name));
          delete pNetworkDetails;
        }
      }
    }

    pNode = pNode->next;
  }
}

// void Prefs::LoadMiscPrefs(PrefsCollection_t *pPC, xmlNodePtr pNode) // prefs collection
void Prefs::XML_LoadMiscPrefs(xmlNodePtr pNode)
{  
  char *prop,*value;
  int i;
  BOOL done = FALSE;

  while (pNode != NULL)
  {
    if (pNode->type == XML_ELEMENT_NODE && stricmp((char*)pNode->name,"pref") == 0)
    {
      done = FALSE;
      prop = (char*)xmlGetProp(pNode, (xmlChar*)"name");

      for (i = 0; i < 4 && !done; i ++)
      {
        switch(i)
        {
          case 0:
            value = (char*)xmlGetProp(pNode, (xmlChar*)"boolean");
            break;
          case 1:
            value = (char*)xmlGetProp(pNode, (xmlChar*)"integer");
            break;
          case 2:
            value = (char*)xmlGetProp(pNode, (xmlChar*)"path");
            break;
          case 3:
            value = (char*)xmlGetProp(pNode, (xmlChar*)"string");
            break;
        }

        if (value)
        {
          // done = SetPref(pPC, prop,value); // prefscollection
          done = SetPref(prop,value);
          free(value);
/*
          switch(i)
          {
            case 2:
            case 3:
              // don't free the string pref if it was used.
              if (done)
                break;
              // and follow through...
            case 0:
            case 1:
              // free the value string
              free(value);
              break;
          }
*/
        }
      }

      if (!done)
      {
        sys_Printf(BIC_XML,"Error parsing <Misc>, Name = \"%s\"\n",UseEmptyString(prop));
      }
      if (prop) free(prop);
    }

    pNode = pNode->next;
  }
}

// void Prefs::XML_LoadColors(PrefsCollection_t *pPC, xmlNodePtr pNode)
void Prefs::XML_LoadColors(xmlNodePtr pNode, COLORREF *pColorPrefs)
{  
  char *prop,*color_val;
  int i;//,prefindex;
  BOOL done = FALSE;

  while (pNode != NULL)
  {
    if (pNode->type == XML_ELEMENT_NODE && stricmp((char*)pNode->name,"item") == 0)
    {
      prop = (char*)xmlGetProp(pNode, (xmlChar*)"name");
      color_val = (char*)xmlGetProp(pNode, (xmlChar*)"color");
      done = FALSE;

      for (i = 0 ; i < PREF_COLOR_MAX && !done ; i ++)
      {
        if (stricmp(prop,PREF_COLOR_NAME(i)) == 0)
        {
          /* PrefsCollections
          prefindex = GetPrefIndexFromID(pPC,PT_COLOR,PREF_COLOR_FIRST + i);
          if (prefindex != -1)
          {
            done = SetIntPref(color_val,(int *)(&pPC->m_pColors[prefindex]));
            if (done)
              pPC->m_pColors[prefindex] = RGBVALTOCOLORREF(pPC->m_pColors[prefindex]);
          }
          */
          done = SetIntPref((int&)pColorPrefs[i],color_val);
          if (done)
            pColorPrefs[i] = RGBVALTOCOLORREF(pColorPrefs[i]); // convert from COLORVAL to RGB
        }
      }

      if (!done)
      {
        sys_Printf(BIC_XML,"Error parsing <Colors>, Name = \"%s\"\n",UseEmptyString(prop));
      }

      if (prop) free(prop);
      if (color_val) free(color_val);

    }

    pNode = pNode->next;
  }
}

void Prefs::LoadPrefs( void )
{
  SetCurrentDirectory(g_HomeDir);

  xmlDocPtr pDoc;
  xmlNodePtr pNode;  
  char *FileName = "HydraIRC.xml";

  xmlSetGenericErrorFunc(NULL, HandleXMLError);
  pDoc = xmlParseFile(FileName);
  // TODO: add error checking for failed load.  
  if (pDoc)
  {   
    sys_Printf(BIC_INFO,"Loaded Prefs - \"%s\"\n",FileName);

    pNode = pDoc->children;
    while (pNode != NULL && stricmp((const char*)pNode->name, "HydraIRC"))
      pNode=pNode->next;
    
    if (pNode)
    {
#ifdef PREF_DEBUG
      sys_Printf(BIC_XML,"pNode->name = \"%s\"\n",pNode->name);
#endif

      pNode = pNode->children;

      while (pNode != NULL)
      {
        if (pNode->type == XML_ELEMENT_NODE)
        {
#ifdef PREF_DEBUG
          sys_Printf(BIC_XML,"pNode->name = \"%s\"\n",pNode->name);
#endif
          // Load global prefs
          if (stricmp((char*)pNode->name,"networklist") == 0) XML_LoadNetworkList(pNode->children,INDF_MASTERLIST);
          if (stricmp((char*)pNode->name,"serverlist") == 0) XML_LoadServerList(pNode->children,ISDF_MASTERLIST);

          // User profile can over-rides these
          if (stricmp((char*)pNode->name,"fonts") == 0) XML_LoadFonts(pNode->children);
          if (stricmp((char*)pNode->name,"colors") == 0) XML_LoadColors(pNode->children,m_ColorPrefs);
          if (stricmp((char*)pNode->name,"misc") == 0) XML_LoadMiscPrefs(pNode->children);
          /*
          if (stricmp((char*)pNode->name,"colors") == 0) XML_LoadColors(&m_PC,pNode->children);
          if (stricmp((char*)pNode->name,"misc") == 0) XML_LoadMiscPrefs(&m_PC,pNode->children);
          */
        }
        pNode = pNode->next;
      }
    }
    xmlFreeDoc(pDoc);
    sys_Printf(BIC_INFO,"Closed Prefs - \"%s\"\n",FileName);
  }
  else
  {
    sys_Printf(BIC_WARNING,"Failed to load XML file - \"%s\" - Have you checked it for corruption?\n",FileName);
  }
}

/*
void Prefs::LoadChannelPrefsCollections(xmlNodePtr pNode, ChannelPrefs *pChannelPrefs)
{
  while (pNode != NULL)
  {
    if (pNode->type == XML_ELEMENT_NODE)
    {
      if (stricmp((char*)pNode->name,"colors") == 0) XML_LoadColors(&pChannelPrefs->m_PC,pNode->children);
      if (stricmp((char*)pNode->name,"misc") == 0) XML_LoadMiscPrefs(&pChannelPrefs->m_PC,pNode->children);
    }
    pNode = pNode->next;
  }
}

void Prefs::LoadChannelPrefs(xmlNodePtr pNode)
{  
  char *Name, *NetworkID;

  while (pNode != NULL)
  {
    if (pNode->type == XML_ELEMENT_NODE && stricmp((char *)pNode->name,"channel") == 0)
    {
      // Get the properties!
      Name      = (char*)xmlGetProp(pNode, (xmlChar*)"name");
      NetworkID = (char*)xmlGetProp(pNode, (xmlChar*)"network");

      if (Name && NetworkID)
      {
        // TODO: verify GroupID is unique

        ChannelPrefs *pChannelPrefs = new ChannelPrefs(this,Name,atoi(NetworkID));

        if (pChannelPrefs)
        {
#ifdef PREF_DEBUG
          sys_Printf(BIC_XML,"ChannelPref, Name = \"%s\"\n",pChannelPrefs->m_GroupName);
#endif

          m_ChannelPrefList.Add(pChannelPrefs);

          LoadChannelPrefsCollections(pNode->children,pChannelPrefs);

        }
      }
      else
      {
        sys_Printf(BIC_XML,"Error parsing <channel>, Name = \"%s\"\n",UseEmptyString(Name));
      }
      if (Name) free(Name);
      if (NetworkID) free(NetworkID);
    }
    pNode = pNode->next;
  }
}
*/

OutputThemeItem_t *Prefs::LoadThemeByName(char *ThemeName)
{
  // scan the themes directories for .xml files, load them up and find the theme were after. *sigh*

  OutputThemeItem_t *pTheme = NULL;
  xmlDocPtr pDoc;
  xmlNodePtr pNode;  
  char *prop,*version,*author,*date;


  WIN32_FIND_DATA FileData;
  HANDLE hSearch = NULL;

  char ThemeFile[MAX_PATH];
  char ThemeFilter[MAX_PATH];
  char *ThemesFolder = "Themes\\"; // note trailing "\"
  BOOL Done = FALSE;
  
  strcpy(ThemeFilter,ThemesFolder);
  strcat(ThemeFilter,"*.xml");
  
  SetCurrentDirectory(g_HomeDir);

  // Scan "Themes\" directory for .xml files

  hSearch = FindFirstFile(ThemeFilter, &FileData);

  if (hSearch == INVALID_HANDLE_VALUE) 
  {
    Done = TRUE;
  }

  while (!Done)
  {
    // build the filename
    _snprintf(ThemeFile,MAX_PATH,"%s%s",ThemesFolder,FileData.cFileName);
    sys_Printf(BIC_INFO,"Searching File - \"%s\" for theme \"%s\"\n",ThemeFile, ThemeName);

    pDoc = xmlParseFile(ThemeFile);
    if (pDoc)
    {   
      sys_Printf(BIC_INFO,"Loaded XML File - \"%s\"\n",ThemeFile);

      pNode = pDoc->children;
      do
      {
        while (pNode != NULL && stricmp((const char*)pNode->name, "HydraIRCThemeCollection"))
          pNode=pNode->next;

        if (pNode)
        {
#ifdef PREF_DEBUG
          sys_Printf(BIC_XML,"pNode->name = \"%s\"\n",pNode->name);
#endif

          prop = (char*)xmlGetProp(pNode, (xmlChar*)"name");
          author = (char*)xmlGetProp(pNode, (xmlChar*)"author");
          version = (char*)xmlGetProp(pNode, (xmlChar*)"version");
          date = (char*)xmlGetProp(pNode, (xmlChar*)"date");

          sys_Printf(BIC_INFO,"Theme Collection - \"%s\" by \"%s %s %s\"\n",prop,UseEmptyString(author),UseEmptyString(version),UseEmptyString(date));

          if (prop) free(prop);
          if (author) free(author);
          if (version) free(version);
          if (date) free(date);

          pNode = pNode->children;
          while (pNode != NULL && !Done)
          {
            if (pNode->type == XML_ELEMENT_NODE)
            {
#ifdef PREF_DEBUG
              sys_Printf(BIC_XML,"pNode->name = \"%s\"\n",pNode->name);
#endif
              if (stricmp((char*)pNode->name,"theme") == 0)
              {
                prop = (char*)xmlGetProp(pNode, (xmlChar*)"name");

                if (prop)
                {
                  // is this the theme we want?
                  if (stricmp(prop,ThemeName) == 0)
                  {
                    pTheme = XML_LoadTheme(pNode->children);
                    sys_Printf(pTheme == NULL ? BIC_ERROR : BIC_INFO,"Loading theme \"%s\" from \"%s\" %s\n", prop, ThemeFile, pTheme == NULL ? "Failed" : "Completed");
                    Done = TRUE;
                  }
                  free(prop);
                }
              }
            }
            pNode = pNode->next;
          }

        }
      } while (!Done && pNode != NULL);
      xmlFreeDoc(pDoc);
      sys_Printf(BIC_INFO,"Closed XML File - \"%s\"\n",ThemeFile);
    }
    else
    {
      sys_Printf(BIC_WARNING,"Failed to load XML file - \"%s\" - Have you checked it for corruption?\n",ThemeFile);
    }

    if (!Done)
    {
      if (!FindNextFile(hSearch, &FileData) != 0) 
      {
        if (GetLastError() == ERROR_NO_MORE_FILES) 
        {
          Done = TRUE;
        }
        else
        {
          Done = TRUE;
          // some other error..
        }
      }
    }
  }

  FindClose(hSearch);

  return pTheme;
}

ColorPrefItem_t *Prefs::LoadColorSchemeByName(char *SchemeName)
{
  // scan the themes directories for .xml files, load them up and find the theme were after. *sigh*

  ColorPrefItem_t *pScheme = NULL;
  xmlDocPtr pDoc;
  xmlNodePtr pNode;  
  char *prop,*version,*author,*date;


  WIN32_FIND_DATA FileData;
  HANDLE hSearch = NULL;

  char SchemeFile[MAX_PATH];
  char SchemeFilter[MAX_PATH];
  char *SchemesFolder = "ColorSchemes\\"; // note trailing "\"
  BOOL Done = FALSE;
  
  strcpy(SchemeFilter,SchemesFolder);
  strcat(SchemeFilter,"*.xml");
  
  SetCurrentDirectory(g_HomeDir);

  // Scan "ColorSchemes\" directory for .xml files

  hSearch = FindFirstFile(SchemeFilter, &FileData);

  if (hSearch == INVALID_HANDLE_VALUE) 
  {
    Done = TRUE;
  }

  while (!Done)
  {
    // build the filename
    _snprintf(SchemeFile,MAX_PATH,"%s%s",SchemesFolder,FileData.cFileName);
    sys_Printf(BIC_INFO,"Searching File - \"%s\" for theme \"%s\"\n",SchemeFile, SchemeName);

    pDoc = xmlParseFile(SchemeFile);
    if (pDoc)
    {   
      sys_Printf(BIC_INFO,"Loaded XML File - \"%s\"\n",SchemeFile);

      pNode = pDoc->children;
      do
      {
        while (pNode != NULL && stricmp((const char*)pNode->name, "HydraIRCColorSchemeCollection"))
          pNode=pNode->next;

        if (pNode)
        {
#ifdef PREF_DEBUG
          sys_Printf(BIC_XML,"pNode->name = \"%s\"\n",pNode->name);
#endif

          prop = (char*)xmlGetProp(pNode, (xmlChar*)"name");
          author = (char*)xmlGetProp(pNode, (xmlChar*)"author");
          version = (char*)xmlGetProp(pNode, (xmlChar*)"version");
          date = (char*)xmlGetProp(pNode, (xmlChar*)"date");

          sys_Printf(BIC_INFO,"Color Scheme Collection - \"%s\" by \"%s %s %s\"\n",prop,UseEmptyString(author),UseEmptyString(version),UseEmptyString(date));

          if (prop) free(prop);
          if (author) free(author);
          if (version) free(version);
          if (date) free(date);

          pNode = pNode->children;
          while (pNode != NULL && !Done)
          {
            if (pNode->type == XML_ELEMENT_NODE)
            {
#ifdef PREF_DEBUG
              sys_Printf(BIC_XML,"pNode->name = \"%s\"\n",pNode->name);
#endif
              if (stricmp((char*)pNode->name,"colorscheme") == 0)
              {
                prop = (char*)xmlGetProp(pNode, (xmlChar*)"name");

                if (prop)
                {
                  // is this the theme we want?
                  if (stricmp(prop,SchemeName) == 0)
                  {
                    pScheme = XML_LoadColorScheme(pNode->children);
                    Done = TRUE;
                  }
                  free(prop);
                }
              }
            }
            pNode = pNode->next;
          }

        }
      } while (!Done && pNode != NULL);
      xmlFreeDoc(pDoc);
      sys_Printf(BIC_INFO,"Closed XML File - \"%s\"\n",SchemeFile);
    }
    else
    {
      sys_Printf(BIC_WARNING,"Failed to load XML file - \"%s\" - Have you checked it for corruption?\n",SchemeFile);
    }

    if (!Done)
    {
      if (!FindNextFile(hSearch, &FileData) != 0) 
      {
        if (GetLastError() == ERROR_NO_MORE_FILES) 
        {
          Done = TRUE;
        }
        else
        {
          Done = TRUE;
          // some other error..
        }
      }
    }
  }

  FindClose(hSearch);

  return pScheme;
}

/*
TODO: this needs testing on the following weird tree
just to see what the tree ends up like 
(can you even have text nodes with children??, do they get merged?)

it currently works for it's purpose (see Prefs::LoadFavorites()), 
but i would do more testing before putting in general use

doc
+ text node
|-+ text node
| |-+ text node
| | | text node
| | | text node
| | - element node
| |
| | text node
| | text node
| |
| |-+ text node
| | | text node
| | | text node
| | - element node
| |
| - text node
|
| text node
| element node
- text node

*/
void xmlStripTextNodes(xmlNodePtr *pNode)
{
  xmlNodePtr pSearchNode = *pNode;

  while (pSearchNode)
  {
    if (pSearchNode->type == XML_TEXT_NODE)
    {
      xmlNodePtr pTemp = pSearchNode->next;
      if (*pNode == pSearchNode)
        *pNode = pTemp;

      xmlUnlinkNode(pSearchNode);
      xmlFreeNode(pSearchNode);
      pSearchNode = pTemp;
    }
    else
    {
      if (pSearchNode->children)
        xmlStripTextNodes(&pSearchNode->children);

      pSearchNode=pSearchNode->next;
    }
  }
}

void Prefs::XML_LoadFavorites(xmlNodePtr pNode)
{ 
  if (m_Favorites)
    xmlFreeNodeList(m_Favorites);

  m_Favorites = xmlCopyNodeList(pNode);

  // we keep a copy of all the nodes, but we don't want the text nodes
  // so that the formatting is saved correctly when the file is written
  // back to disk.


  // pass the favorites pointer so it can be updated (the first node is
  // often a text node...)

  xmlStripTextNodes(&m_Favorites); 
}

ColorPrefItem_t *Prefs::XML_LoadColorScheme(xmlNodePtr pNode)
{ 
  int ItemCount = 0;
  char *prop,*color_val;
  BOOL Found;

  ColorPrefItem_t *pSchemeBase, *pSchemeItem;

  xmlNodePtr pItemNode = pNode;
  while (pItemNode != NULL)
  {
    if (pItemNode->type == XML_ELEMENT_NODE)
      ItemCount++;
    pItemNode = pItemNode->next;
  }

  if (!ItemCount)
    return NULL;

  pSchemeBase = (ColorPrefItem_t *) malloc(sizeof(ColorPrefItem_t) * (ItemCount + 1)); // +1 for list terminator.

  if (!pSchemeBase)
    return NULL;

  pSchemeItem = pSchemeBase; // point pSchemeItem to first item in array.

  while (pNode != NULL && ItemCount)
  {
    if (pNode->type == XML_ELEMENT_NODE && stricmp((char*)pNode->name,"item") == 0)
    {
      pSchemeItem->m_PrefID = -1;
      // Get the properties!
      prop = (char*)xmlGetProp(pNode, (xmlChar*)"name");
      color_val = (char*)xmlGetProp(pNode, (xmlChar*)"color");

      Found = FALSE;
      if (prop && color_val)
      {
        for (int i = 0 ; i < PREF_COLOR_MAX && !Found ; i ++)
        {
          if (stricmp(prop,PREF_COLOR_NAME(i)) == 0)
          {
            pSchemeItem->m_PrefID = PREF_COLOR_FIRST + i;
            Found = SetIntPref((int&)pSchemeItem->m_Value,color_val);
            if (Found)
              pSchemeItem->m_Value = RGBVALTOCOLORREF(pSchemeItem->m_Value);
          }
        }
      }

      if (!Found)
      {
        sys_Printf(BIC_XML,"Error parsing <Colors>, Name = \"%s\"\n",UseEmptyString(prop));
      }

      if (prop) free(prop);
      if (color_val) free(color_val);

      if (pSchemeItem->m_PrefID != -1)
      {
        pSchemeItem++;
        pSchemeItem->m_PrefID = -1; // set the end of the list.
        ItemCount--;
      }
    }
    pNode = pNode->next;
  }

  return pSchemeBase;

}

OutputThemeItem_t *Prefs::XML_LoadTheme(xmlNodePtr pNode)
{ 
  int ItemCount = 0;
  char *prop;

  OutputThemeItem_t *pThemeBase, *pThemeItem;

  xmlNodePtr pItemNode = pNode;
  while (pItemNode != NULL)
  {
    if (pItemNode->type == XML_ELEMENT_NODE)
      ItemCount++;
    pItemNode = pItemNode->next;
  }

  if (!ItemCount)
    return NULL;

  pThemeBase = (OutputThemeItem_t *) malloc(sizeof(OutputThemeItem_t) * (ItemCount + 1)); // +1 for list terminator.

  if (!pThemeBase)
    return NULL;

  pThemeItem = pThemeBase; // point pThemeItem to first item in array.

  while (pNode != NULL && ItemCount)
  {
    if (pNode->type == XML_ELEMENT_NODE && stricmp((char*)pNode->name,"item") == 0)
    {
      pThemeItem->m_WindowType = -1;
      // Get the properties!
      prop = (char *)xmlGetProp(pNode, (xmlChar*)"appliesto");
      if (prop)
      {
        pThemeItem->m_WindowType = 0;
        for (int i = 0 ; prop[i] ; i++)
        {
          switch(toupper(prop[i]))
          {
            case 'S':
              pThemeItem->m_WindowType |= THEME_SERVER;
              break;
            case 'C':
              pThemeItem->m_WindowType |= THEME_CHANNEL;
              break;
            case 'Q':
              pThemeItem->m_WindowType |= THEME_QUERY;
              break;
            case 'D':
              pThemeItem->m_WindowType |= THEME_DCCCHAT;
              break;
          }
        }
        free(prop);

        prop = (char *)xmlGetProp(pNode, (xmlChar*)"itemid");
        if (prop)
        {
          pThemeItem->m_IRCCommandID = atoi(prop);
          free(prop);

          prop = (char *)xmlGetProp(pNode, (xmlChar*)"formatstring");
          if (prop)
          {
            char UnescapedString[512];
            UnescapeString(UnescapedString,sizeof(UnescapedString),prop);
            pThemeItem->m_FormatStr = strdup(UnescapedString);
            free(prop);
          }
        }

      }

      if (pThemeItem->m_WindowType != -1)
      {
        pThemeItem++;
        pThemeItem->m_WindowType = -1; // set the end of the list.
        ItemCount--;
      }
    }
    pNode = pNode->next;
  }

  return pThemeBase;
}

/** Load a user profile
 *
 */
void Prefs::LoadProfile(char *ProfileName, char *FileName )
{
  xmlDocPtr pDoc;
  xmlNodePtr pNode;  
  char *filename;
  char *prop;
  BOOL Done = FALSE;

  ATLASSERT(ProfileName);
  if (!ProfileName)
    return;

  if (FileName)
    filename = strdup(FileName);
  else
  {
    filename = (char *)malloc(8 + strlen(ProfileName) + 5); // "Profile_<filename>.xml\0"
    sprintf(filename,"Profile_%s.xml",ProfileName);
  }

  sys_Printf(BIC_INFO,"Loading Profile - \"%s\" from \"%s\"\n",ProfileName, filename);

  xmlSetGenericErrorFunc(NULL, HandleXMLError); // TODO: move this!

  SetCurrentDirectory(g_HomeDir);

  pDoc = xmlParseFile(filename);

  if (pDoc)
  {   
    sys_Printf(BIC_INFO,"Loaded Profile - \"%s\"\n",filename);

    pNode = pDoc->children;
    do
    {
      while (pNode != NULL && stricmp((const char*)pNode->name, "HydraIRCProfile"))
        pNode=pNode->next;
      
      if (pNode)
      {
#ifdef PREF_DEBUG
        sys_Printf(BIC_XML,"pNode->name = \"%s\"\n",pNode->name);
#endif

        prop = (char*)xmlGetProp(pNode, (xmlChar*)"name");
        if (!prop || stricmp(prop,ProfileName) != 0)
        {
#ifdef PREF_DEBUG
          sys_Printf(BIC_XML,"Profile Name Mismatch - \"%s\" != \"%s\"\n",prop,ProfileName);
#endif
          pNode = pNode->next;
        }
        else
        {
          Done = TRUE; // we found our profile, don't look for any more..
          pNode = pNode->children;

          while (pNode != NULL)
          {
            if (pNode->type == XML_ELEMENT_NODE)
            {
#ifdef PREF_DEBUG
              sys_Printf(BIC_XML,"pNode->name = \"%s\"\n",pNode->name);
#endif

              if (stricmp((char*)pNode->name,"networklist") == 0) XML_LoadNetworkList(pNode->children,INDF_CUSTOMLIST);
              if (stricmp((char*)pNode->name,"serverlist") == 0) XML_LoadServerList(pNode->children,ISDF_CUSTOMLIST);

              if (stricmp((char*)pNode->name,"fonts") == 0) XML_LoadFonts(pNode->children);
              if (stricmp((char*)pNode->name,"buddygroups") == 0) XML_LoadBuddyGroups(pNode->children);
              if (stricmp((char*)pNode->name,"identities") == 0) XML_LoadUserIdentities(pNode->children);
              if (stricmp((char*)pNode->name,"notifications") == 0) XML_LoadNotifications(pNode->children);
              if (stricmp((char*)pNode->name,"colors") == 0) XML_LoadColors(pNode->children,m_ColorPrefs);
              if (stricmp((char*)pNode->name,"misc") == 0) XML_LoadMiscPrefs(pNode->children);
              if (stricmp((char*)pNode->name,"commandprofiles") == 0) XML_LoadCommandProfiles(pNode->children);
              if (stricmp((char*)pNode->name,"favorites") == 0) XML_LoadFavorites(pNode->children);
              if (stricmp((char*)pNode->name,"ignores") == 0) XML_LoadIgnores(pNode->children);
              if (stricmp((char*)pNode->name,"windowprefs") == 0) XML_LoadWindowPrefs(pNode->children);

              /*
              if (stricmp((char*)pNode->name,"channelprefs") == 0) XML_LoadChannelPrefs(pNode->children);
              if (stricmp((char*)pNode->name,"colors") == 0) XML_LoadColors(&m_PC,pNode->children);
              if (stricmp((char*)pNode->name,"misc") == 0) XML_LoadMiscPrefs(&m_PC,pNode->children);
              */
            }
            pNode = pNode->next;
          }
        }
        if (prop) free(prop);
      }
    } while (!Done && pNode != NULL);
    xmlFreeDoc(pDoc);
    sys_Printf(BIC_INFO,"Closed Profile - \"%s\"\n",filename);
  }
  else
  {
    sys_Printf(BIC_WARNING,"Failed to load XML file - \"%s\" - Have you checked it for corruption?\n",filename);
  }
  free (filename);
}

// SAVING
//
/////////////////////////////////////////////////////////////////////////////

void Prefs::XML_SaveGeneralNotifications(xmlNodePtr pParentNode)
{
  xmlNodePtr pNode,pNewNode,pEventNode;  
  NotificationPrefInfo_t *pNPI;
  char numstr[10];
  int GroupID;
  BOOL ChildAdded;
  pNode = xmlNewChild(pParentNode, NULL, (xmlChar *)"general", NULL);


  for (int i = -1 ; i < m_CustomUserGroups.m_UserGroupList.GetSize(); i ++)
  {
    if (i == -1)
      GroupID = -2;
    else
      GroupID = m_CustomUserGroups.m_UserGroupList[i]->m_GroupID;

    ChildAdded = FALSE;

    for (int j = 0 ; j < m_NotificationPrefList.GetSize() ; j++)
    {
      pNPI = m_NotificationPrefList[j];

      // only looking for items that match the GroupID
      if (pNPI->m_GroupID != GroupID)
        continue;
      
      // create somewhere to put the child nodes, if we've not already done so.
      if (!ChildAdded)
      {
        ChildAdded = TRUE;
        pNewNode = xmlNewChild(pNode, NULL, (xmlChar *)"group", NULL);
        if (!pNewNode)
          continue;
        _snprintf(numstr,9,"%d",GroupID);
        xmlSetProp(pNewNode, (xmlChar *)"id", (xmlChar *)numstr);
      }

      pEventNode = xmlNewChild(pNewNode, NULL, (xmlChar *)"event", NULL);
      if (!pEventNode)
        continue;

      xmlSetProp(pEventNode, (xmlChar *)"name", (xmlChar *)g_NotificationEventNames[pNPI->m_EventCode]);
      _snprintf(numstr,9,"%d",pNPI->m_Flags);
      xmlSetProp(pEventNode, (xmlChar *)"flags", (xmlChar *)numstr);
      if (pNPI->m_Flags & NE_FLAG_SOUNDFILE)
      {
        xmlSetProp(pEventNode, (xmlChar *)"soundfilename", (xmlChar *)pNPI->m_FileName);
      }
      if (pNPI->m_Flags & NE_FLAG_SYSTEMSOUND)
      {
        _snprintf(numstr,9,"%d",pNPI->m_SoundID);
        xmlSetProp(pEventNode, (xmlChar *)"soundid", (xmlChar *)numstr);
      }

      // save the highlighting prefs.
      if (pNPI->m_MatchString)
        xmlSetProp(pEventNode, (xmlChar *)"matchstring", (xmlChar *)pNPI->m_MatchString);

      if (pNPI->m_HighlightFlags != HIGHLIGHT_NONE)
      {
        _snprintf(numstr,9,"%d",pNPI->m_HighlightFlags);
        xmlSetProp(pEventNode, (xmlChar *)"highlightflags", (xmlChar *)numstr);
      }

      if (pNPI->m_ForegroundColor != -1)
      {
        _snprintf(numstr,9,"#%06x",RGBVALTOCOLORREF(pNPI->m_ForegroundColor)); // convert from COLORVAL to RGB
        xmlSetProp(pEventNode, (xmlChar *)"foregroundcolor", (xmlChar *)numstr);
      }

      if (pNPI->m_BackgroundColor != -1)
      {
        _snprintf(numstr,9,"#%06x",RGBVALTOCOLORREF(pNPI->m_BackgroundColor)); // convert from COLORVAL to RGB
        xmlSetProp(pEventNode, (xmlChar *)"backgroundcolor", (xmlChar *)numstr);
      }


    }
  }

}

void Prefs::XML_SaveNotifications(xmlNodePtr pParentNode)
{
  xmlNodePtr pNode;  

  pNode = xmlNewChild(pParentNode, NULL, (xmlChar *)"notifications", NULL);
  if (pNode)
    XML_SaveGeneralNotifications(pNode);
  // TODO: save other notifications here
}

char *str_True = "true";
char *str_False = "false";

void Prefs::XML_SaveMiscPrefs(xmlNodePtr pParentNode)
{
  xmlNodePtr pNode,pNewNode;  
  char numstr[10];
  pNode = xmlNewChild(pParentNode, NULL, (xmlChar *)"misc", NULL);
  for (int i = 0 ; i < PREF_BOOL_MAX ; i++)
  {
#ifndef COMPLETE_PREFS
    // only save the things that are different from the defaults.
    if (m_BoolPrefs[i] == g_MainPrefs_Bools[i].m_Value)
      continue;
#endif
    pNewNode = xmlNewChild(pNode, NULL, (xmlChar *)"pref", NULL);
    if (!pNewNode)
      continue;

    xmlSetProp(pNewNode, (xmlChar *)"name", (xmlChar *)PREF_BOOL_NAME(i));
    xmlSetProp(pNewNode, (xmlChar *)"boolean", (xmlChar *)(m_BoolPrefs[i] ? str_True : str_False));
  }
  for (int i = 0 ; i < PREF_INT_MAX ; i++)
  {
#ifndef COMPLETE_PREFS
    // only save the things that are different from the defaults.
    if (m_IntPrefs[i] == g_MainPrefs_Ints[i].m_Value)
      continue;
#endif
    pNewNode = xmlNewChild(pNode, NULL, (xmlChar *)"pref", NULL);
    if (!pNewNode)
      continue;

    xmlSetProp(pNewNode, (xmlChar *)"name", (xmlChar *)PREF_INT_NAME(i));    
    _snprintf(numstr,9,"%d",m_IntPrefs[i]);
    xmlSetProp(pNewNode, (xmlChar *)"integer", (xmlChar *)numstr);
  }
  for (int i = 0 ; i < PREF_STRING_MAX ; i++)
  {
#ifndef COMPLETE_PREFS
    // only save the things that are different from the defaults.
    if (stricmp(m_StringPrefs[i],g_MainPrefs_Strings[i].m_Value) == 0)
      continue;
#endif
    pNewNode = xmlNewChild(pNode, NULL, (xmlChar *)"pref", NULL);
    if (!pNewNode)
      continue;

    xmlSetProp(pNewNode, (xmlChar *)"name", (xmlChar *)PREF_STRING_NAME(i));    
    xmlSetProp(pNewNode, (xmlChar *)"string", (xmlChar *)m_StringPrefs[i]);
  }

  for (int i = 0 ; i < PREF_PATH_MAX ; i++)
  {
#ifndef COMPLETE_PREFS
    // only save the things that are different from the defaults.
    if (stricmp(m_PathPrefs[i],g_MainPrefs_Paths[i].m_Value) == 0)
      continue;
#endif

    pNewNode = xmlNewChild(pNode, NULL, (xmlChar *)"pref", NULL);
    if (!pNewNode)
      continue;

    xmlSetProp(pNewNode, (xmlChar *)"name", (xmlChar *)PREF_PATH_NAME(i));    
    xmlSetProp(pNewNode, (xmlChar *)"path", (xmlChar *)m_PathPrefs[i]);
  }
}

void Prefs::XML_SaveColors(xmlNodePtr pParentNode, COLORREF *ColorPrefs)
{
  xmlNodePtr pNode,pNewNode;  
  char numstr[10];
  pNode = xmlNewChild(pParentNode, NULL, (xmlChar *)"colors", NULL);
  for (int i = 0 ; i < PREF_COLOR_MAX ; i++)
  {

    pNewNode = xmlNewChild(pNode, NULL, (xmlChar *)"item", NULL);
    if (!pNewNode)
      continue;

    xmlSetProp(pNewNode, (xmlChar *)"name", (xmlChar *)PREF_COLOR_NAME(i));
    if (ColorPrefs[i] == -1)
      strcpy(numstr,"-1");
    else
    {
      _snprintf(numstr,sizeof(numstr),"#%06x",RGBVALTOCOLORREF(ColorPrefs[i])); // convert from COLORVAL to RGB
      numstr[sizeof(numstr)-1] = 0;
    }
    xmlSetProp(pNewNode, (xmlChar *)"color", (xmlChar *)numstr);
  }
}

void Prefs::XML_SaveCommandProfiles(xmlNodePtr pParentNode)
{
  xmlNodePtr pNode,pNewNode;  

  pNode = xmlNewChild(pParentNode, NULL, (xmlChar *)"commandprofiles", NULL);
  for (int i = 0 ; i < m_CommandProfileList.GetSize() ; i++)
  {
    pNewNode = xmlNewChild(pNode, NULL, (xmlChar *)"commandprofile", NULL);
    if (!pNewNode)
      continue;

    CCommandProfile *pCommandProfile = m_CommandProfileList[i];

    xmlSetProp(pNewNode, (xmlChar *)"name", (xmlChar *)pCommandProfile->m_Name);

    char EscapedString[MAX_COMMAND_PROFILE_LEN];
    EscapeString(EscapedString,sizeof(EscapedString),pCommandProfile->m_Commands);
    xmlSetProp(pNewNode, (xmlChar *)"commands", (xmlChar *)EscapedString);
  }
}

void Prefs::XML_SaveIgnores(xmlNodePtr pParentNode)
{
  xmlNodePtr pNode,pNewNode,pNewChannelListNode;  
  char numstr[10];

  pNode = xmlNewChild(pParentNode, NULL, (xmlChar *)"ignores", NULL);

  IgnoreInfo *pIgnoreInfo;
  int i,j;
  for (i = 0 ; i < m_IgnoreList.GetSize() ; i++ )
  {
    pIgnoreInfo = m_IgnoreList[i];

    pNewNode = xmlNewChild(pNode, NULL, (xmlChar *)"ignore", NULL);
    if (!pNewNode)
      continue;

    xmlSetProp(pNewNode, (xmlChar *)"nick", (xmlChar *)pIgnoreInfo->m_Nick);

    if (pIgnoreInfo->m_HostMask)
      xmlSetProp(pNewNode, (xmlChar *)"hostmask", (xmlChar *)pIgnoreInfo->m_HostMask);

    if (pIgnoreInfo->m_Notes)
      xmlSetProp(pNewNode, (xmlChar *)"notes", (xmlChar *)pIgnoreInfo->m_Notes);

    _snprintf(numstr,9,"%d",pIgnoreInfo->m_NetworkID);
    xmlSetProp(pNewNode, (xmlChar *)"networkid", (xmlChar *)numstr);
    _snprintf(numstr,9,"%d",pIgnoreInfo->m_Flags);
    xmlSetProp(pNewNode, (xmlChar *)"flags", (xmlChar *)numstr);

    if (pIgnoreInfo->m_ChannelNameList.GetSize() > 0)
    {
      for (j = 0 ; j < pIgnoreInfo->m_ChannelNameList.GetSize() ; j++ )
      {
        pNewChannelListNode = xmlNewChild(pNewNode, NULL, (xmlChar *)"channel", NULL);
        if (!pNewChannelListNode)
          continue;

        xmlSetProp(pNewChannelListNode, (xmlChar *)"name", (xmlChar *)pIgnoreInfo->m_ChannelNameList[j]);
      }
    }

  }
}

void Prefs::XML_SaveFonts(xmlNodePtr pParentNode)
{
  xmlNodePtr pNode,pNewNode;  
  char numstr[10];

  pNode = xmlNewChild(pParentNode, NULL, (xmlChar *)"fonts", NULL);
  for (int i = 0 ; i < PREF_FONT_MAX ; i++)
  {
    pNewNode = xmlNewChild(pNode, NULL, (xmlChar *)"font", NULL);
    if (!pNewNode)
      continue;

    xmlSetProp(pNewNode, (xmlChar *)"description", (xmlChar *)PREF_FONT_NAME(i));
    xmlSetProp(pNewNode, (xmlChar *)"name", (xmlChar *)m_FontPrefs[i].m_Name);
    _snprintf(numstr,9,"%d",m_FontPrefs[i].m_Size);
    xmlSetProp(pNewNode, (xmlChar *)"size", (xmlChar *)numstr);
    if (m_FontPrefs[i].m_Flags & FIF_BOLD)
      xmlSetProp(pNewNode, (xmlChar *)"bold", (xmlChar *)str_True);
    if (m_FontPrefs[i].m_Flags & FIF_ITALIC)
      xmlSetProp(pNewNode, (xmlChar *)"italic", (xmlChar *)str_True);
    if (m_FontPrefs[i].m_Flags & FIF_UNDERLINE)
      xmlSetProp(pNewNode, (xmlChar *)"underline", (xmlChar *)str_True);
  }
}

// use flags to specify which list to save, see ISDF_*  flags of ISDF_NONE == save all!
void Prefs::XML_SaveServerList(xmlNodePtr pParentNode,int Flags)
{
  xmlNodePtr pNode,pNewNode;  
  char numstr[10];
  IRCServerDetails *pServerDetails;

  BOOL First = TRUE;

  for (int i = 0 ; i < g_ServerDetailsList.GetSize() ; i++ )
  {
    pServerDetails = g_ServerDetailsList[i];

    if (!(pServerDetails->m_Flags & Flags || Flags == ISDF_NONE))
      continue; // ignore this one!

    if (First)
      pNode = xmlNewChild(pParentNode, NULL, (xmlChar *)"serverlist", NULL);

    if (!pNode)
      continue;

    First = FALSE;

    pNewNode = xmlNewChild(pNode, NULL, (xmlChar *)"server", NULL);
    if (!pNewNode)
      continue;

    xmlSetProp(pNewNode, (xmlChar *)"name", (xmlChar *)pServerDetails->m_Name);
    xmlSetProp(pNewNode, (xmlChar *)"description", (xmlChar *)pServerDetails->m_Description);
    _snprintf(numstr,9,"%d",pServerDetails->m_Port);
    xmlSetProp(pNewNode, (xmlChar *)"port", (xmlChar *)numstr);

    // only save non-blank passwords
    if (pServerDetails->m_Password && *pServerDetails->m_Password)
      xmlSetProp(pNewNode, (xmlChar *)"password", (xmlChar *)pServerDetails->m_Password);

    // only save non-default priorities
    if (pServerDetails->m_Priority != PRI_NORMAL)
    {
      _snprintf(numstr,9,"%d",pServerDetails->m_Priority);
      xmlSetProp(pNewNode, (xmlChar *)"priority", (xmlChar *)numstr);
    }
    _snprintf(numstr,9,"%d",pServerDetails->m_NetworkID);
    xmlSetProp(pNewNode, (xmlChar *)"network", (xmlChar *)numstr);

    _snprintf(numstr,9,"%d",pServerDetails->m_Flags);
    xmlSetProp(pNewNode, (xmlChar *)"flags", (xmlChar *)numstr);
  }
}

// use flags to specify which list to save, see INDF_*  flags of INDF_NONE == save all!
void Prefs::XML_SaveNetworkList(xmlNodePtr pParentNode, int Flags)
{
  xmlNodePtr pNode,pNewNode;  
  char numstr[10];
  IRCNetworkDetails *pNetworkDetails;

  BOOL First = TRUE;

  for (int i = 0 ; i < g_NetworkList.GetSize() ; i++ )
  {
    pNetworkDetails = g_NetworkList[i];

    if (!(pNetworkDetails->m_Flags & Flags || Flags == INDF_NONE))
      continue; // ignore this one!

    if (First)
      pNode = xmlNewChild(pParentNode, NULL, (xmlChar *)"networklist", NULL);

    if (!pNode)
      continue;

    First = FALSE;

    pNewNode = xmlNewChild(pNode, NULL, (xmlChar *)"network", NULL);
    if (!pNewNode)
      continue;

    xmlSetProp(pNewNode, (xmlChar *)"name", (xmlChar *)pNetworkDetails->m_Name);
    _snprintf(numstr,9,"%d",pNetworkDetails->m_ID);
    xmlSetProp(pNewNode, (xmlChar *)"id", (xmlChar *)numstr);
    if (pNetworkDetails->m_IdentityName && *pNetworkDetails->m_IdentityName)
      xmlSetProp(pNewNode, (xmlChar *)"preferredidentityname", (xmlChar *)pNetworkDetails->m_IdentityName);
    _snprintf(numstr,9,"%d",pNetworkDetails->m_Flags);
    xmlSetProp(pNewNode, (xmlChar *)"flags", (xmlChar *)numstr);
  }
}

void Prefs::XML_SaveTheme(xmlNodePtr pParentNode, OutputThemeItem_t *pTheme)
{
  if (!pTheme)
    return;

  xmlNodePtr pNewNode;//,pCDataNode;  
  char numstr[10],tmpstr[5];

  while (pTheme->m_WindowType != -1)
  {
    pNewNode = xmlNewChild(pParentNode, NULL, (xmlChar *)"item", NULL);
    if (pNewNode)
    {
      _snprintf(tmpstr,sizeof(tmpstr),"%s%s%s%s",
        pTheme->m_WindowType & THEME_SERVER ? "S" : "",
        pTheme->m_WindowType & THEME_CHANNEL ? "C" : "",
        pTheme->m_WindowType & THEME_QUERY ? "Q" : "",
        pTheme->m_WindowType & THEME_DCCCHAT ? "D" : "");

      xmlSetProp(pNewNode, (xmlChar *)"appliesto", (xmlChar *)tmpstr);
      _snprintf(numstr,9,"%d",pTheme->m_IRCCommandID);
      xmlSetProp(pNewNode, (xmlChar *)"itemid", (xmlChar *)numstr);

      char EscapedString[512];
      EscapeString(EscapedString,sizeof(EscapedString),pTheme->m_FormatStr);
      xmlSetProp(pNewNode, (xmlChar *)"formatstring", (xmlChar *)EscapedString);

      /*
      pCDataNode = xmlNewCDataBlock(NULL,(xmlChar *)pTheme->m_FormatStr,strlen(pTheme->m_FormatStr));
      xmlAddChildList(pNewNode,pCDataNode);
      */
    }

    pTheme++;
  }
}

void Prefs::XML_SaveWindowPrefs(xmlNodePtr pParentNode)
{
  xmlNodePtr pNode,pNewNode;  
  char numstr[100];

  pNode = xmlNewChild(pParentNode, NULL, (xmlChar *)"windowprefs", NULL);
  for (int i = 0 ; i < m_WindowPrefsList.GetSize() ; i++)
  {
    WindowPrefs *pWindowPrefs = m_WindowPrefsList[i];

    pNewNode = xmlNewChild(pNode, NULL, (xmlChar *)"windowpref", NULL);
    if (!pNewNode)
      continue;

    _snprintf(numstr,9,"%d",pWindowPrefs->m_WindowType);
    xmlSetProp(pNewNode, (xmlChar *)"type", (xmlChar *)numstr);

    switch(pWindowPrefs->m_WindowType)
    {
      case CWTYPE_CHANNEL:
      case CWTYPE_QUERY:
        xmlSetProp(pNewNode, (xmlChar *)"name", (xmlChar *)pWindowPrefs->m_WindowName);
        break;
    }
    _snprintf(numstr,9,"%d",pWindowPrefs->m_NetworkID);
    xmlSetProp(pNewNode, (xmlChar *)"networkid", (xmlChar *)numstr);

    _snprintf(numstr,9,"%d",pWindowPrefs->m_Flags);
    xmlSetProp(pNewNode, (xmlChar *)"flags", (xmlChar *)numstr);

    if (pWindowPrefs->m_Flags & WPF_HAVEPOSITIONS)
    {
      _snprintf(numstr,99,"%d,%d,%d,%d",
        pWindowPrefs->m_WindowPositions.x,
        pWindowPrefs->m_WindowPositions.y,
        pWindowPrefs->m_WindowPositions.w,
        pWindowPrefs->m_WindowPositions.h);
      xmlSetProp(pNewNode, (xmlChar *)"positions", (xmlChar *)numstr);
    }

    if (pWindowPrefs->m_Flags & WPF_HAVECOLORS)
    {
      XML_SaveColors(pNewNode,pWindowPrefs->m_ColorPrefs);
    }
  }
}


void Prefs::ConvertColorSchemeToPrefs(ColorPrefItem_t *pScheme, COLORREF *pColorPrefs)
{
  if (!pColorPrefs)
    return;

  int i;

  while (pScheme && pScheme->m_PrefID != -1)
  {
    for (i = 0 ; i < PREF_COLOR_MAX; i ++)
    {
      if (pScheme->m_PrefID == PREF_COLOR_FIRST + i)
      {
        pColorPrefs[i] = pScheme->m_Value;
        break;
      }
    }
    pScheme++;
  }
}

void Prefs::XML_SaveBuddyGroups(xmlNodePtr pParentNode)
{
  xmlNodePtr pNode,pNewNode,pBuddyNode;  
  IRCUserGroup *pGroup;
  char numstr[10];

  pNode = xmlNewChild(pParentNode, NULL, (xmlChar *)"buddygroups", NULL);
  for (int i = 0 ; i < m_CustomUserGroups.m_UserGroupList.GetSize() ; i++)
  {
    pGroup = m_CustomUserGroups.m_UserGroupList[i];

    pNewNode = xmlNewChild(pNode, NULL, (xmlChar *)"group", NULL);

    xmlSetProp(pNewNode, (xmlChar *)"name", (xmlChar *)pGroup->m_GroupName);
    _snprintf(numstr,9,"%d",pGroup->m_GroupID);
    xmlSetProp(pNewNode, (xmlChar *)"id", (xmlChar *)numstr);
    _snprintf(numstr,9,"%d",pGroup->m_NetworkID);
    xmlSetProp(pNewNode, (xmlChar *)"network", (xmlChar *)numstr);

    for (int j = 0 ; j < pGroup->m_UserList.GetSize() ; j++)
    {
      pBuddyNode = xmlNewChild(pNewNode, NULL, (xmlChar *)"buddy", NULL);
      xmlSetProp(pBuddyNode, (xmlChar *)"name", (xmlChar *)pGroup->m_UserList[j]->m_Nick);
    }
  }
}

void Prefs::XML_SaveIdentities(xmlNodePtr pParentNode)
{
  xmlNodePtr pNode,pNewNode,pNickNode;  
  UserIdentity *pIdentity;
  char numstr[10];

  pNode = xmlNewChild(pParentNode, NULL, (xmlChar *)"identities", NULL);
  if (!pNode)
    return;

  for (int i = 0 ; i < m_UserIdentitiesList.GetSize() ; i++)
  {
    pIdentity = m_UserIdentitiesList[i];

    pNewNode = xmlNewChild(pNode, NULL, (xmlChar *)"identity", NULL);

    xmlSetProp(pNewNode, (xmlChar *)"name", (xmlChar *)pIdentity->m_Description);
    _snprintf(numstr,9,"%d",pIdentity->m_ID);
    xmlSetProp(pNewNode, (xmlChar *)"id", (xmlChar *)numstr);
    xmlSetProp(pNewNode, (xmlChar *)"realname", (xmlChar *)pIdentity->m_RealName);
    xmlSetProp(pNewNode, (xmlChar *)"username", (xmlChar *)pIdentity->m_UserName);
    if (pIdentity->m_IdentdOS)
      xmlSetProp(pNewNode, (xmlChar *)"identdos", (xmlChar *)pIdentity->m_IdentdOS);
    if (pIdentity->m_IdentdUserName)
    xmlSetProp(pNewNode, (xmlChar *)"identdusername", (xmlChar *)pIdentity->m_IdentdUserName);

    for (int j = 0 ; j < pIdentity->m_NickList.GetSize() ; j++)
    {
      pNickNode = xmlNewChild(pNewNode, NULL, (xmlChar *)"nick", NULL);
      xmlSetProp(pNickNode, (xmlChar *)"name", (xmlChar *)pIdentity->m_NickList[j]);
    }
  }
}

void Prefs::XML_SaveFavorites(xmlNodePtr pParentNode)
{
  xmlNodePtr pNode,pCopy;  
  pNode = xmlNewChild(pParentNode, NULL, (xmlChar *)"favorites", NULL);
  if (!pNode)
    return;

  pCopy = xmlCopyNodeList(m_Favorites);

  xmlStripTextNodes(&pCopy); 

  xmlAddChildList(pNode,pCopy);

}

BOOL Prefs::SaveTheme(char *ThemeName, char *FileName)
{
  if (!ThemeName || !*ThemeName || !FileName || !*FileName)
    return FALSE;

  BOOL Saved = FALSE;

  xmlDocPtr pDoc;
  xmlNodePtr pNewNode;

  sys_Printf(BIC_INFO,"Saving Theme - \"%s\" to \"%s\"\n", ThemeName, FileName);

  char *Author = NULL;
  if (m_UserIdentitiesList.GetSize() > 0)
    Author = m_UserIdentitiesList[0]->m_RealName;

  if (pDoc = xmlNewDoc((xmlChar *)"1.0"))
  {
    if (pDoc->children = xmlNewDocNode(pDoc, NULL, (xmlChar *)"HydraIRCThemeCollection", NULL))
    {
      xmlSetProp(pDoc->children, (xmlChar *)"name", (xmlChar *)ThemeName);
      if (Author)
        xmlSetProp(pDoc->children, (xmlChar *)"author", (xmlChar *)Author);
      xmlSetProp(pDoc->children, (xmlChar *)"version", (xmlChar *)"1.0");
      xmlSetProp(pDoc->children, (xmlChar *)"date", (xmlChar *)GetTimestamp(NULL,"%d/%m/%Y"));

      pNewNode = xmlNewChild(pDoc->children, NULL, (xmlChar *)"theme", NULL);
      if (pNewNode)
      {
        xmlSetProp(pNewNode, (xmlChar *)"name", (xmlChar *)ThemeName);
        if (Author)
          xmlSetProp(pNewNode, (xmlChar *)"author", (xmlChar *)Author);
        xmlSetProp(pNewNode, (xmlChar *)"version", (xmlChar *)"1.0");
        xmlSetProp(pNewNode, (xmlChar *)"date", (xmlChar *)GetTimestamp(NULL,"%d/%m/%Y"));


        XML_SaveTheme(pNewNode,g_DefaultOutputTheme);

        int result = xmlSaveFormatFile (FileName, pDoc, 1);
        if (result == -1)
          sys_Printf(BIC_ERROR,"Error Saving Theme to \"%s\"\n",FileName);
        else
        {
          sys_Printf(BIC_INFO,"Saved Theme - \"%s\" (%d bytes)\n",FileName,result);
          Saved = TRUE;
        }
      }

    }
    // TODO: else display error..
    xmlFreeDoc(pDoc);
  }
  // TODO: else display error..
  return Saved;
}

// FileName is optional, it saves to "Profile_<ProfileName>.xml" if omitted
void Prefs::SaveProfile(char *ProfileName, char *FileName)
{
  xmlDocPtr pDoc;
  char *filename;
  
  if (FileName)
    filename = strdup(FileName);
  else
  {
    filename = (char *)malloc(8 + strlen(ProfileName) + 5);
    sprintf(filename,"Profile_%s.xml",ProfileName);
  }

  sys_Printf(BIC_INFO,"Saving Profile - \"%s\" to \"%s\"\n",ProfileName, filename);

  if (pDoc = xmlNewDoc((xmlChar *)"1.0"))
  {
    if (pDoc->children = xmlNewDocNode(pDoc, NULL, (xmlChar *)"HydraIRCProfile", NULL))
    {
      xmlSetProp(pDoc->children, (xmlChar *)"name", (xmlChar *)ProfileName);
      xmlSetProp(pDoc->children, (xmlChar *)"hydraircversion", (xmlChar *)VERSIONNUM);

      XML_SaveIdentities(pDoc->children);
      XML_SaveBuddyGroups(pDoc->children);
      XML_SaveFonts(pDoc->children);
      XML_SaveColors(pDoc->children,m_ColorPrefs);
      XML_SaveMiscPrefs(pDoc->children);
      XML_SaveNotifications(pDoc->children);
      XML_SaveCommandProfiles(pDoc->children);
      XML_SaveFavorites(pDoc->children);
      XML_SaveNetworkList(pDoc->children,ISDF_CUSTOMLIST);
      XML_SaveServerList(pDoc->children,ISDF_CUSTOMLIST);
      XML_SaveIgnores(pDoc->children);
      XML_SaveWindowPrefs(pDoc->children);      

      int result = xmlSaveFormatFile (filename,pDoc,1);
      if (result == -1)
        sys_Printf(BIC_ERROR,"Error Saving Profile to \"%s\"\n",filename);
      else
        sys_Printf(BIC_INFO,"Saved Profile - \"%s\" (%d bytes)\n",filename,result);

    }
    // else display error.. // TODO:
    xmlFreeDoc(pDoc);
  }
  // else display error.. // TODO:
  free(filename);
}

void Prefs::SavePrefs( void )
{
  SetCurrentDirectory(g_HomeDir);
  // TODO: save default prefs, server lists, etc..

  // Save the profile
  SaveProfile("Default");
}

CCommandProfile *Prefs::FindCommandProfile(char *Name)
{
  if (!Name || !*Name)
    return NULL;
  CCommandProfile *pCommandProfile;
  int i;
  for (i = 0; i < m_CommandProfileList.GetSize(); i++ )
  {
    pCommandProfile = m_CommandProfileList[i];
    if (pCommandProfile->m_Name && stricmp(pCommandProfile->m_Name,Name) == 0)
      return pCommandProfile;
  }
  return NULL;
}

// Initialising
//
/////////////////////////////////////////////////////////////////////////////


/*
void Prefs::InitPrefsCollection(PrefsCollection_t *pPC, PrefsCollection_t *pParent)
{
  // A NULL pParent is valid.
  BOOL *pBool = pPC->m_pBools;
  BoolPrefItem_t *pBPI = pPC->m_pBoolsTable;
  if (pBPI)
  {
    while (pBPI->m_PrefID != PREF_END_OF_LIST)
    {
      *pBool++ = GetBooleanPref(pParent, pBPI++->m_PrefID);
    }
  }

  int *pInt = pPC->m_pInts;
  IntPrefItem_t *pIPI = pPC->m_pIntsTable;
  if (pIPI)
  {
    while (pIPI->m_PrefID != PREF_END_OF_LIST)
    {
      *pInt++ = pIPI++->m_Int; // TODO use *pInt++ = GetDefaultInt(pIPI++,PT_INT,pParent);
    }
  }

  char **pString = pPC->m_pStrings;
  StringPrefItem_t *pSPI = pPC->m_pStringsTable;
  if (pSPI)
  {
    while (pSPI->m_PrefID != PREF_END_OF_LIST)
    {
      *pString++ = strdup(pSPI++->m_String); // TODO use *pString++ = GetDefaultString(pSPI++,PT_STRING,pParent);
    }
  }

  char **pPath = pPC->m_pPaths;
  PathPrefItem_t *pPPI = pPC->m_pPathsTable;
  if (pPPI)
  {
    while (pPPI->m_PrefID != PREF_END_OF_LIST)
    {
      *pPath++ = strdup(pPPI++->m_Path); // TODO use *pPath++ = GetDefaultPath(pPPI++,PT_PATH,pParent);
    }
  }

  COLORREF *pColor = pPC->m_pColors;
  ColorPrefItem_t *pCPI = pPC->m_pColorsTable;
  if (pCPI)
  {
    while (pCPI->m_PrefID != PREF_END_OF_LIST)
    {
      *pColor++ = GetColorPref(pParent, pCPI++->m_PrefID); // if parent, init this collection to the parent's value.
    }
  }

  FontPrefInfo_t *pFont = pPC->m_pFonts;
  FontPrefItem_t *pFPI = pPC->m_pFontsTable;
  if (pFPI)
  {
    while (pFPI->m_PrefID != PREF_END_OF_LIST)
    {
      (*pFont).FontName = strdup(pFPI->m_Name); // TODO use something to get the defaults..
      (*pFont).Flags = pFPI->m_Flags;
      (*pFont).FontSize = pFPI->m_Size;
      //(*pFont).FontScript = pFPI->m_Script; // ZANN_FONTS

      pFont++;
      pFPI++;
    }
  }

}
*/

void Prefs::Initialise( void )
{
  /* // prefscollections
  m_PC.m_pBoolsTable = g_MainPrefs_Bools;
  m_PC.m_pIntsTable = g_MainPrefs_Ints;
  m_PC.m_pStringsTable = g_MainPrefs_Strings;
  m_PC.m_pPathsTable = g_MainPrefs_Paths;
  m_PC.m_pColorsTable = g_MainPrefs_Colors;
  m_PC.m_pFontsTable = g_MainPrefs_Fonts;
  m_PC.m_pBools = this->m_BoolPrefs;
  m_PC.m_pInts = this->m_IntPrefs;
  m_PC.m_pStrings = this->m_StringPrefs;
  m_PC.m_pPaths = this->m_PathPrefs;
  m_PC.m_pColors = this->m_Colors;
  m_PC.m_pFonts = this->m_FontPrefInfo;
  */

  m_Favorites = NULL;
  m_Favorites = xmlNewText(NULL);

  BoolPrefItem_t *pBPI = g_MainPrefs_Bools;
  while (pBPI->m_PrefID != PREF_END_OF_LIST)
  {
    m_BoolPrefs[pBPI->m_PrefID - PREF_BOOL_FIRST] = pBPI->m_Value;
    pBPI++;
  }

  IntPrefItem_t *pIPI = g_MainPrefs_Ints;
  while (pIPI->m_PrefID != PREF_END_OF_LIST)
  {
    m_IntPrefs[pIPI->m_PrefID - PREF_INT_FIRST] = pIPI->m_Value;
    pIPI++;
  }

  StringPrefItem_t *pSPI = g_MainPrefs_Strings;
  while (pSPI->m_PrefID != PREF_END_OF_LIST)
  {
    m_StringPrefs[pSPI->m_PrefID - PREF_STRING_FIRST] = strdup(pSPI->m_Value);
    pSPI++;
  }

  pSPI = g_MainPrefs_Paths;
  while (pSPI->m_PrefID != PREF_END_OF_LIST)
  {
    m_PathPrefs[pSPI->m_PrefID - PREF_PATH_FIRST] = strdup(pSPI->m_Value);
    pSPI++;
  }

  ColorPrefItem_t *pCPI = g_MainPrefs_Colors;
  while (pCPI->m_PrefID != PREF_END_OF_LIST)
  {
    m_ColorPrefs[pCPI->m_PrefID - PREF_COLOR_FIRST] = pCPI->m_Value;
    pCPI++;
  }

  FontPrefItem_t *pFPI = g_MainPrefs_Fonts;
  while (pFPI->m_PrefID != PREF_END_OF_LIST)
  {
    m_FontPrefs[pFPI->m_PrefID - PREF_FONT_FIRST].m_Name = strdup(pFPI->m_Name);
    m_FontPrefs[pFPI->m_PrefID - PREF_FONT_FIRST].m_Flags = pFPI->m_Flags;
    m_FontPrefs[pFPI->m_PrefID - PREF_FONT_FIRST].m_Size = pFPI->m_Size;
    // m_FontPrefs[PREF_FONT_FIRST - pFPI->m_PrefID].m_Script = pFPI->m_Script; // ZANN_FONTS
    pFPI++;
  }
}  

  /*
  CopyMemory(m_BoolPrefs,g_DefaultBoolPrefs,sizeof(g_DefaultBoolPrefs));
  CopyMemory(m_IntPrefs,g_DefaultIntPrefs,sizeof(g_DefaultIntPrefs));
  CopyMemory(m_Colors,g_DefaultColors,sizeof(m_Colors));

  for (int i = 0 ; i < STRINGPREF_LAST ; i ++)
  {
    m_StringPrefs[i]=strdup(g_DefaultStringPrefs[i]);
  }

  for (int i = 0 ; i < PATHPREF_LAST ; i ++)
  {
    m_PathPrefs[i]=strdup(g_DefaultPathPrefs[i]);
  }

  // Default fonts.
  for (int i = 0 ; i < FONT_MAX_FONTS ; i ++)
  {
    m_FontPrefInfo[i].FontName = strdup("Tahoma");
    m_FontPrefInfo[i].FontSize = 8;
    m_FontPrefInfo[i].Flags    = FIF_NONE;

    switch(i)
    {
      case FONT_SERVERLISTDIRTYFONT:
        m_FontPrefInfo[i].Flags    = FIF_UNDERLINE;
        break;
    }
  }
  */

void Prefs::SetupPrefs( void )
{
  // Special Cases

  // set audio mute back to the default if we're not supposed to remember the setting
  if (!m_BoolPrefs[PREF_bRememberAudioMute - PREF_BOOL_FIRST])
    m_BoolPrefs[PREF_bAudioMute - PREF_BOOL_FIRST] = g_MainPrefs_Bools[PREF_bAudioMute].m_Value;

// example test data
#if 0

  WindowPrefs *pWindowPrefs = new WindowPrefs(this);

  if (pWindowPrefs)
  {
    pWindowPrefs->m_NetworkID = 1; // user's first own network
    pWindowPrefs->m_WindowType = CWTYPE_CHANNEL;
    pWindowPrefs->m_WindowName = strdup("#test");
    pWindowPrefs->m_WindowPositions.Set(10,10,300,200);
    pWindowPrefs->m_Flags |= WPF_HAVEPOSITIONS;

    // load in a colorscheme for this WindowPrefs instance.
    ColorPrefItem_t *pColorScheme = LoadColorSchemeByName("Light On Dark");
    if (pColorScheme)
    {
      ConvertColorSchemeToPrefs(pColorScheme,pWindowPrefs->m_ColorPrefs);
      pWindowPrefs->m_Flags |= WPF_HAVECOLORS;
      free(pColorScheme);
    }

    // add this example to the list.
    m_WindowPrefsList.Add(pWindowPrefs);
  }
#endif
}

Prefs::Prefs(void)
{
  Initialise();
  //InitPrefsCollection(&m_PC,NULL); // no parent. // prefscollections
  LoadPrefs();
  LoadProfile("Default");
  SetupPrefs();
}

void Prefs::Copy(Prefs *pOther)
{
  // this function does not free any memory, so don't copy prefs from one
  // instance to another already initialised instance...

  int i,j;

  // ChannelPrefs *pChannelPrefs; // prefscollections

  m_Favorites = xmlCopyNodeList(pOther->m_Favorites);

  // Misc prefs
  CopyMemory(m_BoolPrefs, pOther->m_BoolPrefs,sizeof (m_BoolPrefs));
  CopyMemory(m_IntPrefs, pOther->m_IntPrefs,sizeof (m_IntPrefs));
  CopyMemory(m_ColorPrefs,pOther->m_ColorPrefs,sizeof(m_ColorPrefs));

  for (i = 0 ; i < PREF_STRING_MAX ; i ++)
  {
    m_StringPrefs[i] = strdup(pOther->m_StringPrefs[i]);
  }

  for (i = 0 ; i < PREF_PATH_MAX ; i ++)
  {
    m_PathPrefs[i] = strdup(pOther->m_PathPrefs[i]);
  }

  for (i = 0 ; i < PREF_FONT_MAX ; i ++)
  {
    // copy the numbers, then copy the strings..
    CopyMemory(&m_FontPrefs[i],&pOther->m_FontPrefs[i],sizeof (FontPrefItem_t));
    m_FontPrefs[i].m_Name = strdup(pOther->m_FontPrefs[i].m_Name);
  }

  // prefscollections
  /*
  // channel prefs
  for (i = 0; i < pOther->m_ChannelPrefList.GetSize() ; i++ )
  {
    pChannelPrefs = new ChannelPrefs(this,pOther->m_ChannelPrefList[i]);
    if (!pChannelPrefs)
      continue;

      m_ChannelPrefList.Add(pChannelPrefs);
  }
  */

  // Command Profiles
  CCommandProfile *pCommandProfile;
  for (i = 0 ; i < pOther->m_CommandProfileList.GetSize() ; i++ )
  {
    pCommandProfile = new CCommandProfile(pOther->m_CommandProfileList[i]);
    if (!pCommandProfile)
      continue;

    m_CommandProfileList.Add(pCommandProfile);
    // TODO: else break and throw exception..
  }

  // Ignore List
  IgnoreInfo *pIgnoreInfo;
  for (i = 0 ; i < pOther->m_IgnoreList.GetSize() ; i++ )
  {
    pIgnoreInfo = new IgnoreInfo(pOther->m_IgnoreList[i]);
    if (!pIgnoreInfo)
      continue;

    m_IgnoreList.Add(pIgnoreInfo);
    // TODO: else break and throw exception..
  }

  // User identities
  UserIdentity *pIdentity;

  for (i = 0 ; i < pOther->m_UserIdentitiesList.GetSize() ; i++ )
  {
    pIdentity = new UserIdentity(pOther->m_UserIdentitiesList[i]);
    if (!pIdentity)
      continue;

    m_UserIdentitiesList.Add(pIdentity);
    // TODO: else break and throw exception..
  }

  // User groups
  IRCUserGroup *pNewGroup,*pGroup;
  IRCUser *pNewUser, *pUser;
  for (i = 0 ; i < pOther->m_CustomUserGroups.m_UserGroupList.GetSize(); i ++ )
  {
    pGroup = pOther->m_CustomUserGroups.m_UserGroupList[i];

    pNewGroup = new IRCUserGroup(pGroup->m_GroupName,pGroup->m_GroupID,pGroup->m_NetworkID);
    if (!pNewGroup)
      continue;

    for (j = 0 ; j < pGroup->m_UserList.GetSize() ; j ++)
    {
      pUser = pGroup->m_UserList[j];
      pNewUser = new IRCUser(pUser);
      pNewGroup->AddUser(pNewUser);
    }
    m_CustomUserGroups.m_UserGroupList.Add(pNewGroup);
  }

  // Notifications
  NotificationPrefInfo_t *pNewNPI;

  for (i = 0 ; i < pOther->m_NotificationPrefList.GetSize(); i ++)
  {
    pNewNPI = (NotificationPrefInfo_t *)malloc(sizeof(NotificationPrefInfo_t));
    if (!pNewNPI)
      continue;

    CopyMemory(pNewNPI,pOther->m_NotificationPrefList[i],sizeof(NotificationPrefInfo_t));
    //we could do this, but it's slower, m_FileName's alread pointing a the right string to be copied..
    //pNewNPI->m_FileName = strdup(pOther->m_NotificationPrefList[i]->m_FileName);
    pNewNPI->m_FileName = strdup(pNewNPI->m_FileName);
    pNewNPI->m_MatchString = strdup(pNewNPI->m_MatchString);
    m_NotificationPrefList.Add(pNewNPI);
  }

  // Window Prefs
  WindowPrefs *pNewWindowPrefs;

  for (i = 0 ; i < pOther->m_WindowPrefsList.GetSize(); i ++)
  {
    pNewWindowPrefs = new WindowPrefs(pOther->m_WindowPrefsList[i]);
    if (!pNewWindowPrefs)
      continue;

    m_WindowPrefsList.Add(pNewWindowPrefs);
  }
}

Prefs::Prefs(Prefs *pOther)
{
  Copy(pOther);
}

Prefs::~Prefs(void) 
{
  int i;
  NotificationPrefInfo_t *pNPI;

  if (m_Favorites)
  {
    xmlFreeNodeList(m_Favorites);
    m_Favorites = NULL;
  }

  for (i = 0 ; i < PREF_STRING_MAX ; i ++)
  {
    free(m_StringPrefs[i]);
  }

  for (i = 0 ; i < PREF_PATH_MAX ; i ++)
  {
    free(m_PathPrefs[i]);
  }

  for (i = 0 ; i < PREF_FONT_MAX ; i ++)
  {
    free(m_FontPrefs[i].m_Name);
  }

  // prefscollections
  /*
  while (m_ChannelPrefList.GetSize() > 0)
  {
    delete m_ChannelPrefList[0];
    m_ChannelPrefList.RemoveAt(0);
  }
  */

  while (m_CommandProfileList.GetSize() > 0)
  {
    delete m_CommandProfileList[0];
    m_CommandProfileList.RemoveAt(0);
  }

  while (m_IgnoreList.GetSize() > 0)
  {
    delete m_IgnoreList[0];
    m_IgnoreList.RemoveAt(0);
  }

  while (m_UserIdentitiesList.GetSize() > 0)
  {
    delete m_UserIdentitiesList[0];
    m_UserIdentitiesList.RemoveAt(0);
  }

  while (m_NotificationPrefList.GetSize() > 0)
  {
    pNPI = m_NotificationPrefList[0];
    if (pNPI->m_FileName) free(pNPI->m_FileName);
    if (pNPI->m_MatchString) free(pNPI->m_MatchString);
    free(pNPI);
    m_NotificationPrefList.RemoveAt(0);
  }

  while (m_WindowPrefsList.GetSize() > 0)
  {
    delete m_WindowPrefsList[0];
    m_WindowPrefsList.RemoveAt(0);
  }

  m_CustomUserGroups.DeleteAll();

  xmlCleanupParser(); // Free any memory allocated by the XML parser
};

//////////////////////

/*
ChannelPrefs::ChannelPrefs( Prefs *pPrefs, char *ChannelName, int NetworkID )
{
  m_pPrefs = pPrefs;
  m_ChannelName = strdup(ChannelName);
  m_NetworkID = NetworkID;
  Initialise();
  pPrefs->InitPrefsCollection(&m_PC,&pPrefs->m_PC);
}

void ChannelPrefs::Initialise( void )
{
  ZeroMemory(&m_PC,sizeof(m_PC));

  m_PC.m_pBools = (BOOL *)malloc(sizeof (BOOL) * (sizeof(g_ChannelPrefs_Bools) / sizeof(BoolPrefItem_t)));
  m_PC.m_pBoolsTable = g_ChannelPrefs_Bools;

  m_PC.m_pInts = (int *)malloc(sizeof (BOOL) * (sizeof(g_ChannelPrefs_Ints) / sizeof(IntPrefItem_t)));
  m_PC.m_pIntsTable = g_ChannelPrefs_Ints;

  m_PC.m_pColors =  (COLORREF *)malloc(sizeof (COLORREF) * (sizeof(g_ChannelPrefs_Colors) / sizeof(ColorPrefItem_t)));
  m_PC.m_pColorsTable = g_ChannelPrefs_Colors;

}

ChannelPrefs::~ChannelPrefs( void )
{
  if (m_ChannelName)
    free(m_ChannelName);

  if (m_PC.m_pBools)
    free(m_PC.m_pBools);

  if (m_PC.m_pInts)
    free(m_PC.m_pInts);

  if (m_PC.m_pColors)
    free(m_PC.m_pColors);
}

ChannelPrefs::ChannelPrefs( Prefs *pPrefs, ChannelPrefs *pOther )
{
  m_pPrefs = pPrefs;
  m_ChannelName = strdup(pOther->m_ChannelName);
  m_NetworkID = pOther->m_NetworkID;
  Initialise();
  pPrefs->InitPrefsCollection(&m_PC,&pOther->m_PC);
}

*/


ThemeHandle *ThemeManager::LoadTheme(char *ThemeName)
{    
  ThemeHandle *pTheme = new ThemeHandle;
  if (!pTheme)
    return NULL;

  if (!ThemeName || stricmp(ThemeName,g_MainPrefs_Strings[PREF_sDefaultTheme - PREF_STRING_FIRST].m_Value) == 0)
  {
    // if ThemeName == NULL, use the built-in theme
    pTheme->m_ThemeItems = g_DefaultOutputTheme;
    pTheme->m_Name = strdup(g_MainPrefs_Strings[PREF_sDefaultTheme - PREF_STRING_FIRST].m_Value);
  }
  else
  {
    // Load themes from XML files via prefs...
    pTheme->m_ThemeItems = g_pPrefs->LoadThemeByName(ThemeName);
    pTheme->m_Name = strdup(ThemeName);
  }

  if (pTheme->m_Name && pTheme->m_ThemeItems)
  {
    m_ThemeList.Add(pTheme); // make a note of it..
  }
  else
  {
    delete pTheme;
    pTheme = FindTheme(NULL); // use the built-in theme!
  }

  // NOTE: we don't increase the refcount, nothing's using it yet...
  return pTheme;
}

ThemeHandle *ThemeManager::FindTheme(char *ThemeName)
{
  if (!ThemeName)
    ThemeName = g_MainPrefs_Strings[PREF_sDefaultTheme - PREF_STRING_FIRST].m_Value;

  ThemeHandle *pTheme;
  for (int i=0 ; i < m_ThemeList.GetSize(); i++)
  {
    pTheme = m_ThemeList[i];
    if (pTheme->m_Name && stricmp(pTheme->m_Name,ThemeName) == 0)
    {
      return pTheme; // found
    }
  }
  return NULL; // not found
}

BOOL Prefs::DeleteWindowPrefs(int WindowType, int NetworkID, char *WindowName)
{
  WindowPrefs *pWindowPrefs = GetWindowPrefs(WindowType, NetworkID, WindowName);
  if (pWindowPrefs)
  {
    m_WindowPrefsList.Remove(pWindowPrefs);
    delete pWindowPrefs;
    return TRUE;
  }
  return FALSE;
}

WindowPrefs *Prefs::GetWindowPrefs(int WindowType, int NetworkID, char *WindowName)
{

  WindowPrefs *pWindowPrefs;
  for (int i=0 ; i < m_WindowPrefsList.GetSize(); i++)
  {
    pWindowPrefs = m_WindowPrefsList[i];
    // find one..
    // if server, match window type, networkid (and ignore WindowName parameter)
    // if channel or query, match window type, network id, and windowname
    if (WindowType == pWindowPrefs->m_WindowType &&
        NetworkID == pWindowPrefs->m_NetworkID &&
        ((WindowType == CWTYPE_SERVER && pWindowPrefs->m_WindowName == NULL) ||
         ((WindowType == CWTYPE_CHANNEL || WindowType == CWTYPE_QUERY) &&
          pWindowPrefs->m_WindowName && WindowName && stricmp(pWindowPrefs->m_WindowName,WindowName) == 0)))
        
    {
      return pWindowPrefs; // found
    }
  }

  return NULL;
}
/*****************************************************************************/

WindowPrefs::WindowPrefs(Prefs *pPrefs)
{
  m_NetworkID = NETWORK_UNKNOWN;
  m_WindowType = CWTYPE_UNKNOWN;
  m_WindowName = NULL;
  m_Flags = WPF_NONE;

  // copy the colors from another prefs instance.
  CopyMemory(m_ColorPrefs,pPrefs->m_ColorPrefs,sizeof(m_ColorPrefs));
}

WindowPrefs::~WindowPrefs( void )
{
  SetString(m_WindowName,NULL);
}

WindowPrefs::WindowPrefs(WindowPrefs *pOther)
{
  m_NetworkID = pOther->m_NetworkID;
  m_WindowType = pOther->m_WindowType;
  m_WindowName = strdup(pOther->m_WindowName);
  m_Flags = pOther->m_Flags;
  m_WindowPositions.Copy(&pOther->m_WindowPositions);
  CopyMemory(m_ColorPrefs,pOther->m_ColorPrefs,sizeof(m_ColorPrefs));
}
