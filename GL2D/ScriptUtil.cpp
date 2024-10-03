#include "ScriptUtil.h"

void ScriptUtil::Import(const char* FileName) {
	if (FileExist)
		return;

	if (!Doc.LoadFile(FileName)) {
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

int ScriptUtil::LoadIntData(const char* CategoryName, const char* DataName) {
	return GetIntData(FindCategory(CategoryName), DataName);
}

float ScriptUtil::LoadFloatData(const char* CategoryName, const char* DataName) {
	return GetFloatData(FindCategory(CategoryName), DataName);
}

const char* ScriptUtil::LoadStringData(const char* CategoryName, const char* DataName) {
	return GetStringData(FindCategory(CategoryName), DataName);
}

int ScriptUtil::GetIntData(TiXmlElement* CategoryVar, const char* DataName) {
	const char* DataValue = CategoryVar->Attribute(DataName);
	if (DataValue)
		return std::stoi(DataValue);
	else {
		std::cout << "Failed to find data" << std::endl;
		exit(EXIT_FAILURE);
	}
}

float ScriptUtil::GetFloatData(TiXmlElement* CategoryVar, const char* DataName) {
	const char* DataValue = CategoryVar->Attribute(DataName);
	if (DataValue)
		return std::stof(DataValue);
	else {
		std::cout << "Failed to find data" << std::endl;
		exit(EXIT_FAILURE);
	}
}

const char* ScriptUtil::GetStringData(TiXmlElement* CategoryVar, const char* DataName) {
	const char* DataValue = CategoryVar->Attribute(DataName);
	if (DataValue)
		return DataValue;
	else {
		std::cout << "Failed to find data" << std::endl;
		exit(EXIT_FAILURE);
	}
}

TiXmlElement* ScriptUtil::FindCategory(const char* CategoryName) {
	return Root->FirstChildElement(CategoryName);
}

const char* ScriptUtil::FindData(const char* CategoryName, const char* DataName) {
	return FindCategory(CategoryName)->Attribute(DataName);
}