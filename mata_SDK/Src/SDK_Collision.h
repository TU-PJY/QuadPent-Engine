#pragma once
#include "SDK_Header.h"
#include "SDK_RectBrush.h"
#include "SDK_CircleBrush.h"
using namespace DirectX;


namespace MSDK {
	class OOBB;
	class BoundingCircle;

	class AABB {
	private:
		BoundingBox   aabb;
		bool          Collide{};

		MSDK::LineRectBrush LineRect{ true, true };
		MSDK::RectBrush     RECT{ true };

	public:
		void Update(float X, float Y, float xScale, float yScale);
		void Update(glm::vec2& Position, float xScale, float yScale);
		void Render();
		bool CheckCollision(const AABB& Other);
		bool CheckCollision(const OOBB& Other);
		bool CheckCollision(const BoundingCircle& Other);
		bool CheckCollisionPoint(float X, float Y);
		bool CheckCollisionPoint(glm::vec2& Position);
		bool CheckCollisionRay(MSDK::RayVector& Ray);
		bool CheckCollisionLine(MSDK::RayVector& Ray);
		BoundingBox Get() const;
	};


	class OOBB {
	private:
		BoundingOrientedBox oobb;
		float             Rotation{};
		bool                Collide{};

		MSDK::LineRectBrush       LineRect{ true, true };
		MSDK::RectBrush           RECT{ true };

	public:
		void Update(float X, float Y, float BoxWidth, float BoxHeight, float RotationValue);
		void Update(glm::vec2& Position, float BoxWidth, float BoxHeight, float RotationValue);
		void Render();
		bool CheckCollision(const OOBB& Other);
		bool CheckCollision(const AABB& Other);
		bool CheckCollision(const BoundingCircle& Other);
		bool CheckCollisionPoint(float X, float Y);
		bool CheckCollisionPoint(glm::vec2& Position);
		bool CheckCollisionRay(MSDK::RayVector& Ray);
		bool CheckCollisionLine(MSDK::RayVector& Ray);
		BoundingOrientedBox Get() const;
	};


	class BoundingCircle {
	private:
		BoundingSphere  sphere;
		glm::vec2       Center{};
		float         Radius{};
		float         Size{};

		bool            Collide{};

		MSDK::LineCircleBrush LineCircle{ true, true };
		MSDK::CircleBrush     Circle{ true };

	public:
		void Update(float X, float Y, float Diameter);
		void Update(glm::vec2& Position, float SizeValue);
		void Render();
		bool CheckCollision(const BoundingCircle& Other);
		bool CheckCollision(const AABB& Other);
		bool CheckCollision(const OOBB& Other);
		bool CheckCollisionPoint(float X, float Y);
		bool CheckCollisionPoint(glm::vec2& Position);
		bool CheckCollisionRay(MSDK::RayVector& Ray);
		bool CheckCollisionLine(MSDK::RayVector& Ray);
		BoundingSphere Get() const;
	};
}