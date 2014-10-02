CHANGES.TXT
===========
There is lots of good information about HydraIRC in here, if you can't
figure out how to do something, then search this file and you'll probably find
what you're looking for.

This file contains lots of developer notes as well as changes.

Key:

  * Addition/Improvement
  + Bug fix
  - Other
  . personal note
  (name) - the name of the person running HydraIRC who first reported
  the problem or requested the feature

25/April/2008 - AM (0.3.166) - Experimental
  Medlir
  + Fixed DCC connect issue with some IP addresses, if you had a problem with DCC file transfers
    in HydraIRC please try again as this issue affected everyone that had an IP address that
    started with a number over 127.x.x.x


12/December/2008 - PM (0.3.165) - Released
  + Fixing remote DoS exploit in the parsing code for long irc:// urls when hydrairc's
    url handler is enabled.

25/July/2008 - PM (0.3.164) - Released
  * Added /listplugins - outputs plugin information to "Output" view.   
  + Made the winamp button appear on the toolbar again (broken in 0.3.162)
  * Stripping of winamp tag changed in order to be compatible with Media Monkey (finally!)
  * Re-worked the layout of the prefs window and it's treeview in preperation for some serious work
  * Changed the default format for the winamp plugin so it doesn't say winamp as no-one seems to use
    winamp these days anyway :D

25/July/2008 - PM (0.3.163) - Experimental
  * Added /omsg to alias plugin (same as /raw PRIVMSG @<channel> :<message>)

25/February/2008 - PM (0.3.162) - Experimental
  SVN Revision ??
  . Compiled with VS 2008 Team Studio
  . Compiled with WTL 8 - 7161
  . Compiled with ATL 9 - 70425
  
  - TODO: Currently the Winamp toolbar icon no-longer appears, cause unknown, use /winamp instead for now

17/July/2007 - PM (0.3.161) - Experimental
  SVN Revision 95
  Hydra
  + Users receiving files from other clients on systems with an external ip with the first octet higher
    than 127 would have been unable to receive the file due to a signed bit problem in the DCC file transfer
    code.
  * Changed "Router Address" to "External Address", as it makes more sense especially in this day and age.

05/May/2007 - PM (0.3.160) - Released
  SVN Revision 94
  Hydra
  * Reverted the multi-column treeview code back to the old single column treeview code in the connect dialog
    until a fix for the missing tvn_* messages is found.

24/April/2007 - PM (0.3.159) - Experimental
  SVN Revision 90
  Hydra
  * Added a multi column treeview to the connect dialog so it shows the description for the server
  * Updated the response handler for /links so that it updates the list of servers for the network that
    the /links command was issued on.  Prefs are not saved automatically, so to get the updated server
    list stored enter prefs and click ok.
  * Fixed two minor memory leaks in the prefs code.
  * Updated quit messages :D

24/April/2007 - PM (0.3.158) - Experimental
  SVN Revision 87
  Hydra
  * Recompiled with VS.Net 2005 SP1 which includes updated CATLRegExp code that hopefully handles
    high-ascii characters properly (and about time too Microsoft...)


10/April/2007 - PM (0.3.157) - Experimental
  SVN Revision 87
  Hydra
  * Fixing a reconnect bug introduced in 0.3.156 (while not re-introducing the 
    "socket disconnected" bug)
  
07/April/2007 - PM (0.3.156) - Experimental
  SVN Revision 85
  Hydra
  * Added a fix for "socket disconnected" when reconnecting to a server.
  * Added code back in that hides tabs for hidden windows that got lost when Daniel Bowen's updated
    tab code got added in 0.3.153
  * Updated some keyboard accelerator (shortcut) keys.
  * Added /timer and /stoptimer commands.  To use a timer create a command profile and then use
    /timer <delay in seconds> <command profile name>
    To stop it, use
    /stoptimer <command profile name>
    Timers are run until stopped, whether the window they were started in is connected or not.
    Timers can start other timers
    Timers can stop other timers
    The two commands ONLY work on timers for the current window.
    When a window is closed all it's timers are stopped.
    
    /timer in combination with /getnick can be quite handy from troublesome nick-stealers :)
  * Changed handling of ERR_NEEDREGGEDNICK as freenode servers seem to send a message when joining
    a channel that caused hydrairc problems.  This might cause other issues when a channel really
    does need a registered nick, needs further testing.

05/April/2007 - PM (0.3.155) - Experimental, debug only release.
  SVN Revision 84
  Hydra
  * Added a fix for a crash caused by malicious users sending malformed dcc file transfer requests
  * Added a fix for a crash which could be caused remotely if the user was connected to a non-rfc
    compliant irc server *and* a malicious user sent a really long malformed message to you.
    (Basically, a fix for something that'd never happen in real life, but *sigh* these security
    'experts' have nothing better to do, next time, a source diff an an email would be nice...)

06/October/2006 - PM (0.3.154) - Experimental
  Hydra
  * Some changes I can't remember. :D  I'll update this file when I've looked at the svn changelog :D
  
21/May/2006 - PM (0.3.153) - Experimental
  SVN Revision 80
  Hydra
  * Updated to WTL 8.0
  * Updated Daniel Bowen's tab controls
  * Compiles in Visual Studio 2005, older versions are now unsupported by me
    some things may be broken:
    1) hiding tabs
    2) possible crashes when closing windows, might still need to 
       migrate changes from atlframe.h wtl 7.5 altered includes to
       a copy of the respective wtl 8.0 code
  

14/Apr/2006 - PM (0.3.152) - Experimental
  SVN Revision 79
  Hydra
  * Added a new pref: "Activate Link On Double-Click", Default: FALSE
    By default you open now URL's with a single click, to restore the old
    behavour enable this item.  This brings HydraIRC in-line with most other apps.
  * Added ONOTICE alias, usage: /onotice <message> to send a message to all 
    the ops in the current channel.
    Note: you must be an op to be able to use this.
  * Added a timestamp to outgoing CTCP PING requests.
    use "/CTCP <nick> PING" to ping someone

  Hydra+Wicked
  * Updated to WTL 7.5 Final
  
  Excelsior
  * Extended plugin API
  
  Wicked
  * Fixed a few minor memory leaks
  
  Medlir
  * Some nice changes to the Eventlog to make it more usable
  + Fix to stop command profiles running twice if server and network name are the same
  * Basic Ping timing
  
  Motoko-ch
  + Fixes the conflict in the key for the right-click server menu (Logging 
    and Channel List were sharing the same key). Moves Logging to G
  + Fixes mis-spellings in slap messages
  + Replaces &nbsp in Server 2003 with a real space

  * Adds custom winamp playing messages - Allows for display of extra
    features in messages
    
    Added plugins\playing.ini - edit the first line to configure the winamp
    plugin output
    WinAMP.ini should be placed in same directory as WinAMP.dll or built-in
    message will be used


19/Nov/2005 - PM (0.3.151) - Released
  SVN Revision 66
  Hydra
  * Fix to /WINDOW position activiating windows
  * Fixed non-working buddy groups in 0.3.150.

14/Mar/2005 - PM (0.3.150) - Experimental - for #HydraIRC users only.
  SVN Revision - not commited
  Hydra
  * Fixed the bug where an ignoremask instead of a hostmask was shown in the
    header and window title of query windows when someone /msg'ed you
    It now correctly displays the hostmask.
  * Added hostmask matching to /ignore - note the /ignore command now needs
    re-working a bit to make it easier to specify just a mask, but it'll do
    for now - the rest of the work that actually does the ignoring is done
    at least..
    
    To ignore a user by an ignore mask of the form nick!identname@host do this
    
    /ignore +nick mask=<ignoremask>
    
    e.g.
    
    /ignore +spammer mask=*!*@*spammers-r-us.com
    
    and this will ignore everything coming from any user with any nick coming
    from a host that ends with spammers-r-us.com
    
    Someone should update the wiki with this information and with an
    explanation of hostmasks and ignoremasks.
    
    ideally we want to just be able to do this:
    
    /ignore <ignoremask> but you can't yet, sorry.
    
    Additionally i added the NOTES=<notes_with_no_spaces> option to /IGNORE so
    you can make a note about why you're ignoring this person.
    
  - Added a couple of bugs and the info to the TODO.TXT after investing them
    for an hour or so.
  - Also had a look at doing host matching on buddy groups, but the host matching
    stuff will require a re-write of the way buddy groups are a) stored and 
    b) used internally (basically, we can't use the IRCUser class for buddy groups
    and the whole CustomGroupID business has to go)
    Maybe when i do the Friends docking panel I'll write some new classes for storing
    display names, current nick+hostmask and buddymask match and then use them for
    buddy groups.

27/Jan/2005 - PM (0.3.149) - Released
  SVN Revision ??
  Hydra
  * HydraIRC doesn't quit if it encounters riched20.dll 5.0.152.0.
    It will not continue but will still show the warning each time
    you start the app.
    (This is mainly for users of Windows 98)
    
18/Jan/2005 - PM (0.3.148) - Released
  SVN Revision ??
  Hydra
  * Recompiled and uploaded for web release
  
04/Jan/2005 - PM (0.3.147) - Experimental - for #HydraIRC users only.
  SVN Revision ??
  Hydra
  * When you're disconnected from a server a timestamp is now part of the
    disconnected message that's displayed.

22/Dec/2004 - PM (0.3.146) - Experimental - for #HydraIRC users only.
  SVN Revision ??
  Hydra
  * Made CTCP reply's work during the login phase - some servers send
    CTCP version requests to all connecting clients.
    e.g. irc.sorcery.net (jimmothy)
    
22/Dec/2004 - PM (0.3.145) - Experimental - for #HydraIRC users only.
  SVN Revision 62
  Hydra
  * Added a new pref: "Channel Rejoin Delay Seconds", Default: 60
    If, when reconnecting to a channel, the server returns unavailable
    resource (numeric 437 - ERR_UNAVAILRESOURCE) HydraIRC will
    attempt to reconnect to the channel after a delay equal to
    this preference.
    This will help you stay in your channels after a netsplit.
  * Added a new PREF: "Kick Rejoin Delay Seconds", Default: 10
    When kicked, HydraIRC will wait this many seconds before rejoining
    the channel automatically - before hydrairc instantly rejoined.
    to instantly rejoin, set the value to 0 seconds.

19/Dec/2004
  Hydra
  + Updated the QUIT messages - going for a more professional feel which
    matches the website's taglines..

17/Dec/2004 - PM (0.3.144) - Experimental - for #HydraIRC users only.
  SVN Revision 61
  Hydra
  + Fixed focus issue when creating a new query window from the user
    list's context window (MiaCo)
    TODO: still not fixed focus issue when double clicking a user
    though..
  + Fixed crashbug (actually, it was an assert) when a buddy from a buddy
    group left and the channel user list was not showing buddy groups.
    
14/Dec/2004 - PM (0.3.143) - Experimental - for #HydraIRC users only.
  SVN Revision 60
  Hydra
  + Fixed a crashbug for servers that don't pass a username for the
    topicend reply.
  * Tweaked the buddy groups dialog so that people can understand it
    better - simply added two groupboxs "Group" and "Members" and
    moved the controls into the relavent boxes.
  * Added On-The-Fly toggling of Buddy Groups in the user list.
    To toggle buddy groups for a given channel, right click on a
    group name or a blank area of the user list and select the
    "Show Buddy Groups" item (it's checked or unchecked depeding on
    current status)  this is per-user list too so you can have
    buddy groups on in one channel and off in another.
  * Added pref: "Show Buddy Groups In User List" default: TRUE
    When disabled buddy groups are not shown in the userlists (zann)
    Note: cycle the channels or use the new context menu on the
    user list after changing the pref to have the change take effect.
    If anyone wants a /command to toggle this on or off let me know.
  * Added checkmarks to the verbose level menu options on the
    channel window's context menu.
  + fixed auto maxize again (broke it in 0.3.140)

14/Dec/2004 - PM (0.3.142) - Experimental - for #HydraIRC users only.
  Hydra
  + Fixed displaying of nick prefixes in the URL grabber.
  + Fixed displaying of nick prefixes in the for logged Channel Highlights
  * You can now configure a notification for when someone changes the
    topic.

13/Dec/2004 - PM (0.3.141) - Experimental - for #HydraIRC users only.
  SVN Revision 59
  Hydra
  + Commented out a section of code that broke the restoring of auto-hiding
    docking windows in the docking window framework - emailed the author of
    it to find out what's going on. - auto hiding seems to work, but there
    may be another issue which I've not found yet.
  + Gliptic's nick prefix broke buddy group matching for highlight
    notifications, added a simple fix by storing the prefixes and unprefixed
    nicks and only ever using the unprefixed nick for nick matching in the 
    highlighting code.

12/Dec/2004 - PM (0.3.140) - Experimental - for #HydraIRC users only.
  Hydra
  * Finished code relating to unlocking output windows after the user performed
    a click-drag operation, sometimes the window could remain locked causing
    no output to appear in a window until the user left clicked in the window.    
    This only effects cases where timed locks are added, manual locking of a
    window still required manual unlocking of a window. (to unlock a window,
    just left-click it).
  * Added pref: "Scroll Lock Timeout Seconds", Default: 60.
    This appears just under Inactivity Scroll Delay in the prefs, generally you
    want both prefs to have the same value.
  + If a sound event was configured to play a sound file, then reconfigured to
    play a system sound the sound file would still play, this was by design.
    Now, if a sound event is re-configured to play a system sound hydrairc
    will play the system sound.  aditionally, if an event is configured to play
    both a system sound and a sound event (not possible unless the prefs are
    mucked around with) then both will be played. finally, if a sound event
    is configured to play a sound, but the filename is blank then the default
    beep will play instead. (zann)
  * Upgraded the docking framework and the tabs to the latest version
    (from Daniel Bowen's article on code project which contains Sergey's
    original code and lots of updates)
  * Added the MDI tabs back in, let me know if you have any issues.
    I rewrote the window hiding code a while back, and re-implemented the
    MDI tab code which now uses the new window hiding code and initial tests
    seem to be good.

11/Dec/2004
  Hydra
  * Removed all remaining _open _seek _write _read _tell and _close
    now using CreateFile throughout, did basic dcc send, receive, resume send
    and resume receive, all ok. - please test on a LAN and let me know if speeds
    improve and if cpu usage is better.
  * Added ": <message>" to eventlog "<user> is talking in <channel>" messages
    (in the Event Log, shown when you're using a ChannelText notification
    and your buddy is talking).
  * Renumbered verbose levels, 3 is MORE verbose 1 is less verbose
  * Added #breed-sucks to the quakenet favorites (removed #breed from quakenet
    favorites)
    And next time Brat/CDV, don't release games if they're not finished.
  * IP addresses shown in status message when dcc xfers are connecting
    (in addition to appearing in the IP/Port column)
  * Added the message: "Resume Error: A larger file already exists" when a
    user sends you a file that's smaller than one you already have in your
    downloads folder.
  * Added Pref "Server List Channel Groups" default: FALSE
    This enables and disables the grouping of channels under their own tree
    view node ("Channels") in the server list.  If you're on lots of networks
    this option can save valuable screen real estate.
    TODO: you can toggle this with open channels, but channels won't move into
    and out of the channels group, however new channels will appear in the
    right place.    
    
  Gliptic+Hydra
  + Fixed DCC Send, DCC Chat and WhoIs query context menu items not working
    correctly.
  + If a DCC transfer speed reaches 0 "Stalled" is shown in the ETA column
    instead of a crash.
  * Added pref: "DCC Auto Accept", Default: TRUE
    When set to FALSE the transfer will be shown in the transfer window as
    "Pending...", right click it and select "Accept" to start the transfer.
  * Added pref: "Show Nick Prefixes", Default: TRUE
    When enabled, this shows the @'s +'s etc for the user in the channel when
    they talk.

  Gliptic
  * Added pref: "Clear Dirty Flag On Deactivate", Default: TRUE
  * Added drop target capability, drag and drop files on to query windows to
    have them sent via DCC.

27/Sep/2004 - PM (0.3.139) - Experimental - for #HydraIRC users only.
  Hydra
  * Fixed a small crashbug on the +h mode handling for IRCPlus/2000 Servers (appetiser)
  
19/Aug/2004 - PM (0.3.138) - Released
  Hydra
  * Upgraded to WTL 7.5.

02/Aug/2004 - PM (0.3.137) - Experimental - for #HydraIRC users only.
  Hydra
  * Updated /help
  * Removed alias /exitnow
  * /EXIT's -NOW parameter has the preceding "-" removed, so it's just "/EXIT NOW", now. :)
  * Removed /TOGGLEAUDIOMUTE
  * Removed /TOGGLEUSERLIST
  * Removed /SHOWUSERLIST
  * Removed /HIDEUSERLIST
  * Removed /TOGGLEHEADER
  * Removed /SHOWHEADER
  * Removed /HIDEHEADER
  * Removed /TOGGLEINFO
  * Removed /SHOWINFO
  * Removed /HIDEINFO
  * Removed /SHOWWINDOW
  * Removed /HIDEWINDOW
  * Removed /MAXIMIZEWINDOW
  * Removed /POSITIONWINDOW
  * Removed /CLEAR
  * /AUDIOMUTE, supports ON, OFF, TOGGLE and STATUS params
  * Added /MONITOR, supports STOP, START and STATUS params
  * Added /LOGGING, supports STOP, START and STATUS params
  * Added /WINDOW
  
    /WINDOW is now your primary command for enabling disabling UI features
    
    It supports the following arguments
    
    SHOW
    HIDE
    MAXIMIZE
    POSITION
    USERLIST [TOGGLE|SHOW|HIDE]
    HEADER [TOGGLE|SHOW|HIDE]
    INFO [TOGGLE|SHOW|HIDE]
    CLEAR [ALL]
  * Added support for IRC numeric 477 - ERR_NEEDREGGEDNICK, this is returned on
    some networks when joining a channel that required your nick to be registered
    with a nickserv service.
    After a disconnect you needed to close the channel window and cycling wouldn't work
    but now it does.
  + Fixed bug in GetTokens's handing of quoted text, you can now join channels
    like #a'b'c, #""", and #''' correctly.
    This may have broken other stuff else, please test!!
  * Added version information resource blocks to all plugin dll's
  * Added keyboard accelerators that TOGGLE the display of items on the view menu.
    Note that if the docking window is displayed, but isn't active then it's hidden, and
    not brought to the front.  If you'd like different behaviour (i think i do) then
    let me know. (e.g. key shortcut to makes docker active if not active, hides otherwise)

06/May/2004 - PM (0.3.137) - Experimental - for #HydraIRC users only.
  Hydra
  * Update to the server list item name building code.

04/May/2004 - PM (0.3.136) - Experimental - for #HydraIRC users only.
  Hydra
  * More work on the channel list dialog
  
    Open it via the server list's server items context menu (right click
    on a server in the server list and select "Channel List") or from
    the "Channel/Channel List" menu item in the main window's menu or from
    a server window's context menu.
    
    You can now populate the channel list - when a channel list dialog is open
    for a server all /list command output is grabbed by the dialog.

    /list still does /list
    
    You can also select (or multi-select) channel(s) from the list and click
    the Join button to join the channels.
    
    Double clicking a channel will also join it.   
    
    The dialog stays open until you close it.
    
    If you close the dialog in the middle of a /list operation then the remainer
    of the channel list will go to the server window as normal.
    
    The dialog is resizable.
    
    The columns in the channel list dialog can be sorted by name, users and
    topic in ascending or descending order. (Click the column headers).
    
    TODO: Not all GUI elements are usable yet.
    
  * Added pref: "Show Channel List Dialog On Connect", default TRUE
    note: only shown when connecting to the network for the first time
    and when there are no *_OnLoggedIn command profiles for the server
    being connected to.
  + Fixed "Channel/Properties" menu item being available in some circumstances
    (e.g. after opening HydraIRC with shift key held down..)
  * Server list now shows network name for servers (if known), this makes
    it easier to find the channels you want as they'll always be in the same
    place on the server list no matter what server you're using for the network.
  
28/April/2004 - PM (0.3.135) - Experimental - for #HydraIRC users only.
  Hydra
  * Added option for audible and visual notification events for CTCP Pings
  + Fixed channel list window XP redraw issues (channel list window still 
    not finished yet..)
  + DCC File xfer events are only logged to the event log if notification
    events are created for them.
    
23/April/2004 - PM (0.3.135) - Experimental - for #HydraIRC users only.
  Hydra
  + Fixed server window kept popping to front after a reconnect (introduced in
    0.3.134)
  + Fixed Notice log using MaxPrivMsgLogItems instead of MaxNoticeLogItems
  * Added PREF_nMaxURLCatcherAgeMins, PREF_nMaxEventLogAgeMins,
    PREF_nMaxURLCatcherItems, PREF_nMaxEventLogItems,
    PREF_bRemoveOldURLCatcherItems and PREF_bRemoveOldEventLogItems.
          
    Note, if you DISABLE any of the "Remove Old ... Items" prefs you
    *will* run out of memory eventually, it just might take a while.
    They're intended for disabling, then let the logs pile up, then you can
    look at the oldest item that's still useful to you at which point
    you can then set the other two related prefs items ("Max ... Items" and
    "Max ... Age Mins") according to your tastes and then re-enable
    the "Remove Old ... Items" prefs item.
  * Changed the defaults for all the above named prefs so they log more by
    default. (4320 mins (3 days) worth or 300 items, whichever comes first).
    
19/April/2004 - PM (0.3.134) - Experimental - for #HydraIRC users only.
  Hydra
  * updated URL Catcher:
    - Duplicate URL's are not added to the list of urls, so if there's someone
    advertising a URL in a channel every 5 mins your URL grabber won't fill up
    with loads of lines each with the same URL in it.  Now it just finds the old
    item in the list and updates the item's "Count" column by 1.
    - Visited URL's are drawn in a seperate color.
    See "prefs/misc/colors/Item_urlcatchervisitedurltext"
    - Default URL color changed to match internet explorer defaults.
  + When using an existing window to connect to a server for a network that
    specifies a non-default identity to use we correctly select the user identity
    to use (and we log a message to the Output window to this effect).
    This will help when your default identity is not your first identity and you
    connect to a server via your OnStartup command profile.
  * /server and /newserver now accept IRC URI's
    
    /[new]server [irc://]<server>[:port][[,<password>]|[/<uriparams>]
    
    examples:
    
    1) change current server to irc.efnet.net, port 6667 and join #test
    
    /server irc://irc.efnet.net:6667/#test
    
    2) new server window to myserver.mydomain.com port 7000, server password is mypassword

    /newserver myserver.mydomain.com:7000,mypassword

    3) change server to irc.efnet.net and open a query window to "hydra"    
    
    /server irc://irc.efnet.net/hydra,isnick
    
    todo: you can't specify a server password *and* uri params yet why? because the irc
    uri specification sucks balls and needs updating, we only support [,<password>] because
    the old /server and /newserver commands supported it.
  + Moved /uptime into it's own seperate plugin so that the default plugin will work
    on windows 98.  /uptime is only for Windows NT based systems (e.g. nt,2000,xp) because
    it uses a high resolution uptime counter from the system's performance monitor library
    which doesn't exist on windows 98.
  * When using the nick completer, the nick that is completed is highlighted in the
    current window's user list.  Makes it nice and easy to locate a user in the list
    so you can right click their name.
  * It's now possible to have all notifications logged in the eventlog.  
  
    Edit your existing notifications and enable the "Log in Eventlog" checkbox.
    There is a new "Log" column in the list of events that should help you.
    
    You will probably need to ADD new events to the "All Networks/Defaults" buddy group
    so that events that were previously logged to the eventlog are still logged in
    this new version.
    There should be an updated example configuration supplied with HydraIRC.
    
    You could just cut and paste the <notifications> section from it to your existing
    config for the "-2" group... 
    
    <notifications>
      <general>
        <group id="-2">        
          ... cut and paste these lines from the new config ...          
        </group>
        
    If you do this all the things that were previously logged will still be logged,
    but if you made any changes to those notifications you will have to make your changes
    again.
    
    There is no easy way for me to convert old configs, sorry!
      
  . It's late, not tested all these changes, uploading so people can test and report
    I still have to make some eventlog items obey the "Log To Eventlog" option.
    A basic updated example config included, wait for at least 0.3.135 for a more
    complete one.

14/March/2004 - PM (0.3.133) - Experimental - for #HydraIRC users only.
  Hydra
  + fixed logging of two servers with the same name but different ports configured
    under different networks logging to the same folder (e.g.  two psyBNCs)
  + fixed creating logs for channels with *|:\/<> in their names.
  + fixed problem deleting networks from connect dialog (AK47)
  + a few other fixes too.
    
14/March/2004 - PM (0.3.132) - Experimental - for #HydraIRC users only.
  Hydra
  - Oops, accidentally compiled 0.3.131 with the broken regexp parser causing
    the app to crash when it processes certain text through certain regexps.
  * Added /savetheme command, usage: <ThemeName> <Filename>
    Filename can be relative or absolute.
    Themename is something like "Basic" or whatever.
    Currently it only saves HydraIRC's default theme (Basic) so it's not much
    use to anyone other than theme designers that have the source code and are
    adding new themes to the core app.  However, if you wanted to export the
    theme to a single file ready for modification then it's useful.
  * Reworked/renamed some internal code in the prefs.cpp file.

13/March/2004 - PM (0.3.131) - Experimental - for #HydraIRC users only.
  Hydra
  * AK47 sent in a patch for some changes to the connect dialog, I didn't
    use his patch but it prompted me to tidy up the source and add some
    new features to the connect dialog.
    
  * Added context menu to the connect dialog's Network items, you can
    now add/delete network items and also edit networks that you've added
    You can't edit names of networks that are supplied with HydraIRC, 
    though you can select the prefered identity for a given network and
    you can still edit your preferences for the servers that are supplied with
    HydraIRC.
    
  * When adding a server, the network combo box in the add/edit server dialog
    now defaults to the current network, saves you having to select it each time
  + Fixed tab order in Add/Edit server and Add/Edit Network dialogs.
  + Prefs are only saved once if multiple changes are made to the
    server/network list via the connect dialog.
  * Adding items to favorites causes the date to be added to the favorite name
    e.g. "#bleh on efnet (dd/mm/yy)"
  + Fixed the input line focus to wrong window after clicking title bar of
    inactive window when making the application itself active again.
    (Calling ActivateTextInputPane from OnMDIActivate instead of SetFocus)
  + Fixed the incorrect selection of the first identity rather than the preferred
    identity for the network when using the /server command and when using the
    favorites menu.
  * When pasting multiple lines from the clipboard, the text is no-longer parsed
    for commands and whitespace is no longer stripped.   Finally I can paste
    my ASCII art! ;)
    Note: if anyone wants prefs to use the old behaviour for this then please
    let me know and I can easily add them.
  + Fixed double whois when opening a query window from favorites
    
01/March/2004 - PM (0.3.130) - Experimental - for #HydraIRC users only.
  Hydra
  * Tidied up and applied fryguy's patch for url catcher and misc changes (see below)
  
    TODO: (for fryguy)
    
    1) add support for query windows
    2) adjust regexp to match urls that start with "www." are numeric or end
       in a valid address (e.g. .com, .co.uk) so that addresses like 
       "foo.bar.co.uk" and "82.23.12.254" are also captured.
    3) Adjust the url catcher so that you can launch other applications via
       context menu (e.g.  telnet, ftp) 
    4) create an icon for the urlcatcher's docking tab/window
    5) add support for visited url colors (inherit the textqueueitem class)
  * Added new prefs for chaning the colors of the urlcatcher, notice, query,
    eventlog and transfer window items and backgrounds.
    See colors section in options\prefs\misc.

    IMPORTANT: Theme authors, please submit updated themes to me!
  
  FryGuy
  * Added a URL Catcher that captures urls from output (in channel windows only)
    that start with "http://".
  * Added "DCC Send File" to query window context menus.
  - Changed shortcuts for some menus.
    

27/February/2004 - PM (0.3.129) - Experimental - for #HydraIRC users only.
  Hydra
  * Fixed mis-applied patch causing lack of userlist updates when user modes change.

