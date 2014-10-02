/*

  HydraIRC
  Copyright (C) 2002 Dominic Clifton aka Hydra

  License not settled on yet, for now this code cannot be used to contribute
  to the HydraIRC project and may not be used in any other projects until the
  license has been decided.

*/

#include "objbase.h"

#ifndef __AsyncMessagePump_H
#define __AsyncMessagePump_H

#define THREAD_MSG_MAP(msgMapID) ALT_MSG_MAP(msgMapID)

#define WM_SENDMSG (WM_APP + 1535)
enum ThreadStates
    {
    Starting=0,
    Running=1,
    Stopping=2,
    Stopped=3,
    };

class CAsyncMessagePump
    {
    CMessageMap * m_pTarget; //where the messages should go
    HANDLE m_hThread;        //handle of the thread
    DWORD m_dwThreadId;      //Thread ID of the thread
    long m_dwMsgMapID;       //msg map ID
    CComAutoCriticalSection m_cs; //So we can synchronize
    ThreadStates m_ThreadState; //Current state of child thread

    //Private struct holding thread init parms.
    struct ThreadParms
        {
        CAsyncMessagePump * pThis;
        HANDLE hInit;
        long dwCoInit;
        };

    //This is used for the synchronous version.
    struct SendMessage_t
        {
        MSG msg;            //The message to process
        HANDLE hEvent;      //handle to fire.
        LRESULT result;
        };

    //simple little object lock
    struct ObjLock
        {
        CAsyncMessagePump * m_pThis;
        ObjLock(CAsyncMessagePump *pThis)
          {
          m_pThis = pThis; 
          m_pThis->m_cs.Lock();
          }
        ~ObjLock()
          { m_pThis->m_cs.Unlock();}
        };
    friend struct ObjLock;

    void OnThreadStarting(HANDLE hEvent)
        {
        //bogus peekmessage just establishes msg queue.  I've been bitten by
        //this in the past - if a PostMessage comes in before the thread proc
        //has set up a message queue the message just vanishes.  

        //Note that I don't enter a critical section here.  That's because
        //Start() is already holding it, and in fact that thread is frozen
        //waiting for me to pulse the event telling it that the child thread
        //is up and running.
        MSG m;
        ::PeekMessage(&m,0,0,0,PM_NOREMOVE);
        m_ThreadState = Running;
        ::SetEvent(hEvent);
        }


    void OnThreadTerminating()
        {
        ObjLock lock(this);
        m_pTarget = 0;
        ::CloseHandle(m_hThread);
        m_dwThreadId = 0;
        m_dwMsgMapID = 0;
        m_ThreadState = Stopped;
        }

    static DWORD WINAPI _ThreadProc(void * pArgs) 
        {
        ThreadParms * p = reinterpret_cast<ThreadParms *>(pArgs);
        CAsyncMessagePump * pThis = p->pThis;

        long CoInit = p->dwCoInit;
        if(CoInit >= 0) 
        {
          //::CoInitializeEx(NULL, CoInit);
          CoInitialize(NULL);
        }
        pThis->OnThreadStarting(p->hInit);

        //Now that we've got a message queue we let the caller proceed.
        MSG m;
        while(::GetMessage(&m,0,0,0))
            {
            //If the message has a non-NULL hwnd then we pass off to standard processing.  This shouldn't happen
            //however; the only way we should get messages is via our own PostMessageRoutine.
            if(m.hwnd != NULL)
                {
                TranslateMessage(&m);
                DispatchMessage(&m);
                }
            else
                {
                //If we get a WM_SENDMSG we check WPARAM to see if _we_ sent this or if some poor shmo just happens
                //to be passing the same message.  I suppose I could _register_ a window message, but that seems like
                //overkill - this works fine.  
                if((m.message == WM_SENDMSG) && (m.wParam == 0xE1E10000))
                    {
                    //For the synchronous messages we extract the real MSG from the passed parm, push it through the
                    //message map, then set the passed event handle.  We store the return value from the map into
                    //ps->result, and extract it on the other side.
                    SendMessage_t * ps = reinterpret_cast<SendMessage_t *>(m.lParam);
	    	        pThis->m_pTarget->ProcessWindowMessage(0, ps->msg.message, ps->msg.wParam, ps->msg.lParam, ps->result, pThis->m_dwMsgMapID);
                    ::SetEvent(ps->hEvent);
                    }
                else 
                    {
                    LRESULT lRes;
	    	        pThis->m_pTarget->ProcessWindowMessage(0, m.message, m.wParam, m.lParam, lRes, pThis->m_dwMsgMapID);
                    }
                }
            //check to see if we should keep running.
            if(pThis->m_ThreadState == Stopping) break;
            }

        //Leave the COM apartment. Any objects services by this thread are now screwed if this is an STA thread.
            if(CoInit > 0) ::CoUninitialize();

        pThis->OnThreadTerminating();
        return 0;
        }

 public:
    CAsyncMessagePump()
      : m_pTarget(0),
        m_dwMsgMapID(0),
        m_dwThreadId(0),
        m_hThread(0),
        m_ThreadState(Stopped)
        {}

    ~CAsyncMessagePump()
        {Stop();}

    //We'll return the thread id from Start().  The dwCoInit gives you a chance
    //to have this thread enter a COM apartment if you like.
    DWORD Start(CMessageMap * pTarget, long Id, long dwCoInit = -1)
        {
        ObjLock lock(this);

        //If the thread is already running then we don't start up again.
        if(m_ThreadState != Stopped)
            { return 0;  }

        m_ThreadState = Starting;
        m_pTarget = pTarget;
        m_dwMsgMapID = Id;

        ThreadParms p;
        p.pThis = this;
        p.hInit = ::CreateEvent(NULL, TRUE, FALSE, NULL);
        p.dwCoInit = dwCoInit;

        //Create the thread and wait for it to signal us that it's ready...
        m_hThread = ::CreateThread(NULL, 0, _ThreadProc, &p, 0, &m_dwThreadId);

        if(m_hThread != NULL) 
            {
            ::WaitForSingleObject(p.hInit, INFINITE);
            }

        ::CloseHandle(p.hInit);

        return m_dwThreadId;
        }

    //Here we stop the thread,
    void Stop(bool bWaitForCompletion=true)
        {
        m_cs.Lock();
        if(m_ThreadState != Running) 
            { m_cs.Unlock(); return; }

        m_ThreadState = Stopping;
        PostMessage(WM_QUIT, 0, 0);
        m_cs.Unlock();

        if(bWaitForCompletion)
            {
            ::WaitForSingleObject(m_hThread, INFINITE);
            }
        }

    //simple helper to post async messages
    BOOL PostMessage(DWORD dwMsg, WPARAM wParam, LPARAM lParam)
        {
        ObjLock lock(this);
        if(m_dwThreadId!=0)
            return PostThreadMessage(m_dwThreadId, dwMsg, wParam, lParam);
        else return 0;
        }
    
    //more complicated helper to post & wait.
    BOOL SendMessage(DWORD dwMsg, WPARAM wParam, LPARAM lParam, LRESULT &lResult)    
        {
        ObjLock lock(this);
        
        if(m_dwThreadId!=0)
            {
            SendMessage_t s;
            s.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
            if(s.hEvent == NULL) return FALSE;

            s.msg.message = dwMsg;
            s.msg.wParam = wParam;
            s.msg.lParam = lParam;
            s.msg.time = ::GetTickCount();
            s.msg.pt.x = 0;
            s.msg.pt.y = 0;

       
            ::PostThreadMessage(m_dwThreadId, WM_SENDMSG, (WPARAM)(0xE1E10000), (LPARAM)&s);
            ::WaitForSingleObject(s.hEvent, INFINITE);
            ::CloseHandle(s.hEvent);
            lResult = s.result;
            return TRUE;
            }
        else
            {
            lResult = 0;
            return FALSE;
            }
        }
    };


#endif
