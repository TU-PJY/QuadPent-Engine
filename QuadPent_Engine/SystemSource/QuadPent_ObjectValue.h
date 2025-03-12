#pragma once
#include "QuadPent_Header.h"

namespace QP {
	extern QP::Matrix4 LocalMatrix;
	extern QP::Matrix4 GlobalMatrix;
	extern QP::Matrix4 ResultMatrix;
	extern int LocalFlipFlag;
	extern int GlobalFlipFlag;

	extern QP::Matrix4 ViewportPositionMatrix;

	extern QP::Color3 LocalColorValue, GlobalColorValue;
	extern float LocalOpacityValue, GlobalOpacityValue;
	extern float LocalBlurValue, GlobalBlurValue;
}