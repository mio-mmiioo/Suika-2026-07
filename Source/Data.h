#pragma once
#include <string>
#include <map>
#include <vector>
#include "MyLibrary/ButtonArea.h"

namespace Data
{
	// フルーツの種類
	enum FRUIT_TYPE
	{
		SAKURANNBO,
		ITIGO,
		BUDOU,
		ORENNJI,
		MIKANN,
		RINNGO,
		NASI,
		MOMO,
		PAINAPPURU,
		MERONN,
		SUIKA,
		MAX_FRUIT_TYPE
	};

	// フルーツを管理するのに必要なデータ
	struct FruitData
	{
		float distanceR;	// 当たり判定の半径
		int score;			// 得点
		int image;			// 画像
		FRUIT_TYPE type;	// 種類
	};

	void Init(); // 初期化
	FRUIT_TYPE NumberToFruitType(int number);

	extern std::map<FRUIT_TYPE, FruitData> fruitDataList; // フルーツのデータリスト
	extern std::map<std::string, float> fruitPhysics;	// フルーツの物理計算に使用する数値
	extern std::map<std::string, Area> areaList; // Areaの位置リスト
	extern std::map<std::string, int> image; // 画像のリスト
}
