#pragma once
#include "SDK_Header.h"

namespace MSDK {
	class CircleBrush {
	private:
		glm::vec3 Color{};
		float   Radius{};
		float   Opacity{};
		int       RenderType{ RENDER_TYPE_DEFAULT };

		bool      CamInheritanceCommand{};

	public:
		CircleBrush(bool CamInheritanceFlag = false);
		void SetColor(float R, float G, float B);
		void SetColor(glm::vec3& ColorValue);
		void SetColorRGB(int R, int G, int B);
		void SetRenderType(int Opt);
		void Draw(float X, float Y, float Diameter, float OpacityValue = 1.0f);

	private:
		void Render();
	};

	class LineCircleBrush {
	private:
		glm::vec3 Color{};
		float   Radius{}, WidthValue{};
		float   Opacity{};
		int       RenderType{ RENDER_TYPE_DEFAULT };

		bool      CamInheritanceCommand{};
		bool      StaticWidthCommand{};

	public:
		LineCircleBrush(bool CamInheritanceFlag = false, bool StaticWidthFlag = false);
		void SetColor(float R, float G, float B);
		void SetColor(glm::vec3& ColorValue);
		void SetColorRGB(int R, int G, int B);
		void SetRenderType(int Opt);
		void Draw(float X, float Y, float Diameter, float Width, float OpacityValue = 1.0f);

	private:
		void Render();
	};
}