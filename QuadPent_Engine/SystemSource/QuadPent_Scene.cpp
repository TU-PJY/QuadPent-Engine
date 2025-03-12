#include "QuadPent_Scene.h"
#include "QuadPent_ErrorMessage.h"
#include "QuadPent_SoundTool.h"

QP::QuadPent_Scene QP::Scene;

void QP::QuadPent_Scene::InputFrameTime(float ElapsedTime) {
	FrameTime = ElapsedTime;
}

std::string QP::QuadPent_Scene::CurrentModeName() {
	return CurrentRunningModeName;
}

bool QP::QuadPent_Scene::CheckCurrentMode(QP::Mode ModePtr) {
	if (ModePtr == CurrentRunningModePtr)
		return true;
	return false;
}

QP::Mode QP::QuadPent_Scene::CurrentMode() {
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

void QP::QuadPent_Scene::Init(QP::Mode ModePtr) {
	ModePtr();

	for (int Layer = 0; Layer < SceneLayer; ++Layer)
		CommandLocation[Layer].reserve(COMMAND_LOCATION_BUFFER_SIZE);

	SetWindowSubclass(QP::System_HWND, Controller, 1, 0);
}

void QP::QuadPent_Scene::SwitchMode(QP::Mode ModePtr, int SwitchOption) {
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

void QP::QuadPent_Scene::RegisterDestructor(QP::Mode ModeDestructorPtr) {
	DestructorBuffer = ModeDestructorPtr;
}

void QP::QuadPent_Scene::RegisterModeName(std::string ModeName) {
	CurrentRunningModeName = ModeName;
}

void QP::QuadPent_Scene::RegisterModePtr(QP::Mode ModePtr) {
	CurrentRunningModePtr = ModePtr;
}

void QP::QuadPent_Scene::RegisterInputObjectList(std::vector<QP::Object*>& List, int ModeType) {
	InputObjectListPtr = &List;
	if (ModeType == MODE_TYPE_DEFAULT)
		InputObjectListPtrBuffer = InputObjectListPtr;
}

void QP::QuadPent_Scene::StartFloatingMode(QP::Mode ModePtr, bool FloatingFocusFlag) {
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

	if(InputObjectListPtr)
		InputObjectListPtr = InputObjectListPtrBuffer;

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

	if (UseController) {
		Object->HasController = true;
		InputObjectListPtr->emplace_back(Object);
	}

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
	CheckObjectHasController(Object);
	Object->DeleteCommand = true;
	Object->ObjectTag = "";
}

void QP::QuadPent_Scene::DeleteObject(std::string Tag, int DeleteRange) {
	switch (DeleteRange) {
	case DELETE_RANGE_SINGLE:
		for (int Layer = 0; Layer < SceneLayer - 1; ++Layer) {
			for (auto const& Object : ObjectList[Layer]) {
				if (Object->ObjectTag.compare(Tag) == 0) {
					CheckObjectHasController(Object);
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
					CheckObjectHasController(Object);
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

	return ObjectList[EOL].back();
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
				CheckObjectHasController(ObjectList[Layer][i]);
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
				CheckObjectHasController(ObjectList[Layer][i]);
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
	RemoveWindowSubclass(QP::System_HWND, Controller, 1);
	glutKeyboardFunc(ErrorScreenController);

	SystemLayerLock = false;

	if (Value2Buffer.empty())
		AddSystemObject(new QuadPent_ErrorMessage(ErrorTypeBuffer, Value1Buffer));
	else
		AddSystemObject(new QuadPent_ErrorMessage(ErrorTypeBuffer, Value1Buffer));
}

void QP::QuadPent_Scene::CheckObjectHasController(QP::Object* Object) {
	if (Object->HasController)
		DeleteInputObject(Object);
}

LRESULT CALLBACK QP::QuadPent_Scene::Controller(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam, UINT_PTR SubClassID, DWORD_PTR RefData) {
	if (!Scene.InputObjectListPtr)
		return DefSubclassProc(Hwnd, Message, wParam, lParam);

	int ProcEvent{};
	int WheelDelta{};
	int MouseEvent{ EVENT_NONE };

	switch (Message) {
	case WM_LBUTTONDOWN:
		MouseEvent = LEFT_BUTTON_DOWN; break;
	case WM_RBUTTONDOWN:
		MouseEvent = RIGHT_BUTTON_DOWN; break;
	case WM_MBUTTONDOWN:
		MouseEvent = MIDDLE_BUTTON_DOWN; break;
	case WM_LBUTTONUP:
		MouseEvent = LEFT_BUTTON_UP; break;
	case WM_RBUTTONUP:
		MouseEvent = RIGHT_BUTTON_UP; break;
	case WM_MBUTTONUP:
		MouseEvent = MIDDLE_BUTTON_UP; break;

	case WM_XBUTTONDOWN:
		ProcEvent = GET_XBUTTON_WPARAM(wParam);
		if (ProcEvent == XBUTTON1)
			MouseEvent = BACKWARD_BUTTON_DOWN;
		else if (ProcEvent == XBUTTON2)
			MouseEvent = FORWARD_BUTTON_DOWN;
		break;

	case WM_XBUTTONUP:
		ProcEvent = GET_XBUTTON_WPARAM(wParam);
		if (ProcEvent == XBUTTON1)
			MouseEvent = BACKWARD_BUTTON_UP;
		else if (ProcEvent == XBUTTON2)
			MouseEvent = FORWARD_BUTTON_UP;
		break;

	case WM_MOUSEWHEEL:
		WheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		if (WheelDelta > 0)
			MouseEvent = WHEEL_UP;
		else if (WheelDelta < 0)
			MouseEvent = WHEEL_DOWN;
		break;

	case WM_KEYDOWN: case WM_KEYUP: case WM_CHAR:
		if (ENABLE_DEV_EXIT && wParam == VK_ESCAPE)
			QP::System.Exit();

		QP::KeyEvent Event{};
		Event.Type = Message;
		Event.Key = wParam;

		for (auto const& Object : *Scene.InputObjectListPtr)
			if (Object)  Object->InputKey(Event);
		
		return DefSubclassProc(Hwnd, Message, wParam, lParam);
		break;
	}

	if (MouseEvent != EVENT_NONE) {
		for (auto const& Object : *Scene.InputObjectListPtr)
			if (Object)  Object->InputMouse(MouseEvent);
	}

	return DefSubclassProc(Hwnd, Message, wParam, lParam);
}
