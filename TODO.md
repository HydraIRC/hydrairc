TODO.TXT
========

*** See the end of this document for the list of Known bugs ***

Website Sutff
=============

* Nothing

Important Things To Do
======================

get Motoko-ch to update the WinAMP plugin to use a .xml file!!!! ini files, bah...

Add an option so that people who've donated can change their quit messages

[09:43] <SlicerSV> just so you know, so that you can do error handling for it, when someone puts a dot in their ident username HydraIRC crashes

add window buffer saving/reloading to all server/channel/query/dcc chat windows

typing /<something> in a dcc chat sends "<something>" to the other person and doesn't echo it locally

Quick Things To Do
==================

option for ignore duplicate transfers (then what?  rename destination file? how?)

<Naelphin> A CTCP submenu would be nice, version/ping/time would be sufficient
<Naelphin> It'd also be nice if the Op commands were suppressed if you are not an op

add /stoplog and maybe /log or /startlog or something

rework command syntax of /ignore and also store date+time

Things To Check
===============

[15:15] <b34nb4||> I just tried to dcc a 2 byte file to someone and it flooded him.
[15:15] <b34nb4||> it just kept sending him the file till i disconnected

[00:53] <hdaalz> Hydra, /ctcp %n still crashes me
--> fixed now?

[13:52:11] <ShAd0\\> dunno if it's been reported yet, but if you enter a channel with a . in the name
[13:52:18] <ShAd0\\> the client blows up ;)
--> can't reproduce here.

Things To finish
================

channel list dialog.
also add Save and Load buttons to it... (and maybe, an option to refresh the topic for a selected channel)

channel properties dialog
-> show bans

all notifications should be able to add items to the eventlog
--> done some,  need to find all instances of g_pNotificationManager->NotificationEvent and 
    check to see if the returned value is used,  if not then it should be checked and 
    then logged to the event log if the flag is specified.

Things to Fix/Reimplement
=========================

fix autotile? (RawSide)

gradient filled docking pane titles

Things To Do
============

 <teb> could you have /onotice echo the text at the channel window? right now it goes to
the server window instead (unless it's just my prefs? don't think so though) .. also, 
same goes for logging your /onotices, they go to the server logfile instead of the channel
           
[13:16] <Excelsior> which reminds me, i noticed your plugin had some code to detect multi processors
[13:17] <Excelsior> HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager

use /links to update the server lists

[20:38] <Hydra> seems to be a problem with the channel properties dialog though
[20:38] <Hydra> as it didn't figure out the channel mode correctly
[20:38] <Hydra> i got disconnected a day or so ago, and when i tried to come back it was +i
[20:39] <Hydra> and then i tried later on that day and it was ok
[20:39] <Hydra> but hydrairc still thought it was +i
[20:39] <Hydra> i need to reset the internal channel mode status when rejoining a channel

ideally, it'd be nice to have some kind of tab-bar per window
and then you drag items from the server list to the window's tab bar

split channel window using excel-like vertical splitters so you can see two
views of a single channel at the same time
also allow different different verbose levels for the top and bottom
as well as being able to turn off channel text and actions for the top window
(so you can see channel+actions at the bottom, and quits, joins, parts in
the top part of the window)

implement cool dcc transfer bars (as seen in DC++ 0.305)

add notes fields to buddies and buddy groups (so you can make notes about users, etc)
--> not so quick, buddy groups needs a rewrite, also see hostmask matching issues
    as per changes.txt for v 0.3.150

fix /positionwindow problems in onstartup causing small text input view
http://www.hydrairc.com/forums/index.php?showtopic=278

some commands implemented in the default plugin could be moved to the core app,
they were just created there to show how to create commands in the plugin.

move /sysinfo to a new plugin

hostmask in query window title

single clicking url still looses input line focus
--> can't easily fix this, need to time clicks and check DoubleClick() ourselves as
    setting focus to the input line between clicks causes the WM_DBLCLICK message
    not to be sent

Need to show scrollbar on info page when it's too small (richedit bug, as usual)

sometimes it's possible for the text input line to become really small, seen it here
once,  has something to do with the app being minimized when a system pref is change.
need to find out how to reproduce.
--> http://www.hydrairc.com/forums/index.php?showtopic=278

Save and Reload all buffers and logs when closing and opening the app
(for channels & servers, url catcher, event, query, notice and privmsg logs)
-> ignore for query and dccchats,  doesn't really make too much sense
-> maybe for queries...

channel list window (with search, sort and filter and favorites)

