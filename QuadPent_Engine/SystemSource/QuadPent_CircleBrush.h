#pragma once
#include "QuadPent_Header.h"

namespace QP {
	class CircleBrush {
	private:
		float   Radius{};

		Color3    Color{};
		float     Opacity{};

		int       RenderType{ RENDER_TYPE_DEFAULT };

		bool      CamInheritanceCommand{};

	public:
		CircleBrush(bool CamInheritanceFlag = false);
		void SetColor(float R, float G, float B);
		void SetColor(QP::Color3& ColorValue);
		void SetColorRGB(int R, int G, int B);
		void SetRenderType(int Opt);
		void Render(float X, float Y, float Diameter, float OpacityValue = 1.0f);

	private:
		void ProcessTransform();
	};

	class LineCircleBrush {
	private:
		float   Radius{}, WidthValue{};

		Color3    Color{};
		float     Opacity{};

		int       RenderType{ RENDER_TYPE_DEFAULT };

		bool      CamInheritanceCommand{};
		bool      StaticWidthCommand{};

	public:
		LineCircleBrush(bool CamInheritanceFlag = false, bool StaticWidthFlag = false);
		void SetColor(float R, float G, float B);
		void SetColor(QP::Color3& ColorValue);
		void SetColorRGB(int R, int G, int B);
		void SetRenderType(int Opt);
		void Draw(float X, float Y, float Diameter, float Thickness, float OpacityValue = 1.0f);

	private:
		void Render();
	};
}