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

// CBufferedFile.h - By Hydra!
//
/*
  Nice little class that wraps some basic FileIO functions to provide buffered writes.
  can be adapted to use the stdio or lowlevel IO functions easily.

  There's a little bit of performance analysis code too, just define CBF_PERF_ANALYZE
  and every time a file is closed you'll get a summary of disk writes and memory copies 
  and requested writes.

  Obviously, for it to be doing anything useful, you want disk writes to be less than 
  requested wites...

  TODO: add buffered file reading ?
*/

// You need these include files...
//#include <malloc.h>
//#include <windows.h>

//#define CBF_PERF_ANALYZE
//#define CBF_DEBUG

#define CBUFFEREDFILE_DEFAULT_BUFFER_SIZE (16 * 1024) // intel sized 64k blocks are probably more efficient..

enum CBufferedFileErrors
{
  CBF_NO_MORE_ERRORS,
  CBF_NO_ERROR,
  CBF_NO_MEMORY,
  CBF_OS_ERROR,
  CBF_FILE_NOT_OPEN,
  CBF_BAD_PARAMS
};

class CBufferedFile
{
private:
  HANDLE m_File;
  char *m_Buffer;
  char *m_FileName;
  DWORD m_BufferSize;
  DWORD m_BufferStart; // the offset from which to write data to the file.
  DWORD m_BufferPos; // the offset from which to append new data
  DWORD m_BytesInBuffer; // the amount of data in the buffer (the difference between m_BufferPos and  m_BufferStart)

#ifdef CBF_PERF_ANALYZE
  DWORD m_RequestedWrites;
  DWORD m_Writes;
  DWORD m_Copies;
#endif

  CBufferedFileErrors m_LastError;

  void Reset( void )
  {
    m_BufferSize = CBUFFEREDFILE_DEFAULT_BUFFER_SIZE;
    m_Buffer = NULL;
    m_File = NULL;
    m_FileName = NULL;
    m_LastError = CBF_NO_MORE_ERRORS;
    m_BytesInBuffer = FALSE;
    m_BufferPos = 0;
    m_BufferStart = 0;

#ifdef CBF_PERF_ANALYZE
    m_RequestedWrites = 0;
    m_Writes = 0;
    m_Copies = 0;
#endif
  }

public:
  CBufferedFile( void )
  {
    Reset();
  }

  ~CBufferedFile( void )
  {
    if (m_File)
    {
      CloseFile();
    }
    if (m_Buffer) free(m_Buffer);
  }

  BOOL IsOpen( void )
  {
    return (m_File != NULL);
  }

  int GetLastError( void )
  {
    int CurrentError = m_LastError;
    m_LastError = CBF_NO_MORE_ERRORS;
    return CurrentError;
  }

  char *GetFileName( void )
  {
    if (!m_File)
      return NULL;

    return m_FileName;
  }

  void SetBufferSize( DWORD NewBufferSize )
  {
    // adjust buffer size only if file not open or when buffer is empty, empty buffer now if needed.
    if (!m_File || Flush())
    {
      m_BufferSize = NewBufferSize; 

      if (m_File)
      {
        // keep the old buffer while we alloc a new one, in case it fails.
        char *NewBuffer = (char *)malloc(NewBufferSize);
        if (NewBuffer)
        {
          if (m_Buffer) free(m_Buffer);
          m_Buffer = NewBuffer;
        }
      }
    }
  }

  // apps should call this before closing the file to check for write errors.
  BOOL Flush( void )
  {
    if (!m_File)
    {
      m_LastError = CBF_FILE_NOT_OPEN;
      return FALSE;
    }

    m_LastError = CBF_NO_ERROR;

    if (m_BytesInBuffer == 0)
      return TRUE; // OK
    else
    {
      DWORD BytesWritten = 0;

#ifdef CBF_DEBUG
      if (m_BytesInBuffer != m_BufferPos - m_BufferStart)
        printf("%d != %d (%d - %d)\n",m_BytesInBuffer,m_BufferPos - m_BufferStart,m_BufferPos,m_BufferStart);
#endif

#ifdef CBF_PERF_ANALYZE
      m_Writes++;
#endif

      if (!::WriteFile(m_File,m_Buffer + m_BufferStart,m_BytesInBuffer,&BytesWritten,NULL))
        m_LastError=CBF_OS_ERROR;

      m_BufferStart += BytesWritten;
      m_BytesInBuffer -= BytesWritten;

      // all data in buffer written, reset pointer to beginning of buffer.
      if (m_BytesInBuffer == 0)
      {
        m_BufferStart = 0;
        m_BufferPos = 0;
      }

      return (m_BytesInBuffer == 0); // return false if not all data was written
    }
  }

