#include "Framework.h"

FWM_Log FLog;

Framework::Framework() {
	if (SHOW_FRAMEWORK_MESSAGE) {
		std::cout << "======== Framework Message ========\n";
		std::cout << "Framework is prepared.\n\n";
	}
}

void Framework::InputFrameTime(float ElapsedTime) {
	FrameTime = ElapsedTime;
}

std::string Framework::Mode() {
	return CurrentRunningMode;
}

void Framework::Routine() {
	if (RoutineRunningActivated) {
		for (int i = 0; i < Layers; ++i) {
			for (auto It = begin(ObjectDeque[i]); It != end(ObjectDeque[i]); ++It) {
				if (!(*It)->DeleteObjectMarked) {
					if (FloatingRunningActivated && FloatingFocusActivated && (*It)->FloatingObjectMarked)
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

void Framework::Init(Function ModeFunction) {
	if (RoutineRunningActivated)
		return;

	CurrentRunningMode = ModeFunction();

	FLog.CurrentMode = CurrentRunningMode;
	FLog.Log(LogType::FM_INIT);

	for (int i = 0; i < Layers; ++i)
		AddObject(new FWM_DUMMY, "MATA_ENGINE_CONTAINER_DUMMY", static_cast<Layer>(i), true);

	RoutineRunningActivated = true;
}

void Framework::SwitchMode(Function ModeFunction) {
	if (!RoutineRunningActivated)
		return;

	FLog.PrevMode = CurrentRunningMode;

	ClearAll();

	CurrentRunningMode = ModeFunction();
	FLog.CurrentMode = CurrentRunningMode;

	if (FLog.CurrentMode == FLog.PrevMode)
		FLog.ErrorLog(LogType::ERROR_SAME_MODE);

	if (FloatingRunningActivated) {
		FLog.Log(LogType::END_FLOATING_MODE);
		FloatingRunningActivated = false;
		FloatingFocusActivated = false;
		FLog.IsOnlyFloating = FloatingFocusActivated;
	}

	FLog.Log(LogType::MODE_SWITCH);
}

void Framework::StartFloatingMode(Function ModeFunction, bool FloatingFocus) {
	if (!RoutineRunningActivated || FloatingRunningActivated)
		return;

	PrevRunningMode = CurrentRunningMode;
	FLog.PrevMode = CurrentRunningMode;

	CurrentRunningMode = ModeFunction();

	FloatingFocusActivated = FloatingFocus;
	FLog.IsOnlyFloating = FloatingFocusActivated;

	FLog.CurrentMode = CurrentRunningMode;
	if (FLog.CurrentMode == FLog.PrevMode)
		FLog.ErrorLog(LogType::ERROR_SAME_MODE);

	FLog.Log(LogType::START_FLOATING_MODE);
	FLog.Log(LogType::MODE_SWITCH);

	FloatingRunningActivated = true;
}

void Framework::EndFloatingMode() {
	if (!RoutineRunningActivated || !FloatingRunningActivated)  
		return;

	FLog.PrevMode = CurrentRunningMode;

	ClearFloatingObject();
	CurrentRunningMode = PrevRunningMode;

	if (ControllerBuffer)  
		ControllerBuffer();

	FloatingRunningActivated = false;
	FloatingFocusActivated = false;

	FLog.IsOnlyFloating = FloatingFocusActivated;
	FLog.CurrentMode = CurrentRunningMode;
	if (FLog.CurrentMode == FLog.PrevMode)
		FLog.ErrorLog(LogType::ERROR_SAME_MODE);

	FLog.Log(LogType::END_FLOATING_MODE);
	FLog.Log(LogType::MODE_SWITCH);
}

void Framework::ResetControlState(GameObject* Object) {
	Object->ResetControlState();
}

void Framework::ResetControlState(std::string Tag) {
	auto It = ObjectList.find(Tag);
	if (It != end(ObjectList) && !It->second->DeleteObjectMarked)
		It->second->ResetControlState();
}

void Framework::InputKey(std::string Tag, KeyType Key, KeyState State, unsigned char NormalKey, int SpecialKey) {
	auto It = ObjectList.find(Tag);
	if (It != end(ObjectList) && !It->second->DeleteObjectMarked)
		It->second->InputKey(Key, State, NormalKey, SpecialKey);
}

void Framework::InputMouse(std::string Tag, int button, int state, int x, int y) {
	auto It = ObjectList.find(Tag);
	if (It != end(ObjectList) && !It->second->DeleteObjectMarked)
		It->second->InputMouse(button, state, x, y);
}

void Framework::InputScroll(std::string Tag, int button, int Wheel, int x, int y) {
	auto It = ObjectList.find(Tag);
	if (It != end(ObjectList) && !It->second->DeleteObjectMarked)
		It->second->InputScroll(button, Wheel, x, y);
}

void Framework::AddObject(GameObject* Object, std::string Tag, Layer AddLayer, bool SetStaticObject, bool SetFloatingObject) {
	int DestLayer = static_cast<int>(AddLayer);

	ObjectDeque[DestLayer].push_back(Object);
	Object->ObjectTag = Tag;
	Object->PrevLayer = DestLayer;

	if (Tag != "MATA_ENGINE_CONTAINER_DUMMY") {
		ObjectList.insert(std::make_pair(Tag, Object));

		FLog.ObjectTag = Tag;
		FLog.Log(LogType::ADD_OBJECT);

		if (SetFloatingObject) {
			Object->FloatingObjectMarked = true;
			FLog.Log(LogType::SET_FLOATING_OBJECT);
		}

		if (SetStaticObject) {
			Object->StaticObjectMarked = true;
			FLog.Log(LogType::SET_STATIC_OBJECT);
		}
	}
}

void Framework::SwapLayer(GameObject* Object, Layer TargetLayer) {
	int DestLayer = static_cast<int>(TargetLayer);

	if (Object->PrevLayer == DestLayer)
		return;

	Object->SwapLayerMarked = true;
	Object->DestLayer = DestLayer;
	Object->PrevLayer = DestLayer;
}

void Framework::DeleteSelf(GameObject* Object) {
	Object->DeleteObjectMarked = true;

	FLog.ObjectTag = Object->ObjectTag;
	FLog.Log(LogType::DELETE_OBJECT);
}

void Framework::DeleteObject(std::string Tag, DeleteRange deleteRange) {
	if (deleteRange == DeleteRange::One) {
		auto It = ObjectList.find(Tag);
		if (It != end(ObjectList) && !It->second->DeleteObjectMarked) {
			It->second->DeleteObjectMarked = true;
			FLog.ObjectTag = It->second->ObjectTag;
			FLog.Log(LogType::DELETE_OBJECT);
		}
	}

	else if (deleteRange == DeleteRange::All) {
		auto Range = ObjectList.equal_range(Tag);
		if (Range.first != Range.second) {
			for (auto It = Range.first; It != Range.second; ++It) {
				if (It->first == Tag && !It->second->DeleteObjectMarked) {
					It->second->DeleteObjectMarked = true;
					FLog.ObjectTag = It->second->ObjectTag;
					FLog.Log(LogType::DELETE_OBJECT);
				}
			}
		}
	}
}

GameObject* Framework::Find(std::string Tag) {
	auto It = ObjectList.find(Tag);
	if (It != end(ObjectList) && !It->second->DeleteObjectMarked)
		return It->second;

	return nullptr;
}

GameObject* Framework::Find(std::string Tag, Layer LayerToSearch, int Index) {
	int layer = static_cast<int>(LayerToSearch);

	if (Index >= ObjectDeque[layer].size())
		return nullptr;

	if (ObjectDeque[layer][Index]->ObjectTag == Tag && !ObjectDeque[layer][Index]->DeleteObjectMarked)
		return ObjectDeque[layer][Index];

	return nullptr;
}

size_t Framework::Size(Layer TargetLayer) {
	return ObjectDeque[static_cast<int>(TargetLayer)].size();
}

void Framework::SetController(ControllerFunction Controller, ModeType Type) {
	Controller();
	if (Type == ModeType::Default)
		ControllerBuffer = Controller;
}

void Framework::Exit() {
	glutDestroyWindow(1);
}

//////// private ///////////////
void Framework::UpdateContainer(int i) {
	auto It = std::erase_if(ObjectList, [](const std::pair<std::string, GameObject*>& Object) {
		return Object.second->DeleteObjectMarked;
		});

	if (It == 0)
		return;

	for (auto It = begin(ObjectDeque[i]); It != end(ObjectDeque[i]);) {
		if ((*It)->DeleteObjectMarked) {
			delete* It;
			*It = nullptr;
			It = ObjectDeque[i].erase(It);
			continue;
		}

		if ((*It)->SwapLayerMarked) {
			int DestLayer = (*It)->DestLayer;
			ObjectDeque[DestLayer].push_back(*It);
			ObjectDeque[DestLayer].back()->SwapLayerMarked = false;
			It = ObjectDeque[i].erase(It);
			continue;
		}

		++It;
	}
}

void Framework::ClearFloatingObject() {
	for (int i = 0; i < Layers; ++i) {
		for (auto It = begin(ObjectDeque[i]); It != end(ObjectDeque[i]); ++It) {
			if ((*It)->FloatingObjectMarked && !(*It)->StaticObjectMarked)
				(*It)->DeleteObjectMarked = true;
		}
	}
}

void Framework::ClearAll() {
	for (int i = 0; i < Layers; ++i) {
		for (auto It = begin(ObjectDeque[i]); It != end(ObjectDeque[i]); ++It) {
			if (!(*It)->StaticObjectMarked)
				(*It)->DeleteObjectMarked = true;
		}
	}
}
