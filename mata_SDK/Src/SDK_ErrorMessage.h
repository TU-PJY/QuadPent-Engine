#pragma once
#include "SDK_Object.h"

class SDK_ErrorMessage : public SDK::Object {
private:
	SDK::Text Message{};
	std::string Result{};

	std::string ErrorValue1{};
	std::string ErrorValue2{};
	int ErrorType{};

	SDK::RectBrush Rect{};
	SDK::LineRectBrush LineRect{};
	float HeightOffset = 0.1;

	bool InitState{};

public:
	SDK_ErrorMessage(int Type, std::string Value) {
		ErrorValue1 = Value;
		ErrorType = Type;
	}

	SDK_ErrorMessage(int Type, std::string Value1, std::string Value2) {
		ErrorValue1 = Value1;
		ErrorValue2 = Value2;
		ErrorType = Type;
	}

	void RenderFunc() {
		if (!InitState)
			InitErrorMessage();

		else {
			Rect.SetColorRGB(31, 31, 31);
			Rect.Draw(0.0, 0.0, SDK::ASP(2.0), 2.0);

			Rect.SetColorRGB(45, 45, 45);
			Rect.Draw(0.0, HeightOffset, 0.8, 0.2, -45.0f);
			Rect.Draw(0.0, HeightOffset, 0.8, 0.2, 45.0f);

			LineRect.Draw(0.0, HeightOffset, 1.5, 1.0, 0.1);
			Rect.Draw(0.0, -0.65 + HeightOffset, 0.4, 0.3);
			Rect.Draw(0.0, -0.75 + HeightOffset, 0.7, 0.1);

			Message.RenderStr(SDK::ASP(-1.0) + 0.01, 1.0 - 0.06, 0.06, Result);
			Message.Render(SDK::ASP(-1.0) + 0.01, -1.0 + 0.02, 0.06, L"Press ESC or Enter to terminate system...");
		}
	}


	void InitErrorMessage() {
		Rect.SetRenderType(RENDER_TYPE_STATIC);
		LineRect.SetRenderType(RENDER_TYPE_STATIC);
		LineRect.SetColorRGB(45, 45, 45);

		Message.Init(SDK::FONT, FW_DONTCARE);
		Message.SetColor(1.0, 1.0, 1.0);
		Message.SetLineGap(0.01);

		PlaySoundW(TEXT("SDKResource\\Sound\\sound-error.wav"), NULL, SND_FILENAME | SND_ASYNC);

		Result = "An error occurred in the mata_SDK system.\n\n";

		if (ErrorValue2.empty()) {
			switch (ErrorType) {
			case ERROR_TYPE_IMAGE_LOAD:
				SDK::StringTool.AddString(Result, "Error Type : Image file load failure\nFile with problem: ");
				SDK::StringTool.AddString(Result, ErrorValue1 + "\n\n");
				SDK::StringTool.AddString(Result, "The file may be missing or damaged.\nMake sure the image file is correct.\n\n");
				break;

			case ERROR_TYPE_AUDIO_LOAD:
				SDK::StringTool.AddString(Result, "Error Type : Audio file load failure\nFile with problem: ");
				SDK::StringTool.AddString(Result, ErrorValue1 + "\n\n");
				SDK::StringTool.AddString(Result, "The file may be missing or damaged.\nMake sure the audio file is correct.\n\n");
				break;

			case ERROR_TYPE_FONT_LOAD:
				SDK::StringTool.AddString(Result, "Error Type : Font file load failure\nFile with problem: ");
				SDK::StringTool.AddString(Result, ErrorValue1 + "\n\n");
				SDK::StringTool.AddString(Result, "The file may be missing or damaged.\nMake sure the font file is correct.\n\n");
				break;

			case ERROR_TYPE_SCRIPT_LOAD:
				SDK::StringTool.AddString(Result, "Error Type : Script file load failure\nFile with problem: ");
				SDK::StringTool.AddString(Result, ErrorValue1 + "\n\n");
				SDK::StringTool.AddString(Result, "The file may be missing or damaged.\nMake sure the script file is correct.\n\n");
				break;

			case ERROR_TYPE_SCRIPT_PARSE:
				SDK::StringTool.AddString(Result, "Error Type : Script file parsing failure\nFile with problem: ");
				SDK::StringTool.AddString(Result, ErrorValue1 + "\n\n");
				SDK::StringTool.AddString(Result, "The file may be damaged or have an incorrect structure.\nMake sure the script file is correct.\n\n");
				break;

			case ERROR_TYPE_SCRIPT_CATEGORY:
				SDK::StringTool.AddString(Result, "Error Type : Script file category data load failure\nCategory with problem: ");
				SDK::StringTool.AddString(Result, ErrorValue1 + "\n\n");
				SDK::StringTool.AddString(Result, "The category may be missed.\nMake sure the script file's category is correct.\n\n");
				break;

			case ERROR_TYPE_DATA_FILE_PARSE:
				SDK::StringTool.AddString(Result, "Error Type : Data file parsing failure\nFile with problem: ");
				SDK::StringTool.AddString(Result, ErrorValue1 + "\n\n");
				SDK::StringTool.AddString(Result, "The file may be damaged or have an incorrect structure.\nMake sure the data file is correct.\n\n");
				break;

			case ERROR_TYPE_DATA_FILE_CATEGORY:
				SDK::StringTool.AddString(Result, "Error Type : Data file category data load failure\nCategory with problem: ");
				SDK::StringTool.AddString(Result, ErrorValue1 + "\n\n");
				SDK::StringTool.AddString(Result, "The category may be missed.\nMake sure the data file's category is correct.\n\n");
				break;
			}
		}

		else {
			switch (ErrorType) {
			case ERROR_TYPE_SCRIPT_DATA:
				SDK::StringTool.AddString(Result, "Error Type : Script file data load failure\nData with problem: ");
				SDK::StringTool.AddString(Result, "Category: " + ErrorValue1 + " | Data Name: " + ErrorValue2 + "\n\n");
				SDK::StringTool.AddString(Result, "Data may be missing.\nMake sure the category's data is correct.\n\n");
				break;

			case ERROR_TYPE_DATA_FILE_DATA_LOAD:
				SDK::StringTool.AddString(Result, "Error Type : Data file data load failure\nData with problem: ");
				SDK::StringTool.AddString(Result, "Category: " + ErrorValue1 + " | Data Name: " + ErrorValue2 + "\n\n");
				SDK::StringTool.AddString(Result, "Data may be missing.\nMake sure the category's data is correct.\n\n");
				break;

			case ERROR_TYPE_DATA_FILE_DATA_WRITE:
				SDK::StringTool.AddString(Result, "Error Type : Data file data write failure\nData with problem: ");
				SDK::StringTool.AddString(Result, "Category: " + ErrorValue1 + " | Data Name: " + ErrorValue2 + "\n\n");
				SDK::StringTool.AddString(Result, "Data may be missing.\nMake sure the category's data is correct.\n\n");
				break;
			}
		}

		InitState = true;
	}
};