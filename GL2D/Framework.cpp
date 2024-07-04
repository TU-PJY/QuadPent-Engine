#include "Framework.h"
#include <ranges>
#include <algorithm>


FWM_Log FLog;

Framework::Framework() {
	if (DebugMessage) {
		std::cout << "======== Framework Message ========\n";
		std::cout << "Framework is prepared.\n\n";
	}
}

void Framework::SetFrameTime(float ElapsedTime) {
	FrameTime = ElapsedTime;
}

std::string Framework::Mode() {
	return RunningMode;
}

void Framework::Routine() {
	if (!ModeSwitchReserveDesc && RunningActivateDesc) {
		for (int i = 0; i < Num; ++i) {
			for (auto It = begin(Container[i]); It != end(Container[i]);) {
				if (!(*It)->DeleteDesc) {
					if (FloatingModeRunningDesc && FloatingOnlyDesc) {
						if((*It)->FloatingObjectDesc)
							(*It)->Update(FrameTime);
					}

					else
						(*It)->Update(FrameTime);
					(*It)->Render();
				}

				if (ModeSwitchReserveDesc)
					break;

				++It;
			}

			ClearDeleteTargetObject(i);

			if (ModeSwitchReserveDesc) {
				ModeSwitchingDesc = true;
				break;
			}

		}
	}

	else
		ChangeMode();
}

void Framework::Init(Function ModeFunction, ControllerFunction Controller) {
	if (RunningActivateDesc)
		return;

	RunningMode = ModeFunction();

	if (Controller)  Controller();

	ControllerBackUpBuffer = Controller;

	FLog.CurrentMode = RunningMode;
	FLog.Log(LogType::FM_INIT);

	for (int i = 0; i < Num; ++i)
		AddObject(new FWM_DUMMY, "FWM_DUMMY", static_cast<Layer>(i));

	RunningActivateDesc = true;
}

void Framework::SwitchMode(Function ModeFunction, ControllerFunction Controller) {
	if (!RunningActivateDesc)
		return;

	ModeFunctionBuffer = ModeFunction;
	ControllerBuffer = Controller;
	ControllerBackUpBuffer = Controller;

	FLog.PrevMode = RunningMode;

	ModeSwitchReserveDesc = true;
}

void Framework::StartFloatingMode(Function ModeFunction, ControllerFunction Controller, bool FloatingOnlyOption) {
	if (!RunningActivateDesc || FloatingModeRunningDesc)
		return;

	PrevRunningMode = RunningMode;
	FLog.PrevMode = RunningMode;

	RunningMode = ModeFunction();
	Controller();

	FloatingOnlyDesc = FloatingOnlyOption;
	FLog.IsOnlyFloating = FloatingOnlyDesc;

	FLog.CurrentMode = RunningMode;
	if (FLog.CurrentMode == FLog.PrevMode)
		FLog.ErrorLog(LogType::ERROR_SAME_MODE);

	FLog.Log(LogType::START_FLOATING_MODE);
	FLog.Log(LogType::MODE_SWITCH);

	FloatingModeRunningDesc = true;
}

void Framework::EndFloatingMode() {
	if (!RunningActivateDesc || !FloatingModeRunningDesc)
		return;

	FLog.PrevMode = RunningMode;

	ClearFloatingObject();
	RunningMode = PrevRunningMode;

	if (ControllerBackUpBuffer)
		ControllerBackUpBuffer();

	FloatingModeRunningDesc = false;
	FloatingOnlyDesc = false;

	FLog.Log(LogType::END_FLOATING_MODE);

	FLog.IsOnlyFloating = FloatingOnlyDesc;
	FLog.CurrentMode = RunningMode;
	if (FLog.CurrentMode == FLog.PrevMode)
		FLog.ErrorLog(LogType::ERROR_SAME_MODE);
	FLog.Log(LogType::MODE_SWITCH);
}

void Framework::ResetControlState(OBJ_BASE* Object) {
	Object->ResetControlState();
}

void Framework::AddObject(OBJ_BASE* Object, std::string Tag, Layer AddLayer, bool SetFloatingObject) {
	Container[static_cast<int>(AddLayer)].push_back(Object);
	Object->ObjectTag = Tag;

	ObjectList.insert(std::make_pair(Tag, Object));

	if (Tag != "FWM_DUMMY") {
		FLog.ObjectTag = Tag;
		FLog.Log(LogType::ADD_OBJECT);
	}

	if (SetFloatingObject) {
		Object->FloatingObjectDesc = true;
		FLog.Log(LogType::SET_FLOATING_OBJECT);
	}
}

