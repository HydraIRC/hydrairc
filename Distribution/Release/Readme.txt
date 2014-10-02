HydraIRC                                          
==============================================================================

Noob?
-----

READ THE README.TXT! READ THE README.TXT! READ THE README.TXT!

Still got a problem?

READ THE DOCS! READ THE DOCS!  READ THE DOCS!  READ THE DOCS!  READ THE DOCS!

Still got a problem?

READ CHANGES.TXT! READ CHANGES.TXT! READ CHANGES.TXT! READ CHANGES.TXT! 

Still got a problem?

READ THE FAQ! READ THE FAQ! READ THE FAQ! READ THE FAQ! READ THE FAQ!

Still got a problem?

Read TODO.TXT and see if it's known or not done yet!

Still got a problem?

Then ask in the channel: start HydraIRC & click Favorites/HydraIRC/#HydraIRC

Failure to have NOT read the docs will result in you being slapped around.

Remember, this is NOT v1.0 yet!!!!!!!!!


HydraIRC                                          -< http://hydrairc.sf.net >-
==============================================================================
read the TODO.TXT for known bugs/issues.
read CHANGES.TXT for changes and lots more info!

report any unknown issues/bugs to me ASAP, include screenshots, logs and any
other information (e.g.  exception address, version, etc..)

Read all of this file, even if you're upgrading!

Installation
============

If you have an exe installer, just run it and follow onscreen prompts.  If you
had a version of HydraIRC before you used the installer, make sure you read the
upgrading instruction below.

if you have a zip file distribution then unzip all the files from this zip
file to a new directory (e.g. C:\Program Files\HydraIRC)

When you unzip, be sure to keep the folder/path names so the plugins go in 
the right places.

Upgrading?
==========

When upgrading from a previous version when NOT using the installer, or if you
have a version of HydraIRC prior to v0.349 on your PC follow these steps:

1) backup your existing profile_default.xml file
2) install as normal
3) retore your profile_default.xml file
4) start hydrairc
5) select tools/options, click "OK".  at this point new preference items will
   be saved to your profile
6) Optional Steps
   a) Close HydraIRC
   b) edit the profile_default.xml file using a text editor.
   c) Restart HydraIRC.

Usage
=====

Start HydraIRC by double clicking the HydraIRC.exe file.

Please also take a look at docs.htm and then read the rest of this file

If you need more info on something, try searching CHANGES.TXT and TODO.TXT

Notes
=====
This is a very very early version.

The IRC commands implemented are as follows:

Global Commands:
/NICK                   <newnick>
/EXIT
/SERVER                 <servername>[:<port>][,<password>]
/NEWSERVER              <servername>[:<port>][,<password>]
/AUDIOMUTE
/TIMESTAMPS             ON|OFF
/SET                    <prefpath>/<prefoption>=<value>

Channel and Server Commands:

/RECONNECT
/MSG                    <nick|channel> <message>
/RAW                    <args>
/CTCP                   <nick> <ctcp request>
/JOIN                   <channel> [<key>][,...]
/PART                   [channel][,...]
/QUIT                   [<quit message>]
/QUERY                  <nick>[,...] [<message>]
/DCCSEND                <nick>

Channel Commands:

/ME                     <action>
/USERLIST
/MONITOR
/STOPMONITOR

The basic plugin also provides these commands:

Channel Commands:
/KICK                   <nick> [<reason>]
/KR                     <reason>
/MODE                   [<channel> <modes> <mode params>]
/TOPIC                  [<new topic>]
/UPTIME
/PIMP
/SLAP

Current Aliases:
op=mode $# +ooo $1 $2 $3
dop=mode $# -ooo $1 $2 $3
voice=mode $# +vvv $1 $2 $3
j=join $1-
qj=quietjoin $1-
q=query $1-
disconnect=quit $1-
leave=part $1-
quote=raw $1-
nserver=newserver $1-
exitnow=exit -now
xget=ctcp $1 xdcc send $2-
xlist=ctcp $1 xdcc list $2-
ns=msg NICKSERV $1-
ms=msg MEMOSERV $1-
cs=msg CHANSERV $1-
dalns=msg nickserv@services.dal.net $1-
auth=msg NICKSERV identify $1-
dalauth=msg nickserv@services.dal.net identify $1-
qauth=msg q@cserve.quakenet.org auth $1-
notice=NOTICE $1 :$2-

Alias Notes:
Escape Character: $
Max Parameters: 3
Item                       Definition           Example
Channel Name               <escchar>#           $#
Current Nick               <escchar>!           $!
Param                      <escchar><paramnum>  $2
Param and Remaining Params <escchar><paramnum>- $1-
Additonal Params           <escchar>-           $-


For more information on the commands available, search the changes.txt file
for occurences of the command.

For the curious
===============

Checkout the enclosed XML config files (edit them in a text editor) and
change them if you feel like it.

You can add new servers, change the fonts, setup buddy lists, etc..

See the website for more details

Known Issues
============

See TODO.txt for a list of known BUGS and todo features,  if you spot
something that's not listed then let me know ASAP.

Problems/Troubleshooting?
=========================

If you have problems, then please download the latest debug version from the
website and run that, re-create the problem and then send me the log-file.
(HydraIRC_Log.txt, a screenshot is always useful too, to create a screenshot
just press print-screen on your keyboard, then paste the clipboard into your
new e-mail message or a paint program.

Additionally, if you have problems with the docking windows then close the
program down and use regedit to clear the following key and all it's subkeys:
"HKEY_CURRENT_USER\Software\HydraProductions\HydraIRC\WindowStates"

Also, see the troubleshooting section in docs.htm

License
=======

SEE LICENSE.TXT
