#include "QuadPent_ImageTool.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "QuadPent_EXTool.h"
#include "QuadPent_Scene.h"

#include "QuadPent_ObjectValue.h"
#include "QuadPent_Transform.h"


QP::QuadPent_ImageTool QP::ImageTool;

float ImagePannel[][48] = {  // default size 1.0 * 1.0
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0, 0.0,
	 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0, 0.0,
	 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0, 1.0,
	 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0, 1.0,
	-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0, 1.0,
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0, 0.0
};

void QP::QuadPent_ImageTool::Init() {
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

void QP::QuadPent_ImageTool::LoadImage(QP::Image& ImageStruct, std::string FilePath, int Type) {
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
		QP::Scene.SetErrorScreen(ERROR_TYPE_IMAGE_LOAD, FilePath);
		return;
	}

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, Width, Height);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Width, Height, GL_RGBA, GL_UNSIGNED_BYTE, TextureData);
	stbi_image_free(TextureData);

	ImageStruct.Width = Width;
	ImageStruct.Height = Height;

	if (Width > Height) {
		ImageStruct.RealWidth = 1.0;
		ImageStruct.RealHeight = (float)Height / (float)Width;
	}
	else if (Width < Height) {
		ImageStruct.RealWidth = (float)Width / (float)Height;
		ImageStruct.RealHeight = 1.0;
	}
	else {
		ImageStruct.RealWidth = 1.0;
		ImageStruct.RealHeight = 1.0;
	}
}

void QP::QuadPent_ImageTool::LoadClip(QP::Image& ImageStruct, std::string FilePath, int X, int Y, int ClipWidth, int ClipHeight, int Type) {
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
		QP::Scene.SetErrorScreen(ERROR_TYPE_IMAGE_LOAD, FilePath);
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

	if (ClipWidth > ClipHeight) {
		ImageStruct.RealWidth = 1.0;
		ImageStruct.RealHeight = (float)ClipHeight / (float)ClipWidth;
	}
	else if (ClipWidth < ClipHeight) {
		ImageStruct.RealWidth = (float)ClipWidth / (float)ClipHeight;
		ImageStruct.RealHeight = 1.0;
	}
	else {
		ImageStruct.RealWidth = 1.0;
		ImageStruct.RealHeight = 1.0;
	}
}

void QP::QuadPent_ImageTool::SetSpriteSheetSize(int ValueClipWidth, int ValueClipHeight, int ValueNumRow, int ValueNumCol, int ValueStartCol, int ValueNumBlank) {
	ClipWidth = ValueClipWidth;
	ClipHeight = ValueClipHeight;
	NumRow = ValueNumRow;
	NumCol = ValueNumCol;
	StartLocation = ValueStartCol;
	BlankLocation = ValueNumBlank;
}

void QP::QuadPent_ImageTool::LoadSpriteSheet(QP::SpriteSheet& SpriteSheetStruct, std::string FilePath, int Type) {
	int Width{}, Height{}, Channel{};
	unsigned char* TextureData = stbi_load(FilePath.c_str(), &Width, &Height, &Channel, 4);
	if (!TextureData) {
		QP::Scene.SetErrorScreen(ERROR_TYPE_IMAGE_LOAD, FilePath);
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
	QP::EXTool.ClampValue(CurrentYPosition, 0, CLAMP_LESS);

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
			QP::EXTool.ClampValue(CurrentXPosition, Width, CLAMP_GREATER);

			++CurrentIndex;
		}

		CurrentYPosition -= ClipHeight;
		QP::EXTool.ClampValue(CurrentYPosition, 0, CLAMP_LESS);

		CurrentXPosition = 0;
	}

	stbi_image_free(TextureData);

	SpriteSheetStruct.Width = ClipWidth;
	SpriteSheetStruct.Height = ClipHeight;

	if (ClipWidth > ClipHeight) {
		SpriteSheetStruct.RealWidth = 1.0;
		SpriteSheetStruct.RealHeight = (float)ClipHeight / (float)ClipWidth;
	}
	else if (ClipWidth < ClipHeight) {
		SpriteSheetStruct.RealWidth = (float)ClipWidth / (float)ClipHeight;
		SpriteSheetStruct.RealHeight = 1.0;
	}
	else {
		SpriteSheetStruct.RealWidth = 1.0;
		SpriteSheetStruct.RealHeight = 1.0;
	}
}

