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

// Audio.h : declatation of the NotificationManager class
//
/////////////////////////////////////////////////////////////////////////////

enum SoundIDs 
{
  SID_DEFAULT,
  SID_SYSTEMQUESTION,
  SID_SYSTEMNOTIFICATION,
  SID_SYSTEMEXCLAMATION,
  SID_SYSTEMASTERIX,
  SID_LAST
};

// see g_NotificationEventNames, must have corresponding items
enum NotificationEventCodes
{
  // all these items can be configured for the defaults
                   // Data paramater   Notes           
  NE_LOGGEDIN,     // NULL             we logged in to a server
  NE_DISCONNECT,   // NULL             we disconnected from a server
  NE_WEKICK,       // NULL             we were kicked from a channel
  NE_WEJOIN,       // NULL             we joined a channel
  NE_WEPART,       // NULL             we left the channel
  // add more here.

  // but only items below here can be configured for a buddy group.

  // Note: NE_GROUPSTART is used as the dividing reference
  NE_GROUPSTART,
  NE_HIGHLIGHT = NE_GROUPSTART,//      some text was highlighted by a filter
  NE_JOIN,         // nick             a buddy joined a channel
  NE_KICK,         // nick             a buddy was kicked from a channel
  NE_PART,         // nick             a buddy left a channel
  NE_QUIT,         // nick             a buddy quit irc.
  NE_PRIVMSG,      // nick             someone (maybe a buddy) is /msg'ing us.
  NE_NOTICE,       // nick|server      someone (maybe a buddy) is sending a notice
  NE_CHANNELTEXT,  // nick             someone said something in a channel
  NE_ACTION,       // nick             someone did an action in a channel
  NE_TOPIC,        // nick             someone changed the channel topic
  NE_DCCCHATCONNECT,        // nick    connected to a dcc chat
  NE_DCCCHATDISCONNECT,     // nick    disconnected from a dcc chat
  NE_DCCFILERECEIVE,        // nick         
  NE_DCCFILESEND,           // nick         
  NE_DCCFILERECEIVECOMPLETE,// nick
  NE_DCCFILESENDCOMPLETE,   // nick
  NE_DCCFILERECEIVEFAILED,  // nick
  NE_DCCFILESENDFAILED,     // nick
  NE_CTCPPING,              // nick

  // NE_DCCFILEACCEPT
  NE_LAST
};

// here's how we store events in memory.
typedef struct NotificationEvent_s
{
  // used when adding and removing events.
  // this is so that plugins can add their own events and remove
  // them later.
  int   m_SourceID;  // Read-only, do not edit!

  // we match both of these these
  int   m_EventCode; // a valid number from NotificationEventCodes
  int   m_Flags;     // 0 = Disabled, NE_FLAGS_* defines.
  void *m_UserData;  // Misc data, NULL, or something valid.  It's type depends on the eventcode id.

  // and then play a sound using one of these:
  char *m_FileName;  // NULL for system beep, or a valid path+filename
  int m_SoundID;     // number of a system sound (e.g. default beep, exclamation)
                     // only used if m_FileName is NULL
                     // taken from "soundid"

} NotificationEvent_t;

/* Examples:

 NE_PRIVMSG
 "default_privmsg.wav"
 NULL
 -1

 NE_PRIVMSG
 "friend_privmsg.wav"
 (IRCUserGroup *)
 -1

 NE_DISCONNECT
 NULL
 NULL
 0                        ; See SoundIDs

 NE_HIGHLIGHT
 "highlight_mynick.wav"
 (ChannelFilter *)
 -1

*/

/*
 A plugin uses the NotificationManager like this:

 1) int MySourceID = g_pNotificationManager->ObtainSourceID();
 2) NotificationPrefInfo_t NPI; populate the fields
 3) NotificationEvent_t *MyNotificationEvent = g_pNotificationManager->Add(MySourceID,&NPI)
 4) store the result of the Add if you need to edit or remove your event later.
 5) repeat 2,3,4 as many times as you like.
 6) run your program, or sit and wait till you're told by the main app to cleanup.
 7) DeleteSource(MySourceID);
 8) You now cannot use any returned values from Add() or ObtainSourceID().
*/
class NotificationManager
{
private:
  CSimpleArray<NotificationEvent_t *> m_NotificationEventList;
  IDManager m_SourceIDs;

  void DoNotification(NotificationEvent_t *pAEvent);

public:
  ~NotificationManager( void );
  NotificationEvent_t *NotificationEvent(int EventCode, int NetworkID, const void *Data);

  // do a notification via a NotificationPrefInfo_t struct.
  void DoNotification(NotificationPrefInfo_t *pNPI);

  // pass the return value as a parmater to Add(), return -1 if error
  int ObtainSourceID(void);

  // the data in pNPI is duplicated, so you can free(pNPI) safely after calling this
  NotificationEvent_t *Add(int SourceID, NotificationPrefInfo_t *pNPI);

  // if you want to delete a specific event, and you remembered to keep the return value
  // from the Add call you can pass it as a parameter here.
  void Delete(NotificationEvent_t *pNE);

  // to delete all the events associated with a particular source, call this.
  // note: you can still use your sourceid after this to add more events..
  void DeleteEvents(int SourceID);

  // to delete all the events associated with a particular source
  // *AND* relinquish the sourceid, call this.
  void DeleteSource(int SourceID);

  // delete ALL the events.
  void DeleteAll(void);


  void PlaySoundID(int SoundID);        // TODO: move somewhere else ?
  void PlaySoundFile(char *FileName);   // TODO: move somewhere else ?

};
