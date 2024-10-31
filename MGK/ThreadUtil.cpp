#include "ThreadUtil.h"


void ThreadUtil::New(HANDLE& HandleValue, LPTHREAD_START_ROUTINE ThreadFunction, LPVOID Param) {
	HandleValue = CreateThread(NULL, 0, ThreadFunction, Param, 0, NULL);
}

bool ThreadUtil::GetState(HANDLE& HandleValue) {
	DWORD Result;
	GetExitCodeThread(HandleValue, &Result);

	if (Result == STILL_ACTIVE)
		return true;
	return false;
}

void ThreadUtil::Delete(HANDLE& HandleValue) {
	CloseHandle(HandleValue);
}

void ThreadUtil::InitSection(LPCRITICAL_SECTION& Section) {
	InitializeCriticalSection(Section);
}

void ThreadUtil::DeleteSection(LPCRITICAL_SECTION& Section) {
	DeleteCriticalSection(Section);
}

void ThreadUtil::Lock(LPCRITICAL_SECTION& Section) {
	EnterCriticalSection(Section);
}

void ThreadUtil::Unlock(LPCRITICAL_SECTION& Section) {
	LeaveCriticalSection(Section);
}