void QP::QuadPent_ImageTool::LoadImageT(QP::Image& ImageStruct, std::string FilePath, int Type) {
	QP::ImageLoadBufferData Buffer{};
	int Width{}, Height{}, Channel{};
	unsigned char* TextureData = stbi_load(FilePath.c_str(), &Width, &Height, &Channel, 4);
	if (!TextureData) {
		QP::Scene.SetErrorScreen(ERROR_TYPE_IMAGE_LOAD, FilePath);
		return;
	}

	ImageStruct.Width = Width;
	ImageStruct.Height = Height;

	if (Width > Height) {
		ImageStruct.RealWidth = 1.0;
		ImageStruct.RealHeight = (float)Height / (float)Width;
	}
	else if (Width < Height) {
		ImageStruct.RealWidth = (float)Width / (float)Height;
		ImageStruct.RealHeight = 1.0;
	}
	else {
		ImageStruct.RealWidth = 1.0;
		ImageStruct.RealHeight = 1.0;
	}

	Buffer.ImagePtr = &ImageStruct;
	Buffer.ImageType = Type;
	Buffer.TextureData = TextureData;

	ImageLoadBuffer.emplace_back(Buffer);
}

void QP::QuadPent_ImageTool::LoadClipT(QP::Image& ImageStruct, std::string FilePath, int X, int Y, int ClipWidth, int ClipHeight, int Type) {
	QP::ImageLoadBufferData Buffer{};
	int Width{}, Height{}, Channel{};
	unsigned char* TextureData = stbi_load(FilePath.c_str(), &Width, &Height, &Channel, 4);
	if (!TextureData) {
		QP::Scene.SetErrorScreen(ERROR_TYPE_IMAGE_LOAD, FilePath);
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

	if (ClipWidth > ClipHeight) {
		ImageStruct.RealWidth = 1.0;
		ImageStruct.RealHeight = (float)ClipHeight / (float)ClipWidth;
	}
	else if (ClipWidth < ClipHeight) {
		ImageStruct.RealWidth = (float)ClipWidth / (float)ClipHeight;
		ImageStruct.RealHeight = 1.0;
	}
	else {
		ImageStruct.RealWidth = 1.0;
		ImageStruct.RealHeight = 1.0;
	}

	Buffer.ImagePtr = &ImageStruct;
	Buffer.ImageType = Type;
	Buffer.TextureData = ClippedTextureData;

	stbi_image_free(TextureData);

	ImageLoadBuffer.emplace_back(Buffer);
}

void QP::QuadPent_ImageTool::LoadSpriteSheetT(QP::SpriteSheet& SpriteSheetStruct, std::string FilePath, int Type) {
	QP::SpriteSheetLoadBufferData Buffer{};
	int Width{}, Height{}, Channel{};
	unsigned char* TextureData = stbi_load(FilePath.c_str(), &Width, &Height, &Channel, 4);
	if (!TextureData) {
		QP::Scene.SetErrorScreen(ERROR_TYPE_IMAGE_LOAD, FilePath);
		return;
	}

	SpriteSheetStruct.Texture.assign(NumRow * NumCol, {});

	if (BlankLocation != 0)
		SpriteSheetStruct.Frame = NumRow * NumCol - (NumRow - BlankLocation);
	else
		SpriteSheetStruct.Frame = NumRow * NumCol;

	int CurrentXPosition = 0;
	int CurrentYPosition = Height - ClipHeight * StartLocation;
	QP::EXTool.ClampValue(CurrentYPosition, 0, CLAMP_LESS);

	for (int C = 0; C < NumCol; ++C) {
		for (int R = 0; R < NumRow; ++R) {
			unsigned char* ClippedTextureData = (unsigned char*)malloc(ClipWidth * ClipHeight * Channel);
			if (!ClippedTextureData) {
				stbi_image_free(TextureData);
				return;
			}

			for (int i = 0; i < ClipHeight; ++i)
				memcpy(ClippedTextureData + i * ClipWidth * Channel, TextureData + ((CurrentYPosition + i) * Width + CurrentXPosition) * Channel, ClipWidth * Channel);

			Buffer.TextureData.emplace_back(ClippedTextureData);

			CurrentXPosition += ClipWidth;
			QP::EXTool.ClampValue(CurrentXPosition, Width, CLAMP_GREATER);
		}

		CurrentYPosition -= ClipHeight;
		QP::EXTool.ClampValue(CurrentYPosition, 0, CLAMP_LESS);

		CurrentXPosition = 0;
	}

	SpriteSheetStruct.Width = ClipWidth;
	SpriteSheetStruct.Height = ClipHeight;

	if (ClipWidth > ClipHeight) {
		SpriteSheetStruct.RealWidth = 1.0;
		SpriteSheetStruct.RealHeight = (float)ClipHeight / (float)ClipWidth;
	}
	else if (ClipWidth < ClipHeight) {
		SpriteSheetStruct.RealWidth = (float)ClipWidth / (float)ClipHeight;
		SpriteSheetStruct.RealHeight = 1.0;
	}
	else {
		SpriteSheetStruct.RealWidth = 1.0;
		SpriteSheetStruct.RealHeight = 1.0;
	}

	Buffer.SpriteSheetPtr = &SpriteSheetStruct;
	Buffer.ImageType = Type;

	SpriteSheetLoadBuffer.emplace_back(Buffer);

	stbi_image_free(TextureData);
}

void QP::QuadPent_ImageTool::Map() {
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

void QP::QuadPent_ImageTool::SetLocalBlur(float BlurStrength) {
	LocalBlurValue = BlurStrength;
}

void QP::QuadPent_ImageTool::SetGlobalBlur(float BlurStrength) {
	GlobalBlurValue = BlurStrength;
}

void QP::QuadPent_ImageTool::SetGlobalOpacity(float Value) {
	GlobalOpacityValue = Value;
}

void QP::QuadPent_ImageTool::SetGlobalColor(float R, float G, float B) {
	GlobalColorValue.r = R;
	GlobalColorValue.g = G;
	GlobalColorValue.b = B;
}

void QP::QuadPent_ImageTool::SetGlobalColor(QP::Color3& Color) {
	GlobalColorValue = Color;
}

void QP::QuadPent_ImageTool::SetGlobalColorRGB(int R, int G, int B) {
	GlobalColorValue.r = (1.0f / 255.0f) * (float)R;
	GlobalColorValue.g = (1.0f / 255.0f) * (float)G;
	GlobalColorValue.b = (1.0f / 255.0f) * (float)B;
}

void QP::QuadPent_ImageTool::SetLocalColor(float R, float G, float B) {
	LocalColorValue.r = R;
	LocalColorValue.g = G;
	LocalColorValue.b = B;
}

void QP::QuadPent_ImageTool::SetLocalColor(QP::Color3& Color) {
	LocalColorValue = Color;
}

void QP::QuadPent_ImageTool::SetLocalColorRGB(int R, int G, int B) {
	LocalColorValue.r = (1.0f / 255.0f) * (float)R;
	LocalColorValue.g = (1.0f / 255.0f) * (float)G;
	LocalColorValue.b = (1.0f / 255.0f) * (float)B;
}

QP::Vector2 QP::QuadPent_ImageTool::RealSize(QP::Image& ImageStruct) {
	return QP::Vector2(ImageStruct.RealWidth, ImageStruct.RealHeight);
}

QP::Vector2 QP::QuadPent_ImageTool::RealSize(QP::SpriteSheet& SpriteShetStruct) {
	return QP::Vector2(SpriteShetStruct.RealWidth, SpriteShetStruct.RealHeight);
}

void QP::QuadPent_ImageTool::RenderImage(QP::Image& ImageStruct, float OpacityValue, bool ApplyGlobalAttribute, bool DisableAdjustAspect) {
	ProcessTransform(ImageStruct.RealWidth, ImageStruct.RealHeight, OpacityValue, ApplyGlobalAttribute, DisableAdjustAspect);
	PrepareRender(ImageStruct);

	glBindTexture(GL_TEXTURE_2D, ImageStruct.Texture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void QP::QuadPent_ImageTool::RenderSpriteSheet(QP::SpriteSheet& SpriteSheetStruct, float& Frame, float OpacityValue, bool ApplyGlobalAttribute, bool DisableAdjustAspect) {
	if (Frame <= 0.0)
		return;
	
	if ((int)Frame >= SpriteSheetStruct.Frame)
		Frame = 0.0;

	ProcessTransform(SpriteSheetStruct.RealWidth, SpriteSheetStruct.RealHeight, OpacityValue, ApplyGlobalAttribute, DisableAdjustAspect);
	PrepareRender(SpriteSheetStruct);

	glBindTexture(GL_TEXTURE_2D, SpriteSheetStruct.Texture[(int)Frame]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void QP::QuadPent_ImageTool::RenderStaticSpriteSheet(QP::SpriteSheet& SpriteSheetStruct, int Frame, float OpacityValue, bool ApplyGlobalAttribute, bool DisableAdjustAspect) {
	if (Frame > SpriteSheetStruct.Frame || Frame < 0)
		return;

	ProcessTransform(SpriteSheetStruct.RealWidth, SpriteSheetStruct.RealHeight, OpacityValue, ApplyGlobalAttribute, DisableAdjustAspect);
	PrepareRender(SpriteSheetStruct);

	glBindTexture(GL_TEXTURE_2D, SpriteSheetStruct.Texture[Frame]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void QP::QuadPent_ImageTool::RenderRaw() {
	glBindTexture(GL_TEXTURE_2D, 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void QP::QuadPent_ImageTool::UnmapImage(QP::Image& ImageStruct) {
	glDeleteTextures(1, &ImageStruct.Texture);
}

void QP::QuadPent_ImageTool::UnmapSpriteSheet(QP::SpriteSheet& SpriteSheetStruct) {
	size_t SheetSize = SpriteSheetStruct.Texture.size();
	for(int i = 0; i < SheetSize; ++i)
		glDeleteTextures(1, &SpriteSheetStruct.Texture[i]);
}

void QP::QuadPent_ImageTool::PrepareRender(QP::Image& ImageStruct) {
	glUseProgram(IMAGE_SHADER);
	QP::Camera.PrepareRender(SHADER_TYPE_IMAGE);

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

void QP::QuadPent_ImageTool::PrepareRender(QP::SpriteSheet& SpriteSheetStruct) {
	glUseProgram(IMAGE_SHADER);
	QP::Camera.PrepareRender(SHADER_TYPE_IMAGE);

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

void QP::QuadPent_ImageTool::ProcessTransform(float Width, float Height, float OpacityValue, bool ApplyGlobalAttribute, bool DisableAdjustAspect) {
	QP::Transform.Identity(ResultMatrix);
	QP::Transform.Identity(ImageAspectMatrix);

	ResultMatrix = LocalMatrix;

	if (!DisableAdjustAspect && (Width != 1.0 || Height != 1.0)) {
		ImageAspectMatrix = scale(ImageAspectMatrix, QP::Vector3(Width, Height, 1.0));
		ResultMatrix *= ImageAspectMatrix;
	}

	switch (LocalFlipFlag) {
	case FLIP_TYPE_H:
		ResultMatrix = rotate(ResultMatrix, glm::radians(180.0f), QP::Vector3(0.0, 1.0, 0.0));
		break;
	case FLIP_TYPE_V:
		ResultMatrix = rotate(ResultMatrix, glm::radians(180.0f), QP::Vector3(1.0, 0.0, 0.0));
		break;
	case FLIP_TYPE_HV:
		ResultMatrix = rotate(ResultMatrix, glm::radians(180.0f), QP::Vector3(1.0, 0.0, 0.0));
		ResultMatrix = rotate(ResultMatrix, glm::radians(180.0f), QP::Vector3(0.0, 1.0, 0.0));
		break;
	}

	LocalOpacityValue = OpacityValue;

	if (ApplyGlobalAttribute) {
		ResultMatrix = GlobalMatrix * ResultMatrix;

		switch (GlobalFlipFlag) {
		case FLIP_TYPE_H:
			ResultMatrix = rotate(ResultMatrix, glm::radians(180.0f), QP::Vector3(0.0, 1.0, 0.0));
			break;
		case FLIP_TYPE_V:
			ResultMatrix = rotate(ResultMatrix, glm::radians(180.0f), QP::Vector3(1.0, 0.0, 0.0));
			break;
		case FLIP_TYPE_HV:
			ResultMatrix = rotate(ResultMatrix, glm::radians(180.0f), QP::Vector3(1.0, 0.0, 0.0));
			ResultMatrix = rotate(ResultMatrix, glm::radians(180.0f), QP::Vector3(0.0, 1.0, 0.0));
			break;
		}

		LocalColorValue += GlobalColorValue;
		LocalOpacityValue *= GlobalOpacityValue;
		LocalBlurValue *= GlobalBlurValue;
	}
}