use masked edit controls in the gui instead of normal edit controls so that we can make sure that
all data/strings are entered correctly. (e.g.  don't allow commas in the real name field.)

fix EV_TICK events (and other events) when pc's system clock is set backwards while HydraIRC is running
(currently, this can cause a disconnect, because the flood control gets enabled by the PONGS)
--> i thought i fixed this - verify this still causes a problem

crash after 49.7 days of APPLICATION use (not system uptime, but check that too) tick long int limit?
--> confirm fixed?

[22:09] <_vip> hydra, small bug maybe .. when using timestamps [H:M:S] format, the timestamps randomly stop working
--> testing myself, not seen this happen yet
--> have seen screenshots from a few people that have this issue, cause still unknown.

[22:11] <Hydra> read what you said about empty line on command profile
[22:11] <Hydra> onjoin, onstartup or all of them
[22:11] <Hydra> (i'm guessing all/any of them)
--> looked at code, couldn't see how this could happen, need more info.

remove (or fix?!) the shitty 3rd party docking window state manager that is the cause of startup
crashes for some users

re-jig the auto scroll-to-end stuff to use the same scroll-lock features.

check channel window user list width settings
--> why?

remove all MDI tabs, hide window code, taskbar code and re-write.

when resuming a dcc i got this on sucessful completion:
[21:03] ctcp/dcc Receive from nv|fsrv92 of ash_-_candy_(totp)-v1p0n3-nv.mpg Failed: Connection Closed (Received 41334KB of 41334KB)
(the file size was correct, but it still said "FAILED")

hydrairc doesn't check for larger than max length text and save the rest..." (mcorey)
<Hydra> i can't just set the input line to 512 max...
<Hydra> because what if you were sending a long message to lots of people
<Hydra> or were using a big command..
<Hydra> would be for hydrairc to figure out that it's all channel/query text
<Hydra> and split the line and send two messages

add a workaround for the scolling while trying to select text problem
--> done, need to show window locked status somewhere.

support IPv6 addresses in DCC requests:
"[17:42] CTCP/DCC request from darix : SEND HydraIRCGCS00001.png 2001:768:1806:: 2242 13045"

fix minor memory leak when exiting app.

tweak connect/disconnect/new server icons

[01:36] <dedbob> oh, did you get around to changing the "m/privmsg" occurrences to "q/query"?
[01:36] <Hydra> the icons

Weird issue with irc.ufnet.org/betas, channel text was ignored,
probably a dodgy ircd, need socket logging information to fix this. (zero_cool)

<tium> when dcc receiving a file containing "umlauts", the filename is cropped. ie "abcdüefg12345.ext" turns into "efg12345.ext"

when richedit is replaced, allow child windows to have a split view, so that the top of
the window can be used to scroll back in the buffer and the bottom of the window acts
like a normal channel window. like in excel/word when you split a document and see two
parts of it at once.  (fryguy/hydra)

show in brackets after channel now how many different users have been active in a period of time
e.g. #HydraIRC (5) when there are five people chatting.  Also handy for checking active f-serves and bots.

when a channel highlight is triggered, change the channel to a different color in the server list for a configurable duration

show recent queries on server item context menu

add a check to see if /exec is already execing the current command profile to prevent endless loops

the command profiles dont accept special chars (æøåø fx.) (Anders)

set the irc:// control to display "irc://" if the server window that it relates to has *never* been connected...

parse "'s in filenames in CTCP command when being sent files with spaces in

add timestamp context menu items to channel monitor and output views

add "Add to favorites..." to context menus.

logging prefs dialog: Add edit buttons for log file flush interval

command to reload prefs from the file

colorable transfer, notice and privmsg dockers

<netnomad> I'm waiting for /ignore to be implemented before I make the plunge.  ;)

Look at DCC sends and add DCC send resuming,  check our iroffer source.
--> Add buffered file IO for DCC sending

Mattan 16:11: did i ever tell you i'd like to see the neat feature of specifying your own command prefix in your irc client?
Mattan 16:12: like, instead of typing "/msg hydra hello hydra!", i would type "-msg hydra hello hydra!"
Mattan 16:16: that would really make things easier on swedish keyboards, since the slash-sign is typed by pressing Shift+7
--> basically, command prefix preference

ability to specify CONTENT filters for channels. (e.g. join, part) + prefs

ability to specify STRING MATCHING filters (e.g. contains "MyNick") + prefs
(for highlighting,filtering and event notifications)
--> partially done.

channel specific highlighting - currently we have highting for:
network * - <nick>
<network> - <nick>
but not:
<network> - <channel> - nick *
<network> - <channel> - <nick>
e.g: "i want to be notified everytime someone in channel X says something"
or "i want to be notified everytime user y in channel x says something"

i guess one way to do this is to ditch the current regexp parser, then build a string
for each message that comes from the server, like this:
<networkname>:<servername>:<channel>:<nick>:<message>
then you could write a regexp that can match any combination.

option to do "Lock Toolbars" (Rovastar/Hydra)

change color/flash or cycle icon of DCC Sends / Receives in status bar when receiving/sending

change color/flash or cycle icon of connected servers when a server is disconnected.

depending on the ircd, +h means hidden channel or half-op, another case for needing to use the PREFIX ircd message, but
typically, irc.stardock.com uses "IRCPlus/2000" (check) which sends this: 
"irc.stardock.com IRCP2.5 acdefghiknopqrstuwxyzACEFGIJLPRS abdefghijklmnopqrstuvwxyzACEFIKLMOPT"
"IRCX  CHANTYPES=%#&+ MODES=6 NOQUIT WALLCHOPS NICKLEN=31 SILENCE=1000 WATCH=128 :are available on this server"

wildcard/regexp support for /ignore so you can ignore botnets

right click url and go "Highlight in log" so you can see who said the url and what it was they were talking about.

allow all highlight items to log to the eventlog.
--> might be nice to be able to rightclick these events so you can
    go directly to the notification prefs and see what caused it.

"startup" group for favorites, easy way for n00bie autoconnect...
--> would have to add favorite properties dialog for this too
    might be issues with multiple channels on the same server
    would have to do a bit of testing.  (might end up with two server
    windows for the same network).

allow user to select preferred identity/network from edit/add network dialog

add method of choosing identity when using /server /newserver commands

add hostmasking support to the buddygroups

option to change default channel verbosity level (TherapyCl)

DCC Passive Sending
  Instead of saying "i've got file x that's y bytes long on port z for you" we say
  "please open a port so i can send you file x that's y bytes long and tell me the port to connect to"

bold highlights (YorT)

Show a scrollbar (or something) when header is larger than Max Header Lines

add support for /whois <nick> <nick> for global whois support (braska)

timed ignores /ignore +aXL 5
ignores someone for 5 mins

tooltip notes for switchbar and serverlist:
[12:17] <Hydra> e.g.  hold it over a query, and it could say:
[12:17] <Hydra> Nick, Network
[12:17] <Hydra> last message received at d/m/y/ hh:mm:ss
[12:18] <Hydra> last message: Hi there you knob!
[12:18] <Hydra> hold it over a channel and see the topic, network, etc

maybe when you hover over a username in a userlist update the info pane to
show nick, whois, connect time, etc

dialog for first time users to select a default window layout
with little screenshots showing a few different possible layouts.

Add a submenu to the View menu that lets the user select between these layouts.
e.g. View/GUI Layouts/Default|Bersirc|Klient|mIRC|Professional|Basic|etc
-> done a bit of this.  need to add toolbar/taskbar settings too.
   we can add loads of styles very easily...
-> small icons showing the layout on the menu items would be nice too.

[09:52] <eaze\> hey, first of all I just want to say that this seems to be a great client.. just want
to report a bug.. 1) I can't join channels that have a ' character in the name
[09:53] <eaze\> 2) I can't use special characters like Æ, Ø and Å in for example my profile (all of
those characters are in the norwegian alphabet)
[20:08] <Hydra> I can "/join #sdfsdfØØÆØÆØØtest" just fine
--> might be referring to the saving of a "/join ØØÆØÆØØ" in a command profile to the xml file

[21:19] <Hydra> i probably need to restore the app before closing it so i can save the window
positions correctly
--> talking about adding a "close" option to the systray menu.

context menu items on users list for viewing query log files.

add more event log items for DCC transfers and resumes

add highlighting for query + dcc chat windows

high: it is not possible to log channels containing "|" eg. #bla|intern

[23:56] <imajes> and also, fix invite support? :)
[23:56] <imajes> DEBUG: "INVITE" "imajes" "#bmo"

add a reset toolbars option somewhere (to fix toolbar drift issue)

[16:46] <Hydra_> codified, provide me with a list of working servers and i'll add it
[16:50] <codified> http://www.irc-hispano.org/servidores/
[16:51] <codified> and the main/random server i use irc.irc-hispano.org 6667

add the Windows/More Windows dialog and list all hidden and normal windows on the window menu

[09:18] <ZeroForce> Any plan to fix the server/channel list - specifically, the
  name of said window going red when you say something and the window is active?
--> this seems ok except that if you do an action it's triggered.
    looks like we need a BIC_OWNACTION in addition to the existing BIC_OWNTEXT.

when clicking the "Connect" toolbar button for a server that's already connected
instead of asking "server is already connected, reconnect [yes|no]" ask
"server is already connected [reconnect][change server][cancel]"

[17:15] <Aymz> the pm's pop up and cover all three windows up at once and I have to resize them
[17:15] <Aymz> so I have them set to hidden.. but then sometimes I don't realize they are there
[17:56] <Hydra_> Aymz, how about a "New Privmsg" counter on the status bar?
[17:56] <Hydra_> rather, new privmsg/query WINDOW counter
[17:56] <Hydra_> thad's be nice..
--> with a context menu to allow you to jump directly to the new query windows

UPnP firewall support for automatic opening of ports for IDENTD and DCC's (if possible...)

server list context menus

