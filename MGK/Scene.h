#pragma once
#include "GameObject.h"
#include <algorithm>
#include <array>
#include <map>
#include <deque>
#include <vector>

typedef void(*Function)(void);
typedef void(*ControllerFunction)(void);
constexpr int Layers = END;

using ObjectRange    = std::multimap<std::string, GameObject*>::iterator;

class Scene {
private:
	std::array<std::deque<GameObject*>, Layers> ObjectList{};
	std::multimap<std::string, GameObject*>     ObjectIndex{};
	std::array<std::vector<int>, Layers>        DeleteLocation{};

	int                               CurrentReferLocation{};
	int                               SceneCommandCount{};
	bool                              CommandExist{};

	std::string						  CurrentRunningMode{};
	std::string						  PrevRunningMode{};

	bool							  FloatingActivateCommand{};
	bool                              FloatingFocusCommand{};

	float							  FrameTime{};

	ControllerFunction				  ControllerBuffer{};
	Function						  DestructorBuffer{};

	bool                              UpdateActivateCommand{ true };

public:
	// Returns the name of the currently running mode.
	std::string Mode();

	//  Stop updating the scene.
	void Stop();

	// Resume scene updates.
	void Resume();

	// Register the mode name to run.
	void RegisterModeName(std::string ModeName);

	// Initialize the scene and enter start mode.
	void Init(Function ModeFunction);

	// Register the controller in the scene.
	// When MODE_TYPE_FLOATING is specified, the controller is not stored in the controller buffer.
	void RegisterController(ControllerFunction Controller, int Type);

	// Enter the frame time in Scene.
	void InputFrameTime(float ElapsedTime);

	// Executes update and rendering of the scene.
	void Routine();

	// Switch to a specific mode.
	void SwitchMode(Function ModeFunction);

	// Register a mode destructor with the Scene.
	void RegisterDestructor(Function DestructorFunction);

	// Removes the mode destructor registered with the Scene.
	void ReleaseDestructor();

	// Start floating mode.Existing objects are not deleted.
	// When true is specified for FloatingFocus, only floating objects are updated. This state is cleared when floating mode ends.
	void StartFloatingMode(Function ModeFunction, bool FloatingFocusFlag=false);

	// Exit floating mode.Floating objects are deleted, and regular objects are not deleted.
	void EndFloatingMode();
	
	// Initializes the control state of a specific object.
	void ResetControlState(GameObject* Object);

	// Initializes the control state of a specific object.
	void ResetControlState(std::string Tag);

	// Sends keyboard events to a specific object class.
	void InputKey(std::string Tag, int State, unsigned char NormalKey, int SpecialKey);

	// Sends mouse events to a specific object class.
	void InputMouse(std::string Tag, int State);

	// Sends mouse scroll events to a specific object class.
	void InputScroll(std::string Tag, int State);

	// Add an object to the scene. You can give two options to an object.
	// OBJECT_TYPE_STATIC: It will not be deleted even if the mode is changed.
	// OBJECT_TYPE_FLOATING: Specify it as a floating mode object.
	void AddObject(GameObject* Object, std::string Tag, int AddLayer, int Type1= OBJECT_TYPE_NONE, int Type2=OBJECT_TYPE_NONE);

	//  Change the layer where the object is located.
	void SwapLayer(GameObject* Object, int TargetLayer);

	// Deletes an object from the Scene.
	// If the object is located on a layer that is not currently being referenced, activates DeleteReserveCommand.
	void DeleteObject(GameObject* Object);

	// Deletes an object from the Scene.
	void DeleteObject(std::string Tag, int deleteRange);

	// Gets a pointer to a specific object that exists in the Scene. Returns nullptr for objects that do not exist.
	GameObject* Find(std::string Tag);

	// Gets a pointer to multiple objects with a specific tag that exist in the scene. Returns nullptr for objects that do not exist.
	// You need to find the number of objects that exist in a specific layer and then access it using the for statement.
	GameObject* FindMulti(std::string Tag, int SearchLayer, int Index);

	// Gets a range of multiple objects with a specific tag that exist in the Scene. The range can be accessed using the for statement.
	std::pair<ObjectRange, ObjectRange> EqualRange(std::string Tag);

	// 	Returns the number of objects present in a specific Scene layer.
	size_t LayerSize(int TargetLayer);

	void CompleteCommand();

private:
	void AddLocation(int Layer, int Position);
	void ProcessObjectCommand();
	void ProcessSceneCommand();
	void ClearFloatingObject();
	void ClearAll();
};
extern Scene scene;