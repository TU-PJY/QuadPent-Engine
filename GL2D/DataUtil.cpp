#include "DataUtil.h"
#ifdef USE_DATA_SYSTEM

#define USE_CUSTOM_PATH
#define USE_DATA_SECURITY
#define DATA_FILE_DEBUG_MODE

constexpr byte KEY[AES::DEFAULT_KEYLENGTH] =
{ 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36 };

constexpr byte IV[AES::BLOCKSIZE] =
{ 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36 };


constexpr float APPLICATION_VERSION = 1.0;
constexpr const char* FOLDER_NAME = "DataFolder";
constexpr const char* FILE_NAME = "Data";

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
	// add root, category, data here

	UpdateDataFile();
	std::cout << "Created new data file: " << FilePathStr << std::endl;
}



void DataUtil::RegisterNewData() {
	// add category and data here

	UpdateDataFile();
}



void DataUtil::Init() {
	if (!std::filesystem::exists(FolderPath)) {
		if (!std::filesystem::create_directory(FolderPath)) {
			std::cout << "Falied to find folder" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	FilePathStr = FilePath.string();

#ifdef DATA_FILE_DEBUG_MODE
	DebugFilePath = FolderPath / "DebugDataFile.xml";
	DebugFilePathStr = DebugFilePath.string();
#endif

#ifdef USE_DATA_SECURITY
	if (!LoadDataFile(FilePathStr.c_str()))
		CreateNewData();
	else
		UpdateDataVersion(APPLICATION_VERSION);
#else
	FilePathStr += ".xml";

	if (!Doc.LoadFile(FilePathStr.c_str()))
		CreateNewData();
	else
		UpdateDataVersion(APPLICATION_VERSION);
#endif

}

void DataUtil::UpdateData(const char* CategoryName, const char* DataName, float Value) {
	WriteData(FindCategory(CategoryName), DataName, Value);
	UpdateDataFile();
}

void DataUtil::UpdateStringData(const char* CategoryName, const char* DataName, const char* Value) {
	WriteStringData(FindCategory(CategoryName), DataName, Value);
	UpdateDataFile();
}

float DataUtil::LoadData(const char* CategoryName, const char* DataName) {
	return GetData(FindCategory(CategoryName), DataName);
}

const char* DataUtil::LoadStringData(const char* CategoryName, const char* DataName) {
	return GetStringData(FindCategory(CategoryName), DataName);
}

void DataUtil::ResetData() {
	Doc.Clear();
	CreateNewData();
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

	if (std::stof(Version) < VersionValue) {
		std::ostringstream OSS;
		OSS << std::fixed << std::setprecision(1) << VersionValue;
		std::string NewVersionStr = OSS.str();

		TiXmlDeclaration NewDecl(NewVersionStr.c_str(), Encoding ? Encoding : "", StandAlone ? StandAlone : "");

		Doc.ReplaceChild(DeclNode, NewDecl);

		RegisterNewData();
	}
}

void DataUtil::CreateDec(float VersionValue) {
	std::ostringstream OSS;
	OSS << std::fixed << std::setprecision(1) << VersionValue;
	std::string NewVersionStr = OSS.str();
	Doc.LinkEndChild(new TiXmlDeclaration(NewVersionStr.c_str(), "", ""));
}

void DataUtil::AddCategory(const char* CategoryName) {
	FindRoot()->LinkEndChild(new TiXmlElement(CategoryName));
}

void DataUtil::AddData(const char* CategoryName, const char* DataName, float Value) {
	FindCategory(CategoryName)->SetDoubleAttribute(DataName, Value);
}

void DataUtil::AddStringData(const char* CategoryName, const char* DataName, const char* Value) {
	FindCategory(CategoryName)->SetAttribute(DataName, Value);
}

void DataUtil::AddRoot(const char* RootName) {
	Doc.LinkEndChild(new TiXmlElement(RootName));
}

void DataUtil::WriteData(TiXmlElement* CategoryVar, const char* DataName, float Value) {
	const char* DataValue = CategoryVar->Attribute(DataName);
	if (DataValue)
		CategoryVar->SetDoubleAttribute(DataName, Value);
	else {
		std::cout << "Failed to update data" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void DataUtil::WriteStringData(TiXmlElement* CategoryVar, const char* DataName, const char* Value) {
	const char* DataValue = CategoryVar->Attribute(DataName);
	if (DataValue)
		CategoryVar->SetAttribute(DataName, Value);
	else {
		std::cout << "Failed to update string data" << std::endl;
		exit(EXIT_FAILURE);
	}
}

float DataUtil::GetData(TiXmlElement* CategoryVar, const char* DataName) {
	const char* DataValue = CategoryVar->Attribute(DataName);
	if (DataValue)
		return std::stof(DataValue);
	else {
		std::cout << "Failed to find data" << std::endl;
		exit(EXIT_FAILURE);
	}
}

const char* DataUtil::GetStringData(TiXmlElement* CategoryVar, const char* DataName) {
	const char* DataValue = CategoryVar->Attribute(DataName);
	if (DataValue)
		return DataValue;
	else {
		std::cout << "Failed to find data" << std::endl;
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

bool DataUtil::LoadDataFile(const char* FileName) {
	std::ifstream EncryptedFile(FileName, std::ios::binary);
	if (!EncryptedFile)
		return false;

	std::string EncryptedContent((std::istreambuf_iterator<char>(EncryptedFile)), std::istreambuf_iterator<char>());
	EncryptedFile.close();

	std::string DecryptedXML = Decrypt(EncryptedContent, KEY, IV);
	Doc.Parse(DecryptedXML.c_str());

	if (Doc.Error()) {
		std::cout << "Failed to parse data file" << std::endl;
		exit(EXIT_FAILURE);
	}

	return true;
}

void DataUtil::UpdateDataFile() {
#ifdef USE_DATA_SECURITY
	TiXmlPrinter Printer;
	Doc.Accept(&Printer);

	std::string XML_String = Printer.CStr();
	std::string EncryptedXML = Encrypt(XML_String, KEY, IV);

	std::ofstream ExportFile(FilePathStr, std::ios::binary);
	ExportFile.write(EncryptedXML.c_str(), EncryptedXML.size());
	ExportFile.close();

	Doc.Clear();
	LoadDataFile(FilePathStr.c_str());

#ifdef DATA_FILE_DEBUG_MODE
	Doc.SaveFile(DebugFilePathStr.c_str());
#endif
#else
	Doc.SaveFile(FilePathStr.c_str());
#endif
}

std::string DataUtil::Encrypt(const std::string& plainText, const byte key[], const byte iv[]) {
	std::string CipherText;

	try {
		CBC_Mode<AES>::Encryption Encryption;
		Encryption.SetKeyWithIV(key, AES::DEFAULT_KEYLENGTH, iv);
		StringSource(plainText, true, new StreamTransformationFilter(Encryption, new StringSink(CipherText)));
	}
	catch (const Exception& E) {
		std::cerr << E.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	return CipherText;
}

std::string DataUtil::Decrypt(const std::string& cipherText, const byte key[], const byte iv[]) {
	std::string PlainText;

	try {
		CBC_Mode<AES>::Decryption Decryption;
		Decryption.SetKeyWithIV(key, AES::DEFAULT_KEYLENGTH, iv);
		StringSource(cipherText, true, new StreamTransformationFilter(Decryption, new StringSink(PlainText)));
	}
	catch (const Exception& E) {
		std::cerr << E.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	return PlainText;
}

#endif