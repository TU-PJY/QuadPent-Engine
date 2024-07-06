#pragma once
#pragma warning(disable: 4244) 
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <string>
#include <cmath>

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



// window name
constexpr const char* WindowName = "MATA_ENGINE-1";

// fullscreen option
constexpr bool StartWithFullScreen = false;

// boundbox option
constexpr bool ShowBoundBox = true;

// console window option
constexpr bool ShowConsole = true;

//vendor print option
constexpr bool PrintVendorInfoOpt = false;



// global scope shader
extern GLuint ImageShader;
extern GLuint TextShader;

void SetBackColor(GLfloat R, GLfloat G, GLfloat B);
extern glm::vec3 BackColor;


// Multiply Xposition with Aspect
inline void ASP(GLfloat& ValueX) {
	ValueX *= ASPECT;
}

// Divide Xposition with Aspect
inline void DivASP(GLfloat& ValueX) {
	ValueX /= ASPECT;
}

// Calculate distance of 2 dots
inline GLfloat GetDistance(GLfloat X1, GLfloat Y1, GLfloat X2, GLfloat Y2) {
	GLfloat DistanceX = X2 - X1;
	GLfloat DistanceY = Y2 - Y1;
	return sqrt(pow(DistanceX, 2) + pow(DistanceY, 2));
}