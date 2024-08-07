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

			UpdateContainer(i);
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
		AddObject(new FWM_DUMMY, "MATA_ENGINE_CONTAINER_DUMMY", static_cast<Layer>(i), true);

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

void Framework::ResetControlState(std::string Tag) {
	auto It = ObjectList.find(Tag);
	if (It != end(ObjectList) && !It->second->DeleteDesc)
		It->second->ResetControlState();
}

void Framework::InputKey(std::string Tag, unsigned char KEY, int S_KEY, bool KeyDown, bool SpecialKey) {
	auto It = ObjectList.find(Tag);
	if (It != end(ObjectList) && !It->second->DeleteDesc)
		It->second->InputKey(KEY, S_KEY, KeyDown, SpecialKey);
}

void Framework::InputMouse(std::string Tag, int button, int state, int x, int y) {
	auto It = ObjectList.find(Tag);
	if (It != end(ObjectList) && !It->second->DeleteDesc)
		It->second->InputMouse(button, state, x, y);
}

void Framework::InputScroll(std::string Tag, int button, int Wheel, int x, int y) {
	auto It = ObjectList.find(Tag);
	if (It != end(ObjectList) && !It->second->DeleteDesc)
		It->second->InputScroll(button, Wheel, x, y);
}

void Framework::AddObject(BASE* Object, std::string Tag, Layer AddLayer, bool SetStaticObject, bool SetFloatingObject) {
	int DestLayer = static_cast<int>(AddLayer);

	Container[DestLayer].push_back(Object);
	Object->ObjectTag = Tag;
	Object->PrevLayer = DestLayer;

	if (Tag != "MATA_ENGINE_CONTAINER_DUMMY") {
		ObjectList.insert(std::make_pair(Tag, Object));

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

void Framework::SwapLayer(BASE* Object, Layer TargetLayer) {
	int DestLayer = static_cast<int>(TargetLayer);

	if (Object->PrevLayer == DestLayer)
		return;

	Object->SwapLayerDesc = true;
	Object->DestLayer = DestLayer;
	Object->PrevLayer = DestLayer;
}

void Framework::DeleteSelf(BASE* Object) {
	Object->DeleteDesc = true;

	FLog.ObjectTag = Object->ObjectTag;
	FLog.Log(LogType::DELETE_OBJECT);
}

void Framework::DeleteObject(std::string Tag, DeleteRange deleteRange) {
	if (deleteRange == DeleteRange::One) {
		auto It = ObjectList.find(Tag);
		if (It != end(ObjectList) && !It->second->DeleteDesc) {
			It->second->DeleteDesc = true;
			FLog.ObjectTag = It->second->ObjectTag;
			FLog.Log(LogType::DELETE_OBJECT);
		}
	}

	else if (deleteRange == DeleteRange::All) {
		auto Range = ObjectList.equal_range(Tag);
		if (Range.first != Range.second) {
			for (auto It = Range.first; It != Range.second; ++It) {
				if (It->first == Tag && !It->second->DeleteDesc) {
					It->second->DeleteDesc = true;
					FLog.ObjectTag = It->second->ObjectTag;
					FLog.Log(LogType::DELETE_OBJECT);
				}
			}
		}
	}
}

BASE* Framework::Find(std::string Tag) {
	auto It = ObjectList.find(Tag);
	if (It != end(ObjectList) && !It->second->DeleteDesc)
		return It->second;

	return nullptr;
}

BASE* Framework::Find(std::string Tag, Layer LayerToSearch, int Index) {
	int layer = static_cast<int>(LayerToSearch);

	if (Index >= Container[layer].size())
		return nullptr;

	if (Container[layer][Index]->ObjectTag == Tag && !Container[layer][Index]->DeleteDesc)
		return Container[layer][Index];

	return nullptr;
}

size_t Framework::Size(Layer TargetLayer) {
	return Container[static_cast<int>(TargetLayer)].size();
}

//////// private ///////////////
void Framework::UpdateContainer(int i) {
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

		if ((*It)->SwapLayerDesc) {
			int DestLayer = (*It)->DestLayer;
			Container[DestLayer].push_back(*It);
			Container[DestLayer].back()->SwapLayerDesc = false;
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
}

void Framework::ClearAll() {
	for (int i = 0; i < Num; ++i) {
		for (auto It = begin(Container[i]); It != end(Container[i]); ++It) {
			if (!(*It)->StaticDesc)
				(*It)->DeleteDesc = true;
		}
	}
}
