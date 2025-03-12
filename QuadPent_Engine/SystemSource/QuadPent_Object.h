#pragma once
#include "QuadPent_UtilPack.h"

namespace QP {
	class Object {
	public:
		std::string ObjectTag{};
		int         ObjectLayer{};

		bool        FloatingCommand{};
		bool        StaticCommand{};
		bool        DeleteCommand{};
		bool        SwapCommand{};

		////////////////////////////////////////////// Built-in functions
		// class destructor
		virtual ~Object() {}

		// Updates a value inside an object class.
		virtual void UpdateFunc(float FrameTime) {}

		// Renders image resources used by object classes.
		virtual void RenderFunc() {}

		// Sends keyboard events to a specific object class.
		virtual void InputKey(QP::KeyEvent& Event) {}

		// Sends mouse events to a specific object class.
		virtual void InputMouse(int Type) {}

		// Initializes the control state for a specific object class.
		virtual void ResetControlState() {}

		// Obtains an AABB object from a specific object class.
		virtual QP::AABB GetAABB() { return {}; }

		// Obtains an OOBB object from a specific object class.
		virtual QP::OOBB GetOOBB() { return{}; }

		// Obtains an BoundingCircle object from a specific object class.
		virtual QP::BoundingCircle GetBoundingCircle() { return {}; }

		// Obtains an RayVector from a specific object class.
		virtual QP::RayVector GetRayVector() { return {}; }

		////////////////////////////////////////////// Define user functions below

	};
}