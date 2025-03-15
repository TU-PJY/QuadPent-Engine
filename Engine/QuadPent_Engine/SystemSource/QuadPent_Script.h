#pragma once
#include "QuadPent_Header.h"

// read only

namespace QP {
	class Script {
	private:
		TiXmlDocument Doc{};
		TiXmlElement* Root{};
		bool          FileExist{};

		std::string   CategorySearch{};
		std::string   DataSearch{};

	public:
		Script() {}

		void Load(std::string FileName);
		void LoadSecure(std::string FileName);
		float LoadDigitData(std::string CategoryName, std::string DataName);
		std::string LoadStringData(std::string CategoryName, std::string DataName);
		std::wstring LoadWstringData(std::string CategoryName, std::string DataName);
		int CountCategory();
		QP::DigitDataVec LoadCategoryDigitData(std::string CategoryName);
		QP::StringDataVec LoadCategoryStringData(std::string CategoryName);
		void Release();


	private:
		TiXmlElement* FindCategory(std::string CategoryName);
		std::string Decrypt(const std::string& CipherText, const byte Key[], const byte IV[]);
		float GetDigitData(TiXmlElement* CategoryVar, std::string DataName);
		std::string GetStringData(TiXmlElement* CategoryVar, std::string DataName);
	};
}