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
	if (!ModeSwitchReserveDescriptor && RunningActivateDescriptor) {
		for (int i = 0; i < Num; ++i) {
			for (auto It = begin(Container[i]); It != end(Container[i]);) {
				if (!(*It)->DeleteDescriptor) {
					if (!FloatingModeRunningDescriptor) {
						(*It)->Update(FrameTime);
						(*It)->Render();
					}

					else {
						if (FloatingOnlyDescriptor && (*It)->FloatingDescriptor)
							(*It)->Update(FrameTime);
						else
							(*It)->Update(FrameTime);

						(*It)->Render();
					}
				}

				if (ModeSwitchReserveDescriptor)
					break;

				++It;
			}

			ClearDeleteTargetObject(i);

			if (ModeSwitchReserveDescriptor) {
				ModeSwitchingDescriptor = true;
				break;
			}

		}
	}

	else
		ChangeMode();
}

void Framework::Init(Function ModeFunction, ControllerFunction Controller) {
	if (RunningActivateDescriptor)
		return;

	RunningMode = ModeFunction();

	if (Controller)  Controller();

	ControllerBackUpBuffer = Controller;

	FLog.CurrentMode = RunningMode;
	FLog.Log(LogType::FM_INIT);

	for (int i = 0; i < Num; ++i)
		AddObject(new FWM_DUMMY, "FWM_DUMMY", static_cast<Layer>(i));

	RunningActivateDescriptor = true;
}

void Framework::SwitchMode(Function ModeFunction, ControllerFunction Controller) {
	if (!RunningActivateDescriptor)
		return;

	ModeFunctionBuffer = ModeFunction;
	ControllerBuffer = Controller;
	ControllerBackUpBuffer = Controller;

	FLog.PrevMode = RunningMode;

	ModeSwitchReserveDescriptor = true;
}

void Framework::StartFloatingMode(Function ModeFunction, ControllerFunction Controller, bool FloatingOnlyOption) {
	if (!RunningActivateDescriptor || FloatingModeRunningDescriptor)
		return;

	PrevRunningMode = RunningMode;
	FLog.PrevMode = RunningMode;

	RunningMode = ModeFunction();
	Controller();

	FloatingOnlyDescriptor = FloatingOnlyOption;
	FLog.IsOnlyFloating = FloatingOnlyDescriptor;

	FLog.CurrentMode = RunningMode;
	if (FLog.CurrentMode == FLog.PrevMode)
		FLog.ErrorLog(LogType::ERROR_SAME_MODE);

	FLog.Log(LogType::START_FLOATING_MODE);
	FLog.Log(LogType::MODE_SWITCH);

	FloatingModeRunningDescriptor = true;
}

void Framework::EndFloatingMode() {
	if (!RunningActivateDescriptor || !FloatingModeRunningDescriptor)
		return;

	FLog.Log(LogType::END_FLOATING_MODE);

	FLog.PrevMode = RunningMode;

	FloatingModeEndReserveDescriptor = true;
	ModeSwitchReserveDescriptor = true;
}

void Framework::ResetControlState(OBJ_BASE* Object) {
	Object->ResetControlState();
}

void Framework::AddObject(OBJ_BASE* Object, std::string Tag, Layer AddLayer, bool SetFloatingObject) {
	Container[static_cast<int>(AddLayer)].push_back(Object);
	Object->ObjectTag = Tag;

	ObjectList.insert(std::make_pair(Tag, Object));

	FLog.ObjectTag = Tag;
	FLog.Log(LogType::ADD_OBJECT);

	if (SetFloatingObject) {
		Object->FloatingDescriptor = true;
		FLog.Log(LogType::SET_FLOATING_OBJECT);
	}
}

void Framework::DeleteSelf(OBJ_BASE* Object) {
	Object->DeleteDescriptor = true;

	std::erase_if(ObjectList, [](const std::pair<std::string, OBJ_BASE*>& Object) {
		return Object.second->DeleteDescriptor;
		});

	FLog.ObjectTag = Object->ObjectTag;
	FLog.Log(LogType::DELETE_OBJECT);
}

void Framework::DeleteObject(std::string Tag, DeleteRange deleteRange) {
	if (deleteRange == DeleteRange::One) {
		auto It = ObjectList.lower_bound(Tag);
		if (It != end(ObjectList))
			It->second->DeleteDescriptor = true;
	}

	else if (deleteRange == DeleteRange::All) {
		for (auto It = begin(ObjectList); It != end(ObjectList); ++It) {
			if (It->first == Tag)
				It->second->DeleteDescriptor = true;
		}
	}

	std::erase_if(ObjectList, [](const std::pair<std::string, OBJ_BASE*>& Object) {
		return Object.second->DeleteDescriptor;
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
	if (FloatingModeEndReserveDescriptor) {
		ClearFloatingObject();
		RunningMode = PrevRunningMode;

		if (ControllerBackUpBuffer)
			ControllerBackUpBuffer();

		FloatingModeRunningDescriptor = false;
		FloatingOnlyDescriptor = false;

		FLog.CurrentMode = RunningMode;
		FLog.Log(LogType::END_FLOATING_MODE);
	}

	else {
		ClearAll();
		RunningMode = ModeFunctionBuffer();

		if (ControllerBuffer)
			ControllerBuffer();

		FLog.CurrentMode = RunningMode;
		FloatingOnlyDescriptor = false;
	}

	FLog.IsOnlyFloating = FloatingOnlyDescriptor;

	if (FLog.CurrentMode == FLog.PrevMode)
		FLog.ErrorLog(LogType::ERROR_SAME_MODE);

	FLog.CurrentMode = RunningMode;
	FLog.Log(LogType::MODE_SWITCH);

	FloatingModeEndReserveDescriptor = false;
	ModeSwitchReserveDescriptor = false;
	ModeSwitchingDescriptor = false;
}

void Framework::ClearDeleteTargetObject(int i) {
	std::erase_if(ObjectList, [](const std::pair<std::string, OBJ_BASE*>& Object) {
		return Object.second->DeleteDescriptor;
		});

	for (auto It = begin(Container[i]); It != end(Container[i]);) {
		if ((*It)->DeleteDescriptor) {
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
			if ((*It)->FloatingDescriptor)
				(*It)->DeleteDescriptor = true;
		}
	}

	std::erase_if(ObjectList, [](const std::pair<std::string, OBJ_BASE*>& Object) {
		return Object.second->DeleteDescriptor;
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