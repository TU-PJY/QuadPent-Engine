#include "SDK_Scene.h"
#include "SDK_ErrorMessage.h"
#include "SDK_SoundTool.h"

MSDK::SDK_Scene MSDK::Scene;

void MSDK::SDK_Scene::InputFrameTime(float ElapsedTime) {
	FrameTime = ElapsedTime;
}

std::string MSDK::SDK_Scene::ModeName() {
	return CurrentRunningModeName;
}

bool MSDK::SDK_Scene::CheckCurrentMode(MSDK::MODE_PTR ModePtr) {
	if (ModePtr == CurrentRunningModePtr)
		return true;
	return false;
}

MSDK::MODE_PTR MSDK::SDK_Scene::Mode() {
	return CurrentRunningModePtr;
}

void MSDK::SDK_Scene::Stop() {
	UpdateActivateCommand = false;
}

void MSDK::SDK_Scene::Resume() {
	if (!ErrorOccured)
		UpdateActivateCommand = true;
}

void MSDK::SDK_Scene::Update() {
	CurrentRunningProcess = SCENE_UPDATE_PROCESS;

	if (!ErrorScreenState && ErrorOccured) {
		MSDK::SoundTool.StopAllSounds();
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

			if (LoopEscapeCommand || ErrorOccured)
				return;

			if (Object->DeleteCommand)
				AddLocation(Layer, CurrentReferLocation);

			else if (Object->SwapCommand)
				AddLocation(Layer, CurrentReferLocation);

			++CurrentReferLocation;
		}
		CurrentReferLocation = 0;
	}
}

void MSDK::SDK_Scene::Render() {
	CurrentRunningProcess = SCENE_RENDER_PROCESS;

	if (LoopEscapeCommand) {
		LoopEscapeCommand = false;
		return;
	}

	for (int i = 0; i < SceneLayer; ++i) {
		for (auto& Object : ObjectList[i]) {
			if (!Object->DeleteCommand) 
				Object->RenderFunc();
		}
	}
}

void MSDK::SDK_Scene::Init(MSDK::MODE_PTR ModeFunction) {
	ModeFunction();
	for (int Layer = 0; Layer < SceneLayer; ++Layer)
		DeleteLocation[Layer].reserve(DELETE_LOCATION_BUFFER_SIZE);
}

void MSDK::SDK_Scene::SwitchMode(MSDK::MODE_PTR ModeFunction) {
	if (CurrentRunningProcess == SCENE_RENDER_PROCESS)
		return;

	ClearAll();

	if (DestructorBuffer)
		DestructorBuffer();

	ModeFunction();

	if (FloatingActivateCommand) {
		FloatingActivateCommand = false;
		FloatingFocusCommand = false;
	}

	LoopEscapeCommand = true;
}

void MSDK::SDK_Scene::RegisterDestructor(MSDK::MODE_PTR DestructorFunction) {
	DestructorBuffer = DestructorFunction;
}

void MSDK::SDK_Scene::RegisterModeName(std::string ModeName) {
	CurrentRunningModeName = ModeName;
}

void MSDK::SDK_Scene::RegisterModePtr(MSDK::MODE_PTR ModePtr) {
	CurrentRunningModePtr = ModePtr;
}

void MSDK::SDK_Scene::RegisterController(MSDK::CONTROLLER_PTR Controller, int Type) {
	Controller();
	if (Type == MODE_TYPE_DEFAULT)
		ControllerBuffer = Controller;
}

void MSDK::SDK_Scene::RegisterInputObjectList(std::vector<MSDK::Object*>& Vec) {
	InputObjectListPtr = &Vec;
}

void MSDK::SDK_Scene::ReleaseDestructor() {
	DestructorBuffer = nullptr;
}

void MSDK::SDK_Scene::StartFloatingMode(MSDK::MODE_PTR ModeFunction, bool FloatingFocusFlag) {
	if (FloatingActivateCommand)
		return;

	PrevRunningModeName = CurrentRunningModeName;
	PrevRunningModePtr = CurrentRunningModePtr;
	ModeFunction();
	FloatingFocusCommand = FloatingFocusFlag;

	FloatingActivateCommand = true;
}

void MSDK::SDK_Scene::EndFloatingMode() {
	if (!FloatingActivateCommand)  
		return;

	ClearFloatingObject();
	CurrentRunningModeName = PrevRunningModeName;
	CurrentRunningModePtr = PrevRunningModePtr;

	if (ControllerBuffer)  
		ControllerBuffer();

	FloatingActivateCommand = false;
	FloatingFocusCommand = false;
}

