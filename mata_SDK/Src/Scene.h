#pragma once
#include "Object.h"
#include <algorithm>
#include <array>
#include <deque>
#include <vector>

typedef void(*Function)(void);
typedef void(*ControllerFunction)(void);
constexpr int SceneLayer = EOL;

class SDK_Scene {
private:
	std::array<std::deque<SDK::Object*>, SceneLayer> ObjectList{};
	std::array<std::vector<int>, SceneLayer>        DeleteLocation{};

	int                               CurrentReferLocation{};
	bool                              CommandExist{};

	std::string						  CurrentRunningMode{};
	std::string						  PrevRunningMode{};

	bool							  FloatingActivateCommand{};
	bool                              FloatingFocusCommand{};

	float							  FrameTime{};

	CONTROLLER_PTR				      ControllerBuffer{};
	MODE_PTR						  DestructorBuffer{};

	bool                              UpdateActivateCommand{ true };
	bool                              LoopEscapeCommand{};

	bool                              ErrorOccured{};
	bool                              ErrorScreenState{};
	int								  ErrorTypeBuffer{};
	std::string						  Value1Buffer{};
	std::string						  Value2Buffer{};

	std::vector<SDK::Object*>* InputObjectListPtr{};

public:
	// Returns the name of the currently running mode.
	std::string Mode();

	//  Stop updating the scene.
	void Stop();

	// Resume scene updates.
	// It cannot be used when an error occurs.
	void Resume();

	// Initialize the scene and enter start mode.
	void Init(MODE_PTR ModeFunction);

	// Register the mode name to run.
	void RegisterModeName(std::string ModeName);

	// Register a mode destructor with the Scene.
	void RegisterDestructor(MODE_PTR DestructorFunction);

	// Removes the mode destructor registered with the Scene.
	void ReleaseDestructor();

	// Register the controller in the scene.
	// When MODE_TYPE_FLOATING is specified, the controller is not stored in the controller buffer.
	void RegisterController(CONTROLLER_PTR Controller, int Type);

	// Register the mode controller input object list in Scene.
	void RegisterInputObjectList(std::vector<SDK::Object*>& Vec);

	// Enter the frame time in Scene.
	void InputFrameTime(float ElapsedTime);

	// Executes update of the scene.
	void Update();

	// Executes rendering of the scene.
	void Render();

	// Switch to a specific mode.
	void SwitchMode(MODE_PTR ModeFunction);

	// Start floating mode.Existing objects are not deleted.
	// When true is specified for FloatingFocus, only floating objects are updated. This state is cleared when floating mode ends.
	void StartFloatingMode(MODE_PTR ModeFunction, bool FloatingFocusFlag=false);

	// Exit floating mode.Floating objects are deleted, and regular objects are not deleted.
	void EndFloatingMode();
	
	// Add an object to the scene. You can give two options to an object.
	// OBJECT_TYPE_STATIC: It will not be deleted even if the mode is changed.
	// OBJECT_TYPE_FLOATING: Specify it as a floating mode object.
	void AddObject(SDK::Object* Object, std::string Tag, int AddLayer, int Type1= OBJECT_TYPE_NONE, int Type2=OBJECT_TYPE_NONE);

	// Deletes an object from the Scene.
	// If the object is located on a layer that is not currently being referenced, activates DeleteReserveCommand.
	void DeleteObject(SDK::Object* Object);

	// Deletes an object from the Scene.
	// The DELETE_RANGE_SINGLE option is recommended when the target object is guaranteed to exist as a single object.
	void DeleteObject(std::string Tag, int deleteRange);
	
	// Adds an object to the mode controller input object list.
	void AddInputObject(SDK::Object* Object);

	// Deletes an object from the mode controller input object list.
	void DeleteInputObject(SDK::Object* Object);

	//  Change the layer where the object is located.
	void SwapLayer(SDK::Object* Object, int TargetLayer);

	// Gets a pointer to a specific object that exists in the Scene. Returns nullptr for objects that do not exist.
	SDK::Object* Find(std::string Tag);

	// Gets a pointer to a specific object that exists in the Scene. Returns nullptr for objects that do not exist.
	SDK::Object* ReverseFind(std::string Tag);

	// Gets a pointer to multiple objects with a specific tag that exist in the scene. Returns nullptr for objects that do not exist.
	// You need to find the number of objects that exist in a specific layer and then access it using the for statement.
	SDK::Object* FindMulti(std::string Tag, int SearchLayer, int Index);


	// Removes the object tag for a specific object.
	void DeleteTag(SDK::Object* Object);

	void DeleteTag(std::string Tag);

	// 	Returns the number of objects present in a specific Scene layer.
	size_t LayerSize(int TargetLayer);

	// Checks the state of an object and takes action appropriate to the state.
	void CompleteCommand();

	// When an error occurs, it switches to the error screen and stops the system.
	void SetErrorScreen(int ErrorType, std::string Value1, std::string Value2="");

private:
	void AddLocation(int Layer, int Position);
	void UpdateObjectList();
	void ClearFloatingObject();
	void ClearAll();
	void SwitchToErrorScreen();
};

namespace SDK {
	extern SDK_Scene Scene;
}