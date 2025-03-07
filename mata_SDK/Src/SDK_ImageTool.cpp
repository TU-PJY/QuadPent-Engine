#include "SDK_ImageTool.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "SDK_EXTool.h"
#include "SDK_Scene.h"

#include "SDK_ObjectValue.h"
#include "SDK_Transform.h"
#include "SDK_ComputeTool.h"


SDK::SDK_ImageTool SDK::ImageTool;

float ImagePannel[][48] = {  // default size 1.0 * 1.0
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0, 0.0,
	 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0, 0.0,
	 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0, 1.0,
	 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0, 1.0,
	-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0, 1.0,
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0, 0.0
};

void SDK::SDK_ImageTool::Init() {
	unsigned int VBO{};
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(ImagePannel), ImagePannel, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(VAO);

	stbi_set_flip_vertically_on_load(true);
}

void SDK::SDK_ImageTool::LoadImage(SDK::Image& ImageStruct, std::string FilePath, int Type) {
	int Width{}, Height{}, Channel{};

	glGenTextures(1, &ImageStruct.Texture);
	glBindTexture(GL_TEXTURE_2D, ImageStruct.Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	switch (Type) {
	case IMAGE_TYPE_LINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;

	case IMAGE_TYPE_NEAREST:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;
	}

	unsigned char* TextureData = stbi_load(FilePath.c_str(), &Width, &Height, &Channel, 4);
	if (!TextureData) {
		SDK::Scene.SetErrorScreen(ERROR_TYPE_IMAGE_LOAD, FilePath);
		return;
	}

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, Width, Height);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Width, Height, GL_RGBA, GL_UNSIGNED_BYTE, TextureData);
	stbi_image_free(TextureData);

	ImageStruct.Width = Width;
	ImageStruct.Height = Height;
}

void SDK::SDK_ImageTool::LoadClip(SDK::Image& ImageStruct, std::string FilePath, int X, int Y, int ClipWidth, int ClipHeight, int Type) {
	int Width{}, Height{}, Channel{};

	glGenTextures(1, &ImageStruct.Texture);
	glBindTexture(GL_TEXTURE_2D, ImageStruct.Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	switch (Type) {
	case IMAGE_TYPE_LINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;

	case IMAGE_TYPE_NEAREST:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;
	}

	unsigned char* TextureData = stbi_load(FilePath.c_str(), &Width, &Height, &Channel, 4);
	if (!TextureData) {
		SDK::Scene.SetErrorScreen(ERROR_TYPE_IMAGE_LOAD, FilePath);
		return;
	}

	if (X + ClipWidth > Width || Y + ClipHeight > Height) {
		stbi_image_free(TextureData);
		return;
	}

	unsigned char* ClippedTextureData = (unsigned char*)malloc(ClipWidth * ClipHeight * Channel);
	if (!ClippedTextureData) {
		stbi_image_free(TextureData);
		return;
	}

	for (int Row = 0; Row < ClipHeight; ++Row)
		memcpy(ClippedTextureData + Row * ClipWidth * Channel, TextureData + ((Y + Row) * Width + X) * Channel, ClipWidth * Channel);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, ClipWidth, ClipHeight);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, ClipWidth, ClipHeight, GL_RGBA, GL_UNSIGNED_BYTE, ClippedTextureData);
	stbi_image_free(TextureData);
	stbi_image_free(ClippedTextureData);

	ImageStruct.Width = ClipWidth;
	ImageStruct.Height = ClipHeight;
}

void SDK::SDK_ImageTool::SetSpriteSheetSize(int ValueClipWidth, int ValueClipHeight, int ValueNumRow, int ValueNumCol, int ValueStartCol, int ValueNumBlank) {
	ClipWidth = ValueClipWidth;
	ClipHeight = ValueClipHeight;
	NumRow = ValueNumRow;
	NumCol = ValueNumCol;
	StartLocation = ValueStartCol;
	BlankLocation = ValueNumBlank;
}

