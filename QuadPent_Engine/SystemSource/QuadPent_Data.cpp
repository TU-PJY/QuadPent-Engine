#include "QuadPent_Data.h"
#include "QuadPent_Scene.h"
#include "QuadPent_StringTool.h"

void QP::Data::Load(std::string FileDirectory, DataFormat Fmt) {
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
	CheckVersion();
	std::cout << "File util opened file: " << FilePath << std::endl;
	FileExist = true;
}

void QP::Data::UpdateDigitData(std::string CategoryName, std::string DataName, float Value) {
	CategorySearch = CategoryName;
	DataSearch = DataName;
	WriteDigitData(FindCategory(CategoryName), DataName, Value);
	SaveDataChanges();
}

void QP::Data::UpdateStringData(std::string CategoryName, std::string DataName, std::string Value) {
	CategorySearch = CategoryName;
	DataSearch = DataName;
	WriteStringData(FindCategory(CategoryName), DataName, Value);
	SaveDataChanges();
}

float QP::Data::LoadDigitData(std::string CategoryName, std::string DataName) {
	CategorySearch = CategoryName;
	DataSearch = DataName;
	return GetDigitData(FindCategory(CategoryName), DataName);
}

std::string QP::Data::LoadStringData(std::string CategoryName, std::string DataName) {
	CategorySearch = CategoryName;
	DataSearch = DataName;
	return GetStringData(FindCategory(CategoryName), DataName);
}

std::wstring QP::Data::LoadWStringData(std::string Categoryname, std::string DataName) {
	return StringTool.Wstring(LoadStringData(Categoryname, DataName));
}

QP::DigitDataVec QP::Data::LoadCategoryDigitData(std::string CategoryName) {
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

QP::StringDataVec QP::Data::LoadCategoryStringData(std::string CategoryName) {
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

void QP::Data::ResetData() {
	Doc.Clear();
	FileExist = false;
	SetupData();
}

void QP::Data::Release() {
	Doc.Clear();
	FileExist = false;
}

//////////////////////////////// private
void QP::Data::SetupData() {
	if (!FileExist) {
		CreateDec();
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

	SaveDataChanges();

	if (!FileExist) {
		std::cout << "Created new data file: " << FilePath << std::endl;
		FileExist = true;
	}
}

void QP::Data::CheckVersion() {
	TiXmlNode* DeclNode = Doc.FirstChild();
	TiXmlDeclaration* Decl = DeclNode->ToDeclaration();

	if (Decl) {
		const char* Version = Decl->Version();
		float VersionValue = std::stof(Version);

		float IntPart{};
		float FracPart = modf(VersionValue, &IntPart);
		int CurrentMajorVersion = static_cast<int>(IntPart);
		int CurrentMinorVersion = static_cast<int>(FracPart * 10);

		if(CurrentMajorVersion < APPLICATION_MAJOR_VERSION || (CurrentMajorVersion == APPLICATION_MAJOR_VERSION && CurrentMinorVersion < APPLICATION_MINOR_VERSION))
			UpdateVersion();
	}
	else {
		std::cout << "Failed to find data version" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void QP::Data::UpdateVersion() {
	for (auto& D : DataFormatInfo) {
		if (CheckCategoryExist(D.CategoryName)) {
			if (D.DataType == DATA_TYPE_DIGIT) {
				float Value = LoadDigitData(D.CategoryName, D.DataName);
				Buffer.emplace_back(D.DataType, D.CategoryName, D.DataName, Value, "");
			}
			else if (D.DataType == DATA_TYPE_STRING) {
				std::string Value = LoadStringData(D.CategoryName, D.DataName);
				Buffer.emplace_back(D.DataType, D.CategoryName, D.DataName, 0.0, Value);
			}
		}
	}

	Release();
	SetupData();

	for (auto& B : Buffer) {
		if(B.DataType == DATA_TYPE_DIGIT)
			UpdateDigitData(B.CategoryName, B.DataName, B.DigitValue);
		else if(B.DataType == DATA_TYPE_STRING)
			UpdateStringData(B.CategoryName, B.DataName, B.StringValue);
	}

	Buffer.clear();
}

void QP::Data::CreateDec() {
	std::string NewVersionStr = std::to_string(APPLICATION_MAJOR_VERSION) + "." + std::to_string(APPLICATION_MINOR_VERSION);
	Doc.LinkEndChild(new TiXmlDeclaration(NewVersionStr.c_str(), "UTF-8", ""));
}

void QP::Data::AddRoot(std::string RootName) {
	Doc.LinkEndChild(new TiXmlElement(RootName.c_str()));
}

void QP::Data::AddCategory(std::string CategoryName) {
	Root->LinkEndChild(new TiXmlElement(CategoryName.c_str()));
}

void QP::Data::AddDigitData(std::string CategoryName, std::string DataName, float Value) {
	FindCategory(CategoryName)->SetDoubleAttribute(DataName.c_str(), Value);
}

void QP::Data::AddStringData(std::string CategoryName, std::string DataName, std::string Value) {
	FindCategory(CategoryName)->SetAttribute(DataName.c_str(), Value.c_str());
}

void QP::Data::WriteDigitData(TiXmlElement* CategoryVar, std::string DataName, float Value) {
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

void QP::Data::WriteStringData(TiXmlElement* CategoryVar, std::string DataName, std::string Value) {
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

float QP::Data::GetDigitData(TiXmlElement* CategoryVar, std::string DataName) {
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

std::string QP::Data::GetStringData(TiXmlElement* CategoryVar, std::string DataName) {
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

TiXmlElement* QP::Data::FindRoot() {
	return Doc.RootElement();
}

TiXmlElement* QP::Data::FindCategory(std::string CategoryName) {
	return Root->FirstChildElement(CategoryName.c_str());
}

bool QP::Data::CheckCategoryExist(std::string CategoryName) {
	if (!Root->FirstChildElement(CategoryName.c_str()))
		return false;

	return true;
}

bool QP::Data::CheckDataExist(std::string CategoryName, std::string DataName) {
	TiXmlElement* FoundCategory = FindCategory(CategoryName);
	if (!FindCategory(CategoryName)->Attribute(DataName.c_str()))
		return false;

	return true;
}

bool QP::Data::LoadDataFile(std::string FileName) {
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

void QP::Data::SaveDataChanges() {
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

std::string QP::Data::Encrypt(const std::string& PlainText, const byte Key[], const byte IV[]) {
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

std::string QP::Data::Decrypt(const std::string& CipherText, const byte Key[], const byte IV[]) {
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

std::string QP::Data::GetFileName(const std::string& FileDirectory) {
	std::string MainString = FileDirectory;
	size_t Pos = MainString.rfind("//");

	return (Pos != std::string::npos) ? MainString.substr(Pos + 2) : MainString;
}

std::string QP::Data::GetFolderPath(const std::string& FileDirectory, const std::string& RemoveString) {
	std::string MainString = FileDirectory;
	size_t Pos = MainString.rfind("//");
	if (Pos != std::string::npos)
		MainString = MainString.substr(0, Pos);

	return MainString;
}