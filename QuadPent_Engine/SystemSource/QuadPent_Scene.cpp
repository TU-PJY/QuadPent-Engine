#include "QuadPent_Scene.h"
#include "QuadPent_ErrorMessage.h"
#include "QuadPent_SoundTool.h"

QP::QuadPent_Scene QP::Scene;

void QP::QuadPent_Scene::InputFrameTime(float ElapsedTime) {
	FrameTime = ElapsedTime;
}

std::string QP::QuadPent_Scene::ModeName() {
	return CurrentRunningModeName;
}

bool QP::QuadPent_Scene::CheckCurrentMode(QP::MODE_PTR ModePtr) {
	if (ModePtr == CurrentRunningModePtr)
		return true;
	return false;
}

QP::MODE_PTR QP::QuadPent_Scene::Mode() {
	return CurrentRunningModePtr;
}

void QP::QuadPent_Scene::Stop() {
	UpdateActivateCommand = false;
}

void QP::QuadPent_Scene::Resume() {
	if (!ErrorOccured)
		UpdateActivateCommand = true;
}

void QP::QuadPent_Scene::Update() {
	if (!ErrorScreenState && ErrorOccured) {
		QP::SoundTool.StopAllSounds();
		SwitchToErrorScreen();
		ErrorScreenState = true;
	}

	if (ErrorOccured)
		return;

	for (int Layer = 0; Layer < SceneLayer; ++Layer) {
		for (auto& Object : ObjectList[Layer]) {
			if (UpdateActivateCommand) {
				if (!Object->DeleteCommand) {
					if (FloatingFocusCommand) {
						if(Object->FloatingCommand)
							Object->UpdateFunc(FrameTime);
					}
					else
						Object->UpdateFunc(FrameTime);
				}
			}

			if (LoopEscapeCommand || ErrorOccured) {
				LoopEscapeCommand = false;
				return;
			}

			if (Object->DeleteCommand)
				AddLocation(Layer, CurrentReferLocation);

			else if (Object->SwapCommand)
				AddLocation(Layer, CurrentReferLocation);

			++CurrentReferLocation;
		}
		CurrentReferLocation = 0;
	}
}

void QP::QuadPent_Scene::Render() {
	for (int i = 0; i < SceneLayer; ++i) {
		for (auto& Object : ObjectList[i]) {
			if (!Object->DeleteCommand) 
				Object->RenderFunc();
		}
	}
}

void QP::QuadPent_Scene::Init(QP::MODE_PTR ModePtr) {
	ModePtr();
	for (int Layer = 0; Layer < SceneLayer; ++Layer)
		CommandLocation[Layer].reserve(COMMAND_LOCATION_BUFFER_SIZE);
}

void QP::QuadPent_Scene::SwitchMode(QP::MODE_PTR ModePtr, int SwitchOption) {
	if (SwitchOption == MODE_SWITCH_DEFAULT && CurrentRunningModePtr == ModePtr) {
		SetErrorScreen(ERROR_TYPE_EXECUTED_MODE_EXECUTION, CurrentRunningModeName);
		return;
	}

	ClearAll();

	if (DestructorBuffer)
		DestructorBuffer();

	if (!ModePtr) {
		SetErrorScreen(ERROR_TYPE_UNMAPPED_MODE_EXECUTION, CurrentRunningModeName);
		return;
	}
	ModePtr();

	if (FloatingActivateCommand) {
		FloatingActivateCommand = false;
		FloatingFocusCommand = false;
	}

	LoopEscapeCommand = true;
}

void QP::QuadPent_Scene::RegisterDestructor(QP::MODE_PTR ModeDestructorPtr) {
	DestructorBuffer = ModeDestructorPtr;
}

void QP::QuadPent_Scene::RegisterModeName(std::string ModeName) {
	CurrentRunningModeName = ModeName;
}