void SDK::SDK_ImageTool::LoadSpriteSheet(SDK::SpriteSheet& SpriteSheetStruct, std::string FilePath, int Type) {
	int Width{}, Height{}, Channel{};
	unsigned char* TextureData = stbi_load(FilePath.c_str(), &Width, &Height, &Channel, 4);
	if (!TextureData) {
		SDK::Scene.SetErrorScreen(ERROR_TYPE_IMAGE_LOAD, FilePath);
		return;
	}

	SpriteSheetStruct.Texture.assign(NumRow * NumCol, {});

	if (BlankLocation != 0) 
		SpriteSheetStruct.Frame = NumRow * NumCol - (NumRow - BlankLocation);
	else 
		SpriteSheetStruct.Frame = NumRow * NumCol;

	int CurrentIndex{};
	int CurrentXPosition = 0;
	int CurrentYPosition = Height - ClipHeight * StartLocation;
	SDK::EXTool.ClampValue(CurrentYPosition, 0, CLAMP_LESS);

	for (int C = 0; C < NumCol; ++C) {
		for (int R = 0; R < NumRow; ++R) {
			glGenTextures(1, &SpriteSheetStruct.Texture[CurrentIndex]);
			glBindTexture(GL_TEXTURE_2D, SpriteSheetStruct.Texture[CurrentIndex]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			switch (Type) {
			case IMAGE_TYPE_LINEAR:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;

			case IMAGE_TYPE_NEAREST:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				break;
			}

			unsigned char* ClippedTextureData = (unsigned char*)malloc(ClipWidth * ClipHeight * Channel);
			if (!ClippedTextureData) {
				stbi_image_free(TextureData);
				return;
			}

			for (int i = 0; i < ClipHeight; ++i)
				memcpy(ClippedTextureData + i * ClipWidth * Channel, TextureData + ((CurrentYPosition + i) * Width + CurrentXPosition) * Channel, ClipWidth * Channel);

			glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, ClipWidth, ClipHeight);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, ClipWidth, ClipHeight, GL_RGBA, GL_UNSIGNED_BYTE, ClippedTextureData);
			stbi_image_free(ClippedTextureData);

			CurrentXPosition += ClipWidth;
			SDK::EXTool.ClampValue(CurrentXPosition, Width, CLAMP_GREATER);

			++CurrentIndex;
		}

		CurrentYPosition -= ClipHeight;
		SDK::EXTool.ClampValue(CurrentYPosition, 0, CLAMP_LESS);

		CurrentXPosition = 0;
	}

	stbi_image_free(TextureData);

	SpriteSheetStruct.Width = ClipWidth;
	SpriteSheetStruct.Height = ClipHeight;
}

void SDK::SDK_ImageTool::LoadImageT(SDK::Image& ImageStruct, std::string FilePath, int Type) {
	SDK::ImageLoadBufferData PLI{};
	int Width{}, Height{}, Channel{};
	unsigned char* TextureData = stbi_load(FilePath.c_str(), &Width, &Height, &Channel, 4);
	if (!TextureData) {
		SDK::Scene.SetErrorScreen(ERROR_TYPE_IMAGE_LOAD, FilePath);
		return;
	}

	ImageStruct.Width = Width;
	ImageStruct.Height = Height;
	PLI.ImagePtr = &ImageStruct;
	PLI.ImageType = Type;
	PLI.TextureData = TextureData;

	ImageLoadBuffer.emplace_back(PLI);
}

void SDK::SDK_ImageTool::LoadClipT(SDK::Image& ImageStruct, std::string FilePath, int X, int Y, int ClipWidth, int ClipHeight, int Type) {
	SDK::ImageLoadBufferData ILBD{};
	int Width{}, Height{}, Channel{};
	unsigned char* TextureData = stbi_load(FilePath.c_str(), &Width, &Height, &Channel, 4);
	if (!TextureData) {
		SDK::Scene.SetErrorScreen(ERROR_TYPE_IMAGE_LOAD, FilePath);
		return;
	}

	if (X + ClipWidth > Width || Y + ClipHeight > Height) {
		stbi_image_free(TextureData);
		return;
	}

	unsigned char* ClippedTextureData = (unsigned char*)malloc(ClipWidth * ClipHeight * Channel);
	if (!ClippedTextureData) {
		stbi_image_free(TextureData);
		return;
	}

	for (int Row = 0; Row < ClipHeight; ++Row)
		memcpy(ClippedTextureData + Row * ClipWidth * Channel, TextureData + ((Y + Row) * Width + X) * Channel, ClipWidth * Channel);

	ImageStruct.Width = ClipWidth;
	ImageStruct.Height = ClipHeight;
	ILBD.ImagePtr = &ImageStruct;
	ILBD.ImageType = Type;
	ILBD.TextureData = ClippedTextureData;

	stbi_image_free(TextureData);

	ImageLoadBuffer.emplace_back(ILBD);
}

