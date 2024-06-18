#pragma once
#include <array>
#include <deque>
#include <ranges>
#include <string>
#include <algorithm>
#include "Object.h"

enum class Type
{ Main, Sub };

enum class LayerRange
{ One, All };

enum class DeleteRange
{ One, All };

constexpr int NumLayer{ static_cast<int>(Layer::END) };

class FrameWork {
private:
	std::array<std::deque<OBJ*>, NumLayer> Cont;
	std::string RunningMode{};

	float FrameTime{};
	bool PauseState{};
	bool SwitchState{};

	typedef std::string(*Function)(void);

public:
	void SetFrameTime(float ElapsedTime) {
		FrameTime = ElapsedTime;
	}

	void SetModeName(std::string ModeName) {
		RunningMode = ModeName;
	}

	void SetMode(Function Func) {
		SwitchState = true;
		ClearContainer();
		RunningMode = Func();
		SwitchState = false;
	}

	void Pause(bool Flag) {
		PauseState = Flag;
	}

	std::string Mode() {
		return RunningMode;
	}

	void Routine() {
		using namespace std;
		if (!SwitchState && !RunningMode.empty()) {
			for (int i = 0; i < NumLayer; ++i) {
				for (auto It = ranges::begin(Cont[i]); It != ranges::end(Cont[i]);) {
					if (!PauseState)
						if ((*It) && !(*It)->NoUpdateAtPause) (*It)->Update(FrameTime);
					if (*It) (*It)->ProcessTransform();
					if (*It) (*It)->Render();
					if (*It) ++It;
				}
			}
		}
	}

	void AddObject(OBJ*&& Object, Layer LayerNum) {
		Cont[static_cast<int>(LayerNum)].push_back(Object);
	}

	void DeleteSelf(OBJ* Object, Layer LayerNum) {
		using namespace std;

		int FindLayer = static_cast<int>(LayerNum);
		auto It = ranges::find(Cont[FindLayer], Object);

		if (It != ranges::end(Cont[FindLayer])) {
			delete* It;
			*It = nullptr;
			It = Cont[FindLayer].erase(It);
		}
	}

	void DeleteObject(std::string ObjectTag, DeleteRange Range1, LayerRange Range2, Layer LayerNum = static_cast<Layer>(0)) {
		using namespace std;

		if (Range1 == DeleteRange::One) {
			if (Range2 == LayerRange::One) {
				int FindLayer = static_cast<int>(LayerNum);
				auto It = ranges::find_if(Cont[FindLayer], [&ObjectTag](OBJ*& Object) {return Object->Tag == ObjectTag; });

				if (It != ranges::end(Cont[FindLayer])) {
					delete* It;
					*It = nullptr;
					It = Cont[FindLayer].erase(It);
				}
			}

			else if (Range2 == LayerRange::All) {
				for (int i = 0; i < NumLayer; ++i) {
					auto It = ranges::find_if(Cont[i], [&ObjectTag](OBJ*& Object) {return Object->Tag == ObjectTag; });
					if (It != ranges::end(Cont[i])) {
						delete* It;
						*It = nullptr;
						It = Cont[i].erase(It);

						return;
					}
				}
			}
		}

		else if (Range1 == DeleteRange::All) {
			if (Range2 == LayerRange::One) {
				int FindLayer = static_cast<int>(LayerNum);
				auto It = ranges::begin(Cont[FindLayer]);

				while (It != ranges::end(Cont[FindLayer])) {
					It = ranges::find_if(Cont[FindLayer], [&ObjectTag](OBJ*& Object) {return Object->Tag == ObjectTag; });

					if (It != ranges::end(Cont[FindLayer])) {
						delete* It;
						*It = nullptr;
						It = Cont[FindLayer].erase(It);
					}
				}
			}

			else if (Range2 == LayerRange::All) {
				for (int i = 0; i < NumLayer; ++i) {
					auto It = ranges::begin(Cont[i]);

					while (It != ranges::end(Cont[i])) {
						It = ranges::find_if(Cont[i], [&ObjectTag](OBJ*& Object) {return Object->Tag == ObjectTag; });

						if (It != ranges::end(Cont[i])) {
							delete* It;
							*It = nullptr;
							It = Cont[i].erase(It);
						}
					}
				}
			}
		}
	}

	OBJ* FindObject(std::string ObjectTag, LayerRange Range, Layer LayerNum) {
		using namespace std;

		if (Range == LayerRange::One) {
			int FindLayer = static_cast<int>(LayerNum);
			auto It = ranges::find_if(Cont[FindLayer], [&ObjectTag](OBJ*& Object) {return Object->Tag == ObjectTag; });
			if (It != ranges::end(Cont[FindLayer]))
				return *It;

			else
				return nullptr;
		}

		else if (Range == LayerRange::All) {
			for (int i = 0; i < NumLayer; ++i) {
				auto It = ranges::begin(Cont[i]);

				while (It != ranges::end(Cont[i])) {
					It = ranges::find_if(Cont[i], [&ObjectTag](OBJ*& Object) {return Object->Tag == ObjectTag; });

					if (It != ranges::end(Cont[i]))
						return *It;

					++It;
				}
			}
			return nullptr;
		}
	}

	OBJ* FindObject(std::string ObjectTag, Layer LayerNum, int Index) {
		int FindLayer = static_cast<int>(LayerNum);

		if (Cont[FindLayer][Index]->Tag == ObjectTag)
			return Cont[FindLayer][Index];

		return nullptr;
	}

	void ClearContainer() {
		using namespace std;

		for (int i = 0; i < NumLayer; ++i) {
			for (auto It = ranges::begin(Cont[i]); It != ranges::end(Cont[i]);) {
				delete* It;
				*It = nullptr;
				It = Cont[i].erase(It);
			}
		}
	}

	void ClearLayer(Layer LayerNum) {
		using namespace std;

		int DeleteLayer = static_cast<int>(LayerNum);

		for (auto It = ranges::begin(Cont[DeleteLayer]); It != ranges::begin(Cont[DeleteLayer]);) {
			delete* It;
			*It = nullptr;
			It = Cont[DeleteLayer].erase(It);
		}
	}

	size_t LayerSize(Layer LayerNum) {
		int Num = static_cast<int>(LayerNum);
		return Cont[Num].size();
	}
};
extern FrameWork fw;