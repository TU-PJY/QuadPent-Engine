#include "CollisionUtil.h"
#include "ImageUtil.h"
#include "CameraUtil.h"


void AABB::Update(GLfloat X, GLfloat Y, GLfloat xScale, GLfloat yScale) {
	OffsetX = xScale / 2;
	OffsetY = yScale / 2;

	LeftX = X - OffsetX;
	RightX = X + OffsetX;
	LeftY = Y - OffsetY;
	RightY = Y + OffsetY;

	CenterX = X;
	CenterY = Y;

	Width = xScale;
	Height = yScale;
}

void AABB::InitMatrix() {
#ifdef SHOW_BOUND_BOX
	TranslateMatrix = glm::mat4(1.0f);
	ScaleMatrix = glm::mat4(1.0f);
	RotateMatrix = glm::mat4(1.0f);
#endif
}

void AABB::Move(glm::mat4& Matrix, GLfloat X, GLfloat Y) {
#ifdef SHOW_BOUND_BOX
	Matrix = translate(Matrix, glm::vec3(X, Y, 0.0));
#endif
}

void AABB::Rotate(glm::mat4& Matrix, GLfloat Rotation) {
#ifdef SHOW_BOUND_BOX
	Matrix = rotate(Matrix, glm::radians(Rotation), glm::vec3(0.0, 0.0, 1.0));
#endif
}

void AABB::Scale(glm::mat4& Matrix, GLfloat X, GLfloat Y) {
#ifdef SHOW_BOUND_BOX
	Matrix = scale(Matrix, glm::vec3(X, Y, 1.0));
#endif
}

void AABB::Render() {
#ifdef SHOW_BOUND_BOX
	ProcessTransform();
	imageUtil.Render(ImageCollisionBox);
	if (Collide)
		imageUtil.Render(ImageCollidedBox);
#endif
}

bool AABB::CheckCollision(const AABB& Other) {
	if (RightX < Other.LeftX || LeftX > Other.RightX) {
		Collide = false;
		return false;
	}

	if (RightY < Other.LeftY || LeftY > Other.RightY) {
		Collide = false;
		return false;
	}
	
	Collide = true;
	return true;
}

bool AABB::CheckCollisionEdge(GLfloat Value, Edge Edge) {
	switch (Edge) {
	case Edge::Right:
		if (RightX > Value) {
			CenterX = RightX - (RightX - Value) - OffsetX;
			return true;
		}
		break;

	case Edge::Left:
		if (LeftX < Value) {
			CenterX = LeftX + (Value - LeftX) + OffsetX;
			return true;
		}
		break;

	case Edge::Top:
		if (RightY > Value) {
			CenterY = RightY - (RightY - Value) - OffsetY;
			return true;
		}
		break;

	case Edge::Bottom:
		if (LeftY < Value) {
			CenterY = LeftY + (Value - LeftY) + OffsetY;
			return true;
		}
	}

	return false;
}

