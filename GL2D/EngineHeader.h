#pragma once
#pragma warning(disable: 4244) 
#include "Setting.h"
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include "fmod.hpp"
#include "fmod_errors.h"
#include <string>
#include <vector>

enum class KeyType
{ NormalKey, SpecialKey };

enum class KeyState
{ Down, Up };

enum class RenderType
{ Static, Default };

enum class ShaderType
{ Image, Text };

typedef std::string(*Start_Mode)(void);
using Sound = FMOD::Sound*;
using Channel = FMOD::Channel*;
using Image = unsigned int;

// display width, height and apsect ratio
extern int WIDTH, HEIGHT;
extern int PREV_WIDTH, PREV_HEIGHT;
extern GLfloat ASPECT;

// corner position of display
struct Rect {
	GLfloat lx, ly, rx, ry;
};
extern Rect rect;

// global scope shader
extern GLuint ImageShader;
extern GLuint TextShader;

void SetBackColor(GLfloat R, GLfloat G, GLfloat B);
void SetBackColorRGB(int R, int G, int B);
void SetupWindow();
void SetupSystem(int argc, char** argv);
void LoadShader();
void InitSystem();
void SetGlOption();
void LoadImageResources();
void LoadSoundResources();
void LoadSystemResources();
GLvoid GLMain();
GLvoid DisplayReshape(int w, int h);

extern glm::vec3 BackColor;
extern Start_Mode StartMode;