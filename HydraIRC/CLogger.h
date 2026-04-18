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

#define LOGF_NONE       0
#define LOGF_ENABLED    (1<<0)
#define LOGF_STRIPCODES (1<<1)

class CLogger :
  public CListener
{
private:
  int m_UpdateFrequency;  // how long to wait in seconds before the log buffer is flushed
  int m_SecondsSinceLastFlush;
  CBufferedFile m_LogFile;
  DWORD m_Flags; // see LOGF_* defines
  int *m_FilterList;

public:
  char *m_LogFormatStr; // UNUSED so far.
  char *m_LogFileName; // just the relative path+filename part. e.g. network\server_log.txt
  char *m_LogFolder; // just the path part, e.g. "c:\documents and settings\me\My Documents\Logs\IRC"  // no trailing '\'

  CLogger(void);
  ~CLogger(void);

  void Start( void );
  void Stop( void );
  void Flush( void );

  void UpdateSettings(DWORD Flags); // starts logging if LOGF_ENABLED was specified in the new flags
  inline int GetSettings( void ) { return m_Flags; }

  // To log: first call IsFiltered(), then call Log() if IsFiltered() returns FALSE
  inline BOOL IsFiltered(int Contents) { return ProcessSimpleFilter(m_FilterList,Contents); }
  BOOL Log( char *Data );

  BOOL NeedsFlushing( void ); // if returns true, call Flush() asap.  use with EV_TICK 
  inline BOOL IsEnabled( void ) { return (m_Flags & LOGF_ENABLED); }
  inline void SetFilterList(int *FilterList) { m_FilterList = FilterList; }
  void SetPaths(char *Folder, char *FileName); // NOTE: a copy of the strings is NOT taken, so make sure you allocate the strings so they can be free()'d
  char *GetFileName( void ); //returns a copy of the filename, dont' forget to free() it

  void OnEvent(int EventID, void *pData);
};
