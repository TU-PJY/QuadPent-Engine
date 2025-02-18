#pragma once
#include "SDK_Header.h"
#include <cmath>

namespace MSDK {
	class SinLoop {
	private:
		float Num{ MSDK::Preset::MaxNegative };

	public:
		void Update(float& Value, float MoveScale, float Speed, float FrameTime);
		void SetValue(float Value);
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
		bool    MoveState{ true };
		float Velocity{};
		float Acc{};

	public:
		void Update(float& Value, float Dest, float Speed, float IncreaseSpeed, float FrameTime);
		void Reset();
	};
}
