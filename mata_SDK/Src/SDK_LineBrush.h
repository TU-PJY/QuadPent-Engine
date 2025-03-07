#pragma once
#include "SDK_Header.h"

namespace SDK {
	class LineBrush {
	private:
		float   Rotation{};
		float   Length{};

		SDK::Color3 Color{};
		float   Opacity{};

		int       RenderType{ RENDER_TYPE_DEFAULT };
		int       LineType{ LINE_TYPE_RECT };

		bool      StaticWidthCommand{};

	public:
		LineBrush(bool StaticWidthFlag = false);
		void SetRenderType(int Opt);
		void SetColor(float R, float G, float B);
		void SetColor(SDK::Color3& ColorValue);
		void SetColorRGB(int R, int G, int B);
		void SetLineType(int LineTypeOpt);
		void Draw(float X1, float Y1, float X2, float Y2, float Thickness, float OpacityValue = 1.0);

	private:
		void Render();
		void DrawCircle(float X1, float Y1, float X2, float Y2, float Thickness);
		void RenderCircle(float Thickness);
	};
}