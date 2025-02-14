#include "GameMode1.h"
//////////////////////////////////// Start Mode Option
// Specifies the mode to run first when the program starts.
//You must include the mode header file, and declare the mode class and mode's instance.

GameMode1 Mode1;
GameMode1* GameMode1::M_Inst;

// Map modes' Start() function pointer here
void ModeAttribute() {
	Mode1.Map(SDK::MODE.Mode1);

	// Set start mode
	START_MODE = SDK::MODE.Mode1;
}