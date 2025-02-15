#pragma once
#include "CollisionUtil.h"

class FrustumUtil {
private:
	SDK::OOBB FrustumOOBB{};
	SDK::LineRectBrush LineRect{false, true};

public:
	FrustumUtil();
	void Update();
	bool Check(SDK::AABB& aabb);
	bool Check(SDK::OOBB& oobb);
	bool Check(SDK::BoundingCircle& circle);
};

namespace SDK {
	extern FrustumUtil Frustum;
}