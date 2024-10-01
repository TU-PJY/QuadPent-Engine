#include "Framework.h"

void Framework::InputFrameTime(float ElapsedTime) {
	FrameTime = ElapsedTime;
}

const char* Framework::Mode() {
	return CurrentRunningMode;
}

void Framework::Routine() {
	for (int i = 0; i < Layers; ++i) {
		for (auto const& O : ObjectList) {
			if (O.second->ObjectLayer == i && !O.second->DeleteObjectMarked) {
				if (!FloatingRunningActivated)
					O.second->Update(FrameTime);

				else {
					if (FloatingFocusActivated && O.second->FloatingObjectMarked)
						O.second->Update(FrameTime);
					else
						O.second->Update(FrameTime);
				}

				O.second->Render();
			}
		}

		UpdateObjectList();
	}
}

void Framework::Init(Function ModeFunction) {
	ObjectList.reserve(OBJECT_LIST_RESERVE);
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

void Framework::AddObject(GameObject* Object, const char* Tag, Layer AddLayer, ObjectType Type1, ObjectType Type2) {
	int DestLayer = static_cast<int>(AddLayer);

	Object->ObjectTag = Tag;
	Object->ObjectLayer = DestLayer;

	ObjectList.insert(std::make_pair(Tag, Object));

	if (Type1 == Type2)
		return;

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

ObjectRange Framework::EqualRange(const char* Tag) {
	ObjectRange Range{};

	auto It = ObjectList.equal_range(Tag);
	Range.First = It.first;
	Range.End = It.second;

	return Range;
}

void Framework::Exit() {
	glutDestroyWindow(1);
}

//////// private ///////////////
void Framework::UpdateObjectList() {
	for (auto It = begin(ObjectList); It != end(ObjectList);) {
		if (It->second->DeleteObjectMarked) {
			delete It->second;
			It->second = nullptr;
			It = ObjectList.erase(It);
			continue;
		}
		++It;
	}
}

void Framework::ClearFloatingObject() {
	for (auto& O : ObjectList) {
		if (O.second->FloatingObjectMarked && !O.second->StaticObjectMarked)
			O.second->DeleteObjectMarked = true;
	}
}

void Framework::ClearAll() {
	for (auto& O : ObjectList) {
		if(!O.second->StaticObjectMarked)
			O.second->DeleteObjectMarked = true;
	}
}
