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

#define PLUGIN_API_BASIC 0

typedef void (* PFN_SYSPRINTF) (const int Contents, const char *format, ...);
typedef void *(* PFN_SYSMALLOC) (size_t);
typedef void (* PFN_SYSFREE) (void *);
typedef char *(* PFN_HYDRAIRCBUILDSTRING) (const int MaxSize, const char *format, ...);

typedef void (* PFN_HYDRAIRCSENDRAW) (const int ServerID, const char *Buffer);

typedef void (* PFN_HYDRAIRCSERVERPUT) (const int ServerID, const int Contents, const char *Buffer);
typedef void (* PFN_HYDRAIRCCHANNELPUT) (const int ChannelID, const int Contents, const char *Buffer);
typedef void (* PFN_HYDRAIRCSERVERPRINTF) (const int ServerID, const int Contents, const char *format, ...);
typedef void (* PFN_HYDRAIRCCHANNELPRINTF) (const int ChannelID, const int Contents, const char *format, ...);

typedef void (* PFN_HYDRAIRCCMDPRIVMSG) (const int WindowID, const char *To, const char *Message); // to can be NULL
typedef void (* PFN_HYDRAIRCCMDACTION) (const int WindowID, const char *Message);

typedef BOOL (* PFN_HYDRAIRCGETCHANNELINFO) (const int ChannelID, HIRC_ChannelInfo_t *pCI);
typedef BOOL (* PFN_HYDRAIRCGETSERVERINFO) (const int ServerID, HIRC_ServerInfo_t *pSI);
typedef BOOL (* PFN_HYDRAIRCGETWINDOWINFO) (const int WindowID, HIRC_WindowInfo_t *pWI);
typedef BOOL (* PFN_HYDRAIRCGETACTIVEWINDOWINFO) (HIRC_WindowInfo_t *pWI);


typedef HWND (* PFN_HYDRAIRCGETTOOLBAR) (void);
typedef HIMAGELIST (* PFN_HYDRAIRCGETTOOLBARIMAGELIST) (void);
typedef int  (* PFN_HYDRAIRCGETNEWTOOLBARID) (void);
typedef void (* PFN_HYDRAIRCRELEASETOOLBARID) (const int ID);
typedef int  (* PFN_HYDRAIRCGETNEWIMAGELISTID) (void);
typedef void (* PFN_HYDRAIRCRELEASEIMAGELISTID) (const int ID);

typedef char *(* PFN_HYDRAIRCALIASPROCESSOR) (char *Input, char *Alias, int InputTokenBuffers, char EscapeCharacter ,char **SubsitutionTable);

struct PluginBasicTable
{
  PFN_SYSPRINTF m_pfnsys_Printf;
  PFN_SYSMALLOC m_pfnsys_MAlloc;
  PFN_SYSFREE m_pfnsys_Free;
  PFN_HYDRAIRCBUILDSTRING m_pfnHydraIRC_BuildString;

  PFN_HYDRAIRCSENDRAW m_pfnHydraIRC_SendRaw;

  PFN_HYDRAIRCSERVERPUT m_pfnHydraIRC_ServerPut;
  PFN_HYDRAIRCCHANNELPUT m_pfnHydraIRC_ChannelPut;
  PFN_HYDRAIRCSERVERPRINTF m_pfnHydraIRC_ServerPrintf;
  PFN_HYDRAIRCCHANNELPRINTF m_pfnHydraIRC_ChannelPrintf;

  PFN_HYDRAIRCCMDPRIVMSG m_pfnHydraIRC_CMD_PrivMsg;
  PFN_HYDRAIRCCMDACTION m_pfnHydraIRC_CMD_Action;

