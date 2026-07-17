#pragma once
#include <string>
#include <map>
#include <vector>
#include "MyLibrary/ButtonArea.h"

namespace Data
{
	void Init(); // 初期化

	extern std::map<std::string, Area> areaList; // Areaの位置リスト
	extern std::map<std::string, int> image; // 画像のリスト
}
