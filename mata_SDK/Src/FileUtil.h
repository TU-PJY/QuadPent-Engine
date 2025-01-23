#pragma once
#include "SystemConfig.h"

#ifdef USE_FILE_SYSTEM
#define _CRT_SECURE_NO_WARNINGS
#include "Config.h"
#include "Library/tinyxml/tinyxml.h"
#include "Library/cryptopp/aes.h" 
#include "Library/cryptopp/filters.h"
#include "Library/cryptopp/modes.h"
#include "Library/cryptopp/osrng.h"
#include "Library/cryptopp/hex.h"
#include "SDKHeader.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <cctype>
using namespace CryptoPP;

// write and read
class FileUtil {
private:
	std::string   FilePath{};
	TiXmlDocument Doc{};
	TiXmlElement* Root{};
	
	DataFormat    DataFormatInfo{};

	bool          FileExist{};

	std::string   CategorySearch{};
	std::string   DataSearch{};

public:
	FileUtil() {}

	void Load(std::string FileName, DataFormat Format);
	void UpdateDigitData(std::string CategoryName, std::string DataName, float Value);
	void UpdateStringData(std::string CategoryName, std::string DataName, std::string Value);
	float LoadDigitData(std::string CategoryName, std::string DataName);
	std::string LoadStringData(std::string CategoryName, std::string DataName);
	std::vector<float> LoadCategoryDigitData(std::string CategoryName);
	std::vector<std::string> LoadCategoryStringData(std::string CategoryName);
	void ResetData();
	void Release();

private:
	void SetupData();
	void CheckDataVersion();
	void UpdateDataVersion(float VersionValue);
	void CreateDec(float VersionValue);
	void AddRoot(std::string RootName);
	void AddCategory(std::string CategoryName);
	void AddDigitData(std::string CategoryName, std::string DataName, float Value);
	void AddStringData(std::string CategoryName, std::string DataName, std::string Value);
	void WriteDigitData(TiXmlElement* CategoryVar, std::string DataName, float Value);
	void WriteStringData(TiXmlElement* CategoryVar, std::string DataName, std::string Value);
	float GetDigitData(TiXmlElement* CategoryVar, std::string DataName);
	std::string GetStringData(TiXmlElement* CategoryVar, std::string DataName);
	TiXmlElement* FindRoot();
	TiXmlElement* FindCategory(std::string CategoryName);
	std::string FileUtil::FindData(std::string CategoryName, std::string DataName);
	bool LoadDataFile(std::string FileName);
	void UpdateDataFile();
	std::string Encrypt(const std::string& PlainText, const byte Key[], const byte IV[]);
	std::string Decrypt(const std::string& CipherText, const byte Key[], const byte IV[]);
	std::string GetFileName(const std::string& FileDirectory);
	std::string GetFolderPath(const std::string& FileDirectory, const std::string& RemoveString);
};
#endif