[01:32] <HydraIRC_> can you please type this in
[01:32] <HydraIRC_> /ctcp #hydrairc ACTION test
[01:32] * hdaalz test
[01:32] <HydraIRC_> [01:32] * hdaalz test
[01:32] <hdaalz> how come I don't see it?
[01:33] <HydraIRC_> because it's not processed client side
[01:33] <HydraIRC_> actually. i need to rework a bit of code..
[01:33] <HydraIRC_> for things that the server doesn't echo back
[01:33] <HydraIRC_> at the moment, server.cpp/Parse() parses everything
[01:34] <HydraIRC_> but i need to make it so I can feed server.cpp/Parse() a string so that it then echo's it locally too.
[01:34] <HydraIRC_> (but only for things that don't get echod back by the server)

when not starting HydraIRC with a url on the commandline hydrairc should ask if it should start a second instance.

change the color of the tabs (or change the icons) to show they're being monitored

change the icon in the server list and to show the channel is being monitored.
--> also, replace current channel/query/dcc icons for better ones.

Allow notes for add favorites
e.g.  "found this and that here.."

<raggie> Hydra: is there any way to change usermodes in command profiles?
<raggie> you have to compensate for current nickname though
<raggie> /mode (nick) +whatever
<Gliptic> maybe there's a $nick variable
--> add one somehow...

automatic md5 checksums for files dcc'd

<zann> bout box pops up in the center of the main window
<zann> but confirmation messages - like are you sure to disconnect - etc - dont
<zann> they popup in the center of the screen
--> all calls to CenterWindow in all dialogs use the parent window as a reference,
calls to MessageBox() don't appear to...

merge "options" and "tools" menus

event log menu item: "delete all from this user"

add ability to ignore channel notices from certain users

show timeout countdown timer when offering files via dccs

warn the user if they send a message or open a query window to a user
that is being ignored

dialog for users that only enter one nick that is shown when nick is in use
and the connect attempt is the first connection made to the server

[17:46] <Hydra> give me some quick TODO's
[17:46] <HeadHarve> Channel Monitor always on top option
[17:47] <kjun> initiate DCC chat
[17:47] <HeadHarve> fix funky double texting
[17:47] <`-Anders> input line for the channel monitor
[17:48] <HeadHarve> coloured chan text for notification
[17:53] <HeadHarve> ooh ooh fix for Show Desktop messing up window positions
[17:54] <MrNobody> Hydra: Auto-reject for DCC?
[17:54] <MrNobody> Hydra: Persistent window positions?
[17:55] <HeadHarve> ^^^^ yup what he said
[17:55] <MrNobody> Hydra: Persistent like you can save them between boots.
[17:57] <HeadHarve> fix for psybnc multiple server channel windows not appearing

<sholom> I suddenly realize a feature which HydraIRC needs...
<sholom> The ability to list a custom server name in the server list rather than the DNS name
<Hydra> i am planning a 4 way selection:  network, dnsname, ip and description

<blub> different colors for friends/buddies in the user lists.


<Random832> so, is correcting the output format and output order of incoming ctcps vs outgoing ctcp replies on the todo list?

<`duckman> would it be possible to add a feature where if you do a tab complete after a # character it completes to the name of the channel whose window is active
<`duckman> i have seen that before and i like it
<Hydra> actually i will be going a bit further and adding channel name cycleing, starting with the active channel then going through the rest of the joined channels on the network
<Hydra> and also / command cycling
<Hydra> when pressing tab in a server window, it will cycle through nicks of people you have msg'd or people that have messaged you

<Fiendish1> Hydra: Would you consider adding an option where we can automatically hide the server window when accessing a channel via favorites?
<Hydra> Fiendish1, not specifically for that
<Hydra> but favorites actually create temporary custom command profiles
<Hydra> so if i add a /hidewindow command
<Hydra> i can add that to the temporary custom command profile
<Hydra> and bingo!

<Fiendish1> I just introduced a new user to Hydra. He Loves it but he misses the feature that mIRC had where you could drag and drop a file onto a query window to initiate a DCC file xfer.

<biohaz> will there be an option to display a users prefix infront of nick in channel?
<biohaz> like <@biohaz>

@windows (i.e.  child windows not bound to servers/channels/chats/queries)

<CBG> Hydra:
1) the script:    on 1:text:*PS2*:*: if ($len($strip($1-)) <= 150) { window -e @CONSOLE | echo @CONSOLE  ( $+ $time $+ ) # <<<>>> $nick $1- }
2) the source in the channel posted: <PLA-DCC39> #1  11x [563M] The_Mark_of_Kri_USA_PS2-KALISTO
3) the output to @window:    (17:29:59) #playstation-2 <<<>>> PLA-DCC39 #1  11x [563M] The_Mark_of_Kri_USA_PS2-KALISTO


column mode copy of text, like in UltraEdit
-> needs richedit replacment

change z order before tiling windows so that active/new window always appears top at top left when auto-tile is used

add a pref for adding a "-" to split lines

Usage: /QUOTE LIST parameters
Where parameters is a space or comma seperated list of one or more of:
 <max_users	; Show all channels with less than max_users.
 >min_users	; Show all channels with more than min_users.
 C<max_minutes ; Channels that exist less than max_minutes.
 C>min_minutes ; Channels that exist more than min_minutes.
 T<max_minutes ; Channels with a topic last set less than max_minutes ago.
 T>min_minutes ; Channels with a topic last set more than min_minutes ago.
Example: LIST <3,>1,C<10,T>0  ; 2 users, younger than 10 min., topic set.

support TTN_LINKCLICK

add different gui views to the server list (imajes).  make it changable between
"Tree View" <- what it is now
"Channels" <- list view, alpabetically sorted, option to have network in ()'s after name
"Servers" <- list view, alpabetically sorted, option to have network in ()'s after name
"All" <- list view, alpabetically sorted, option to have network in ()'s after name

add different gui views to the user lists
"Tree View" <- what it is now, combine with a list view, and have seperate columns for "hostmask", idle time, etc..
"List View" <- bog standard list view, option to show prefixes (@%+, etc..)

[02:44] <darix> Hydra: minor bug: in "user identity" dialog: pressing delete should select the select the next field after the deleted

<Kaze> i think the channel monitor is ok, but it doesn't show the network :(
<`-Anders> add a pref for it :P
* Hydra makes a note in todo.txt

option to NOT send "unknown ctcp command" replies (Kez)

show SENT notices in server window and current window (if different from server window)

scan topics and channel text for rogerwilco/teamspeak/quake3/cs server ip's, ports, passwords and channels and
provide way to launch respective app.

strip color codes "No | From Server Responces | From Everywhere"

icon for New Query in the server list

<Kez> going to implement the /ignore command soon?

/url
/run (/shell)
/mp3

socks 5 support

look into these: (#cmp3 on irc.nullus.net)
CTCP/SLOTS request from lillamer : 3 0 8min 33 999 132933 553
CTCP/SLOTS request from MasterRock : 1 0 89m 13 999 41954 51
[00:38:34] <arithon> those are requests from search bots (ie, www.ircspy.com) seeing if you have any download slots open

ctcp/2

proper ctcp parsing

<Hydra_> oops, ctrl+f4 in wrong window
<zann> btw - would be nice to have configurable confirmations for closeing all mdi windows, not just servers
<zann> a real high-end is a timeout on those confirmation dialogs - one number of seconds for all of them - after which the take default action (OK)

/dns nick, or /dns ip

[00:23] <netnomad> Alt-Minus traditionally brings up the Restore, Minimize, Maximize menu for the current window,
kinda like Alt-Space does for the whole program.  It would be nice to see this shortcut implemented.

[21:57] <Kez> hey, is it a known issue that if one types "mode #channelname +l 6" the outcome would be **Kez sets mode +b l6*!*@*

store the IP address of the server upon a sucessful connect & login in the server list.
(int he connect dialog box, add a radio box to select between ip address and dns name
if a server has a valid ip address)

[13:55] <X|-Jabb0r-HIRC> you can't join channels with a : in the name
[13:55] <X|-Jabb0r-HIRC> for isntance: #o:i
-> :'s aren;t valid characters for channel names.  According to rfc spec : is channel
  mask delimiter.  Look up channel masks and implement... ;)

store recent prvmsg, whois, etc nicks in list of nicks for tab completion of server and query windows.

<Fuzzy76> hmf... When issuing "/pass somethinghere" HydraIRC responds "/pass not implemented yet". Shouldn't the correct thing to
<Fuzzy76> do be to send the command as raw to the server? I know /raw works, but I'm used to /pass from every other client... :p
<Fuzzy76> Oh, and emoticons. You can never have enough emoticons. :)

Pie menus, like in Mozilla and Maya

<Gerhard> I want to be able to change the font and flash the taskbar button on private messages always (and not only on new PM)

option to NOT show server windows on the taskbar (as well as options not to show other types of windows too)

add right-click menu to the title bars of undocked (and docked) windows so you can select "always on top" etc..
(and then remember this in the state manager)

channel pref profiles
apply channel prefs X to channels #this,#that and #theother
and then apply channel prefs Y to channels #a,#b and #ciguess
a channel profile would contain prefs like: show user list, monitor channel, colors, logging formats, filters, etc

auto-sizing status bar panes
-> has to be done manually, no WTL methods for doing it..  size of icon AND text has to be calculated.

new icons for transfer windows

use a different icon for normal (non-voice) users when channel is moderated (e.g.  a face
with a picture of a speaker with a no entry sign on top of the speaker)

new API for logging channels and servers, so that a plugin could create HTML/XML output.

remember channels/servers/query windows as well as buffer and command history, and MDI window positions/state
(hidden/normal/maximized and sizes)

show amount of transfers and server connections in window title. "HydraIRC v0.xxx [Servers: x, Channels: x, Transfers: 0]"

[14:46] <SKroWLwrk>  Mouse wheel support
[15:00] <SKroWLwrk>  DCC bind to specific IP address and port range
-> eg, /dccsendconnect <ip>:<port> or /dccreceiveconnect <ip>:<port>

