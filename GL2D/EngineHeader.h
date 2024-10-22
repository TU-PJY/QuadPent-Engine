#pragma once
#pragma warning(disable: 4244) 
#include "Setting.h"
#include "glew.h"
#include "freeglut.h"
#include "freeglut_ext.h"
#include "glm/ext.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "fmod.hpp"
#include "fmod_errors.h"
#include <iostream>

extern float DestFPS;
extern int FPSLimit;

typedef void(*Start_Mode)(void);
using Sound = FMOD::Sound*;
using SoundChannel = FMOD::Channel*;


enum KeyType
{ KEY_TYPE_NORMAL, KEY_TYPE_SPECIAL };

enum KeyState
{ KEY_DOWN, KEY_UP };

enum RenderType
{ RENDER_TYPE_DEFAULT, RENDER_TYPE_STATIC };

enum ShaderType
{ SHADER_TYPE_IMAGE, SHADER_TYPE_TEXT };


// corner position of display
struct ViewportRect {
	GLfloat lx, ly, rx, ry;
};
extern ViewportRect WindowRect;

// image struct
typedef struct {
	unsigned int Texture;
	int Width, Height;
}Image;

// display width, height and apsect ratio
extern int WIDTH, HEIGHT;
extern int PREV_WIDTH, PREV_HEIGHT;
extern GLfloat ASPECT;

extern glm::vec3 BackColor;
extern Start_Mode StartMode;

// global scope shader
extern GLuint ImageShader;
extern GLuint TextShader;

void SetFrameLimit(int FrameLimit);
void SetBackColor(GLfloat R, GLfloat G, GLfloat B);
void SetBackColorRGB(int R, int G, int B);
void SetupWindow();
void SetupSystem(int argc, char** argv);
void LoadShader();
void InitSystem();
void SetGlOption();
GLvoid GLMain();
GLvoid DisplayReshape(int w, int h);