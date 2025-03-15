#pragma once
#include "QuadPent_Object.h"

class QuadPent_ErrorMessage : public QP::Object {
private:
	QP::Text Message{};
	std::string Result{};

	std::string ErrorValue1{};
	std::string ErrorValue2{};
	int ErrorType{};

	QP::RectBrush Rect{};
	QP::LineRectBrush LineRect{};
	float HeightOffset = 0.1;

	bool InitState{};

public:
	QuadPent_ErrorMessage(int Type, std::string Value) {
		ErrorValue1 = Value;
		ErrorType = Type;
	}

	QuadPent_ErrorMessage(int Type, std::string Value1, std::string Value2) {
		ErrorValue1 = Value1;
		ErrorValue2 = Value2;
		ErrorType = Type;
	}

	void RenderFunc() {
		if (!InitState)
			InitErrorMessage();

		else {
			Rect.SetColorRGB(31, 31, 31);
			Rect.Render(0.0, 0.0, QP::ViewportWidth, QP::ViewportHeight);

			Rect.SetColorRGB(45, 45, 45);
			Rect.Render(0.0, HeightOffset, 0.8, 0.2, -45.0f);
			Rect.Render(0.0, HeightOffset, 0.8, 0.2, 45.0f);

			LineRect.Render(0.0, HeightOffset, 1.5, 1.0, 0.1);
			Rect.Render(0.0, -0.65 + HeightOffset, 0.4, 0.3);
			Rect.Render(0.0, -0.75 + HeightOffset, 0.7, 0.1);

			Message.RenderString(QP::WindowRect.LeftX + 0.01, 1.0 - 0.06, 0.06, Result);
			Message.Render(QP::WindowRect.LeftX + 0.01, -1.0 + 0.02, 0.06, L"Press \\YESC\\E or \\YEnter\\E to terminate system...");
		}
	}


