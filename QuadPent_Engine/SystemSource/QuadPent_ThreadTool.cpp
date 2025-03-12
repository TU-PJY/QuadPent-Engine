#include "QuadPent_ThreadTool.h"

QP::QuadPent_ThreadTool QP::ThreadTool;

void QP::QuadPent_ThreadTool::Create(QP::ThreadHandle& Handle, LPTHREAD_START_ROUTINE ThreadFunction, LPVOID Param) {
	Handle = CreateThread(NULL, 0, ThreadFunction, Param, 0, NULL);
}

bool QP::QuadPent_ThreadTool::CheckRunning(QP::ThreadHandle& Handle) {
	if (!Handle)
		return false;

	DWORD Result;
	GetExitCodeThread(Handle, &Result);

	if (Result == STILL_ACTIVE)
		return true;

	return false;
}

void QP::QuadPent_ThreadTool::Close(QP::ThreadHandle& Handle) {
	if(Handle)
		CloseHandle(Handle);
}

void QP::QuadPent_ThreadTool::InitSection(CRITICAL_SECTION& Section) {
	InitializeCriticalSection(&Section);
}

bool QP::QuadPent_ThreadTool::InitSectionAndCount(CRITICAL_SECTION& Section, int SpinCount) {
	return InitializeCriticalSectionAndSpinCount(&Section, SpinCount);
}

void QP::QuadPent_ThreadTool::DeleteSection(CRITICAL_SECTION& Section) {
	DeleteCriticalSection(&Section);
}

void QP::QuadPent_ThreadTool::Lock(CRITICAL_SECTION& Section) {
	EnterCriticalSection(&Section);
}

void QP::QuadPent_ThreadTool::TryLock(CRITICAL_SECTION& Section) {
	TryEnterCriticalSection(&Section);
}

void QP::QuadPent_ThreadTool::Unlock(CRITICAL_SECTION& Section) {
	LeaveCriticalSection(&Section);
}