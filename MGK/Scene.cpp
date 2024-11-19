#include "Scene.h"
#include <chrono>
#include <thread>

void Scene::InputFrameTime(float ElapsedTime) {
	FrameTime = ElapsedTime;
}

std::string Scene::Mode() {
	return CurrentRunningMode;
}

void Scene::Stop() {
	UpdateActivateCommand = false;
}

void Scene::Resume() {
	UpdateActivateCommand = true;
}

void Scene::Routine() {
	for (int i = 0; i < Layers; ++i) {
		for (auto& Object : ObjectList[i]) {
			if (UpdateActivateCommand) {
				if (FloatingFocusCommand && Object->FloatingOpt)
					Object->UpdateFunc(FrameTime);
				else
					Object->UpdateFunc(FrameTime);
			}
			Object->RenderFunc();
			Object->CheckDeleteReserveCommand();
			++CurrentReferLocation;
		}
		CurrentReferLocation = 0;
	}
}

void Scene::Init(Function ModeFunction) {
	ModeFunction();
	for(int i = 0; i < Layers; ++i)
		DeleteLocation[i].reserve(DELETE_LOCATION_BUFFER_SIZE);
}

void Scene::SwitchMode(Function ModeFunction) {
	ClearAll();

	if (DestructorBuffer)
		DestructorBuffer();

	ModeFunction();

	if (FloatingActivateCommand) {
		FloatingActivateCommand = false;
		FloatingFocusCommand = false;
	}
}

void Scene::RegisterDestructor(Function DestructorFunction) {
	DestructorBuffer = DestructorFunction;
}

void Scene::RegisterModeName(std::string ModeName) {
	CurrentRunningMode = ModeName;
}

void Scene::RegisterController(ControllerFunction Controller, int Type) {
	Controller();
	if (Type == MODE_TYPE_DEFAULT)
		ControllerBuffer = Controller;
}

void Scene::ReleaseDestructor() {
	DestructorBuffer = nullptr;
}

void Scene::StartFloatingMode(Function ModeFunction, bool FloatingFocus) {
	if (FloatingActivateCommand)
		return;

	PrevRunningMode = CurrentRunningMode;
	ModeFunction();
	FloatingFocusCommand = FloatingFocus;

	FloatingActivateCommand = true;
}

void Scene::EndFloatingMode() {
	if (!FloatingActivateCommand)  
		return;

	ClearFloatingObject();
	CurrentRunningMode = PrevRunningMode;

	if (ControllerBuffer)  
		ControllerBuffer();

	FloatingActivateCommand = false;
	FloatingFocusCommand = false;
}

void Scene::ResetControlState(GameObject* Object) {
	Object->ResetControlState();
}

void Scene::ResetControlState(std::string Tag) {
	auto Object = ObjectIndex.find(Tag);
	if (Object != end(ObjectIndex))
		Object->second->ResetControlState();
}

void Scene::InputKey(std::string Tag, int State, unsigned char NormalKey, int SpecialKey) {
	auto Object = ObjectIndex.find(Tag);
	if (Object != end(ObjectIndex))
		Object->second->InputKey(State, NormalKey, SpecialKey);
}

void Scene::InputMouse(std::string Tag, int State) {
	auto Object = ObjectIndex.find(Tag);
	if (Object != end(ObjectIndex))
		Object->second->InputMouse(State);
}

void Scene::InputScroll(std::string Tag, int State) {
	auto Object = ObjectIndex.find(Tag);
	if (Object != end(ObjectIndex))
		Object->second->InputScroll(State);
}

void Scene::AddObject(GameObject* Object, std::string Tag, int AddLayer, int Type1, int Type2) {
	ObjectList[AddLayer].emplace_back(Object);
	ObjectIndex.insert(std::pair(Tag, Object));

	Object->ObjectTag = Tag;
	Object->ObjectLayer = AddLayer;

	if(Type1 == Type2) {
		if(Type1 == OBJECT_TYPE_STATIC)
			Object->StaticOpt = true;

		else if(Type1 == OBJECT_TYPE_FLOATING)
			Object->FloatingOpt = true;

		return;
	}

	else {
		if(Type1 == OBJECT_TYPE_STATIC || Type2 == OBJECT_TYPE_STATIC)
			Object->StaticOpt = true;

		if(Type1 == OBJECT_TYPE_FLOATING || Type2 == OBJECT_TYPE_FLOATING)
			Object->FloatingOpt = true;
	}
}

