#pragma once
#include "QuadPent_Scene.h"
#include "QuadPent_Mouse.h"

#include "QuadPent_StartUp.h"

class QuadPent_StartUpMode {
public:
	int ModeType{ MODE_TYPE_DEFAULT };

	static void Start() {
		SetUp();
		QP::System.SetBackColor(0.0, 0.0, 0.0);
		QP::Scene.AddSystemObject(new QuadPent_StartUp);
	}

	static void Destructor() {
		QP::Scene.LockSystemLayer();
	}

#pragma region FoldRegion 
	std::vector<QP::Object*> InputObject{};
	inline static QuadPent_StartUpMode* M_Inst;

	QuadPent_StartUpMode() { M_Inst = this; }

	static void SetUp() {
		std::string ModeName = typeid(*M_Inst).name();
		QP::StringTool.RemoveString(ModeName, "class ");
		QP::Scene.RegisterModeName(ModeName);

		std::cout << "Switched mode to [ " << ModeName << " ]." << std::endl;

		M_Inst->InputObject.clear();
		QP::Scene.RegisterInputObjectList(M_Inst->InputObject, M_Inst->ModeType);

		QP::Scene.RegisterDestructor(Destructor);
		QP::Scene.RegisterModePtr(M_Inst->Start);
	}
#pragma endregion
};