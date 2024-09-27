#define _CRT_SECURE_NO_WARNINGS
#include "DataUtil.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <filesystem>

//#define USE_CUSTOM_PATH
constexpr float APPLICATION_VERSION = 1.0;
constexpr const char* FOLDER_NAME = "DataFolder";
constexpr const char* FILE_NAME = "Data.xml";

#ifndef USE_CUSTOM_PATH
// data file path
//C://user//user name//FolderName//FileName
std::filesystem::path FolderPath = std::filesystem::path(getenv("USERPROFILE")) / FOLDER_NAME;
std::filesystem::path FilePath = FolderPath / FILE_NAME;
#endif

#ifdef USE_CUSTOM_PATH
// custom data file path
std::filesystem::path FolderPath = FOLDER_NAME;
std::filesystem::path FilePath = FolderPath / FILE_NAME;
#endif



void DataUtil::CreateNewData() {
	CreateDec(APPLICATION_VERSION);
	// add category and data here

	Doc.SaveFile(FilePathStr.c_str());
	std::cout << "New data file created on " << FilePathStr << std::endl;
}

void DataUtil::RegisterNewData() {
	// add category and data here
	Doc.SaveFile(FilePathStr.c_str());
}

///////////////////////////////



void DataUtil::Init() {
	FilePathStr = FilePath.string();

	if (!std::filesystem::exists(FolderPath)) {
		if (!std::filesystem::create_directory(FolderPath)) {
			std::cout << "Falied to find folder" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	if (!Doc.LoadFile(FilePathStr.c_str()))
		CreateNewData();
	else
		CheckDataVersion();
}

void DataUtil::UpdateData(const char* CategoryName, const char* DataName, float Value) {
	TiXmlElement* DataCategory = FindCategory(CategoryName);
	WriteData(DataCategory, CategoryName, Value);
	Doc.SaveFile(FilePathStr.c_str());
}

void DataUtil::UpdateStringData(const char* CategoryName, const char* DataName, const char* Value) {
	TiXmlElement* DataCategory = FindCategory(CategoryName);
	WriteStringData(DataCategory, DataName, Value);
	Doc.SaveFile(FilePathStr.c_str());
}

float DataUtil::LoadData(const char* CategoryName, const char* DataName) {
	TiXmlElement* DataCategory = FindCategory(CategoryName);
	return GetData(DataCategory, DataName);
}

const char* DataUtil::LoadStringData(const char* CategoryName, const char* DataName) {
	TiXmlElement* DataCategory = FindCategory(CategoryName);
	return GetStringData(DataCategory, DataName);
}

void DataUtil::CheckDataVersion() {
	TiXmlNode* DeclNode = Doc.FirstChild();
	TiXmlDeclaration* Decl = DeclNode->ToDeclaration();

	if (Decl) {
		const char* Version = Decl->Version();
		if (std::stof(Version) < APPLICATION_VERSION)
			UpdateDataVersion(APPLICATION_VERSION);
	}
	else {
		std::cout << "Failed to find data version" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void DataUtil::UpdateDataVersion(float VersionValue) {
	TiXmlNode* DeclNode = Doc.FirstChild();
	TiXmlDeclaration* Decl = DeclNode->ToDeclaration();

	const char* Version = Decl->Version();
	const char* Encoding = Decl->Encoding();
	const char* StandAlone = Decl->Standalone();

	std::ostringstream OSS;
	OSS << std::fixed << std::setprecision(1) << VersionValue;
	std::string NewVersionStr = OSS.str();

	TiXmlDeclaration NewDecl(NewVersionStr.c_str(), Encoding ? Encoding : "", StandAlone ? StandAlone : "");

	Doc.ReplaceChild(DeclNode, NewDecl);
	Doc.SaveFile(FilePathStr.c_str());

	RegisterNewData();
}

void DataUtil::CreateDec(float VersionValue) {
	std::ostringstream OSS;
	OSS << std::fixed << std::setprecision(1) << VersionValue;
	std::string NewVersionStr = OSS.str();

	TiXmlDeclaration* Dec = new TiXmlDeclaration(NewVersionStr.c_str(), "", "");
	Doc.LinkEndChild(Dec);
}

void DataUtil::AddCategory(const char* CategoryName) {
	TiXmlElement* Root = FindRoot();
	TiXmlElement* FileCategory = new TiXmlElement(CategoryName);
	Root->LinkEndChild(FileCategory);
}

void DataUtil::AddData(const char* CategoryName, const char* DataName, float Value) {
	TiXmlElement* FileCategory = FindCategory(CategoryName);
	FileCategory->SetDoubleAttribute(DataName, Value);
}

void DataUtil::AddStringData(const char* CategoryName, const char* DataName, const char* Value) {
	TiXmlElement* FileCategory = FindCategory(CategoryName);
	FileCategory->SetAttribute(DataName, Value);
}

void DataUtil::AddRoot(const char* RootName) {
	TiXmlElement* FileRoot = new TiXmlElement(RootName);
	Doc.LinkEndChild(FileRoot);
}

void DataUtil::WriteData(TiXmlElement* CategoryVar, const char* DataName, float Value) {
	const char* DataValue = CategoryVar->Attribute(DataName);
	if (DataValue)
		CategoryVar->SetDoubleAttribute(DataName, Value);
	else {
		std::cout << "Failed to update data" << Doc.ErrorDesc() << std::endl;
		exit(EXIT_FAILURE);
	}
}

void DataUtil::WriteStringData(TiXmlElement* CategoryVar, const char* DataName, const char* Value) {
	const char* DataValue = CategoryVar->Attribute(DataName);
	if (DataValue)
		CategoryVar->SetAttribute(DataName, Value);
	else {
		std::cout << "Failed to update data" << Doc.ErrorDesc() << std::endl;
		exit(EXIT_FAILURE);
	}
}

float DataUtil::GetData(TiXmlElement* CategoryVar, const char* DataName) {
	const char* DataValue = CategoryVar->Attribute(DataName);
	if (CategoryVar)
		return std::stof(DataValue);
	else {
		std::cout << "Failed to find data" << Doc.ErrorDesc() << std::endl;
		exit(EXIT_FAILURE);
	}
}

const char* DataUtil::GetStringData(TiXmlElement* CategoryVar, const char* DataName) {
	const char* DataValue = CategoryVar->Attribute(DataName);
	if (CategoryVar)
		return DataValue;
	else {
		std::cout << "Failed to find data" << Doc.ErrorDesc() << std::endl;
		exit(EXIT_FAILURE);
	}
}

TiXmlElement* DataUtil::FindRoot() {
	TiXmlElement* Root = Doc.RootElement();
	if (!Root) {
		std::cout << "Failed to find root" << std::endl;
		exit(EXIT_FAILURE);
	}
	return Root;
}

TiXmlElement* DataUtil::FindCategory(const char* CategoryName) {
	TiXmlElement* DataCat = FindRoot()->FirstChildElement(CategoryName);
	if (!DataCat) {
		std::cout << "Failed to find category" << std::endl;
		exit(EXIT_FAILURE);
	}
	return DataCat;
}