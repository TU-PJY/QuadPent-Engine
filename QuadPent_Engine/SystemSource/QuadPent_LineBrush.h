#pragma once
#include "QuadPent_Header.h"

namespace QP {
	class LineBrush {
	private:
		float   Rotation{};
		float   Length{};

		QP::Color3 Color{};
		float   Opacity{};

		int       RenderType{ RENDER_TYPE_DEFAULT };
		int       LineType{ LINE_TYPE_RECT };

		bool      StaticWidthCommand{};

	public:
		LineBrush(bool StaticWidthFlag = false);
		void SetRenderType(int Opt);
		void SetColor(float R, float G, float B);
		void SetColor(QP::Color3& ColorValue);
		void SetColorRGB(int R, int G, int B);
		void SetLineType(int LineTypeOpt);
		void Render(float X1, float Y1, float X2, float Y2, float Thickness, float OpacityValue = 1.0);

	private:
		void ProcessTransform();
		void RenderCircle(float X1, float Y1, float X2, float Y2, float Thickness);
		void ProcessCircleTransform(float Thickness);
	};
}