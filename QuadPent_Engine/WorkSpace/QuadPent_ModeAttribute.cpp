#include "GameMode1.h"
//////////////////////////////////// Start Mode Option
// Specifies the mode to run first when the program starts.
//You must include the mode header file, and declare the mode class.

GameMode1 Mode1;

// Map modes' Start() function pointer here
void ModeAttribute() {
	Mode1.Map(ASSET::MODE.Mode1);

	// Set start mode
	QP::START_MODE = ASSET::MODE.Mode1;
}