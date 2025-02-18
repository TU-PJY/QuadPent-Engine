#include "SDK_Data.h"
#include "SDK_Scene.h"
#include "SDK_StringTool.h"

void MSDK::Data::Load(std::string FileDirectory, DataFormat Fmt) {
	if (FileExist)
		return;

	std::string FolderPath = GetFolderPath(FileDirectory, GetFileName(FileDirectory));
	FilePath = FileDirectory;
	DataFormatInfo = Fmt;

	if (!USE_FILE_SECURITY)
		FilePath += ".xml";

	if (!std::filesystem::exists(FolderPath))
		std::filesystem::create_directories(FolderPath);

	if (!LoadDataFile(FilePath)) {
		SetupData();
		return;
	}

	Root = FindRoot();
	CheckDataVersion();
	std::cout << "File util opened file: " << FilePath << std::endl;
	FileExist = true;
}

void MSDK::Data::UpdateDigitData(std::string CategoryName, std::string DataName, float Value) {
	CategorySearch = CategoryName;
	DataSearch = DataName;
	WriteDigitData(FindCategory(CategoryName), DataName, Value);
	UpdateDataFile();
}

void MSDK::Data::UpdateStringData(std::string CategoryName, std::string DataName, std::string Value) {
	CategorySearch = CategoryName;
	DataSearch = DataName;
	WriteStringData(FindCategory(CategoryName), DataName, Value);
	UpdateDataFile();
}

float MSDK::Data::LoadDigitData(std::string CategoryName, std::string DataName) {
	CategorySearch = CategoryName;
	DataSearch = DataName;
	return GetDigitData(FindCategory(CategoryName), DataName);
}

std::string MSDK::Data::LoadStringData(std::string CategoryName, std::string DataName) {
	CategorySearch = CategoryName;
	DataSearch = DataName;
	return GetStringData(FindCategory(CategoryName), DataName);
}

std::wstring MSDK::Data::LoadWStringData(std::string Categoryname, std::string DataName) {
	return StringTool.Wstring(LoadStringData(Categoryname, DataName));
}

MSDK::DigitDataVec MSDK::Data::LoadCategoryDigitData(std::string CategoryName) {
	DigitDataVec LoadedData{};
	TiXmlElement* Category = FindCategory(CategoryName);

	if (!Category) {
		Scene.SetErrorScreen(ERROR_TYPE_DATA_FILE_CATEGORY, CategorySearch);
		return LoadedData;
	}

	const TiXmlAttribute* Attribute = Category->FirstAttribute();
	while (Attribute) {
		LoadedData.emplace_back(std::stof(Attribute->Value()));
		Attribute = Attribute->Next();
	}

	return LoadedData;
}

MSDK::StringDataVec MSDK::Data::LoadCategoryStringData(std::string CategoryName) {
	StringDataVec LoadedData{};
	TiXmlElement* Category = FindCategory(CategoryName);

	if (!Category) {
		Scene.SetErrorScreen(ERROR_TYPE_DATA_FILE_CATEGORY, CategorySearch);
		return LoadedData;
	}

	const TiXmlAttribute* Attribute = Category->FirstAttribute();
	while (Attribute) {
		LoadedData.emplace_back((std::string)Attribute->Value());
		Attribute = Attribute->Next();
	}

	return LoadedData;
}

void MSDK::Data::ResetData() {
	Doc.Clear();
	FileExist = false;
	SetupData();
}

void MSDK::Data::Release() {
	Doc.Clear();
	FileExist = false;
}

//////////////////////////////// private
void MSDK::Data::SetupData() {
	if (!FileExist) {
		CreateDec(APPLICATION_VERSION);
		AddRoot("Data");
	}

	Root = FindRoot();

	for (auto const& D : DataFormatInfo) {
		if (!CheckCategoryExist(D.CategoryName))
			AddCategory(D.CategoryName);

		if (!CheckDataExist(D.CategoryName, D.DataName)) {
			switch (D.DataType) {
			case DATA_TYPE_DIGIT:
				AddDigitData(D.CategoryName, D.DataName, D.DigitValue);
				break;

			case DATA_TYPE_STRING:
				AddStringData(D.CategoryName, D.DataName, D.StringValue);
				break;
			}
		}
	}

	UpdateDataFile();

	if (!FileExist) {
		std::cout << "Created new data file: " << FilePath << std::endl;
		FileExist = true;
	}
}

