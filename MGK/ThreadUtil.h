#pragma once
#include "EngineHeader.h"

namespace ThreadUtil {
	void New(HANDLE& HandleValue, LPTHREAD_START_ROUTINE ThreadFunction, LPVOID Param=NULL);
	bool GetState(HANDLE& HandleValue);
	void Delete(HANDLE& HandleValue);
	void InitSection(LPCRITICAL_SECTION& Section);
	void DeleteSection(LPCRITICAL_SECTION& Section);
	void Lock(LPCRITICAL_SECTION& Section);
	void Unlock(LPCRITICAL_SECTION& Section);
}