void Scene::SwapLayer(GameObject* Object, int TargetLayer) {
	Object->SwapCommand = true;
	AddLocation(Object->ObjectLayer, CurrentReferLocation);
	Object->ObjectLayer = TargetLayer;
	CommandExist = true;
}

void Scene::DeleteObject(GameObject* Object) {
	Object->DeleteCommand = true;
	AddLocation(Object->ObjectLayer, CurrentReferLocation);
	CommandExist = true;
}

void Scene::DeleteObject(std::string Tag, int DeleteRange) {
	if (DeleteRange == DELETE_RANGE_SINGLE) {
		auto Object = ObjectIndex.find(Tag);
		if (Object != end(ObjectIndex))
			Object->second->DeleteReserveCommand = true;
	}

	else if (DeleteRange == DELETE_RANGE_EQUAL) {
		auto Range = ObjectIndex.equal_range(Tag);
		if (Range.first != Range.second) {
			for (auto Object = Range.first; Object != Range.second; ++Object) {
				if (Object->first == Tag)
					Object->second->DeleteReserveCommand = true;
			}
		}
	}
}

GameObject* Scene::Find(std::string Tag) {
	auto Object = ObjectIndex.find(Tag);
	if (Object != end(ObjectIndex))
		return Object->second;

	return nullptr;
}

GameObject* Scene::FindMulti(std::string Tag, int SearchLayer, int Index) {
	auto Object = ObjectList[SearchLayer][Index];
	if(Object->ObjectTag == Tag)
		return ObjectList[SearchLayer][Index];
	
	return nullptr;
}

std::pair<ObjectRange, ObjectRange> Scene::EqualRange(std::string Tag) {
	return ObjectIndex.equal_range(Tag);
}

size_t Scene::LayerSize(int TargetLayer) {
	return ObjectList[TargetLayer].size();
}

void  Scene::CompleteCommand() {
	if(!CommandExist)
		return;

	ProcessObjectCommand();
	ProcessSceneCommand();
	CommandExist = false;
}

//////// private ///////////////
void Scene::AddLocation(int Layer, int Position) {
	DeleteLocation[Layer].emplace_back(Position);
}

void Scene::ProcessObjectCommand() {
	int Offset{};

	for (int Layer = 0; Layer < Layers; ++Layer) {
		size_t Size = DeleteLocation[Layer].size();
		if (Size == 0)
			continue;

		for (int i = 0; i < Size; ++i) {
			auto Object = ObjectList[Layer].begin() + DeleteLocation[Layer][i] - Offset;

			if ((*Object)->DeleteCommand) {
				ObjectList[Layer].erase(Object);
				++SceneCommandCount;
			}

			else if ((*Object)->SwapCommand) {
				auto Ptr = (*Object);
				ObjectList[Ptr->ObjectLayer].emplace_back((*Object));
				ObjectList[Layer].erase(Object);
				Ptr->SwapCommand = false;
			}

			++Offset;
		}

		DeleteLocation[Layer].clear();
		Offset = 0;
	}
}

void Scene::ProcessSceneCommand() {
	auto Object = begin(ObjectIndex);
	while (Object != end(ObjectIndex) && SceneCommandCount != 0) {
		if (Object->second->DeleteCommand) {
			delete Object->second;
			Object->second = nullptr;
			Object = ObjectIndex.erase(Object);
			--SceneCommandCount;
			continue;
		}
		++Object;
	}
}

void Scene::ClearFloatingObject() {
	int ReferPosition{};
	for (int Layer = 0; Layer < Layers; ++Layer) {
		for (auto Object = begin(ObjectList[Layer]); Object != end(ObjectList[Layer]); ++ Object) {
			if ((*Object)->FloatingOpt && !(*Object)->StaticOpt) {
				(*Object)->DeleteCommand = true;
				AddLocation(Layer, ReferPosition);
			}
			++ReferPosition;
		}
		ReferPosition = 0;
	}
}

void Scene::ClearAll() {
	int ReferPosition{};
	for (int Layer = 0; Layer < Layers; ++Layer) {
		for (auto Object = begin(ObjectList[Layer]); Object != end(ObjectList[Layer]); ++ Object) {
			if (!(*Object)->StaticOpt) {
				(*Object)->DeleteCommand = true;
				AddLocation(Layer, ReferPosition);
			}
			++ReferPosition;
		}
		ReferPosition = 0;
	}
}
