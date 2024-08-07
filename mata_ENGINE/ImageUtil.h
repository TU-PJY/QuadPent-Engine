#pragma once
#include "MATA_HEADER.h"
#include <string>
#include <map>

class ImageUtil {
private:
	GLuint VAO{};
	std::map<std::string, unsigned int> LoadedImageList;

public:
	void Init();
	void LoadImageFromList();
	void LoadImageFromFile(unsigned int& Image, const char* FileName);
	void SetImage(unsigned int& Image, std::string ImageName);
	void Render(unsigned int ImageVar);
};
extern ImageUtil imageUtil;