#include "QuadPent_ObjectValue.h"

// Object Value
QP::Matrix4 QP::LocalMatrix	           = QP::Matrix4(1.0);
QP::Matrix4 QP::GlobalMatrix           = QP::Matrix4(1.0);
QP::Matrix4 QP::ResultMatrix           = QP::Matrix4(1.0);

int QP::LocalFlipFlag  = FLIP_TYPE_NONE;
int QP::GlobalFlipFlag = FLIP_TYPE_NONE;

QP::Matrix4 QP::ViewportPositionMatrix = QP::Matrix4(1.0);

QP::Color3  QP::LocalColorValue;
QP::Color3  QP::GlobalColorValue;

float QP::LocalOpacityValue  = 1.0;
float QP::LocalBlurValue     = 0.0;

float QP::GlobalOpacityValue = 1.0;
float QP::GlobalBlurValue    = 0.0;