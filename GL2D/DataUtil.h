#pragma once
#include "tinyxml/tinyxml.h"
#include <string>

class DataUtil {
private:
	TiXmlDocument Doc{};
	std::string FilePathStr{};

public:
	void Init();
	void UpdateData(const char* CategoryName, const char* DataName, float Value);
	void UpdateStringData(const char* CategoryName, const char* DataName, const char* Value);
	float LoadData(const char* CategoryName, const char* DataName);
	const char* LoadStringData(const char* CategoryName, const char* DataName);

private:
	void CreateNewData();
	void RegisterNewData();

	void CheckDataVersion();
	void UpdateDataVersion(float VersionValue);

	void CreateDec(float VersionValue);
	void AddCategory(const char* CategoryName);
	void AddData(const char* CategoryName, const char* DataName, float Value);
	void AddStringData(const char* CategoryName, const char* DataName, const char* Value);
	void AddRoot(const char* RootName);

	void WriteData(TiXmlElement* CategoryVar, const char* DataName, float Value);
	void WriteStringData(TiXmlElement* CategoryVar, const char* DataName, const char* Value);

	float GetData(TiXmlElement* CategoryVar, const char* DataName);
	const char* GetStringData(TiXmlElement* CategoryVar, const char* DataName);

	TiXmlElement* FindRoot();
	TiXmlElement* FindCategory(const char* CategoryName);
};
extern DataUtil dataUtil;