[09:59] <r4vine> there's an option in mirc that when turned on does an automatic whois when someone engage chat (query)
[10:07] <r4vine> it would also be nice if there was the same option for dcc chat and dcc get also

don't try to transfer (send AND receive) zero length files

user identities window isn't resizeable.

server list status flags for when buddies join/leave

rememeber last used identity in the network list, and select the correct identity in the connect dialog
and via the /server and /newserver commands
-> save IRCNetworkDetails->m_IdentityName to the current user profile and make sure it's loaded and that it
  overwrites the valuses in g_NetworkList when they're loaded.

convert "/RAW PRIVMSG"'s aliases to "/PRIVMSG"

IRC URI password processing missing!

a class for storing lists of users, a set of flags and an option expiry time
e.g. for DCCIgnore, DCCAllow, CTCPIgnore

check int max's when editing the misc prefs in the prefs gui

change Query window icon to "Q" instead of "M"

Store the rest of the /exit args somewhere, and use as the quit message.

use SAPI 4.x and 5.x to provide text to speech, define some kind of API for
things that can be spoken, and a way for plugins to change what is spoken.

make it so you can doubleclick channel names in the channel monitor to show
the channel window.

Make sure m_UserModes gets updated when a /whois or /who (??) is done on the
user.  As an op/half op can be +v AND +o/+h. (Anders)

Taskbar Icons pref only affects taskbar items created AFTER the preference
is changed.  The effects are not instant.
-> MainWnd needs to process prefs changes.

NON-buddy notifications on STRING MATCHING filters + prefs
-> i guess we could add buddy STRING MATCHING notifications
   but that would mean adding another combobox control to the add
   notification from that contain a list of defined string matching
   strings.
   i.e. it'd be nice to be able to configure "if friend says 'die scum' then
   do a notification" and "if anyone else says 'die scum' then
   do this notifcation instead"

Pressing Ctrl+Shift+S (channel/show server window) works for Query & DCC windows
    too,  the menu item will be moved to the view menu at a later date.

[16:06] <Equim> auto tile - as in channel windows are tiled automatically
when you minimise/maximise each .. and when you resize the main window etc
[16:16] <Hydra> i don't like the program moving the windows
[16:16] <Hydra> basically, it needs to remember the z-order of the windows before it retiles.

TODO: See if there's a WTL (or MFC + source) class that disables
editing but allows text selection and selected text copying.
(For use in Channel Properties for the topic)

total bytes in/out per second in status bar (and maybe seperate by server and dcc)
and also a little autoscaling graph, like one in getright, also in status bar

transfer status tooltips!  (or just add extra columns, need to show server somewhere)

make it so that each CMsgView window can have it's own set of colors.  This will allow
a channel window on one server could have a different color scheme to a channel on a
different server, or even a different color scheme for each channel/server window!!
--> this is easy to implement, just add a m_Colors to the CMsgView class, and then change
    the COLORVAL macro in HydraIRCCommon.h, then add a bit of initialisation code
    to fill in the m_Color's from the g_pPrefs->m_Colors member and you're done.
--> The only bit that might be a bit tricky is getting the windows to update after
    the user changes the colors in the prefs,  i guess an EV_PREFSCHANGE broadcast
    message would do the trick.  worry about that later though, as at the moment
    they can't change the colors in the prefs.
--> the XML node that colors are in will be bumped down a level, so that it's possible
    to include multiple <colors> sections in a single profile
    like this:
    <colors>
      <colorprofile name="default">
        <item =....
      </colorprofile>
      <colorprofile name="alternate">
        <item =....
      </colorprofile>
    </colors>

    then, we'll later be able to go "/set colors=alternate"
    or something along those lines..

wysiwyg text input

make /newserver set the network information for the new server correctly
--> check the new server name against the list of known servers/networks.

create an importer for the server list at www.netsplit.de
--> semi-manual at the moment, using Excel + VBA

check for DCC ACCEPT's without resumes and warn the user somehow.

API for text forwarding, so that server responces can be checked and sent to the
current window if the plugin wants them to.

Add logging
--> needs a new logging class, each object to be logged can have it's own
    logger, so that channel/server windows can have html logger plugins...
--> Add max log file size and trimming.

add GetNextChildWnd(CChildFrame *pChild) to the ServerList class, for ctrl+arrow key
channel navigation. (and GetPrev...)

make it so the output view can filter sys_Printf() messages, just like we do for
the channels/channel monitor.
--> this is half done now, all printf()'s have a contents flag.

make it so that the user can select to always log all output, rather than just when
the output window is visible.

add notice/privmsg forwarding ignore plugin callback (so i can create a plugin
that can do user and message pattern-matching so that
i can ignore certain messages from ^xtecbot^ on quakenet/#ctfpickup.euro
that sends notice messages everytime a game starts)
note: currently my active channel gets filled up with stuff i don't need
to see.

if logfile cannot be opened for writing add an "_n" to the filename
e.g.  "MyServer_MyChannel_1.txt"

make it so that plugins can provide prefs pages that appear in the prefs
window.

