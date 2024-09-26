#include "Framework.h"

void Framework::InputFrameTime(float ElapsedTime) {
	FrameTime = ElapsedTime;
}

void Framework::InputModeName(const char* ModeName) {
	CurrentRunningMode = ModeName;
}

const char* Framework::Mode() {
	return CurrentRunningMode;
}

void Framework::Routine() {
	for (int i = 0; i < Layers; ++i) {
		for (auto const& O : ObjectVector[i]) {
			if (!O->DeleteObjectMarked) {
				if (FloatingRunningActivated && FloatingFocusActivated && O->FloatingObjectMarked)
					O->Update(FrameTime);
				else
					O->Update(FrameTime);
				O->Render();
			}
		}

		UpdateContainer(i);
	}
}

void Framework::Init(Function ModeFunction) {
	for (int i = 0; i < Layers; ++i) {
		ObjectVector[i].reserve(200);
		ObjectVector[i].emplace_back(new __DUMMY__);
		ObjectVector[i].back()->StaticObjectMarked = true;
	}

	ModeFunction();
}

void Framework::SwitchMode(Function ModeFunction) {
	ClearAll();

	if (DestructorBuffer)
		DestructorBuffer();

	ModeFunction();

	if (FloatingRunningActivated) {
		FloatingRunningActivated = false;
		FloatingFocusActivated = false;
	}
}

void Framework::RegisterDestructor(Function DestructorFunction) {
	DestructorBuffer = DestructorFunction;
}

void Framework::ReleaseDestructor() {
	DestructorBuffer = nullptr;
}

void Framework::StartFloatingMode(Function ModeFunction, bool FloatingFocus) {
	if (FloatingRunningActivated)
		return;

	PrevRunningMode = CurrentRunningMode;
	ModeFunction();
	FloatingFocusActivated = FloatingFocus;

	FloatingRunningActivated = true;
}

void Framework::EndFloatingMode() {
	if (!FloatingRunningActivated)  
		return;

	ClearFloatingObject();
	CurrentRunningMode = PrevRunningMode;

	if (ControllerBuffer)  
		ControllerBuffer();

	FloatingRunningActivated = false;
	FloatingFocusActivated = false;
}

void Framework::ResetControlState(GameObject* Object) {
	Object->ResetControlState();
}

void Framework::ResetControlState(const char* Tag) {
	auto It = ObjectList.find(Tag);
	if (It != end(ObjectList) && !It->second->DeleteObjectMarked)
		It->second->ResetControlState();
}

void Framework::InputKey(const char* Tag, KeyType Key, KeyState State, unsigned char NormalKey, int SpecialKey) {
	auto It = ObjectList.find(Tag);
	if (It != end(ObjectList) && !It->second->DeleteObjectMarked)
		It->second->InputKey(Key, State, NormalKey, SpecialKey);
}

void Framework::InputMouse(const char* Tag, int button, int state, int x, int y) {
	auto It = ObjectList.find(Tag);
	if (It != end(ObjectList) && !It->second->DeleteObjectMarked)
		It->second->InputMouse(button, state, x, y);
}

void Framework::InputScroll(const char* Tag, int button, int Wheel, int x, int y) {
	auto It = ObjectList.find(Tag);
	if (It != end(ObjectList) && !It->second->DeleteObjectMarked)
		It->second->InputScroll(button, Wheel, x, y);
}

void Framework::AddObject(GameObject* Object, const char* Tag, Layer AddLayer, bool SetFloatingObject, bool SetStaticObject) {
	int DestLayer = static_cast<int>(AddLayer);

	ObjectVector[DestLayer].emplace_back(Object);
	Object->ObjectTag = Tag;
	Object->DestLayer = DestLayer;
	
	ObjectList.insert(std::make_pair(Tag, Object));

	if (SetFloatingObject) 
		Object->FloatingObjectMarked = true;
		
	if (SetStaticObject) 
		Object->StaticObjectMarked = true;
}

void Framework::SwapLayer(GameObject* Object, Layer TargetLayer) {
	int DestLayer = static_cast<int>(TargetLayer);

	if (Object->DestLayer == DestLayer)
		return;

	Object->SwapLayerMarked = true;
	Object->DestLayer = DestLayer;
}

void Framework::DeleteObject(GameObject* Object) {
	Object->DeleteObjectMarked = true;
}

void Framework::DeleteObject(const char* Tag, DeleteRange deleteRange) {
	if (deleteRange == DeleteRange::One) {
		auto It = ObjectList.find(Tag);
		if (It != end(ObjectList) && !It->second->DeleteObjectMarked) 
			It->second->DeleteObjectMarked = true;
	}

	else if (deleteRange == DeleteRange::All) {
		auto Range = ObjectList.equal_range(Tag);
		if (Range.first != Range.second) {
			for (auto It = Range.first; It != Range.second; ++It) {
				if (It->first == Tag && !It->second->DeleteObjectMarked) 
					It->second->DeleteObjectMarked = true;
			}
		}
	}
}

GameObject* Framework::Find(const char* Tag) {
	auto It = ObjectList.find(Tag);
	if (It != end(ObjectList) && !It->second->DeleteObjectMarked)
		return It->second;

	return nullptr;
}

GameObject* Framework::Find(const char* Tag, Layer LayerToSearch, int Index) {
	int layer = static_cast<int>(LayerToSearch);

	if (Index >= ObjectVector[layer].size())
		return nullptr;

	if (ObjectVector[layer][Index]->ObjectTag == Tag && !ObjectVector[layer][Index]->DeleteObjectMarked)
		return ObjectVector[layer][Index];

	return nullptr;
}

size_t Framework::Size(Layer TargetLayer) {
	return ObjectVector[static_cast<int>(TargetLayer)].size();
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

	for (auto It = begin(ObjectVector[i]); It != end(ObjectVector[i]);) {
		if ((*It)->DeleteObjectMarked) {
			delete* It;
			*It = nullptr;
			It = ObjectVector[i].erase(It);
			continue;
		}

		if ((*It)->SwapLayerMarked) {
			int DestLayer = (*It)->DestLayer;
			ObjectVector[DestLayer].emplace_back(*It);
			ObjectVector[DestLayer].back()->SwapLayerMarked = false;
			It = ObjectVector[i].erase(It);
			continue;
		}

		++It;
	}
}

void Framework::ClearFloatingObject() {
	for (int i = 0; i < Layers; ++i) {
		for (auto It = begin(ObjectVector[i]); It != end(ObjectVector[i]); ++It) {
			if ((*It)->FloatingObjectMarked && !(*It)->StaticObjectMarked)
				(*It)->DeleteObjectMarked = true;
		}
	}
}

void Framework::ClearAll() {
	for (int i = 0; i < Layers; ++i) {
		for (auto It = begin(ObjectVector[i]); It != end(ObjectVector[i]); ++It) {
			if (!(*It)->StaticObjectMarked)
				(*It)->DeleteObjectMarked = true;
		}
	}
}
