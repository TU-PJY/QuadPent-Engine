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
						O->UpdateFunc(FrameTime);

					else {
						if (FloatingFocusActivated && O->FloatingObjectMarked)
							O->UpdateFunc(FrameTime);
						else
							O->UpdateFunc(FrameTime);
					}
				}

				O->RenderFunc();
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

void Framework::RegisterController(ControllerFunction Controller, int Type) {
	Controller();
	if (Type == MODE_TYPE_DEFAULT)
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

void Framework::InputKey(const char* Tag, int State, unsigned char NormalKey, int SpecialKey) {
	auto It = ObjectIndex.find(Tag);
	if (It != end(ObjectIndex) && !It->second->DeleteObjectMarked)
		It->second->InputKey(State, NormalKey, SpecialKey);
}

void Framework::InputMouse(const char* Tag, int State) {
	auto It = ObjectIndex.find(Tag);
	if (It != end(ObjectIndex) && !It->second->DeleteObjectMarked)
		It->second->InputMouse(State);
}

void Framework::InputScroll(const char* Tag, int State) {
	auto It = ObjectIndex.find(Tag);
	if (It != end(ObjectIndex) && !It->second->DeleteObjectMarked)
		It->second->InputScroll(State);
}

void Framework::AddObject(GameObject* Object, const char* Tag, int AddLayer, int Type1, int Type2) {
	ObjectList[AddLayer].emplace_back(Object);
	ObjectIndex.insert(std::make_pair(Tag, Object));

	Object->ObjectTag = Tag;
	Object->ObjectLayer = AddLayer;

	if (Type1 == Type2 && Type1 == OBJECT_TYPE_STATIC) {
		Object->StaticObjectMarked = true;
		return;
	}

	if (Type1 == Type2 && Type1 == OBJECT_TYPE_FLOATING) {
		Object->FloatingObjectMarked = true;
		return;
	}

	if(Type1 == OBJECT_TYPE_STATIC || Type2 == OBJECT_TYPE_STATIC)
		Object->StaticObjectMarked = true;

	if(Type1 == OBJECT_TYPE_FLOATING || Type2 == OBJECT_TYPE_FLOATING)
		Object->FloatingObjectMarked = true;
}

void Framework::SwapLayer(GameObject* Object, int TargetLayer) {
	if (Object->ObjectLayer == TargetLayer)
		return;

	Object->SwapLayerMarked = true;
	Object->ObjectLayer = TargetLayer;
}

void Framework::DeleteObject(GameObject* Object) {
	Object->DeleteObjectMarked = true;
}

void Framework::DeleteObject(const char* Tag, int DeleteRange) {
	if (DeleteRange == DELETE_RANGE_SINGLE) {
		auto It = ObjectIndex.find(Tag);
		if (It != end(ObjectIndex) && !It->second->DeleteObjectMarked)
			It->second->DeleteObjectMarked = true;
	}

	else if (DeleteRange == DELETE_RANGE_EQUAL) {
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

GameObject* Framework::FindMulti(const char* Tag, int SearchLayer, int Index) {
	auto Object = ObjectList[SearchLayer][Index];
	if(Object->ObjectTag == Tag)
		return ObjectList[SearchLayer][Index];
	
	return nullptr;
}

ObjectRange Framework::EqualRange(const char* Tag) {
	ObjectRange Range;
	auto It = ObjectIndex.equal_range(Tag);

	Range.First = It.first;
	Range.End = It.second;

	return Range;
}

size_t Framework::LayerSize(int TargetLayer) {
	return ObjectList[TargetLayer].size();
}

void Framework::Exit() {
	glutDestroyWindow(1);
}

void Framework::SwitchScreenState() {
	if (!FullscreenState) {
		glutFullScreen();
		WIDTH = GetSystemMetrics(SM_CXSCREEN);
		HEIGHT = GetSystemMetrics(SM_CYSCREEN);
		FullscreenState = true;
	}

	else {
		WIDTH = WINDOW_WIDTH;
		HEIGHT = WINDOW_HEIGHT;
		glutReshapeWindow(WIDTH, HEIGHT);
		glutPositionWindow(0, 0);
		FullscreenState = false;
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
