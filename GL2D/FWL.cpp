#include "FWL.h"
#include "FWL_Message.h"

FWL_Log FLog;

FWL::FWL() {
	if (DebugMessage) {
		std::cout << "======== FWL Message ========\n";
		std::cout << "FWL is prepared.\n\n";
	}
}

void FWL::SetFrameTime(float ElapsedTime) {
	FrameTime = ElapsedTime;
}

std::string FWL::Mode() {
	return RunningMode;
}

void FWL::SwitchToSubRunningState() {
	SubRunningState = true;
}

void FWL::SwitchToDefaultRunningState() {
	SubRunningState = false;
}

void FWL::Init(Function ModeFunction) {
	if (RunningState)
		return;

	RunningMode = ModeFunction();

	FLog.CurrentMode = RunningMode;
	FLog.Log(LogType::FWL_INIT);

	RunningState = true;
}

void FWL::Routine() {
	using namespace std;
	if (!ModeSwitchState && RunningState) {
		for (int i = 0; i < Num; ++i) {
			if (Container[i].empty())
				continue;

			for (auto It = begin(Container[i]); It != end(Container[i]);) {
				if (CheckDeleteFlag(It, i))
					continue;

				if (!SubRunningState)
					(*It)->Update(FrameTime);

				else if (SubRunningState) {
					if (!(*It)->StopAtPauseFlag)
						(*It)->Update(FrameTime);
				}

				(*It)->ProcessTransform();
				(*It)->Render();

				if (CheckDeleteFlag(It, i))
					continue;

				++It;
			}

			if (ReserveState) {
				ModeSwitchState = true;
				break;
			}
		}
	}

	if (ReserveState) {
		ChangeMode();
		ReserveState = false;
	}
}

void FWL::SwitchMode(Function ModeFunction, bool PauseOption) {
	if (!RunningState)
		return;

	Buffer = ModeFunction;
	ReserveState = true;

	if (PauseOption)
		SubRunningState = true;
	else
		SubRunningState = false;

	FLog.IsPause = SubRunningState;
	FLog.PrevMode = RunningMode;
}

void FWL::AddObject(OBJ* Object, std::string Tag, Layer AddLayer, bool AddAsSubObject) {
	Container[static_cast<int>(AddLayer)].push_back(Object);
	Object->ObjectTag = Tag;

	FLog.ObjectTag = Tag;
	FLog.Log(LogType::ADD_OBJECT);

	if (AddAsSubObject) {
		Object->StopAtPauseFlag = false;
		FLog.Log(LogType::SET_AS_SUB_OBJECT);
	}
}

void FWL::DeleteSelf(OBJ* Object) {
	Object->DeleteFlag = true;

	FLog.ObjectTag = Object->ObjectTag;
	FLog.Log(LogType::DELETE_OBJECT);
}

void FWL::DeleteObject(std::string Tag, DeleteRange deleteRange, SearchRange searchRange, Layer LayerToSearch) {
	switch (searchRange) {
	case SearchRange::One:
		int layer;
		layer = static_cast<int>(LayerToSearch);

		switch (deleteRange) {
		case DeleteRange::One:
			for (auto& It : Container[layer]) {
				if (It->ObjectTag == Tag) {
					It->DeleteFlag = true;

					FLog.ObjectTag = It->ObjectTag;
					FLog.Log(LogType::DELETE_OBJECT);
					return;
				}
			}
			break;

		case DeleteRange::All:
			for (auto& It : Container[layer]) {
				if (It->ObjectTag == Tag) {
					It->DeleteFlag = true;

					FLog.ObjectTag = It->ObjectTag;
					FLog.Log(LogType::DELETE_OBJECT);
				}
			}
			break;
		}
		break;

	case SearchRange::All:
		switch (deleteRange) {
		case DeleteRange::One:
			for (auto& A : Container) {
				for (auto& It : A) {
					if (It->ObjectTag == Tag) {
						It->DeleteFlag = true;

						FLog.ObjectTag = It->ObjectTag;
						FLog.Log(LogType::DELETE_OBJECT);
						return;
					}
				}
			}
			break;

		case DeleteRange::All:
			for (auto& A : Container) {
				for (auto& It : A) {
					if (It->ObjectTag == Tag) {
						It->DeleteFlag = true;

						FLog.ObjectTag = It->ObjectTag;
						FLog.Log(LogType::DELETE_OBJECT);
					}
				}
			}
			break;
		}
		break;
	}
}

OBJ* FWL::Find(std::string Tag, SearchRange searchRange, Layer LayerToSearch) {
	switch (searchRange) {
	case SearchRange::One:
		int layer;
		layer = static_cast<int>(LayerToSearch);

		for (auto& It : Container[layer]) {
			if (It->ObjectTag == Tag)
				return It;
		}
		break;

	case SearchRange::All:
		for (auto& A : Container) {
			for (auto& It : A) {
				if (It->ObjectTag == Tag)
					return It;
			}
		}
		break;
	}

	return nullptr;
}

OBJ* FWL::Find(std::string Tag, Layer LayerToSearch, int Index) {
	int layer = static_cast<int>(LayerToSearch);

	if (Index >= Container[layer].size())
		return nullptr;

	if (Container[layer][Index]->ObjectTag == Tag)
		return Container[layer][Index];

	return nullptr;
}

size_t FWL::Size(Layer TargetLayer) {
	return Container[static_cast<int>(TargetLayer)].size();
}



bool FWL::CheckDeleteFlag(std::deque<OBJ*>::iterator& It, int Layer) {
	if ((*It)->DeleteFlag) {
		delete* It;
		*It = nullptr;
		It = Container[Layer].erase(It);
		return true;
	}
	return false;
}

void FWL::ChangeMode() {
	ClearAll();

	RunningMode = Buffer();
	Buffer = nullptr;

	FLog.CurrentMode = RunningMode;

	if (FLog.CurrentMode == FLog.PrevMode)
		FLog.ErrorLog(LogType::ERROR_SAME_MODE);

	FLog.Log(LogType::MODE_SWITCH);

	ModeSwitchState = false;
}

void FWL::ClearAll() {
	using namespace std;

	for (int i = 0; i < Num; ++i) {
		for (auto It = begin(Container[i]); It != end(Container[i]);) {
			delete* It;
			*It = nullptr;
			It = Container[i].erase(It);
		}
	}
}