[15:27] <Hydra_> what network supports superops?
[15:27] <`-Anders> irc.betas-online.com
[15:27] <`-Anders> they have all kinds of crazy modes
[15:28] <Hydra_> PREFIX=(aohVvd)!@%=+-
[15:29] <`-Anders> d is the opposite of voice, you cant speak in the channel even tho its not moderated
[15:29] <`-Anders> not the opposite then .. but you get my point
[15:29] <Hydra_> ok, what about a and V
[15:30] <`-Anders> im not sure, perhaps a is ircops, but i have no idea, theyre not exactly standard

Audio Mute Levels:
assign audio mute level to notification events, then add a little down arrow
to the side of the audio mute button so you can select from 1-5 audio mute
levels.  names for audio mute levels can be defined in prefs.  e.g:
(1-Silence, 2-Important, 3-Average, 4-Common, 5-Everything)
e.g.  you would define a "Friends" private message (/msg) buddy group
notification as 2 (important) and you might define a "all networks / default"
buddy group channel text notification as 5 (everything)
Then when you were away, alseep, or whatever you could switch the audio mute to
2 (important) so you would only get notified of important events (in this
case, one of your buddies trying to get hold of you.)

timestamps to use (optionally) a different font e.g. fixed font

customizable timestamp format

get rid of /timestamps command and work it into /set instead

configurable colors for channel names in channel monitor
--> maybe option to recolor the actual channel text too ?

remember user list treeview folder expanded status
-> remove current method, replace with a CList of this:
GroupID,Status
1      ,expanded
2      ,not expanded
etc...
store in registry?  under channels\<channelname>\userliststatus
also: prefs for automatic expanding (or not) of buddy groups

strip leading non-alphanumeric chars from nicks when building nick
completion list so that you can type in the first alphanumeric
character of a nick and then press tab,  e.g.  pressing h then tab
with "{hydra}" and "hydra" in a channel should return "{hydra}"
first, not "hydra"

Only the channel monitor window stays open when the app is minimized.
    this will be configurable at a later date
-> add right-click option to toggle always-on-top mode.
-> store setting in prefs

add java script support, actually, this all comes with active scripting
-> this will probably have to be in the core.

tab completion of / commands
-> each plugin must give a list of commands and aliases to the core.

add a text input view to the channel monitor and allow the user to type in
commands like: "#xpd we got a game yet or what ?" to have the text send to
the channel.
additionally allow the user to press TAB to cycle through available
channel names.  e.g. like nick completion, but for channel names.

highlight the out-of-focus channels that have had text added to them
-> in tashbar, serverlist and mdi tabs

hotkeys
-> hotkeys create events, any listener can handle the event.
   so plugins can handle hotkeys too.

URL grabber

add check button to connect dialog: "Only show favorites"
-> or one better, change the check button to a combo box
   and show as "Show all", "Favorites", "Sucessful connects"
-> for sucessful connects, when we toggle IRCServer::m_LoggedIn,
   store the status in the network list and save prefs at next
   opportunity.

tooltips for taskbar buttons

prefs for channel monitor CONTENT filters

Need to be able to specify wildcards and hostmasks for buddies.

color preferences dialog
-> hahahha, never! you shall all bow down to my default prefs! :) lol zann/imajes

Starting a channel monitor should display the docker if it's not
already displayed. (Zann)
-> but this should be a prefs item too.

Auto-typo-fix(tm):
it might be kinda nice if you could type in a find/replace command
that hydrairc could process on your last command sent, for example:
<Hydra> this is a niec day today we're having tody
and i could type in:
"niec=nice,tody=today" into the text input and press return.
Hydrairc would then EDIT the last line I sent changing the strings.
And then any user running HydraIRC would see the text magically
corrected in their channel view

splitter size pref
-> probably not, as the docking window prefs seem to be hard coded

remember channel splitter positions, or rather, user list width
-> yeah, in registry, channels\<channelname>\userlistwidth

mousewheel scrolling on mouse-over for all items (zann)

optional horizontal scrollbars for treeviews (zann)

window/channel groups.  The ability to add group some channels together and then have them
shown and tiled via a hotkey.  e.g.  when I play Q3 i join #xpd, #xpdprivate and
#ctfpickup.euro so it's often useful for me to see them at the same time.
selecting a the "Quake3" channel group would then hide all other windows, show those windows
in a "restored" state, and then tile them horizontally.

add a /ARRANGE/ horizontally option (e.g. not TILED, but just one above the other..)

need handle connection timeouts
-> ping the server every 1 min.

need to remember channel keys on per-network basis.

make the text output windows bottom justified, not top justified
(so there is never a blank space between the last line of text and the top
of the input box).

need to tell user if they're issue commands while not logged in.

don't do local channel echo if channel mode is "+m" (moderated)
and user is not an OP or Voice (+v) and user is not kicked (or banned)
and if server is not connected.

> scroll windows to the bottom when they get focus

prompt for profile to use when starting app.

save prefs (not profile, that's done already)

add channel and server window toolbars

use EM_SETPARAFORMAT with PFM_OFFSET/200 for output windows

add icons to connect dialog box treeview

user list context menus

server list context menus

text input context menus

channel/server output context menus

add MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo) to child windows
and main window so you can't make the windows too small.

merge users from cross-network buddy groups and non-cross-network buddy groups with the same
group name,  e.g. The Friends group can appears twice at the moment.

store user host addresses in Channel's m_UserList

when copying text from channel/server windows, copy the control codes too.   So we'll need
to parse the selected text, bleh..

need to check for no socket input received for >5 mins
and attempt to send a command (ping) or reconnect if this is the case.

add a int m_SortOrder member to the groups so that the user can change the priority of
user-to-group matching in the case of one user is a member of more than one group
(e.g so you can set a global "bots" group, user "mybot" with a lower priority than
that of a network based group called "fserves" with a user of "mybot" so that when
you join a channel on that network the user "mybot" appears in "fserves" and not
"bots")

Display this in RUSSIAN_CHARSET correctly: "Ïð?ïðîâåðêå ïî÷ò??êîãä?íà ôò?çàéò?ïûòàåøüñÿ"

Inter-process communicaton
==========================

Add bit of code to listen for WM_USER messages from other apps and then process these so that other apps
can send notification messages to HydraIRC.

This will allow 3rd party apps to send away messages, e.g. when you're playing a game, etc.

Messages planned to support so far:

  ACTION <taglist>, 0

    Send a CTCP ACTION message

    tag            argument                             description
    ===            ========                             ===========
    MESSAGE        string                               the message to say in the channel(s)
    DESTINATIONS   list of destinations                 list of channels or nicks to say the message in
    ACTIVEONLY     boolean                              flag to say the message only in the active window

    Behaviour notes:
    if ACTIVEONLY and DESTINATIONS are both specified, ACTIVEONLY takes precedence
    if DESTINATIONS isn't specified the message is broadcast to all channels

  MESSAGE <taglist>, 0

    Send a normal message to channel(s) and/or user(s)

    tag list as per ACTION

  AWAY <taglist>, 0

    Set away mode on one or more networks

    tag            argument                             description
    ===            ========                             ===========
    MESSAGE        string                               the away message
    NETWORKS       list of networks                     list of networks to set away mode on
    SERVERS        list of servers                      list of servers to set away mode on
    DESTINATIONS   list of destinations                 list of channels or nicks to say away the message in
    ACTIVEONLY     boolean                              flag to say the message only in the active window
    NOBROADCAST    boolean                              flag to say that the away message should not be repeated to any destinations
    AUDIOMUTE      boolean                              tells hydrairc to remember the current audio mute setting and then mute it's audio output

    Behaviour notes:

    if NETWORKS and SERVERS tags are not present then the away mode is set on all servers.
    if DESTINATIONS isn't specified the message is broadcast to all channels
    if NOBROADCAST and DESTINATIONS are both specified, NOBROADCAST takes precedence
    if ACTIVEONLY and DESTINATIONS are both specified, ACTIVEONLY takes precedence


  BACK <taglist>, 0

    Un-set away mode on one or more networks

    tag            argument                             description
    ===            ========                             ===========
    MESSAGE        string                               the message to repeat to the destinations
    NETWORKS       list of networks                     list of networks to un-set away mode on
    SERVERS        list of servers                      list of servers to un-set away mode on
    DESTINATIONS   list of destinations                 list of channels or nicks to say the message in
    ACTIVEONLY     boolean                              flag to say the message only in the active window
    RESTOREMUTE    boolean                              tells hydrairc to restore the previous audio mute setting


  Definitions
  -----------

    destination lists look like this:

    <network name>/<destination>[,...]

    Examples:

    "EFNet/#chat"
    "QuakeNET/#Chat,EFNet/#TottyChat"
    "QuakeNET/GameLogBot,QuakeNET/#ctfpickup.euro,FreeNode,#Chat"
    "EFNet/SomeGuy,EFNet/SomeOtherGuy"

    list of networks looks like this

    <networkname>[,...]

    e.g.

    EFNet,QuakeNet

    list of servers looks like this

    <servername>[,...]

    e.g.

    irc.aol.com,irc.quakenet.net,irc.efnet.net

  C++ code
  ========

  IPC_<messagename>
  IPC_TAG_<tagname>

  end taglists by using TAG_ENDOFLIST as the tagid - no further tags after it are processed.


  e.g.

  typdef struct TagItem_s
  {
    LONG TagID;
    void *Data; // typecast as required.
  } TagItem_t;

  TagItem_t myTagList[3];

  myTagList[0].TagID = IPC_TAG_MESSAGE;
  myTagList[0].Data = (void *)"is away, reason: playing Q3 on Jolt CTF, 192.168.1.100";

  myTagList[1].TagID = IPC_TAG_DESTINATIONS;
  myTagList[1].Data = (void *)"GameLogBot,QuakeNET/#ctfpickup.euro,FreeNode/#Chat";

  myTagList[2].TagID = TAG_ENDOFLIST;

  SendMessage(hWndHydraIRC, IPC_ACTION, &myTagList, 0);

Create a HydraIRCSupport.dll file so that 3rd part apps coded in languages other than C++ can integrate more easily.

  RequiredFunctions:
    Result = IsHydraIRCRunning()
    Success = StartHydraIRC(WaitTillStarted)
    TagListHandle = Createtaglist(Tag,Param,...)
    Success = TellHydraIRC(IPCMessageID,TagListHandle)
    DestroyTagList(TagListHandle)

  The DLL will be accessable in:

    <HKEY_LOCAL_MACHINE\SOFTWARE\HydraProductions\HydraIRC\InstallPath>\Support\HydraIRCSupport.dll


Adding DCC Chats
================

define a more custom status
  void CChildFrame::UpdateStatus( void )

allow:  CMD_ChannelMsg (with NULL channel only), CMD_Set, CMD_Nick, CMD_Timestamps, CMD_Me, CMD_ToggleAudioMute, CMD_Raw
disallow: CMD_PrivMsg, CMD_CTCP, CMD_ToggleUserList, CMD_Monitor, CMD_StopMonitor, CMD_Part, CMD_Server, CMD_Quit, CMD_Join
--> need to confirm

update SetNick
--> done???

need to add PFN_PROCESSDCCCHATCOMMAND to plugin api's

Adding Query Windows
====================

need to add PFN_PROCESSQUERYCOMMAND to plugin api's

check all /commands are ok.
--> need to confirm

Memory Leaks
============

These leaks have been researched:

c:\program files\microsoft visual studio .net\vc7\include\crtdbg.h(674) : {33498} normal block at 0x00C13018, 12 bytes long.
 Data: <t           > 74 05 16 01 80 05 16 01 D8 05 16 01

This is an APIDescriptor created by the plugin, this is freed when when the plugin CPlugin
instance is deleted, which occurs when the plugin dll is unloaded.
TODO: We could force unloading of DLL's before the program exits.

Ideas
=====

Prefs window with a tree view for prefs items, when selected
the pane to the right shows the appropriate dialog.

Prefs
|- User Profiles
|- Servers
|- Auto Connect
|- Command Profiles

Servers shown in a treeview, with add/delete/edit buttons

Servers
+ <group name>       Store a GUID for each server group
| |- <server name>   Store a GUID for each server
| |- <server name>
+ <group name>
| |- <server name>
...

double click a server to bring up the Server Options
 - server description (include country code, etc..)
 - server name
 - ports
 - password
 - Attach Command Profile
   [ ] override server's network group command profile
 - Attach User Profile (always overrides any network/global user profile)
 - Priority, selectable from: Preferred, Don't Care, Never Connect

user profile
 - name
 - list of nicks (not just nick + altnick)
 - ident response

User can create multiple profiles (different username, etc..)
these can be attached to server groups, so you can have different nicks
for different networks.

User can create command profiles which can be attached to a server group
so that you can have different onconnect commands for different networks
and different servers.

Auto connect dialog lets you add servers from the server list to the auto
connect list.

Ident server must check who the ident is coming from and then return
the correct ident string so that you can have different idents for
different servers.

URI's for IRC url's:
irc:[ //[ <host>[:<port>] ]/[<target>] [,needpass] ][?<command>=<params>[&...]]
e.g.: irc://irc.network.net/#hydrairc?msg=hello&msg=uri's%20r%20cool

Allow user to create custom groups in the user group list,  e.g. "friends" and "bots"
and then add a user to the groups (via rightclickmenu/addto/"New group"|<existing group>|...)

nick color on specific users.

have an option to show last few lines with your name in them, e.g.
#GtkRadiant: <TTimo> Hydra? you there...
#XPD: <[XPD]Jazz> Hydra
#XPD: * [XPD]Jazz fwaps Hydra

word statistics
  lines of text sent total
  most active network/channel
  words per day/hour/min
  lines of text received total/network/channel

deny deleting of preference items that are in-use by a current server.
e.g. if the user is trying to delete a user identity and they are
connected to a server using an identity with the same id, disable the delete button
or show the user a message when they attempt to delete it.

obtain your preferred nick (or last used nick) after a re-connect with a temporary alternate nick

Processing: ":TAtari!JohnDoe@c-cf2172d5.019-260-76786a11.cust.bredbandsbolaget.se
PRIVMSG #bersirc :how about: Wingdings or Symbols or maybe even Fixed Fonts?"


implement receiving of
CTCP/MYSTATUS <away>|<back>|<dnd>|<etc...>
and sending of to people who are on your contact list to provide an IRC network based IM user-list.

global command line history and per-window history, configurable.

channel output montor window:  shows incoming channel text for a) all channels
b) all hidden channels, c) selected channels
output takes the form of:
#Channel: <User> message
Window can stay open even when app is minimized.

the PlaySound() function in the notification manager is limited to
WAV files.  This need to be able to be overridden via plugins

double clicking a channel should bring up a channel properties window
this window could be overridden by plugins, so that more advanced
plugins can provide their own dialogs instead of the default one.

TODO: API Documentation (bleh)

Audio Events
============

all audio events are configurable for a default, and for a buddy group

Suggestions
===========

Pyyro: Allow users to have different color backgrounds (or text) in the user list.
e.g. bots red, friends green, normal users white.

Ibnull: Build in a bot that scans and catalogs IRC channels on a server and then
lets you download files from the catalog.
- implementation,  join a server in the usual way, then add a bot to the server
which creates an IRC<mybot> class instance, with m_pServer set, create a new
child form (CWTYPE_BOT) and handle accordingly.

Ydnar: have a dockable multi-colum user list view, with sortable headers.
name | host | op | voice | group

MarsMattel: who when and what channel notification window. (Unread
Notifiations window)  just a list of items in a listbox.

Zann: remember open windows when hiding windows and then unhide them if you
click hide again.

[15:19] <TTimo> well
[15:20] <TTimo> - can it remember server / channels and auto start / join
[15:20] <TTimo> - can it show blue highlights on channel names when there's activity, red highlight if client nick is being printed
[15:20] <TTimo> - can it have keybd shortcuts (typically Ctrl+arrows) to move between channels on the server tab
[15:21] <TTimo> - can it have queries support
[15:21] <TTimo> - can it have whois support
[15:21] <farenheit> TTimo
[15:21] <TTimo> mh, I think that's about it more or less :-)
[15:21] <farenheit> is on a rant
[15:21] <TTimo> otherwise, what's working is working great

Code Snippets
=============

Subject: Re: How do I insert text into StatusBar on Wizard generated WTL SDI app

This should work:

// include atlctrls.h at beginning of file

HWND hwndParent = GetParent(m_hWnd);
HWND hwndStatus = FindWindowEx(hwndParent,
                               NULL,
                               CStatusBarCtrl::GetWndClassName(),
                               NULL);
if (hwndStatus) {
    CStatusBarCtrl st = hwndStatus;
    st.SetText(0 /*number of pane you're using*/, "Text");
}


Goals
=====

IRC Client
----------
Features (Updated list is on the website)
* Written in C++ using modern tools (Visual Studio 7)
* Uses WTL/ATL for a Super fast, streamlined and efficent program!
* Dockable windows, MDI client area.
* Event based core (with user definable events)
* Nice looking, intuitive GUI, no skinnable ugly rubbish.
* Supports connecting to an unlimited number of servers
* Reliable reconnects
* Supports private messages
* Supports DCC chat and File transfer
* DCC File resumes with rollback and same-file checking.
* Nick Completion
* FServe functions built in (e.g. auto-requeue on disconnect + more)
* User profiles (username, nick list, etc) can be assigned on a per-server/per-network basis
* Highlighting can trigger events.
* Audio Mute function!
* Command profiles (a bunch of commands that are given to be run after the other)
* Auto-Connect on startup list lets you connect to multiple servers on startup
* Auto Connect Command profiles can be attached on a per-server/per-network
* Auto Join Command profiles can be attach to channels on a per-network basis.
* User list icons denoting ops/voice, etc.
* Remembers nicks on a per-server or per-network basis, can be told to update the corresponding user profile automatically.
* DLL Plugin SDK.

Stuff For V2 - Maybe, dunno, still thinking about it.
=====================================================

Rearrange code and class layouts into templates like this:

IRCServer : public IRCCommands
+- Connect
+- Send
+- OnReceive
+- Parse
+- QueueServerEvent
+- OnServerEvent
+- m_MDIWindows[]

IRCCommands
+- CMD_Join \
+- CMD_Quit  + overridable functions.
+- CMD...   /

IRCWindow : public MDIChild
+- OnServerEvent
+- ShowWindow
+- m_MsgView
+- m_TextInputView
+- m_UserListView

IRCWindow's and IRCServer's to run in seperate threads.

So that a bot can inherit from IRCServer, and can implement it's own windows,
commands and event handling.
This is what i meant by "Event based core" which v0.x doesn't have yet, not really

Now that I've a better understanding of how windows apps are written this is
definately the OO design we need.

Notes when implementing scintilla
=================================

make a user preference for SCI_SETBUFFEREDDRAW(bool isbuffered)

Translations
============
Inho 7777 (brasilian translator) cidadao at estadao dot com dot br
<AciD/telefragged> hey btw, if you need translators, i would be pleased to translate it to french :)
[21:18] <Flyoc> If you want a french translation, I'm your man
[21:21] <hobgoblin> well i could take a crack at norwegian;)

Suggested QUIT messages
=======================
"Silly faggots, mIRC is for kids! Use HydraIRC ( URL )"
"you fucken punks try HydraIRC ( URL )"
"Got mIRC? LOL, HydraIRC the all new client - ( URL )"

Fixed Bug Notes
===============

Problem with favorites dialog and in-place tree-view label editing
  http://www.geocities.com/SiliconValley/2060/articles/label-editing.html
  http://support.microsoft.com/default.aspx?scid=kb%3Ben-us%3B130691
  http://www.google.com/search?sourceid=navclient&ie=UTF-8&oe=UTF-8&q=Q130691
  http://www.codeguru.com/editctrl/SubClassedEditControl.html


crash bug when the following strings were pasted
  "http://bonsai.mozilla.org/cvsquery.cgi?module=MozillaTinderboxAll&branch=HEAD&cvsroot=/cvsroot&date=day&who=seawood%25netscape.com"
  "the favorites dlg and loading/saving is working 100% now"
  --> % sign needed to be escaped, current just using /'s instead of %'s

crash bug when really long urls/lines of text
  Tue Feb 11 15:00:19 2003 - INFO - channelmonitorstr after stripcodes() is 543 bytes: "#testtest: <Hydra> http://bonsai.mozilla.org/cvsquery.cgi?module=MozillaTinderboxAll&branch=HEAD&cvsroot=/cvsroot&date=day&who=sspitzer%25netscape.comhttp://bonsai.mozilla.org/cvsquery.cgi?module=MozillaTinderboxAll&branch=HEAD&cvsroot=/cvsroot&date=day&who=sspitzer%25netscape.comhttp://bonsai.mozilla.org/cvsquery.cgi?module=MozillaTinderboxAll&branch=HEAD&cvsroot=/cvsroot&date=day&who=sspitzer%25netscape.comhttp://bonsai.mozilla.org/cvsquery.cgi?module=MozillaTinderboxAll&branch=HEAD&cvsroot=/cvsroot&date=day&who=sspitzer%25netscape.com"
  Tue Feb 11 15:00:19 2003 - INFO - channelmonitorstr after g_pMainWnd->SetStatusBar() is 1 bytes: "?
  Tue Feb 11 15:00:19 2003 - INFO - channelmonitorstr address is 0x010208a0 - calling free()
  Run-Time Check Failure #0 - The value of ESP was not properly saved across a function call.  This is usually a result of calling a function declared with one calling convention with a function pointer declared with a different calling convention.
  --> set max status bar text length to 128 now.

Updating "Use Gradient Filled Panes" doesn't cause a redraw.
-> might be fixed in 108, untested
-> tested ok.

BUG:
  fix crash when leaving large channel:
more info:
Callstack:
LRESULT CWindowImplBaseT::DefWindowProc(UINT uMsg = 16, WPARAM wParam = 0, LPARAM lParam = 0)
LRESULT CWindowImplBaseT::DefWindowProc(UINT uMsg = 274, WPARAM wParam = 61536, LPARAM lParam = 12649514)
LRESULT CWindowImplBaseT::DefWindowProc(UINT uMsg = 161, WPARAM wParam = 20, LPARAM lParam = 12649514)
int nRet = theLoop.Run();
--> this has not happened recently...
--> this has not happened for several weeks...
--> this should be now fixed, see 0.3.116

dcc transfers are being sent the machine's local ip address when the user is
behind a firewall.
-> add parsing of welcome messages and /whois's and grab the external ip from there
   -> done
-> add IP address config option for people behind routers.
   -> done
-> obtain remote IP automatically via a "/raw userhost <nick>" command
   as it *always* returns the correct FQDN or IP regardless of wether or not
   the whois replies use masked hosts.
   -> done

BUG:
logging of two servers with different ports configured to be on different network
results in the log file being created in the wrong folder.
probably to do server's network lookup not taking into account the port number
http://www.hydrairc.com/forums/index.php?showtopic=316

Quotes
======

<CBG> damn bro this is a sick ass program

Un-repeatable crashes:
======================
Windows 2000:
Faulting application hydrairc.exe, version 0.3.59.0, faulting module hydrairc.exe, version 0.3.59.0, fault address 0x00037896.

Windows 98:
HYDRAIRC caused an invalid page fault in module HYDRAIRC.EXE at 0167:00408075.

Known Bugs
==========

Starting a DCC send always causes HydraIRC (and all other winsock apps) to be
disconnected, but a disconnect message is not sent by winsock to HydraIRC.
--> hdaalz has that weird "I get disconnected from server and I timeout" when
    doing a DCC CHAT as well as when doing a DCC SEND
--> imajes has this issue, though not when his machine was brought round to my
    house for testing, this leads me to beleive there might be something screwy
    with the network hardware that both hdaalz and imajes use.
--> they both use Netgear routers.  (why am I not suprised)
--> problem happens on Windows 2000 and Windows XP SP1
--> I will have to try and create a workaround for this issue,  maybe i'm setting
    up the socket incorrectly and the netgear kit is borking on it??  Only thing
    I can think of that might cause it...
    Need to find out where

BUG: atlregexp class is broken, need to find a replacement.
  The full regexp used by HydraIRC is currently "^{((.)* )?}{%s}{( (.)*)?}$"

  Where %s is replaced with the configured highlight stringregexp part.

  "«" and "»" characters cause ATLRegExp to crash, so some code was commented out
  to avoid the crash, but this breaks the regexp routine in some weird ways, still
  it seems to work ok for some regexps but not for others.

  If you want to play with regexps grab my little ATLRegExp program available
  from www.Hydras-World.com
  
  Possible solution taken from here:
  
  http://groups.google.com/group/microsoft.public.vc.atl/msg/b525c84477676c4f
  
  "I was running in the same problem with german umlauts.

  The algorithm seems to be buggy. To word around that, copy the file

  C:\Program Files\Microsoft Visual Studio .NET 2003\Vc7\atlmfc\include\atlrx.h

  to your project path and make the following changes and include it in your cpp:

  Line 637

  <!new                unsigned char* usz = (unsigned char *) sz;
  <!new                size_t u = (size_t) *usz;
  !>old                size_t u = (size_t) *sz;

  and Line 1181

  <! new                               unsigned char uchStart = chStart;
  <! new                               unsigned char uchEnd = chEnd;
  <! new                               for (int i=uchStart; i<=uchEnd; i++)
  !> old                               for (int i=chStart; i<=chEnd; i++)
                                          pBits[i >> 3] |= 1 << (i & 0x7);

  With these changes everthing should work fine.

  Good luck!
  Michael"
  
  Alternatives:
  
  http://research.microsoft.com/projects/greta/
  http://www.boost.org/libs/regex/doc/index.html
  

BUG: channel monitor needs to be ENABLED (bot not actually visible) and then
  focus after restore fromsystray is ok.
  if channel monitor FLOATING, or DISABLED it breaks.

BUG: fix MDI window icon when MDI window is maximized

BUG: fix maximized MDI windows scrolling to the bottom when switching between
  them
--> this happens due to the way tabbing between MDI windows occurs as the current
    windows is restored, the next one is shown and then maximized.  due to the restore
    of the previously maximized active window it receives an OnSize() which
    then requests a scroll to end.  however the mdi window's richedit also
    recevies the onsize and it resizes it self and scrolls things around so even if
    you don't scroll to end on maximized windows the bug is still not fixed something
    even weirder happens instead.

BUG: when the last line of text in an output window contains a url, placing
  the mouse pointer below the url but above the text input box still leaves
  the pointer with a hand icon instead of a pointer icon.
--> this is a richedit bug, need a richedit replacement.

BUG: <jugg> quick bug report:  If you have a channel window maximized
  and do a "Hide Active Window" the window does not get hidden.  You
  can still get to it by doing a CTRL+TAB and also if you unmaximize
  (restore) your current window, the window that you told to be hidden is
  on the screen.  Even though it has been removed from the task bar.
  Everything works fine if the window is not maximized when you
  choose "Hide Active Window".
--> thought i'd fixed that already....
--> ok, to get this bug to occur, do this:
  1) have lots of channels, hide all of them (ctrl+h)
  2) use the server list to show a server and a channel window
  3) make them non-maximized
  4) maximize one of them
  5) attempting to minimize one window makes the other one appear



BUG: if you connect to a server and then do something like
  /join a,b,c,d,e,f,g,h,i,j,k
  and then while it's joining the channels close the app, it doesn't exit
  cleanly.  (something to do with creating IRCChannel classes after the
  server class has been deleted.)
  *sigh*
--> seems to be ok now. (0.3.116)

BUG: need to find away to show the channel monitor window when it's
  undocked and the app is minized, but NOT minimizing to the systray (anders)
  It stays open when you do minimize to systray though.
  ShowWindow(SW_MINIMIZE) hides it for some reason, even when
  WS_EX_APPWINDOW style applied to the channel monitor's window.
  Try removing/changing the parent?

BUG: docking windows leak a bit of memory when in autohide mode
  to reproduce, set the docking window to be autohiding, then restart
  the app then you can trace the leak.

BUG: default window layout is borked!
  Please adjust the gui by dragging all the windows around, see the
  screenshots on the website so that you know what it can look like.
--> should be ok now, though maybe a clean of the registry settings
  between versions should be done.

BUG: user list mode -v handling for ops is broken
  Hydra sets channel #hydrairc mode +o zann
  Hydra sets channel #hydrairc mode -v zann

  user moves from ops list back to normal users

  When you +v the user they're color also doesn't change if they're in
  your friends list.

BUG: missing taskbar buttons if your fonts are too big - make them
  smaller for now.

BUG: When you use a larger or bold font for the server list tree items
  the text of the treeview items marked dirty are not fully displayed.

BUG: crash on no disk space when downloading.
--> more error checks are present, verify it still does this.

BUG: duplicate "Failed" when attempting DCC resumes from some hosts.

BUG: socket already connected when reconnecting using /server after an attempt
  to connect to a server failed.
  Also related, connecting to a non existant server, then connecting to
  another non existing server - but for which an ip address can be resolved
  (e.g. localhost) - results in the error above, but a reconnect does not
  occur.
--> need to overhaul the reconnect, I don't like it.  also need to
  add prefs to give up on a server and try the next one in the network
  as well as check the server name on the /server's command line against
  the server list so we can update the server's "this->m_pDetails->m_Network"

BUG: docking windows are eating keypresses, e.g. alt+t, alt+f
--> happens in the docking window sample too, not my problem. :)

BUG: sometimes, after a dropped connection, disconnect, reconnect,
  channel close and rejoin, your nick can be duplicated in the list.
--> changing servers with the server window activated forces this!
--> additionally, server max nick has something to do with it
    "Hydra" doesn't have problem, but "Rovastar" or "`-Anders" does
    Nick length on efnet is 9.

