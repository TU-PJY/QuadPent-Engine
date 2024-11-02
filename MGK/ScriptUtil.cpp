#include "ScriptUtil.h"

void ScriptUtil::Import(std::string FileName) {
	if (FileExist)
		return;

	if (!Doc.LoadFile(FileName.c_str())) {
		std::cout << "Failed to open script file" << std::endl;
		exit(EXIT_FAILURE);
	}

	Root = Doc.RootElement();
	FileExist = true;
}

void ScriptUtil::ImportSecure(std::string FileName) {
	std::ifstream EncryptedFile(FileName, std::ios::binary);
	if (!EncryptedFile) {
		std::cout << "Failed to open script file" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string EncryptedContent((std::istreambuf_iterator<char>(EncryptedFile)), std::istreambuf_iterator<char>());
	EncryptedFile.close();

	std::string DecryptedXML = Decrypt(EncryptedContent, AES_KEY, IV_KEY);
	Doc.Parse(DecryptedXML.c_str());

	if (Doc.Error()) {
		std::cout << "Failed to parse script file" << std::endl;
		exit(EXIT_FAILURE);
	}

	Root = Doc.RootElement();
	FileExist = true;
}

void ScriptUtil::Release() {
	Doc.Clear();
	FileExist = false;
}

float ScriptUtil::LoadDigitData(std::string CategoryName, std::string DataName) {
	return GetDigitData(FindCategory(CategoryName), DataName);
}

std::string ScriptUtil::LoadStringData(std::string CategoryName, std::string DataName) {
	return GetStringData(FindCategory(CategoryName), DataName);
}

float ScriptUtil::GetDigitData(TiXmlElement* CategoryVar, std::string DataName) {
	const char* DataValue = CategoryVar->Attribute(DataName.c_str());
	if (DataValue)
		return std::stof(DataValue);
	else {
		std::cout << "Failed to find data" << std::endl;
		exit(EXIT_FAILURE);
	}
}

std::string ScriptUtil::GetStringData(TiXmlElement* CategoryVar, std::string DataName) {
	const char* DataValue = CategoryVar->Attribute(DataName.c_str());
	if (DataValue)
		return (std::string)DataValue;
	else {
		std::cout << "Failed to find data" << std::endl;
		exit(EXIT_FAILURE);
	}
}

TiXmlElement* ScriptUtil::FindCategory(std::string CategoryName) {
	return Root->FirstChildElement(CategoryName.c_str());
}

std::string ScriptUtil::FindData(std::string CategoryName, std::string DataName) {
	const char* DataValue = FindCategory(CategoryName)->Attribute(DataName.c_str());
	return DataValue ? (std::string)DataValue : "";
}

std::string ScriptUtil::Decrypt(const std::string& CipherText, const byte Key[], const byte IV[]) {
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