  BOOL WriteFile(LPCVOID lpBuffer,        // data buffer
    DWORD nNumberOfBytesToWrite,          // number of bytes to write
    LPDWORD lpNumberOfBytesWritten)       // number of bytes written
  {
    if (lpNumberOfBytesWritten)
      *lpNumberOfBytesWritten = 0;

    if (!m_File)
    {
      m_LastError = CBF_FILE_NOT_OPEN;
      return FALSE;
    }

    if (!lpBuffer || nNumberOfBytesToWrite == 0)
    {
      m_LastError = CBF_BAD_PARAMS;
      return FALSE;
    }

    m_LastError = CBF_NO_ERROR;

#ifdef CBF_PERF_ANALYZE
    m_RequestedWrites++;
#endif
    /*
      Four possible things happen:

      write directly                                                            (1 disk write , 0 memory copies)
      Buffer ALL of lpBuffer to m_Buffer                                        (0 disk writes, 1 memory copy)
      copy part of lpBuffer to m_Buffer, Flush, then Buffer the rest (if any).  (1 disk write , 1 or 2 memory copies)
      Flush, write directly.                                                    (2 disk writes, 0 memory copies)

      With this setup, all disk writes are multiple of the buffer size, unless the
      data being asked to be written is the same or larger than the buffer size.
      Memory copies are also kept to a minimum, but a memory copy rather than a disk write is done in the
      case that there's data left in the buffer and the new buffer is larger than the space left in out buffer
      this is because we might end up doing small+small+big+small+small which could give you
      small, buffered, small, buffered, big, write buffer, buffer rest, small, buffered, small buffered, flush, write = 2 writes.
      but if we didn't do the extra copy, then we end up with this:
      small, buffered, small, buffered, big, write buffer, write big, small, buffered, small, buffered, flush, write = 3 writes
    */

    /*************************/

    // nothing in buffer ?  nNumberOfBytesToWrite bigger or same size than buffer ?
    if (nNumberOfBytesToWrite >= m_BufferSize && m_BytesInBuffer == 0)
    {
      // write directly
#ifdef CBF_PERF_ANALYZE
      m_Writes++;
#endif

      if (!::WriteFile(m_File,lpBuffer,nNumberOfBytesToWrite,lpNumberOfBytesWritten,NULL))
        m_LastError=CBF_OS_ERROR;

      return (m_LastError == CBF_NO_ERROR);
    }

    /*************************/

    // can we buffer all of it ?
    if (nNumberOfBytesToWrite < m_BufferSize - m_BufferPos)
    {
      // append it to the buffer
#ifdef CBF_PERF_ANALYZE
      m_Copies++;
#endif

      memcpy(m_Buffer + m_BufferPos, lpBuffer, nNumberOfBytesToWrite);
      m_BufferPos += nNumberOfBytesToWrite;
      m_BytesInBuffer += nNumberOfBytesToWrite;

      *lpNumberOfBytesWritten = nNumberOfBytesToWrite;

      return TRUE;
    }

    /*************************/

    // can we fill the remainder of the current buffer and then partly fill a new buffer ?
    int SpaceInBuffer = m_BufferSize - m_BytesInBuffer;
    if (nNumberOfBytesToWrite < m_BufferSize + SpaceInBuffer)
    {
      // fill the buffer
#ifdef CBF_PERF_ANALYZE
      m_Copies++;
#endif
      memcpy(m_Buffer + m_BufferPos, lpBuffer, SpaceInBuffer);
      m_BufferPos += SpaceInBuffer;
      m_BytesInBuffer += SpaceInBuffer;

      nNumberOfBytesToWrite -= SpaceInBuffer;

#ifdef CBF_DEBUG
      if (m_BufferPos != m_BufferSize)
        printf("%d != %d\n",m_BufferPos,m_BufferSize);
#endif

      // flush the full buffer.
      if (!Flush())
        return FALSE;


      // any bytes left to write  in lpBuffer ?
      if (nNumberOfBytesToWrite)
      {
        // now copy the remaining part of lpBuffer to m_Buffer (Note: we don't use m_BufferPos as it's always 0 at this point due to the Flush call)
#ifdef CBF_PERF_ANALYZE
        m_Copies++;
#endif
        memcpy(m_Buffer, (char *)lpBuffer+SpaceInBuffer, nNumberOfBytesToWrite);
        m_BufferPos = nNumberOfBytesToWrite;
        m_BytesInBuffer = nNumberOfBytesToWrite;
      }
      return TRUE;
    }

    /*************************/

    // to big to fit in buffer, flush the buffer and then write the data directly.

    if (!Flush())
      return FALSE;

#ifdef CBF_PERF_ANALYZE
      m_Writes++;
#endif

    if (!::WriteFile(m_File,lpBuffer,nNumberOfBytesToWrite,lpNumberOfBytesWritten,NULL))
      m_LastError=CBF_OS_ERROR;

    return (m_LastError == CBF_NO_ERROR);
  }