BUG: sometimes, after a +i (invite) is set on a channel and you reconnect
  to the server, the global user list ends up with 2 of each nick.

BUG: When the scroll position of the channel monitor and output window
  is not at the bottom, and new text is displayed in the window it scrolls
  to the bottom.  After having a look at this it seems the RichEdit window
  is being updated differently to the channel windows (which use the same
  code in CMsgView::Put(), bleh).  The code that is used to scroll the
  window to the bottom is already a work-around for the problem with
  GetScrollInfo(SB_VERT,&ScrollInfo) which is failing when we ask
  for the scroll position of the richedit's scrollbar.
  So, we need yet another workaround, or solution.  If anyone cares to
  take a look...

  Fixing that, will probably also fix this TODO item:
  Scroll windows to bottom after resize
  -> this is done but sometimes, when windows are resized via a tile
     or arrange operation, they don't scroll to the bottom!
  -> need a richedit replacement really..

BUG:
  Selecting text from the server/channel buffer when new text is entered
  into the window causes the window to scroll to the bottom and the selection
  lost.  We need a check to see if we're selecting text, and if we are then
  we need to somehow delay new text being entered into the buffer while
  we're selecting text.
  So it looks like we'll need yet another buffer to store text, but
  at least it's only when we're selecting text, so there's going to be
  any performance issues.
  -> need a richedit replacement really..

