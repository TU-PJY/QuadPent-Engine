#include "ImageUtil.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "SystemResource.h"
#include "EXUtil.h"
#include "Scene.h"

ImageUtil imageUtil;

GLfloat ImagePannel[][48] = {  // default size 1.0 * 1.0
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0, 0.0,
	 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0, 0.0,
	 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0, 1.0,
	 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0, 1.0,
	-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0, 1.0,
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0, 0.0
};

void ImageUtil::Init() {
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

void ImageUtil::Load(Image& ImageStruct, std::string FilePath, int Type) {
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
		scene.ErrorScreen(ERROR_TYPE_IMAGE_LOAD, FilePath);
		return;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, TextureData);
	stbi_image_free(TextureData);

	ImageStruct.Width = Width;
	ImageStruct.Height = Height;
}

void ImageUtil::LoadClip(Image& ImageStruct, std::string FilePath, int X, int Y, int ClipWidth, int ClipHeight, int Type) {
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
		scene.ErrorScreen(ERROR_TYPE_IMAGE_LOAD, FilePath);
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

void ImageUtil::SetSpriteSheetSize(int ValueClipWidth, int ValueClipHeight, int ValueNumRow, int ValueNumCol, int ValueStartCol, int ValueNumBlank) {
	ClipWidth = ValueClipWidth;
	ClipHeight = ValueClipHeight;
	NumRow = ValueNumRow;
	NumCol = ValueNumCol;
	StartLocation = ValueStartCol;
	BlankLocation = ValueNumBlank;
}

void ImageUtil::LoadSpriteSheet(SpriteSheet& SpriteSheetStruct, std::string FilePath, int Type) {
	int Width{}, Height{}, Channel{};
	unsigned char* TextureData = stbi_load(FilePath.c_str(), &Width, &Height, &Channel, 4);
	if (!TextureData) {
		scene.ErrorScreen(ERROR_TYPE_IMAGE_LOAD, FilePath);
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
	EX.ClampValue(CurrentYPosition, 0, CLAMP_LESS);

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
			EX.ClampValue(CurrentXPosition, Width, CLAMP_GREATER);

			++CurrentIndex;
		}

		CurrentYPosition -= ClipHeight;
		EX.ClampValue(CurrentYPosition, 0, CLAMP_LESS);

		CurrentXPosition = 0;
	}

	stbi_image_free(TextureData);

	SpriteSheetStruct.Width = ClipWidth;
	SpriteSheetStruct.Height = ClipHeight;
}

void ImageUtil::PreLoadSpriteSheet(SpriteSheet& SpriteSheetStruct, std::string FilePath, int Type) {
	PreLoadSpriteSheetInfo PLSS{};
	int Width{}, Height{}, Channel{};
	unsigned char* TextureData = stbi_load(FilePath.c_str(), &Width, &Height, &Channel, 4);
	if (!TextureData) {
		scene.ErrorScreen(ERROR_TYPE_IMAGE_LOAD, FilePath);
		return;
	}

	SpriteSheetStruct.Texture.assign(NumRow * NumCol, {});

	if (BlankLocation != 0)
		SpriteSheetStruct.Frame = NumRow * NumCol - (NumRow - BlankLocation);
	else
		SpriteSheetStruct.Frame = NumRow * NumCol;

	int CurrentXPosition = 0;
	int CurrentYPosition = Height - ClipHeight * StartLocation;
	EX.ClampValue(CurrentYPosition, 0, CLAMP_LESS);

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
			EX.ClampValue(CurrentXPosition, Width, CLAMP_GREATER);
		}

		CurrentYPosition -= ClipHeight;
		EX.ClampValue(CurrentYPosition, 0, CLAMP_LESS);

		CurrentXPosition = 0;
	}

	SpriteSheetStruct.Width = ClipWidth;
	SpriteSheetStruct.Height = ClipHeight;

	PLSS.SpriteSheetPtr = &SpriteSheetStruct;
	PLSS.ImageType = Type;

	LoadSpriteSheetBuffer.emplace_back(PLSS);

	stbi_image_free(TextureData);
}

void ImageUtil::PreLoad(Image& ImageStruct, std::string FilePath, int Type) {
	PreLoadInfo PLI{};
	int Width{}, Height{}, Channel{};
	unsigned char* TextureData = stbi_load(FilePath.c_str(), &Width, &Height, &Channel, 4);
	if (!TextureData) {
		scene.ErrorScreen(ERROR_TYPE_IMAGE_LOAD, FilePath);
		return;
	}

	ImageStruct.Width = Width;
	ImageStruct.Height = Height;
	PLI.ImagePtr = &ImageStruct;
	PLI.ImageType = Type;
	PLI.TextureData = TextureData;

	LoadBuffer.emplace_back(PLI);
}

void ImageUtil::PreLoadClip(Image& ImageStruct, std::string FilePath, int X, int Y, int ClipWidth, int ClipHeight, int Type) {
	PreLoadInfo PLI{};
	int Width{}, Height{}, Channel{};
	unsigned char* TextureData = stbi_load(FilePath.c_str(), &Width, &Height, &Channel, 4);
	if (!TextureData) {
		scene.ErrorScreen(ERROR_TYPE_IMAGE_LOAD, FilePath);
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

void ImageUtil::Map() {
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

void ImageUtil::Render(Image& ImageStruct) {
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, ImageStruct.Texture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void ImageUtil::RenderSheet(SpriteSheet& SpriteSheetStruct, int Frame) {
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, SpriteSheetStruct.Texture[Frame]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void ImageUtil::RenderRaw() {
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void ImageUtil::Release(Image& ImageStruct) {
	glDeleteTextures(1, &ImageStruct.Texture);
}

void ImageUtil::ReleaseSpriteSheet(SpriteSheet& SpriteSheetStruct) {
	size_t SheetSize = SpriteSheetStruct.Texture.size();
	for(int i = 0; i < SheetSize; ++i)
		glDeleteTextures(1, &SpriteSheetStruct.Texture[i]);
}