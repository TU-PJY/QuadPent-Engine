#pragma once
#include "EngineHeader.h"

namespace ThreadUtil {
	void New(HANDLE& HandleValue, LPTHREAD_START_ROUTINE ThreadFunction, LPVOID Param=NULL);
	bool GetState(HANDLE& HandleValue);
	void Delete(HANDLE& HandleValue);
}