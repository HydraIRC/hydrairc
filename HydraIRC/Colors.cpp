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

// colors.cpp : colors'n'stuff
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

/*
//see enum color_id in HydraIRCTypes.h
COLORREF g_DefaultColors[GUI_COLOR_ID_MAX] = {
  // actual colors..

#if 1 // change to 0 to change the defaults!

  // dark colors, light background (black on white)
  RGB(255,255,255),         // white
  RGB(  0,  0,  0),         // black
  RGB(  0,  0,127),         // dark blue
  RGB(  0,147,  0),         // dark green
  RGB(255,  0,  0),         // red
  RGB(127,  0,  0),         // brown
  RGB(156,  0,156),         // purple
  RGB(252,127,  0),         // orange
  RGB(255,255,  0),         // yellow
  RGB(  0,252,  0),         // green
  RGB(  0,147,147),         // dark cyan
  RGB(  0,255,255),         // cyan
  RGB(  0,  0,252),         // blue
  RGB(255,  0,255),         // pink
  RGB(127,127,127),         // gray
  RGB(170,170,170),         // lightgray

  // items..

  RGB(255,255,255),         // background  
  RGB( 64, 64, 64),         // normal
  RGB(127,  0,  0),         // notice
  RGB(  0,147,  0),         // topic
  RGB(  0,147,  0),         // join
  RGB(  0,  0,127),         // kick
  RGB(  0,147,  0),         // mode
  RGB(  0,147,  0),         // nick
  RGB(  0,  0,127),         // part
  RGB(156,  0,156),         // me
  RGB(192,  0,192),         // ctcp
  RGB(  0,  0,127),         // quit
  RGB(  0,  0,127),         // connecting
  RGB(  0,192,192),         // querytext

  RGB(  0,128,  0),         // info
  RGB(128,128,  0),         // warning
  RGB(128,  0,  0),         // error

  RGB(128,128,128),         // timestamp
  RGB(255,255,255),         // userlist background
  RGB(  0,  0,  0),         // userlist normal text
  RGB(255,255,255),         // textinput background
  RGB(  0,  0,  0),         // textinput normal text
  RGB(255,255,255),         // global userlist background
  RGB(  0,  0,  0),         // global userlist normal text

  RGB(255,255,255),         // serverlist background
  RGB(  0,  0,  0),         // serverlist normal text
  RGB(128,  0,  0),         // serverlist disconnected
  RGB(128,128,  0),         // serverlist connecting
  RGB(  0,128,  0),         // serverlist connected
  RGB(255,  0,  0),         // serverlist serverdirtytext 
  RGB(255,255,255),         // serverlist serverdirtybackground 
  RGB(255,  0,  0),         // serverlist dccchatdirtytext 
  RGB(255,255,255),         // serverlist dccchatdirtybackground 
  RGB(255,  0,  0),         // serverlist channeldirtytext 
  RGB(255,255,255),         // serverlist channeldirtybackground 
  RGB(255,  0,  0),         // serverlist querydirtytext 
  RGB(255,255,255),         // serverlist querydirtybackground 

  // background is not reversed for these items:
  RGB(147,  0,  0),         // transferlist errortext,
  RGB(  0,147,  0),         // transferlist completetext,
  RGB(  0,  0,  0)          // transferlist normaltext,

#else
  // light colors, dark background (white on black)
  RGB(255,255,255),         // white
  RGB(  0,  0,  0),         // black
  RGB( 64, 64,192),         // dark blue
  RGB( 64,192, 64),         // dark green
  RGB(255, 64, 64),         // red
  RGB(192,128, 64),         // brown
  RGB(192, 64,192),         // purple
  RGB(255,128, 64),         // orange
  RGB(255,255, 64),         // yellow
  RGB( 64,255, 64),         // green
  RGB( 64,192,192),         // dark cyan
  RGB( 64,255,255),         // cyan
  RGB( 64, 64,255),         // blue
  RGB(255,192,255),         // pink
  RGB(128,128,128),         // gray
  RGB(192,192,192),         // lightgray

  // items..

  RGB(  0,  0,  0),         // background  
  RGB(224,224,224),         // normal
  RGB(192,255,255),         // notice
  RGB( 64,255, 64),         // topic
  RGB( 64,255, 64),         // join
  RGB( 64, 64,255),         // kick
  RGB( 64,255, 64),         // mode
  RGB( 64,255, 64),         // nick
  RGB( 64, 64,255),         // part
  RGB(255,192,255),         // action
  RGB(255,255, 64),         // ctcp
  RGB( 64, 64,255),         // quit
  RGB( 64, 64,255),         // connecting
  RGB( 64,255,255),         // querytext

  RGB( 64,255, 64),         // info
  RGB(255,255, 64),         // warning
  RGB(255, 64, 64),         // error

  RGB(192,192,192),         // timestamp
  RGB(  0,  0,  0),         // userlist background
  RGB(255,255,255),         // userlist normal text
  RGB(  0,  0,  0),         // textinput background
  RGB(255,255,255),         // textinput normal text
  RGB(  0,  0,  0),         // global userlist background
  RGB(255,255,255),         // global userlist normal text

  RGB(  0,  0,  0),         // serverlist background
  RGB(255,255,255),         // serverlist normal text
  RGB(255, 64, 64),         // serverlist disconnected
  RGB(255,255, 64),         // serverlist connecting
  RGB( 64,255, 64)          // serverlist connected
  RGB(255,255,255),         // serverlist serverdirtytext 
  RGB(255,  0,  0),         // serverlist serverdirtybackground 
  RGB(255,255,255),         // serverlist dccchatdirtytext 
  RGB(255,  0,  0),         // serverlist dccchatdirtybackground 
  RGB(255,255,255),         // serverlist channeldirtytext 
  RGB(255,  0,  0),         // serverlist channeldirtybackground 
  RGB(255,255,255),         // serverlist querydirtytext 
  RGB(255,  0,  0),         // serverlist querydirtybackground 

  // background is not reversed for these items:
  RGB(255,  0,  0),         // transferlist errortext,
  RGB(  0,255,  0),         // transferlist completetext,
  RGB(  0,  0,  0)          // transferlist normaltext,

#endif
};
*/

/*
//see enum color_id in HydraIRCTypes.h
char *g_ColorItemNames[GUI_COLOR_ID_MAX] = {
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

  "Item_info",            
  "Item_warning",         
  "Item_error",           

  "Item_timestamp",       

  "Item_userlistbackground",
  "Item_userlistnormaltext",
  "Item_textinputbackground",
  "Item_textinputnormaltext",
  "Item_globaluserlistbackground",
  "Item_globaluserlistnormaltext",

  "Item_serverlistbackground",
  "Item_serverlistnormaltext",
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

  "Item_transferlisterrortext",
  "Item_transferlistcompletetext",
  "Item_transferlistnormaltext"

};
*/