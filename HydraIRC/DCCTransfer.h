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

#pragma once

#define DCCT_STATUS_UNKNOWN      0 // at class init
#define DCCT_STATUS_CONNECTING   1 // just after class init, before a connection is attempted
#define DCCT_STATUS_RESUMING     2 // attempting to resume
#define DCCT_STATUS_ERROR        3 // error!
#define DCCT_STATUS_NEGOTIATE    4 // waiting for data
#define DCCT_STATUS_CONNECTED    5 // transferring data
#define DCCT_STATUS_COMPLETE     6 // transfer is complete, socket closed or closing
//+ <gliptic> Pending accept state
#define DCCT_STATUS_PENDING      7 // transfer is not accepted yet
// </gliptic>

#define DCCT_STATUS_MAX 256

class CDCCTransfer
{
public:
  IRCServer *m_pServer;
  int m_NetworkID;
  Socket *m_pTransferSocket,*m_pServerSocket; // server socket only using when sending files.
  char m_SocketBuf[16384]; // 16k socket receive buffer, or should it be 8k?
  char m_FileBuf[16384];
  BOOL m_ReadyToSend;
  BOOL m_FastSend;
  HANDLE m_hFile;
  int m_Type; // either DCC_SEND or DCC_RECEIVE
  int m_Status;
  unsigned int m_PacketSize;
  char m_StatusStr[DCCT_STATUS_MAX];
  int m_CheckTicks;  // reset to 0 each time we receive data.
  // incremented by one each time Check() is called
  BOOL m_StatusDirty; // is set when status changes, reset when we've updated the status.
  BOOL m_Verbose;
  BOOL m_CanRetry;
    short m_Retries;
  char m_IPAddressString[15+1+5+1]; // 14+1+5 == strlen("123.123.123.123:65535")+1 for null term


  // Size Information
  unsigned long m_Received;
  unsigned long m_Sent;
  unsigned long m_Position; // the current position of the transfer.
  unsigned long m_FilePosition; // when sending, the current position of the file
  unsigned long m_RemotePosition; // the current position of the file that the remote has acknowledged
  unsigned long m_ResumeOffset; // if resuming, this is where we resumed from.

  // times
  time_t m_ResumeTime; // is set when the file is resumed.
  time_t m_Time; // start time

  // Remote user info
  char *m_OtherNick; // who it's from, or who it's to
  unsigned long m_Address;
  int m_Port;
  unsigned long m_Size;

  // Filenames
  char *m_FileName; // name of the file that is sent by/to the remote user
  char *m_LocalFileName; // the absolute name of the local file

  CDCCTransfer(IRCServer *pServer, char *OtherNick, char *FileName, unsigned long Address, int Port, unsigned long Size,int Flags);
  ~CDCCTransfer( void );

  void WriteTransferSocket( void );
  void ReadTransferSocket( void );
  void OnDCCTransferSocketEvent(int Event, int Error);
  void OnDCCServerSocketEvent(int Event, int Error);

  BOOL ProcessFastDCCSend( void );

  void Accept( void );
  void Retry( void );
  void Connect( void );
  void Resume( long Offset);

  void Disconnect( char *Reason );
  void OnConnect( void );
  void Check( void );
  void UpdateStatus( void );

  BOOL IsActive( void );

private:
  void Go( int NewStatus );
};
