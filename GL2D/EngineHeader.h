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

extern float DestFPS;
extern int FPSLimit;

typedef void(*Start_Mode)(void);
using Sound = FMOD::Sound*;
using Channel = FMOD::Channel*;

enum class KeyType
{ Normal, Special };

enum class KeyState
{ Down, Up };

enum class RenderType
{ Static, Default };

enum class ShaderType
{ Image, Text };


// corner position of display
struct Rect {
	GLfloat lx, ly, rx, ry;
};
extern Rect rect;

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