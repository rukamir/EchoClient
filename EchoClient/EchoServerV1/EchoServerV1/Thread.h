#pragma once

#include <Windows.h>
#include <iostream>
#include <assert.h>
#include <process.h>

using namespace std;

class Runnable {
public:
	virtual void* run() = 0;
	virtual ~Runnable() = 0;
};

// Pure virtual destructor: function body required
Runnable::~Runnable(){};

class Thread {
public:
	Thread(std::auto_ptr<Runnable> run);
	Thread();
	virtual ~Thread();
	void start();
	void* join();
private:
	HANDLE hThread;
	unsigned wThreadID;
	// runnable object will be deleted automatically
	std::auto_ptr<Runnable> runnable;
	Thread(const Thread&);
	const Thread& operator=(const Thread&);
	// called when run() completes
	void setCompleted();
	// stores return value from run()
	void* result;
	virtual void* run() {return 0;}
	static unsigned WINAPI startThreadRunnable(LPVOID pVoid);
	static unsigned WINAPI startThread(LPVOID pVoid);
	void printError(LPTSTR lpszFunction, LPSTR fileName, int lineNumber);
};


//int main() {
//	// thread1 and thread2 are created on the heap
//	// thread3 is created on the stack
//	// The destructor for thread1 and thread2 will automatically 
//	// delete the thread objects.
//	std::auto_ptr<Runnable> r(new simpleRunnable(1));
//	std::auto_ptr<Thread> thread1(new Thread(r));
//	thread1->start();
//	std::auto_ptr<simpleThread> thread2(new simpleThread(2));
//	thread2->start();
//	simpleThread thread3(3);
//	thread3.start();
//	// wait for the threads to finish
//	int result1 = reinterpret_cast<int>(thread1->join());
//	int result2 = reinterpret_cast<int>(thread2->join());
//	int result3 = reinterpret_cast<int>(thread3.join());
//	std::cout << result1 << ' ' << result2 << ' ' << result3 
//		<< std::endl;
//	return 0;
//}