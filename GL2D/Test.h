#pragma once
#include "FWL.h"
#include "ImageUtil.h"

class Test : public OBJ {
	unsigned int image = imageUtil.ImportImage("sample.png");

	void Render() {
		imageUtil.Draw(image);
	}
};