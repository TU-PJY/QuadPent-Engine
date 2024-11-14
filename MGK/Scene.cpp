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
			
			++CurrentReferPosition;
		}

		CurrentReferPosition = 0;
	}
}

void Scene::Init(Function ModeFunction) {
	ObjectCommandList.reserve(OBJECT_COMMAND_LIST_BUFFER_SIZE);
	ModeFunction();
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

	if (Type1 == Type2 && Type1 == OBJECT_TYPE_STATIC) {
		Object->StaticOpt = true;
		return;
	}

	if (Type1 == Type2 && Type1 == OBJECT_TYPE_FLOATING) {
		Object->FloatingOpt = true;
		return;
	}

	if(Type1 == OBJECT_TYPE_STATIC || Type2 == OBJECT_TYPE_STATIC)
		Object->StaticOpt = true;

	if(Type1 == OBJECT_TYPE_FLOATING || Type2 == OBJECT_TYPE_FLOATING)
		Object->FloatingOpt = true;
}

void Scene::SwapLayer(GameObject* Object, int TargetLayer) {
	if (Object->ObjectLayer == TargetLayer)
		return;

	SubmitCommand(COMMAND_OBJECT_SWAP, Object->ObjectLayer, CurrentReferPosition, TargetLayer);
}

void Scene::DeleteObject(GameObject* Object) {
	SubmitCommand(COMMAND_OBJECT_DELETE, Object->ObjectLayer, CurrentReferPosition);
}

void Scene::DeleteObject(std::string Tag, int DeleteRange) {
	if (DeleteRange == DELETE_RANGE_SINGLE) {
		auto Object = ObjectIndex.find(Tag);
		if (Object != end(ObjectIndex)) 
			SubmitCommand(COMMAND_OBJECT_DELETE, Object->second->ObjectLayer, CurrentReferPosition);
	}

	else if (DeleteRange == DELETE_RANGE_EQUAL) {
		auto Range = ObjectIndex.equal_range(Tag);
		if (Range.first != Range.second) {
			for (auto Object = Range.first; Object != Range.second; ++Object) {
				if (Object->first == Tag) 
					SubmitCommand(COMMAND_OBJECT_DELETE, Object->second->ObjectLayer, CurrentReferPosition);
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

void Scene::ProcessCommandListQueue() {
	if (CommandExist) {
		std::array<int, Layers> Offset{};

		for (auto& Command : ObjectCommandList) {
			auto Object = begin(ObjectList[Command.ObjectLayer]) + Command.ReferPosition - Offset[Command.ObjectLayer];

			switch (Command.CommandType) {
			case COMMAND_OBJECT_DELETE:
				(*Object)->DeleteMark = true;
				ObjectList[Command.ObjectLayer].erase(Object);
				++Offset[Command.ObjectLayer];
				break;

			case COMMAND_OBJECT_SWAP:
				ObjectList[Command.TargetLayer].emplace_back((*Object));
				(*Object)->ObjectLayer = Command.TargetLayer;
				ObjectList[Command.ObjectLayer].erase(Object);
				++Offset[Command.ObjectLayer];
				break;
			}
		}

		ObjectCommandList.clear();

		for (auto Object = begin(ObjectIndex); Object != end(ObjectIndex); ) {
			if (Object->second->DeleteMark) {
				delete Object->second;
				Object->second = nullptr;
				Object = ObjectIndex.erase(Object);
				continue;
			}
			++Object;
		}

		CommandExist = false;
	}
}


//////// private ///////////////
void Scene::SubmitCommand(int CommandType, int ObjectLayer, int ReferPosition, int TargetLayer) {
	ObjectCommandList.emplace_back(ObjectCommand{CommandType, ObjectLayer,  ReferPosition, TargetLayer});
	CommandExist = true;
}

void Scene::ClearFloatingObject() {
	int ReferPosition{};

	for (int i = 0; i < Layers; ++i) {
		for (auto& Object : ObjectList[i]) {
			if (Object->FloatingOpt && !Object->StaticOpt) 
				SubmitCommand(COMMAND_OBJECT_DELETE, Object->ObjectLayer, ReferPosition);
			++ReferPosition;
		}
		ReferPosition = 0;
	}
}

void Scene::ClearAll() {
	int ReferPosition{};

	for (int i = 0; i < Layers; ++i) {
		for (auto& Object : ObjectList[i]) {
			if (!Object->StaticOpt) 
				SubmitCommand(COMMAND_OBJECT_DELETE, Object->ObjectLayer, ReferPosition);
			++ReferPosition;
		}
		ReferPosition = 0;
	}
}
