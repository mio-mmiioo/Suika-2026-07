#pragma once
#include <string>
#include <map>
#include <vector>
#include "MyLibrary/ButtonArea.h"
#include "../Engine/Data/Sound.h"
#include "../Engine/Data/Image.h"

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
	struct FRUIT_DATA
	{
		float distanceR;	// 当たり判定の半径
		int score;			// 得点
		int image;			// 画像
		FRUIT_TYPE type;	// 種類
	};

	// ゲームを保存するときに必要なフルーツのデータ
	struct SAVE_FRUIT_DATA
	{
		FRUIT_TYPE type; // 種類
		float x; // x座標
		float y; // y座標
	};

	void Init(); // 初期化
	FRUIT_TYPE NumberToFruitType(int number);

	void InitSaveFruitData(int* score);

	extern std::map<FRUIT_TYPE, FRUIT_DATA> fruitDataList; // フルーツのデータリスト
	extern std::vector<SAVE_FRUIT_DATA> saveFruitData; // 保存したフルーツのデータ
	extern std::map<std::string, float> fruitPhysics;	// フルーツの物理計算に使用する数値
	extern std::map<std::string, AREA> areaList; // Areaの位置リスト
	extern std::map<std::string, int> image; // 画像のリスト
	extern std::map<std::string, int> sound; // 音源のリスト
}
