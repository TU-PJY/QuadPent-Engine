#pragma once
#include "QuadPent_Header.h"

namespace QP {
	class SinLoop {
	private:
		float Num{ 0.0 };
		Vector2 Num2{ 0.0, 0.0 };

	public:
		void Update(float& Value, float MoveScale, float Speed, float FrameTime);
		void Update(QP::Vector2& Value, QP::Vector2& MoveScale, QP::Vector2& Speed, float FrameTime);
		void SetValue(float Value);
		void SetValue(QP::Vector2& Value);
		void Reset();
	};

	class SinMove {
	private:
		float Num{QP::Preset::MaxNegative};
		float SingleStartPosition{};
		float SingleDistance{};

		Vector2 StartPosition{};
		Vector2 Distance{};

	public:
		void SetMovePoint(float StartPoint, float EndPoint);
		void SetMovePoint(QP::Vector2& StartPoint, QP::Vector2& EndPoint);
		void SetValue(float Value);
		void Update(float& Value, float Speed, float FrameTime);
		void Update(QP::Vector2& Value, float Speed, float FrameTime);
		void Reset();
	};

	class PopBounce {
	private:
		float Num1{}, Num2{}, Num3{};

	public:
		void Update(float& Value, float SizeDest, float ShakeValue, float SizeIncreaseSpeed, float ShakeSpeed, float ShakeReduceSpeed, float FrameTime);
		void Reset();
	};

	class ReverseLerp {
	private:
		bool  SingleMoveState{ true };
		float SingleVelocity{};
		float SingleAcc{};
		float SingleDistance{};
		float SingleStartPosition{};

		bool MoveState[2] { true, true };
		QP::Vector2 Velocity{};
		QP::Vector2 Acc{};
		QP::Vector2 Distance{};
		QP::Vector2 StartPosition{};

	public:
		void Update(float& Value, float Speed, float IncreaseSpeed, float FrameTime);
		void Update(QP::Vector2& Value, float Speed, float IncreaseSpeed, float FrameTime);
		void SetMovePoint(float StartPoint, float EndPoint);
		void SetMovePoint(QP::Vector2& StartPoint, QP::Vector2& EndPoint);
		void Reset();
	};
}
