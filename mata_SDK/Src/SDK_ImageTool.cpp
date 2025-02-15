#include "SDK_ImageTool.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "SystemResource.h"
#include "SDK_EXTool.h"
#include "SDK_Scene.h"

#include "SDK_ObjectValue.h"
#include "SDK_Transform.h"
#include "SDK_ComputeTool.h"


SDK_ImageTool SDK::ImageTool;

GLfloat ImagePannel[][48] = {  // default size 1.0 * 1.0
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0, 0.0,
	 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0, 0.0,
	 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0, 1.0,
	 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0, 1.0,
	-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0, 1.0,
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0, 0.0
};

void SDK_ImageTool::Init() {
	GLuint VBO{};
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(ImagePannel), ImagePannel, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	stbi_set_flip_vertically_on_load(true);
}

void SDK_ImageTool::LoadImage(Image& ImageStruct, std::string FilePath, int Type) {
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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, TextureData);
	stbi_image_free(TextureData);

	ImageStruct.Width = Width;
	ImageStruct.Height = Height;
}

void SDK_ImageTool::LoadClip(Image& ImageStruct, std::string FilePath, int X, int Y, int ClipWidth, int ClipHeight, int Type) {
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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ClipWidth, ClipHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, ClippedTextureData);
	stbi_image_free(TextureData);
	stbi_image_free(ClippedTextureData);

	ImageStruct.Width = ClipWidth;
	ImageStruct.Height = ClipHeight;
}

void SDK_ImageTool::SetSpriteSheetSize(int ValueClipWidth, int ValueClipHeight, int ValueNumRow, int ValueNumCol, int ValueStartCol, int ValueNumBlank) {
	ClipWidth = ValueClipWidth;
	ClipHeight = ValueClipHeight;
	NumRow = ValueNumRow;
	NumCol = ValueNumCol;
	StartLocation = ValueStartCol;
	BlankLocation = ValueNumBlank;
}

void SDK_ImageTool::LoadSpriteSheet(SpriteSheet& SpriteSheetStruct, std::string FilePath, int Type) {
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

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ClipWidth, ClipHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, ClippedTextureData);
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

void SDK_ImageTool::LoadSpriteSheetT(SpriteSheet& SpriteSheetStruct, std::string FilePath, int Type) {
	PreLoadSpriteSheetInfo PLSS{};
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

			PLSS.TextureData.emplace_back(ClippedTextureData);

			CurrentXPosition += ClipWidth;
			SDK::EXTool.ClampValue(CurrentXPosition, Width, CLAMP_GREATER);
		}

		CurrentYPosition -= ClipHeight;
		SDK::EXTool.ClampValue(CurrentYPosition, 0, CLAMP_LESS);

		CurrentXPosition = 0;
	}

	SpriteSheetStruct.Width = ClipWidth;
	SpriteSheetStruct.Height = ClipHeight;

	PLSS.SpriteSheetPtr = &SpriteSheetStruct;
	PLSS.ImageType = Type;

	LoadSpriteSheetBuffer.emplace_back(PLSS);

	stbi_image_free(TextureData);
}

void SDK_ImageTool::LoadImageT(Image& ImageStruct, std::string FilePath, int Type) {
	PreLoadInfo PLI{};
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

	LoadBuffer.emplace_back(PLI);
}

void SDK_ImageTool::LoadClipT(Image& ImageStruct, std::string FilePath, int X, int Y, int ClipWidth, int ClipHeight, int Type) {
	PreLoadInfo PLI{};
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
	PLI.ImagePtr = &ImageStruct;
	PLI.ImageType = Type;
	PLI.TextureData = ClippedTextureData;

	stbi_image_free(TextureData);

	LoadBuffer.emplace_back(PLI);
}

void SDK_ImageTool::Map() {
	if (!LoadBuffer.empty()) {
		for (auto& B : LoadBuffer) {
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

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, B.ImagePtr->Width, B.ImagePtr->Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, B.TextureData);
			stbi_image_free(B.TextureData);
		}

		LoadBuffer.clear();
	}

	if (!LoadSpriteSheetBuffer.empty()) {
		for (auto& B : LoadSpriteSheetBuffer) {
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

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, B.SpriteSheetPtr->Width, B.SpriteSheetPtr->Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, B.TextureData[i]);
				stbi_image_free(B.TextureData[i]);
			}
		}

		LoadSpriteSheetBuffer.clear();
	}
}

