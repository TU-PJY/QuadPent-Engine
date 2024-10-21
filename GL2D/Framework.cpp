#include "Framework.h"

void Framework::InputFrameTime(float ElapsedTime) {
	FrameTime = ElapsedTime;
}

const char* Framework::Mode() {
	return CurrentRunningMode;
}

void Framework::Stop() {
	RoutineUpdateActivated = false;
}

void Framework::Resume() {
	RoutineUpdateActivated = true;
}

void Framework::Routine() {
	for (int i = 0; i < Layers; ++i) {
		for (auto const& O : ObjectList[i]) {
			if (!O->DeleteObjectMarked) {
				if (RoutineUpdateActivated) {
					if (!FloatingRunningActivated)
						O->Update(FrameTime);

					else {
						if (FloatingFocusActivated && O->FloatingObjectMarked)
							O->Update(FrameTime);
						else
							O->Update(FrameTime);
					}
				}

				O->Render();
			}
		}

		UpdateObjectList(i);
	}

	UpdateObjectIndex();
}

void Framework::Init(Function ModeFunction) {
	for(int i = 0; i < Layers; ++i)
		ObjectList[i].reserve(OBJECT_LIST_RESERVE);
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

void Framework::RegisterModeName(const char* ModeName) {
	CurrentRunningMode = ModeName;
}

void Framework::RegisterController(ControllerFunction Controller, ModeType Type) {
	Controller();
	if (Type == ModeType::Default)
		ControllerBuffer = Controller;
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
	auto It = ObjectIndex.find(Tag);
	if (It != end(ObjectIndex) && !It->second->DeleteObjectMarked)
		It->second->ResetControlState();
}

void Framework::InputKey(const char* Tag, KeyType Key, KeyState State, unsigned char NormalKey, int SpecialKey) {
	auto It = ObjectIndex.find(Tag);
	if (It != end(ObjectIndex) && !It->second->DeleteObjectMarked)
		It->second->InputKey(Key, State, NormalKey, SpecialKey);
}

void Framework::InputMouse(const char* Tag, int button, int state, int x, int y) {
	auto It = ObjectIndex.find(Tag);
	if (It != end(ObjectIndex) && !It->second->DeleteObjectMarked)
		It->second->InputMouse(button, state, x, y);
}

void Framework::InputScroll(const char* Tag, int button, int Wheel, int x, int y) {
	auto It = ObjectIndex.find(Tag);
	if (It != end(ObjectIndex) && !It->second->DeleteObjectMarked)
		It->second->InputScroll(button, Wheel, x, y);
}

void Framework::AddObject(GameObject* Object, const char* Tag, Layer AddLayer, ObjectType Type1, ObjectType Type2) {
	int DestLayer = static_cast<int>(AddLayer);

	ObjectList[DestLayer].emplace_back(Object);
	ObjectIndex.insert(std::make_pair(Tag, Object));

	Object->ObjectTag = Tag;
	Object->ObjectLayer = DestLayer;

	if (Type1 == Type2 && Type1 == ObjectType::Static)
		Object->StaticObjectMarked = true;

	if(Type1 == Type2 && Type1 == ObjectType::Floating)
		Object->FloatingObjectMarked = true;

	if(Type1 == ObjectType::Static || Type2 == ObjectType::Static)
		Object->StaticObjectMarked = true;

	if(Type1 == ObjectType::Floating || Type2 == ObjectType::Floating)
		Object->FloatingObjectMarked = true;
}

void Framework::SwapLayer(GameObject* Object, Layer TargetLayer) {
	int DestLayer = static_cast<int>(TargetLayer);

	if (Object->ObjectLayer == DestLayer)
		return;

	Object->SwapLayerMarked = true;
	Object->ObjectLayer = DestLayer;
}

void Framework::DeleteObject(GameObject* Object) {
	Object->DeleteObjectMarked = true;
}

void Framework::DeleteObject(const char* Tag, DeleteRange deleteRange) {
	if (deleteRange == DeleteRange::One) {
		auto It = ObjectIndex.find(Tag);
		if (It != end(ObjectIndex) && !It->second->DeleteObjectMarked)
			It->second->DeleteObjectMarked = true;
	}

	else if (deleteRange == DeleteRange::All) {
		auto Range = ObjectIndex.equal_range(Tag);
		if (Range.first != Range.second) {
			for (auto It = Range.first; It != Range.second; ++It) {
				if (It->first == Tag && !It->second->DeleteObjectMarked) 
					It->second->DeleteObjectMarked = true;
			}
		}
	}
}

GameObject* Framework::Find(const char* Tag) {
	auto It = ObjectIndex.find(Tag);
	if (It != end(ObjectIndex) && !It->second->DeleteObjectMarked)
		return It->second;

	return nullptr;
}

GameObject* Framework::FindMulti(const char* Tag, Layer SearchLayer, int Index) {
	auto Object = ObjectList[static_cast<int>(SearchLayer)][Index];
	if(Object->ObjectTag == Tag)
		return ObjectList[static_cast<int>(SearchLayer)][Index];
	
	return nullptr;
}

ObjectRange Framework::EqualRange(const char* Tag) {
	ObjectRange Range;
	auto It = ObjectIndex.equal_range(Tag);

	Range.First = It.first;
	Range.End = It.second;

	return Range;
}

size_t Framework::LayerSize(Layer TargetLayer) {
	return ObjectList[static_cast<int>(TargetLayer)].size();
}

void Framework::Exit() {
	glutDestroyWindow(1);
}

void Framework::ApplyBlurDefalutObject(GLfloat Value) {
	for (auto const& O : ObjectIndex) {
		if (!O.second->FloatingObjectMarked && !O.second->DeleteObjectMarked)
			O.second->BlurValue = Value;
	}
}

void Framework::ApplyBlurFloatingObject(GLfloat Value) {
	for (auto const& O : ObjectIndex) {
		if (O.second->FloatingObjectMarked && !O.second->DeleteObjectMarked)
			O.second->BlurValue = Value;
	}
}

//////// private ///////////////
void Framework::UpdateObjectList(int Index) {	
	for (auto It = begin(ObjectList[Index]); It != end(ObjectList[Index]);) {
		if ((*It)->DeleteObjectMarked) {
			It = ObjectList[Index].erase(It);
			continue;
		}

		if ((*It)->SwapLayerMarked) {
			auto Object = (*It);
			int DestLayer = (*It)->ObjectLayer;
			It = ObjectList[Index].erase(It);
			ObjectList[DestLayer].emplace_back(Object);
			Object->SwapLayerMarked = false;
			continue;
		}
		++It;
	}
}

void Framework::UpdateObjectIndex() {
	for (auto It = begin(ObjectIndex); It != end(ObjectIndex);) {
		if (It->second->DeleteObjectMarked) {
			delete It->second;
			It->second = nullptr;
			It = ObjectIndex.erase(It);
			continue;
		}
		++It;
	}
}

void Framework::ClearFloatingObject() {
	for (auto const& O : ObjectIndex) {
		if (O.second->FloatingObjectMarked && !O.second->StaticObjectMarked)
			O.second->DeleteObjectMarked = true;
	}
}

void Framework::ClearAll() {
	for (auto const& O : ObjectIndex) {
		if (!O.second->StaticObjectMarked)
			O.second->DeleteObjectMarked = true;
	}
}