MSDK::Object* MSDK::SDK_Scene::AddObject(MSDK::Object* Object, std::string Tag, int AddLayer, int Type1, int Type2) {
	if (AddLayer > SceneLayer)
		return nullptr;

	if (Type1 == OBJECT_TYPE_STATIC_SINGLE || Type2 == OBJECT_TYPE_STATIC_SINGLE) {
		if (auto Object = Find(Tag); Object)
			return nullptr;
	}

	ObjectList[AddLayer].emplace_back(Object);

	Object->ObjectTag = Tag;
	Object->ObjectLayer = AddLayer;

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

void MSDK::SDK_Scene::DeleteObject(MSDK::Object* Object) {
	Object->DeleteCommand = true;
	Object->ObjectTag = "";
}

void MSDK::SDK_Scene::DeleteObject(std::string Tag, int DeleteRange) {
	if (DeleteRange == DELETE_RANGE_SINGLE) {
		for (int Layer = 0; Layer < SceneLayer; ++Layer) {
			for (auto const& Object : ObjectList[Layer]) {
				if (Object->ObjectTag == Tag) {
					Object->DeleteCommand = true;
					Object->ObjectTag = "";
					return;
				}
			}
		}
	}

	else if (DeleteRange == DELETE_RANGE_ALL) {
		for (int Layer = 0; Layer < SceneLayer; ++Layer) {
			for (auto const& Object : ObjectList[Layer]) {
				if (Object->ObjectTag == Tag) {
					Object->DeleteCommand = true;
					Object->ObjectTag = "";
				}
			}
		}
	}
}

void MSDK::SDK_Scene::AddInputObject(MSDK::Object* Object) {
	if (InputObjectListPtr)
		InputObjectListPtr->emplace_back(Object);
}

void MSDK::SDK_Scene::DeleteInputObject(MSDK::Object* Object) {
	if (InputObjectListPtr) {
		auto Found = std::find(begin(*InputObjectListPtr), end(*InputObjectListPtr), Object);
		if (Found != end(*InputObjectListPtr))
			InputObjectListPtr->erase(Found);
	}
}

void MSDK::SDK_Scene::SwapLayer(MSDK::Object* Object, int TargetLayer) {
	Object->SwapCommand = true;
	Object->ObjectLayer = TargetLayer;
}

MSDK::Object* MSDK::SDK_Scene::Find(std::string Tag) {
	for (int Layer = 0; Layer < SceneLayer; ++Layer) {
		for (auto const& Object : ObjectList[Layer]) {
			if (Object->ObjectTag == Tag)
				return Object;
		}
	}

	return nullptr;
}

MSDK::Object* MSDK::SDK_Scene::ReverseFind(std::string Tag) {
	for (int Layer = SceneLayer - 1; Layer > 0; --Layer) {
		for (auto Object = ObjectList[Layer].rbegin(); Object != ObjectList[Layer].rend(); ++Object) {
			if ((*Object)->ObjectTag == Tag)
				return *Object;
		}
	}

	return nullptr;
}

MSDK::Object* MSDK::SDK_Scene::FindMulti(std::string Tag, int SearchLayer, int Index) {
	auto Object = ObjectList[SearchLayer][Index];
	if(Object->ObjectTag == Tag)
		return ObjectList[SearchLayer][Index];
	
	return nullptr;
}

size_t MSDK::SDK_Scene::LayerSize(int TargetLayer) {
	return ObjectList[TargetLayer].size();
}

void MSDK::SDK_Scene::DeleteTag(MSDK::Object* Object) {
	Object->ObjectTag = "";
}

void MSDK::SDK_Scene::DeleteTag(std::string Tag) {
	auto Object = Find(Tag);
	if (Object)
		Object->ObjectTag = "";
}

void MSDK::SDK_Scene::CompleteCommand() {
	if(!CommandExist)
		return;

	UpdateObjectList();
	CommandExist = false;
}

void MSDK::SDK_Scene::SetErrorScreen(int ErrorType, std::string Value1, std::string Value2) {
	Value1Buffer = Value1;
	Value2Buffer = Value2;
	ErrorTypeBuffer = ErrorType;
	ErrorOccured = true;
}

//////// private ///////////////
void MSDK::SDK_Scene::AddLocation(int Layer, int Position) {
	DeleteLocation[Layer].emplace_back(Position);
	CommandExist = true;
}

void MSDK::SDK_Scene::UpdateObjectList() {
	int Offset{};

	for (int Layer = 0; Layer < SceneLayer; ++Layer) {
		size_t Size = DeleteLocation[Layer].size();
		if (Size == 0)
			continue;

		for (int i = 0; i < Size; ++i) {
			auto Object = begin(ObjectList[Layer]) + DeleteLocation[Layer][i] - Offset;

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

		DeleteLocation[Layer].clear();
		Offset = 0;
	}
}

void MSDK::SDK_Scene::ClearFloatingObject() {
	for (int Layer = 0; Layer < SceneLayer; ++Layer) {
		size_t Size = LayerSize(Layer);
		for (int i = 0; i < Size; ++i) {
			if (!ObjectList[Layer][i]->StaticCommand && ObjectList[Layer][i]->FloatingCommand) {
				ObjectList[Layer][i]->DeleteCommand = true;
				ObjectList[Layer][i]->ObjectTag = "";
				AddLocation(Layer, i);
			}
		}
	}

	LoopEscapeCommand = true;
}

void MSDK::SDK_Scene::ClearAll() {
	for (int Layer = 0; Layer < SceneLayer; ++Layer) {
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

void MSDK::SDK_Scene::SwitchToErrorScreen() {
	glutMotionFunc(nullptr);
	glutPassiveMotionFunc(nullptr);
	glutKeyboardFunc(nullptr);
	glutKeyboardUpFunc(nullptr);
	glutMouseWheelFunc(nullptr);
	glutMouseFunc(nullptr);
	glutSpecialFunc(nullptr);
	glutSpecialUpFunc(nullptr);

	if (Value2Buffer.empty())
		AddObject(new SDK_ErrorMessage(ErrorTypeBuffer, Value1Buffer), "error_message", EOL - 1);
	else
		AddObject(new SDK_ErrorMessage(ErrorTypeBuffer, Value1Buffer, Value2Buffer), "error_message", EOL - 1);
}
