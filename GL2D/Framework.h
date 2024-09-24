#pragma once
#include "GameObject.h"
#include <array>
#include <vector>
#include <ranges>
#include <algorithm>
#include <map>

typedef void(*Function)(void);
typedef void(*ControllerFunction)(void);
constexpr int Layers = static_cast<int>(Layer::END);

enum class DeleteRange
{ One, All };

enum class SearchRange
{ One, All };

enum class ModeType
{ Default, Floating };

class Framework {
private:
	std::array<std::vector<GameObject*>, Layers> ObjectVector;
	std::map<std::string, GameObject*> ObjectList;

	const char*						  CurrentRunningMode{};
	const char*						  PrevRunningMode{};

	bool							  FloatingRunningActivated{};
	bool                              FloatingFocusActivated{};

	float							  FrameTime{};

	ControllerFunction				  ControllerBuffer{};

public:
	const char* Mode();
	void InputModeName(const char* ModeName);
	void Init(Function ModeFunction);
	void SetController(ControllerFunction Controller, ModeType Type);
	void InputFrameTime(float ElapsedTime);
	void Routine();
	void SwitchMode(Function ModeFunction);
	void StartFloatingMode(Function ModeFunction, bool FloatingFocus=false);
	void EndFloatingMode();
	void ResetControlState(GameObject* Object);
	void ResetControlState(const char* Tag);
	void InputKey(const char* Tag, KeyType Key, KeyState State, unsigned char NormalKey, int SpecialKey);
	void InputMouse(const char* Tag, int button, int state, int x, int y);
	void InputScroll(const char* Tag, int button, int Wheel, int x, int y);
	void AddObject(GameObject* Object, const char* Tag, Layer AddLayer, bool SetFloatingObject=false, bool SetStaticObject =false);
	void SwapLayer(GameObject* Object, Layer TargetLayer);
	void DeleteObject(GameObject* Object);
	void DeleteObject(const char* Tag, DeleteRange deleteRange);
	GameObject* Find(const char* Tag);
	GameObject* Find(const char* Tag, Layer LayerToSearch, int Index);
	size_t Size(Layer TargetLayer);
	void Exit();

private:
	void UpdateContainer(int i);
	void ClearFloatingObject();
	void ClearAll();
};
extern Framework framework;