26/February/2004 - PM (0.3.128) - Experimental - for #HydraIRC users only.
  Gliptic
  * Added complete user mode support.
  + Added fix for crash when channel creation timestamp was missing
  
  Hydra
  * Patched in gliptic's l33t new user mode support,  now user prefixes like "&" and
    "~" are supported!
  + Fixed /getnick interfering with /nick.
    

12/February/2004 - PM (0.3.127) - Experimental - for #HydraIRC users only.
  Hydra
  + Fixed scrolling to bottom issues that got broken recently.
  * All output buffers are initially filled with blank lines, this will cause
    all text to scroll up from the bottom of the window.
    (Which, btw, is how I've always wanted it, but haven't done due to richedit
    issues...)

12/February/2004 - PM (0.3.126) - Release - Downloads: 34130 - Hits: 118647
  Hydra
  + Fixed /NOTICE that i accidentally broke (it didn't actually send the notice)
  + Fixed single line display of info panel text, was supposed to be multiline.
  
11/February/2004 - PM (0.3.125) - Experimental - for #HydraIRC users only.
  Hydra
  + Fixed broken <servername>_onloggedin command profile (i broke it in 0.3.124)
  + Fixed crash when sleecting from global user list context menus when the
    current MDI window was not a channel window.
  * Upgraded CommandBarXP code to latest version (which supports chrevon menus)
  + Fix to BuildFavoritesMenu() not releasing allocated IDs
  * height of the info panel is reset to it's default height when it's
    toggled on and off
  + Fixed selecting from child frame context menus causes lack of input line focus.  
    (only for some items, not fixing for other because a richedit replacement will fix this)
  + Fixed saving of high-ascii characters in command profiles (profiled)  
  + Another fix for the propertylist (profiled)  

  
09/February/2004 - PM (0.3.124) - Experimental - for #HydraIRC users only.
  Hydra
  + Yet another attempt to fix the close window crash bug.
  + "Retry" on DCC transfer context menu wasn't implemented, shortcut was wrong too
  + "Remove" on DCC transfer context menu was doing "Cancel" instead.
  * Renamed "Select All *" to "Select Only *" on the DCC transfer menu and also
    made it deselect everything first.
  + Fixed crash when trying to close a query window that had not been shown.
  + Fixed header pane memory leak every time header was changed (when visible or not)

08/February/2004 - PM (0.3.123) - Release - Downloads: 32903 - Hits: 115937
  Hydra
  * Added clipboard copy support for header and info panes via edit/copy and ctrl+c
  * Added HydraIRC icon to the prefs dialog.
  * Added a rather exhaustive context menu to the DCC Transfers docking window.
    If you didn't already know, you can also select all, retry, cancel and delete items
    using Ctrl+A, R and DEL respectively.
  * When a query window is created, a /whois command is issued to the server
    and the whois responses are directed to the query window.
    Note: this has a side effect of not showing whois responses in the current
    window if a query window is open for the user.
    Whois responses are still always shown in the server window.
    Note 2: the themes has to be updated, if you use your own theme add at least
    "Q" and "S" to the "appliesto" string for the whois items...
  + fixed a typo in one of the /slaps and made them work for both male and female
    IRC users ;)
  * Added context menus to the server list.
  + Fixed HydraIRC-Update that i broke in 122
    
  
07/February/2004 - PM (0.3.122) - Experimental - for #HydraIRC users only.
  Hydra
  * Switched the logger to use the clistener class instead of using
    GetTickCount(), also IRCServer, IRCQuery and IRCChannel no longer
    need to check the log file's flush status as the logger class does it
    by itself now.  Very minor CPU usage improvement and code size decrease.
    this will also fix the logger stopping flushing after 49.7 days of uptime
  * Switched statusbar to use time() instead of GetTickCount()
  * Added > 49.7day uptime resolution
  + Adding missing call to srand() for default plugin (now /slap will use
    a proper random order...)
  + Fixed "view log file" only worked if logging for the window was enabled.
  * Added De Voice to user-list context menu (`Duckman)
    
07/February/2004 - PM (0.3.121) - Experimental - for #HydraIRC users only.
  Hydra
  + Further tweaking of showing MDI windows, when you're using maximized
    windows.
  * Added SLAP to user-list context menu (XxBoBxX)

07/February/2004 - PM (0.3.120) - Experimental - for #HydraIRC users only.
  Hydra
  + Added Auto Maximize support back in.
  + Further tweaking of showing MDI windows, when you're using maximized
    windows you should notice that you don't see a "restored" window
    momentarily.  _vip and h0bgoblin, please confirm.
  + Fixed the two infinite loops in the propertylist control used in the
    misc page in prefs.  Again, credit goes to "profiled" for finding
    the bugs, even though his submitted patch wasn't quite the right
    way of fixing it. :)


06/February/2004 - PM (0.3.120) - Experimental - for #HydraIRC users only.
  Hydra
  + Fixed the crash when expanding/unexpanding items in the Misc page in prefs
    Credit goes to "profiled" for finding the cause, even though his submitted
    patch wasn't quite the right way of fixing it.
  * Aded support for stripping mIRC color 99 codes.
  
06/February/2004 - PM (0.3.119) - Experimental - for #HydraIRC users only.
  Hydra
  + Added in a fix for server forced channels that mrbkap broke in
    0.3.118.

05/February/2004 - PM (0.3.118) - Experimental - for #HydraIRC users only.
  Hydra
  * Added a new pref "Hide Not Minimize".  Default: FALSE
    Enabling this uses causes MDI windows to be hidden instead of minimized
    (like in 0.3.116-117 and some earlier pre 0.3.100 versions).
    The default is FALSE because the Windows MDI implementation does not
    handling hiding of windows very well and there are a couple of quirks.
    Note: if you use maximized windows then you're better off leaving this
    option set to FALSE.
  * Changed the default colors for quit/part/kick to be dark red instead of
    dark blue.
    
  mrbkap
  + fixes the bug where typing /names in a channel that you're already in
    clears the userlist
  + fix bug with wrong nick displayed in local echoing of CTCP replies.

05/February/2004 - PM (0.3.117) - Experimental - for #HydraIRC users only.
  Hydra
  + Tweaked window redrawing/changing (redrawing is disabled, then forced during
    calls to CChildCommon::ActivateWindow() (h0bgoblin, _vip)
  + Really fixed the about box size (was too small in 0.3.115-116)
  * CTCP requests that HydraIRC makes when DCCing files (accepting, sending 
    and resuming) or initiating DCC chats are echoed to the server window. (Zann)
  * /NOTICE commands are echoed to the server window now, like the were
    before /NOTICE was handled by the alias plugin. (Zann)
  * Fixed up and corrected the patch for ctrl+k99 handling.
    Here's an example of correct (HydraIRC) and incorrect (mIRC) handling
    of the 99 code.
    
    "ctrl+k8,2>>test1<<ctrl+k99>>fg 99<<ctrl+k8,2>>test2<<ctrl+k,99>>bg 99<<ctrl+k8,2>>test<<ctrl+k99,99>>fg,bg 99<<"
    
    Renders in hydrairc like this:
    http://www.hydrairc.com/images/colorrendering/correct-render-1.png
    and incorrectly in mIRC like this:
    http://www.hydrairc.com/images/colorrendering/incorrect-render-1.png
    
    Works in topic/header pane too.

04/February/2004 - PM (0.3.116) - Experimental - for #HydraIRC users only.
  Hydra
  + fix about box size (was too small in 0.3.115)
  * Added the fix to crashing when closing MDI windows to the WTL 7.1 source.
    CMDIFrameWindowImpl in atlframe.h
    and
    CWindowImplBaseT in atlwin.h
    The change to CWindowImplBaseT made no difference, but the change to
    CMDIFrameWindowImpl seems to have fixed it,  to test connect to a server
    then in the server window type in "/join a,b,c,d,e,f,g,h,i,j,k,l"
    then wait till it's joined all the channels
    then press and hold ctrl+f4, this will close all the channel windows
    one after the other, in 0.3.115 and before it would crash randomly when
    closing child windows, this fix seems to be working here but i need
    other people to verify this for me.  Please let me know via a PM in the
    #HydraIRC channel when you test this and state if it is OK or if it still
    crashes.  I have closed over 200 channel windows now with no crashes!
    
    Hopefully, if this is fixed then the only other crash bug is with the
    property view in the prefs - yay!
  * Changed a bunch of INT pref names so they say what the value is. (Zann)
    "DCC Chat Connect Timeout"
    is now
    "DCC Chat Connect Timeout Seconds"
    
    Note: If you're upgrading, back up your profile_default.xml, launch hydrairc
    then close HydraIRC and the profole_default.xml will have the new
    prefs items in it, then adjust the values so they're the same as in your
    old profile_default.xml file - all items are in the same order.
    If you don't do this then HydraIRC will just use it's defaults for the
    renamed items.
  * Re-wrote the window hiding routines, instead of child windows minimizing
    they're now hidden completely.  I checked for maximization issues and all seems good
    please report back if you have issues.   


04/February/2004 - PM (0.3.115) - Experimental - for #HydraIRC users only.
  Hydra
  * Added link to paypal donatation form to the menus "Help/Donate" - 

    Please donate as much as you can.  People that donate have their
    requested features implemented before all other people!
    
  * Show nick change message in server window when own nick is changed
  * Patched in a diff from Blake Kaplan for handling the stoopid mirc
    color code of ctrl+k99 (same as ctrl+o) (it resets the colors)
    NOT TESTED
  * Adjusted copyright and other credits in the about box.
  + Fixed single mouse clicking on url caused loss of focus to input line.
  + Possible bugfix to using the nick "Unknown" after a disconnect.
    NOT TESTED
  * Colors not lost in header panes when prefs are updated.

31/December/2003 - PM (0.3.114) - Experimental - for #HydraIRC users only.
  Hydra
  * Added some new prefs:
    "PREF_bShowChannelWindowUserListPaneByDefault" Default: TRUE
    "PREF_bShowChannelWindowHeaderPaneByDefault"   Default: TRUE
    "PREF_bShowChannelWindowInfoPaneByDefault"     Default: TRUE
    "PREF_bShowServerWindowUserListPaneByDefault"  Default: FALSE
    "PREF_bShowServerWindowHeaderPaneByDefault"    Default: TRUE
    "PREF_bShowServerWindowInfoPaneByDefault"      Default: TRUE
    "PREF_bShowQueryWindowUserListPaneByDefault"   Default: TRUE
    "PREF_bShowQueryWindowHeaderPaneByDefault"     Default: FALSE
    "PREF_bShowQueryWindowInfoPaneByDefault"       Default: TRUE
    "PREF_bShowDCCChatWindowUserListPaneByDefault" Default: TRUE
    "PREF_bShowDCCChatWindowHeaderPaneByDefault"   Default: TRUE
    "PREF_bShowDCCChatWindowInfoPaneByDefault"     Default: TRUE

  * Query windows only show Connected/Disconnected messages if they're the active window
    when the server is Connected/Disconnected.
  - Removed "Channel Window User List" pref.
  * Minor tweak for minimum user list size.
  * Tweaked focus issues when hiding panes
  * Tweaking the showing of info and userlist panes so that if the other pane
    isn't already visible it will not be made visible.
    (e.g.  if you had both panes visible, then did a /hideinfo and /hideuserlist
    then a /showuserlist you'd also see the info pane too, this no-longer occurs)
  * Disabled the Office XP-like menus due to two issues
    1) checked menu items with icons are not shown as checked.
    2) mdi command bar is not shown until the mouse hovers over the are it's
       supposed to be (! bah, what next...)
    I have to fix this two issues before they can be re-enabled, however they
    do look way cool when enabled.  Any developers reading this should be sure to
    peek at stdafx.h and look for the USE_XPCOMMANDBAR #define.
  * Headerpane height should be reflected when header text changes.
    (Missing a call to SendMessage(WM_HEADERCHANGED...), oops!)
    
  . Happy new year one and all!
  
22/December/2003 - PM (0.3.114) - Experimental - for #HydraIRC users only.
  Hydra
  * Added Office XP-like menus throughout HydraIRC - this took bloody ages. 
  . Merged FixedMDICommandBar.h back into atlctrlw.h (WTL 7.1 supplied file)
    so that the define "USE_XPCOMMANDBAR" can be used when compiling

21/December/2003 - PM (0.3.113) - Release
  Hydra
  * Tweaked status bar margin sizes
  * Added auto-maximize back in.  
  . Note: taskbar and mdi tabs have been removed, a switch bar will replace them
    in upcoming versions. (See below for more details...)
  
19/December/2003 - PM (0.3.112) - Experimental - for #HydraIRC users only.
  Hydra
  * Added in Daniels docking window focus handler, not sure if this is better
    or worse than what was in 0.3.110, we'll see.
    (0.3.111 was missing a docking window focus handler)        
  + Fixed the layout of the buddy groups prefs contained dialog (prefs
    window was too small to hold it)

18/December/2003 - PM (0.3.111) - Experimental - for #HydraIRC users only.
  Hydra
  * Updated Tabbed docking framework to Daniel Bowen's latest code,
    reapplied my own fixes to it and switched to using VC7 like docking boxes
    rather the the old-skool Outlook like docking boxes.
    
    This should also fix the Minimize App, Lock Workstation, Restore App
    and then Docking Window positions were incorrectly adjusted problem.

  * Re-kludged atlrx.h after loosing my already kludged copy when I rebuild
    my machine a few weeks ago, this was the case of all random crashes in
    builds since 0.3.091.  Thanks to hobgoblin and gliptic for the bug reports.

    atlrx.h - line 630 and 653 now should read:    
  	if (*sz == '\0' || (unsigned char)(*sz) > 127) // Hydra - fixed!
  	
  	Note: Developers building hydrairc should make the same change in their
  	copy of atlrx.h, at some point i'll provide this in the svn tree.
  	I still don't like the code, this is just a workaround, i'd rather it
  	was properly fixed.  bascially a crash is caused by any high-ascii
  	characters in the regexp input string (> 127), this fix just ignores them

  	Thus, don't use high-ascii characters in your regexps as they may not
  	work.  (but at least it won't crash...)  	
    
  . Having wisdom teeth removed sucks.
  
16/December/2003 - PM (0.3.110) - Experimental - for #HydraIRC users only.
  Hydra
  * Fixed disappearing MDI control buttons for maximized MDI child windows
    Many thanks to Nenad Stefanovic from Microsoft for the fix to the
    WTL 7.1 code.

16/December/2003 - PM (0.3.109) - Experimental - for #HydraIRC users only.
  Hydra
  * Added a fix for missing Child Window list on the "Window" menu.
    You can now change between windows using the Window menu.
    (Many thanks to the WTL Mailing list)
  - Spent hours looking into the disappearing MDI maximized window
    controls issue.  Found a way to reproduce it outside of HydraIRC
    info posted to WTL Mailing list, awaiting responce...
  - All windows are created maximized when the client starts up.
    TODO: This will be a setting in the future
    
16/December/2003 - PM (0.3.108) - Experimental - for #HydraIRC users only.
  Hydra
  * Tweaked Channel, Server and Query context menus
  * Moved menu item "Channel\User List" to "Options\Toggle User List"
  * Added menu options for toggling Info and Header Panes
  
  * Patched in FryGuy's /sysinfo stuff, many thanks!
  
16/December/2003 - PM (0.3.107) - Experimental - for #HydraIRC users only.
  Hydra
  * Added new pref "Max Header Lines", default: 3
  * Fixed another resource leak that i added in 0.3.106 ;D
  * Header controls now process IRC color codes, so topic texts show up
    in the headers nicely now!
  
16/December/2003 - PM (0.3.106) - Experimental - for #HydraIRC users only.
  Hydra
  + fixed a buffer overflow in the output formatter routine
    It was using MAXBUFFERLEN rather than MAXBUFFERLEN-1 and then ended up
    doing variable[MAXBUFFERLEN]=0 causing a 1 byte overlow.
  * Status bar widths are automatically updated to fit content
  * When /getnick is used, the nick HydraIRC is attempting to get appears
    brackets after the nick in the status bar.
  * When you issue a /getnick command, a nick change is attempted immediately
    previously it would only activate nick retention.
  + Fixed possible crashbug with /getnick
  + Played around with nick retention code, it might be broken..
    When a nick *is* changed (the server tells us) then you see the
    RPLS_NICK message
    When we tell the server we want to change out nick we wee the
    RPLS_REQUESTNICK message
    This should help avoid some confusion.
    
    What should happen is this:
    
    When you connect for the first time it will attempt to use all your nicks
    in your profile in order.
    
    If you get disconnected a note is made of the nick you were using.
    
    When you reconnect the last nick you were logged in as is used.
    
    If that nick is in use it will cycle from the first nick again.
    
    When the old nick you were using is no longer in use and hydrairc
    spots the ghost disconnecting it will attempt to use that nick.
    
    Note: If you had a nick set by /getnick then this will be used
    in preference to the last nick you were using when you were connected 
    when the disconnect happened.
    
15/December/2003 - PM (0.3.105) - Experimental - for #HydraIRC users only.
  Hydra
  + fixed GDI leak in new pane container drawing code. (`duckman)
  + fixed a buffer overflow in the alias processor where length of
    alias was longer that input string, e.g.  typing "/q" by itself caused
    this because the "/query" command for the "/q" alias was copied into
    a buffer that was too short.

12/December/2003 - PM (0.3.104) - Experimental - for #HydraIRC users only.
  Hydra
  + Fixed not scrolling to bottom of text when a non-active maximized MDI
    window is made active. (Zann)
  + Fixed lack-of focus to text input window after focus to a child window
    is restored.
  * Ability double click url's from Header windows
  - Checked all WM_* return codes and fixed a couple, this might fix some
    other issues
  - Code cleanup
  - Using ES_DISABLENOSCROLL (no why didn't i spot that before...)
    'Disables scroll bars instead of hiding them when they are not needed.'
    I was manually forcing the scrollbars on for richedit controls prior
    to this.
  
12/December/2003 - PM (0.3.103) - Experimental - for #HydraIRC users only.
  Hydra
  + Fixed /toggle* breaks scrolling of the console (zann)
  . Tweaked metrics
  
11/December/2003 - PM (0.3.102) - Experimental - for #HydraIRC users only.
  Hydra
  * Brand new child window layout (Info Pane, Header/Topic Pane)
  * Cool new View Container Panes
  * New Prefs: "Use Gradient Filled Panes", Default: TRUE
  - Fixed lots of MDI related bugs
  - Upgraded to Microsoft WTL 7.1 headers.
  * New prefs for Header and Info pane fonts
  * New prefs for header and Info pane colors
    "Item_headertext"
    "Item_headerbackground"
    "Item_infotext"
    "Item_infobackground"
    
    *IMPORTANT* Would all ColorScheme designers please submit their updated
    color schemes to me ASAP.
    
  * Added some new commands:
    /HIDEINFO
    /SHOWINFO
    /TOGGLEINFO
    
    /HIDEHEADER
    /SHOWHEADER
    /TOGGLEHEADER
      
    /HIDEUSERLIST
    /SHOWUSERLIST
    /TOGGLEUSERLIST // actually, this one already existed.
  - Fixed fonts not updating after changing them in prefs
  . There are a whole bunch of things changed in this release, expect things to
    be weird, broken or just fudged.  Use at your own risk
    
    All feedback appreciated

08/December/2003 - PM (0.3.101) - Experimental - for #HydraIRC users only.
  Hydra
  - Removed all traces of Taskbar, Window Hiding and Tabbed MDI's
  
08/December/2003 - PM (0.3.100) - Experimental - for #HydraIRC users only.
  Hydra
  + Added missing call to CMD_Clear(), now /clear actually works ;)

07/December/2003 - PM (0.3.099) - Experimental - for #HydraIRC users only.
  Hydra
  + Crash for for editing the name of "Favorites" in the Favorites
    dialogs (thanks dev\null)

07/December/2003 - PM (0.3.098) - Release
  Hydra
  + Fixed minor issue with scroll-lock stuff new items could appear just
    before the buffered items were shown in busy output windows.
  - Switched back to using EN_LINK messages, code still better than it was
    before the scroll-lock stuff

07/December/2003 - PM (0.3.097) - Experimental - for #HydraIRC users only.
  Hydra
  * Added ability to automatically scroll-lock a window while selecting
    text with the mouse.
    -> this is a major usability fix for busy channels!
    
    TODO: re-jig the auto scroll-to-end stuff to use the same scroll-lock
    features.
    
    Developer note:  this change also changed the overriding for
    EN_MSGFILTER and link double-click handling, resulting in cleaner code.

06/December/2003 - PM (0.3.096) - Experimental - for #HydraIRC users only.
  * Added new alias for /action (same as /me) (Lacan)
  * Added new command, /clear [ALL]
    This command can be used to clear the current window buffer or,
    when used with the ALL parameter, all window buffers.
  * Tweaked user identity dialog (due to font size differences under
    Windows 98/ME)
    Also changed order of Ident OS and Ident Username boxes round.
  - Updated the todo list which is still growing daily, if there are any
    developers out there reading this then please have a look through
    the list find a small isolated thing to do!

05/December/2003 - PM (0.3.095) - Experimental - for #HydraIRC users only.
  Hydra
  + Small fix for rejoining channels that are in your OnLoggedIn command
    profiles caused by reordering OnLoggedIn and auto-rejoin in 094
  + Fixed Favorites dialog resize issues.
  
04/December/2003 - PM (0.3.094) - Experimental - for #HydraIRC users only.
  Hydra
  * Added a "/POSITIONWINDOW <X> <Y> <WIDTH> <HEIGHT>" command
    Note: no checks are done to see if the window is still visible after this
    command is issued so take care when specifiying values, though you should
    be able to scroll around in the MDI area.
    Minimum Width and Hight values are both set to 50.
    
    Tip: use this in your OnStartup, OnLoggedIn and OnJoin command profiles to
    move keep your windows where you want them  
      
  * Changed default of "Max Clipboard Paste Lines" to 10 instead of 100.
    (methangas)  
  * Added for per-window prefs to remember such things as colors, size and
    position.
    
    This allows you to set a colorscheme on a per-window basis!
    
    Very handy for FServe channels and for easy distinguishing of windows
    from one another.   
    
  * Added /SET WINDOWCOLORSCHEME=<colorscheme name>
  
    This command updates the color scheme used for the window until the prefs
    are next changed.  It's good for setting a window's color schemes prior
    to enabling window preferences for a given MDI window. (see below)
    
  * Added /SET WINDOWPREFS=[TRUE|FALSE]
   
    This command enables or disables the remembering prefs for a given MDI
    window.
    
    When this command is issued HydraIRC will remember the colors for the
    MDI window the command was used in.  The window's color scheme is saved
    in the user profile (profile_default.xml) in the <windowprefs> section.
    
    Currently you can only enable and disable window prefs, you can't change
    the prefs once they're remembered (without editing the profile when HydraIRC
    is closed, that is..).  So to remember new prefs/colors for a window you
    must disable window prefs for the window, change the colors and then
    re-enable window prefs.
         
  * Identd: Added server addresses and tidied up output window messages  
  + Connect dialog now shows networks without any servers (e.g. just after
    you've added your own new network.)
  + Selective flood control was totally broken
  * Added new pref: "Flood Control Items Per Second" default: 2
    When flood control is active, HydraIRC will send this many commands
    to the server per second until flood control is deactivated.
    Most servers can cope with this, but you may have to set it back to
    1 if you get disconnected while pasting a few lines from the clipboard
    to a channel or query window.
  + Added a dirty status filter to query windows
  + Fixed the swapped tile vert/horizontal function mismatch on the toolbar
  * OnLoggedIn command profile is now run before we try and rejoin any
    channels, this is so you can register/auth with bots, useful for channels
    that require registered nicks before joining.  UNTESTED. (Tonberry)
    
  * Other misc changes
    
22/October/2003 - PM (0.3.093) - Experimental - for #HydraIRC users only.
  Hydra
  * Added a new alias to the Alias plugin: "umode=mode $! $1-" (raggie)
  * Fixed slap to accept strings with spaces in (gliptic)
  
15/October/2003 - PM (0.3.092) - Experimental - for #HydraIRC users only.
  Hydra
  + Fixed a minor display issue when sending files with very long names.  
  + Fixed a bug with % chars in /ctcp commands. (hdaalz)
    (e.g. "/ctcp yourself dcc send %n 0 0 0" would cause a crash)
  + When creating a new server, the priority is correctly set to Normal
    instead of Disabled.
  * Adjusted the "Add To Favorites" dialog so that you can create new 
    folders and edit the names of existing folders, now you can place
    your new favorite in the right place!
    Additionally, I tweaked it so that the "New Folder" tree view item is
    set to edit mode when it's created in both the "Add To Favorites"
    and "organize Favorites" dialogs.
  * made the layout of "Organize Favorites" better, you can see more of
    the tree now.
  * Added support for users sending you files with spaces in the filenames
    (as long as the filename had quotes round it)
    TODO: use regexps for extracting all CTCP parameters   

07/October/2003 - PM (0.3.091) - Experimental - for #HydraIRC users only.
  Hydra
  * Tweaked the ATLRegExp stuff so that a workaround breaks less regexp's
    ("®" and "¯" caused an unmodified ATLRegExp class to crash) 
    NOTE: this change might cause instability depending on the content
    of some user's channel output.

06/October/2003 - PM (0.3.091)
  Hydra
  * Fixed a %d insead of a %s in one of the random slaps (MSBob) 
  
27/September/2003 - PM (0.3.090) - Release
  Hydra
  * Added random slaps to /SLAP <nick>    
    See if you can find them all ;)    
  * Actions in channel windows now cause the channel to be marked as dirty in
    the server list. (Dedbob)
  * You can now enable/disable the MDI tabs at run-time via the View menu.
  * Fixed a crash bug with ignore when changing ignore options via the
    /ignore command
  * All /IGNORE settings are now written to your profile, (profile_default.xml)
    and are loaded up again when starting HydraIRC.
  * Prefs are now saved when the app exits.  (Before, they were only saved when
    you edited prefs, favorites or the server list).
  * Added two new prefs, "Audio Mute" and "Remember Audio Mute", audio mute can
    be toggled on and off as usual, but when HydraIRC starts up the Audio Mute
    setting is set to the default (OFF) if Remember Audio Mute is FALSE.
    Hopefully this arrangement will please those that like the setting to be
    persistant and those (like me) that don't ;)
  - Reverted nick-in-user after first connect that was added in 0.3.087, it was
    broken.
    
27/August/2003 - PM (0.3.087) - Experimental - for #HydraIRC users only.
  Hydra
  * When nick is in-use after first connect to server HydraIRC will automatically
    try to attempt the first nick in your identities list of nicks
  * Added Clipboard sub-menu to userlist context menu, you can now copy
    the nick or the user's hostmask (if known) to the clipboard.
  * Added /CYCLE and a "Cycle" channel window context menu item, this allows
    you to part and join the channel *without* closing the channel window!

22/August/2003 - PM (0.3.086) - Experimental - for #HydraIRC users only.
  Hydra
  * Fixed the problem of the undo buffer being erased if you press cursor down
    one to many times after having typed in some text into a text input view
    window.
    
    Additionally, this lets you type in lines and then you can use cursor
    up/down and type in more lines, press return then using cursor up/down
    you can go back to the line you were typing and either continue typing
    or just press return.
    
    This is great if you're issuing complex commands and chatting at the same
    time!!!
        
    Note: This was a bitch to fix/do and i've actually reworked the history
    buffer so that it never loses a line you type in and never looses it's
    place in the list of history items, additionally this is all done with
    less memory being used and one less class member,  though I'm sure you all
    couldn't care less about that ;) (zann)    
  + When a buddy group is removed, it's associated buddy group notifications
    are also removed. (IgGn0red)  
  + The function calls for the toolbar buttons Tile Horizontally and Tile
    Vertically were the wrong way round - one did the other (AZure)
  . Changed default of "Server List Click Again To Hide" to "False", it
    confused too many simple people's little brains.    
  * Added the following aliases to the alias plugin (alias.dll)
      
      "operwall=OPERWALL :$1-"
      "wallop=WALLOP :$1-"
      
  * Added support for receiving wallops
   
    Additionally, the event log window logs all wallops, to see them make sure
    "Others" is not filtered from the event log context window.
    
    Wallops appear as "*<from>* <message>" in the server window, in a different
    shade to the normal server notices.
    
    The themes and color scheme files that I created have been updated
    
    Note: The authors of the other supplied color schemes need to update their
    color schemes and submit new versions to me, with the relavent lines for
    "item_wallops" included.  

