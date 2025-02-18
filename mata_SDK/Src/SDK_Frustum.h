#pragma once
#include "SDK_Collision.h"

namespace MSDK {
	class SDK_Frustum {
	private:
		MSDK::OOBB FrustumOOBB{};
		MSDK::LineRectBrush LineRect{ false, true };

	public:
		SDK_Frustum();
		void Update();
		bool Check(MSDK::AABB& aabb);
		bool Check(MSDK::OOBB& oobb);
		bool Check(MSDK::BoundingCircle& circle);
	};

	extern MSDK::SDK_Frustum Frustum;
}