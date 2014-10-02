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

#include "StdAfx.h"
#include "HydraIRC.h"

CLogger::CLogger(void)
{
  m_LogFile.SetBufferSize(INTPREF(PREF_nLogBufferSize));
  m_UpdateFrequency = INTPREF(PREF_nLogFlushInterval); // flush every X seconds
  m_Flags = LOGF_NONE;
  m_FilterList = NULL; // don't filter anything by default
  m_SecondsSinceLastFlush = 0;
  m_LogFolder = NULL;
  m_LogFileName = NULL;
  m_LogFormatStr = NULL;
  CListener::Start(&g_EventManager);
}

CLogger::~CLogger(void)
{
  Stop();
  SetPaths(NULL,NULL);
  if (m_LogFormatStr) free(m_LogFormatStr);
}

void CLogger::Stop( void )
{
  if (m_LogFile.IsOpen())
  {
    sys_Printf(BIC_INFO,"Stopping logging to \"%s\"\n",m_LogFile.GetFileName());

    m_LogFile.CloseFile();
  }
  m_Flags &= ~LOGF_ENABLED; // turn logging off.
}

void CLogger::Start( void )
{  
  // TODO: more error checking on the file IO

  if (!m_LogFile.IsOpen()) // only if we're not already logging, to use a different filename, call Stop() first!
  {
    m_Flags &= ~LOGF_ENABLED; // turn logging off unles we actually start it ok.

    if (!m_LogFolder || !m_LogFileName)
      return; // yikes, no filenames yet!

    ReplaceString(&m_LogFolder,"<HOMEDIR>",g_HomeDir);

    if (!CreatePath(m_LogFolder))
      sys_Printf(BIC_ERROR,"Could not create log directory \"%s\"\n",m_LogFolder);

    char FileName[MAX_PATH];
    _snprintf(FileName,sizeof(FileName),"%s\\%s",m_LogFolder,m_LogFileName);

    if (!m_LogFile.CreateFile(FileName,
      GENERIC_WRITE,
      FILE_SHARE_READ, 
      NULL, 
      OPEN_ALWAYS, 
      FILE_ATTRIBUTE_NORMAL, 
      NULL))
    {
      /*
      ErrorNum = MyFile.GetLastError();
      printf("Error creating file - %d\n",ErrorNum);
      if (ErrorNum == CBF_OS_ERROR)
      {
        printf("OS error, call OS error function..(GetLastError() = %d)\n",GetLastError());
      }
      */
      sys_Printf(BIC_ERROR,"Error opening log file \"%s\"\n",FileName);
      
    }
    else
    {
      // file ok!
      m_LogFile.SetFilePointer(0,NULL,FILE_END); // seek to end.
      m_SecondsSinceLastFlush = 0;
      m_Flags |= LOGF_ENABLED; // it's running!

      sys_Printf(BIC_INFO,"Starting logging to \"%s\"\n",FileName);
    }
  }
}

void CLogger::UpdateSettings(DWORD Flags)
{
  Stop();

  m_UpdateFrequency = INTPREF(PREF_nLogFlushInterval); // flush every X seconds
  m_Flags = Flags;

  m_LogFile.SetBufferSize(INTPREF(PREF_nLogBufferSize));

  if (m_Flags & LOGF_ENABLED)
    Start();
}

void CLogger::Flush( void )
{
  m_SecondsSinceLastFlush = 0;
  if (m_LogFile.IsOpen())
  {
    m_LogFile.Flush();
  }
}

BOOL CLogger::NeedsFlushing( void )
{ 
  return (m_LogFile.IsOpen() && (m_SecondsSinceLastFlush) > m_UpdateFrequency);
}

BOOL CLogger::Log( char *Data )
{
  // don't log if logging is not enabled, or if the file is closed.
  if (!(m_Flags & LOGF_ENABLED) || !m_LogFile.IsOpen())
    return FALSE;

  char *DataPtr;
  if (m_Flags & LOGF_STRIPCODES)
  {
    DataPtr = strdup(Data);
    if (DataPtr)
      stripcodes(DataPtr);
  }
  else
  {
    DataPtr = Data;
  }

  int DataLength = strlen(DataPtr);
  DWORD BytesWritten;
  BOOL Success;
  Success = m_LogFile.WriteFile(DataPtr,DataLength,&BytesWritten);
  if (Success) Success = m_LogFile.WriteFile("\r\n",2,&BytesWritten);

  if (m_Flags & LOGF_STRIPCODES && DataPtr)
    free(DataPtr);

  return Success;
}

void CLogger::SetPaths(char *Folder, char *FileName)
{
  if (m_LogFolder) free(m_LogFolder);
  if (m_LogFileName) free(m_LogFileName);

  if (FileName)
    ReplaceInvalidFilenameCharacters(FileName,'_');

  m_LogFolder = Folder;
  m_LogFileName = FileName;
}

// return a copy of the filename, don't forget to free() it
char *CLogger::GetFileName( void )
{
  if (!(m_LogFolder && m_LogFileName))
    return NULL;

  char FileName[MAX_PATH];
  _snprintf(FileName,sizeof(FileName),"%s\\%s",m_LogFolder,m_LogFileName);

  return strdup(FileName);
}

void CLogger::OnEvent(int EventID, void *pData)
{
  switch(EventID)
  {
    case EV_TICK:
      if (m_LogFile.IsOpen())
      {
        m_SecondsSinceLastFlush++;
        if (NeedsFlushing())
          Flush();
      }
      break;
  }
}