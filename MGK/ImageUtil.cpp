#include "ImageUtil.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "SystemResource.h"

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

void ImageUtil::Import(Image& ImageStruct, std::string FileName, int Type) {
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

	unsigned char* texture_data = stbi_load(FileName.c_str(), &Width, &Height, &Channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	stbi_image_free(texture_data);

	ImageStruct.Width = Width;
	ImageStruct.Height = Height;
}

void ImageUtil::PreLoad(Image& ImageStruct, std::string FileName, int Type) {
	PreLoadInfo PLI{};
	int Width{}, Height{}, Channel{};
	unsigned char* texture_data = stbi_load(FileName.c_str(), &Width, &Height, &Channel, 4);

	PLI.ImagePtr = &ImageStruct;
	PLI.ImageType = Type;
	PLI.Width = Width;
	PLI.Height = Height;
	PLI.TextureData = texture_data;

	LoadBuffer.emplace_back(PLI);
}

void ImageUtil::FinishLoad() {
	if (LoadBuffer.empty())
		return;

	for (auto& B : LoadBuffer) {
		glGenTextures(1, &B.Texture);
		glBindTexture(GL_TEXTURE_2D, B.Texture);
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

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, B.Width, B.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, B.TextureData);
		stbi_image_free(B.TextureData);

		B.ImagePtr->Width = B.Width;
		B.ImagePtr->Height = B.Height;
		B.ImagePtr->Texture = B.Texture;
	}

	LoadBuffer.clear();
}

void ImageUtil::Render(Image& ImageStruct) {
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, ImageStruct.Texture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void ImageUtil::RenderRaw() {
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void ImageUtil::Release(Image& ImageStruct) {
	glDeleteTextures(1, &ImageStruct.Texture);
}