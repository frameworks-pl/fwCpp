#ifndef THREAD_H_INCLUDED
#define THREAD_H_INCLUDED

#include <afxmt.h>  //CCriticalSection


namespace fw 
{

    namespace core
    {

        /**
          @brief This is base class for any "thread info" class
          it provided methods to synchronize two threads 
          (force 2nd to stop and notify 1st when it is done)
        
          //NODE: NOT tested with more than two threads working toghether
        */
        class Thread : public CObject 
        {
        
          public:
          
            Thread();
        
            /**
              Starts the thread - calls AfxBeginThread and remembers the pointer
              to the thread
            */
            void start(AFX_THREADPROC pfnThreadProc, LPVOID pParam);
        
        
            /** Sets/clears the terminated flag, in a thread-safe way */
            void terminate(bool pTerminate = true, bool pPompMessages = false);

			//non waiting terminate
			void requestTerminate();

			//reset terminate flag
			void resetTerminateFlag();
        
            /**
              Returns true if the the thread is in the "terminating" state,
              that is mThread pointer is still set but mTerminate flag is set as well
            */
            bool isTerminating();
        
            /**
              Returns true if the mTerminate flag is set (in a thread-safe way)
            */
            bool stopRequest();
        
            /**
              Returns true if the thread is running (see mThread for details)
            */
            bool isRunning();
        
            /**
              Unlocks the other thread (by setting the event)
            */
            void unlock();
        
            /**
              Waits for an event to be signaled.
              The method can simply wait by putting lock on an event 
              (pPompMessages = false) or it can wait in more efficient
              way with message pomp (pPompMessages = true)
            */
            void lock(bool pPumpMessages);
        
            /**
              This method should be called if the thread completed its work.
              It will set mWasKilled variable to false meaning it did not 
              stopped on request from outside
            */
            void done();
        
            /**
              This method should be called when thread ended its work
              before it completed the task.
              The function will set mWasKilled to true (indicating that
              the thread should be restarted as soon as possible)
            */
            void threadTerminated();
        
            /**
              Return true if the last run of the thread ended premature 
              (was terminated) or false if the thread ended in the
              'natrual' way (made it to the end)
            */
            bool wasKilled();
        
            /**
               Resets the 'killed' flag - sets it to false.
               The function returns the state of the flag
               before it was reseted.
            */
            bool resetKilledFlag();

            /**
              Locks the thread's critical section to protect shared data fields
              Use this method to protect inter-thread access to your own
              data!
            */
            void lockDataAccess();
        
            /**
              Unlocks the thread's critical section.
              Use this method to protect inter-thread access to your own
              data (not that this method should always be matched with
              corresponding lockDataAccess call)
            */
            void unlockDataAccess();
        
          protected:
        
            /**
              Pointer to the thread, if it is not NULL
              it means that the thread is currently running.
        
              start method initizlizes this files (so thread should alwyas be started by this method)
              The thread function should set the tread pointer back to NULL when it is ending!
            */
            CWinThread* mThread;
        
            /**
              A flag indicating that someone wants the thread to stop
            */
            bool mTerminate;
        
            /**
              Critical section to protect all data that might be used
              concurrently
            */
            CCriticalSection mThreadCS;
        
            /**
              An pointer to event used to synchronize main process with a worker thread
              (usually to wait for the thread until it is terminated)
            */
            CEvent* mWaitEvent;
        
            /**
              This to store information whether the tread ended
              in the 'natural' way or was terminated
            */
            bool mWasKilled;
        
        }; //class


		class ThreadDataLock
		{
			public:
				ThreadDataLock(Thread* pThread) : m_pThread(pThread) 
				{ 
					m_pThread->lockDataAccess();
				}
				virtual ~ThreadDataLock()
				{
					m_pThread->unlockDataAccess();
					m_pThread = NULL;
				}
			protected:
				Thread* m_pThread;
		};
        
    } //namespace common

}; //namespace  jg


#endif //THREAD_H_INCLUDED