#include "Framework.h"

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
	return CurrentRunningMode;
}

void Framework::Routine() {
	if (RoutineRunningDesc) {
		for (int i = 0; i < Num; ++i) {
			for (auto It = begin(Container[i]); It != end(Container[i]); ++It) {
				if (!(*It)->DeleteDesc) {
					if (FloatingRunningDesc && FloatingFocusDesc && (*It)->FloatingObjectDesc)
						(*It)->Update(FrameTime);
					else
						(*It)->Update(FrameTime);
					(*It)->Render();
				}
			}

			ClearDelObjects(i);
		}
	}
}

void Framework::Init(Function ModeFunction, ControllerFunction Controller) {
	if (RoutineRunningDesc)
		return;

	CurrentRunningMode = ModeFunction();

	if (Controller)  
		Controller();

	ControllerBuffer = Controller;

	FLog.CurrentMode = CurrentRunningMode;
	FLog.Log(LogType::FM_INIT);

	for (int i = 0; i < Num; ++i)
		AddObject(new FWM_DUMMY, "DUMMY", static_cast<Layer>(i), true);

	RoutineRunningDesc = true;
}

void Framework::SwitchMode(Function ModeFunction, ControllerFunction Controller) {
	if (!RoutineRunningDesc)
		return;

	FLog.PrevMode = CurrentRunningMode;

	ClearAll();
	CurrentRunningMode = ModeFunction();

	if (Controller) {
		Controller();
		ControllerBuffer = Controller;
	}

	FLog.CurrentMode = CurrentRunningMode;

	FLog.IsOnlyFloating = FloatingFocusDesc;
	FLog.CurrentMode = CurrentRunningMode;
	if (FLog.CurrentMode == FLog.PrevMode)
		FLog.ErrorLog(LogType::ERROR_SAME_MODE);

	if (FloatingRunningDesc) {
		FLog.Log(LogType::END_FLOATING_MODE);
		FloatingRunningDesc = false;
		FloatingFocusDesc = false;
	}

	FLog.Log(LogType::MODE_SWITCH);
}

void Framework::StartFloatingMode(Function ModeFunction, ControllerFunction Controller, bool FloatingFocus) {
	if (!RoutineRunningDesc || FloatingRunningDesc)
		return;

	PrevRunningMode = CurrentRunningMode;
	FLog.PrevMode = CurrentRunningMode;

	CurrentRunningMode = ModeFunction();

	if(Controller)  
		Controller();

	FloatingFocusDesc = FloatingFocus;
	FLog.IsOnlyFloating = FloatingFocusDesc;

	FLog.CurrentMode = CurrentRunningMode;
	if (FLog.CurrentMode == FLog.PrevMode)
		FLog.ErrorLog(LogType::ERROR_SAME_MODE);

	FLog.Log(LogType::START_FLOATING_MODE);
	FLog.Log(LogType::MODE_SWITCH);

	FloatingRunningDesc = true;
}

void Framework::EndFloatingMode() {
	if (!RoutineRunningDesc || !FloatingRunningDesc)  
		return;

	FLog.PrevMode = CurrentRunningMode;

	ClearFloatingObject();
	CurrentRunningMode = PrevRunningMode;

	if (ControllerBuffer)  
		ControllerBuffer();

	FloatingRunningDesc = false;
	FloatingFocusDesc = false;

	FLog.IsOnlyFloating = FloatingFocusDesc;
	FLog.CurrentMode = CurrentRunningMode;
	if (FLog.CurrentMode == FLog.PrevMode)
		FLog.ErrorLog(LogType::ERROR_SAME_MODE);

	FLog.Log(LogType::END_FLOATING_MODE);
	FLog.Log(LogType::MODE_SWITCH);
}

void Framework::ResetControlState(BASE* Object) {
	Object->ResetControlState();
}

void Framework::AddObject(BASE* Object, std::string Tag, Layer AddLayer, bool SetStaticObject, bool SetFloatingObject) {
	Container[static_cast<int>(AddLayer)].push_back(Object);
	Object->ObjectTag = Tag;

	ObjectList.insert(std::make_pair(Tag, Object));

	if (Tag != "DUMMY") {
		FLog.ObjectTag = Tag;
		FLog.Log(LogType::ADD_OBJECT);

		if (SetFloatingObject) {
			Object->FloatingObjectDesc = true;
			FLog.Log(LogType::SET_FLOATING_OBJECT);
		}

		if (SetStaticObject) {
			Object->StaticDesc = true;
			FLog.Log(LogType::SET_STATIC_OBJECT);
		}
	}
}

void Framework::DeleteSelf(BASE* Object) {
	Object->DeleteDesc = true;

	std::erase_if(ObjectList, [](const std::pair<std::string, BASE*>& Object) {
		return Object.second->DeleteDesc;
		});

	FLog.ObjectTag = Object->ObjectTag;
	FLog.Log(LogType::DELETE_OBJECT);
}

void Framework::DeleteObject(std::string Tag, DeleteRange deleteRange) {
	if (deleteRange == DeleteRange::One) {
		auto It = ObjectList.find(Tag);
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

	std::erase_if(ObjectList, [](const std::pair<std::string, BASE*>& Object) {
		return Object.second->DeleteDesc;
		});
}

BASE* Framework::Find(std::string Tag) {
	auto It = ObjectList.find(Tag);
	if (It != end(ObjectList))
		return It->second;

	return nullptr;
}

BASE* Framework::Find(std::string Tag, Layer LayerToSearch, int Index) {
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
void Framework::ClearDelObjects(int i) {
	std::erase_if(ObjectList, [](const std::pair<std::string, BASE*>& Object) {
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
			if ((*It)->FloatingObjectDesc && !(*It)->StaticDesc)
				(*It)->DeleteDesc = true;
		}
	}

	std::erase_if(ObjectList, [](const std::pair<std::string, BASE*>& Object) {
		return Object.second->DeleteDesc;
		});
}

void Framework::ClearAll() {
	for (int i = 0; i < Num; ++i) {
		for (auto It = begin(Container[i]); It != end(Container[i]); ++It) {
			if (!(*It)->StaticDesc)
				(*It)->DeleteDesc = true;
		}
	}

	std::erase_if(ObjectList, [](const std::pair<std::string, BASE*>& Object) {
		return Object.second->DeleteDesc;
		});
}