	void InitErrorMessage() {
		Rect.SetRenderType(RENDER_TYPE_STATIC);
		LineRect.SetRenderType(RENDER_TYPE_STATIC);
		LineRect.SetColorRGB(45, 45, 45);

		Message.Init(QP::SYSRES.SYSTEM_FONT_REGULAR);
		Message.SetColor(1.0, 1.0, 1.0);
		Message.SetLineGap(0.01);

		if (ERROR_TYPE_IMAGE_LOAD <= ErrorType && ErrorType <= ERROR_TYPE_NOT_CREATED_FONT_RENDER) {
			PlaySoundW(TEXT("SystemComponent\\Sound\\sound-error.wav"), NULL, SND_FILENAME | SND_ASYNC);
			Result = "An error occurred in the QuadPent system.\n\n";
		}
		else {
			PlaySoundW(TEXT("SystemComponent\\Sound\\sound-critical-error.wav"), NULL, SND_FILENAME | SND_ASYNC);
			Result = "A critical error occurred in the QuadPent system.\n\n";
		}

		

		if (ErrorValue2.empty()) {
			switch (ErrorType) {
			case ERROR_TYPE_IMAGE_LOAD:
				QP::StringTool.AddString(Result, "Error Type: ERROR_TYPE_IMAGE_LOAD\n\\YFile with problem: ");
				QP::StringTool.AddString(Result, ErrorValue1 + "\n\\E");
				QP::StringTool.AddString(Result, "While running mode: " + QP::Scene.CurrentModeName() + "\n\n");
				QP::StringTool.AddString(Result, "The file may be missing or damaged.\nMake sure the image file is correct.");
				break;

			case ERROR_TYPE_SOUND_LOAD:
				QP::StringTool.AddString(Result, "Error Type: ERROR_TYPE_SOUND_LOAD\n\\YFile with problem: ");
				QP::StringTool.AddString(Result, ErrorValue1 + "\n\\E");
				QP::StringTool.AddString(Result, "While running mode: " + QP::Scene.CurrentModeName() + "\n\n");
				QP::StringTool.AddString(Result, "The file may be missing or damaged.\nMake sure the audio file is correct.");
				break;

			case ERROR_TYPE_FONT_LOAD:
				QP::StringTool.AddString(Result, "Error Type : ERROR_TYPE_FONT_LOAD\n\\YFile with problem: ");
				QP::StringTool.AddString(Result, ErrorValue1 + "\n\\E");
				QP::StringTool.AddString(Result, "While running mode: " + QP::Scene.CurrentModeName() + "\n\n");
				QP::StringTool.AddString(Result, "The file may be missing or damaged.\nMake sure the font file is correct.");
				break;

			case ERROR_TYPE_SCRIPT_LOAD:
				QP::StringTool.AddString(Result, "Error Type: ERROR_TYPE_SCRIPT_LOAD\n\\YFile with problem: ");
				QP::StringTool.AddString(Result, ErrorValue1 + "\n\\E");
				QP::StringTool.AddString(Result, "While running mode: " + QP::Scene.CurrentModeName() + "\n\n");
				QP::StringTool.AddString(Result, "The file may be missing or damaged.\nMake sure the script file is correct.");
				break;

			case ERROR_TYPE_SCRIPT_PARSE:
				QP::StringTool.AddString(Result, "Error Type: ERROR_TYPE_SCRIPT_PARSE\n\\YFile with problem: ");
				QP::StringTool.AddString(Result, ErrorValue1 + "\n\\E");
				QP::StringTool.AddString(Result, "While running mode: " + QP::Scene.CurrentModeName() + "\n\n");
				QP::StringTool.AddString(Result, "The file may be damaged or have an incorrect structure.\nMake sure the script file is correct.");
				break;

			case ERROR_TYPE_SCRIPT_CATEGORY:
				QP::StringTool.AddString(Result, "Error Type: ERROR_TYPE_SCRIPT_CATEGORY\n\\YCategory with problem: ");
				QP::StringTool.AddString(Result, ErrorValue1 + "\n\\E");
				QP::StringTool.AddString(Result, "While running mode: " + QP::Scene.CurrentModeName() + "\n\n");
				QP::StringTool.AddString(Result, "The category may be missed.\nMake sure the script file's category is correct.");
				break;

			case ERROR_TYPE_DATA_FILE_PARSE:
				QP::StringTool.AddString(Result, "Error Type: ERROR_TYPE_DATA_FILE_PARSE\n\\YFile with problem: ");
				QP::StringTool.AddString(Result, ErrorValue1 + "\n\\E");
				QP::StringTool.AddString(Result, "While running mode: " + QP::Scene.CurrentModeName() + "\n\n");
				QP::StringTool.AddString(Result, "The file may be damaged or have an incorrect structure.\nMake sure the data file is correct.");
				break;

			case ERROR_TYPE_DATA_FILE_CATEGORY:
				QP::StringTool.AddString(Result, "Error Type: ERROR_TYPE_DATA_FILE_CATEGORY\n\\YCategory with problem: ");
				QP::StringTool.AddString(Result, ErrorValue1 + "\n\\E");
				QP::StringTool.AddString(Result, "While running mode: " + QP::Scene.CurrentModeName() + "\n\n");
				QP::StringTool.AddString(Result, "The category may be missed.\nMake sure the data file's category is correct.");
				break;

			case ERROR_TYPE_NOT_CREATED_FONT_RENDER:
				QP::StringTool.AddString(Result, "Error Type: ERROR_TYPE_NOT_CREATED_FONT_RENDER\n");
				QP::StringTool.AddString(Result, "While running mode: " + QP::Scene.CurrentModeName() + "\n\n");
				QP::StringTool.AddString(Result, "Make sure the font was created.\nMake sure you have mapped the font in your text object.");
				break;

				/////////////////////////////////////////////////////

			case ERROR_TYPE_UNMAPPED_MODE_EXECUTION:
				QP::StringTool.AddString(Result, "Error Type: ERROR_TYPE_UNMAPPED_MODE_EXECUTION\n\\YWhile running mode: ");
				QP::StringTool.AddString(Result, ErrorValue1 + "\n\n\\E");
				QP::StringTool.AddString(Result, "You have run an unmapped mode. Make sure the mode is mapped.");
				break;

			case ERROR_TYPE_EXECUTED_MODE_EXECUTION:
				QP::StringTool.AddString(Result, "Error Type: ERROR_TYPE_EXECUTED_MODE_EXECUTION\n\\YWhile running mode: ");
				QP::StringTool.AddString(Result, ErrorValue1 + "\n\n\\E");
				QP::StringTool.AddString(Result, "You have attempted to execute a mode that has already been executed.\nCheck which part overlaps with the currently running mode.");
				break;

			case ERROR_TYPE_EXECUTED_FLOATING_MODE_EXECUTUION:
				QP::StringTool.AddString(Result, "Error Type: ERROR_TYPE_EXECUTED_FLOATING_MODE_EXECUTUION\n\\YWhile running floating mode: ");
				QP::StringTool.AddString(Result, ErrorValue1 + "\n\n\\E");
				QP::StringTool.AddString(Result, "You have attempted to execute a floating mode that has already been executed.\nCheck which part overlaps with the currently running floating mode.");
				break;

			case ERROR_TYPE_TERMINATED_FLOATING_MODE_TERMINATION:
				QP::StringTool.AddString(Result, "Error Type: ERROR_TYPE_TERMINATED_FLOATING_MODE_TERMINATION\n\\YWhile running mode: ");
				QP::StringTool.AddString(Result, ErrorValue1 + "\n\n\\E");
				QP::StringTool.AddString(Result, "You have attempted to terminate a floating mode that has already been terminated.\nCheck which part attempts to terminate the floating mode that terminated.");
				break;

			case ERROR_TYPE_OBJECT_ADD_OUT_OF_SCENE_LAYER:
				QP::StringTool.AddString(Result, "Error Type: ERROR_TYPE_OBJECT_ADD_OUT_OF_SCENE_LAYER\n\\YWhile running mode: ");
				QP::StringTool.AddString(Result, ErrorValue1 + "\n\n\\E");
				QP::StringTool.AddString(Result, "You attempted to add an object at a location that exceeds the maximum layers.\nCheck which part adds the object in the wrong location.");
				break;

			case ERROR_TYPE_OBJECT_SWAP_OUT_OF_SCENE_LAYER:
				QP::StringTool.AddString(Result, "Error Type: ERROR_TYPE_OBJECT_SWAP_OUT_OF_SCENE_LAYER\n\\YWhile running mode: ");
				QP::StringTool.AddString(Result, ErrorValue1 + "\n\n\\E");
				QP::StringTool.AddString(Result, "You attempted to swap an object's layer to a location that exceeds the maximum layers.\nCheck which part swaps the object's layer to the wrong location.");
				break;

			case ERROR_TYPE_OBJECT_FIND_OUT_OF_SCENE_LAYER:
				QP::StringTool.AddString(Result, "Error Type: ERROR_TYPE_OBJECT_FIND_OUT_OF_SCENE_LAYER\n\\YWhile running mode: ");
				QP::StringTool.AddString(Result, ErrorValue1 + "\n\n\\E");
				QP::StringTool.AddString(Result, "You attempted to find an object at a location that exceeds the maximum layers.\nCheck which part finds the object in the wrong location.");
				break;

			case ERROR_TYPE_RETRIEVE_LAYER_SIZE_OUT_OF_SCENE_LAYER:
				QP::StringTool.AddString(Result, "Error Type: ERROR_TYPE_RETRIEVE_LAYER_SIZE_OUT_OF_SCENE_LAYER\n\\YWhile running mode: ");
				QP::StringTool.AddString(Result, ErrorValue1 + "\n\n\\E");
				QP::StringTool.AddString(Result, "You attempted to retrieve the number of objects at a location that exceeds the maximum layers.\nCheck which part retrieves the number of objects in a location that exceeds the maximum layer.");
				break;

			case ERROR_TYPE_PERMISSION_VIOLATION_SYSTEM_LAYER_ACCESS:
				QP::StringTool.AddString(Result, "Error Type: ERROR_TYPE_PERMISSION_VIOLATION_SYSTEM_LAYER_ACCESS\n\\YWhile running mode: ");
				QP::StringTool.AddString(Result, ErrorValue1 + "\n\n\\E");
				QP::StringTool.AddString(Result, "You do not have permission to access the system layer.\nTo ensure safe system operation, access to the system interior is prohibited.");
				break;
			}
		}

		else {
			switch (ErrorType) {
			case ERROR_TYPE_SCRIPT_DATA:
				QP::StringTool.AddString(Result, "Error Type: ERROR_TYPE_SCRIPT_DATA\n\\YData with problem: ");
				QP::StringTool.AddString(Result, "Category: " + ErrorValue1 + " | Data Name: " + ErrorValue2 + "\n\\E");
				QP::StringTool.AddString(Result, "While running mode: " + QP::Scene.CurrentModeName() + "\n\n");
				QP::StringTool.AddString(Result, "Data may be missing.\nMake sure the category's data is correct.");
				break;

			case ERROR_TYPE_DATA_FILE_DATA_LOAD:
				QP::StringTool.AddString(Result, "Error Type: ERROR_TYPE_DATA_FILE_DATA_LOAD\n\\YData with problem: ");
				QP::StringTool.AddString(Result, "Category: " + ErrorValue1 + " | Data Name: " + ErrorValue2 + "\n\\E");
				QP::StringTool.AddString(Result, "While running mode: " + QP::Scene.CurrentModeName() + "\n\n");
				QP::StringTool.AddString(Result, "Data may be missing.\nMake sure the category's data is correct.");
				break;

			case ERROR_TYPE_DATA_FILE_DATA_WRITE:
				QP::StringTool.AddString(Result, "Error Type: ERROR_TYPE_DATA_FILE_DATA_WRITE\n\\YData with problem: ");
				QP::StringTool.AddString(Result, "Category: " + ErrorValue1 + " | Data Name: " + ErrorValue2 + "\n\\E");
				QP::StringTool.AddString(Result, "While running mode: " + QP::Scene.CurrentModeName() + "\n\n");
				QP::StringTool.AddString(Result, "Data may be missing.\nMake sure the category's data is correct.");
				break;
			}
		}

		InitState = true;
	}
};