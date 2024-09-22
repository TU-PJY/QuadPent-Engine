#include "ExUtil.h"

void Switch::Bool(bool& Value) {
	if (!Value)
		Value = true;
	else
		Value = false;
}