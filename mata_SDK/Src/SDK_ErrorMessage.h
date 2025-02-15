#pragma once
#include "SDK_Object.h"

class SDK_ErrorMessage : public SDK::Object {
private:
	SDK::Text Message{};
	std::string Result{};

public:
	SDK_ErrorMessage(int Type, std::string Value) {
		SDK::System.SetBackColorRGB(31, 31, 31);

		Message.Init(SDK::FONT, FW_DONTCARE);
		Message.SetColor(1.0, 1.0, 1.0);
		Message.SetLineGap(0.01);

		PlaySoundW(TEXT("SDKResource\\Sound\\sound-error.wav"), NULL, SND_FILENAME | SND_ASYNC);

		Result = "An error occurred in the mata_SDK system.\n\n";

		switch (Type) {
		case ERROR_TYPE_IMAGE_LOAD:
			SDK::StringTool.AddString(Result, "Error Type : Image file load failure\nFile with problem: ");
			SDK::StringTool.AddString(Result, Value + "\n\n");
			SDK::StringTool.AddString(Result, "The file may be missing or damaged.\nMake sure the image file is correct.\n\n");
			break;

		case ERROR_TYPE_AUDIO_LOAD:
			SDK::StringTool.AddString(Result, "Error Type : Audio file load failure\nFile with problem: ");
			SDK::StringTool.AddString(Result, Value + "\n\n");
			SDK::StringTool.AddString(Result, "The file may be missing or damaged.\nMake sure the audio file is correct.\n\n");
			break;

		case ERROR_TYPE_FONT_LOAD:
			SDK::StringTool.AddString(Result, "Error Type : Font file load failure\nFile with problem: ");
			SDK::StringTool.AddString(Result, Value + "\n\n");
			SDK::StringTool.AddString(Result, "The file may be missing or damaged.\nMake sure the font file is correct.\n\n");
			break;

		case ERROR_TYPE_SCRIPT_LOAD:
			SDK::StringTool.AddString(Result, "Error Type : Script file load failure\nFile with problem: ");
			SDK::StringTool.AddString(Result, Value + "\n\n");
			SDK::StringTool.AddString(Result, "The file may be missing or damaged.\nMake sure the script file is correct.\n\n");
			break;

		case ERROR_TYPE_SCRIPT_PARSE:
			SDK::StringTool.AddString(Result, "Error Type : Script file parsing failure\nFile with problem: ");
			SDK::StringTool.AddString(Result, Value + "\n\n");
			SDK::StringTool.AddString(Result, "The file may be damaged or have an incorrect structure.\nMake sure the script file is correct.\n\n");
			break;

		case ERROR_TYPE_SCRIPT_CATEGORY:
			SDK::StringTool.AddString(Result, "Error Type : Script file category data load failure\nCategory with problem: ");
			SDK::StringTool.AddString(Result, Value + "\n\n");
			SDK::StringTool.AddString(Result, "The category may be missed.\nMake sure the script file's category is correct.\n\n");
			break;

		case ERROR_TYPE_DATA_FILE_PARSE:
			SDK::StringTool.AddString(Result, "Error Type : Data file parsing failure\nFile with problem: ");
			SDK::StringTool.AddString(Result, Value + "\n\n");
			SDK::StringTool.AddString(Result, "The file may be damaged or have an incorrect structure.\nMake sure the data file is correct.\n\n");
			break;

		case ERROR_TYPE_DATA_FILE_CATEGORY:
			SDK::StringTool.AddString(Result, "Error Type : Data file category data load failure\nCategory with problem: ");
			SDK::StringTool.AddString(Result, Value + "\n\n");
			SDK::StringTool.AddString(Result, "The category may be missed.\nMake sure the data file's category is correct.\n\n");
			break;
		}
	}

	SDK_ErrorMessage(int Type, std::string Value1, std::string Value2) {
		Result = "An error occurred in the mata_SDK system.\n\n";

		switch (Type) {
		case ERROR_TYPE_SCRIPT_DATA:
			SDK::StringTool.AddString(Result, "Error Type : Script file data load failure\nData with problem: ");
			SDK::StringTool.AddString(Result, "Category: " + Value1 + " | Data Name: " + Value2 + "\n\n");
			SDK::StringTool.AddString(Result, "Data may be missing.\nMake sure the category's data is correct.\n\n");
			break;
			
		case ERROR_TYPE_DATA_FILE_DATA_LOAD:
			SDK::StringTool.AddString(Result, "Error Type : Data file data load failure\nData with problem: ");
			SDK::StringTool.AddString(Result, "Category: " + Value1 + " | Data Name: " + Value2 + "\n\n");
			SDK::StringTool.AddString(Result, "Data may be missing.\nMake sure the category's data is correct.\n\n");
			break;

		case ERROR_TYPE_DATA_FILE_DATA_WRITE:
			SDK::StringTool.AddString(Result, "Error Type : Data file data write failure\nData with problem: ");
			SDK::StringTool.AddString(Result, "Category: " + Value1 + " | Data Name: " + Value2 + "\n\n");
			SDK::StringTool.AddString(Result, "Data may be missing.\nMake sure the category's data is correct.\n\n");
			break;
		}
	}

	void RenderFunc() {
		if (SDK::SYSRES.SDK_IMAGE_ERROR.Texture != 0) {
			Begin(RENDER_TYPE_STATIC);
			SDK::ImageTool.RenderImage(SDK::SYSRES.SDK_IMAGE_ERROR, 0.3);
		}

		Message.RenderStr(ASP(-1.0) + 0.01, 1.0 - 0.06, 0.06, Result);
		Message.Render(ASP(-1.0) + 0.01, -1.0 + 0.02, 0.06, L"Press Alt + F4 or close window to terminate system...");
	}
};