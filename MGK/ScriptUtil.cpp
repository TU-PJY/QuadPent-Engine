#include "ScriptUtil.h"

void ScriptUtil::Import(std::string FileName) {
	if (FileExist)
		return;

	if (!Doc.LoadFile(FileName.c_str())) {
		std::cout << "Failed to open script file" << std::endl;
		exit(EXIT_FAILURE);
	}

	Root = Doc.RootElement();
	FileExist = true;
}

void ScriptUtil::Release() {
	Doc.Clear();
	FileExist = false;
}

float ScriptUtil::LoadDigitData(std::string CategoryName, std::string DataName) {
	return GetDigitData(FindCategory(CategoryName), DataName);
}

std::string ScriptUtil::LoadStringData(std::string CategoryName, std::string DataName) {
	return GetStringData(FindCategory(CategoryName), DataName);
}

float ScriptUtil::GetDigitData(TiXmlElement* CategoryVar, std::string DataName) {
	const char* DataValue = CategoryVar->Attribute(DataName.c_str());
	if (DataValue)
		return std::stof(DataValue);
	else {
		std::cout << "Failed to find data" << std::endl;
		exit(EXIT_FAILURE);
	}
}

std::string ScriptUtil::GetStringData(TiXmlElement* CategoryVar, std::string DataName) {
	const char* DataValue = CategoryVar->Attribute(DataName.c_str());
	if (DataValue)
		return (std::string)DataValue;
	else {
		std::cout << "Failed to find data" << std::endl;
		exit(EXIT_FAILURE);
	}
}

TiXmlElement* ScriptUtil::FindCategory(std::string CategoryName) {
	return Root->FirstChildElement(CategoryName.c_str());
}

std::string ScriptUtil::FindData(std::string CategoryName, std::string DataName) {
	const char* DataValue = FindCategory(CategoryName)->Attribute(DataName.c_str());
	return DataValue ? (std::string)DataValue : "";
}