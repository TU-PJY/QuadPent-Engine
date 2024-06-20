#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <string>

// display width, height and apsect ratio
extern int WIDTH, HEIGHT;
extern GLfloat ASPECT;

// corner position of display
struct Rect {
	GLfloat lx, ly, rx, ry;
};
extern Rect rect;

// GPU vendor name and vendor print option
extern std::string VENDOR;
constexpr bool PrintVendorInfoOpt = false;

// fullscreen option
constexpr bool StartWithFullScreen = false;

// boundbox option
constexpr bool ShowBoundBox = true;

// shader
extern GLuint ImageShader;
extern GLuint TextShader;