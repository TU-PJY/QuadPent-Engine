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
	ObjectCommandList.reserve(1000);
	ThreadUtil::InitSection(Section);
	ThreadUtil::New(UpdateThread, ObjectIndexUpdateThread, this);
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
	ThreadUtil::Lock(Section);
	auto Object = ObjectIndex.find(Tag);
	if (Object != end(ObjectIndex))
		Object->second->ResetControlState();
	ThreadUtil::Unlock(Section);
}

void Scene::InputKey(std::string Tag, int State, unsigned char NormalKey, int SpecialKey) {
	ThreadUtil::Lock(Section);
	auto Object = ObjectIndex.find(Tag);
	if (Object != end(ObjectIndex))
		Object->second->InputKey(State, NormalKey, SpecialKey);
	ThreadUtil::Unlock(Section);
}

void Scene::InputMouse(std::string Tag, int State) {
	ThreadUtil::Lock(Section);
	auto Object = ObjectIndex.find(Tag);
	if (Object != end(ObjectIndex))
		Object->second->InputMouse(State);
	ThreadUtil::Unlock(Section);
}

void Scene::InputScroll(std::string Tag, int State) {
	ThreadUtil::Lock(Section);
	auto Object = ObjectIndex.find(Tag);
	if (Object != end(ObjectIndex))
		Object->second->InputScroll(State);
	ThreadUtil::Unlock(Section);
}

void Scene::AddObject(GameObject* Object, std::string Tag, int AddLayer, int Type1, int Type2) {
	ObjectList[AddLayer].emplace_back(Object);

	ThreadUtil::Lock(Section);
	ObjectIndex.insert(std::pair(Tag, Object));
	ThreadUtil::Unlock(Section);

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
		ThreadUtil::Lock(Section);
		auto Object = ObjectIndex.find(Tag);
		if (Object != end(ObjectIndex))
			SubmitCommand(COMMAND_OBJECT_DELETE, Object->second->ObjectLayer, CurrentReferPosition);
		ThreadUtil::Unlock(Section);
	}

	else if (DeleteRange == DELETE_RANGE_EQUAL) {
		ThreadUtil::Lock(Section);
		auto Range = ObjectIndex.equal_range(Tag);
		if (Range.first != Range.second) {
			for (auto Object = Range.first; Object != Range.second; ++Object) {
				if (Object->first == Tag)
					SubmitCommand(COMMAND_OBJECT_DELETE, Object->second->ObjectLayer, CurrentReferPosition);
			}
		}
		ThreadUtil::Unlock(Section);
	}
}

GameObject* Scene::Find(std::string Tag) {
	GameObject* Found{};

	ThreadUtil::Lock(Section);
	auto Object = ObjectIndex.find(Tag);
	if (Object != end(ObjectIndex))
		Found = Object->second;
	ThreadUtil::Unlock(Section);

	return Found;
}

GameObject* Scene::FindMulti(std::string Tag, int SearchLayer, int Index) {
	auto Object = ObjectList[SearchLayer][Index];
	if(Object->ObjectTag == Tag)
		return ObjectList[SearchLayer][Index];
	
	return nullptr;
}

std::pair<ObjectRange, ObjectRange> Scene::EqualRange(std::string Tag) {
	ThreadUtil::Lock(Section);
	auto Range = ObjectIndex.equal_range(Tag);
	ThreadUtil::Unlock(Section);

	return Range;
}

size_t Scene::LayerSize(int TargetLayer) {
	return ObjectList[TargetLayer].size();
}

void Scene::ProcessCommandListQueue() {
	if (CommandExist) {
		for (auto& Command : ObjectCommandList) {
			auto Object = begin(ObjectList[Command.ObjectLayer]) + Command.ReferPosition;

			switch (Command.CommandType) {
			case COMMAND_OBJECT_DELETE:
				(*Object)->DeleteMark = true;
				ObjectList[Command.ObjectLayer].erase(Object);
				break;

			case COMMAND_OBJECT_SWAP:
				ObjectList[Command.TargetLayer].emplace_back((*Object));
				(*Object)->ObjectLayer = Command.TargetLayer;
				ObjectList[Command.ObjectLayer].erase(Object);
				break;
			}
		}

		ObjectCommandList.clear();
		CommandExist = false;
	}
}


//////// private ///////////////
void Scene::SubmitCommand(int CommandType, int ObjectLayer, int ReferPosition, int TargetLayer) {
	ObjectCommand Command{};
	Command.CommandType = CommandType;
	Command.ObjectLayer = ObjectLayer;
	Command.TargetLayer = TargetLayer;
	Command.ReferPosition = ReferPosition;
	ObjectCommandList.emplace_back(Command);

	CommandExist = true;
}

DWORD WINAPI Scene::ObjectIndexUpdateThread(LPVOID Param) {
	Scene* scene = static_cast<Scene*>(Param);

	while (true) {
		ThreadUtil::Lock(scene->Section);
		for (auto Object = begin(scene->ObjectIndex); Object != end(scene->ObjectIndex); ) {
			if (Object->second->DeleteMark) {
				delete Object->second;
				Object->second = nullptr;
				Object = scene->ObjectIndex.erase(Object);
				continue;
			}
			++Object;
		}
		ThreadUtil::Unlock(scene->Section);

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	return 0;
}

void Scene::ClearFloatingObject() {
	int ReferPosition{};

	for (int i = 0; i < Layers; ++i) {
		for (auto& Object : ObjectList[i]) {
			if(Object->FloatingOpt && !Object->StaticOpt)
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