void QP::QuadPent_Scene::RegisterModePtr(QP::MODE_PTR ModePtr) {
	CurrentRunningModePtr = ModePtr;
}

void QP::QuadPent_Scene::RegisterController(SUBCLASSPROC Controller, int Type) {
	if(CurrentController)
		RemoveWindowSubclass(QP::System_HWND, CurrentController, 1);
	SetWindowSubclass(QP::System_HWND, Controller, 1, 0);
	CurrentController = Controller;

	if(Type == MODE_TYPE_DEFAULT)
		ControllerBuffer = Controller;
}

void QP::QuadPent_Scene::RegisterInputObjectList(std::vector<QP::Object*>& Vec) {
	InputObjectListPtr = &Vec;
}

void QP::QuadPent_Scene::StartFloatingMode(QP::MODE_PTR ModePtr, bool FloatingFocusFlag) {
	if (FloatingActivateCommand) {
		SetErrorScreen(ERROR_TYPE_EXECUTED_FLOATING_MODE_EXECUTUION, CurrentRunningModeName);
		return;
	}

	if (!ModePtr) {
		SetErrorScreen(ERROR_TYPE_UNMAPPED_MODE_EXECUTION, CurrentRunningModeName);
		return;
	}

	ModePtr();

	PrevRunningModeName = CurrentRunningModeName;
	PrevRunningModePtr = CurrentRunningModePtr;
	FloatingFocusCommand = FloatingFocusFlag;

	FloatingActivateCommand = true;
}

void QP::QuadPent_Scene::EndFloatingMode() {
	if (!FloatingActivateCommand) {
		SetErrorScreen(ERROR_TYPE_TERMINATED_FLOATING_MODE_TERMINATION, CurrentRunningModeName);
		return;
	}

	ClearFloatingObject();
	CurrentRunningModeName = PrevRunningModeName;
	CurrentRunningModePtr = PrevRunningModePtr;

	RemoveWindowSubclass(QP::System_HWND, CurrentController, 1);
	SetWindowSubclass(QP::System_HWND, ControllerBuffer, 1, 0);
	CurrentController = ControllerBuffer;
	ControllerBuffer = nullptr;

	FloatingActivateCommand = false;
	FloatingFocusCommand = false;
}

QP::Object* QP::QuadPent_Scene::AddObject(QP::Object* Object, std::string Tag, unsigned int AddLayer, bool UseController, int Type1, int Type2) {
	if (AddLayer > SceneLayer - 2) {
		SetErrorScreen(ERROR_TYPE_OBJECT_ADD_OUT_OF_SCENE_LAYER, CurrentRunningModeName);
		return nullptr;
	}

	if (Type1 == OBJECT_TYPE_STATIC_SINGLE || Type2 == OBJECT_TYPE_STATIC_SINGLE) {
		if (auto Object = Find(Tag); Object)
			return nullptr;
	}

	ObjectList[AddLayer].emplace_back(Object);

	Object->ObjectTag = Tag;
	Object->ObjectLayer = AddLayer;

	if (UseController) 
		InputObjectListPtr->emplace_back(Object);

	if(Type1 == Type2) {
		if(Type1 == OBJECT_TYPE_STATIC || Type1 == OBJECT_TYPE_STATIC_SINGLE)
			Object->StaticCommand = true;

		else if(Type1 == OBJECT_TYPE_FLOATING)
			Object->FloatingCommand = true;
	}

	else {
		if(Type1 == OBJECT_TYPE_STATIC || Type2 == OBJECT_TYPE_STATIC || Type1 == OBJECT_TYPE_STATIC_SINGLE || Type2 == OBJECT_TYPE_STATIC_SINGLE)
			Object->StaticCommand = true;

		if(Type1 == OBJECT_TYPE_FLOATING || Type2 == OBJECT_TYPE_FLOATING)
			Object->FloatingCommand = true;
	}

	return Object;
}

