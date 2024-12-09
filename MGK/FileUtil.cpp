#include "FileUtil.h"

#ifdef USE_FILE_SYSTEM
#include "Setting.h"

void FileUtil::Import(std::string FileDirectory, DataFormat Format) {
	if (FileExist)
		return;

	std::string FolderPath = GetFolderPath(FileDirectory, GetFileName(FileDirectory));
	FilePath = FileDirectory;
	DataFormatInfo = Format;

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

void FileUtil::UpdateDigitData(std::string CategoryName, std::string DataName, float Value) {
	WriteDigitData(FindCategory(CategoryName), DataName, Value);
	UpdateDataFile();
}

void FileUtil::UpdateStringData(std::string CategoryName, std::string DataName, std::string Value) {
	WriteStringData(FindCategory(CategoryName), DataName, Value);
	UpdateDataFile();
}

float FileUtil::LoadDigitData(std::string CategoryName, std::string DataName) {
	return GetDigitData(FindCategory(CategoryName), DataName);
}

std::string FileUtil::LoadStringData(std::string CategoryName, std::string DataName) {
	return GetStringData(FindCategory(CategoryName), DataName);
}

DigitDataSet FileUtil::LoadCategoryDigitData(std::string CategoryName) {
	DigitDataSet LoadedData{};
	TiXmlElement* Category = FindCategory(CategoryName);

	const TiXmlAttribute* Attribute = Category->FirstAttribute();
	while (Attribute) {
		LoadedData.emplace_back(std::stof(Attribute->Value()));
		Attribute = Attribute->Next();
	}

	return LoadedData;
}

StringDataSet FileUtil::LoadCategoryStringData(std::string CategoryName) {
	StringDataSet LoadedData{};
	TiXmlElement* Category = FindCategory(CategoryName);

	const TiXmlAttribute* Attribute = Category->FirstAttribute();
	while (Attribute) {
		LoadedData.emplace_back((std::string)Attribute->Value());
		Attribute = Attribute->Next();
	}

	return LoadedData;
}

void FileUtil::ResetData() {
	Doc.Clear();
	FileExist = false;
	SetupData();
}

void FileUtil::Release() {
	Doc.Clear();
	FileExist = false;
}


//////////////////////////////// private
void FileUtil::SetupData() {
	if (!FileExist) {
		CreateDec(APPLICATION_VERSION);
		AddRoot("Data");
	}

	Root = FindRoot();

	for (auto const& D : DataFormatInfo) {
		if(!FindCategory(D.CategoryName))
			AddCategory(D.CategoryName);

		if (FindData(D.CategoryName, D.DataName).empty()) {
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

void FileUtil::CheckDataVersion() {
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

void FileUtil::UpdateDataVersion(float VersionValue) {
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

void FileUtil::CreateDec(float VersionValue) {
	std::ostringstream OSS;
	OSS << std::fixed << std::setprecision(1) << VersionValue;
	std::string NewVersionStr = OSS.str();
	Doc.LinkEndChild(new TiXmlDeclaration(NewVersionStr.c_str(), "", ""));
}

void FileUtil::AddRoot(std::string RootName) {
	Doc.LinkEndChild(new TiXmlElement(RootName.c_str()));
}

void FileUtil::AddCategory(std::string CategoryName) {
	Root->LinkEndChild(new TiXmlElement(CategoryName.c_str()));
}

void FileUtil::AddDigitData(std::string CategoryName, std::string DataName, float Value) {
	FindCategory(CategoryName)->SetDoubleAttribute(DataName.c_str(), Value);
}

void FileUtil::AddStringData(std::string CategoryName, std::string DataName, std::string Value) {
	FindCategory(CategoryName)->SetAttribute(DataName.c_str(), Value.c_str());
}

void FileUtil::WriteDigitData(TiXmlElement* CategoryVar, std::string DataName, float Value) {
	if (CategoryVar->Attribute(DataName.c_str()))
		CategoryVar->SetDoubleAttribute(DataName.c_str(), Value);
	else {
		std::cout << "Failed to update data" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void FileUtil::WriteStringData(TiXmlElement* CategoryVar, std::string DataName, std::string Value) {
	if (CategoryVar->Attribute(DataName.c_str()))
		CategoryVar->SetAttribute(DataName.c_str(), Value.c_str());
	else {
		std::cout << "Failed to update string data" << std::endl;
		exit(EXIT_FAILURE);
	}
}

float FileUtil::GetDigitData(TiXmlElement* CategoryVar, std::string DataName) {
	const char* DataValue = CategoryVar->Attribute(DataName.c_str());
	if (DataValue)
		return std::stof(DataValue);
	else {
		std::cout << "Failed to find data" << std::endl;
		exit(EXIT_FAILURE);
	}
}

std::string FileUtil::GetStringData(TiXmlElement* CategoryVar, std::string DataName) {
	const char* DataValue = CategoryVar->Attribute(DataName.c_str());
	if (DataValue)
		return (std::string)DataValue;
	else {
		std::cout << "Failed to find data" << std::endl;
		exit(EXIT_FAILURE);
	}
}

TiXmlElement* FileUtil::FindRoot() {
	return Doc.RootElement();
}

TiXmlElement* FileUtil::FindCategory(std::string CategoryName) {
	return Root->FirstChildElement(CategoryName.c_str());
}

std::string FileUtil::FindData(std::string CategoryName, std::string DataName) {
	const char* DataValue = FindCategory(CategoryName)->Attribute(DataName.c_str());
	return DataValue ? (std::string)DataValue : "";
}

bool FileUtil::LoadDataFile(std::string FileName) {
	if (USE_FILE_SECURITY) {
		std::ifstream EncryptedFile(FileName, std::ios::binary);
		if (!EncryptedFile)
			return false;

		std::string EncryptedContent((std::istreambuf_iterator<char>(EncryptedFile)), std::istreambuf_iterator<char>());
		EncryptedFile.close();

		std::string DecryptedXML = Decrypt(EncryptedContent, AES_KEY, IV_KEY);
		Doc.Parse(DecryptedXML.c_str());

		if (Doc.Error()) {
			std::cout << "Failed to parse data file" << std::endl;
			exit(EXIT_FAILURE);
		}
		return true;
	}

	else
		return Doc.LoadFile(FileName.c_str());
}

void FileUtil::UpdateDataFile() {
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

std::string FileUtil::Encrypt(const std::string& PlainText, const byte Key[], const byte IV[]) {
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

std::string FileUtil::Decrypt(const std::string& CipherText, const byte Key[], const byte IV[]) {
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

std::string FileUtil::GetFileName(const std::string& FileDirectory) {
	std::string MainString = FileDirectory;
	size_t Pos = MainString.rfind("//");

	return (Pos != std::string::npos) ? MainString.substr(Pos + 2) : MainString;
}

std::string FileUtil::GetFolderPath(const std::string& FileDirectory, const std::string& RemoveString) {
	std::string MainString = FileDirectory;
	size_t Pos = MainString.rfind("//");
	if (Pos != std::string::npos)
		MainString = MainString.substr(0, Pos);
	
	return MainString;
}
#endif