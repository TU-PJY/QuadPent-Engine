#pragma once
#include <Scene.h>
#include <MouseUtil.h>
#include <CameraController.h>

extern std::vector<GameObject*>* InputObjectListPtr;
void AddControlObject(GameObject* Object);
void DeleteControlObject(GameObject* Object);