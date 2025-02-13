#include "GameMode1.h"

START_MODE_PTR START_MODE;
//////////////////////////////////// Start Mode Option
// Specifies the mode to run first when the program starts.
//You must include the mode header file, and declare the mode class and mode's instance.

GameMode1 Mode1;
GameMode1* GameMode1::M_Inst;

void ModeAttribute() {
	// Map modes' Start() function pointer here
	Mode1.Map(ModePtr.Mode1);

	// Set start mode
	START_MODE = ModePtr.Mode1;
}