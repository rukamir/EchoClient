#include "ThreadController.h"

ThreadController::~ThreadController()
{
	for(const auto &th : m_vThreads){
		delete th;
	}

	m_mThreads;
}

DWORD ThreadController::CreateNewThread(void* funcProc, void* funcParam, bool startSuspended)
{
	DWORD threadID;
	if(startSuspended){
		handle = CreateThread(NULL,
								0,
								(LPTHREAD_START_ROUTINE)funcProc,		// func pointer
								funcParam,								// fun params
								CREATE_SUSPENDED,						// start suspended
								&threadID);
	} else {
		handle = CreateThread(NULL,
								0,
								(LPTHREAD_START_ROUTINE)funcProc,		// func pointer
								funcParam,								// fun params
								0,										// starts now
								&threadID);
	}

	thInfo	=	new ThreadInfo(handle, THREAD_PRIORITY_NORMAL, startSuspended);
	thInfo->ID	=	threadID;

	m_mThreads.insert(make_pair(threadID, thInfo));

	return threadID;
}

void ThreadController::ResumeThread(DWORD threadID){
	ResumeThread(m_mThreads.find(threadID)->second->ID);
	m_mThreads.find(threadID)->second->suspended = true;
}

void ThreadController::SuspendThread(DWORD threadID){
	SuspendThread(m_mThreads.find(threadID)->second->ID);
	m_mThreads.find(threadID)->second->suspended = true;
}