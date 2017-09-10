#include "fwCommon_pch.h"
#include "fwthread.h"
#include "fwnumbers.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace fw 
{

    namespace core
    {

        Thread::Thread()  {
        
          //by default we don't want the thread to stop
          mTerminate = false;
          mThread = NULL;
          mWasKilled = false; 
          mWaitEvent = NULL;
        
        }; //constructor
        
        void 
        Thread::start(AFX_THREADPROC pfnThreadProc, LPVOID pParam) {
        
        #ifdef _DEBUG
            //checking if the wait event is NULL
            //if it is no we probably lost the thread synchronization
            lockDataAccess();
            CEvent* pEvent = mWaitEvent;
            unlockDataAccess();
            //ASSERT(pEvent == NULL);
        #endif 
          
          if (isTerminating())
              return;
        
          //the thread will not start until the "terminated"
          //flag is cleared:
          //so it can start if the working thread did not start so far
          //or was already terminated
        
          bool bNotTerminated = true;
          CWinThread* pThread = NULL;
          do {
            //reseting the "terminate request" flag
            lockDataAccess();
            bNotTerminated  = mTerminate;
            pThread = mThread;
            unlockDataAccess();
            Sleep(100);
          } while (pThread && bNotTerminated);
        
          lockDataAccess();
          //mTerminate = false;
          mThread = AfxBeginThread(pfnThreadProc, pParam);
          unlockDataAccess();
        
        }; //start
        
        
        void 
        Thread::lock(bool pPumpMessages) {
        
        
        
            lockDataAccess();
        #ifdef _DEBUG
            //checking if the wait event is NULL
            //if it is no we probably lost the thread synchronization
            CEvent* pEvent = mWaitEvent;
           // ASSERT(pEvent == NULL);
        #endif
        
        
        
            //creating event that will be used to wait
            mWaitEvent = new CEvent;
            unlockDataAccess();    
         
        
            if (pPumpMessages) {
        
              DWORD result = WAIT_OBJECT_0;
              do {
                //process all messages waiting to be dispatched
                MSG msg;
                while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                  DispatchMessage(&msg);
                }
        
                //wait for event or our object to be singaled
                TRACE(_T("Locking the main thread\n"));
                HANDLE handles[1];
                lockDataAccess();
                handles[0] = mWaitEvent->m_hObject;
                unlockDataAccess();
                result = MsgWaitForMultipleObjects(1 /* one object in the array */,
                                          handles, 
                                          TRUE, /* wiat for ALL objects - in our case there is only one */
                                          1,
                                          QS_ALLEVENTS);
              }
			  while (result != WAIT_OBJECT_0 && isRunning());
            }
            else {
              //if we don't need message pomping then just wait for the
              //event (this freezes the message loop)
              mWaitEvent->Lock();
            }
        
        
            //we are here, so the thread has been unlocked
            //destroying the event 
            lockDataAccess();
            delete mWaitEvent;
            mWaitEvent = NULL;
            unlockDataAccess();
        
        }; //lock
        
        
		void Thread::requestTerminate()
		{
			lockDataAccess();
			mTerminate = true;
			unlockDataAccess();
		}


		void Thread::resetTerminateFlag()
		{
			lockDataAccess();
			mTerminate = false;
			unlockDataAccess();
		}
        
        void 
        Thread::terminate(bool pTerminate, bool pPumpMessages) {
        
          bool bDoNothing = false;
          CWinThread* pThread = NULL;
          lockDataAccess();
          if (pTerminate && mTerminate)
              bDoNothing = true;
          else
          {
            mTerminate = pTerminate;
            pThread = mThread;
          }
          unlockDataAccess();
        
        
          //we are trying to terminate a thread
          //that is already terminating
          if (bDoNothing)
              return;
        
        
        
          //if this is termination request and the tread is still active
          //wait until it terminates
          if (pThread && pTerminate) 
            lock(pPumpMessages);
        
          lockDataAccess();
          mTerminate = false;
          unlockDataAccess();
        
          TRACE(_T("Thread::terminate (unlocked).\n"));
        
        }; //terminate
        
        
        bool
        Thread::stopRequest() 
        {
        
          
        
          lockDataAccess();
          bool result = mTerminate;
          unlockDataAccess();
        
        //  TRACE(_T("Thread::stopRequest (%d)\n"), result);
          return result;
        
        }; //terminate
        
        
        void
        Thread::unlock() {
        
          //unlocking but only if the event exists!
          //this way we can call unlock as many time as we wont
          //and still the next time we lock it will be sucessfull
        
          TRACE(_T("Thread::unlock (unlocking)\n"));
          if (mWaitEvent)
              mWaitEvent->SetEvent();
        
        }; //unlock
        
        bool
        Thread::isRunning() {
        
          lockDataAccess();
          bool bResult = mThread ? true : false;
          unlockDataAccess();
        
          return bResult;
        
        }; //isRunning
        
        
        bool
        Thread::isTerminating() {
        
          //if "terminate" flag is set and thread pointer
          //is still set we assume that thread is terminating 
          lockDataAccess();
          bool bIsTerminating = ((mThread != NULL) && mTerminate);
          unlockDataAccess();
        
          return bIsTerminating;
        
        }; //isTerminating
        
        
        void 
        Thread::threadTerminated() {
        
          lockDataAccess();
          mThread = NULL;
          //mTerminate = false;
          mWasKilled = true;
          TRACE(_T("Thread terminated (was killed %d).\n"), mWasKilled);
          unlockDataAccess(); 
          
        
        }; //threadTerminated
        
        
        void 
        Thread::done() {
        
          lockDataAccess();
          //mTerminate = false;
          mThread = NULL;
          TRACE(_T("Thread done.\n"));
          unlockDataAccess();
          
        
        }; //done
        
        
        
        
        void
        Thread::lockDataAccess() {
        
          mThreadCS.Lock();
        
        };
        
        
        void
        Thread::unlockDataAccess() {
        
          mThreadCS.Unlock();
        
        }; //unlockDataAccess
        
        
        bool 
        Thread::wasKilled() {
        
          lockDataAccess();
          bool result = mWasKilled;
          TRACE(_T("Thread::wasKilled %d\n"), mWasKilled);
          unlockDataAccess();
        
          return result;
        
        }; //
        
        
        bool 
        Thread::resetKilledFlag() {
        
          lockDataAccess();
          bool bWasKilled = mWasKilled;
          mWasKilled = false;
          TRACE(_T("SITread::resetKilledFlag %d\n"), mWasKilled);
          unlockDataAccess();
        
          return bWasKilled;
        
        }; //resetKilledFlag
        
        } //namespace common


}; //namespace jg