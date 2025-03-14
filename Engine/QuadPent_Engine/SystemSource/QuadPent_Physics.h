#pragma once
#include "QuadPent_Header.h"

namespace QP {
	class Physics {
	private:
		float Gravity{};
		float Friction{};
		float RebounceReduce{};
		float MinRebounceValue{};

		float FloorHeight{};
		float HeightOffset{};

		float GravityAcc{};
		float MoveAcc{};

		bool    FallingState{};

	public:
		bool GetFallingState();
		void EnableFalling();
		void DisableFalling();
		void AddGravityAcc(float AccValue);
		void SetGravityAcc(float AccValue);
		void SetGravity(float Value);
		void SetFloorHeight(float Value);
		void SetFriction(float Value);
		void SetHeightOffset(float Value);
		void SetRebounceReduce(float Value);
		void SetMinimumRebounce(float Value);
		bool CheckFloorCollision(float& Position);
		bool CheckFloorCollision(QP::Vector2& DestPosition);
		void LandOnFloor(float& Position);
		void LandOnFloor(QP::Vector2& DestPosition);
		void UpdateFalling(float& HeightPosition, float FrameTime);
		void UpdateFalling(QP::Vector2& DestPosition, float FrameTime);
		void UpdateBouncing(float& HeightPosition, float FrameTime);
		void UpdateBouncing(QP::Vector2& DestPosition, float FrameTime);
		void LerpAcceleratation(float& Speed, float DestSpeed, float AccValue, float FT);
		void LerpDeceleration(float& Speed, float FT);
		void LinearAcceleratation(float& Speed, float DestSpeed, float AccValue, float FT);
		void LinearDeceleration(float& Speed, float FT);
		void AdjustSpeedEqual(float& SpeedX, float& SpeedY);
	};
}