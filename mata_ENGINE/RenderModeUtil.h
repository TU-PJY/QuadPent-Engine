#pragma once
#include "MATA_HEADER.h"

class RenderModeUtil {
public:
	void SetTextMode();
	void SetImageMode();
	void SetStaticImageMode();
	void SetStaticTextMode();
};
extern RenderModeUtil renderMode;