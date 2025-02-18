#pragma once
#include "SDK_Object.h"

class SDK_ErrorMessage : public MSDK::Object {
private:
	MSDK::Text Message{};
	std::string Result{};

	std::string ErrorValue{};
	std::string ErrorValue2{};
	int ErrorType{};

	MSDK::RectBrush Rect{};

	bool InitState{};

public:
	SDK_ErrorMessage(int Type, std::string Value) {
		ErrorValue = Value;
		ErrorType = Type;

		Rect.SetColorRGB(31, 31, 31);
		Rect.SetRenderType(RENDER_TYPE_STATIC);
	}

	SDK_ErrorMessage(int Type, std::string Value1, std::string Value2) {
		ErrorValue = Value1;
		ErrorValue2 = Value2;
		ErrorType = Type;
	}

	void RenderFunc() {
		if (!InitState)
			InitErrorMessage();

		Rect.Draw(0.0, 0.0, MSDK::ASP(2.0), 2.0);
		
		if (MSDK::SYSRES.SDK_IMAGE_ERROR.Texture != 0) {
			Begin(RENDER_TYPE_STATIC);
			MSDK::ImageTool.RenderImage(MSDK::SYSRES.SDK_IMAGE_ERROR, 0.3);
		}

		Message.RenderStr(MSDK::ASP(-1.0) + 0.01, 1.0 - 0.06, 0.06, Result);
		Message.Render(MSDK::ASP(-1.0) + 0.01, -1.0 + 0.02, 0.06, L"Press Alt + F4 or close window to terminate system...");
	}


	void InitErrorMessage() {
		Message.Init(MSDK::FONT, FW_DONTCARE);
		Message.SetColor(1.0, 1.0, 1.0);
		Message.SetLineGap(0.01);

		PlaySoundW(TEXT("SDKResource\\Sound\\sound-error.wav"), NULL, SND_FILENAME | SND_ASYNC);

		Result = "An error occurred in the mata_SDK system.\n\n";

		if (ErrorValue2.empty()) {
			switch (ErrorType) {
			case ERROR_TYPE_IMAGE_LOAD:
				MSDK::StringTool.AddString(Result, "Error Type : Image file load failure\nFile with problem: ");
				MSDK::StringTool.AddString(Result, ErrorValue + "\n\n");
				MSDK::StringTool.AddString(Result, "The file may be missing or damaged.\nMake sure the image file is correct.\n\n");
				break;

			case ERROR_TYPE_AUDIO_LOAD:
				MSDK::StringTool.AddString(Result, "Error Type : Audio file load failure\nFile with problem: ");
				MSDK::StringTool.AddString(Result, ErrorValue + "\n\n");
				MSDK::StringTool.AddString(Result, "The file may be missing or damaged.\nMake sure the audio file is correct.\n\n");
				break;

			case ERROR_TYPE_FONT_LOAD:
				MSDK::StringTool.AddString(Result, "Error Type : Font file load failure\nFile with problem: ");
				MSDK::StringTool.AddString(Result, ErrorValue + "\n\n");
				MSDK::StringTool.AddString(Result, "The file may be missing or damaged.\nMake sure the font file is correct.\n\n");
				break;

			case ERROR_TYPE_SCRIPT_LOAD:
				MSDK::StringTool.AddString(Result, "Error Type : Script file load failure\nFile with problem: ");
				MSDK::StringTool.AddString(Result, ErrorValue + "\n\n");
				MSDK::StringTool.AddString(Result, "The file may be missing or damaged.\nMake sure the script file is correct.\n\n");
				break;

			case ERROR_TYPE_SCRIPT_PARSE:
				MSDK::StringTool.AddString(Result, "Error Type : Script file parsing failure\nFile with problem: ");
				MSDK::StringTool.AddString(Result, ErrorValue + "\n\n");
				MSDK::StringTool.AddString(Result, "The file may be damaged or have an incorrect structure.\nMake sure the script file is correct.\n\n");
				break;

			case ERROR_TYPE_SCRIPT_CATEGORY:
				MSDK::StringTool.AddString(Result, "Error Type : Script file category data load failure\nCategory with problem: ");
				MSDK::StringTool.AddString(Result, ErrorValue + "\n\n");
				MSDK::StringTool.AddString(Result, "The category may be missed.\nMake sure the script file's category is correct.\n\n");
				break;

			case ERROR_TYPE_DATA_FILE_PARSE:
				MSDK::StringTool.AddString(Result, "Error Type : Data file parsing failure\nFile with problem: ");
				MSDK::StringTool.AddString(Result, ErrorValue + "\n\n");
				MSDK::StringTool.AddString(Result, "The file may be damaged or have an incorrect structure.\nMake sure the data file is correct.\n\n");
				break;

			case ERROR_TYPE_DATA_FILE_CATEGORY:
				MSDK::StringTool.AddString(Result, "Error Type : Data file category data load failure\nCategory with problem: ");
				MSDK::StringTool.AddString(Result, ErrorValue + "\n\n");
				MSDK::StringTool.AddString(Result, "The category may be missed.\nMake sure the data file's category is correct.\n\n");
				break;
			}
		}

		else {
			switch (ErrorType) {
			case ERROR_TYPE_SCRIPT_DATA:
				MSDK::StringTool.AddString(Result, "Error Type : Script file data load failure\nData with problem: ");
				MSDK::StringTool.AddString(Result, "Category: " + ErrorValue + " | Data Name: " + ErrorValue2 + "\n\n");
				MSDK::StringTool.AddString(Result, "Data may be missing.\nMake sure the category's data is correct.\n\n");
				break;

			case ERROR_TYPE_DATA_FILE_DATA_LOAD:
				MSDK::StringTool.AddString(Result, "Error Type : Data file data load failure\nData with problem: ");
				MSDK::StringTool.AddString(Result, "Category: " + ErrorValue + " | Data Name: " + ErrorValue2 + "\n\n");
				MSDK::StringTool.AddString(Result, "Data may be missing.\nMake sure the category's data is correct.\n\n");
				break;

			case ERROR_TYPE_DATA_FILE_DATA_WRITE:
				MSDK::StringTool.AddString(Result, "Error Type : Data file data write failure\nData with problem: ");
				MSDK::StringTool.AddString(Result, "Category: " + ErrorValue + " | Data Name: " + ErrorValue2 + "\n\n");
				MSDK::StringTool.AddString(Result, "Data may be missing.\nMake sure the category's data is correct.\n\n");
				break;
			}
		}

		InitState = true;
	}
};