void SDK::SDK_ImageTool::LoadSpriteSheetT(SDK::SpriteSheet& SpriteSheetStruct, std::string FilePath, int Type) {
	SDK::SpriteSheetLoadBufferData SLBD{};
	int Width{}, Height{}, Channel{};
	unsigned char* TextureData = stbi_load(FilePath.c_str(), &Width, &Height, &Channel, 4);
	if (!TextureData) {
		SDK::Scene.SetErrorScreen(ERROR_TYPE_IMAGE_LOAD, FilePath);
		return;
	}

	SpriteSheetStruct.Texture.assign(NumRow * NumCol, {});

	if (BlankLocation != 0)
		SpriteSheetStruct.Frame = NumRow * NumCol - (NumRow - BlankLocation);
	else
		SpriteSheetStruct.Frame = NumRow * NumCol;

	int CurrentXPosition = 0;
	int CurrentYPosition = Height - ClipHeight * StartLocation;
	SDK::EXTool.ClampValue(CurrentYPosition, 0, CLAMP_LESS);

	for (int C = 0; C < NumCol; ++C) {
		for (int R = 0; R < NumRow; ++R) {
			unsigned char* ClippedTextureData = (unsigned char*)malloc(ClipWidth * ClipHeight * Channel);
			if (!ClippedTextureData) {
				stbi_image_free(TextureData);
				return;
			}

			for (int i = 0; i < ClipHeight; ++i)
				memcpy(ClippedTextureData + i * ClipWidth * Channel, TextureData + ((CurrentYPosition + i) * Width + CurrentXPosition) * Channel, ClipWidth * Channel);

			SLBD.TextureData.emplace_back(ClippedTextureData);

			CurrentXPosition += ClipWidth;
			SDK::EXTool.ClampValue(CurrentXPosition, Width, CLAMP_GREATER);
		}

		CurrentYPosition -= ClipHeight;
		SDK::EXTool.ClampValue(CurrentYPosition, 0, CLAMP_LESS);

		CurrentXPosition = 0;
	}

	SpriteSheetStruct.Width = ClipWidth;
	SpriteSheetStruct.Height = ClipHeight;

	SLBD.SpriteSheetPtr = &SpriteSheetStruct;
	SLBD.ImageType = Type;

	SpriteSheetLoadBuffer.emplace_back(SLBD);

	stbi_image_free(TextureData);
}

void SDK::SDK_ImageTool::Map() {
	for (auto& B : ImageLoadBuffer) {
		glGenTextures(1, &B.ImagePtr->Texture);
		glBindTexture(GL_TEXTURE_2D, B.ImagePtr->Texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		switch (B.ImageType) {
		case IMAGE_TYPE_LINEAR:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;

		case IMAGE_TYPE_NEAREST:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		}

		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, B.ImagePtr->Width, B.ImagePtr->Height);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, B.ImagePtr->Width, B.ImagePtr->Height, GL_RGBA, GL_UNSIGNED_BYTE, B.TextureData);
		stbi_image_free(B.TextureData);
	}

	ImageLoadBuffer.clear();
	
	for (auto& B : SpriteSheetLoadBuffer) {
		size_t SheetSize = B.SpriteSheetPtr->Texture.size();
		for (int i = 0; i < SheetSize; ++i) {
			glGenTextures(1, &B.SpriteSheetPtr->Texture[i]);
			glBindTexture(GL_TEXTURE_2D, B.SpriteSheetPtr->Texture[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			switch (B.ImageType) {
			case IMAGE_TYPE_LINEAR:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;

			case IMAGE_TYPE_NEAREST:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				break;
			}

			glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, B.SpriteSheetPtr->Width, B.SpriteSheetPtr->Height);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, B.SpriteSheetPtr->Width, B.SpriteSheetPtr->Height, GL_RGBA, GL_UNSIGNED_BYTE, B.TextureData[i]);
			stbi_image_free(B.TextureData[i]);
		}
	}

	SpriteSheetLoadBuffer.clear();
}

