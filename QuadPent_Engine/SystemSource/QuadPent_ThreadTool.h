#pragma once
#include "QuadPent_Header.h"

namespace QP {
	class QuadPent_ThreadTool {
	public:
		void Create(QP::ThreadHandle& Handle, LPTHREAD_START_ROUTINE ThreadFunction, LPVOID Param = NULL);
		bool CheckRunning(QP::ThreadHandle& Handle);
		void Close(QP::ThreadHandle& Handle);
		void InitSection(CRITICAL_SECTION& Section);
		bool InitSectionAndCount(CRITICAL_SECTION& Section, int SpinCount);
		void DeleteSection(CRITICAL_SECTION& Section);
		void Lock(CRITICAL_SECTION& Section);
		void TryLock(CRITICAL_SECTION& Section);
		void Unlock(CRITICAL_SECTION& Section);
	};

	extern QP::QuadPent_ThreadTool ThreadTool;
}