#pragma once

class Thread
{
public:

	Thread() 
    : m_ThreadHandle(NULL), m_ThreadID(0)
  { };

	virtual ~Thread() 
  { 
		if(m_ThreadHandle)
			CloseHandle(m_ThreadHandle);
	};

	BOOL Start()
  {
		m_ThreadHandle = CreateThread(NULL, 0, &Starter, this, 0, &m_ThreadID);
    return (m_ThreadHandle ? TRUE : FALSE);
	}

  DWORD GetThreadID( void )
  {
    return m_ThreadID;
  }

protected:
	virtual int Run() = 0;

private:
	HANDLE m_ThreadHandle;
	DWORD m_ThreadID;

	static DWORD WINAPI Starter(void *p) 
  {
    sys_Printf(BIC_INFO,"Thread Starting!\n");
		Thread* t = (Thread*)p;
		t->Run();
    sys_Printf(BIC_INFO,"Thread Stopped!\n");
		return 0;
	}

};
