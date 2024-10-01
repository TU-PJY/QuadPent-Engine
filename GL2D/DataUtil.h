#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Setting.h"

#ifdef USE_DATA_SYSTEM
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "tinyxml/tinyxml.h"
#include "cryptopp/aes.h" 
#include "cryptopp/filters.h"
#include "cryptopp/modes.h"
#include "cryptopp/osrng.h"
#include "cryptopp/hex.h"
using namespace CryptoPP;


class DataUtil {
private:
	TiXmlDocument Doc{};
	std::string FilePathStr{};

	std::filesystem::path DebugFilePath{};
	std::string DebugFilePathStr{};

public:
	void Init();
	void UpdateData(const char* CategoryName, const char* DataName, float Value);
	void UpdateStringData(const char* CategoryName, const char* DataName, const char* Value);
	float LoadData(const char* CategoryName, const char* DataName);
	const char* LoadStringData(const char* CategoryName, const char* DataName);
	void ResetData();


private:
	void CreateNewData();
	void RegisterNewData();
	void CheckDataVersion();
	void UpdateDataVersion(float VersionValue);
	void CreateDec(float VersionValue);
	void AddRoot(const char* RootName);
	void AddCategory(const char* CategoryName);
	void AddData(const char* CategoryName, const char* DataName, float Value = 0.0);
	void AddStringData(const char* CategoryName, const char* DataName, const char* Value);
	void WriteData(TiXmlElement* CategoryVar, const char* DataName, float Value);
	void WriteData(TiXmlElement* CategoryVar, const char* DataName, int Value);
	void WriteStringData(TiXmlElement* CategoryVar, const char* DataName, const char* Value);
	float GetData(TiXmlElement* CategoryVar, const char* DataName);
	const char* GetStringData(TiXmlElement* CategoryVar, const char* DataName);
	TiXmlElement* FindRoot();
	TiXmlElement* FindCategory(const char* CategoryName);
	bool LoadDataFile(const char* FileName);
	void UpdateDataFile();
	std::string Encrypt(const std::string& plainText, const byte key[], const byte iv[]);
	std::string Decrypt(const std::string& cipherText, const byte key[], const byte iv[]);
};

extern DataUtil dataUtil;
#endif