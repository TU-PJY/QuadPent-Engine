#pragma once
#include "GameObject.h"
#include <algorithm>
#include <array>
#include <map>
#include <vector>

typedef void(*Function)(void);
typedef void(*ControllerFunction)(void);
constexpr int Layers = END;

using ObjectRange = std::multimap<std::string, GameObject*>::iterator;

enum DeleteRange { 
	DELETE_RANGE_SINGLE, 
	DELETE_RANGE_EQUAL 
};

enum ModeType { 
	MODE_TYPE_NONE, 
	MODE_TYPE_DEFAULT,
	MODE_TYPE_FLOATING 
};

enum ObjectType { 
	OBJECT_TYPE_NONE, 
	OBJECT_TYPE_STATIC, 
	OBJECT_TYPE_FLOATING 
};


class Scene {
private:
	std::array<std::vector<GameObject*>, Layers> ObjectList;
	std::multimap<std::string, GameObject*> ObjectIndex;

	std::string						  CurrentRunningMode{};
	std::string						  PrevRunningMode{};

	bool							  FloatingActivateCommand{};
	bool                              FloatingFocusCommand{};

	float							  FrameTime{};

	ControllerFunction				  ControllerBuffer{};
	Function						  DestructorBuffer{};

	bool RoutineActivateCommand{ true };

public:
	std::string Mode();
	void Stop();
	void Resume();
	void RegisterModeName(std::string ModeName);
	void Init(Function ModeFunction);
	void RegisterController(ControllerFunction Controller, int Type);
	void InputFrameTime(float ElapsedTime);
	void Routine();
	void SwitchMode(Function ModeFunction);
	void RegisterDestructor(Function DestructorFunction);
	void ReleaseDestructor();
	void StartFloatingMode(Function ModeFunction, bool FloatingFocus=false);
	void EndFloatingMode();
	void ResetControlState(GameObject* Object);
	void ResetControlState(std::string Tag);
	void InputKey(std::string Tag, int State, unsigned char NormalKey, int SpecialKey);
	void InputMouse(std::string Tag, int State);
	void InputScroll(std::string Tag, int State);
	void AddObject(GameObject* Object, std::string Tag, int AddLayer, int Type1= OBJECT_TYPE_NONE, int Type2=OBJECT_TYPE_NONE);
	void SwapLayer(GameObject* Object, int TargetLayer);
	void DeleteObject(GameObject* Object);
	void DeleteObject(std::string Tag, int deleteRange);
	GameObject* Find(std::string Tag);
	GameObject* FindMulti(std::string Tag, int SearchLayer, int Index);
	std::pair<ObjectRange, ObjectRange> EqualRange(std::string Tag);
	size_t LayerSize(int TargetLayer);
	void Exit();

private:
	void ProcessListCommand(int Index);
	void ProcessIndexCommand();
	void ClearFloatingObject();
	void ClearAll();
};
extern Scene scene;