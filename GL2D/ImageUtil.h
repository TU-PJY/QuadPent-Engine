#pragma once
#include "GLHeader.h"

class ImageUtil {
private:
	GLuint VAO{};

public:
	void Init();
	unsigned int ImportImage(const char* directory, bool nearest_opt = false);
	void Draw(unsigned int ImageVar);
};
extern ImageUtil imageUtil;