#pragma once
#pragma warning(disable: 4244) 
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

// display width, height and apsect ratio
extern int WIDTH, HEIGHT;
extern int PREV_WIDTH, PREV_HEIGHT;
extern GLfloat ASPECT;

// corner position of display
struct Rect {
	GLfloat lx, ly, rx, ry;
};
extern Rect rect;

// GPU vendor name and vendor print option
extern std::string GPU_Vendor;

// global scope shader
extern GLuint ImageShader;
extern GLuint TextShader;

void SetBackColor(GLfloat R, GLfloat G, GLfloat B);
extern glm::vec3 BackColor;

struct ImageInfo {
	std::string Name;
	const char* FileName;
};

struct FileNameAndOption {
	std::string Name;
	const char* FileName;
	FMOD_MODE Option;
};

extern std::vector<ImageInfo> ImageList;
extern std::vector<FileNameAndOption> SoundList;

// window name
constexpr const char* WindowName = "MATA_ENGINE-1";

// window size
constexpr int WindowWidth = 1200;
constexpr int WindowHeight = 800;

// fullscreen option
constexpr bool StartWithFullScreen = false;

// boundbox option
constexpr bool ShowBoundBox = true;

// console window option
constexpr bool ShowConsole = true;

// vendor print option
constexpr bool PrintGPU_VendorInfoOpt = false;