void SDK_ImageTool::Blur(GLfloat BlurStrength) {
	ObjectBlurValue = BlurStrength;
}

void SDK_ImageTool::UnitBlur(GLfloat BlurStrength) {
	UnitBlurValue = BlurStrength;
}

void SDK_ImageTool::UnitOpacity(GLfloat Value) {
	UnitOpacityValue = Value;
}

void SDK_ImageTool::SetColor(GLfloat R, GLfloat G, GLfloat B) {
	ObjectColor.r = R;
	ObjectColor.g = G;
	ObjectColor.b = B;
}

void SDK_ImageTool::SetColor(glm::vec3& Color) {
	ObjectColor.r = Color.r;
	ObjectColor.g = Color.g;
	ObjectColor.b = Color.b;
}

void SDK_ImageTool::SetColorRGB(int R, int G, int B) {
	ObjectColor.r = (1.0f / 255.0f) * (GLfloat)R;
	ObjectColor.g = (1.0f / 255.0f) * (GLfloat)G;
	ObjectColor.b = (1.0f / 255.0f) * (GLfloat)B;
}

void SDK_ImageTool::RenderImage(Image& ImageStruct, GLfloat OpacityValue, bool ApplyUnitTransform, bool DisableAdjustAspect) {
	GLfloat Width = (GLfloat)ImageStruct.Width;
	GLfloat Height = (GLfloat)ImageStruct.Height;

	ProcessTransform(Width, Height, OpacityValue, ApplyUnitTransform, DisableAdjustAspect);
	PrepareRender(ImageStruct);

	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, ImageStruct.Texture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void SDK_ImageTool::RenderSpriteSheet(SpriteSheet& SpriteSheetStruct, GLfloat& Frame, GLfloat OpacityValue, bool ApplyUnitTransform, bool DisableAdjustAspect) {
	if ((int)Frame >= SpriteSheetStruct.Frame)
		Frame = 0.0;

	GLfloat Width = (GLfloat)SpriteSheetStruct.Width;
	GLfloat Height = (GLfloat)SpriteSheetStruct.Height;

	ProcessTransform(Width, Height, OpacityValue, ApplyUnitTransform, DisableAdjustAspect);
	PrepareRender(SpriteSheetStruct);

	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, SpriteSheetStruct.Texture[(int)Frame]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void SDK_ImageTool::RenderStaticSpriteSheet(SpriteSheet& SpriteSheetStruct, int Frame, GLfloat OpacityValue, bool ApplyUnitTransform, bool DisableAdjustAspect) {
	GLfloat Width = (GLfloat)SpriteSheetStruct.Width;
	GLfloat Height = (GLfloat)SpriteSheetStruct.Height;

	ProcessTransform(Width, Height, OpacityValue, ApplyUnitTransform, DisableAdjustAspect);
	PrepareRender(SpriteSheetStruct);

	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, SpriteSheetStruct.Texture[Frame]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void SDK_ImageTool::RenderRaw() {
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void SDK_ImageTool::UnmapImage(Image& ImageStruct) {
	glDeleteTextures(1, &ImageStruct.Texture);
}

void SDK_ImageTool::UnmapSpriteSheet(SpriteSheet& SpriteSheetStruct) {
	size_t SheetSize = SpriteSheetStruct.Texture.size();
	for(int i = 0; i < SheetSize; ++i)
		glDeleteTextures(1, &SpriteSheetStruct.Texture[i]);
}

void SDK_ImageTool::PrepareRender(Image& ImageStruct) {
	glUseProgram(IMAGE_SHADER);
	SDK::Camera.PrepareRender(SHADER_TYPE_IMAGE);

	glUniform1f(IMAGE_OPACITY_LOCATION, ObjectOpacityValue);
	glUniform3f(IMAGE_COLOR_LOCATION, ObjectColor.r, ObjectColor.g, ObjectColor.b);

	if (ObjectBlurValue > 0.0) {
		glUniform1i(BLUR_STATE_LOCATION, 1);
		glUniform1f(BLUR_STRENGTH_LOCATION, ObjectBlurValue);
		glUniform2f(TEXTURE_SIZE_LOCATION, 1.0 / (GLfloat)ImageStruct.Width, 1.0 / (GLfloat)ImageStruct.Height);
	}
	else
		glUniform1i(BLUR_STATE_LOCATION, 0);

	glUniformMatrix4fv(IMAGE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(ResultMatrix));
}

void SDK_ImageTool::PrepareRender(SpriteSheet& SpriteSheetStruct) {
	glUseProgram(IMAGE_SHADER);
	SDK::Camera.PrepareRender(SHADER_TYPE_IMAGE);

	glUniform1f(IMAGE_OPACITY_LOCATION, ObjectOpacityValue);
	glUniform3f(IMAGE_COLOR_LOCATION, ObjectColor.r, ObjectColor.g, ObjectColor.b);

	if (ObjectBlurValue > 0.0) {
		glUniform1i(BLUR_STATE_LOCATION, 1);
		glUniform1f(BLUR_STRENGTH_LOCATION, ObjectBlurValue);
		glUniform2f(TEXTURE_SIZE_LOCATION, 1.0 / (GLfloat)SpriteSheetStruct.Width, 1.0 / (GLfloat)SpriteSheetStruct.Height);
	}
	else
		glUniform1i(BLUR_STATE_LOCATION, 0);

	glUniformMatrix4fv(IMAGE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(ResultMatrix));
}

void SDK_ImageTool::ProcessTransform(GLfloat Width, GLfloat Height, GLfloat OpacityValue, bool DisableAdjustAspect, bool ApplyUnitTransform) {
	if (!DisableAdjustAspect)
		SDK::Transform.ImageScale(ImageAspectMatrix, Width, Height);

	if (USE_COMPUTE_SHADER)
		SDK::ComputeTool.ComputeMatrix(ResultMatrix, MoveMatrix, RotateMatrix, ScaleMatrix, ImageAspectMatrix, FlipMatrix);
	else {
		if (!SDK::Transform.CheckIdentity(MoveMatrix)) { ResultMatrix *= MoveMatrix; }
		if (!SDK::Transform.CheckIdentity(RotateMatrix)) { ResultMatrix *= RotateMatrix; }
		if (!SDK::Transform.CheckIdentity(ScaleMatrix)) { ResultMatrix *= ScaleMatrix; }
		if (!SDK::Transform.CheckIdentity(ImageAspectMatrix)) { ResultMatrix *= ImageAspectMatrix; }
		if (!SDK::Transform.CheckIdentity(FlipMatrix)) { ResultMatrix *= FlipMatrix; }
	}

	ObjectOpacityValue = OpacityValue;

	if (ApplyUnitTransform) {
		if (USE_COMPUTE_SHADER)
			SDK::ComputeTool.ComputeMatrix(ResultMatrix, UnitMoveMatrix, UnitRotateMatrix, UnitScaleMatrix, UnitFlipMatrix, ResultMatrix);
		else {
			if (!SDK::Transform.CheckIdentity(UnitMoveMatrix)) { ResultMatrix = UnitMoveMatrix * ResultMatrix; }
			if (!SDK::Transform.CheckIdentity(UnitRotateMatrix)) { ResultMatrix = UnitRotateMatrix * ResultMatrix; }
			if (!SDK::Transform.CheckIdentity(UnitScaleMatrix)) { ResultMatrix = UnitScaleMatrix * ResultMatrix; }
			if (!SDK::Transform.CheckIdentity(UnitFlipMatrix)) { ResultMatrix *= UnitFlipMatrix; }
		}
		ObjectOpacityValue = ObjectOpacityValue * UnitOpacityValue;
		ObjectBlurValue = ObjectBlurValue * UnitBlurValue;
	}
}