  PFN_HYDRAIRCGETCHANNELINFO m_pfnHydraIRC_GetChannelInfo;
  PFN_HYDRAIRCGETSERVERINFO m_pfnHydraIRC_GetServerInfo;
  PFN_HYDRAIRCGETWINDOWINFO m_pfnHydraIRC_GetWindowInfo;
  PFN_HYDRAIRCGETACTIVEWINDOWINFO m_pfnHydraIRC_GetActiveWindowInfo;

  PFN_HYDRAIRCGETTOOLBAR m_pfnHydraIRC_GetToolbar;
  PFN_HYDRAIRCGETTOOLBARIMAGELIST m_pfnHydraIRC_GetToolbarImageList;
  PFN_HYDRAIRCGETNEWTOOLBARID m_pfnHydraIRC_GetNewToolbarID;
  PFN_HYDRAIRCRELEASETOOLBARID m_pfnHydraIRC_ReleaseToolbarID;
  PFN_HYDRAIRCGETNEWIMAGELISTID m_pfnHydraIRC_GetNewImageListID;
  PFN_HYDRAIRCRELEASEIMAGELISTID m_pfnHydraIRC_ReleaseImageListID;
  PFN_HYDRAIRCALIASPROCESSOR m_pfnHydraIRC_AliasProcessor;
};

// define this for shorthand use
#ifdef USE_BASICTABLE_DEFINE
#ifndef __BASICTABLENAME
#define __BASICTABLENAME g_PluginBasicTable
#endif
#define sys_Printf                   __BASICTABLENAME.m_pfnsys_Printf
#define sys_MAlloc                   __BASICTABLENAME.m_pfnsys_MAlloc
#define sys_Free                     __BASICTABLENAME.m_pfnsys_Free
#define HydraIRC_BuildString         __BASICTABLENAME.m_pfnHydraIRC_BuildString

#define HydraIRC_SendRaw             __BASICTABLENAME.m_pfnHydraIRC_SendRaw

#define HydraIRC_ServerPut           __BASICTABLENAME.m_pfnHydraIRC_ServerPut
#define HydraIRC_ChannelPut          __BASICTABLENAME.m_pfnHydraIRC_ChannelPut
#define HydraIRC_ServerPrintf        __BASICTABLENAME.m_pfnHydraIRC_ServerPrintf
#define HydraIRC_ChannelPrintf       __BASICTABLENAME.m_pfnHydraIRC_ChannelPrintf

#define HydraIRC_CMD_PrivMsg         __BASICTABLENAME.m_pfnHydraIRC_CMD_PrivMsg
#define HydraIRC_CMD_Action          __BASICTABLENAME.m_pfnHydraIRC_CMD_Action

#define HydraIRC_GetChannelInfo      __BASICTABLENAME.m_pfnHydraIRC_GetChannelInfo
#define HydraIRC_GetServerInfo       __BASICTABLENAME.m_pfnHydraIRC_GetServerInfo
#define HydraIRC_GetWindowInfo       __BASICTABLENAME.m_pfnHydraIRC_GetWindowInfo
#define HydraIRC_GetActiveWindowInfo __BASICTABLENAME.m_pfnHydraIRC_GetActiveWindowInfo

#define HydraIRC_GetToolbar          __BASICTABLENAME.m_pfnHydraIRC_GetToolbar
#define HydraIRC_GetToolbarImageList __BASICTABLENAME.m_pfnHydraIRC_GetToolbarImageList
#define HydraIRC_GetNewToolbarID     __BASICTABLENAME.m_pfnHydraIRC_GetNewToolbarID
#define HydraIRC_ReleaseToolbarID    __BASICTABLENAME.m_pfnHydraIRC_ReleaseToolbarID
#define HydraIRC_GetNewImageListID   __BASICTABLENAME.m_pfnHydraIRC_GetNewImageListID
#define HydraIRC_ReleaseImageListID  __BASICTABLENAME.m_pfnHydraIRC_ReleaseImageListID

#define HydraIRC_AliasProcessor      __BASICTABLENAME.m_pfnHydraIRC_AliasProcessor

#endif
