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

// if it's defined here, we use it.  if it's not, we don't

#define RPL_WELCOME           001
#define RPL_YOURHOST          002
#define RPL_CREATED           003
#define RPL_MYINFO            004

#define RPL_PROTOCTL          005 // defined as RPL_BOUNCE in rfc2812

#define RPL_STATSDLINE        250
#define RPL_LUSERCLIENT       251
#define RPL_LUSEROP           252
#define RPL_LUSERUNKNOWN      253
#define RPL_LUSERCHANNELS     254
#define RPL_LUSERME           255

#define RPL_LOCALUSERS        265 // not documented in rfc2812
#define RPL_GLOBALUSERS       266 // not documented in rfc2812

#define RPL_MOTD              372
#define RPL_MOTDSTART         375
#define RPL_ENDOFMOTD         376

#define RPL_WHOISAWAY         301
#define RPL_WHOISUSER         311
#define RPL_WHOISHOST         378 // not documented in rfc2812
#define RPL_WHOISACTUALLY     338

#define RPL_WHOISSERVER       312
#define RPL_WHOISOPERATOR     313
#define RPL_WHOISIDLE         317
#define RPL_ENDOFWHOIS        318
#define RPL_WHOISCHANNELS     319

// when joining channels..
#define ERR_NOSUCHCHANNEL     403
#define ERR_TOOMANYCHANNELS   405
#define ERR_BANNEDFROMCHAN    474
#define ERR_CHANNELISFULL     471
#define ERR_INVITEONLYCHAN    473
#define ERR_BADCHANNELKEY     475
#define ERR_NEEDREGGEDNICK    477
#define ERR_UNAVAILRESOURCE   437

#define RPL_CREATIONTIME      329
#define RPL_NOTOPIC           331
#define RPL_TOPIC             332
#define RPL_TOPICEND          333 // not documented in rfc2812

#define RPL_NAMREPLY          353
#define RPL_ENDOFNAMES        366

#define RPL_CHANNELMODEIS     324

// mode stuff
#define ERR_CHANOPRIVSNEEDED  482

// nick stuff
#define ERR_NOSUCHNICK        401
#define ERR_ERRONEUSNICKNAME  432
#define ERR_NICKNAMEINUSE     433

// channel list stuff
#define RPL_LISTSTART         321 // rfc2812 says obsolete, but is still used (at least on EFnet)
#define RPL_LIST              322
#define RPL_LISTEND           323
#define ERR_TOOMANYMATCHES    416 // only partly documented in rfc2812
#define ERR_NOSUCHSERVER      402

// server links (/LINKS command)
#define RPL_LINKS             364
#define RPL_ENDOFLINKS        365