bool AABB::CheckCollisionPoint(GLfloat X, GLfloat Y) {
	if ((LeftX <= X && X <= RightX) && (LeftY <= Y && Y <= RightY)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

void AABB::InterpolateX(GLfloat& X) {
	X = CenterX;
}

void AABB::InterpolateY(GLfloat& Y) {
	Y = CenterY;
}

void AABB::ProcessTransform() {
#ifdef SHOW_BOUND_BOX
	TransparencyLocation = glGetUniformLocation(ImageShader, "transparency");
	glUniform1f(TransparencyLocation, 1.0);

	ObjectColorLocation = glGetUniformLocation(TextShader, "objectColor");
	glUniform3f(ObjectColorLocation, 1.0, 0.0, 0.0);

	ModelLocation = glGetUniformLocation(ImageShader, "model");
	glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, value_ptr(TranslateMatrix * RotateMatrix * ScaleMatrix));
#endif
}



void OBB::Update(GLfloat X, GLfloat Y, GLfloat BoxWidth, GLfloat BoxHeight, GLfloat RotationValue) {
	Center = glm::vec2(X, Y);
	Offset = glm::vec2(BoxWidth / 2, BoxHeight / 2);

	GLfloat Rad = glm::radians(RotationValue);
	Axis[0] = glm::vec2(std::cos(Rad), std::sin(Rad));
	Axis[1] = glm::vec2(-std::sin(Rad), std::cos(Rad));

	Width = BoxWidth;
	Height = BoxHeight;
}

void OBB::InitMatrix() {
#ifdef SHOW_BOUND_BOX
	TranslateMatrix = glm::mat4(1.0f);
	RotateMatrix = glm::mat4(1.0f);
	ScaleMatrix = glm::mat4(1.0f);
#endif
}

void OBB::Move(glm::mat4& Matrix, GLfloat X, GLfloat Y){
#ifdef SHOW_BOUND_BOX
	Matrix = translate(Matrix, glm::vec3(X, Y, 0.0));
#endif
}

void OBB::Rotate(glm::mat4& Matrix, GLfloat Rotation){
#ifdef SHOW_BOUND_BOX
	Matrix = rotate(Matrix, glm::radians(Rotation), glm::vec3(0.0, 0.0, 1.0));
#endif
}

void OBB::Scale(glm::mat4& Matrix, GLfloat X, GLfloat Y) {
#ifdef SHOW_BOUND_BOX
	Matrix = scale(Matrix, glm::vec3(X, Y, 1.0));
#endif
}

void OBB::Render() {
#ifdef SHOW_BOUND_BOX
		ProcessTransform();
		imageUtil.Render(ImageCollisionBox);
		if (Collide)
			imageUtil.Render(ImageCollidedBox);
#endif
}

bool OBB::OverlapOnAxis(const OBB& OBB1, const OBB& OBB2, const glm::vec2& Axis) {
	auto Project = [](const OBB& OBB, const glm::vec2& Axis) {
		glm::vec2 Corners[4] = {
			OBB.Center + OBB.Axis[0] * OBB.Offset.x + OBB.Axis[1] * OBB.Offset.y,
			OBB.Center + OBB.Axis[0] * OBB.Offset.x - OBB.Axis[1] * OBB.Offset.y,
			OBB.Center - OBB.Axis[0] * OBB.Offset.x + OBB.Axis[1] * OBB.Offset.y,
			OBB.Center - OBB.Axis[0] * OBB.Offset.x - OBB.Axis[1] * OBB.Offset.y
		};

		float Min = glm::dot(Corners[0], Axis);
		float Max = Min;

		for (int i = 1; i < 4; i++) {
			float Projection = glm::dot(Corners[i], Axis);

			if (Projection < Min) 
				Min = Projection;

			if (Projection > Max) 
				Max = Projection;
		}

		return std::make_pair(Min, Max);
	};

	auto [Min1, Max1] = Project(OBB1, Axis);
	auto [Min2, Max2] = Project(OBB2, Axis);

	return !(Max1 < Min2 || Max2 < Min1);
}

bool OBB::CheckCollision(const OBB& Other) {
	glm::vec2 Axises[] = { Axis[0], Axis[1], Other.Axis[0], Other.Axis[1] };

	for (const auto& Axis : Axises) {
		if (!OverlapOnAxis(*this, Other, Axis)) {
			Collide = false;
			return false;
		}
	}

	Collide = true;
	return true;
}

bool OBB::CheckCollisionPoint(GLfloat X, GLfloat Y) {
	glm::vec2 D = glm::vec2(X, Y) - Center;

	for (int i = 0; i < 2; ++i) {
		GLfloat Dist = glm::dot(D, Axis[i]);
		if (std::abs(Dist) > Offset[i]) {
			Collide = false;
			return false;
		}
	}

	Collide = true;
	return true;
}

void OBB::ProcessTransform() {
#ifdef SHOW_BOUND_BOX
	TransparencyLocation = glGetUniformLocation(ImageShader, "transparency");
	glUniform1f(TransparencyLocation, 1.0);

	ObjectColorLocation = glGetUniformLocation(TextShader, "objectColor");
	glUniform3f(ObjectColorLocation, 1.0, 0.0, 0.0);

	ModelLocation = glGetUniformLocation(ImageShader, "model");
	glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, value_ptr(TranslateMatrix * RotateMatrix * ScaleMatrix));
#endif
}



void Range::InitMatrix() {
#ifdef SHOW_BOUND_BOX
	TranslateMatrix = glm::mat4(1.0f);
	ScaleMatrix = glm::mat4(1.0f);
	RotateMatrix = glm::mat4(1.0f);
#endif
}

void Range::Move(glm::mat4& Matrix, GLfloat X, GLfloat Y) {
#ifdef SHOW_BOUND_BOX
	Matrix = translate(Matrix, glm::vec3(X, Y, 0.0));
#endif
}

void Range::Rotate(glm::mat4& Matrix, GLfloat Rotation) {
#ifdef SHOW_BOUND_BOX
	Matrix = rotate(Matrix, glm::radians(Rotation), glm::vec3(0.0, 0.0, 1.0));
#endif
}

void Range::Scale(glm::mat4& Matrix, GLfloat X, GLfloat Y) {
#ifdef SHOW_BOUND_BOX
	Matrix = scale(Matrix, glm::vec3(X, Y, 1.0));
#endif
}

void Range::Update(GLfloat X, GLfloat Y, GLfloat Size) {
	CenterX = X;
	CenterY = Y;
	Radius = Size / 2.0;
	Extent = Size;
}

void Range::Render() {
#ifdef SHOW_BOUND_BOX
	ProcessTransform();
	imageUtil.Render(ImageCollisionSphere);
	if (Collide)
		imageUtil.Render(ImageCollidedSphere);
#endif
}

GLfloat Range::CalculateDistance( GLfloat X, GLfloat Y) {
	GLfloat DX = X - CenterX;
	GLfloat DY = Y - CenterY;
	return std::sqrt(DX * DX + DY * DY);
}

bool Range::CheckCollision(const Range& Other) {
	if (CalculateDistance(Other.CenterX, Other.CenterY) < Radius + Other.Radius) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool Range::CheckCollisionPoint(GLfloat X, GLfloat Y) {
	if (CalculateDistance(X, Y) < Radius) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

void Range::ProcessTransform() {
#ifdef SHOW_BOUND_BOX
	TransparencyLocation = glGetUniformLocation(ImageShader, "transparency");
	glUniform1f(TransparencyLocation, 1.0);

	ObjectColorLocation = glGetUniformLocation(TextShader, "objectColor");
	glUniform3f(ObjectColorLocation, 1.0, 0.0, 0.0);

	ModelLocation = glGetUniformLocation(ImageShader, "model");
	glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, value_ptr(TranslateMatrix * RotateMatrix * ScaleMatrix));
#endif
}