void SDK::SDK_ImageTool::SetLocalBlur(float BlurStrength) {
	LocalBlurValue = BlurStrength;
}

void SDK::SDK_ImageTool::SetGlobalBlur(float BlurStrength) {
	GlobalBlurValue = BlurStrength;
}

void SDK::SDK_ImageTool::SetGlobalOpacity(float Value) {
	GlobalOpacityValue = Value;
}

void SDK::SDK_ImageTool::SetGlobalColor(float R, float G, float B) {
	GlobalColorValue.r = R;
	GlobalColorValue.g = G;
	GlobalColorValue.b = B;
}

void SDK::SDK_ImageTool::SetGlobalColor(SDK::Color3& Color) {
	GlobalColorValue = Color;
}

void SDK::SDK_ImageTool::SetGlobalColorRGB(int R, int G, int B) {
	GlobalColorValue.r = (1.0f / 255.0f) * (float)R;
	GlobalColorValue.g = (1.0f / 255.0f) * (float)G;
	GlobalColorValue.b = (1.0f / 255.0f) * (float)B;
}

void SDK::SDK_ImageTool::SetLocalColor(float R, float G, float B) {
	LocalColorValue.r = R;
	LocalColorValue.g = G;
	LocalColorValue.b = B;
}

void SDK::SDK_ImageTool::SetLocalColor(SDK::Color3& Color) {
	LocalColorValue = Color;
}

void SDK::SDK_ImageTool::SetLocalColorRGB(int R, int G, int B) {
	LocalColorValue.r = (1.0f / 255.0f) * (float)R;
	LocalColorValue.g = (1.0f / 255.0f) * (float)G;
	LocalColorValue.b = (1.0f / 255.0f) * (float)B;
}

