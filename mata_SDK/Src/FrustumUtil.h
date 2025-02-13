#pragma once
#include "CollisionUtil.h"

class FrustumUtil {
private:
	OOBB FrustumOOBB{};
	LineRectBrush LineRect{};

public:
	FrustumUtil();
	void Update();
	bool Check(AABB& aabb);
	bool Check(OOBB& oobb);
	bool Check(BoundingCircle& circle);
};
extern FrustumUtil frustum;