void QP::QuadPent_Scene::DeleteObject(QP::Object* Object) {
	Object->DeleteCommand = true;
	Object->ObjectTag = "";
}

void QP::QuadPent_Scene::DeleteObject(std::string Tag, int DeleteRange) {
	switch (DeleteRange) {
	case DELETE_RANGE_SINGLE:
		for (int Layer = 0; Layer < SceneLayer - 1; ++Layer) {
			for (auto const& Object : ObjectList[Layer]) {
				if (Object->ObjectTag.compare(Tag) == 0) {
					Object->DeleteCommand = true;
					Object->ObjectTag = "";
					return;
				}
			}
		}
		break;

	case DELETE_RANGE_ALL:
		for (int Layer = 0; Layer < SceneLayer - 1; ++Layer) {
			for (auto const& Object : ObjectList[Layer]) {
				if (Object->ObjectTag.compare(Tag) == 0) {
					Object->DeleteCommand = true;
					Object->ObjectTag = "";
				}
			}
		}
		break;
	}
}

void QP::QuadPent_Scene::AddInputObject(QP::Object* Object) {
	if (InputObjectListPtr)
		InputObjectListPtr->emplace_back(Object);
}

void QP::QuadPent_Scene::DeleteInputObject(QP::Object* Object) {
	if (InputObjectListPtr) {
		auto Found = std::find(begin(*InputObjectListPtr), end(*InputObjectListPtr), Object);
		if (Found != end(*InputObjectListPtr))
			InputObjectListPtr->erase(Found);
	}
}

void QP::QuadPent_Scene::SwapLayer(QP::Object* Object, unsigned int TargetLayer) {
	if (TargetLayer > SceneLayer - 2) {
		SetErrorScreen(ERROR_TYPE_OBJECT_SWAP_OUT_OF_SCENE_LAYER, CurrentRunningModeName);
		return;
	}
	Object->SwapCommand = true;
	Object->ObjectLayer = TargetLayer;
}

QP::Object* QP::QuadPent_Scene::Find(std::string Tag) {
	for (int Layer = 0; Layer < SceneLayer - 1; ++Layer) {
		for (auto const& Object : ObjectList[Layer]) {
			if (Object->ObjectTag.compare(Tag) == 0)
				return Object;
		}
	}

	return nullptr;
}

QP::Object* QP::QuadPent_Scene::ReverseFind(std::string Tag) {
	for (int Layer = SceneLayer - 2; Layer >= 0; --Layer) {
		for (auto Object = ObjectList[Layer].rbegin(); Object != ObjectList[Layer].rend(); ++Object) {
			if ((*Object)->ObjectTag.compare(Tag) == 0)
				return *Object;
		}
	}

	return nullptr;
}

QP::Object* QP::QuadPent_Scene::FindMulti(std::string Tag, unsigned int SearchLayer, int Index) {
	if (SearchLayer > SceneLayer - 2) {
		SetErrorScreen(ERROR_TYPE_OBJECT_FIND_OUT_OF_SCENE_LAYER, CurrentRunningModeName);
		return nullptr;
	}

	auto Object = ObjectList[SearchLayer][Index];
	if(Object->ObjectTag.compare(Tag) == 0)
		return ObjectList[SearchLayer][Index];
	
	return nullptr;
}

size_t QP::QuadPent_Scene::LayerSize(unsigned int TargetLayer) {
	if (TargetLayer > SceneLayer - 2) {
		SetErrorScreen(ERROR_TYPE_RETRIEVE_LAYER_SIZE_OUT_OF_SCENE_LAYER, CurrentRunningModeName);
		return 0;
	}

	return ObjectList[TargetLayer].size();
}

void QP::QuadPent_Scene::EraseTag(QP::Object* Object) {
	Object->ObjectTag = "";
}

void QP::QuadPent_Scene::EraseTag(std::string Tag) {
	auto Object = Find(Tag);
	if (Object)
		Object->ObjectTag = "";
}

