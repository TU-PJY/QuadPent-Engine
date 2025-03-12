#pragma once
#include "QuadPent_Object.h"

typedef void(*Function)(void);
typedef void(*ControllerFunction)(void);
constexpr int SceneLayer = EOL + 1;

namespace QP {
	class QuadPent_Scene {
	private:
		std::array<std::deque<QP::Object*>, SceneLayer> ObjectList{};
		std::array<std::vector<int>, SceneLayer>        CommandLocation{};

		std::vector<QP::Object*>*		  InputObjectListPtr{};
		std::vector<QP::Object*>*		  InputObjectListPtrBuffer{};

		int                               CurrentReferLocation{};
		bool                              CommandExist{};

		std::string						  CurrentRunningModeName{};
		std::string						  PrevRunningModeName{};

		QP::Mode						  CurrentRunningModePtr{};
		QP::Mode					   	  PrevRunningModePtr{};

		bool							  FloatingActivateCommand{};
		bool                              FloatingFocusCommand{};

		float							  FrameTime{};

		QP::Mode				          DestructorBuffer{};

		bool                              UpdateActivateCommand{ true };
		bool                              LoopEscapeCommand{};

		bool                              ErrorOccured{};
		bool                              ErrorScreenState{};
		int								  ErrorTypeBuffer{};
		std::string						  Value1Buffer{};
		std::string						  Value2Buffer{};

		bool                              SystemLayerLock{};

	public:
		// Returns the name of the currently running mode.
		std::string CurrentModeName();

		// Check whether the mode pointer is the current running mode. Returns true if there is a match.
		bool CheckCurrentMode(QP::Mode ModePtr);

		// Return current running mode's pointer.
		QP::Mode CurrentMode();

		// Stop updating the scene.
		void Stop();

		// Resume scene updates.
		// It cannot be used when an error occurs.
		void Resume();

		// Initialize the scene and enter start mode.
		void Init(QP::Mode ModePtr);

		// Register the mode name.
		void RegisterModeName(std::string ModeName);

		// Register the mode poiner to run.
		void RegisterModePtr(QP::Mode ModePtr);

		// Register a mode destructor with the Scene.
		void RegisterDestructor(QP::Mode ModeDestructorPtr);

		// Register the mode controller input object list in Scene.
		// If ModeType is MODE_TYPE_DEFAULT, input object buffer is saved.
		void RegisterInputObjectList(std::vector<QP::Object*>& List, int ModeType);

		// Enter the frame time in Scene.
		void InputFrameTime(float ElapsedTime);

		// Executes update of the scene.
		void Update();

		// Executes rendering of the scene.
		void Render();

		// Switch to a specific mode.
		void SwitchMode(QP::Mode ModePtr, int SwitchOption=MODE_SWITCH_DEFAULT);

		// Start floating mode.Existing objects are not deleted.
		// When true is specified for FloatingFocus, only floating objects are updated. This state is cleared when floating mode ends.
		void StartFloatingMode(QP::Mode ModePtr, bool FloatingFocusFlag = false);

		// Exit floating mode.Floating objects are deleted, and regular objects are not deleted.
		void EndFloatingMode();

		// Add an object to the scene. You can give two options to an object.
		// OBJECT_TYPE_STATIC: It will not be deleted even if the mode is changed.
		// OBJECT_TYPE_FLOATING: Specify it as a floating mode object.
		QP::Object* AddObject(QP::Object* Object, std::string Tag, unsigned int AddLayer, bool UseController=false, int Type1 = OBJECT_TYPE_NONE, int Type2 = OBJECT_TYPE_NONE);

		// Deletes an object from the Scene.
		// If the object is located on a layer that is not currently being referenced, activates DeleteReserveCommand.
		void DeleteObject(QP::Object* Object);

		// Deletes an object from the Scene.
		// The DELETE_RANGE_SINGLE option is recommended when the target object is guaranteed to exist as a single object.
		void DeleteObject(std::string Tag, int deleteRange);

		// Adds an object to the mode controller input object list.
		void AddInputObject(QP::Object* Object);

		// Deletes an object from the mode controller input object list.
		void DeleteInputObject(QP::Object* Object);

		//  Change the layer where the object is located.
		void SwapLayer(QP::Object* Object, unsigned int TargetLayer);

		// Gets a pointer to a specific object that exists in the Scene. Returns nullptr for objects that do not exist.
		QP::Object* Find(std::string Tag);

		// Gets a pointer to a specific object that exists in the Scene. Returns nullptr for objects that do not exist.
		QP::Object* ReverseFind(std::string Tag);

		// Gets a pointer to multiple objects with a specific tag that exist in the scene. Returns nullptr for objects that do not exist.
		// You need to find the number of objects that exist in a specific layer and then access it using the for statement.
		QP::Object* FindMulti(std::string Tag, unsigned int SearchLayer, int Index);

		// Removes the object tag for a specific object.
		void EraseTag(QP::Object* Object);
		void EraseTag(std::string Tag);

		// 	Returns the number of objects present in a specific Scene layer.
		size_t LayerSize(unsigned int TargetLayer);

		void CompleteCommand();
		void SetErrorScreen(int ErrorType, std::string Value1, std::string Value2 = "");
		QP::Object* AddSystemObject(QP::Object* Object);
		void LockSystemLayer();
		void UnlockSystemLayer();

		static LRESULT Controller(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam, UINT_PTR SubClassID, DWORD_PTR RefData);

	private:
		void AddLocation(int Layer, int Position);
		void UpdateObjectList();
		void ClearFloatingObject();
		void ClearAll();
		void SwitchToErrorScreen();
		void CheckObjectHasController(QP::Object* Object);
	};

	extern QuadPent_Scene Scene;
}