void SDK::SDK_ImageTool::RenderImage(SDK::Image& ImageStruct, float OpacityValue, bool ApplyGlobalAttribute, bool DisableAdjustAspect) {
	float Width = (float)ImageStruct.Width;
	float Height = (float)ImageStruct.Height;

	ProcessTransform(Width, Height, OpacityValue, ApplyGlobalAttribute, DisableAdjustAspect);
	PrepareRender(ImageStruct);

	glBindTexture(GL_TEXTURE_2D, ImageStruct.Texture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void SDK::SDK_ImageTool::RenderSpriteSheet(SDK::SpriteSheet& SpriteSheetStruct, float& Frame, float OpacityValue, bool ApplyGlobalAttribute, bool DisableAdjustAspect) {
	if ((int)Frame >= SpriteSheetStruct.Frame)
		Frame = 0.0;

	float Width = (float)SpriteSheetStruct.Width;
	float Height = (float)SpriteSheetStruct.Height;

	ProcessTransform(Width, Height, OpacityValue, ApplyGlobalAttribute, DisableAdjustAspect);
	PrepareRender(SpriteSheetStruct);

	glBindTexture(GL_TEXTURE_2D, SpriteSheetStruct.Texture[(int)Frame]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void SDK::SDK_ImageTool::RenderStaticSpriteSheet(SDK::SpriteSheet& SpriteSheetStruct, int Frame, float OpacityValue, bool ApplyGlobalAttribute, bool DisableAdjustAspect) {
	if (Frame > SpriteSheetStruct.Frame)
		return;
	
	float Width = (float)SpriteSheetStruct.Width;
	float Height = (float)SpriteSheetStruct.Height;

	ProcessTransform(Width, Height, OpacityValue, ApplyGlobalAttribute, DisableAdjustAspect);
	PrepareRender(SpriteSheetStruct);

	glBindTexture(GL_TEXTURE_2D, SpriteSheetStruct.Texture[Frame]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void SDK::SDK_ImageTool::RenderRaw() {
	glBindTexture(GL_TEXTURE_2D, 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void SDK::SDK_ImageTool::UnmapImage(SDK::Image& ImageStruct) {
	glDeleteTextures(1, &ImageStruct.Texture);
}

void SDK::SDK_ImageTool::UnmapSpriteSheet(SDK::SpriteSheet& SpriteSheetStruct) {
	size_t SheetSize = SpriteSheetStruct.Texture.size();
	for(int i = 0; i < SheetSize; ++i)
		glDeleteTextures(1, &SpriteSheetStruct.Texture[i]);
}

void SDK::SDK_ImageTool::PrepareRender(SDK::Image& ImageStruct) {
	glUseProgram(IMAGE_SHADER);
	SDK::Camera.PrepareRender(SHADER_TYPE_IMAGE);

	glUniform1f(IMAGE_OPACITY_LOCATION, LocalOpacityValue);
	glUniform3f(IMAGE_COLOR_LOCATION, LocalColorValue.r, LocalColorValue.g, LocalColorValue.b);

	if (LocalBlurValue > 0.0) {
		glUniform1i(BLUR_STATE_LOCATION, true);
		glUniform1i(BLUR_EXECUTION_LOCATION, BLUR_EXECUTION);
		glUniform1f(BLUR_STRENGTH_LOCATION, LocalBlurValue);
		glUniform2f(TEXTURE_SIZE_LOCATION, 1.0 / (float)ImageStruct.Width, 1.0 / (float)ImageStruct.Height);
	}
	else
		glUniform1i(BLUR_STATE_LOCATION, false);

	glUniform1i(CLIPPING_STATE_LOCATION, System_ClippingState);

	glUniformMatrix4fv(IMAGE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(ResultMatrix));
}

void SDK::SDK_ImageTool::PrepareRender(SDK::SpriteSheet& SpriteSheetStruct) {
	glUseProgram(IMAGE_SHADER);
	SDK::Camera.PrepareRender(SHADER_TYPE_IMAGE);

	glUniform1f(IMAGE_OPACITY_LOCATION, LocalOpacityValue);
	glUniform3f(IMAGE_COLOR_LOCATION, LocalColorValue.r, LocalColorValue.g, LocalColorValue.b);

	if (LocalBlurValue > 0.0) {
		glUniform1i(BLUR_STATE_LOCATION, true);
		glUniform1i(BLUR_EXECUTION_LOCATION, BLUR_EXECUTION);
		glUniform1f(BLUR_STRENGTH_LOCATION, LocalBlurValue);
		glUniform2f(TEXTURE_SIZE_LOCATION, 1.0 / (float)SpriteSheetStruct.Width, 1.0 / (float)SpriteSheetStruct.Height);
	}
	else
		glUniform1i(BLUR_STATE_LOCATION, false);

	glUniform1i(CLIPPING_STATE_LOCATION, System_ClippingState);

	glUniformMatrix4fv(IMAGE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(ResultMatrix));
}

void SDK::SDK_ImageTool::ProcessTransform(float Width, float Height, float OpacityValue, bool ApplyGlobalAttribute, bool DisableAdjustAspect) {
	SDK::Transform.Identity(ResultMatrix);

	ResultMatrix = LocalMatrix;

	if (!DisableAdjustAspect) {
		if (Width > Height) {
			ImageAspectMatrix = scale(ImageAspectMatrix, SDK::Vector3(1.0, Height / Width, 1.0));
			ResultMatrix *= ImageAspectMatrix;
		}
		else if (Width < Height) {
			ImageAspectMatrix = scale(ImageAspectMatrix, SDK::Vector3(Width / Height, 1.0, 1.0));
			ResultMatrix *= ImageAspectMatrix;
		}
	}

	if (!SDK::Transform.CheckIdentity(LocalFlipMatrix))
		ResultMatrix *= LocalFlipMatrix;

	LocalOpacityValue = OpacityValue;

	if (ApplyGlobalAttribute) {
		if (!SDK::Transform.CheckIdentity(GlobalMatrix))
			ResultMatrix = GlobalMatrix * ResultMatrix;
		if (!SDK::Transform.CheckIdentity(GlobalFlipMatrix))
			ResultMatrix *= GlobalFlipMatrix;

		LocalColorValue += GlobalColorValue;
		LocalOpacityValue *= GlobalOpacityValue;
		LocalBlurValue *= GlobalBlurValue;
	}

	SDK::Transform.Identity(ImageAspectMatrix);
}
