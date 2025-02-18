#pragma once
#include "SDK_UtilPack.h"

namespace SDK {
	class Object {
	public:
		std::string ObjectTag{};
		int         ObjectLayer{};

		bool        FloatingCommand{};
		bool        StaticCommand{};

		bool        DeleteCommand{};
		bool        SwapCommand{};

		// Initialize matrix. Select a rendering type.
		void Begin(int RenderType = RENDER_TYPE_DEFAULT);

		// Initialize unit matrix.
		void IdentityUnitMatrix();

		// Add color over image
		void SetColor(float R, float G, float B);

		void SetColor(glm::vec3 Color);

		void SetColorRGB(int R, int G, int B);

		// Updates the object's position relative to the viewport. Choose whether to apply aspect ratio.
		void ComputeViewportPosition(float& DestX, float& DestY, bool ApplyAspect = true);

		// Updates the object's position relative to the viewport. Choose whether to apply aspect ratio.
		void ComputeViewportPosition(glm::vec2& DestValue, bool ApplyAspect);

		// Updates the object's position relative to the local coordinate system.
		void ComputeLocalPosition(float& DestX, float& DestY);

		// Updates the object's position relative to the local coordinate system.
		void ComputeLocalPosition(glm::vec2& DestPosition);

		// class destructor
		virtual ~Object() {}

		// Updates a value inside an object class.
		virtual void UpdateFunc(float FrameTime) {}

		// Renders image resources used by object classes.
		virtual void RenderFunc() {}

		// Sends keyboard events to a specific object class.
		virtual void InputKey(SDK::KeyEvent& Event) {}

		// Sends mouse events to a specific object class.
		virtual void InputMouse(int Type) {}

		// Sends mouse scroll events to a specific object class.
		virtual void InputScroll(int Type) {}

		// Initializes the control state for a specific object class.
		virtual void ResetControlState() {}

		// Obtains an AABB object from a specific object class.
		virtual SDK::AABB GetAABB() { return {}; }

		// Obtains an OOBB object from a specific object class.
		virtual SDK::OOBB GetOOBB() { return{}; }

		// Obtains an BoundingCircle object from a specific object class.
		virtual SDK::BoundingCircle GetBoundingCircle() { return {}; }

		// FPS indicator function
		virtual void RenderIndicator() {}

	private:
		glm::vec4 ViewportPosition();
		glm::vec4 LocalPosition();
	};
}