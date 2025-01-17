#pragma once
#include "SDKHeader.h"

class ThreadUtil {
public:
	void Create(HANDLE& HandleValue, LPTHREAD_START_ROUTINE ThreadFunction, LPVOID Param = NULL);
	bool IsRunning(HANDLE& HandleValue);
	void Close(HANDLE& HandleValue);
	void InitSection(CRITICAL_SECTION& Section);
	bool InitSectionAndCount(CRITICAL_SECTION& Section, int SpinCount);
	void DeleteSection(CRITICAL_SECTION& Section);
	void Lock(CRITICAL_SECTION& Section);
	void TryLock(CRITICAL_SECTION& Section);
	void Unlock(CRITICAL_SECTION& Section);
};
extern ThreadUtil threadUtil;