22/August/2003 - PM (0.3.085) - Experimental - for #HydraIRC users only.
  Hydra
  - Changed the default size of the prefs window, now it's bigger!
  * New pref: Log File Viewer, default "NOTEPAD.EXE"
  * Added "View Log File" to the context menus for all channel, server, query
    and dcc chat windows
  * Added context menus to DCC Chat and Query windows
  + Fixed the connect dialog resizing issues (Zann)
  - Merged the "Option" and "Tools" menus into a single menu called "Options"
    The tick boxes next to the items update to reflect the status of the
    currently active MDI window.
  * Added new pref: "Forward Server Messages", default: TRUE (sholom)
    Note: this change might break some responces, let me know if you find
    issues.
    also, disabling this will cause server replies for unknown / commands
    NOT to appear in the current window and ONLY in the server window.
  - Changed "Restrict Queries To Query Windows" so that if it's enabled and
    a query window is present then outgoing and incoming queries are NOT
    echoed to the server (sholom)
  + Added missing shortcut keys to the view menu (hdaalz)
  * Added a couple of extra links on the Help menu - for changes.txt and the
    wiki

18/August/2003 - PM (0.3.085)
  Hydra
  - Tweaked the forwarding of server notices and CTCP messages, this should
    fix the problem of CTCP replies being shown before the request.
    Please let me know if you discover any issues with this.
  * Added more messages to the event logs regarding DCC file transfers

17/August/2003 - PM (0.3.085)
  Hydra
  * Added reporting to the "/set theme=" command to the active window and to the
    output docking window.

17/August/2003 - PM (0.3.084) - Release
  Hydra
  * Added filtering capabilities to the Event Log window, right click on
    the event log window and use the context menu that appears and select
    from the following new options:
    
    Show All
    Show None
    
    Show Notices
    Show Messages
    Show Highlights
    Show Connects (and logins)
    Show Disconnects
    Show Transfers (send/receive/fail/complete)
    Show CTCPs (ping/version/time/sound)
    Show Others (kicked/unclassified messages)
    
    Hopefully these new options will shut imajes up and he might get round to
    setting up the CVS/SVN server that the project has been needing for quite
    some time now...    

30/July/2003 - PM (0.3.083) - Experimental - for #HydraIRC users only.
  Hydra
  + Added a missing check of "Cross Network Forwarding" when forwarding server
    notice messages, this will help with seeing server messages for a
    re-connecting server when the active window is in a channel window on a
    different network.

18/July/2003 - PM (0.3.082) - Release
  Hydra
  * Added "/DCCCHAT <nick>" command for instigating DCC Chat connections.

    New prefs: "DCC Chat First Port" and "DCC Chat Last Port"
    
    HydraIRC now has the ability to do DCC file send and receiving and DCC chat
    instigations and receiving!!  (and about bloody time i hear you all say ;)    
    
  * Added some new prefs to the Integers section on the "Misc" prefs dialog.
    "DCC Chat Connect Timeout" - default 2 mins
    "Server Connect Timeout" - default 2 mins
    
    The latter also will come into effect if the connection is made to the server
    but you've not logged in after the timeout timer expires.
    
    (e.g.  socket connected but irc server is dead..)
    
  * Added /DCC command:
  
    /DCC SEND <nick> [<filename>]
    /DCC CHAT <nick>
    
    This is in addition to /DCCSEND and /DCCCHAT which I will probably remove
    (or at least turn into simple aliases...)
    
  * Added some new icons by srobot to the app.  Thanks srobot!

15/July/2003 - PM (0.3.081) - Experimental - for #HydraIRC users only.
  Hydra
  + Fixed adding and removing an ignore of a blank nick (happened when you put
    a space between the + and the nick,  e.g. "/ignore + nick" should be
    "/ignore +nick"
  * Added new pref "Auto Maximize" default: FALSE
    This maximizes windows when you close, hide or open new windows.
    Note: "Auto Tile" overrides it.
    FIXME: This is a very early implementation and might not work right due to
    other issues involving the shitty MDI interface which doen't work right
    anyway.
    If you have problems with this feature then please report them in the
    forums.
  * Added command profile "<servername>_OnLoggedIn"- in addtion to the command
    profile <networkname>_OnLoggedIn (TTimo).    
  * Added a bit of code to allow you to retry a DCC receive that
    failed to connect to the user offering the file.  e.g.  they fixed their
    firewall quickly and the transfer hadn't timed out yet.  Select one or
    more transfers to retry and press "R" in the transfers docking window.
    Note: you can also re-offer the file when you're sending the file to someone
    and the transfer timed out.  Saves you having to find the file you wanted to
    send again...
    Note: This is pretty much untested and will probably break, but go ahead
    and try it and report back!
  . Sorry there's not been many updates recently,  I've been busy with work
    and my shiny new modded x-box. ;)
    
30/June/2003 - PM (0.3.080)
  Hydra
  + Problem with loading server list that was distributed with HydraIRC 0.3.079  
   
28/June/2003 - PM (0.3.079)
  Hydra
  . Updated master server list, post on forums if there's still more large
    networks that are not present.
  . Updated some color schemes. (Anders, darkjedi, headharve, imajes, crex)
  * Added "Add Server" and "Add Network" to the Connect dialog.
  * Added a "Add Network" dialog
    Note: as always, networks can be defined without having servers
    networks that don't have any servers don't show up in the connection
    dialog until you add a server to that network using "Add Server".
  * Added more options (well, one, "Delete") to the context menu of server items
    in the connect dialog's server tree. 
    Note:  you can't delete servers that appear in HydraIRC.xml, but you can
    DISABLE them (by editing them)  However, don't do this yet as it has no
    effect ;)  (See below)    
  + Finished the "Add/Edit Server" dialog.
  * If you Edit a server in the server list it will be saved in your profile
    and will override ones in HydraIRC.xml (This is why you can only disable
    and not delete servers from HydraIRC.xml
  * Networks and Servers that you add are now saved in your profile.
    This means that you can maintain your own list of networks and servers
    in addition to the one that I supply with the client.
    (Basically, <network> and <server> nodes in profile_default.xml override
    the ones found in hydrairc.xml)
    
    Note: the server Priority functionality is currently unused for now
    leave the Priority setting as "Normal"
    
    TODO: The idea for the future is to be able to use an irc uri like this:
    
    irc://efnet/#hydrairc,isnetwork
        
    This will then connect to the first server in HydraIRC's server list with
    the highest priority for the network "efnet"
    
    Additonally, server cycling will use the priority settings so that we can jump
    to the next server with the same priority if we get disconnected and can't
    reconnect to the server we were using.
    
    additonally, /server and /newserver will support irc uri's in the future
    
    The ",isnetwork" part of that uri is a HydraIRC specific addition to the
    irc uri protocol definition: it means the text between the 2nd / and the
    3rd / characters is a named network, the irc client will match this against
    it's list of networks and select a user's preferred server or a random server
    for that network or a server that is geographically local to the user.
    
    Hopefully this definition will spread to common use because specifying a server
    in an irc uri is often pointless on large networks as users can't always connect
    to the server that the webpage maintainer decides to use.

    TODO: the default supplied server and network lists will be moved to a seperate
    file in the future, this will not effect loading/saving of your custom servers.
    

25/June/2003 - PM (0.3.079)
  Hydra
  * Fixed a possible crash bug with clicking irc:// url's
  
23/June/2003 - PM (0.3.079)
  Hydra
  * Added the hydrairc.com url's into the program now that the hydrairc.com
    domain is live.
    

20/June/2003 - PM (0.3.078)
  Hydra
  + Fixed the shift on startup so that it still parses the command
    line (and thus still process IRC:// links..) (Fiendish)
  * Added the command /SET VERBOSELEVEL=<value>
  
    Where value is:
    
    0 - don't filter anything
    1 - filter join, part
    2 - filter join, part, quit
    3 - filter join, part, quit, mode, kick
    
    The command is only valid for channel windows.
    
    This facility is very handy for busy channels.
    
    Also added this to the context menus of channel windows
    
    Note: this does not effect the log output!!  HydraIRC will
    still log joins, parts, etc regardless of this setting.
    
  * Added ignore facility to the core, currently it supports 4 methods
    of ignoring users:
    
    1) ignore <nick> everywhere
    2) ignore <nick> everywhere on <network>
    3) ignore <nick> on <channel list>
    3) ignore <nick> on <channel list> on <network>
    
    Ignore support ignoring of CTCP, Privmsg, Channel Text and Notices.
    
    When a user is ignored, you will see some output in the "output" window
    (so you can see if they've become less annoying ;)           
    
    now i just have to add the loading/saving/prefs dialog page
    *sigh*  a coder's life is a hard one.
    
  * Added /ignore command, syntax is:
  
    /ignore +<nick> [everywhere] [disabled] [thisnetwork] [<channel> [...]]
    /ignore -<nick>
    /ignore <nick> +|-<channel> [enable] [disable] [usechannellist] [ignorechannellist]
    
    thisnetwork means the current network of the window that the /ignore command
    was entered into.
    
    disabled allows you to create ignore prefs that are not active
    
    e.g.
    
    /ignore +stoopidnoob                      - ignore stoopidnoob on all networks
    
    /ignore +fool thisnetwork                 - ignore fool on this network
    
    /ignore +scriptkid thisnetwork #warezchan - ignore scriptkid in #warezchan on this network
    
    /ignore +bot #this #that                  - ignore bot in channels #this and #that on all networks
    
    /ignore -someop                           - stop ignoring someop
    
    /ignore noob -#help                       - stop ignoring noob in #help
    
    /ignore niceguy disable                   - disables ignoring of niceguy
                                                can be re-enabled later

    /ignore niceguy enable                    - enables ignoring of niceguy
                                                (must have previously been ignoring him at some point)
    
    /ignore idiot +#help                      - add #help to the list of channels to ignore idiot in

    /ignore idiot ignorechannellist           - ignore the list of channels, and ignore idiot everywhere
    
    /ignore idiot usechannellist              - use the list of channels for the user idiot again

    
    ignore lists are NOT currently saved to your profile (or anywhere else..)
    
    you can also type in "/ignore" by itself to get a list of currently ignored users and the settings
    
    Ok, so you all happy now???
       
    
  * Holding shift when logging into servers or joining channels disables running
    of any OnLoggedIn or OnJoin command profiles.
    
    
    
17/June/2003 - PM (0.3.077)
  Hydra
  * srobot created some new icons, and there was a poll in the forum
    and it seemed most people (including myself) preferred them.
    
    I've added them into HydraIRC after much fidling and tweaking,
    unfortunately I didn't have the source to the icons, just a
    JPEG image of them.  If srobot's reading this, then PLEASE
    send me the source files for the icons!
    
    I added/tweaked a few, based on his designs, the ones i created
    were the "transfer completed", "channel monitor" and "server monitor".
 
    Ok, so I think I'm pretty happy with this style of icons, much
    nicer than the ones we had before. 
    
    THANK YOU SROBOT!

  * Added new icons to the transfers window, it's even cooler now!
  + Toolbar icon state for Event Log was not having correctly, fixed!
  * Pressing shift on startup causes the default server window not to
    appear (in addition to not running the OnStartup command profile)
    (Fiendish/Hydra)
  * Tweaked the output from the CTCP/Version command
    

17/June/2003 - PM (0.3.076)
  Hydra
  + Possible crash bug fix (imajes)
  + Updated imajes' color scheme file
  - Removed all references to sf.net and sourceforge.net
  * Changed some url's onthe Help menu to use the new temporary website
  * If you press shift when starting HydraIRC, the OnStartup command profile
    will not be run.
  * You can only run once instance of HydraIRC (for now, this may be a pref at a later date..)
  * Installs a URL handler for HydraIRC,  currently it overrides any existing URL handler
    that may already be installed.
    
    Now you can double click IRC URLs!
  + Fixed a missing call to Validate() in the EditNotification dialog when creating new
    highlights.
  + Fixed loading of blank string prefs items (zann), now it's possible to disable the
    nick complete suffix by setting it to a blank string.
  + Fixed keys being intercepted incorrectly (e.g. ctrl+alt+1 worked for ctrl+1 when it
    shouldn't have) (zann)
  - Fixed a bug that I introduced when I tiedied up the plugin code..
  * Other stuff too...
    
16/June/2003 - PM (0.3.075)
  Hydra
  + Oops, the new dcc code accidently made it so you couldn't SEND or RECEIVE the same
    file to/from mroe than one person at a time.  What i actually wanted was the ability to ignore
    multiple receives to the same local file.  Fixed!

16/June/2003 - PM (0.3.074)
  Hydra
  + Fixed a crash bug which happened when people were kicked from a channel that had a
    quote character (') in it. (e.g "#test'")  Thanks to FRUCHT for the information leading to the bug
    being fixed.
    
16/June/2003 - PM (0.3.073)
  Hydra
  * Added pref: "Rejoin Channel On Kick", default: TRUE (kesselhau)
    If you're kicked from a channel, HydraIRC automatically rejoins the channel
    you can use this option to disable this behaviour
    TODO: Add a "Rejoin Channel Delay" pref.
  * Server responses are that are forwarded to the active window are prefixed
    with the servername if the active window is not on the same server as the
    server sending the response
    
15/June/2003 - PM (0.3.073)
  Hydra
  * Wrote the DCC Send resuming code, HydraIRC can now resume files
    when sending AND receiving.
  + Tweaked the DCC sending code a bit, not much better though, still needs looking at.
  + Multiple Simultaneous DCC file receives with the same local filename (and path)
    are now handled correctly. (i.e.  the first one is accepted, and any more
    are ignored..)

09/June/2003 - PM (0.3.072c)
  Hydra
  + Added in two color schemes by HeadHunter:  
    
    /set colorscheme=Black-Velvet
    /set colorscheme=Rainy-Day
    
    They're stored in "colorschemes/HeadHarve - Collection 1.xml"

  Zann
  + More changes to the installer:

    Revision 0.4
     - Updated to NSIS v2.0b4 CVS
     - Directory page is shown before the options page
     - Enables/disables Use Previous Settings option depending on $INSTDIR


04/June/2003 - AM (0.3.072)
  Hydra
  + Added the missing code for the pref: ForwardCTCPs.
    If disabled you'll only see CTCP requests in the server windows
  * Added an alias for /away: "away=AWAY :$1-"    

  Zann
  + Installer changes:
  
    Revision 0.3
     - When installing in the same dir, deletes all files from ..\plugins\ folder
     - When uninstalling, prompts user to acknowledge that everything will be deleted
     - Changed branding text - couldn't see the problem though :)
     - When doing "light" uninstall, user can still use add/remove programs to uninstall the files
    
    Revision 0.2
     - Updated to NSIS v2.0b3
     - Remembers previously used installation directory
     - Remembers what optional components were previously checked
     - When preserving settings, only two *.xml files are fiddled with
     - Keeps both new and old xml files available, but makes either active depending on user's choice
     - Quick Launch shortcut option
     - Uninstaller: a choice of complete removal, to keep files and folders but remove all shortcuts and registry settings, or to cancel
  
04/June/2003 - AM (0.3.071)
  Hydra
  + Changed the default nick suffix from ": " to "," because you can't cycle
    through nicks at the start of the line as easily when a space is present at
    the end of the nick suffix.
    TODO: The nick completer/cycler itself needs to be aware of the nick
    suffix too..
  + Changed pisto's horrible default orange own text to #000080 (dark blue)

04/June/2003 - AM (0.3.070)
  Hydra / Pistos
  + Added the functionality of most of pistos's patch to the core, alot of
    the code had to be rewritten, but should work the same.
    
    Pistos's new features include:
    
    1) Configurable own-text color
    2) own-text does not cause server list to become active (i think)
    3) you can configure simple channel bookmarks using the names
       of channels, see the tools/options/misc/strings.
       Pressing ctrl+1-9 changes the active window to the bookmarked
       channel.
       TODO: fix this so it can differenciate between channels with the
       same name of different networks.
    4) pressing PGUP/PGDN and ctrl+PGUP/PGDN pages up/down and goes to top
       bottom of the channel/server/query/dcc window output respectively.
    5) a default suffix can be appended to auto completed nicks when the nick
       appears at the beginning of the line.
       
    Side effects that pistos missed:
    
    Small speedup in the highlighting, one less string compare because
    we don't need to check for highlighting on stuff we say and this
    is easier now we have BIC_OWNTEXT
   
  Zann
  * Installer script updated, details later..

26/May/2003 - AM (0.3.070)
  Hydra
  + Added a context menus to the eventlog window (woooo!)

25/May/2003 - AM (0.3.069)
  Hydra
  + Fixed the crash when double clicking eventlog items
    this was caused by me accidently deleting the wrong line of code in
    CEventLogView::::AddItem() - oops!
  * You can now double click various items in the event log to show the
    originating source,  e.g.  
    
    Double clicking a server connect log entry shows the server window
    Double clicking a private message log entry shows a query window
    Double clicking a highlight log entry shows the channel window
    
  * Removed the Notice Log and Private message log icons from the toolbar
  * Added an icon for the new Event Log docking window to the toolbar
    Thanks to zann for supplying a suitable icon.

24/May/2003 - AM (0.3.068)
  Hydra
  * Added a l33t new Event Log docking window, eventually this
    will replace the Notice and Private message log docking windows.
    
    HydraIRC logs information about certain events (such as private
    messages, notices, dcc transfers, server connections, highlights, etc)
    to the EventLog window.
    
    This is great if you go away for a cup of coffee, come back and what to
    see what you missed.
  * Added a checkbox to the Edit Notification dialog for "Log In Event Log"
    defaults to ON for new events.
    
    NOTE: don't forget to enable this option for your existing highlight
    events as appropriate
    
  + Disabled forwarding for the following server replies:
  
    RPL_PROTOCTL
    RPL_LUSEROP
    RPL_LUSERCHANNELS
    RPL_LOCALUSERS
    RPL_GLOBALUSERS
    
    If there's any more, let me know.
      
  + Fixed the resizing dialog issue for the Edit Notification dialog.
  . A few other misc changes...
    
23/May/2003 - PM (0.3.068)
  Hydra
  . Updated docs.htm (see the HydraIRC installation folder)
  * Added more items to the Help menu
  * Removed the old code from Default.dll (Plugin_Default)
    and moved the old aliases to the new Alias.dll plugin.
    Removed old alias code from HydraIRC's core to the
    new Alias.dll plugin.

    The list of aliases is now:
    
    "op=mode $# +ooo $1 $2 $3",
    "dop=mode $# -ooo $1 $2 $3",
    "voice=mode $# +vvv $1 $2 $3",
    "j=join $1-",
    "qj=quietjoin $1-",
    "q=query $1-",
    "disconnect=quit $1-",
    "leave=part $1-",
    "quote=raw $1-",
    "nserver=newserver $1-",
    "exitnow=exit -now",
    "xget=ctcp $1 xdcc send $2-",
    "xlist=ctcp $1 xdcc list $2-",
    "ns=msg NICKSERV $1-",
    "ms=msg MEMOSERV $1-",
    "cs=msg CHANSERV $1-",
    "dalns=msg nickserv@services.dal.net $1-",
    "auth=msg NICKSERV identify $1-",
    "dalauth=msg nickserv@services.dal.net identify $1-",
    "qauth=msg q@cserve.quakenet.org auth $1-",
    "notice=NOTICE $1 :$2-"
  

22/May/2003 - PM (0.3.067)
  Hydra
  . Lots of changes to the source code layout
  * Created an Alias.dll plugin which implements a basic alias processor.
    Aliases are defined similar to those in mirc and other clients:

    Escape Character: $
    Max Parameters: 3
    Item                       Definition           Example
    Channel Name               <escchar>#           $#
    Current Nick               <escchar>!           $!
    Param                      <escchar><paramnum>  $2
    Param and Remaining Params <escchar><paramnum>- $1-
    Additonal Params           <escchar>-           $-

    If the alias /test was defined like this:
    
    t=test $1 '$1-' $2 '$2-' $3 '$3-' '$#' '$!' '$-'
    
    and the user entered:
    
    /t a b "c d" e f
    
    the output would be:
    
    /test a 'a b "c d" e f' b 'b "c d" e f' "c d" '"c d" e f' '#HydraIRC' 'Hydra' 'e f'
    
    As you can see, it copes with quoted strings as parameters too...
    
    The following test aliases have been added:
    
    op=mode # +ooo $$1 $2 $3
    dop=mode # -ooo $$1 $2 $3
    
    Currently there's nowhere for the user to define new aliases ;)    
    
  * Added support for RPL_WHOISAWAY replies.

  * Unknown commands are now passed directly to the IRC server, so commands
    like /away will work now, as will all ircd specific commands.
    
    If you find a command still doesn't work right, add an alias for the
    offending command like one of these:
    
    command=command :$1-
    command=command $1 :$2-
    
    and that should fix it, some servers require the data for the command
    to appear after a ":" character. This is dependant on the IRC server
    program (IRCD) and can be different between commands.
    
    If you define any useful aliases, please let me know when they are
    or post them on the forums for other to use.    
  * Unknown/unprocessed server responses are forwarded to the current window.    
    This makes using various commands much easier as you'll get to see any
    server supplied error messages.
    Eg.  trying to send a message to a user from a query window after they've disconnected
    now results in the "unknown user/nick" server responce appearing in the query window.    

21/May/2003 - PM (0.3.067)
  Hydra
  * Blacklisted version 5.0.152.0 of riched20.dll as it caused HydraIRC to crash on
    startup for no good reason.  Windows 98 SE ships with this version. 
    Solution: upgrade riched20.dll to a newer version (see www.microsoft.com)
  * Added pref "Forward CTCPs" default: TRUE, see Prefs/Misc/Booleans (Zappa)
    This enables/disables CTCP forwarding to the active window.
  * Added highlighting on actions
  . Started adding the alias processor.    

02/May/2003 - PM (0.3.066)
  Hydra
  + Fixed incorrectly enabled controls on the Edit Notification dialog.
  
  
  . Note, sorry, i changed the flag values for the notification prefs a while ago
    and accidentally offset them causing all the filenames and sound event
    names to be lost so if you've not already done it then you'll need to start
    HydraIRC up and just reconfigure them all again.
    
    I wondered why I was missing people /msg'ing me ;)


25/April/2003 - PM (0.3.065)
  Hydra  
  + Disabled highlight events were not actually disabled.
  * Tweaked the regexp a bit, better whole word only matching now

    If you want to play with regexps grab my little ATLRegExp
    program available from www.Hydras-World.com
        
    HydraIRC currently uses this regexp: ^{((.)* )?}{%s}{( (.)*)?}$

    Where %s is replaced with the configured highlight string regexp part.    
  * Added color pickers to the Edit Notification dialog (and left the Hex
    entry fields there too..)
  * Made it so that when you click a favorite channel or query it hides
    the server window after it connects if it has to connect to the server
    first.

24/April/2003 - AM (0.3.064)
  Hydra  
  + When clicking favorites for servers and channels that are not in
    the serverlist hydrairc will now check to see if you're already
    connected to the server before creating another connection to it
  * Added buddy group highlighting, now you can add highlighting for
    buddies in your buddy groups!  Another HydraIRC first! w00t

  + Fixed crash when re-editing the same notification
    
  * The crash problem with highlighting was in the ATLRegExp parser, in fact
    it's still broken I just commented out the bit that caused the crash.
    what side effects this has I don't know but I can't see any difference
    yet.  I think the effected bit was some unicode thing..
    CAtlRegExp::Match() atlrx.h, around line 639:
    offending line:
    if (pBits[u >> 3] & 1 << (u & 0x7))
    
    u >> 3 was a really big number when *sz == "«"

    i commented out the section around that line and made the if() always evaluate to FALSE
    
    like this:
    
    // FIXME: hydra - if *sz == "«" this crashes!!
    // "input string "> «"
    // regexp: "^{(.)*?!\a?}{hydra}{!\a(.)*?}$"

		// if (pBits[u >> 3] & 1 << (u & 0x7))
    if (0)
    