  void CloseFile( void )
  {
    if (!m_File)
    {
      m_LastError = CBF_FILE_NOT_OPEN;
      return;
    }

    Flush(); // ideally, an app should call flush first!

    // SetEndOfFile(m_File); // check?

    CloseHandle(m_File);

#ifdef CBF_PERF_ANALYZE
    printf("Performance: \"%s\" - %d requested writes, %d actual disk writes, %d memory copies\n",m_FileName,m_RequestedWrites,m_Writes,m_Copies);
#endif

    if (m_Buffer) // should never get here without a valid m_Buffer
      free(m_Buffer);

    if (m_FileName)
      free(m_FileName);

    Reset();
  }

  BOOL CreateFile(
    LPCTSTR lpFileName,                         // file name
    DWORD dwDesiredAccess,                      // access mode
    DWORD dwShareMode,                          // share mode
    LPSECURITY_ATTRIBUTES lpSecurityAttributes, // SD
    DWORD dwCreationDisposition,                // how to create
    DWORD dwFlagsAndAttributes,                 // file attributes
    HANDLE hTemplateFile)                       // handle to template file
  {
    // user forgot to close the last file, do it for them..
    if (m_File)
      CloseFile();

    Reset();

    m_Buffer = (char *) malloc(m_BufferSize);
    if (!m_Buffer)
    {
      m_LastError = CBF_NO_MEMORY;
      return FALSE;
    }

    m_File = ::CreateFile(lpFileName,dwDesiredAccess,dwShareMode,lpSecurityAttributes,
                          dwCreationDisposition,dwFlagsAndAttributes,hTemplateFile);
    if (m_File != INVALID_HANDLE_VALUE)
    {
      m_FileName = strdup(lpFileName);
      m_LastError = CBF_NO_ERROR;
      return TRUE;
    }
    else
    {
      m_File = NULL;
      m_LastError = CBF_OS_ERROR;
      free(m_Buffer);
      m_Buffer = NULL;
      return FALSE;
    }
  }

  DWORD GetFileSize(LPDWORD lpFileSizeHigh)  // high-order word of file size
  {
    if (!m_File)
    {
      m_LastError = CBF_FILE_NOT_OPEN;
      return INVALID_SET_FILE_POINTER;
    }
    else
      return ::GetFileSize(m_File,lpFileSizeHigh);
  }

  HANDLE GetHandle( void )
  {
    if (m_File)
      return m_File;
    else
      return INVALID_HANDLE_VALUE;
  }

  DWORD SetFilePointer(
    LONG lDistanceToMove,        // bytes to move pointer
    PLONG lpDistanceToMoveHigh,  // bytes to move pointer
    DWORD dwMoveMethod)          // starting point
  {
    if (!m_File)
    {
      m_LastError = CBF_FILE_NOT_OPEN;
      return 0;
    }
    return ::SetFilePointer(m_File,lDistanceToMove,lpDistanceToMoveHigh,dwMoveMethod);
  }



};

