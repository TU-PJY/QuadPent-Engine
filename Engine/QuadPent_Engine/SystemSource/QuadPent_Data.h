#pragma once
#include "QuadPent_Header.h"

// write and read
namespace QP {
	class Data {
	private:
		std::string   FilePath{};
		TiXmlDocument Doc{};
		TiXmlElement* Root{};

		DataFormat    DataFormatInfo{};
		std::vector<FileData> Buffer{};

		std::string   CategorySearch{};
		std::string   DataSearch{};

		bool          FileExist{};

	public:
		Data() {}

		void Load(std::string FileName, DataFormat Fmt);
		void UpdateDigitData(std::string CategoryName, std::string DataName, float Value);
		void UpdateStringData(std::string CategoryName, std::string DataName, std::string Value);
		float LoadDigitData(std::string CategoryName, std::string DataName);
		std::string LoadStringData(std::string CategoryName, std::string DataName);
		std::wstring LoadWStringData(std::string Categoryname, std::string DataName);
		QP::DigitDataVec LoadCategoryDigitData(std::string CategoryName);
		QP::StringDataVec LoadCategoryStringData(std::string CategoryName);
		void ResetData();
		void Release();

	private:
		void SetupData();
		void CheckVersion();
		void UpdateVersion();
		void CreateDec();
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
		bool CheckCategoryExist(std::string CategoryName);
		bool Data::CheckDataExist(std::string CategoryName, std::string DataName);
		bool LoadDataFile(std::string FileName);
		void SaveDataChanges();
		std::string Encrypt(const std::string& PlainText, const byte Key[], const byte IV[]);
		std::string Decrypt(const std::string& CipherText, const byte Key[], const byte IV[]);
		std::string GetFileName(const std::string& FileDirectory);
		std::string GetFolderPath(const std::string& FileDirectory, const std::string& RemoveString);
	};
}