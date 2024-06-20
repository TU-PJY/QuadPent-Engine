// FWL Version 3
#pragma once
#include "FWL_Message.h"
#include "ObjectBase.h"
#include <string>
#include <array>
#include <deque>
#include <ranges>
#include <algorithm>

typedef std::string(*Function)(void);
constexpr int Num = static_cast<int>(Layer::END);

enum class DeleteRange
{One, All};

enum class SearchRange
{One, All};



class FWL {
private:
	std::array<std::deque<OBJ*>, Num> Container;

	std::string						  RunningMode{};

	bool							  RunningState{};
	bool							  ReserveState{};
	bool							  ModeSwitchState{};
	bool							  SubRunningState{};

	float							  FrameTime{};

	Function						  Buffer{};

public:
	FWL();
	void SetFrameTime(float ElapsedTime); 
	std::string Mode(); 
	void SwitchToSubRunningState(); 
	void SwitchToDefaultRunningState(); 
	void Init(Function ModeFunction); 
	void Routine(); 
	void SwitchMode(Function ModeFunction, bool PauseOption = false); 
	void AddObject(OBJ* Object, std::string Tag, Layer AddLayer, bool AddAsSubObject = false); 
	void DeleteSelf(OBJ* Object);
	void DeleteObject(std::string Tag, DeleteRange deleteRange, SearchRange searchRange, Layer LayerToSearch); 
	OBJ* Find(std::string Tag, SearchRange searchRange, Layer LayerToSearch = Layer::L1); 
	OBJ* Find(std::string Tag, Layer LayerToSearch, int Index); 
	size_t Size(Layer TargetLayer);

private:
	bool CheckDeleteFlag(std::deque<OBJ*>::iterator& It, int Layer); 
	void ChangeMode(); 
	void ClearAll();
};