#pragma once
#include <QuadPent_ModeHeader.h>

class GameMode1 {
public:
	int ModeType { MODE_TYPE_DEFAULT };

	static void Start() {
		SetUp();
		QP::System.SetBackColor(0.3, 0.3, 0.3);
		// Add task here
	}

	static void Destructor() {
		// Add task here
	}

#pragma region FoldRegion 
	std::vector<QP::Object*> InputObject{};
	inline static GameMode1* M_Inst;

	GameMode1() { M_Inst = this; }

	void Map(QP::Mode& ModePtr) {
		ModePtr = Start;
	}

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