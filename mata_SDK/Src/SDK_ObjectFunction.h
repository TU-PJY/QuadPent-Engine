#pragma once
#include "SDK_Header.h"

namespace SDK {
	// Reset local matrix and attributes.
	void Begin(int RenderType=RENDER_TYPE_DEFAULT);

	// Reset global matrix and attributes.
	void ResetGlobalAttribute();

	// Get object's position based on viewport coordinate system.
	void GetViewportPosition(SDK::Vector2& DestPosition);
	void GetViewportPosition(float& DestX, float& DestY);

	// Get object's position based on local coordinate system.
	void GetLocalPosition(SDK::Vector2& DestPosition);
	void GetLocalPosition(float& DestX, float& DestY);
}