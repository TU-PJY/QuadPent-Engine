#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Setting.h"
#include "tinyxml/tinyxml.h"
#include "cryptopp/aes.h" 
#include "cryptopp/filters.h"
#include "cryptopp/modes.h"
#include "cryptopp/osrng.h"
#include "cryptopp/hex.h"
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

enum class FileType
{ None, Secure, Script };

enum class DataType
{ Int, Float, String };


struct FileData {
	const char* CategoryName;
	const char* DataName;
	DataType DType;
	int IValue;
	float FValue;
	const char* StringValue;
};


class FileUtil {
private:
	FileType FType{ FileType::None };
	std::string FilePathStr{};
	std::filesystem::path FilePath{};
	TiXmlDocument Doc{};
	std::vector<FileData> DataListBuffer{};

	bool FileExist{};

public:
	void Init(const char* FolderName, const char* FileName, std::vector<FileData> List, FileType Type);
	void UpdateFloatData(const char* CategoryName, const char* DataName, float Value);
	void UpdateStringData(const char* CategoryName, const char* DataName, const char* Value);
	int LoadIntData(const char* CategoryName, const char* DataName);
	float LoadFloatData(const char* CategoryName, const char* DataName);
	const char* LoadStringData(const char* CategoryName, const char* DataName);
	void ResetData();


private:
	void SetupData(std::vector<FileData> List);
	void UpdateIntData(const char* CategoryName, const char* DataName, int Value);
	void CheckDataVersion();
	void UpdateDataVersion(float VersionValue);
	void CreateDec(float VersionValue);
	void AddRoot(const char* RootName);
	void AddCategory(const char* CategoryName);
	void AddFloatData(const char* CategoryName, const char* DataName, float Value);
	void AddIntData(const char* CategoryName, const char* DataName, int Value);
	void AddStringData(const char* CategoryName, const char* DataName, const char* Value);
	void WriteIntData(TiXmlElement* CategoryVar, const char* DataName, int Value);
	void WriteFloatData(TiXmlElement* CategoryVar, const char* DataName, float Value);
	void WriteStringData(TiXmlElement* CategoryVar, const char* DataName, const char* Value);
	int GetIntData(TiXmlElement* CategoryVar, const char* DataName);
	float GetFloatData(TiXmlElement* CategoryVar, const char* DataName);
	const char* GetStringData(TiXmlElement* CategoryVar, const char* DataName);
	TiXmlElement* FindRoot();
	TiXmlElement* FindCategory(const char* CategoryName);
	const char* FileUtil::FindData(const char* CategoryName, const char* DataName);
	bool LoadDataFile(const char* FileName);
	void UpdateDataFile();
	std::string Encrypt(const std::string& plainText, const byte key[], const byte iv[]);
	std::string Decrypt(const std::string& cipherText, const byte key[], const byte iv[]);
};