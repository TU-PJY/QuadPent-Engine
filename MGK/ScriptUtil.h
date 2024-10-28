#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "tinyxml/tinyxml.h"
#include <iostream>
#include <string>

// read only
class ScriptUtil {
private:
	TiXmlDocument Doc{};
	TiXmlElement* Root{};
	bool FileExist{};

public:
	void Import(std::string FileName);
	float LoadDigitData(std::string CategoryName, std::string DataName);
	std::string LoadStringData(std::string CategoryName, std::string DataName);
	void Release();

private:
	TiXmlElement* FindCategory(std::string CategoryName);
	std::string FindData(std::string CategoryName, std::string DataName);
	float GetDigitData(TiXmlElement* CategoryVar, std::string DataName);
	std::string GetStringData(TiXmlElement* CategoryVar, std::string DataName);
};