BUG:
  Need to update the channels and servers after removing a buddy group
  Notes:  user lists need to handle this in OnEvent(EV_PREFSCHANGED)

BUG:
  Notification Prefs Dialog: new event needs to check to see if that
  event is currently defined and if so remove from the list
  of available choices for a new event.

BUG:
  this line was truncated in the Notice Message Log window
  from this:
  "[Logon News - Jul 08 2002] Attention Users! DO NOT visit websites displayed to you via channel on-join / on-part spam as a large majority of these websites will infect your system with a bug! Please if you are unsure and wish to visit a website, check with the staff in #criten | Don't Risk IT! [ Network Administration ]"
  to this:
  "[Logon News - Jul 08 2002] Attention Users! DO NOT visit websites displayed to you via channel on-join / on-part spam as a large majority of these websites will infect your system with a bug! Please if you are unsure and wish to visit a website, check with th"

BUG:
  MDI icon's for maximized windows are fudged. (aways shows server icon instead of query, etc..)

BUG:
  Need a workaround for this:
  PropertyList.h line 234
  HPROPERTY FindProperty(LPCTSTR pstrName) const
    // BUG: Doesn't find it if it's collapsed. Hmm, what to do?

  Basically, you can't switch to other pages or click OK in prefs if any of the nodes
  on the "Misc" page are collapsed (have a '-' next to them)

  This is 3rd party code however.