void Framework::DeleteSelf(OBJ_BASE* Object) {
	Object->DeleteDesc = true;

	std::erase_if(ObjectList, [](const std::pair<std::string, OBJ_BASE*>& Object) {
		return Object.second->DeleteDesc;
		});

	FLog.ObjectTag = Object->ObjectTag;
	FLog.Log(LogType::DELETE_OBJECT);
}

void Framework::DeleteObject(std::string Tag, DeleteRange deleteRange) {
	if (deleteRange == DeleteRange::One) {
		auto It = ObjectList.lower_bound(Tag);
		if (It != end(ObjectList)) {
			It->second->DeleteDesc = true;
			FLog.ObjectTag = It->second->ObjectTag;
			FLog.Log(LogType::DELETE_OBJECT);
		}
	}

	else if (deleteRange == DeleteRange::All) {
		for (auto It = begin(ObjectList); It != end(ObjectList); ++It) {
			if (It->first == Tag) {
				It->second->DeleteDesc = true;
				FLog.ObjectTag = It->second->ObjectTag;
				FLog.Log(LogType::DELETE_OBJECT);
			}
		}
	}

	std::erase_if(ObjectList, [](const std::pair<std::string, OBJ_BASE*>& Object) {
		return Object.second->DeleteDesc;
		});
}

OBJ_BASE* Framework::Find(std::string Tag) {
	auto It = ObjectList.lower_bound(Tag);
	if (It != end(ObjectList))
		return It->second;

	return nullptr;
}

OBJ_BASE* Framework::Find(std::string Tag, Layer LayerToSearch, int Index) {
	int layer = static_cast<int>(LayerToSearch);

	if (Index >= Container[layer].size())
		return nullptr;

	if (Container[layer][Index]->ObjectTag == Tag)
		return Container[layer][Index];

	return nullptr;
}

size_t Framework::Size(Layer TargetLayer) {
	return Container[static_cast<int>(TargetLayer)].size();
}

//////// private ///////////////

void Framework::ChangeMode() {
	FLog.PrevMode = RunningMode;

	ClearAll();
	RunningMode = ModeFunctionBuffer();

	if (ControllerBuffer)
		ControllerBuffer();

	FLog.CurrentMode = RunningMode;
	FloatingOnlyDesc = false;
	if (FloatingModeRunningDesc) {
		FLog.Log(LogType::END_FLOATING_MODE);
		FloatingModeRunningDesc = false;
	}

	FLog.IsOnlyFloating = FloatingOnlyDesc;
	FLog.CurrentMode = RunningMode;
	if (FLog.CurrentMode == FLog.PrevMode)
		FLog.ErrorLog(LogType::ERROR_SAME_MODE);
	FLog.Log(LogType::MODE_SWITCH);

	ModeSwitchReserveDesc = false;
	ModeSwitchingDesc = false;
}

void Framework::ClearDeleteTargetObject(int i) {
	std::erase_if(ObjectList, [](const std::pair<std::string, OBJ_BASE*>& Object) {
		return Object.second->DeleteDesc;
		});

	for (auto It = begin(Container[i]); It != end(Container[i]);) {
		if ((*It)->DeleteDesc) {
			delete* It;
			*It = nullptr;
			It = Container[i].erase(It);
			continue;
		}
		++It;
	}
}

void Framework::ClearFloatingObject() {
	for (int i = 0; i < Num; ++i) {
		for (auto It = begin(Container[i]); It != end(Container[i]); ++It) {
			if ((*It)->FloatingObjectDesc)
				(*It)->DeleteDesc = true;
		}
	}

	std::erase_if(ObjectList, [](const std::pair<std::string, OBJ_BASE*>& Object) {
		return Object.second->DeleteDesc;
		});
}

void Framework::ClearAll() {
	for (int i = 0; i < Num; ++i) {
		for (auto It = begin(Container[i]); It != end(Container[i]);) {
			if ((*It)->ObjectTag != "FWM_DUMMY") {
				delete* It;
				*It = nullptr;
				It = Container[i].erase(It);
				continue;
			}
			++It;
		}
	}

	ObjectList.clear();
}