#pragma once
#include "QuadPent_Header.h"
#include "QuadPent_RectBrush.h"
#include "QuadPent_CircleBrush.h"


namespace QP {
	class OOBB;
	class BoundingCircle;

	class AABB {
	private:
		BoundingBox   aabb;
		bool          Collide{};

		QP::LineRectBrush LineRect{ true, true };
		QP::RectBrush     RECT{ true };
		float			   Thickness{ BOUND_BOX_THICKNESS };

	public:
		void Update(float X, float Y, float xScale, float yScale);
		void Update(QP::Vector2& Position, float xScale, float yScale);
		void Render();
		void SetThickness(float Value);
		bool CheckCollision(const AABB& Other);
		bool CheckCollision(const OOBB& Other);
		bool CheckCollision(const BoundingCircle& Other);
		bool CheckCollisionPoint(float X, float Y);
		bool CheckCollisionPoint(QP::Vector2& Position);
		bool CheckCollisionRay(QP::RayVector& Ray);
		bool CheckCollisionLine(QP::RayVector& Ray);
		BoundingBox Get() const;
	};


	class OOBB {
	private:
		BoundingOrientedBox oobb;
		float             Rotation{};
		bool              Collide{};

		QP::LineRectBrush     LineRect{ true, true };
		QP::RectBrush         RECT{ true };
		float				   Thickness{ BOUND_BOX_THICKNESS };

	public:
		void Update(float X, float Y, float BoxWidth, float BoxHeight, float RotationValue);
		void Update(QP::Vector2& Position, float BoxWidth, float BoxHeight, float RotationValue);
		void Render();
		void SetThickness(float Value);
		bool CheckCollision(const OOBB& Other);
		bool CheckCollision(const AABB& Other);
		bool CheckCollision(const BoundingCircle& Other);
		bool CheckCollisionPoint(float X, float Y);
		bool CheckCollisionPoint(QP::Vector2& Position);
		bool CheckCollisionRay(QP::RayVector& Ray);
		bool CheckCollisionLine(QP::RayVector& Ray);
		BoundingOrientedBox Get() const;
	};


	class BoundingCircle {
	private:
		BoundingSphere  sphere;
		glm::vec2       Center{};
		float           Radius{};
		float           Size{};

		bool            Collide{};

		QP::LineCircleBrush LineCircle{ true, true };
		QP::CircleBrush     Circle{ true };
		float                Thickness{ BOUND_BOX_THICKNESS };

	public:
		void Update(float X, float Y, float Diameter);
		void Update(QP::Vector2& Position, float SizeValue);
		void Render();
		void SetThickness(float Value);
		bool CheckCollision(const BoundingCircle& Other);
		bool CheckCollision(const AABB& Other);
		bool CheckCollision(const OOBB& Other);
		bool CheckCollisionPoint(float X, float Y);
		bool CheckCollisionPoint(QP::Vector2& Position);
		bool CheckCollisionRay(QP::RayVector& Ray);
		bool CheckCollisionLine(QP::RayVector& Ray);
		BoundingSphere Get() const;
	};
}