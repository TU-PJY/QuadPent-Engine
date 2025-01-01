#pragma once
#include "EngineHeader.h"

class FontUtil {
public:
	bool Load(const std::string& fontFilePath, bool LoadInThread);
};
extern FontUtil fontUtil;
