#pragma once
#include "EngineHeader.h"

class FontUtil {
public:
	bool Import(const std::string& fontFilePath, bool LoadInThread);
};
extern FontUtil fontUtil;
