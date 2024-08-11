#pragma once
#include "Framework_Log.h"
#include "Base.h"
#include <array>
#include <deque>
#include <ranges>
#include <algorithm>
#include <map>

typedef std::string(*Function)(void);
typedef void(*ControllerFunction)(void);
constexpr int Num = static_cast<int>(Layer::END);

enum class DeleteRange
{ One, All };

enum class SearchRange
{ One, All };

enum class ModeType
{ Default, Floating };

class Framework {
private:
	std::array<std::deque<BASE*>, Num> Container;
	std::map<std::string, BASE*> ObjectList;

	std::string						  CurrentRunningMode{};
	std::string                       PrevRunningMode{};

	bool							  RoutineRunningDesc{};
	bool							  FloatingRunningDesc{};
	bool                              FloatingFocusDesc{};

	float							  FrameTime{};

	ControllerFunction				  ControllerBuffer{};

public:
	Framework();
	std::string Mode();
	void Init(Function ModeFunction);
	void SetController(ControllerFunction Controller, ModeType Type);
	void SetFrameTime(float ElapsedTime);
	void Routine();
	void SwitchMode(Function ModeFunction);
	void StartFloatingMode(Function ModeFunction, bool FloatingFocus=false);
	void EndFloatingMode();
	void ResetControlState(BASE* Object);
	void ResetControlState(std::string Tag);
	void InputKey(std::string Tag, KeyType Key, KeyState State, unsigned char NormalKey, int SpecialKey=NULL);
	void InputMouse(std::string Tag, int button, int state, int x, int y);
	void InputScroll(std::string Tag, int button, int Wheel, int x, int y);
	void AddObject(BASE* Object, std::string Tag, Layer AddLayer, bool SetStaticObject=false, bool SetFloatingObject=false);
	void SwapLayer(BASE* Object, Layer TargetLayer);
	void DeleteSelf(BASE* Object);
	void DeleteObject(std::string Tag, DeleteRange deleteRange);
	BASE* Find(std::string Tag);
	BASE* Find(std::string Tag, Layer LayerToSearch, int Index);
	size_t Size(Layer TargetLayer);
	void Exit();

private:
	void UpdateContainer(int i);
	void ClearFloatingObject();
	void ClearAll();
};
extern Framework fw;