void MSDK::Data::CheckDataVersion() {
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

void MSDK::Data::UpdateDataVersion(float VersionValue) {
	TiXmlNode* DeclNode = Doc.FirstChild();
	TiXmlDeclaration* Decl = DeclNode->ToDeclaration();

	std::string Version = Decl->Version();
	std::string Encoding = Decl->Encoding();
	std::string StandAlone = Decl->Standalone();

	if (std::stof(Version) < VersionValue) {
		std::ostringstream OSS;
		OSS << std::fixed << std::setprecision(1) << VersionValue;
		std::string NewVersionStr = OSS.str();

		TiXmlDeclaration NewDecl(NewVersionStr.c_str(), Encoding.c_str() ? Encoding.c_str() : "", StandAlone.c_str() ? StandAlone.c_str() : "");

		Doc.ReplaceChild(DeclNode, NewDecl);

		SetupData();
	}
}

void MSDK::Data::CreateDec(float VersionValue) {
	std::ostringstream OSS;
	OSS << std::fixed << std::setprecision(1) << VersionValue;
	std::string NewVersionStr = OSS.str();
	Doc.LinkEndChild(new TiXmlDeclaration(NewVersionStr.c_str(), "", ""));
}

void MSDK::Data::AddRoot(std::string RootName) {
	Doc.LinkEndChild(new TiXmlElement(RootName.c_str()));
}

void MSDK::Data::AddCategory(std::string CategoryName) {
	Root->LinkEndChild(new TiXmlElement(CategoryName.c_str()));
}

void MSDK::Data::AddDigitData(std::string CategoryName, std::string DataName, float Value) {
	FindCategory(CategoryName)->SetDoubleAttribute(DataName.c_str(), Value);
}

void MSDK::Data::AddStringData(std::string CategoryName, std::string DataName, std::string Value) {
	FindCategory(CategoryName)->SetAttribute(DataName.c_str(), Value.c_str());
}

void MSDK::Data::WriteDigitData(TiXmlElement* CategoryVar, std::string DataName, float Value) {
	if (!CategoryVar) {
		Scene.SetErrorScreen(ERROR_TYPE_DATA_FILE_CATEGORY, CategorySearch);
		return;
	}

	if (CategoryVar->Attribute(DataName.c_str()))
		CategoryVar->SetDoubleAttribute(DataName.c_str(), Value);
	else {
		Scene.SetErrorScreen(ERROR_TYPE_DATA_FILE_DATA_WRITE, CategorySearch, DataSearch);
		return;
	}
}

void MSDK::Data::WriteStringData(TiXmlElement* CategoryVar, std::string DataName, std::string Value) {
	if (!CategoryVar) {
		Scene.SetErrorScreen(ERROR_TYPE_DATA_FILE_CATEGORY, CategorySearch);
		return;
	}

	if (CategoryVar->Attribute(DataName.c_str()))
		CategoryVar->SetAttribute(DataName.c_str(), Value.c_str());
	else {
		Scene.SetErrorScreen(ERROR_TYPE_DATA_FILE_DATA_WRITE, CategorySearch, DataSearch);
		return;
	}
}

float MSDK::Data::GetDigitData(TiXmlElement* CategoryVar, std::string DataName) {
	if (!CategoryVar) {
		Scene.SetErrorScreen(ERROR_TYPE_DATA_FILE_CATEGORY, CategorySearch);
		return 0.0;
	}

	const char* DataValue = CategoryVar->Attribute(DataName.c_str());
	if (DataValue)
		return std::stof(DataValue);
	else {
		Scene.SetErrorScreen(ERROR_TYPE_DATA_FILE_DATA_LOAD, CategorySearch, DataSearch);
		return 0.0;
	}
}

std::string MSDK::Data::GetStringData(TiXmlElement* CategoryVar, std::string DataName) {
	if (!CategoryVar) {
		Scene.SetErrorScreen(ERROR_TYPE_DATA_FILE_CATEGORY, CategorySearch);
		return "";
	}

	const char* DataValue = CategoryVar->Attribute(DataName.c_str());
	if (DataValue)
		return (std::string)DataValue;
	else {
		Scene.SetErrorScreen(ERROR_TYPE_DATA_FILE_DATA_LOAD, CategorySearch, DataSearch);
		return "";
	}
}

TiXmlElement* MSDK::Data::FindRoot() {
	return Doc.RootElement();
}

TiXmlElement* MSDK::Data::FindCategory(std::string CategoryName) {
	return Root->FirstChildElement(CategoryName.c_str());
}

bool MSDK::Data::CheckCategoryExist(std::string CategoryName) {
	if (!Root->FirstChildElement(CategoryName.c_str()))
		return false;

	return true;
}

bool MSDK::Data::CheckDataExist(std::string CategoryName, std::string DataName) {
	TiXmlElement* FoundCategory = FindCategory(CategoryName);
	if (!FindCategory(CategoryName)->Attribute(DataName.c_str()))
		return false;

	return true;
}

bool MSDK::Data::LoadDataFile(std::string FileName) {
	if (USE_FILE_SECURITY) {
		std::ifstream EncryptedFile(FileName, std::ios::binary);
		if (!EncryptedFile)
			return false;

		std::string EncryptedContent((std::istreambuf_iterator<char>(EncryptedFile)), std::istreambuf_iterator<char>());
		EncryptedFile.close();

		std::string DecryptedXML = Decrypt(EncryptedContent, AES_KEY, IV_KEY);
		Doc.Parse(DecryptedXML.c_str(), nullptr, TIXML_ENCODING_UTF8);

		if (Doc.Error()) {
			Scene.SetErrorScreen(ERROR_TYPE_DATA_FILE_PARSE, FileName);
			return false;
		}

		return true;
	}

	else
		return Doc.LoadFile(FileName.c_str(), TIXML_ENCODING_UTF8);
}

void MSDK::Data::UpdateDataFile() {
	if (USE_FILE_SECURITY) {
		TiXmlPrinter Printer;
		Doc.Accept(&Printer);

		std::string XML_String = Printer.CStr();
		std::string EncryptedXML = Encrypt(XML_String, AES_KEY, IV_KEY);

		std::ofstream ExportFile(FilePath, std::ios::binary);
		ExportFile.write(EncryptedXML.c_str(), EncryptedXML.size());
		ExportFile.close();
	}

	else
		Doc.SaveFile(FilePath.c_str());
}

std::string MSDK::Data::Encrypt(const std::string& PlainText, const byte Key[], const byte IV[]) {
	std::string CipherText;

	try {
		CBC_Mode<AES>::Encryption Encryption;
		Encryption.SetKeyWithIV(Key, AES::DEFAULT_KEYLENGTH, IV);
		StringSource(PlainText, true, new StreamTransformationFilter(Encryption, new StringSink(CipherText)));
	}
	catch (const Exception& E) {
		std::cerr << E.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	return CipherText;
}

std::string MSDK::Data::Decrypt(const std::string& CipherText, const byte Key[], const byte IV[]) {
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

std::string MSDK::Data::GetFileName(const std::string& FileDirectory) {
	std::string MainString = FileDirectory;
	size_t Pos = MainString.rfind("//");

	return (Pos != std::string::npos) ? MainString.substr(Pos + 2) : MainString;
}

std::string MSDK::Data::GetFolderPath(const std::string& FileDirectory, const std::string& RemoveString) {
	std::string MainString = FileDirectory;
	size_t Pos = MainString.rfind("//");
	if (Pos != std::string::npos)
		MainString = MainString.substr(0, Pos);

	return MainString;
}