void QP::QuadPent_Scene::CompleteCommand() {
	if(!CommandExist)
		return;

	UpdateObjectList();
	CommandExist = false;
}

void QP::QuadPent_Scene::SetErrorScreen(int ErrorType, std::string Value1, std::string Value2) {
	if (ErrorOccured)
		return;

	Value1Buffer = Value1;
	Value2Buffer = Value2;
	ErrorTypeBuffer = ErrorType;
	ErrorOccured = true;
	SystemLayerLock = false;
}

QP::Object* QP::QuadPent_Scene::AddSystemObject(QP::Object* Object) {
	if (SystemLayerLock) {
		SetErrorScreen(ERROR_TYPE_PERMISSION_VIOLATION_SYSTEM_LAYER_ACCESS, CurrentRunningModeName);
		return nullptr;
	}

	ObjectList[EOL].emplace_back(Object);

	return Object;
}

void QP::QuadPent_Scene::LockSystemLayer() {
	SystemLayerLock = true;
}

void QP::QuadPent_Scene::UnlockSystemLayer() {
	SystemLayerLock = false;
}

//////// private ///////////////
void QP::QuadPent_Scene::AddLocation(int Layer, int Position) {
	CommandLocation[Layer].emplace_back(Position);
	CommandExist = true;
}

void QP::QuadPent_Scene::UpdateObjectList() {
	int Offset{};

	for (int Layer = 0; Layer < SceneLayer - 1; ++Layer) {
		size_t Size = CommandLocation[Layer].size();
		for (int i = 0; i < Size; ++i) {
			auto Object = begin(ObjectList[Layer]) + CommandLocation[Layer][i] - Offset;

			if ((*Object)->DeleteCommand) {
				delete* Object;
				*Object = nullptr;
				ObjectList[Layer].erase(Object);
			}

			else if ((*Object)->SwapCommand) {
				auto Ptr = (*Object);
				ObjectList[Ptr->ObjectLayer].emplace_back((*Object));
				ObjectList[Layer].erase(Object);
				Ptr->SwapCommand = false;
			}

			++Offset;
		}

		CommandLocation[Layer].clear();
		Offset = 0;
	}
}

void QP::QuadPent_Scene::ClearFloatingObject() {
	for (int Layer = 0; Layer < SceneLayer - 1; ++Layer) {
		size_t Size = LayerSize(Layer);
		for (int i = 0; i < Size; ++i) {
			if (!ObjectList[Layer][i]->StaticCommand && ObjectList[Layer][i]->FloatingCommand) {
				ObjectList[Layer][i]->DeleteCommand = true;
				ObjectList[Layer][i]->ObjectTag = "";
			}
		}
	}
}

void QP::QuadPent_Scene::ClearAll() {
	for (int Layer = 0; Layer < SceneLayer - 1; ++Layer) {
		CommandLocation[Layer].clear();
		size_t Size = LayerSize(Layer);
		for (int i = 0; i < Size; ++i) {
			if (!ObjectList[Layer][i]->StaticCommand) {
				ObjectList[Layer][i]->DeleteCommand = true;
				ObjectList[Layer][i]->ObjectTag = "";
				AddLocation(Layer, i);
			}
		}
	}

	LoopEscapeCommand = true;
}

void ErrorScreenController(unsigned char Key, int X, int Y) {
	if (Key == 27 || Key == 13)
		QP::System.Exit();
}

void QP::QuadPent_Scene::SwitchToErrorScreen() {
	RemoveWindowSubclass(QP::System_HWND, CurrentController, 1);
	glutKeyboardFunc(ErrorScreenController);

	SystemLayerLock = false;

	if (Value2Buffer.empty())
		AddSystemObject(new QuadPent_ErrorMessage(ErrorTypeBuffer, Value1Buffer));
	else
		AddSystemObject(new QuadPent_ErrorMessage(ErrorTypeBuffer, Value1Buffer));
}
