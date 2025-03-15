#pragma once
#include "QuadPent_Collision.h"

namespace QP {
	class QuadPent_Frustum {
	private:
		OOBB FrustumOOBB{};
		LineRectBrush LineRect{ false, true };

	public:
		QuadPent_Frustum();
		void Update();
		void Render();
		bool Check(QP::AABB& aabb);
		bool Check(QP::OOBB& oobb);
		bool Check(QP::BoundingCircle& circle);
	};

	extern QuadPent_Frustum Frustum;
}