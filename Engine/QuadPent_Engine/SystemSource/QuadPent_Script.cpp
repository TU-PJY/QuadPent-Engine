#include "QuadPent_Script.h"
#include "QuadPent_Scene.h"
#include "QuadPent_StringTool.h"

void QP::Script::Load(std::string FileName) {
	if (FileExist)
		return;

	if (!Doc.LoadFile(FileName.c_str(), TIXML_ENCODING_UTF8)) {
		Scene.SetErrorScreen(ERROR_TYPE_SCRIPT_LOAD, FileName);
		return;
	}

	Root = Doc.RootElement();
	std::cout << "Script util opened file: " << FileName << std::endl;
	FileExist = true;
}

void QP::Script::LoadSecure(std::string FileName) {
	std::ifstream EncryptedFile(FileName, std::ios::binary);
	if (!EncryptedFile) {
		Scene.SetErrorScreen(ERROR_TYPE_SCRIPT_LOAD, FileName);
		return;
	}

	std::string EncryptedContent((std::istreambuf_iterator<char>(EncryptedFile)), std::istreambuf_iterator<char>());
	EncryptedFile.close();

	std::string DecryptedXML = Decrypt(EncryptedContent, AES_KEY, IV_KEY);
	Doc.Parse(DecryptedXML.c_str(), nullptr, TIXML_ENCODING_UTF8);

	if (Doc.Error()) {
		Scene.SetErrorScreen(ERROR_TYPE_SCRIPT_PARSE, FileName);
		return;
	}

	Root = Doc.RootElement();
	FileExist = true;
}

void QP::Script::Release() {
	Doc.Clear();
	FileExist = false;
}

int QP::Script::CountCategory() {
	int CatCount{};
	for (TiXmlElement* Element = Root->FirstChildElement(); Element != nullptr; Element = Element->NextSiblingElement())
		CatCount++;

	return CatCount;
}

float QP::Script::LoadDigitData(std::string CategoryName, std::string DataName) {
	CategorySearch = CategoryName;
	DataSearch = DataName;
	return GetDigitData(FindCategory(CategoryName), DataName);
}

std::string QP::Script::LoadStringData(std::string CategoryName, std::string DataName) {
	CategorySearch = CategoryName;
	DataSearch = DataName;
	return GetStringData(FindCategory(CategoryName), DataName);
}

std::wstring QP::Script::LoadWstringData(std::string CategoryName, std::string DataName) {
	return StringTool.Wstring(LoadStringData(CategoryName, DataName));
}

QP::DigitDataVec QP::Script::LoadCategoryDigitData(std::string CategoryName) {
	DigitDataVec LoadedData{};
	TiXmlElement* Category = FindCategory(CategoryName);

	if (!Category) {
		Scene.SetErrorScreen(ERROR_TYPE_SCRIPT_CATEGORY, CategoryName);
		return {};
	}

	TiXmlAttribute* Attribute = Category->FirstAttribute();
	while (Attribute) {
		LoadedData.emplace_back(std::stof(Attribute->Value()));
		Attribute = Attribute->Next();
	}

	return LoadedData;
}

QP::StringDataVec QP::Script::LoadCategoryStringData(std::string CategoryName) {
	StringDataVec LoadedData{};
	TiXmlElement* Category = FindCategory(CategoryName);

	if (!Category) {
		Scene.SetErrorScreen(ERROR_TYPE_SCRIPT_CATEGORY, CategoryName);
		return {};
	}

	TiXmlAttribute* Attribute = Category->FirstAttribute();
	while (Attribute) {
		LoadedData.emplace_back((std::string)Attribute->Value());
		Attribute = Attribute->Next();
	}

	return LoadedData;
}

//////////////////////////////// private
float QP::Script::GetDigitData(TiXmlElement* CategoryVar, std::string DataName) {
	if (!CategoryVar) {
		Scene.SetErrorScreen(ERROR_TYPE_SCRIPT_CATEGORY, CategorySearch);
		return 0.0;
	}

	const char* DataValue = CategoryVar->Attribute(DataName.c_str());
	if (DataValue)
		return std::stof(DataValue);
	else {
		Scene.SetErrorScreen(ERROR_TYPE_SCRIPT_DATA, CategorySearch, DataSearch);
		return 0.0;
	}
}

std::string QP::Script::GetStringData(TiXmlElement* CategoryVar, std::string DataName) {
	if (!CategoryVar) {
		Scene.SetErrorScreen(ERROR_TYPE_SCRIPT_CATEGORY, CategorySearch);
		return "";
	}

	const char* DataValue = CategoryVar->Attribute(DataName.c_str());
	if (DataValue)
		return (std::string)DataValue;
	else {
		Scene.SetErrorScreen(ERROR_TYPE_SCRIPT_DATA, CategorySearch, DataSearch);
		return "";
	}
}

TiXmlElement* QP::Script::FindCategory(std::string CategoryName) {
	return  Root->FirstChildElement(CategoryName.c_str());
}

std::string QP::Script::Decrypt(const std::string& CipherText, const byte Key[], const byte IV[]) {
	std::string PlainText;

	try {
		CBC_Mode<AES>::Decryption Decryption;
		Decryption.SetKeyWithIV(Key, AES::DEFAULT_KEYLENGTH, IV);
		StringSource(CipherText, true, new StreamTransformationFilter(Decryption, new StringSink(PlainText)));
	}
	catch (const Exception& E) {
		std::cerr << E.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	return PlainText;
}