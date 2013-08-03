#pragma once
#include <Windows.h>
#include <map>
#include <vector>

using namespace std;

#define ThrdCon ThreadController::Instance()


class ThreadController
{
private:
	struct ThreadInfo{
		DWORD	ID;
		HANDLE	hThread;
		int		priority;
		bool	suspended;
		ThreadInfo(HANDLE ht, int p, bool s) {
			hThread = ht;
			priority = p;
			suspended = s;
		}
	};
	ThreadInfo*					thInfo;
	HANDLE		handle;
	map<DWORD, ThreadInfo*>		m_mThreads;
	vector<ThreadInfo*>			m_vThreads;
public:
	ThreadController(){}
	~ThreadController();

	// Returns thread id
	DWORD		CreateNewThread(void* funcProc, void* funcParam, bool startSuspended);
	void		ResumeThread(DWORD threadID);
	void		SuspendThread(DWORD threadID);

	// Singleton
	static ThreadController* Instance(){
		static ThreadController instance; 
		return &instance;
	}
};