BUG:
  PARSE: "<dad3v1l> 13(05file servers online13) trigger: 05w00t mp3's!13. snagged: 0549.81gb in 986 files13. min cps: 0529.3kb/s13. record cps: 05153.0kb/s by g-nome13. online: 050/513. sends: 050/113. queues: 050/513. accessed: 052730 times13. note: 05-=-=-=- !mp3's - enjoy! -=-=-=-13. 13«~05{13polaris se05}13~»"
  with regexp: "^{(.)*?!\a?}{hydra}{!\a(.)*?}$"

  causes crash in atlrx.h, commented out some code to avoid crash, unknown
  side effects.

UNFIXABLE:
  typing in "DCC CHAT" into a channel window causes a disconnect when using kerio personal firewall 5.0.2's irc packet inspector

UNFIXABLE:
 "[2003-02-10 09:39:48] *** dewzer quit (test över)"
                                                 ^ this caused line spacing to be borked...
     "[2003-02-10 09:47:58] <Switch_> in chat window, i notice that sometimes the line spacing is wider after some time. Anyone notice that?"
--> Had a look at the line spacing issue, doing GetParaFormat() returns
    the paragraph formatting, but "PARAFORMAT2.dyLineSpacing" is still
    0 even when the paragraph formatting has changed.  Look like an
    internal richedit problem,  can't do anything about this bug.
    Needs a richedit replacement to fix.

bug: crash 0x0048dd99, 0.348-release
--> probably fixed, see 0.348 -> 0.349

bug: crash <YorT->  (0x80000003) occurred in the application at location 0x0046ce62, 0.348-release
--> probably fixed, see 0.348 -> 0.349

bug: fighter: 0167:00408065, 0.348-release
--> probably fixed, see 0.348 -> 0.349

fix using "Unknown" as a nick
--> possibly fixed, need to verify, see 0.3.114 -> 0.3.115

[11:18] ctcp/dcc Receive from zanny of Profile_Default.xml Failed: Connection Closed (Received 9KB of 7KB)
[11:19] <zanny> [03:16] CTCP/DCC request from Hydra : RESUME Profile_Default.xml 3072 9244
[11:19] <zanny> [03:16] ctcp/dcc Send to Hydra of Profile_Default.xml Failed: File read error (Sent 9KB of 7KB)
[11:19] <zanny> (Sent 9KB of 7KB) - heck would that mean ?
[11:19] <Hydra> send again
[11:20] ctcp/dcc Receive from zanny of Profile_Default.xml Completed (Received 7KB of 7KB)
[11:20] <Hydra> i had a file in my downloads folder already
[11:21] <Hydra> which was 10k (!)
[11:21] <Hydra> wait, 9244 bytes, rounding error in explorer
[11:22] <Hydra> destination size was already larger that file to receive
[11:22] <Hydra> i need to check that
--> done, not vefified.


EOF
===