22/April/2003 - AM (0.3.063)
  Hydra
  * Added  highlighting.
    To add a highlight use the Notifications page from prefs
    add a new notification to "Defaults \ All Networks" select
    "Highlight" from the list and configure accordingly.
    For colors use #xxxxxx (#rrggbb) e.g. #ff0000 = red.
    You must configure at lease one color (you're highlighting!)
    can can use partial regexp strings.
    e.g. "(sexy)|(woohoo)" would match "sexy" or "woohoo"
    Very experimentanlt, more docs need to be written, don't ask me
    anything yet unless your name is "Zann".
    
    
15/April/2003 - AM
  Hydra
  * I've upgraded to VS.Net 2003, all the project files are now vs.net 2k3
    format, no older project files will be maintained.
  - Had to add a few "typename" keywords to the docking window source to get
    it to compile under vs.net 2k3 as vc is now iso c++ standards compliant.
    Initial build seems to work ok, but be wary of new bugs/issues with the 
    docking window code.
  + Color prefs in the Misc prefs page had the RGB values reversed.
  * Changed some keyboard shortcuts to be more in line with other Microsoft
    apps,  ctrl+q = quit, ctrl+n = new window, ctrl+w = close window    
  * Changed /PIMP for non-release (test) versions.

13/April/2003 - AM (0.3.062)
  Hydra
  * Added the message box "You have no configured identities, would
    you like to create one now?" if you start HydraIRC with no configured
    identities.  (will also appear if the prefs failed to load too..)
  + The fonts all default to "Verdana" as Tahoma under Windows 98 does
    not have italics.
  + Removed some default prefs sections from the HydraIRC.xml and
    profile_defaullt.xml files so that app default preferences are used.    
  + Removed the default identity from the app, you *have* to enter one
    yourself now.
  + Only preferences that are different to the defaults are saved.
    (To disable this behaviour, see the top of prefs.cpp in the source.)
  + Added: PREF_bCTCPUseERRMSGReply, default: TRUE (dedbob)
  * Added nick retention code to NICK and PART as well as QUIT messages
    now the /GETNICK command can be used to get your nick back when you
    nick holding bot changes name, quits or parts!
  + fix uptime leading color changes (color changes are now done before
    that space seprator, rather than after it).  Note:  this only effects
    the output on lesser clients.  HydraIRC of course always rendered the
    /uptime command output correctly...
  + fixed connect dialog erasing the profile combo box when clicking
    server names
  + fixed crash when sent the CTCP command "DCC" by itself
    e.g. "/ctcp <yournick> dcc" would cause a crash.
  + removed a mIRC bashing QUIT message.  All the remaining quit messages
    are of a postive or funny nature.
  * Fixed a problem with joining channels that require registered nicks on
    irc.xbetas.com (subsequent /join commands to the same channel were
    ignored)
  * When you send a file via dcc, the last dcc path you used in the dialog
    is remembered until you quit HydraIRC.  When HydraIRC starts again it
    will default the setting to the path specified in the prefs.

  
30/March/2003 - AM (0.3.061)
  Hydra
  * Changed the favorites so that it stores the servername when adding
    channels and query windows to favorites.
    This helps when connecting to a server that is not in the main
    server/network list and then adding a channel or user to the favorites.
    (before it wouldn't know what server to reuse when you used the favorite
    now it will reconnect to the server you were using too!)
    
    Note: the format of the favorites in the xml file has changed.
    for serveritems, search replace "data=" with "servername=" in
    the <favorites> section for all <favorite> items with a propery of
    type="S".
    
    Apologies for the inconvenience..
    
  + Fixed an issue where the port for the servers was incorrectly saved
    in the favorites as "0" instead of the actual port that was used
  * Channels and queries have the network name or servername in the
    default name used when adding them to favorites
  * Servers have the network name in the default name when adding them to
    favorites
  * Tweaked the connect dialog, so that it shows your preferred identity
    in the list first.

20/March/2003 - AM (0.3.060)
  Hydra
  * Added / updated icons (dedbob gave me some updated ones and I tweaked
    them all further)    

20/March/2003 - AM (0.3.059)
  Hydra
  * Added a Server Monitor docking window (select View/Server Monitor)
    The server monitor current logs all output from all monitored servers into
    a single window (the server monitor window).
  * Added the following Server Monitor related prefs:
    Misc/"Monitor Servers By Default"  Default: TRUE
    Misc/"Server Monitor Timestamps"   Default: TRUE
    Fonts/"Server Monitor"             Default: Tahoma, 8    
  * Added a "Fonts" section to the "Misc" page in prefs, it uses the same
    syntax as the /set font/<fontname>=<settings> command,  search below...
    Obviously, font STYLES are ignored for all OUTPUT windows. (e.g. servers,
    channels, query,dcc chats,...)      
    TODO: eventually, i'll make it pop up a font request dialog.
    and there will be a seperate Fonts page anyway...
  * Added a "Colors" section to the "Misc" page in prefs
    TODO: eventually, i'll make it pop up a color picker dialog.
    and there will be a seperate Colors page anyway...
  * Added ctrl+i keyboard handling for italics. (in addition for ctrl+n)
    now ctrl+b, i and u are just like in your favorite word processors....
  * DCC Windows are reused if a user with the same nick requests a chat with you
    again.  e.g. connect to an fserve, disconnect, and connect again.
  * Added new pref: "Default Window Timestamp Format" default: "[%H:%M]"
    This setting is used for all windows that display timestamps.
    It differs from "Default Timestamp Format" which is generally has
    a time AND date, and also that this new setting has enclosing []'s.
    see the strftime documentation (google) for details on the % values to use.
  * Added to new options to /TIMESTAMPS command
    /TIMESTAMPS SET=<TimeStampFormat>
    and
    /TIMESTAMPS USEDEFAULTFORMAT
    
    e.g.
    
    "/TIMESTAMPS SET=[%Y-%m-%d %H:%M:%S]"
    
    The timestamps command operates on a PER-WINDOW basis so in your 
    <network>_<channel>_OnJoin or <Network>_OnLoggedIn command profiles,
    you could use the /timestamps command to set different timestamps
    for different channel and server windows respectively.
    
    Note, you can't include colors for your timestamp string (yet) (TODO)
    
  * Tweaked the defaults and color schemes for "Light on dark" and "Dark on Light"
  
    
17/March/2003 - AM (0.3.058)
  Hydra
  * item_globaluserlistselectedbackground and
    item_userlistselectedbackground were being use the wrong way round (dedbob)

17/March/2003 - AM (0.3.057)
  Hydra
  . Note: new version numbering comes into effect.
    Old: a.bcd New: a.b.0cd
    This gives me Major, Minor and Build numbers and the average computer
    noobie can see that we're only on v0.3 which is pretty low, so they won't
    expect much yet.
    
  + Better halfop user list handling (e.g.  and op gives himself +h, 
    then -h and then -o)
    Note, they irc protocol is a bit gay, as when you join a channel you can't
    tell if a user is an OP *AND* a HALFOP (as the name list just has an
    @<Nick>, not and @%<Nick>)  HydraIRC will do the best as it can to put
    the user into the right group.
  + Fixed server list always showing query windows that were created hidden
    as having recent text.
  . Changed default of "Strict Channel Characters" to false, sick of people asking
    me why they can't join "###.|stupidchannelname&^#%$*#"
  + Added UserIdentity::GetNick(x) wrapper for UserItentity::m_NickList so that
    we can always cope with having no nicks in a given identity.
  + Fixed whois output problem in non-server windows if the user was a halfop
    in a channel. (i.e.  we handle the %'s properly)  Also, in the course of
    fixing this I made a few other things more robust.
  + Updated the supplied profile_default.xml file with the new defaults.
  * renamed "lastusedidentityname" to "preferredidentityname" property name
    when loading in the server list.   
    To use a particular identity for a particular network just create an identity
    and specify that as the value for "preferredidentityname" in the HydraIRC.xml
    <networklist> section for your network.  e.g.:
    
 		<network name="QuakeNet" id="10271" preferredidentityname="clan-xpd"/>

  * Added pref: "Default Identity", no default.  
    Added button on the identities dialog in prefs so you can set a default one
    changing setting this is also reflected in Misc\Strings and vice-versa
  + When connecting to a server via "/newserver" we try and use a preferred identity
    for the network if one is specified, otherwise we use a default one - if specified,
    otherwise we use the first one in the list of identities.    
    
		Note: if you want to connect to a server from the OnStartup command profile that
		doesn't use your default identity you have to use /newserver.  
		So use /server for a server that uses your default identity and use /newserver
		for any other servers.
		    
  * ctrl+k or ctrl+m with no valid color numbers after it behaves the same
    as ctrl+o (i.e.  it resets the colors to the line's colors) (biohaz)
  * Window position and layout is stored when prefs are saved via the prefs dialog. (imajes)
  * Added support for "/nick <nick> <args>"
    Some servers, like "irc.irc-hispano.org" for instance, require a password
    for a nick (a way of registering nicks, without a "nickserv" service.
    So you can now do this: "/nick Hydra SecretPassword"
    and it will send: "NICK Hydra:SecretPassword" to the server    
  * Fixed an output issue when doing stuff like this:
    "/msg SomeOne %test%test%test"
    (it was incorrectly shown in the current window, due to being parsed twice
    by sprintf() calls.
  * Added configurable backgrounds of selected items in userlist, global userlist
    and serverlist (biohaz)
    New prefs:
    "Item_userlistselectedbackground"        default: blue
    "Item_globaluserlistselectedbackground"  default: blue
    "Item_serverlistselectedbackground"      defailt: grey
  * Added "/set GUILAYOUT=<guilayoutstyle>"
    This lets you choose between different GUI Layouts
    values 0-5 are valid.
    0 = docking windows are hidden
    1 = default layout
    
    Try the others!!
    
    These styles are also available from the new submenu on the "View" menu.
    
    

    
  

16/March/2003 - AM (0.356)
  Hydra
  * I made it so you can do this:
    /set colors/item_serverlistserverdirtytext=-1
    and this:
    /set colors/item_serverlistserverrecenttext=-1
    
    Thus, when a server window gets new text and is not active, the color
    will not be changed (but the font+style will be changed as usual)
    This means that you can see the color statuses of the server even
    when the server is marked as dirty.

16/March/2003 - PM (0.355)
  Hydra
  * When logged into a server, HydraIRC issues a USERHOST command
    to the irc server and extracts the hostname from the response
    and will use that as your IP address/name when giving your
    details ip address details to another user when dccing
  * Paths from incoming dcc file transfers are stripped
    (untested)
  * Invalid characters from incoming dcc file transfers are stripped
  * Background of recent/dirty items in the server list are not changed
    if the current window is active.
  * Tweaked the defaults for recent/dirty colors, to use the new defaults
    close HydraIRC, then remove your old color settings for all
    "item_serverlist*dirty*" items in your profile_default.xml file and
    then restart HydraIRC
  * Using setting of <= 0 for the following settings disables them:
    "Server List DCCChat Recent Seconds"
    "Server List Server Recent Seconds"
    "Server List Channel Recent Seconds"
    "Server List Query Recent Seconds"
    (untested)     
  . Note: just to clarify:
    Recent = new output appeared recently in a window
      (how recently depends on your prefs)
    Dirty = new output appeared in a window and the window is not active.
    
    Recent takes priority over Dirty when using colors.

14/March/2003 - PM (0.354)
  Hydra
  * Added server list recent AND dirty statuses, lots of new prefs

    When someone says something in a channel, and that channel's
    window is not active, the item for the channel in the server list
    changes color (to red by default)
    
    Now it can change color and then after a particular amount of
    time (see the bottom of the Integers section on the "Misc" page in prefs)
    it can change to another color so that you can see which windows
    have new text in the last, say 15 mins (default), and which ones
    you've not looked at for ages.

27/February/2003 - PM
  Hydra
  * Tweaked the DNS/Host information extractor, it doesn't remember
    blank strings anymore. (dedbob)
  + Prefs are saved after editing a user identity via the Connect
    dialog.
    
26/February/2003 - PM (0.353)
  Hydra
  * When connecting to an IRC server, HydraIRC will look at the welcome
    message and extract the DNS/Host information from the message
    (if present).  
    
    Also, if you do a whois on yourself and the server supports
    the 338 or 378 numeric replies, the host information will be extracted
    from the reply.
        
    The extracted string will then be used for DCC sending, unless you
    set a value for "Router Address" in the prefs, thus giving you the
    option of being able to override the address that HydraIRC uses.    
    (Dedbob)
    
26/February/2003 - AM (0.352)
  Hydra
  + Tweaked the code for the winamp plugin not displaying the image
    on the toolbar (/me bleh's @ microsoft)
  + Fixed the issue with duplicate nicks in the userlist if you connect
    to a server with a long nick that is truncated by the server you
    connect to. (Anders)
  + Fixed the problem with multi-line paste data being added to the
    text input window's history buffer *after* it was freed.
    (dedbob/imajes)
    

25/February/2003 - PM (0.351)
  Hydra
  * Updated readme.txt  <-- NEW USERS PLEASE READ!
  * Updated Docs.htm    <-- NEW USERS PLEASE READ!
  * Updated the profile_default.xml file for noobs
  * Added an option so you can specify your router/firewall's external
    address.
    If you are behind a firewall and your machine is NOT the firewall
    then you should specify the EXTERNAL ip address of your router
    and configure it to forward the usual dcc ports to your machine's
    LAN address.
    
    e.g.
    
    Router:  192.168.0.1, Internet Address: 62.1.23.45
    PC1:     192.168.0.2
    PC2:     192.168.0.3 
    
    On PC1, you would use dcc ports 3072-3171
    On PC2, you would use dcc ports 3172-3271
    
    On the router you configure port forwarding so that it forwards
    incoming tcp packets bound for ports 3072-3171 to 192.168.0.1 (PC1)
    and then configure it to forward incoming tcp packets bound
    for ports 3172-3271 to 192.168.0.2 (PC2)
    
    Then any packets heading to 62.1.23.45:3072-3171 are forwarded to
    PC1 and any packets heading to 62.1.23.45:3172-3271 are forwarded
    to PC2.
    
    In HydraIRC you would need to set the "Router Address" option to 
    "62.1.23.45" (or you can use the FQDN of the Router's external
    interface if you like.  e.g "host1343.location.isp.co.uk")
    
    You can use the options "DCC Send First Port" and "DCC Send Last
    Port" in HydraIRC to configure the port ranges for the PC's
    
    e.g.     
    on PC1 you would set "DCC Send First Port" to "3072" and
    "DCC Send Last Port" to "3171"
    on PC2 you would set "DCC Send First Port" to "3172" and
    "DCC Send Last Port" to "3271"
    
    This is untested (My router's PSU died..)
    (dedbob)
    
    
  * Added an new Plugin API for handing User Command messages
  * Changed the plugin code a bit so that plugins are not loaded
    twice (or even attempted to be initialised twice, which was
    the problem).
    /refreshplugins should now work even without unloading all
    the plugins.
    
  * Added a WinAMP plugin.
    It doesn't do much yet though, currently it just adds a button
    to the toolbar.  If this button is clicked and the active window
    is a channel window then HydraIRC will display a message in the
    channel with the name of the currently playing song.
    
    You can load and unload the plugin using the following commands:
    /unloadplugin WinAMP
    /refreshplugins
    
  * Renamed the old plugin from "Default_plugin" to "Default"
  
  * 
    
  * When a channel is joined, hydraIRC will request the channels modes
    from the server (so the GUI is displayed correctly)
    In 0.336 it would do this regardless of wether a join failed.
    In 0.338 this was removed to speed up channel joins
    Now the code is in the right place ;)    

24/February/2003 - PM
  Hydra
  * Tweaked the nick retention code.

24/February/2003 - AM (0.350)
  Hydra
  * No changes yet...  just pumbing the version due to the new license
    and someone else wanted a private build

22/February/2003 - PM (0.349)
  Hydra
  * I found a rather serious problem in 0.348 (It would die randomly due
    to a bug in my new improved socket code..) so this release is just to
    fix that.  Note that I had been running the client since I released
    0.348 with no crashes until today, so finding it was difficult. 
    (The bug manifested itself in other ways than crashing too..)
  + I broke "/exit -now", fixed, really this time... (imajes)

16/February/2003 - PM (0.348)
  Hydra
  * Woohoo!  Added the "Add to favorites..." dialog box
  * the "Organize Favorites..." dialog now supports dragging and
    dropping of favorite items (bleh, I had to code all the drag and
    drop stuff myself - took ages!)
  * Added the favorites menu (no icons denoting types of favorites yet...)
    You can add channels, servers and queries to favorites, additionally
    if you select a query or a channel from the favorites list, HydraIRC will
    connect to the server on the same network and then open the channel
    or query window.
    When a query window is opened via a favorites, a /whois is also done.
  + Limited clipboard paste to 400 chars maximum (imajes)
  + Fixed crash when a topic was set to an empty string. (imajes)
  + Added a HydraIRC-Uptime to the /uptime command
    (Actually, it's a plugin.dll uptime...)

15/February/2003 - PM (0.347)
  Hydra
  * More work on the favorites dialog,  you can now create groups
    and delete items.

15/February/2003 - AM
  Hydra
  + I broke /exitnow, fixed. (Imajes)

14/February/2003 - AM (0.346)
  Hydra
  - still not fixed random crashing when closing MDI windows...
    I even went back to an old version (0.342) and found it happens
    there too, so it's not due to my recent changes.
  + Added a fix for WM_MOUSEACTIVATE messages.
    (Maybe it'll help double-clicking urls when app is not active...)    
    
13/February/2003 - AM
  Hydra
  + I broke auto tile,  fixed!
  . more work on the random crashing after closing MDI child windows
    (argggghhh!!!!)
    
12/February/2003 - PM (0.345)
  Hydra
  * I cleaned up the exit process for HydraIRC as well as the server cleanup
    process (so that a server kills its channel windows before it kills
    itself and it's all done cleanly with no kludges now)
    However, there was a lot of stuff changed to acheive this, so if you get
    crashes when closing MDI windows then please let me know.
    (Make sure you take a screenshot!!!)
  + Set null terminators after each _vsnprintf() call, this should make
    the code more resiliant when dealing with incorrect data.
  + Fixed a bug in the code for firing broadcast events that removed one or
    more listeners from the list of listeners.    
	+ Added a workaround for the EM_GETTEXTRANGE richedit bug.
	  For those interested, here's a screenshot of the bug in action:
		http://www.zenadsl5874.zen.co.uk/hydrairc/GetTextRangeBug-Annotated.png
		or see the end of the source to the CMsgView::Put() function (around
		line 563)	
	- removed the CommandBarXP stuff, it causes the app to fault when you exit
	  it.  That and it's menus for checkbox items WITH IMAGES is broken
	  (you can't see the checked status).
  . Had a look at the line spacing issue, doing GetParaFormat() returns
    the paragraph formatting, but "PARAFORMAT2.dyLineSpacing" is still
    0 even when the paragraph formatting has changed.  Look like an
    internal richedit problem,  can't do anything about this bug.
  

11/February/2003 - PM
  Hydra
  + Fixed a rather serious crash bug when the preference "Show Channel
    Monitor Messages on Statusbar" was enabled.
    If a very large string buffer (512 bytes or more) was passed to the
    status bar, it would corrupt the stack (because of a windows
    limitation in SB_SETTEXT).
  
  . Working on fixing the crash bug on Chinese Windows 2000 systems...
    Seems to happen when the amount of lines in a scroll buffer (output, 
    channel, etc.) reaches "Max Scroll Buffer Lines".  Something is going
    wrong in EM_GETTEXTRANGE...
  + Fixed lines being incorrectly added to the history buffer before the
    line was actual used when pasting data.
  * Added nice looking context menus!       
  
10/February/2003 - PM
  Hydra
  * Did more work on making the HydraIRC source code "const correct"
    (Google for definition of "const correct")
    *sigh* this takes ages...
  + Fixed a crash bug when using /set to set strings and paths (hdaalz)
  * Added new pref: "Create Folders For DCC Transfers" default: FALSE
    When enabled, a folder will be created in your default download path
    as the name of the person who is sending you a file and the file will
    be placed in it.
    
    e.g.  if someone called Zann sends you a file, and your DCCDownloads
    folder is set to "C:\Program Files\HydraIRC\Downloads" and the file
    he is sending you is "ILoveHotChicks.JPG" then it would be saved as:
    "C:\Program Files\HydraIRC\Downloads\Zann\ILoveHotChicks.JPG"
    
    This helps users (like me) who receive lots of similarly named files
    from lots of people.
    
    Note:  if you try and resume a DCC transfer from another bot than
    you originally downloaded the file from then the new bot will not
    be able to resume the file (it's in a different directory..) and
    so it will start the transfer from the beginning again.
        
    
  * Tweaked the edit user identity dialog (selects the nick that you
    just added and also selects the next nicks when you delete one)    
    deleting and adding nicks)
  * Fixed a problem with the nick list duplicating itself when re-using a
    server window or when editing a user identity from the edit identity
    dialog. (Newa)
        
10/February/2003 - AM (0.344)
  Hydra
  * Rewrote the server socket receiving code to use less memory and cpu time
    in addition to that it also handles lots of large socket buffers correctly
    (e.g. when doing a "/raw list")
    Previously some lines were truncated.
  * Added PARAM1 - PARAM5 theme variables - untested.
  * Added a themable items for RPL_TOPICEND, RPL_LIST and RPL_LISTEND
    default theme and themes file updated.
  + Only known CTCP commands are forwarded to the active window (dedbob)
  + %'s in status bar are converted to /'s to stop crashes
  + Tidy up of some of the plugin.cpp code
  * Added /LIST alias to the plugin (/RAW LIST)
  * Forward /WHOIS output to active window.
  
  . For those that are interested, try adding this to your profile_defualt.xml
    file just before the </HydraIRCProfile> line
    
  <favorites>
    <group name="HydraIRC">
      <favorite name="#HydraIRC" type="C" networkid="10362"/>
      <favorite name="Hydra" type="Q" networkid="10362"/>
    </group>
    <group name="Stuff for noobies!">
      <favorite name="#IRCHelp" type="C" networkid="10362"/>
    </group>
    <favorite name="Zann" type="Q" networkid="10362"/>    
  </favorites>
    
    Then start up HydraIRC and take a look at the favorites dialog.
    
    Note: favorites don't do anything yet, but you can start adding your
    favorite stuff (channels, querys, servers) to favorites and they'll work
    in future.. ;)
    
    Also note, you can edit the names of favorites in the favorites dialog
    itself by clicking on a name, then clicking again (not double click)
    just like you rename files in Explorer.

08/February/2003 - PM (0.343)
  Hydra
  * Added a /getnick command!
  
    /getnick [nick]
  
    e.g.
    
    /getnick MyNick
    
    If HydraIRC sees another user quitting who had the same nick that you
    gave tot he /getnick command, HydraIRC will change your nick to the one
    you specified.
    
    This helps if someone steals your nick, or you need to do a nick change
    to prevent a nick being stolen.
    
    Another handy command to put in your OnLoggedIn scripts...
    
    Note: if you're not in the channel when the user with the nick quits
    then nothing will happen, also if the user parts from all the channels he
    was in (that you were also in) before he quits nothing will happen.
    
  + Fixed a minor bug in the threme creator tool
  + Fixed a minor bug in UnscapeString - now if you enter "aa\0aa"
    the "\0" bit is skipped correctly.
  * Tweaked server forced-join channels code (it now shows the "Now talking
    in #somechannel" message correctly)
  * More work on the Organize favorites dialog, currently favorites can
    be loaded from the profile, shown and saved.
  + CTCP/SLOTS requests are not shown in current window (dedbob)
  + Added a fix so you don't get banned from #foobar2000 (Switch_IR)
    HAHAHAHAHAHAHA    
    Reminder: if people are getting pissed off with the quit messages then
    remember to part the channels you're in before you disconnect, or type
    in /QUIT <my alternate quit message> to disconnect.
    
  + Fixed missing command between two of the random quit messages causing them
    to be appended to each other.
  * I've added a new command /HELP which says that if they can't find what you
    need in the readme.txt or dcots.htm then search changes.txt.
    I'm sick of telling people to read the changes.txt so hopefully I'll not
    have to answer the question "how do i change the fonts" again.
    But I can only live in hope...
    
08/February/2003 - PM (0.342)
  Hydra
  + Fixed DCC Transfer stats not updating for first 10 seconds
    (Missing call to ResetStats())
  + Fixed socket destructor and CloseSocket() closing non-opened sockets
    (Missing initialisation of socket->m_sock!  /me slaps himself)
  + Fixed DCC Transfers closing a non-opened socket.
  + Fixed the DCC Transfers "File write error" when one file completes and
    another transfer was still going.
    (set m_File to NULL instead of -1 after file was closed, null == 0 which
    could still be a valid file handle (i.e.  the other transfer...)
  * Server and network name are now shown in a new column on the transfer
    window.
  * network name is shown in ()'s in the server column of the privmsg and
    notice message logs
  * privmsg and notice log timestamps use the default timestamp format
    as configured in prefs.
  * /DCCSEND can now send multiple files at a time

    /DCCSEND <nick> [file][,[file][...]

    e.g

    /DCCSEND Hydra c:\file1.txt,c:\file2.txt,d:\my documents\some file.bin

    or just use this to get HydraIRC to prompt for a file    

    /DCCSEND Hydra

    TODO: allow multiple select in file dialog.


07/February/2003 - PM (0.341)
  Hydra
  + Fixed nick change in query (at last)

07/February/2003 - PM (0.340)
  Hydra
  + Fixed multiple updating of the docking windows after a prefs update
    and during application initialisation.
  + When socket logging is disabled, less information is shown in the about
    (i.e. it's as it was in 0.338)

06/February/2003 - PM (0.339)
  Hydra
  . Added a non-functional (yet) Favorites Dialog and menu item
  * Added error reporting if a server socket send fails. (imajes)
    There's currently a weird issue where sometimes the server is
    responding with a garbled command, but HydraIRC appears to be sending
    the data intact,  with this reportingin place I should be able to
    determine whether the IRC server or HydraIRC is at fault.
  * Added /QUIETJOIN (and an alias /QJ)  this is the same as join, except
    that the channel's window is not made active if it already exists.
    This makes it ideal for use in the ..._OnLoggedIn command profiles
    instead of /JOIN.
    
    Note: If you don't use QUIETJOIN in your ..._OnLoggedIn command
    profile then all your channel windows will be redisplayed when if
    you get disconnected and reconnected.
  * Outgoing pings and outgoing pong replies are now shown
  * Pings and pongs are now themeable.
  * Added new pref: "Show Pings" defualt: FALSE
  * Added new pref: "Socket Logging" default: FALSE
    note, this will increase CPU usage, especially if enabled at the same time
    as "Console Logging"
    Additionally, your console log file (HydraIRC_Log.txt) will get very
    big very quickly!
  * Added new pref: "Server List Tool Tips", default: TRUE
    Mhh, Some people are just never happy..... (imajes)
    
06/February/2003 - PM (0.338)
  Hydra
  . Updated the README.TXT and the Docs.htm
  
  + Increased size limit of command profiles to 1K (imajes)
  + Fixed theme nick problem (anders/hdaalz)
  * Added a password field to the Connect dialog
  
  . Reminder:  to connect to a server that has a password, you can
    use the following command
    
    /newserver servername:port,password
  + Removed 3 character minimum server name length restriction
  * Added automatic running of onjoin command profiles named
  
    <Network>_<Channel>_OnJoin
    
    EFNet_#HydraIRC_OnJoin
    
    /msg Hydra I love your IRC Client!
    Hello everyone
    /mode -o Rovastar
    

05/February/2003 - PM (Pre-v0.338)
  Hydra
  * Fixed crash bug when using context menus or double clicking a username
    from the global user list after a channel rejoin failed (e.g. after a ban)
    Solution: clear global user list in (IRCChannel::OnJoin()
  - Changed the refreshing of the global user list after a channel join, this
    may/may not break stuff, let me know if you get problems with the GUL after
    a server disconnect and reconnect.
  * Added a random quit message generator and a new quit message
    (And NO, you still can't change the quit messages yet - that will come with
    HydraIRC v1.0 RC1 as I need to pimp the client as much as possible until
    v1.0 is released.)
  + Fixed Logging prefs page resizing issues
  * Command profiles are now loaded and saved to the XML file (in the same
    escaped format as themes)
  + "Max Input History Lines" was not being updated after a prefs change.
  * Due to popular demand I've added a *temporary* solution to the autoconnect
    and autologin stuff that has been requested.
    
    Please read this bit carefully!
    
    HydraIRC will execute a command profile called "OnStartup" when it starts
    also, when HydraIRC connects to a server it tries to match up the server to
    a server in the server list, if it then finds a matching server name it then
    determines the network that the server belongs to.  Once the network has
    been determined HydraIRC will then execute the command profile called
    "<Network>_OnLoggedIn" when you are logged in to the server.        
    
    Example command profiles:
    
    OnStartup
    
    /server irc.quakenet.org
    /newserver irc.efnet.pl
    
    QuakeNet_OnLoggedIn
    
    /join #ctfpickup.euro,#quake3,#bygames
    /qauth MyName MyPassword
    
    EFNet_OnLoggedIn
    
    /join #hydrairc
    /msg #hydrairc I'm a noobie HydraIRC user!
    
    Note: this is a temporary solution, at a later date you will be able to
    assign command profiles to networks and select which networks to connect to
    when the application starts, but this will do for now.
  
  * Added the /exec command, you can use it to execute a command profile.
  
    e.g.  create a command profile called "MyCommandProfile" and then you can
    issue the following command
    
    /exec MyCommandProfile

04/February/2003 - PM
  Hydra
  * Added the GUI for command profiles as well as all the code that goes
    behind it (See class CCommandProfile).
  + Fixed console logging after I broke it a few days ago
  + Fixed crash at startup if console logging enabled (zann)
  + Fixed "Remove Old Privmsg/Notice log items" settings not being set correctly
    when displaying the "Messages" prefs dialog (zann)
  
03/February/2003 - PM
  Hydra
  + Added Logging button tooltip
  + Fixed incorrectly disabled Logging toolbar button when logging was disabled
    by default.
  * Rearranges some of the Misc prefs so that timestamp options appear next
    to each other.  This won't matter much in the future as there will be a
    seperate prefs page for this stuff anyway...
  * When pasting multiple lines of text to the input buffer, each line is
    now remembered in the input history.
  * Default menu item ("Query") of user list context menus is now bold.
  * Double-clicking notice log items creates new query windows.
  * Added a new pref "Max Input History Lines", default: 25, max 250.    
  * Tweaked the "Not Implemented" pages in the prefs dialog to point users
    to the Misc section as lots of people are overlooking that page.
  * Added server-forced channel auto joining (some servers join you to channels
    automatically) (untested - please let me know if you have problems)
  + Fixed a couple of theme issues.
    (Nick changes and connect/disconnects in query windows)
  * HydraIRC now creates a server window at startup (no prefs yet) if you have
    one or more user identities.  The server window doesn't connect to a server.
  * HydraIRC now displays a welcome message in the default server window.
  * When clicking the Connect button, and if the current server is disconnected,
    the connect dialog box will be shown and allows you to select a different
    server and identity.  Handy if you made a typo in the server name or if
    the server you were previously connected to died.
  * IRC:// no longer required in connection dialog.
  + Tweaked some of the plugin code.
  * Added a basic nick retention system to HydraIRC.
    When you get disconnected from an IRC server, HydraIRC remembers the nick
    that you were using,  HydraIRC will then reconnect to the server as usual.
    If HydraIRC then spots a QUIT message from the user with the nick that was
    previously remembered then HydraIRC will automatically change your nick
    back to the old one.
    This helps when you've managed to secure your favorite nick that someone
    else keeps stealing, and also helps if you have queued up a load of files
    via an FServe/XDCC bot.
    
    TODO: later versions may check hostmasks and may also use notification if
    the IRC server supports it, but for now, this does the job.
    
  . Configured a test-build project settings to create .map and .pdb files
    so that I can trace crash reports.
    Also reconfigured the release build a little.
    
    If HydraIRC crashes, take a screenshot (use printscreen, then paste into
    Photoshop, PSP, or MSPaint and save as a PNG file) and create a crash dump
    file and note down the actual address that the crash occured and send it
    to me, along with a note saying what happened just before it occured (or
    how to reproduce, if you know)
    
    CRASH REPORTS WITHOUT THIS INFOMATION ARE ALMOST USELESS TO ME!
    
    Additionally, if you get frequent crashes then enable "Console Logging"
    from the "Misc" page in the preferences dialog and send the file called
    "HydraIRC_log.txt" that appears in your installation directory.  Note:
    this file can get very large very quickly.  It is safe to delete this file
    even when HydraIRC is running.
    
    Also, as I mentioned below, 0.337/0.338 will be less stable than 0.336 due
    to the new theming stuff and the amount of changes that were made to
    the core.
    
27/January/2003 - PM
  Hydra
  * Double clicking a user in a user-list opens a query window
  * Output that appears before the main window is now remembered and then
    displayed when the main window appears.  So that if you fudge up your
    .XML files it will show you the error (if you scroll back a bit..)
  * Better formatting of XML error messages.
  * Upon reconnecting to a server (e.g. server died) HydraIRC now doesn't
    destroy the channel if we had previously joined it sucessfully.
    Instead it posts an error message (e.g.  "Channel Unavailable") to the
    channel window and sets the status accordingly.
    (This could also happen if someone set a channel to +i while you were
    disconnected, etc.)

24/January/2003 - PM
  Hydra
  * Added options/Timestamps to the menus
  * Added Timestamps to server and channel context menus
  * Added loadable color schemes! woohoo!    
    
    To load a color scheme, issue the command:
    
    /set colorscheme=<nameofscheme>

    where <nameofscheme> is the same as the name property of a colorscheme node
    in any colorscheme file in the colorschemes directory.
    
    The colorschemes\Hydra - Basic.xml file currently contains two schemes, 
    these are called "Dark On Light" (the same as the built-in default) and 
    "Light on Dark".
   
    So, if you used the command:
    
    /set colorscheme=Light On Dark
    
    HydraIRC would scan all *.xml files in ColorSchemes\ and will load the
    scheme if it finds it.
    
    NOTE:  Unlike output themes, color schemes are not per-window, when you
    change the color scheme, it is changed for ALL windows.   This will
    change at a later date.
    
    Delete the following files from your HydraIRC\ColorScheme directory
    as they are no long required.
    DarkOnLight-Hydra.xml
    LightOnDark-Hydra.xml
    
  * Changed the filename of the defualt THEME file from:
    themes\default.xml
    to
    themes\Hydra - Basic.xml
    
    So delete the old one (themes\default.xml)

22/January/2003 - PM
  Hydra
  + Fixed /part in query or server window crash.
    The /part is only valid in Query or Channel windows now.
  * Themes can now be loaded from XML files! woohoo!
  
    Themes are stored in Theme Collection files and you can
    store more than one theme per file (XML is great!)
  
    See the included themes\default.xml for an example file.
    
    You can change themes for EACH AND ANY WINDOW whenever you like by using
    the /set command in the window like this:
    
    /set theme=<nameoftheme>
    
    where <nameoftheme> is the same as the name property of a theme node in any
    theme file in the themes directory.
    
    The themes\default.xml file currently contains two themes, these are called
    "Basic" and "Alternate".
    
    So, if you used the command:
    
    /set theme=alternate
    
    HydraIRC would scan all *.xml files in Themes\ and will load the theme if
    it finds it.
    
    HydraIRC has a built-in theme called "Default", HydraIRC will use the build-in theme
    if for any reason it cannot load the theme that was specified.
    
    HydraIRC 0.336 and before used the "Basic" theme, HydraIRC 0.337 used the
    "Alternate" theme by default.
    
    HydraIRC 0.338 now has a new preference that allow you to select the default
    theme for new windows.  See "Misc/Default Theme". in prefs.
    
    This can also be set with the command:
    
    /set misc/default theme=<nameoftheme>
    
    Note that XML requires that <'s >'s and &'s are escaped  along with the
    color/reverse/bold/normal/italic change codes, otherwise the XML file
    cannot be parsed correctly.    
    This is why you can see lots of \003's, \074's and \076's in the themes.
    These are the octal representation of the ASCII values.
    To get a '\' character you have to use '\\' or '\134'.
    
    For a list of ASCII characters and their octal values see here
    http://www.asciitable.com/
    
    The supported style/codes are:
    
    '\002' bold
    '\003' color change '\003<2 digit decimal foreground>[,<2 digit decimal background>]'
    '\004' hi-color change '\004<6 digit hex foreground>[,<6 digit hex background>]'
    '\037' underline
    '\035' italics
    '\026' reverse foreground and background colors.
    '\017' set normal forground and background colors. (per line basis in HydraIRC)
    
    '\006', '\022' and '\023' are stripped from the output
    
    it's probably not a good idea to use linefeeds and carraige returns
    TODO: these will probably be stripped at a later date.
    
    Important Notes for theme designers:
    When a theme is loaded it is kept in memory and reused by all other windows that use
    the same theme.
    When HydraIRC is told to use a particular theme it first looks to see if it is loaded
    and if it is then HydraIRC does NOT reload it from disk.
    SO.  If you want to see changes to a theme you must first make sure no windows are
    using the theme you're trying to load.  This can be done by issuing a
    "/set theme=default" in each open window.
    
    
    
  
17/January/2003 - PM
  Hydra
  + Fixed privmsg notice theme (used to display "DEFAULT", instead of NOTICE)
  + Sendind CTCP request theme ("-> CTCP") item showing current nick not TO nick
  * Added Context menus to the userlist - wooohooo!
  + A users Ident username and hostname are now remembered by channels when a
    user joins a channel or when you do a /whois on a user so that the new BAN
    (and KICK+BAN) options on the new context menus can function correctly.
    (as a hostmask is required..)
    As a side note, the long you stay in a large channel, the more memory will
    be used by the userlist as it starts to remember each persons hostmask
    as they join.  (Of course, it still frees this information when they part
    too..)
  * Added a link to the HydraIRC web site on the About menu.
  . Updated the About box copyright date.

16/January/2003 - AM
  Hydra
  + Fixed a focus issue when Minimze to Systray was used.
  + Channel monitor did not update when clicking a non active window after
    switching back to HydraIRC from another app.
  + CTCP Send displayed your nick, not the user you were sending the
    request to.
  * Added a pref to allow the changing of the font used for the user lists.
    Pref: "User List", defualt: "Tahoma,8"
    you can use the /set command (as always) to change font and color
    prefs without restarting the client.  e.g.
    
    "/set font/user list=Verdana,9"
    
  * Added prefs for stripping colors/styles from different windows.
    Note: HydraIRC still uses the colors defined for the content of
    the output.  e.g.  if item_join is green, then JOIN lines will still
    be in green, but if a user sends "ctrl+k4,5ctrl+btest" to the channel it
    won't appear in red on brown bold text.
    Prefs:
    "Strip Codes From Server Windows"
    "Strip Codes From Channel Windows"
    "Strip Codes From Query Windows"
    "Strip Codes From DCCChat Windows"
    Default: FALSE
  + Renamed the following prefs:
    "Strip Codes From ServerLogs"
    "Strip Codes From ChannelLogs"
    "Strip Codes From QueryLogs"
    "Strip Codes From DCCChatLogs"
    to
    "Strip Codes From Server Logs"
    "Strip Codes From Channel Logs"
    "Strip Codes From Query Logs"
    "Strip Codes From DCCChat Logs"
    If you changed these fromt he default, you will need to reapply your
    changes or edit your profile_defualt.xml file manually before running
    the new version.  

15/January/2003
  Hydra
  * Setting a color pref using the /set command always reported a failure
    even though it actually changed it
  * Using a /set to change a color now forces the windows to be redrawn.
    Using the multiline clipboard stuff, you can now just copy a bunch of
    commands into a window!
    
    /set colors/Item_userlistbackground=#000000
    /set colors/Item_userlistnormaltext=#ffffff
    /set colors/Item_textinputbackground=#000000
    /set colors/Item_textinputnormaltext=#ffffff

    or this
    
    /set colors/Item_userlistbackground=#000000
    /set colors/Item_userlistnormaltext=#ffffff
    /set colors/Item_textinputbackground=#000000
    /set colors/Item_textinputnormaltext=#ffffff  
        

15/January/2003 - AM  (v0.337)
  Hydra
  + HAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAAH - ROFL!
    I accidentally moved the server PRIVMSG socket send command inside the
    query window if() statement when i moved some other stuff there for the
    privmsg theming.
    Anyway, the effect of it was that stuff you typed into channels was not
    actually sent!
    It took me almost a day to figure that out,  I thought people were just away
    or ignoring me!  Lol!

14/January/2002 - AM
  Hydra
  * Added extra controls to the Logging prefs window and fixed (well, set) the
    tab order.
  * Added extra controls to the Messaging prefs window.
  * Tweaked the Prefs window layout and main treeview.
  * Fixed the CTCP stuff, I broke most of it yesterday
  * Themed all the CTCP responses
  * Fixed the CTCP time responder, it was broken still.
  * Query windows are now themed.
  * DCC Windows are also themed.
  * Made sure that all query/notice/ctcp messages were being forwarded correctly
    Note: Theming HydraIRC and moving stuff out of ChildFrm.cpp broke lots of
    stuff.  Someone please play around with /msg, query windows, channel
    windows and the settings on the new Messages page and let me know if you
    find problems.
    I think it's all ok now, but I might have missed something...
  * Right, I've finally fixed the ctrl+f4-to-close-server-with-open-channels
    crash bug once and for all now.  (I hope)
    Suprised no-one other than me ever noticed this...
  - Spent a while looking at the hiding-last-maximized-windows-shows-another
    window-that's-supposed-to-be-hidden bug,  still no fix for that I'm afraid.
    If I don't call MDINext() very strange things happen, and when you hide the
    active maximized window, the previous window is automatically restored...


13/January/2003 - AM
  Hydra
  * Fixed the output windows text resetting to black after a prefs changed
    (wrote my own SetFont() function instead of using the RichEdit one)
  * Tweaks to alternate theme
  + Some other settings in prefs were not being applied correctly after
    a prefs change.  fixed.
  + New "No CR needed" code in CMsgView broke the buffer line counter so that
    "Max Buffer Lines" setting was being ignored.  Fixed.
  * Made "Edit/Clear Buffer" menu option functional!
  * Added "Edit/Copy Buffer" to the menu.
  . Made the other non-working "Edit" menu functions grayed out for now
  + Added checking to the other Edit menu items (previously, if you had no MDI
    windows they would crash the app, hehe, can't believe  no-one noticed that!)
  * Got the mousewheel messages forwarding fromt he text input window
    to the output window, thus: Mouse wheel scrolling works!
    However, for the compilation to work i had to define _WIN32_WINNT
    this might break it on Windows 98.  I need to someone to check
    HydraIRC on Windows 98.
  * Added context menus to the Channel Windows
  * Added context menus to the "Channel Monitor" docking window
  * Added context menus to the "Output" docking window
  + Fixed the about dialog box to use the "MS Shell Dlg" font (chance-2)


12/January/2003
  Hydra
  * Finished theming the server and connect stuff
  * Started theming the channel windows, working well so far.
    Channel logging is now starting to use the log file format string too.

  * Fixed and tweaked the /part command
    you can now do things like this:
    /part
      -> parts current channel, no message
    /part i'm off
      -> parts current channel with a message
    /part #a i'm off
      -> parts channel #a with a message
    /part #b,#c,nutsack
      -> parts channels #b and #c with no messages and also parts the current
         channel with a message
    /part going home,#b,#c
      -> parts channels #b and #c with no messages and also parts the current
         channel with a message
    /part #a i'm off,#b going home,#c cya later!
      -> parts channels #a,#b and #c with different messages.

    Format is:

    /part [#<channel>][,...]
    or
    /part [<message>][,...]
    or
    /part [#<channel> <message>][,...]

    where "..." is any combination of above params.

    If you want some fun, look at the parse code for CMD_Part in ChildFrm.cpp

    -> Let me know if you find a /part command string that breaks it.
  * Fixed single-line clipboard paste word insertion.
  * Bleh, more tweaks to buffered file io error handling
  * Logging could not be started when the user clicks the logging button
    it is updated to reflect the true status. (before, it would appear enabled
    but actually wasn't)  This situation can occur if you have two windows trying
    to write to the same log file.
  * Channel monitor status bar not stripping codes since theme changes, fixed
  * ChildFrm.cpp has no m_pServer->Send() calls anymore, the CMD_* functions
    now call similarly names functions in the IRCServer class that do the
    actual sending.
    This has the effect of passing everything though the themes processor
    in addition to making it much easier to add a scripting host object to each
    server object.
  * Loads of other little changes.

11/January/2003
  Hydra
  * Created a rather neato server theme and made it the default.
  * Finally got the MDI windows to scroll to the bottom after the
    windows are resized/tiled.  (/me barfs over the Win32 messge system)
    Additionally, due to this fix windows don't scroll to the bottom
    when activated/clicked.  (Which makes copying pasting between two
    windows much easier)
  + Various tweaks to the auto-tiler, e.g, resizing the main window
    forces an auto-tile.
  * Multi-line clipboard paste! ("Finally!" I hear you all shout!)
    hehe, yeah, there's no way to cancel the operation yet, so I wouldn't
    go pasting really large files.
    I've set pref with a default of maximum of 100 lines of text to be
    pasted before the routine exists to safeguard this.
    Pref: "Max Clipboard Paste Lines"
    Note: it works as you'd expect.
    1) if you're pasting some text it's appended to the contents of the
      text input control. (as before)
    2) additionally if you're pasting multiple lines of text then each
      line is sent one after the other
    3) on multi-line edits, the last line of a clipboard is sent
      regardless of wether or not the clipboard ended with a CR/LF/CR+LF
      combination.
    4) blank lines are not sent.
  * More tweaks and fixups to the CBufferedFile io buffer code.
    /me slaps himself.
  * Status bar now shows a message when a new query window is created.
  * Status bar can now show private messages on the status bar
    New Pref: "Show Private Messages On StatusBar" default: TRUE
  * Tweaked the channel monitor's "Always On Top" setting so that it's only
    ever on top when the app is minimized to the systray.
  * Raw command output is now shown in the output window and not the
    server window
  * The MODE command has been tweaked so that you can do it from server
    windows and channel windows.
    Additionally, when using mode from a channel window and no channel
    name is specified then the channel name will be inserted into the
    mode command for you.
    This allows you to change modes of other channels from within a channel
    window.
  * Tweaked the output format builder (less memory copies)
  * Tweaked the replacevariables code to work from static strings
  * The server OutputFormater can now generate formatted output strings
    as well as differently formatted and differently filtered log
    file output strings!!!  how cool is that.  you'll be able to filter
    stuff from th display, but include it in logs!
  * Started converting more of the application strings into a table
    of strings (for easy theming and internationalization

10/January/2003 - PM
  Hydra
  + Fixed linefeed in the wrong place for PONG echos.
  * MsgView derived windows now display a linefeed before it outputs
    anything via the Put() function.  This has allowed me to remove the
    \n's infront of all the output, so that I could make...
  * ... lots of changes the the core. Possible bugs and instability issues
    may arise.  Anyway, the point of the changes are so that each output
    that comes from a server/channel/query/dccchat can be customized, and
    also so that the output that is written to a log file can be different
    from the screen output (wooo!)
    Additionally, it future will be possible to assign different output
    themes to windows and logs on an individual basis.  So you could make
    one channel use this
    RPLS_CHANNELTEXT: <TIMESTAMP><<NICK>> <text>
    and have another channel use this:
    RPLS_CHANNELTEXT: <CHANNEL>\<NICK> said "<text>"
    or whatever takes your fancy.

    At the moment the output is not configurable by an end user, but at
    least some of the core supports configurable output.

    Currently a serverout output theme format is defined
    TODO: channels, queries and dccchats.

  + While doing the above, I fixed a couple of crash bugs that could happen
    if the hacked/broken IRC server ever sent out badly formatted replies
    with missing parameters.  (Not that this ever occurred)
  * Some speedups/less memory copying/calls for the debug window.
  * Fixed an issue with logging stipping codes from the output windows.
  . New quit message!
  + Changed the URLs from http://fly.to/HydraIRC to
    http://HydraIRC.sourceforge.net
  + Fixed a few stability issues in CBufferedFile relating to reusing the
    class instance. (only affects builds post 0.336)
  * When the Channel Monitor status bar option is enabled, it used the same
    filter list as the channel monitor itself, so now you won't see joins
    parts, etc, in the status bar.  (Until such time as the channel monitor
    filter list can be configured)
  * Speedup in the channel monitor code.  (Less string copying)

09/January/2003 - PM
  Hydra
  + IMPORTANT!

    The network ID numbers that HydraIRC uses to distinguish network have
    changed for the following networks:

    Network   Old ID,  New ID
    =========================
    EFnet     3        10362
    Deskmod   N/A      10363
    Freedom   7        10443
    Criten    6        10380

    If you're upgrading, use the new HydraIRC.xml (as you should upon each
    new release) but edit your Profile_Default.xml file and change any
    network ID's that your buddy lists are using.  e.g.

    <group name="Friends" id="101" network="3">

    should be changed to:

    <group name="Friends" id="101" network="10362">

    You'll only ever have to do this once,  all new networks will be assigned
    new ID's and all known networks will never have their ID changed.

    Sorry for the inconvenience.

  * Completed the CLogging class!
  * Added auto-path creation to all paths.
    If the directory is not found, HydraIRC will create it, and
    it will also create any intermediary directories too.
    e.g.  if you specify c:\temp\a\b\c\d\e\f as a folder, HydraIRC
    will create all the subdirectories needed.
  + Fixed a very rare crash bug when you close channel windows
    (it normally only occured when closing very large channels while
    the debugger was running)
  * Basic logging is now implemented, currently the log format strings
    are ignored.
    (Heh, you just gotta love the new toolbar icon!)
  * Added a basic automatic window tiling system when windows
    are opened, closed and hidden. (Equim)
    Pref: "Auto Tile" default: FALSE
    -> please test, I might change the default to TRUE as it's really neat

08/January/2003 - AM
  Hydra
  * Added "Messages" page to prefs
  * Removed the unfinished "Filters" page from prefs (I'll finish
    it later)
  * Added a "Logging" page to prefs, along with lots of prefs.
  * Changes to "Misc" prefs page effects other pages now.
  * When in prefs, if you can't click "OK" or can't select another
    items in the prefs tree it means the selected prefs page has
    a setting that is not correct/valid.
    (Previously, all pages had to be valided, but the treeview on the
    left could point to a different page that the active page)

02/January/2003 - AM  (v0.336)
  Hydra
  * The edit server dialog is now functional, but the server list is
    NOT saved so edits are lost when you exit the app (for now)
    Right-click on servers in the connect dialog box and select Edit
    from the context menu to edit them.
  * Server List docking window is now sorted correctly if you join
    a different server using the /server command
  * Servers in the server list use the color for connecting until the
    user is acutallyed logged in on the irc server.  before it would
    use the color while it was waiting for the tcp ip connection to
    be made.  You can now see it change from red, to yellow to green as it
    logs in much better now.

01/January/2003 - AM
  Hydra
  + Finally got around to rewriting the GetTokens() routine so it can never
    overflow (previously it was using code borrowed from another app
    and had quite a few places where it could overflow).
  * More work on the edit server dialog.

31/December/2002 - AM
  Hydra
  * Pressing Ctrl+Enter sends text as-is,  no parsing is done.
    Useful if you want to tell someone how to use a command. ('-Anders)
  * Added context menu to server items of the Connect Dialog and created
    the resource for the Edit Server dialog.
  * Added a priority field the the server properties, also loaded from
    the config files.  This will be used to sort server, so you can set
    preferred servers and servers never to use.
  + Positioned all OK/Cancel buttons on the right hand side for all
    windows and made the buttons all the same size.

  - Version strings for custom builds now have "-Custom" added to them
    so that the CTCP version reply shows this. Please don't change this
    behaviour. (That is, if you compile it yourself, it will add -Custom)

28/December/2002 - AM
  Hydra
  + Timestamps are displayed correctly when showing a hidden query window
    for the first time.  (Before they had the current time, not the time
    the messages were sent)
  * SENT CTCP replies are shown in the server window.
    format is: "-> [user] ctcp-reply"
  * RECEVICED ctcp replies are shown in current window in addition to the
    server window.
  + Moved "Filename" to column 2 in transfers window.
  * User Icon in status bar now shows the icon relating to your user mode
    if the active window is a channel.  e.g.  if you're opped and the
    current window is a channel then it will display a picture of an opped
    user.  Likewise for halfop, voice and normal.
  * Added pref: "Server List Click Again To Hide" default: TRUE (SKroWL)
    Normally, clicking on an item in the server list window will make
    HydraIRC will show the corresponding window if it's not active or if it
    is active it will minimize/hide the window.  disabling this means
    HydraIRC will never minimize/hide the window.
  * HydraIRC can now select a user identity based on what network you're
    connecting to.

    When you click on a server name in the connect dialog, the user
    identity control is updated to reflect the setting in the config.

    Currently HydraIRC doesn't remember which identity was used, this will
    come later and be saved in the user profile settings, and not the main
    HydraIRC.xml file.

    For the adventurous:

    open up HydraIRC.xml and add lastusedidentityname="<identityname>"
    to a network node.  e.g.

    <network name="Unbound" id="0" lastusedidentityname="Hydra"/>

    obviously this is only useful if you have more than one identity,
    see prefs/identities.

    This is useful for me where I connect to QuakeNET and use
    the nick "[xpd]Hydra" (in my "ClanXPD" identity) and connect to Efnet
    and other networks using the nick "Hydra" (in my "Hydra" identity)

27/December/2002 - AM
  Hydra
  * Added "Strict Channel Characters" preference, defualt: TRUE
    Disabling this pref allows the client to join channels with the
    following characters in them:
    ":#&+!"
    However, this also depends on wether or not the server allows these
    channel characters too.
  + Removed the sizing handle on the sever list when docked.
  + Fixed a crash-bug in the GetTokens() code. (chance-2)
  + Fixed a couple of possible crash bugs when DCC's were active when
    you quit the app.
  * Updated the DEVELOPER.TXT file.

25/December/2002 - AM - Merry X-Mas!
  Hydra
  . yes, I'm sad enuff to be coding on x-mas day...
    Still, I'd rather be coding that sitting though a bunch of films
    I've already seen a million times.
  + If the app was minimized to tray and then restored the active channel's
    dirty status is not cleared (Zann)

16/December/2002 - AM (v0.335)
  Hydra
  * Tweaked the default layout so that Channel Monitor and Private Message
    Logs are active for first time users of the app, rather than Transfers
    and Notice Message Log.
  * If the window state can't be restored from the registry, then HydraIRC
    will now start maximized.  Also, since I patched in the new docking
    windows code the default layout seems to work right.
  * HydraIRC now scans the plugin directory for dll files.
    Thanks to FxCHIP for trying to implement this, but the code was bad so
    I had to rewrite what you did.  Hopefuly you can see where you
    went wrong.
  + Fixed plugin unloading!  w00h00!
    Now /unloadplugin and /refreshplugins are actually useful! (Search below)
    It now means that you can start HydraIRC use the plugins, then unload
    the plugin, then delete/upgrade/change it, and then refresh
    the plugins again!
  + Fixed a memory load in plugin exit code.
  + Unresolved plugins are now unloaded and freed.
    (Unresolved means that a plugin required an API that
     was not provided/supported by the core or any other plugins)

06/December/2002 - AM
  Hydra
  * Small crash bug in /SLAP when specified with no parameter. ([xi]logos)

27/November/2002 - AM
  Hydra
  * Patched in the changes to the 3rd party docking windows.  This fixes
    issues with the docking windows gradually getting smaller/moving
    upwards when the window is resized.  My thanks again to Sergy Klimov
    for writing them, and to Peter Krnjevic for doing the fixes to
    Sergy's Code.

27/November/2002 - PM (v0.334)
  Hydra
  * Ok, icons are now initialised and destroyed correctly, the icon code
    now uses the ObjectManager class.
    This will fix any window redrawing problem you may have encountered
    between v0.330 and v0.333.

27/November/2002 - AM
  Hydra
  * I wrote a nifty little class than can manage "objects" (e.g. anything
    that you can allocate and have a pointer to) which the app now uses
    to keep track of fonts (and soon, icons) that it allocates so that
    it can then free them later.

    It's also nifty in that we can use it for storing ALL font that we
    allocate, so later, when we have custom fonts on a per-channel basis
    we will be able to use this to keep track of the actual HFONT and then
    the channel will keep a track of the object's ID. and can then use
    g_Font.GetObjectByID(m_ChannelInputFontID);

  * Converted the prefs code to use the new g_Fonts and ObjectManager
    This cleaned things up quite nicely.
    Converted the rest of the app to use the new g_Fonts too...

  * Yus! C00l.  When you undock the channel monitor, it stays on top
    (and also appears on the taskbar)
    There is one small problem with the docking window code,  when you
    drag the window around it will try to dock with the app even when
    the app is minimized. lol.  so, for now drag it around with the CTRL
    key held down when the app is minimized.

  + The pref "PREF_bShowChannelMonitorMessageOnStatusBar" was always on
    even when disabled in the prefs.  Compounding the icon resource
    leak problem with the status bar.

  . Is it me or is this changelog file getting a bit too verbose on the
    coding aspect of things?



  . bleh, there was an EVIL resource leak in 0.333...

26/November/2002 - AM (v0.333)
  Hydra
  . Bleh, will I evr get to bed
  * Colors were borked.

26/November/2002 - AM (v0.332)
  Hydra
  . Typical, just as I do another release, i find more bugs..

  * Browse buttons for paths in misc dialog were broken. (Zann)
  * I started to extand the SetStatusBar function, then did it a
    different way but one of the calls to it was using the extended syntax
    so it ended up passing a null pointer to the actual call.
    This resulted in the status bar text not changing after x seconds, but the
    icon was removed. (Zann)

26/November/2002 - AM (v0.331)
  Hydra
    *** This marks the first release of hydraIRC's source code, w00-w00!! ***

  * Bleh, removed most of my recent prefs changes code.  My new CInheritedPrefs
    class wasn't suitable because of issues involved in they way we change
    the prefs using the prefs dialog.

    Waste of a few days, ho hum...

  + Changes handling of WM_LBUTTONDOWN to WM_LBUTTONUP so that it clicking
    on the HydraIRC icon in the system tray doesn't inerfere with other apps.

  + DCC Transfer IP Addresss were in reverse oder when printed to the log file

  * Now show's ip of user who we're tranfering files to/from in seperate
    column on dcc x-fers window (Zann)

  * Text input on server and channels is limited to 512 characters (Anders)

  * Wrote buffered file writing code, ready for the channel/server logging...

  * Added /SLAP


23/November/2002 - AM
  Hydra
  * Status bar now shows the current nick for the active server.
  + Server count in status bar wasn't updated until the status bar
    message changed.  Fixed.

23/November/2002 - AM (v0.331)
  Hydra
  * Didn't like how I wrote the new prefs changes, some of it was good, some
    wasn't as good as I wanted.   So I have now written a CInheritedPrefs
    class and have been testing this in a test-program.  This will be integrated
    and will replace the prefs container code I wrote yesterday.

    Basically, objects can have linked-list of prefs, the pref nodes contain
    a prefs id, an inherited status and a value. (bool, int, string, struct)
    Also, any object that dervies from the new class has the ability to be told
    about prefs changes that occur higher up the prefs object tree.

21/November/2002 - PM
  Hydra
  * Reworked how prefs are stored internally, it's much easier for me to add
    new prefs to the code now.  It's also much tider and easier for me to
    work on.  Not that you'll notice any difference of course.
    We now have prefs containers, which means objects in HydraIRC can have their
    own subset of prefs which can be stored and used seperately and they can also
    inherit their default values from the main prefs.

    An example: it's now possible to have different colors for different channels
    on different networks. !!!!

  * Added a new pref: PREF_bShowChannelMonitorMessageOnStatusBar, default: true (Zann)
    This makes the status bar double as a single-line channel monitor.

21/November/2002 - AM (v0.330)
  Hydra
  * Double clicking a privmsg now creates and shows a query window for the user
    regardless of whether or not the query window existed or was hidden

  * Added a rather cool status bar, with icons and everything..
    To enable it (if it's no already) make sure "View/Status bar" is checked
    in the menus.

    Additionally, the status bar shows Active Transfers (Sends and Receives
    seperately) and Servers (Disconnected and Connected)

    Also, when certain things happen, a message is shown in the status bar and
    it's icon changed.   Then 15 seconds after the event it changes back to the
    default status text.

    Currently status messages are show for these items:

    new Privmsg's
    new CTCP
    Server connects and disconnects.
    A welcome message!

  * New quit message :)))

  * Channel and Query icons for child window are now correct (except when the
    window is maximized, bleh..)

20/November/2002 - PM
  Hydra
  * Pressing DEL when an active dcc transfer is highlighted just cancels it, if you
    press DEL again it will remove it from the list.  Before it cancelled AND
    removed it in one key-press.

19/November/2002 - PM
  Hydra
  + Pressing ctrl+h with maximized windows didn't work, fixed!
  + It wasn't possible to change the identd defaults due to an incorrect
    control (path selector instead of edit box)
    Paths are now seperated from strings in the prefs.
  * Channel properties dialog box resizes better and shows more of the topic
    tex than it did before.
  * Added new feature, "Selective Flood Control", experimental, may be removed...
    Default: FALSE
    When enabled, flood control is only activated when you send a bunch of commands
    to the server that are KNOWN to trigger server/bot flood controlling
    e.g.  lots of PRIVMSG commands (used for CTCP also)
    currently the list of known commands is: "PRIVMSG" (yes, that's it so far..)
    Normally flood control is activated when you send a lot of ANYTHING.

15/November/2002
  Hydra
  + Added /PIMP
  + Plugins can have name, author and info strings which are displayed by HydraIRC
  + Text is always logged to the output view now, even if it's not visible, this
    makes it somewhat more useful since the output window now has filtering on it
    it doesn't output as much developer crap as it used to.

    (Before, HydraIRC would only log output to the window if it was visible)

13/November/2002 - PM
  Hydra
  + Added a secondary reconnect timer and two new prefs:
    "Reconnect Retries2" default: 100
    "Reconnect Delay2" default: 300 (5 mins)

    Existing preferences renamed to:
    "Reconnect Retries1" default: 10
    "Reconnect Delay1" default: 30 (seconds)

    HydraIRC now behaves as follows:

    retry connection "Reconnect Retries1" waiting "Reconnect Delay1" seconds
    between retries, then retry connection "Reconnect Retries2" waiting
    "Reconnect Delay2" seconds between retries.

    Not tested.

  + Server doesn't store it's own copy of "Reconnect Retries" any more.
    this means that a change in prefs will actually take effect immediately
    rather than having to close and re-open the server window.

04/November/2002 - AM
  Hydra
  + The exact ctcp request is shown in the server window too, previously
    the information shown was more limited. (Zann)

03/November/2002 - PM (v0.329)
  Hydra
  * Added "/KR <kick reason>" command to the plugin so you can specify a
    kick reason, and then kick lots of users.

    E.g.

    /KR Get the fudge out
    /KICK person1
    /KICK person2
    /KICK person3
    and so on...

  * Plugins can now be unloaded and re-loaded without restarting HydraIRC.
    This was actually quite a bit of work, as care has to be taken
    that in-use plugins are not unloaded.  Also we have to remember that
    a single plugin can provide multiple API's and can require API's
    provided by other plugins!

  * A Plugin can now prompt the user if the user tries to unload it.

  * Information on which API's plugins provide are now listed when a
    plugin is loaded.

  * Added "/UNLOADPLUGIN <Plugin Name> [FORCE] [IGNOREINUSE]"

    The FORCE keyword makes HydraIRC unload the plugin without prompting
    as long as the plugin is not in-use by other plugins.
    The IGNOREINUSE keyword tells HydraIRC to ignore the fact that
    a plugin may be in-use by another plugin.  If a plugin is still loaded
    that uses the API's that the one you're unloading, you can expect it to
    behave badly.

    To unload two plugins that each use an API that the other provides
    you must use the IGNOREINUSE parameter on each or you will get back
    a "Plugin In Use" error for each plugin that uses the other's APIs.

    btw, It'd be a bad idea trying to issue this command while other plugins
    are still loaded.
    "/UNLOADPLUGIN HydraIRC_Core IGNOREINUSE"

    :)

    FYI: the HydraIRC_Core plugin is only IN-USE if other plugins are loaded.
    Thus it's quite possible for you to do this: "/UNLOADPLUGIN HydraIRC_Core"
    when no other plugins are loaded.
    However, why you would ever want to is a mystery. :)


  * Added "/REFRESHPLUGINS"

    /REFRESHPLUGINS doesn't actually scan for all plugins yet, it just loads
    plugins/plugin.dll again for now.

    Note: this code isn't 100% finished, it should work, but for some reason
    windows isn't unloading plugin.dll even though I'm calling FreeLibrary()

    Also, it doesn't unload plugin.dll first, so it might try and load it
    twice...

    TODO: Finish "/REFRESHPLUGINS"
    TODO: Add "/LOADPLUGIN <Plugin Name>|<DLL Name>"
    TODO: Add "/RELOADPLUGIN <Plugin Name>" alias.



18/October/2002 - AM (v0.328)
  Hydra
  + Corrected a slight bug in the DCC receive code that I introduced in 0.327
    (lol,  I did that classing C syntax bug: i used an = instead of an ==
    Not done that for a while...)

    Oh well, to make up for it, I've added this..

  * New pref: "DCC Receive Double Click Opens File". Default: FALSE
    When you double click on a received file, it opens the directory the file
    is in, if you enable this option it opens the file using the appropriate
    application. (SKroWL)

    NOTE: This is dangerous, if someone sends you a virus/tojan you may
    inadvertantly run it.  If you enable this option make sure you're running
    some anti-virus software, or you trust the sender before opening it.

    TODO: At a later date, I could add a list of file types to OPEN and a list
    of file types to open the directory for (e.g.:  open: zip, jpg, pdf,
    don't open: exe, doc, xls, bat, cmd, vbs, js)

    TODO: Add an option to run a virus scanner on received files.

  * You can now double click an in-progress or failed transfer to open the
    directory that the partially recieved file is in.

  * Transfer list items change colors depending on their status

  * Added more color prefs:

    item_transferlisterrortext
    item_transferlistcompletetext
    item_transferlistnormaltext

  + " &#$:" characters are replaced with '.''s when sending files that have
    any of these characters present in the filename.

    e.g.
    "this is a te$t #1 & it rocks:.txt"
    would become:
    "this.is.a.te.t..1...it.rocks..txt"

    The DCC CTCP commands don't allow for spaces in file names, and some file
    systems don't accept those characters. (Or just cause problems)
    Let me know of any other characters you think should also be excluded from
    remote filenames.

18/October/2002 - AM (v0.327)
  Hydra
  + Corrected a slight bug in the DCC receive code that I introduced in 0.326

17/October/2002 - PM (v0.326)
  Hydra
  * More work done on DCC transfers to avoid stalls.
    Should be better now,  if you have problems enable console logging
    do a transfer and send me the log.

    Lots of information regarding the transfer appears in the output
    window too.

    Added a new pref, "DCC Verbose Transfers", default: FALSE.
    It's *STRONGLY* Advised to turn this on before you send me any logs
    of transfers or I won't have much to go on.

    It's of by default to speed up transfers and lower CPU usage as *lots*
    of messages are generated

17/October/2002 - AM (v0.325)
  Hydra
  * A basic implementation of DCC file sending (but not dcc send resuming) is in.
    check out the prefs, more docs and stuff in 0.326.

    There are probably bugs,  please test and report back to me.

    Oh yeah, better tell you how to send files then too eh, lol

    use "/DCCSEND [nick]" and a file request will be shown.

  . I've got a cold, I'm going to bed.

16/October/2002 - PM
  Hydra
  * Started to adding DCC file sending.
  + Fixed a bug with a DCC SEND string received from another user that would
    cause the client to crash (oops!)
  + Fixed a small memory leak in the nick cycling code.

09/October/2002 - AM
  Hydra
  + Added CTCP to the list of things that make the server status go dirty (Zann)

07/October/2002 - AM (v0.324)
  Hydra
  + Underline style for "Server List Dirty" font was always set. regardless of
    whether or not underline="false" appeared in the config file. (Zann)
  + Identd prefs were not being loaded correctly. (Zann)
  + More verbose Identd usage is now reported.  Check the server status and the
    Output window while connecting to a server
  + Added an EDIT Button to the connect dialog box.
  + Added a fix (i think) to the duplicate nick in global user list on rejoin
    problem.

07/October/2002 - AM (v0.323)
  Hydra
  + Upon restoring HydraIRC from the system tray, the current MDI window's server status
    dirty flag is reset. (Zann)
    Actually, this now happens every time HydraIRC takes focus..
  * Server status dirty flag is now just set on the server list as a whole, instead of
    setting flags on individual items.
  + Fixed an issue where HydraIRC would assert when closed with child windows open
  * Added pref: "Hide Taskbar Buttons for hidden windows" default: TRUE
    So, the taskbar has the same functionality as the MDI tabs.
  - With v0.321 you were to be able to click to show a window, and click again to
    hide it (like the server list) and the pressed status indicated wether or not
    the window was visible or not.  This didn't work for maximized windows. (God
    damn it.)  So I've reverted that back to how it was before 0.321.  Thus making
    the taskbar have the EXACTY same functionality as the MDI tabs.

  . I'm not including Tabbed MDIs in this release, just to see the effect on people :)
    Get used to the taskbar!

06/October/2002 - PM
  Hydra
  * Added pref: "Identd Enabled" so you can disable/enable identd
  * The Ident Responses in your user identities now work.
    When an ident request is received, HydraIRC checks to see if the request
    is coming from a server that it's currently connected/connecting to and if it
    is from the same address then the ident responses associated with that server
    connection's user identity is used.

    This means you can have different ident responses for different servers!
    (And possibly different ident responses for connections to the same server,
    depending on how the irc server in question caches your host name..)
  * User identity dialog updated so that you can configure ident responses
    for your identities.
  * /Set now updates the following prefs immediately:
    "misc/Default User List Width"
    "misc/Alternate Channel Layout"
    "misc/Identd Enabled"
  * Added string pref default capability
  * Added a default for "DCC Downloads Folder", it defaults to:
    "<HOMEDIR>\\Downloads", "<HOMEDIR>" is expanded to the folder that
    HydraIRC was run from.
  * Added two new string prefs, "Ident OS", "Ident UserName", defaults:
    "Win32" and "HydraIRC" respectively.

    These are used
    a) if the server's current user identity doesn't override them
    b) the identd request does not come from a server we're connected to.

    Note: does anyone want a pref called "Ignore Identd requests from unknown sources" ?
    It's quite simple for me to add..
  + The serverlist dirty flags are set according to a filter.
    The filters are as follows:

    Server:
      FILTER_INCLUDE,UNKNOWN,QUERYTEXT,NOTICE,FILTER_END
    Channel:
      FILTER_INCLUDE,UNKNOWN,CHANNELTEXT,QUERYTEXT,NOTICE,TOPIC,FILTER_END

    Possible items are:
    UNKNOWN,NOTICE,TOPIC,JOIN,KICK,MODE,NICK,PART,ACTION,QUIT,CONNECTING,CHANNELTEXT,
    QUERYTEXT,SERVERMSG,CTCP,FUNCTION,SOCKET,WARNING,ERROR,GUI,INFO,XML,CONSOLELAST

    Let me know if you want me to adjust the defaults in leui of prefs for filter lists..

05/October/2002 - AM (v0.322)
  Hydra
  * Added the ability to set/change fonts via the /set command
    The format is:
    /set font/<font item>=<font name>[,<size>|-][,[N][b][i][u]>]

    e.g.

    "/set font/server list=tahoma,8,-"          - tahoma,8,keeps current style
    "/set font/server list dirty=tahoma,8,Nbu"  - tahoma,8,bold underline style
    "/set font/channel input=verdana,10,i"      - verdana,10, adds italic style
    "/set font/query input=gothic,12,N"         - gothic,12,no style
    "/set font/server output=-,-,i"             - adds italic style

    b,i,u = bold, italic, underline, N = No Styles
    if you don't specify N, then existing styles are added to.
    so if the font was already italic, then specifying "b" would
    make it bold AND italic, whereas specifying "Nb" would make
    it ONLY bold.
    - = keep current setting

    Incorrect syntax is ignored.

    Note:  there is a side effect to this, all font formatting is lost
    in CMsgView windows (channel/server/query/chat/etc) due to richedit
    being crap.  When richedit is replaced this side-effect will go away.
  * Added new font prefs:
    "Server List"
    "Server List Dirty"

    Note: Don't make the font size for "Server List Dirty" bigger than
    the font size for "Server List", because it won't fit properly.

  * Added new color prefs:
    "Item_serverlistdisconnected"
    "Item_serverlistconnecting"
    "Item_serverlistconnected"
    "Item_serverlistserverdirtytext"
    "Item_serverlistserverdirtybackground"
    "Item_serverlistdccchatdirtytext"
    "Item_serverlistdccchatdirtybackground"
    "Item_serverlistchanneldirtytext"
    "Item_serverlistchanneldirtybackground"
    "Item_serverlistquerydirtytext"
    "Item_serverlistquerydirtybackground"

    Knock yourself out :)

  . I came across a weird problem when you close a server window, by pressing alt+f4 HydraIRC
    would crash, but only sometimes.  However if you close it using the X button in the top
    right corner of the window it never crashes.
    I *think* this is now fixed (after several hours of debugging) So, can all you beta testers
    please keep closing HydraIRC server windows by pressing Alt+F4, and let me know if it
    crashes or not.

  * Added some bot auth aliases (Anders/Hydra)

    /AUTH     = "/RAW PRIVMSG NICKSERV :identify %s"
    /QNAUTH   = "/RAW PRIVMSG q@cserve.quakenet.org :auth %s"
    /DALAUTH  = "/RAW PRIVMSG nickserv@services.dal.net :identify %s"
    /DALNS    = "/RAW PRIVMSG nickserv@services.dal.net :%s"

    Thus, to auth on dalnet, you can just go: "/DALAUTH mypassword"

    Note: Untested.

  * Added some xdcc aliases. (SKroWL)

    /XGET     = "/CTCP %s xdcc send %s"  e.g. /XGET Nick Pack
    /XLIST    = "/CTCP %s xdcc list"     e.g. /XLIST Nick

    Note: Untested.

04/October/2002 - PM
  Hydra
  * Added some rather cool new functionality to the Server list window
    1) it now changes colors of servers depending on whether they're
       connected, connecting or disconnected.
    2) it changes colors (and fonts) of channels, servers and query
       windows to indicate the presence of new text in the window since
       the window was last active.

    Note: there's always something...  *sigh*  Sometimes, when you use a larger
    or bold font, the text of the treeview items marked as dirty are not fully
    displayed.
    I will attempt to fix this ASAP.

  - *Lots* of changes to internal code..

04/October/2002 - AM (v0.321)
  Hydra
  + Gah, 0.320 still had a bug, i thought i'd fixed it..
    Oh well, I had to change loads of code to finally get
    "Create Query Windows Hidden" working correctly, now the window
    isn't actually created until you select the Query window from the
    taskbar or the server list.
    Note: no MDI tab for the query window will appear until you select
    it, because MDI tabs can *only* exist for MDI windows, and the
    window is not created yet!  This is due to the implementation of
    the MDI tabs, which will at some point be replaced with far more
    useful tabs.
  * Added a new pref: "Use Icons On Taskbar" default: TRUE.
    This new pref is self explanatory.
    However, it only affects taskbar items created AFTER the preference
    is changed.  The effects are not instant.
    Also, there's a gap (sigh) to the left of the buttons where the
    image would be,  blame microsoft for that, nothing I can do until
    I write my owner drawn ReBar control.
  * The pref: "ReOrder Taskbar Buttons" now defaults to FALSE
    See below...
  * The buttons on the taskbar now indicate wether the window it relates
    to is visible or not.  If pressed, the window is visible, if not
    the window is hidden.
    Pressing a button activates the window, unless the window is already
    active - in which case it hides it and activates the next one.

    This behaviour now mimics the server list selection, which is what
    *I* wanted to start with.

    If anyone likes the old way it worked, and doesn't like the new way
    then let me know, and I'll add a preference...
  + The message of a /query command with query window already present
    would not be displayed in the query window. (Zann)

03/October/2002 - AM (v0.320)
  Hydra
  . Damn maximized windows, damn them to hell

  + *sigh* Creating hidden query windows had a weird side effect if the
    current window was maximized.

02/October/2002 - AM (v0.319)
  Hydra
  . I'm not including the non-TabbedMDI version of HydraIRC any more
    now that the MDI tabs seem stable enough, if you want it - ask.
    "HydraIRC-TabbedMDI.exe" is now just "HydraIRC.exe"

  * Added code to update items in the server list, this means that
    if you connect to a server and then use /server to conenct to a new host
    the item in the server ist tree is now updated accordingly.
  * Added a new preference: "Create Query Windows Hidden" default: FALSE
    This relates to query windows not created by the user, but by other
    users.  (e.g. someone sending you a privmsg, not you typing in /query)
  + When sending a message via a /query command, the message is now shown
    in the current window using the correct notation: "-> *user* message"
    instead of "<yournick> message"
  * When sending a /msg with a query window already open, the message is
    now also displayed in the query window, as well as the current channel/server
    window.
  * When a user/bot attempts to dcc you a file that you have already downloaded
    and completed (and is still in your downloads directory) HydraIRC will
    still accept the DCC request and will tell the remote host to resume at
    the end of the file.  Thus causing the remote host to complete the xfer
    rather than sitting there, waiting to time out.  This means that if you
    accidentally requeue a file from an xdcc bot, you won't have to wait
    for the bot to time out before it sends you another file. :)
    The transfer status will show "Connection Lost" and a message will appear
    in the output window when this happens.
  * A default exclude filter has now been applied to the output view, this
    means that Socket output is now longer displayed there.  Later on the filter
    list will be customizable, but this makes the output window a bit more
    useful to the average user now.
  * Added "/QUOTE" as an alias for "/RAW" to the core.
  * If you forget the # on a channel name when joining one, it's added for you
    so "/j foo" will join "#foo" (Imajes)
  * Added "/EXIT [-NOW]".  /Exit is used to quit the application.
    if you specify the optional -NOW parameter then HydraIRC will quit without
    prompting, regardless of how many dcc transfers or server connections are
    open. (Imajes)
  * Added "/EXITNOW" as a plugin alias for "/EXIT -NOW" (Imajes)
  * When an op that is also +v gets deopped, he now moves into the "Voice" group
    instead of the "Normal" group (or if he's a buddy, his icon will change.)
    Unless the op was already +v when you joined the channel, as the irc protocol
    doesn't specify "+@Nick" for a voiced op in the "NAMES" reply when joining
    a channel. (Anders)

01/October/2002 - AM (v0.318)
  Hydra
  + Oops, I goofed.

    i had this:
      if (BOOLPREF(PREF_bRestrictNoticesToQueryWindows) && !pQuery)

    but really i wanted this:
      if ((!pQuery) || (pQuery && !BOOLPREF(PREF_bRestrictNoticesToQueryWindows)))

    /me 's head explodes. logic overload...

30/September/2002 - AM (v0.317)
  Hydra
  + Pressing WindowsKey + M didn't minimize HydraIRC to the systray
    when "Minimize To Systray" was enabled

24/September/2002 - AM
  Hydra
  + Notice messages are no-longer forwarded to other windows if there
    is a query window open for the user.

  * Added "Restrict Notices To Query Windows" (when a Query window is open)

  + Renamed "Restrict Server Notices" to "Forward Notices" and inverted the
    preference default and the code.

    We now have the following NOTICE message related options, please read
    this and make sure you understand them.

    "Forward Notices"
    -> Enables forwarding of NOTICE messages to windows other than
      query windows and the server window.

    "Channel Member Notices Only"
    -> If enabled, only NOTICE messages from users in channels you have joined
      are forwarded to other windows.

    "Cross Network Notice Forwarding"
    -> Enables Displaying of NOTICE messages in the active window
      if "Forward Notices" is enabled and the active window is on server
      other than the server that the message originated from.

    "Restrict Notices To Query Windows"
    -> If enabled, NOTICE messages are NOT forwarded to other windows if
      there is a query window open for the user/host sending the NOTICE
      message.

  * Added a "Forward Messages" preference, defaults to TRUE.
    -> If enabled, PRIVMSG messages are forward to windows other than
      query windows and the server window.

    Note the prefs: "Forward Messages" and "Forward Notices"
    have been added so that it is easy for a script to override and
    provide a more customizable forwarding scheme for notices and
    privmsg's.  e.g.  a script could then disable them and then implement
    "/forwardnotices <nick> [all windows|active window|all server windows]"
    or something along those lines...

24/September/2002 - AM (v0.316)
  Hydra
  * Edit buddy group notification dialog is now resizable, not that it
    really needed to be..
  + Moved sending MODE command from OnJoin to Join,  OnJoin is called
    after each RPL_ENDOFNAMES, and thus after each /raw NAMES <channel>
    which *someone* didn't like.  (Zann)

24/September/2002 - AM (v0.315)
  Hydra
  + 0.314 introduced a bug where the global user list would not update
    if the MDI windows were maximized and you changed channels by clicking
    the MDI tabs.  Fixed! (Zann)

23/September/2002 - AM (v0.314)
  Hydra
  * Channel mode changes are shown differently to server/network modes
  + Mixed mode changes were not being processed correctly, parameters
    were getting muddled.  The code assumed that all modes had parameters
    when of course, the don't!
  * A MODE command is now automatically issued when a channel is joined
    to help the ChannelProperties dialog display more accurate information.
  * We now kept track of Channel mode changes.  Though only modes
    "k,i and l" at the moment.  More later.
  + Small DCC Xfers that take less that 1 second to complete (!) now
    show a transfer time of 1 second and a value in speed, rather than
    [0:00:00] and [00.00kb/sec].
  + Errors were reported for /topic and /mode in channel windows
    with no parameters, rather than just sending the command with
    no parameters to ask the server for the topic and channel modes
    respectively.

  . Note: With all the changes to the mode processing, some testing
    is required.  Please give me feedback if you find problems with
    a particular channel/mode command.  If you do get problems, enable
    console logging via the prefs, this will help me find the cause
    of any such problem.


22/September/2002 - PM
  Hydra
  * Server names are now checked against the list of servers for a match
    when connecting so that the network is set correctly.
    If a match is found the network and description are set to the ones in
    the server list.  This affects typing in a server URI and using the
    /server and /newserver commands.
  * DNS Resolution is now multi-threaded, this means that the application
    remains responsive during servername lookups.
    (Note: this took me several hours of work cos I had to learn
    about thread messages queues for threads that don't have windows.
    Also, this had been bugging me for ages now, and it also seems like a
    nice small contained thing to make turn into a thread)
  * Notice messages are printed to query windows if one is open for the
    user sending the notice.  This is great for talking to bots that use
    send NOTICEs as replys, rather then PRIVMSGs
  * An "Reconnect Aborted" message is shown when you click disconnect if
    the server was still trying to connect.
  * Added a "Channel Properties" dialog box,  for a while now it's been a
    pain in the ass to modify/change the topic.  The Channel Properties
    dialog makes this a breeze.  Select Channel/Properties from the menus.
    Some controls are disabled if you don't have Ops (or Halfops!).
    I experimented with using a custom CEdit control which disables editing
    (as opposed to disabling the control completly) but alas, I've not found
    an easy way of disabling all keyboard input apart from cursor keys
    and ctrl+c. (The idea being that you might want to copy some of the
    topic from the channel properties window to the clipboard).
    TODO: See if there's a WTL (or MFC + source) class that disables
    editing but allows text selection and selected text copying.
    There must be one out there somewhere,  anyone wanna see if they can
    find me one ?

21/September/2002
  Hydra

  . Hehe, too much BF1942, oh and Breed's coming soon, as well as UT2k3
    I fink HydraIRC might be taking a back seat to some serious FPS action! :)

20/September/2002
  Hydra
  + Finally fixed the weird MDI maximization bug.
    The old bug could be triggered by having a server and a channel window
    then maximizing the channel window, then toggling the user list once
    and then switching to the server window.  the server window would appear
    non-maximized over the maximized channel window.
    For some reason the MDI Child didn't like sending itself
    a WM_SIZE message.  weird..
  * Added an option to hide the tabs of hidden MDI windows
    "Hide MDI Tabs for Hidden Windows", default: TRUE
  + Renamed "ReOrder MDI Buttons" to "ReOrder MDI Tabs" to keep things
    in line, changed the default to FALSE
  + Fixed the issue where the last hidden MDI window would re-appear
    if you switch from HydraIRC to another app, and then back to HydraIRC
    Or when you clicked the last MDI window's item in the server list to
    hide it.  It wouldn't hide.

    *However*, this breaks a kludge that was being used to show a hidden
    mdi window for which we have an MDI tab.
    I'm waiting for the TabbedMDI author to get back to me so that I can
    implement proper tab NM_CLICK handling which will fix this.
    So, when "Hide MDI Tabs for Hidden Windows" is DISABLED this kludge
    is still used, so the above two bugs are still present in this mode.


18/September/2002 - AM
  Hydra
  + Average Transfer speed is shown in []'s when the x-fer is complete
    It was supposed to do this before, but i forgot to remove an if()
  + Got rid of window redrawing/flashing when the app starts
  + Window styles for the docking windows were wrong, all windows
    are now WS_POPUP and don't have WS_EX_TOPMOST set.  This will
    be configurable at a later date.
  + taskbar's text is now updated correctly when using the /server
    command to connect to a different server for server windows
  + taskbar's text is now updated correctly when a user in a dcc
    chat or query window changes nicks.

  . Had a quick look at SAPI 5.1, support soon! (Speech API)

11/September/2002 - PM
  Hydra
  * /Q is now an alias of /QUERY instead of /QUIT :)

  . Spent quite some time researching and getting used to multi-threading
    under Win32 (I've never done any windows coding before HydraIRC - it's
    my first Win32 app and first C++ app, let alone any Multithreaded code).

    I found out the following:

    * Multithreading each MDI window is possible, but is a total nightmare
      to implement and will probably cause lots of problems in the future
      and will make further changes to the MainFrm.cpp/ChildFrm.cpp core
      much more time consuming.
    * Multhreading the socket receive will be easy enough to do, without
      causing a headache.
    * Multithreading each server instance is possible (and I did this,
      if you've got the source, see USE_THREADS.  But alas, this is just
      as much of a nightmare as my first point above.
    * Having a single thread for the UI and then seperate threads for each
      server instance is possible, but again, a total pain in the ass when
      it comes to syncronisation.
    * Looking forwarding to scripting, i can see a few problems/possible
      areas that would be problematic for some scripting possibilities
      if scripts had to wait for thread syncronisation.

    So, hmm, from this, it seems that i will not do much multithreading
    of major stuff, but will keep threads for simple/contained stuff,
    like socket receving, dcc x-fers, identd.


11/September/2002 - AM (v0.313)
  Hydra
  * Added slightly updated version of the WTL Docking windows code
    to HydraIRC

    This might fix the Workstation Lock/Resize issue.

    This also seems to fix the problem with the vertical height
    of the tabbed docking windows being reset when the application
    is restarted.

    The ghost boxes (when dragging docking windows) are drawn in
    much better positions now too.  (Before, it would incorrectly
    draw ghost boxes outside the main window now it doesn't)

    The hover delay for showing the autohiding docking windows
    is reduced, more responsive now.

  * If you have a query window open to a user, and then that user
    quits, the quit message is posted to the query window.

  + Attempting to /join after a +b didn't send /JOIN again, it just
    showed the channel window.  Now it attempts to rejoin the
    channel correctly.

  + There was no edit paste/cut/copy for query window text input, fixed!

  + The channel's userlist is now cleared when rejoining a channel
    after a disconnect (Rovastar, please tell me if this fixes your
    problem)

  + Updated libxml2-2.4.22 to libxml2-2.4.24
    There were a few bugs in the old version, I don't think you'll
    notice any changes though.

10/September/2002 - AM (v0.312)
  Hydra
  * Query window taskbar text was servername instead of other nick
  * Forgot to remove a debug text from /query

09/September/2002 - PM
  Hydra
  * Added server window own nick tab-completion.
    (e.g.  press tab in a server window and your current nick
    is added to the command line)
  + Added processing of WM_WINDOWPOSCHANGED messages, hopefully
    the windows will scroll to the bottom when they are tiled.
    Untested.
  + Added processing of WM_SETTINGCHANGE messages, might fix the
    XP SP1 redraw issue untested, not installed SP1 yet. (Zann)

  * Added more new prefs:

    Misc
    ----
    "DCCChat Window Timestamps" default: TRUE
      Enables timestamps in DCC Chat windows by default
    "Query Window Timestamps" default: TRUE
      Enables timestamps in Query windows by default

    "Query Notify Reactivate Delay" default: 300 (5 mins)
      This option specifies how long NOT to process NE_PRIVMSG's
      notification events for when you have a query window open.
      This makes it so that hydrairc doesn't keep beeping everytime
      a user /msg's you in quick sucession (e.g. in  normal conversation
      is a query window).  If the user then /msg's you again after 5
      mins (and the query window is still open) then the NE_PRIVMSG
      notification will be played again.
    "DCC Transfer Timeout" default: 300 (seconds == 5 mins)
    "DCC Resume Timeout" default: 30 (seconds)

    Fonts
    -----
    "DCCChat Input"
    "DCCChat Output"
    "Query Input"
    "Query Output"

    All fonts still default to name="Tahoma" size="8"

    To change the default go into prefs click ok, then quit
    HydraIRC then edit the default_profile.xml file.
    Or use the /SET command.
  * Woot, string prefs are in!

    "DCC Downloads Folder" default: "%workingdir%\Downloads"
      This allows you to configure the path DCC transfers are
      stored in.
  * Added a /NOTICE alias for /RAW NOTICE <nick> :<message> to the
    default plugin.
  * You can double click items in the private message log windows
    to open a new query window (or show the existing one) for the
    user that send the message, as long as the server instance
    that created the message still exists, and the server instance
    is connected to a server on the same network.
  * Added a rather nice property control so that you can change
    all the misc pref settings from within the gui.
    Note that the same issues with the /SET command apply to newly
    changed prefs.  That is, some changes won't take effect
    right away, you may have to close and reopen windows depending
    on what the preference is.
  * Double-clicking a completed DCC Receive Transfer will use the
    configured shell handler for folders (e.g. explorer) to open
    the folder that the file was downloaded to.

09/September/2002 - AM
  Hydra
  + Added pref: "Restrict Server Notices" default: FALSE
    This enabling this prefs makes HydraIRC *ONLY* output /msg's
    into the server windows instead of forwarding it to the current
    window as well.  (ingigo2)

    To change the defaults go into prefs click ok, then quit
    HydraIRC then edit the default_profile.xml file.
    Or use "/SET misc/Restrict Server Notices=value" where
    value is "true" or "false"

07/September/2002 - PM
  Hydra
  + Did a bit of memory leak testing today and found that I goofed
    when I read the docs for xmlGetProp(),  i wasn't freeing the string
    it returned, I assumed it was returning a point to the xml structure
    in memory.

    Found a couple of silly memory leaks here and there, nothing major
    just 12 bytes here, 16 bytes there..

    Found a really funny leak (to me, anyway) in TextQueue.h, the
    code to free the text queue items was there, but somehow I
    put it in the class constructor, instead of the destructor! lol.

    The only leak that would cause a problem over time was where the
    string fro the new group name (with count) wasn't being freed
    so each time you switched channels or users joined on parted
    you'd loose about 14 bytes.  If the global user list was in view
    then this would be exaggerated due the to fact the users are added
    to the global user list.  Really, group name counts needs be delayed
    while switching or joining channels.  However, I already noted
    as such in the source code as a TODO - see UserList.cpp, line 488.

    In fact, heh, all I've done so far today is memory leak testing
    it didn't help that the MS Docs for getting CrtDumpMemoryLeaks()
    to work correctly were wong (FYI: you *must* include <malloc.h>
    *before* you include <crtdbg.h>)  /me slaps Microsoft

    Bleh, 4 hours later..

  . Spent a couple of hours trying to get the richedit control to
    display characters from a different font, codepage but alas
    it seems totally borked.  I even went as far as creating a seperate
    app with just a richedit control and that doesn't work either.
    I've tried everything I can think of regarding that.

    The only solution seems to be to create my own text output class
    that replaces the richedit control.   As it is, the richedit control
    is pissing me off already, just look at all my comments in
    this file and MsgView.cpp.  The list of things that I want to do with
    rich edit, but can't, are growing.  (Such as adding text to the end
    of the document while it's scrolled back and selecting text..)

    That, and it's very slow.
    (even though HydraIRC is fast, other IRC apps that don't use RichEdit
    are faster..)

    For now, richedit will stay, but it will be replaced at a later date.
    However, doing that is a *very* big step, as there is lots to do:
    Text Output
    Scrolling
    Selecting
    Cursors
    Links and URL detection/highlighting.
    .. To name but a few, *all* of which will have to be recoded.
    If anyone wants to find an open source text output control that
    does all the above, let me know.

    In addition to this, I just had a quick look at wordpad, which is a
    rich-edit 3 program.  And even that can't display this:
    "Ïð?ïðîâåðêå ïî÷ò??êîãä?íà ôò?çàéò?ïûòàåøüñÿ" correctly.
    It doesn't let you select or change the charset/script!

    There are also a few other RichEdit based text editors "out there"
    such as KPad and NPad, both of which allow you to select text
    click format/font select "verdana" with script: "cyrllic" and then
    click OK, and none of them format the text correctly either.

    I believe this is a BUG with richedit,  maybe I'll send Microsoft
    a bug report.

    Ok, so, Zann, if you're reading this, you know what you have to do :)

06/September/2002 - PM
  Hydra
  * Added Query windows!
    /QUERY [nick][,...] [message]

    * Query windows appear in the server tree window
    * Nick changes are handled.
    * Tab completion of the other user's nick works.

    Added pref: "Create Query Windows Automatically" default: TRUE
    This pref makes HydraIRC open new query windows for each person
    that /msg's you.

    Added pref: "Restrict Queries To Query Windows" default: TRUE
    This enabling this prefs makes HydraIRC *ONLY* output /msg's
    into query windows (if one is open) instead of forwarding it to
    the current window as well.

    To change the defaults go into prefs click ok, then quit
    HydraIRC then edit the default_profile.xml file.
    Or use the /SET command.

  + Did a code cleanup of ServerList.cpp, merged 6 similar functions
    into 3 generic ones, then added Add, Remove and Find for
    the new Query Windows.
  + Fixed a bug in ActiveWindowPrintf(), it was printing things
    to the server window when it shouldn't because somehow I managed
    do delete a line containing the word "else"
    heh.
  + added mode processing for +h and -h (Zann)
  * Added the following additional color preference items (zann)
    "Item_globaluserlistbackground"
    "Item_globaluserlistnormaltext"
    "Item_serverlistbackground"
    "Item_serverlistnormaltext"

    >>>>>>>>>>>>TODO: Update colorschmes.txt file!!!! <<<<<<<<<<

02/September/2002 - AM (v0.311)
  Hydra
  + Fixed a minor bug that would crash HydraIRC when mIRC sends about
    5-10 small (<100k) files to you at the same time. (PsionicMan)
  + The plugin wasn't handling command alias's with no parameters
    correctly.  (e.g typing /topic in a channel with no parameters
    gave you TWO error messages instead of one)

30/August/2002 - AM (v0.310)
  Hydra
  * Added notification events for DCC Files
    Sending, Receiving, Failed and Complete
    (not that you can send files yet - only receive works for now - but the
    notifications are in place!)
  + Fixed a bug with ACTION private messages (which HydraIRC can't send yet,
    because you don't get query windows to yupe /me into).
    They were dumped to the server window ok, but when I did the message
    forwarding stuff I must have broke it. :)
  + Fixed a small bug with the flood control re-activating incorrectly
    (forgot to reset the counter at the next EV_TICK with nothing in the
    put buffer).
  + Some default notification events were broken
    (one of these days I'll get this bit of code right..)
  * I coded a little Excel spreadsheet that can be used to build an XML
    server list file, I have included in updated HydraIRC.xml file
    if you want to use the updated list, just cut and paste from the old
    one to the new one.

    Note: Network ID's have changed, make sure you update your buddy groups
    in the prefs!

    If it says "<< Unknown >>" instead of a network name, just click
    Edit and select the network again!
  + Added a fix to show "<< Unknown >>" for buddy group names where
    the network name was unknown. It crashed before because of a
    null pointer reference.
  * Some menu items are now greyed out depending on what MDI window is
    active.

  . Other stuff, that I forgot to note down

29/August/2002 - PM
  Hydra
  * Tweaked DCC Transfer status for completed files (shows average cps in []'s
    and shows total time taken in []'s)
  * Added DCC Chat receiving
    Currently it auto-accepts, prefs later..

    Information:
    You can hardly use any /command in DCC chat windows, more will become
    active later.
    DCC's are tied to servers,  killing a server windows kills the dcc windows
    related to the server instance.  DCC's are tied to a server for good
    reasons (like reconnecting, which will come later) so don't complain, even
    though a dcc tcp/ip connection is not directly related to a server.
    Pressing Ctrl+Shift+S (channel/show server window) works for DCC windows
    too,  the menu item will be moved to the view menu at a later date.
    There are new notification events for DCC Chats



26/August/2002 - AM
  Hydra
  + Disabled the docking framework's processing of WM_SETTINGCHANGE because
    of a problem with locking a windows xp session, doing this may have
    other side affects, let me know if you find any.
  + Fixed an issue with timestamps and output window control code stripping
    and the timestamps always being shown in black for every second or more
    lines of text.
    Also, in fixing this, another small speedup was done to the CMsgView::Put
    function.
  + Changed the URL from www.Hydras-World.com to http://fly.to/HydraIRC, this
    will change again when I get enough money to register www.HydraIRC.com
    and www.HydraProductions.com.    * * * Donations accepted! * * *
  * Created a nice color scheme for Light On Dark, it's in colorschemes,
    see the "Misc" section in Readme.txt for details.

24/August/2002 - AM (v0.309)
  Hydra
  * Added new pref: "Minimize To SysTray" default: FALSE
    To change the default go into prefs click ok, then quit
    HydraIRC then edit the default_profile.xml file.
    Or use the /SET command.

  . Tried the XP like menus from viksoe.dk, it has problems, removed again.

23/August/2002 - AM
  Hydra
  + All CTCP requests are logged to the server window, it was supposed
    to do this before, but I put the Printf() in the wrong place!
  + Added flood control, for ALL client -> server communication.
    Added a new pref: "Flood Control Threshold" Default: 5, Max: 15
    To change the default go into prefs click ok, then quit
    HydraIRC then edit the default_profile.xml file.
    This means that if you send more than 5 lines of text per second
    flood control is activated and then any new text that is to be sent
    is queued.  The contents of the queue is then sent to the server
    at a rate one line per second.

    Having internal flood control is good for 2 reasons:
    1) you won't get disconnected by the serverif someone does a ctcp
       against you
    2) It makes it harder to use HydraIRC to do flood attacks
       because the setting has a maximum value! So they can only send
       15 lines before flood control is activated! heheh.
       They can still do a distributed attack using multiple server
       connection to the same network/server and direct them at you
       though. :(
  * Added "Channel\Show Server" + hotkey, so that you can easily
    show, and then hide the server window for the current channel.
    Ctrl+Shift+S to show it, then Ctrl+Shift+H to hide it.
    Great for looking at whois output and the like.
  + The repeat reconnect event item wasn't being removed when a connection
    was actually made!  If you forced a reconnect to the same server
    or a different server, it would conenct, then reconnect when the
    reconnect delay timer was up!
  * Tweaked channel window scrolling further, maybe a touch less
    flickering now.
  * Added four more color items that you can configure:
    "Item_userlistbackground" default: #FFFFFF (white)
    "Item_userlistnormaltext" defualt: #000000 (black)
    Note: these colors are only applied to the CHANNEL WINDOW's userlist.
    and NOT the global user list.
    "Item_textinputbackground" default: #FFFFFF (white)
    "Item_textinputnormaltext" defualt: #000000 (black)
    Note: these are applied to Server AND Channel windows.


23/August/2002 - AM (v0.308)
  Hydra
  + Finally managed to track down the nick completion crash bug
    It happened if you pressed tab when the cursor was in
    the middle of two (or more) spaces. e.g " | ".
    It wasn't actually a bug, it was a debug assert that compared
    the length of the buffer to the buffer allocated which I
    sometimes add in on complicated string mangling routines
    just to make sure everything is going ok, it turned out that
    the new string was one character shot, because the 2nd space
    was getting converted to a null terminator and wasn't be added
    to the new string.  This is also fixed!
    HydraIRC is 100% stable again now!
    /me is happy - reproducing the cause of this problem was
    hard to do!
  + User lists counts didn't decrement when someone left a channel
  * Large speedup on text output, just defering window redraws
    till later, makes ahuuuuge difference.
  + Stopped window flicking on showing hidden child when child
    windows are maximized
  . Moved Show/Toggle/HideWindow to CChildCommon instead of
    IRCCommon, they're more appropriate there, but now it's
    gonna be more work to get plugins to show/hide a window.
    Still, other code is now much improved elsewhere, so it
    a small price to pay. :)
    This is in preperation for DCCChat windows, which will
    also be inherited from CChildCommon.
  * Added Hide Active Window to the Window menu, hotkey: ctrl+shift+h

21/August/2002 - PM (v0.307)
  Hydra
  * Added /SET command, you can use the /set command to set any
    preference that appears in the <misc> or <colors> sections
    of the profile file. (profile_default.xml, for example).

    If you use this command then you will be informed in the current
    window when a preference was or wasn't set.

    An example usage is:
    /set default user list width=125
    /set misc/Send Keep Alive Messages=true
    /set misc\Remove Old Notice Log Items=false
    /set colors/color_red=#ff0000
    /set colors/color_green=#00ff00
    /set colors/color_blue=#0000ff
    /set colors/item_notice=#7f7fff

    Note: There must be NO spaces either side of the "=" sign.

    Note: Some preference items will still require the current channel
    or server window to closed before they take affect,  but this is
    still better than having to restart HydraIRC completely

    Note: If you make changes to the prefs, they are NOT saved until
    you go into the prefs and click "OK".
  * Added Speed and ETA columns to the transfer window.
    The items in ()'s are based on overall transfer time, the
    items outside of the ()'s are based on the last 10 samples
    of transfer activity and give a better indication of the current
    transfer speed and ETA.
    When a transfer is complete, it shows the time taken since
    the last resume point.

    Question: do you want both ETA times ?

  * CTCP messages come up in a different color (purple)
  * PRIVMSG's now come up in a different color (cyan)
  * Colors are now saved to the prefs in a <colors> section.

    See profile_default.xml

    Knock yourself out! :)

    If you come up with any good color schemes that you think should
    be included in future distributions of HydraIRC then please e-mail
    me the <colors> section from the profile.

21/August/2002 - AM (v0.306)
  Hydra
  * Channel and server windows scroll to the end after a delay if there
    is new text in the window since the user scrolled the window up.
    Added a new pref: "Inactivity Scroll Delay" Default: 60 seconds
    To change the default go into prefs click ok, then quit
    HydraIRC then edit the default_profile.xml file.
  + A /ctcp SOUND command, with no parameters, directed at a HydraIRC
    user would crash the client. - Fixed!
  + Clicking on server items in the taskbar wouldn't show the window
    this was introduced when the IRCServer class was inherited from
    the CListener class. - Fixed!

19/August/2002 - PM (v0.305)
  Hydra
  * Changed download status to show xx.xx xB  (e.g.  10.24 MB, or
    433.12 KB)  This makes for a more readable transfer status list
    It also says "Recevied" or "Sent" depending on the direction, rather
    than "Transferred".  Saving yet more space!
  + MDI windows are now hidden, when the minimize button is clicked,
    rather than minimized, then hidden.  This should help with the weird
    MDI maximisation bug.
  * All the API calls in PLUGIN_API_CMDPROC are now used by the core
    and fully documented, see API_Docs.htm
  + when reordering MDI tabs, and the new tab position is off-screen, the
    tabs needed to be scrolled so that appears on-screen again.
  * channel monitor font is now configurable
  * Added a new pref: "Cross Network Notice Forwarding", this can be
    combined with "Channel Member Notices Only" to filter out must of
    the crap that you don't want to see.  It defaults to TRUE.
    e.g.  if your active window was a channel window on network B
    then someone in a channel you were in on network A sent you a notice
    message, depending on the above two settings you'd receive it, or not.
    likewise a different situation would occur if the user on network A
    was NOT in a channel that you were in.
    To change the default go into prefs click ok, then quit
    HydraIRC then edit the default_profile.xml file.
  * If the active window is a server window then privmsg's can now be
    forwarded to that.  (Previously privmsg's were only shown in the
    originating server's window and the active window, but only if it
    was a channel window)
  * Timestamps appear in a different color, this color is configurable
    but not yet ;)
  * Added more function to the PLUGIN_API_BASIC API.
    HydraIRC_GetChannelInfo() and HydraIRC_FreeChannelInfo()
  * Added lots more example code to the test plugin, including:

    Channel Commands:
    /KICK, /MODE, /TOPIC

    Global Commands:
    /J (Alias for "/JOIN")
    /Q (Alias for "/QUIT")
    /WHOIS
    /NS (Alias for "/PRIVMSG NICKSERV")

    Updated the Readme.txt to reflect these changes.
  * Added /RECONNECT
  . Good news!, it's been running nearly 16 hours now, and no
    crash yet,  and it's been download files from multiple users
    with 15 channels and 6 servers.  The log file grew to a nice 33MB
    file :)

  - Applied the following patches;

  ButtMonkey
  * fix for the double click problem in the server connect dialog box


19/August/2002 - AM
  Hydra
  * Added output file logging, in a release build config logging is
    configurable (on or off), in a debug build it gets logged no matter
    what.  The log file is created as "HydraIRC_Log.txt".
  * Output window text is now colorized, red = error, yellow = warning,
    green = information,  anything else = unknown.
    This information is also saved in the log file.
    To enable this, all sys_Printf() calls were changed and now have
    an additional parameter.

  . Hmm, I managed to crash HydraIRC yesterday so I've now added output
    logging.  However, the version that I managed to crash was an
    in development version, still, it was unexpected.
    With output logging it's easier for me to find possible causes of
    crashes.

18/August/2002 - PM
   Hydra
 * Download progress percent never says 100% while still downloading
    Even on *reeeeeeeeeeealy* large files.  (Suggestion by Rovastar)
  * Added CPluginManager::GetFunction() which is really cool function
    for getting the the first, or next function pointer of a particular
    function in a plugin given the API number and a table offset!
    Having this function will greatly reduce the code size in the future
    when more calls to external functions will be made.
  * Did another speedup in the text display window,  we were clearing
    the entire buffer (8k!) for each line of text, and for each control
    code that was processed.  (so lots of colors = slow)  This is now
    not done any more, and the buffer is null-terminated as it is built
    which results is much less cpu usage.
  - Found some bugs, documented the steps to reproduce as much as
    possible, see todo.txt.

17/August/2002 - AM (v0.304)
  Hydra
  + Fixed incorrect status display on non-resumed transfer.

17/August/2002 - AM
  Hydra
  * DCC File Resuming is done, tested, seems to be working fine,
    even with multiple resumes to the same file.
  - Optimized the transfer window refreshing code.

16/August/2002 - PM
  Hydra
  + Went over the server socket data receive code with a fine toothcomb
    and found a couple of rare cases where a overrun might have occured
    this are now fixed.
    I also re-wrote a couple of routines so they don't do as much
    indirect addressing (e.g buf[x++] is now *bufptr++)
    Also did another speedup with the input buffer
    we now don't ZeroMemory that on each line of text we get, thus
    speeding up things even more!
    And finally a tested everything with a 128 character buffer instead
    of the usual 8k buffer to force buffer overrun-cases, there were
    a couple of instances where HydraIRC wouldn't process some received
    input, now eveything checks out ok.
    Dropped the 8k buffer down to 513 bytes, in compliance with RFC2812.
  + the version in the .EXE files were wrong, was "0.0.0.x" should have
    been "0.0.x.0", x = 304
  + Added an EV_TICK event that gets sent to all listeners every second
    useful for things like status/gui updates...
  + Added EV_TICK processing to the TransferManager so that it can
    check for failed transfers.

16/August/2002 - AM
  Hydra
  + I think I've finally fixed the exit problem, the cause was
    having a server window open with channels open too, and exiting
    HydraIRC with the server window as the active window.
    The order of destruction and queue message processing is
    completely different in this case,  took about 4 hours to
    debug!  (Bleh!)
    We now post an EV_DIEDIEDIE message to all listeners
    when the user says it's ok to quit, and then we repost
    a WM_CLOSE message, and keep doing that until we can quit
    This way we never get stuck in a loop like before.
    It's just really annoying how you can't kill a tabbed
    MDI window without lots of messages having to float around
    first. /me sighs in the general direction of Microsoft.
  - DCC Received files now go into the "\Downloads\" subdirectory
    of HydraIRC's folder.  SO MAKE SURE IT EXISTS.  If it
    doesn't don't worry, you'll just get a nice little "Can't create
    file" status on the transfer queue.
  * Added CTCP CLIENTINFO support
  * Added CTCP TIME support
  . Right, it's light outside and I've not slept yet..  Off to bed...

15/August/2002 - PM
  Hydra
  * DCC File receiving!
    The files are saved to the current directory.
    Added the code for the transfer queue window too, works
    nicely!
    Note: DCC resume not supported yet
    Note: DCC sending not supported yet
    Note: DCC XMIT not support yet
  - A few code cleanups and optimizations.
  - Removed a ZeroMemory() on the socket buffer.
    This should speed things up and cpu usage at the same time
    we now just null terminate it, hopefully I got
    all my +1's and -1's right when dealing with buffer sizes
    NOTE: if there's any instability in the app, this is why.
  * Added /CTCP for sending CTCP messages
    e.g.
    /CTCP Hydra SOUND Long Loud Blip 1.wav
    /CTCT Hydra VERSION
  * Added CTCP VERSION support
  * Added CTCP PING support
  * Added CTCP SOUND support
    If a user types in "/ctcp Nick SOUND wavfile.wav" then
    HydraIRC will play any sound file in sounds/ that has the
    same name as the one on the command line!
    NOTE: It's limited to playing WAV files, as the PlaySound()
    function in the notification manager is limited to WAV files.
    This need to be able to be overridden via plugins.
  + IdentD socket instance wasn't destroyed.

15/August/2002 - AM
  Hydra
  - Removed the current method of remembering userlist group
    expanded status,  it worked, but was no good for syncronisation
    with the global user list.
    I will implement something that uses the EventManager messages
  - Added a couple of new functions to the event manager
    and documented the header file.
  - Made it so that MDI tab buttons are MRU ordered!
    Note: HydraIRC-TabbedMDI.exe only!
    Added a new prefs item, "ReOrder MDI Buttons".  Default: TRUE
    To change the default go into prefs click ok, then quit
    HydraIRC then edit the default_profile.xml file.

14/August/2002 - AM (v0.303)
  Hydra
  + Clicking an MDI tab of a hidden window wouldn't unhide it.
    Note: HydraIRC-TabbedMDI.exe only!
  + Removed some redundant ScrollCaret() calls, text output
    might be just a touch quicker now.
  * Added server keepalive messages.  This helps detect
    socket connection errors and makes HydraIRC reconnect
    when it detects the link was broken.
    Keep alive messages are just irc PING commands sent
    to the server, which are echoed back to the client
    as a PONG message.
    Added two new prefs items, "Send Keep Alive Messages"
    and "Keep Alive Delay".  Defaults: TRUE and 60 respectivly.
    To change the default go into prefs click ok, then quit
    HydraIRC then edit the default_profile.xml file.
  * Added a PONG handling and filtering so that the server
    window doesn't get filled up with output from the
    keepalive PING's
    Added a new prefs item, "Show Pongs", Defaults: FALSE
    To change the default go into prefs click ok, then quit
    HydraIRC then edit the default_profile.xml file.

12/August/2002
  Hydra
  - Possibly fixed (i hope) the 100% CPU usage at exit with
    MDI windows open problem which was introduced in v0.300
  * Added group counts to user list group names, default: ON
    To change the default go into prefs click ok, then quit
    HydraIRC then edit the default_profile.xml file.
  + First item in global user list is shown when switching
    channels.
  + User list does not jump around when people join channels
    when the user list has a vertical scrollbar.
  + moved window state prefs to a different registry subkey:
    HKCU\Software\HydraProductions\HydraIRC\WindowStates
    you *only* need to remove this key (WindowStates) when
    trying out new versions for the first time, unless this
    file says otherwise.
  * Added a very nice Event Manager to the core (finally,
    this was long overdue) which can now handle private,
    timed, repeat timed, and broadcast events.
  * Made server reconnects use the timed event manager.
    added two new prefs items, "Reconnect Retries" and
    "Reconnect Delay".  Defaults: 10 and 10 respectivly.
    To change the default go into prefs click ok, then quit
    HydraIRC then edit the default_profile.xml file.


11/August/2002 - (V0.302) - PM
  Hydra
  * Changed the look of privmsg's *nick* instead of <nick>
  * Notice messages from other networks were displaying the
    wrong server name.

11/August/2002 - (V0.301) - AM
  Hydra
  * The plugin manager works 100% now, and I've implemented
    a simple /uptime command to demonstrate a simple use of
    the two API's.
  * Started documenting the API's.
  + Channel tabs were not removed when disconnecting from a server
    that still had channels open.
  + Set splitterbar back to 3, i forgot to reset it in v0.300
  + Added some new icons! (made by me..)
    Hope you like them, now hopefully no-one will say it looks
    the same as bersirc.
    p.s.  if you're wondering why the icon is pink, it's because
    my system tray is full of icons, and to make it so I can pick the
    right program quickly I made the HydraIRC icon a color that
    was not in use, and one that would stand out.  So pink it was!
    That, and girls like pink and there's not enuff chicks on IRC
    lol!
  * Privmsg's and notices are broadcast to the active channel
    if they active channel is on a different server to the
    one that the message/notice was received from, the message
    is prefixed with originating server's name.
  * Defined a color (hehe, pink) for BIC_QUERYTEXT
  * Added a notification event for BIC_NOTICE (NE_NOTICE)
    You can now configure sounds for notice events.
  * Output and Channel monitor windows scroll to the end when they're
    resized.
  * Wrapped lines of text are now indented.
  * Possible to double click URL's in the output window and the channel
    monitor windows now.
  + Consolidated some text window duplicate code relating to doubleclickable
    url's and clipboard copying.
  * Un-recognised server output is displayed in the output window as
    "DEBUG: ...", but in the server window it's displayed as a notice
    however, it's NOT handled like a notice (because it's unrecognised...)
    e.g. no events are triggered.
  * Added timestamps, defaults to OFF.  To change the default go into prefs
    click ok, then quit HydraIRC then edit the default_profile.xml file.
    Use "/timestamps ON|OFF" to toggle in server/channel windows.

  . too tired to continue today, but got lots done.. nn all!

9/August/2002 - PM (v0.300)
  Hydra
  + Wow!!

    I've updated the codebase to use the new version of the WTL Docking
    windows source code, we now have auto-hiding docking windows
    and windows with vertical titlebars.

  + Missed one combo box size issue for W2k/w98 users.
  + When "Add" is clicked in the user identity dialog the
    form contents are validated and the OK button is now enabled/disabled
    accordingly.
    Also, the contents of Nick control is now selected after add is clicked.


9/August/2002 - AM
  Hydra
  + Asterisk notification sound is now played correctly, I misspelled
    "Asterisk" as "Asterix".  I'm sure I watch to many cartoons.. :)
  + Made nick completion more robust, today I actually managed to crash
    HydraIRC for the first time in about 4 weeks!  Still don't know the
    actual cause, but it was just after an alt+tab, then "ib", then "<tab>"

  . Had way to many beers today.

5/August/2002
  Hydra
  * Added a very versatile PluginManger to HydraIRC.
    The plugin manager takes care of:
     - Looking for plugins
     - Loading plugins
     - Handling plugin API requests
       (e.g. I am a plugin of kind A, or I need a plugin of kind B)
     - Checking API versions (just function table sizes for now)
     - Checking and resolving API requests
       (e.g.  you can have two plugins that each provide an API
        and they both can require the other's API!)
     - Core plugin interfaces
     - Displaying plugin resolution information
     - Displaying plugin activation notices
     - And just being totally great. :)
    Also of note:
     - A single DLL file can implement as many API's as it likes
       (e.g.  it could be an User Input Command Processor, as well
       as a Server message processor, and a theme processor, etc..)
    Currently Defined API's are:
     PLUGIN_API_BASIC (Core Provided)
      - allows the plugin to write to the output console
        (and nothing else, yet..)
     PLUGIN_API_CMDPROC (Plugin Provided)
      - processes commands that a user types into a channel/server's
        text input control.

    Notes:
    now that a plugin manager exists, it's just a simple (!) matter
    of creating an event manager to pass events to plugins and
    then defining the events and plugin API's.

    We will even have an event chaining plugin, so that a DLL can
    get a first look at an event, and then handle it itself (or
    ignore it, and let the core handle it.) - This is how the script
    plugins *could* be implemented.


4/August/2002 - AM
  Hydra
  * Did some work implementeding buddy groups expanded state remembering
    try adding flags="0" the buddy group node in the xml config for now..

31/July/2002 - PM
  Hydra
  + Fixed the dropdown box problems under Windows 2000/98

31/July/2002 - AM
  Hydra
  + MDI windows scroll to the bottom when resized (as in v0.23 and before)

31/July/2002 - AM (v0.25)
  Hydra
  + /quit incorrectly re-connected.
  + lowered socket memory usage
  + cleaned up some debug output
  + fixed duplicate connect attempts when reconecting when already disconnected
  + don't keep posting "***disconnected" to channel windows when a reconnect fails.
  + nick now cycles to the next one when attempting to connect to a server
    that times out and disconnects you when identd is enabled and your
    nick is in-use.  (Hopefully I didn't break the nick-cycle for normal
    reconnects..)

28/July/2002 - PM (v0.24)
  Hydra
  * Added nick completion (use Tab/Shift+Tab) when in a channel window.
  + Added the fix to the TabbedMDI version so that it exits correctly
    (Bug fix provided by the author of the Tab controls)
  + windows don't scroll if the scrollbar position is not at the bottom.
  + Using deferred output buffer processing to make Input more responsive
    (i.e. we don't process the whole output buffer in one go, we do
    it line by line)
  + fixed (i think) the mdi minimize issue, where some MDI windows
    would be really small after hiding then showing them.
  + added stripping Ctrl+M color codes (for topic/privmsg windows)
  + TabbedMDI tabs don't have icons any more.  (they will later,
    previously they were all the same icon, and were not correct)
  . been very busy with work recently, not too many changes. :(

26/July/2002 - PM
  Hydra
  + Fixed null pointer dereference in the XML loading.
    (It would only occur if the inpout xml file was corrupt/invalid..)

18/July/2002 - PM
  Hydra
  + The Global user list is no longer refreshed for every channel
    when reconnecting to a server.
  + Added a prefs for ignoring notices in channels if the user sending
    the notice is not a member of the channel, defaults to TRUE.

10/July/2002 - PM (v0.23)
  Hydra
  + Fixed memory leak in 3rd party docking windows
  + You can disable sounds in notification events now
    (you could before, but there was a bug in the prefs dialog)
  * Items are added to the top of the message queue windows now
    (So that new events appear first, and you don't have to scroll down..)
  + Made the minimum hight of the "Taskbar" toolbar 1 pixel taller
    because the buttons didn't fit in some XP visual styles
  * The text inputviews don't store an empty undo buffers now, this means
    we can handle <no text>, up arrow, down arrow, "some text",
    up arrow, then downarrow to get back to "some text" instead of
    an empty buffer.
    We also clear the undo buffer each time it's re-displayed.
    this allows you to do this: <no text>, up arrow, down arrow,
    "some text", up arrow, then downarrow to get back to "some text"
    then type more text " here", then up arrow, down arrow to
    get back to "some text here".  And so on...
    Now, you should never manage to accidentally loose the line of text
    you were editing!
  + Changed the borders on the message log windows so they're the same
    as the transfer window's
  + Added missing DLL init's for non WindowsXP users
  + About box shows correct version info.
  + Added keyboard shortcuts to some dialogs.
  + Added the missing tooltips
  + Only the channel monitor window stays open when the app is minimized.
    this will be configurable at a later date
  + Splitter width is 2, not 5.
  + Several attempts to fix W98/W2000 combo boxes in the prefs dialog...

10/July/2002 - AM
  Hydra
  * Major GUI rework!
  + Fixed nick highlighting, not configurable yet.
  + Notice messages (received after login) get displayed in the server
    windows again..
  - Other stuff, can't remember...

09/July/2002 - AM
  * Finished off the Buddy Groups dialog (phew)
    WARNING: I wouldn't recommend deleting a buddy group if you're
    connected to any servers.  This has not been tested yet.
  + Fixed the connect dialog not using the right Identity
  - Changed the quit message.
  - Other misc stuff, too tired
  . need sleeepZzzz..

08/July/2002
  Hydra
  * new pref (defaults on) to order taskbar in most recently used order
    (mosueclick or channel select moves the button to position 0)
    This helps when navigating between frequently used channels.
  + Global user list is updated when a channel is rejoined (due to kick
    or server reconnect)
  * Notices are broadcast to channels.
  * Added the rather nifty Notice and PrivMsg windows. (see View menu)
    Lots of prefs for them:
      PREF_bRemoveOldPrivmsgLogItems,
      PREF_bRemoveOldNoticeLogItems,
      PREF_nMaxPrivMsgLogAgeMins,
      PREF_nMaxNoticeLogAgeMins,
      PREF_nMaxPrivMsgLogItems,
      PREF_nMaxNoticeLogItems

07/July/2002
  Hydra
  * new pref (defaults on) to order taskbar in most recently used order
    (mouseclick or channel select moves the button to position 0)
    This helps when navigating between frequently used channels.
  * Added the buddy groups prefs dialog.

06/July/2002
  Hydra
  * Added support for tabbed MDI windows!
    (And then disabled it :(, due to some bugs in the 3rd party code.
    It's #ifdef'd out for now, easy to enable when bugfixed version of
    the TabbedMDI implementation appears.)
  * Added a file transfer window (as a normal MDI window, not a docking
    window), umm.. arrr to MDI or not to MDI... comments please...
    It's not functional at all yet, just tell me if you like it or not.
    Try "View/Transfers" from the menus.
  * Added a simple IDManager helper class, changed notification
    manager to use it.  In readness for the taskbar which also requires
    unique ids...
  * Added the rather nice TASKBAR!
    Try "View/Taskbar" from the menus.
    Note, this took me ages due to a really weird resizing and missing
    chevron.  There's a bug in the platform SDK so I had to make
    the chevron always appear.  No biggie though.
    There is one known issue (issue, not bug mind you..) in that you can't
    Unhide i hidden window using the taskbar unless you a) select it
    again from the chevron menu, or b) click another channel first.
    It basically has to do with the way messages are sent (or rather,
    not sent) if you click a grouped toolbar button that's already
    pressed.
  * Added the prefs item "Monitor Channels On Join", default is TRUE.
    edit the .xml after you close HydraIRC to if you don't like the
    default.
  * The docking window 3rd party source code has been updated to the
    latest version, which fixes undocked windows resizing themselves
    when dragged!
  * Added command line history to all windows, with current-line
    undo (in case you accidentally press up arrow).
  - Lots of other code improvments, some structure changes to
    the classes.

05/July/2002
  Hydra
  + Added tooltip to audio mute (had \r not \n, it still didn't show up..)


04/July/2002
  Hydra
  * Removed the lines at the root of the treeview in the serverlist
    so that the window can be made smaller
  + OOOOOOOOOOPS!  The notifications were broken...
    The event matches were only being made as far as the user group
    not whether the user causing the event was in the group!
    Additionally, depending on what notifications you had set up
    notifictions from other buddy groups were played.
    (basically, anything other than the default notifications were
    broken)
    fixed now..
  * Added MODE changes.
  * When a buddy user changes their nick they stay in the buddy group
    (but, conversly, if a buddy is online using a different nick than
    normal, and then they change back to their normal nick, they won't
    be moved into the group in the user list.  All this will be fixed
    when you can specify wildcards for buddy names when you can
    assign a hostmask to a buddy.
  * Added /Server (happy now ydnar ? ;)
  * Renamed /toggleuserlist to just /userlist
  * Added tooltip to audio mute
  * The user profile is now saved when you click "Ok" in the
    prefs dialog! w00000000000000000000000000000000000000000000000t
  * Prefs dialog is now fully resizable!
  + Mode command should process +o-o mynick mynick (and similar)
  - Other misc changes.


03/July/2002 - PM
  * Improved the speed of the text output, it is also more memory
    efficent, and reduced the size of the .exe a little too..
  * When lines are output, the "Ctrl+O" code will reset the
    color of the text back to the color for the line.  e.g.
    if you have topic lines coming out in green, normal channel text
    in black and someone sets a topic like this:
    /raw topic #test :this is Ctrl+K4,5a bit ofCtrl+Oa test
    the "a test" will appear in green now, instead of black.
  * Added filtering capability to all text output windows.
    Filtering can be done by exclusion or inclusion.
    The channel monitor's filter is now this:
      FILTER_INCLUDE,
      BIC_UNKNOWN,
      BIC_CHANNELTEXT,
      BIC_ACTION,
      BIC_TOPIC,
      FILTER_END
      (BIC means Buffer Item Contents..)
  * The toolbar now updates according to what child window is active
    (e.g you can now see easily if channel monitoring is enabled for
    a channel)
  * Added an audio mute button and menu item and a /audiomute command
    (which toggles the setting..)

02/July/2002 - PM
  Hydra
  + I missed a check after the socket's connect() call, so if a connect
    failed for some reason the user wasn't informed.
    (e.g. previously you could connect to "localhost" and it wouldn't
    give you any error!)
  * Added the ability to reconnect if a disconnect is detected and
    if a connect fails when attempting to connect for the first time.
    Currently this is hardcoded to: ON, 10 RETRIES, NO WAITING.
  + You'd get "Channel monitoring stopped" messages for channels that
    were not being monitored.
  * You can now add buddy group notifications for channel text
    and actions.
    e.g.  so you can configure hydrairc to beep if one of your
    buddies does "/me wonders where everyone is"
    or you can configure it so that it beeps if any of your buddies
    says something in any channel you're currently in.
  + Was missing an init in the edit notifications dialog which allowed
    you to change the event, this is not allowed for existing events
    (so the user doesn't add two events the same, but with different
    sounds/etc)
  + Updated the userlist folder icons to hicolor icons
  + Added missing tooltip for hide all windows. (Zann)
  + Added missing menu item descriptions as appropriate
  * Double-clicking a server in Connect dialog now connects you.
  + When closing a channel window, the next active
    mdi windows's userlist was not reflected in the global user list.


02/July/2002 - AM
  Hydra
  * Added an IdentD (AUTH) server on port 113.
    (you cannot disable or configure it yet)
  + Duplicate columns were being added to the notifications prefs page
    if you viewed it, then another page, then notifications page again.
  * The connect dialog is now resizable, this was a test for the usage
    of CDialogResize WTL Class, at a later date I will make all the
    main dialogs resizeable (like the Tools/Options window..)
  * MAJOR speedup when the global user list is visible and you switch
    to a channel with lots of users.

30/June/2002 - PM
  Hydra
  * Finished off the notification handler.
  * Did more work on the notifications prefs window, it's now less cluttered
    and you can right-click events to hear the sound event.
  * Connect dialog is centered in the main window
  * Connect dialog tree view items are now sorted alphabetically.
  * Moved the Notifications prefs items to it's correct location, under
    "Buddy Groups"
  - Code cleanup of the resource file's defines
  * Added a channel menu.
  + The weird MDI maximized child window bug is fixed.
  * Added a channel monitor! (you can use the menus/toolbar to start stop
    monitoring, or you use the /monitor and /stopmonitor commands)
  * Added more toolbar buttons
    (Tile Vertially, Hide all Windows and Toggle channel user list,
    Toggle channel monitor)
  * Thinner child window splitter bar (Thanks Zann)
  + when you're the last person in a channel, that has a topic, and you then
    reconnect to the server the topic stays in the title bar.  It should
    be reset.
  + we now handle bad channel names (needed to check for dis-allowed characters)
  + if the channel window is maximized when a part command is issued and
    the window closes and the next mdi client window is shown in a restored
    state, not a maximized state. - FIXED
  * Keyboard shortcuts on some dialog boxes.


30/June/2002 - AM
  Hydra
  * Added the ability for (future) plugins to define themselves as a source
    of notifications events so that plugins can dynamically add/remove
    notification events.
    (That is, they can add/remove/manage events independently of the
    main application and other plugins!  This means that your
    gamespy->hydrairc plugin could turn off/enable/change/add events
    while you're away playing quake3, and then set them back when you
    return!)
  . Wrote ICQSSX today (29th) so that I can get some samples to go with the
    next version of HydraIRC. See http://fly.to/hydrairc for details
  . Ok, it's 3:47am now, I'm going to bed...

28/June/2002 - PM
  Hydra
  + Minor bug in the global users list, if you disconnected and reconnected
    the old user list wasn't cleared out first.
  * Added the NotificationManager class (for playing sound files on certain events..)
  + internal topic text was being reset when a channel was joined, tooltips
    and window titles would only show the topic after the topic was set
    while you were in the channel.  Fixed.
  * Servers are not sorted alphabetically in the server list.
  * Added the prefs page for the Notification Manger, currently you can't
    edit the notifications from the gui, but you're welcome to fiddle
    with the .xml file (profile_default.xml)

28/June/2002 - AM
  Hydra
  - In my quest for no memory leaks yesterday I added a free() to the
    nicklist settings in the Tools/Options which shouldn't have been
    there (thanks for noticing it RR2DO2)
  - Channel Userlists is now toggleable!
    you can type in /toggleuserlist to toggle it.
    there is also a new prefs item in the HydraIRC.xml file.

27/June/2002
  Hydra
  - ...Except that it wasn't quite, cos it was reading RGB as BGR because
    the RGB(r,g,b) produces 0xbbggrr, not 0xrrggbb, *sigh*
    Working now though..
  - more code tidyups
  - Whay hey!  Added the global user list code, it now shows the users
    in the channel! w00t.
  - fixed a rather amusing memory leak (in sys_Printf(), which is called
    all over the place to show debug output and console output! oops, hehe)
    While doing this I matched up every malloc() and strdup() to it's
    matching free() to I'm quite confident there are no non-class based
    memory leaks.
  - /me's local echo was normal color, not it's theme color.
  - Added a large speedup and optimization to the removal of nodes
    (and their associated data) from the a user list view.

26/June/2002
  Hydra
  - The identities part of the prefs is complete, prefs are NOT saved though.
  - Added scrollbuffer line count limit. (default 500, changable via prefs)
  - Totally reworked the internal colored text display, no longer do we
    have an array of strings and an array of color values and array of
    color items that map to the array of strings.
    We now just store a bunch of RGB color values, the first 16 of which
    map to the standard 16 colors, the rest are the used whenever we
    need to set the color of something (e.g. a notice message, etc..)
    >> this means we can now use 16 million colors for *everything*. <<
    And, as an extra bonus, HydraIRC now supports other users sending us
    16million color codes (use \004<num>,<num> e.g. Ctrl+Mff00ff,00ff7f).
    So you can now do cool color gradiants and stuff!
  - Added nick-highlighting of the user "Hydra" ;) just to test the
    new color stuff if working right, which it is...

25/June/2002
  Hydra
  - Added automatic channel/server/output window text selection copying:
    When you click, drag and then let go of the mouse button the selected text
    is copied to the clipboard.
  - Hmm, and then I realised I wasn't getting any messages sent to the Channel
    windows, but the server windows were ok.  And then I remembered that I need
    message forwarding, so I added FORWARD_NOTIFICATIONS() to the splitter class
    which was receiving the messages instead of the channel's MDI frame window
    Only took me about an hour to figure that out, bah.
  - Added URL doubleclickabilty :)
  - Clicking in the server/channel output window once focuses the text input
    (phew, cos that was bugging the crap outta me)
  - Some code cleanups.
  - After about an hour or so of playing around server window/tree view focus
    issues I've finally made it so that when you click on an item in the server list
    tree view, the activemdi's text input box is properly focused!
    (phew, cos that was bugging the crap outta me)
  - Sorted out a focus issue when the current server/channel was active and
    maximized and you clicked in the server list tree to hide it.  it would hide it
    and then not activate the next mdi window.  it had an odd side affect that when
    you clicked another window that window would maximize itself.
  - Added the shortcuts for connect and disconnect (ctrl+s and ctrl+d
    respectively)
  - Added a prefs item for prompt on disconnect, defaulted it to TRUE, updated
    the default prefs and profiles.
  - It was possible to some issue commands after you got disconnected, fixed
  - If you try to connect using the connect dialog and there are no user identites
    configured the prefs window opens on the identities page.
  - Connect dialog now uses UserIdentities!  Currently you have to manually
    edit the Profile_Default.xml file to change your identity.
  - Added lots of page initialisation selection and validation code for the
    prefs window.
  - The user identities part of the prefs is almost functional!

24/June/2002
  Hydra
  - Added a couple of new functions to class IRCUser
  - A user's status (voice/ops/normal) is no-longer lost when a user
    changes their nick.
  - A few code optimisations.
  - Fixed a debug assert for this character --------------v (and all high-ascii chars)
    ":BosjeR!~Freggel@D5E0BCC9.kabel.telenet.be QUIT :\o/ ¬o STICK 'EM UP"
  - Added support for servers with a '-' character (Thanks Zann)
  - Moved a few global variables into the prefs class, so that we can work on
    a copy of the items when editing the prefs, and then simply switch the pointers
    on the g_pPrefs pointer to the new copy and then delete the old instance when
    we apply the prefs.
  - Added the code to copy a prefs instance class.
  - Added user identity loading to the prefs.
  - Added an "User Identity" dialog box for editing identities.
  - Sorted out some tab ordering (now that I figured how to actually set it!, hehe)
    e.g. when you open the connect dialog box the cursor is now sitting waiting
    for a server name.
  - Added nick-auto-prefixing and improved the nick cycling features.
    Documented this in Docs.htm, see "Nick Processing"
  - Started adding the code for the identity editing dialog.

23/June/2002
  Hydra
  - Damn, still getting asserts when we get too much data in the buffer.
    trying out a new queued input buffer system.
    Instead of parsing the input data right away, we just copy it, store it
    in a queue and send a message to the child window telling it theres
    data in the input queue waiting to be processed.
    This might also make the app slightly more responsive, and will
    probably help when receiving large bursts of data on slow machines
  - Tweaked the input buffer parsing.  Sometimes another command would be
    appended onto a QUIT message causing the QUIT to be processed twice
    instead of once.  (Actually, it was more general, and not related to
    QUIT commands)
  - Fixed a infinite loop condition that could occur when destroying
    IRCChannel class instances.
  - When re-connecting to a server, channels with open windows are re-joined.
  - Now quit is processed only once, but sometimes we still get a debug assert
    when searching for the tree item group that the user is a member of fails.
    (which should never...)
    Maybe I need some semaphore objects when removing assing the user list...
  - YAAAAAAAAAAAAAAAAY!
    (I fixed the bug..)
    It was actually less complicated than I thought (typical),  the assert
    actually happened when any user who was a member of your buddy list quit
    or left a channel, i just never noticed what group the user was in before.
    (hehe, so you I blame my friends for crashing HydraIRC, lol!)
    The search routine was returning the parent item of the treeview, not the
    child item, and then another search routine was not adding the custom group
    identifier.  So there were actually 2 bugs and one condition.
    Anyway, it's fixed now.  Phew, spent about 4 hours on that one. :(
    Still, at least the code is even more robust now. :)
  - Changed raw command prefix from : to /raw, so to issue a raw command to
    change your nick you would type in "/raw NICK mynick"
  - Added /newserver (alias /nserver)
  - You can now have global, cross-network buddy groups.
    (However, groups with the same names as non-cross-network buddy groups
     are not merged, yet)
  - Added /msg
  - Added /nick
  - When a users changes their nick (or you change yours) the changes are reflected
    in the user list.
  - Made the text color routines process Ctrl+K410 correctly, most
    other clients will fuck that code up.  The output should be
    "10" in red text, but they display it as "0" in black.
    See docs.htm for more details.

22/June/2002
  Hydra
  - Buddy groups now work, define your own groups in the Profile_Default.xml file!
  - Added topic updating, topics are shown in the channel window titles now and
    serverlist channel tooltips.
    (color and style codes are removed from tooltips and window titles)
  - Was getting a Debug assert when removing users from the userlist.
    This was in the log:
    Received: ":Q!TheQBot@CServe.quakenet.org QUIT :hub.uk.quakenet.org CServe.quakenet.org"
    Received: ":Q!TheQBot@CServe.quakenet.org QUIT :hub.uk.quakenet.org CServe.quakenet.org:chImp!~timvriend@qn-213-73-183-197.quicknet.nl PRIVMSG #ctfpickup.euro :What's Italian for "You raped my cat!""
    Received: ":Q!TheQBot@CServe.quakenet.org QUIT :hub.uk.quakenet.org CServe.quakenet.org:chImp!~timvriend@qn-213-73-183-197.quicknet.nl PRIVMSG #ctfpickup.euro :What's Italian for "You raped my cat!":Sol4r!ziemaar@213-84-158-171.adsl.xs4all.nl JOIN :#ctfpickup.euro"
    Added a critical section object to the channel and used during IRCServer::Parse()
    Added a critical section object to the socket receive buffer.
    Let's see if this fixes it.
  - we now handle server responses for a failed JOIN command
  - we now handle bans.  If you are in a channel and you are banned the channel
    window stays open.
  - you can now toggle child windows by clicking on their respective item in the
    server list tree view window.  (e.g. click once to show if not active, click again to
    hide if still active)
  - raw command introducer changed from ! to : as ! is often used for channel/bot triggers.
  - added cut/copy/paste support to channel/server text input windows.
  - added keyboard shortcuts for colored text, see docs.htm for info!
    (Ctrl+K - color, Ctrl+N - italics, Ctrl+U - underline,
    Ctrl+R - reverse, Ctrl+B - bold, Ctrl+O - normal/reset)

21/June/2002
  Hydra
  - Color (background and forground), Bold, Underline, Italic, Neverse and Normal text is working now.
    I wrote a small file (docs.htm) that should be enclosed which describes how the output should appear.
    Currenly the keyboard shortcuts mentioned in the document do not work in HydraIRC, but they will soon.
    However, if you were to enter the examples via another client (like bersirc) you can look at the
    correct output in HydraIRC
  - implemented color themes support and used through out.

20/June/2002
  Hydra
  - QUIT properly (when closing server window)
  - PART properly (when closing channel window)
  - fixed Whois (was processing the received info incorrectly)
  - implement KICK processing
  - can't issue commands until logged in
  - lots of join/part/kick stuff, I think it's 100% reliable now.
  - Added the tools/option dialog (with dialog-in-dialogs!)
    Note: The underlying code structure/classes are there, but it doesn't do anything yet.

19/June/2002
  Hydra
  * It's my birthday!  w00t!  Didn't do much today...
  - NAMES output was only showing 1 name per line.
  - Text is correctly aligned to the bottom of the window when the window is resized.

18/June/2002
  Hydra
  - Robustness improvements, more POSSIBLE token parse errors are now trapped
    (even though they had neve happened yet)
  - Improved join command, you can now join multiple channels (even channels that
    require keys, use /JOIN #channel key,#channel key,#channel,...
  - Users are removed from the list when they leave.
  - /QUIT, /PART work
  - NOTICE implemented.
  - Many other minor changes/improvements.

17/June/2002
  Hydra
  - Server communication up
  - Text window buffering/messaging done
  - some commands implemented (/JOIN, /PART /ME)
  - Added a cool output window.
  - shitloads more stuff, to tired....

16/June/2002
  Hydra
  - Added sockets.cpp :)  I can actually connect to a server now.
    (not that i've got send or receive in there yet though, heh..)
  - Did a bit more work on the connect dialog (it can be cancelled!)
  - The child window's text input is now sized according to the font's height
  - Added simple text echoing to the server/child windows
  - More internal messaging done for network stuff.
  - Basics of a command processor in-place.
  - Window re-drawing/scrollbar issues fixed.
  - Maximized MDI cycling issue fixed.  (I broke a couple of days ago)
  - More work on the XML loading routines, more robust now.
  - Fixed an issue with text being inserted intot he buffer int he wrong place

15/June/2002
  Hydra
  - Started implementing the Socket class.
  - Fonts for channel/server windows are setup via prefs
  - Messaging from the text input windows to the channel/server window is now done.
  - User profiles are now loaded from Profile_*.xml files.
    (Currently only Profile_Default.xml is scanned, profile name "Default")

14/June/2002
  Hydra
  - You can now have custom groups of users in a user list! Omg, how cool!
  - Reworked the ServerList to use TreeItemInfo structures, much cleaner now.
  - Clicking minimize on a child window hides the window (rather than keeping a crappy
    small window at the bottom of the MDI area).
  - Created the internal classes for handling users, user lists, groups of users
    and lists of groups of users. (Phew!)

13/June/2002
  Hydra
  - created a custom splitter bar class. (w00t)
  - added a splitter bar to the channel child windows.
  - added the user list to the channel child windows.
  - added lots of methods to the CUserListView class
  - the CUserListView class is generic enough to be a seperate docking window

12/June/2002
  Hydra
  - show windows when corresponding item in tree view is clicked.
  - correctly set the focus when the main app is activated
  - correctly set the focus when the child window is activated.
  - added lots of extra methods to CServerListView
  - added a shell of the user list class.

11/June/2002
  Hydra
  - add and remove channel windows to tree view


09/June/2002
  Hydra
  - Fixed a heap corruption problem, allocaed a string buffer 1 character to short.
  - Added libxml2 to probject
  - server list is now loaded form HydraIRC.xml
  