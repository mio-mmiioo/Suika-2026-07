#include "Data.h"
#include "../Engine/Data/CsvReader.h"
#include "../Engine/Data/Image.h"

namespace Data
{
	// fruitDataListのcsvデータで、各カラムに入っている情報
	enum FRUIT_DATA_NUM
	{
		NUMBER,
		NAME,
		DISTANCE_R,
		SCORE,
		MAX_FRUIT_DATA_NUM
	};

	// areaListのcsvデータで、各カラムに入っている情報
	enum AREA_LIST_NUM
	{
		AREA_NAME,
		LEFT_TOP_X,
		LEFT_TOP_Y,
		RIGHT_DOWN_X,
		RIGHT_DOWN_Y,
		MAX_AREA_LIST_NUM
	};

	// fruitPhysicsのcsvデータで、各カラムに入っている情報
	enum FRUIT_PHYSICS_DATA_NUM
	{
		FRUIT_PHYSICS_DATA_NAME,
		FRUIT_PHYSICS_DATA_NUM,
		MAX_FRUIT_PHYSICS_DATA_NUM
	};

	// saveDataのcsvデータで、各カラムに入っている情報
	enum SAVE_FRUIT_DATA_NUM
	{
		TYPE,
		POS_X,
		POS_Y,
		MAX_SAVE_FRUIT_DATA_NUM
	};


	const int CSV_DATA_START_LINE = 1; // csvデータの読み込みを開始する位置　※一番上は、補足に使用する

	std::map<FRUIT_TYPE, FRUIT_DATA> fruitDataList;	// フルーツのデータリスト
	std::vector<SAVE_FRUIT_DATA> saveFruitData; // 保存したフルーツのデータ
	std::map<std::string, float> fruitPhysics;		// フルーツの物理計算に使用する数値
	std::map<std::string, AREA> areaList;			// areaの位置リスト
	std::map<std::string, int> image; // 画像のリスト
	std::map<std::string, int> sound; // 音源のリスト

	void InitImage(); // 画像の初期化
	void InitSound(); // 音源の初期化
	void InitFruitDataList();	// fruitDataListの初期化
	void InitFruitPhysics();	// 物理演算に使用する数値の初期化
	void InitAreaList();		// areaの位置リストの初期化
}

void Data::Init()
{
	InitImage();
	InitSound();
	InitAreaList();
	InitFruitDataList();
	InitFruitPhysics();
}

void Data::InitImage()
{
	image["title"]		= Image::Load("title.png");			// タイトル
	image["background"] = Image::Load("background.png");	// 背景
	image["box"]		= Image::Load("box.png");			// フルーツを入れる箱
	image["score"]		= Image::Load("score.png");			// スコアを表示する背景
	image["gameOver"]	= Image::Load("gameOver.png");		// ゲームオーバーの時に出す表示
	image["effect"]		= Image::Load("effect.png");		// 果物が進化した時のエフェクト
	
	// 果物の画像
	image["sakurannbo"] = Image::Load("sakurannbo.png");
	image["itigo"]		= Image::Load("itigo.png");
	image["budou"]		= Image::Load("budou.png");
	image["orennji"]	= Image::Load("orennji.png");
	image["mikann"]		= Image::Load("mikann.png");
	image["rinngo"]		= Image::Load("rinngo.png");
	image["nasi"]		= Image::Load("nasi.png");
	image["momo"]		= Image::Load("momo.png");
	image["painappuru"] = Image::Load("painappuru.png");
	image["meronn"]		= Image::Load("meronn.png");
	image["suika"]		= Image::Load("suika.png");
	
	image["sinnkanowa"] = Image::Load("sinnkanowa.png");	// 進化の輪　この順番に大きくなると示す画像

	// ボタンの画像
	image["newStartNormal"]			= Image::Load("newStartNormal.png");	// 新しくはじめる
	image["newStartSelect"]			= Image::Load("newStartSelect.png");
	image["continueStartNormal"]	= Image::Load("continueStartNormal.png");	// 続きからはじめる
	image["continueStartSelect"]	= Image::Load("continueStartSelect.png");
	image["endNormal"]				= Image::Load("endNormal.png");		// 終わりにする
	image["endSelect"]				= Image::Load("endSelect.png");

	// 数字0～9の代入
	std::string name;
	for (int count = 0; count < 10; count++)
	{
		name = std::to_string(count);
		image[name] = Image::Load(name + ".png");
	}
}

void Data::InitSound()
{
	// SE
	Sound::Load("putFruit");		// フルーツを置くとき
	Sound::Load("createFruit");		// フルーツが成長するとき
	Sound::Load("gameOverFruit");	// ゲームオーバーの原因となったフルーツの落下音
	Sound::Load("select");			// 選択音
	Sound::Load("decide");			// 決定音

	// bgm
	Sound::Load("bgm01");
	Sound::Load("bgm02");
}

void Data::InitFruitDataList()
{
	CsvReader* csv = new CsvReader("fruitDataList.csv");
	FRUIT_DATA current; // 確認中の果物
	std::string name = "";
	for (int line = CSV_DATA_START_LINE; line < csv->GetLines(); line++)
	{
		name = csv->GetString(line, FRUIT_DATA_NUM::NAME);
		current.distanceR = csv->GetFloat(line, FRUIT_DATA_NUM::DISTANCE_R);
		current.score = csv->GetInt(line, FRUIT_DATA_NUM::SCORE);
		current.image = image[name];
		current.type = NumberToFruitType(csv->GetInt(line, FRUIT_DATA_NUM::NUMBER));
		fruitDataList[current.type] = current;
	}
	delete csv;
}

void Data::InitFruitPhysics()
{
	CsvReader* csv = new CsvReader("fruitPhysics.csv");
	std::string name = "";
	for (int line = CSV_DATA_START_LINE; line < csv->GetLines(); line++)
	{
		name = csv->GetString(line, FRUIT_PHYSICS_DATA_NAME);
		fruitPhysics[name] = csv->GetFloat(line, FRUIT_PHYSICS_DATA_NUM);
	}
	delete csv;
}

void Data::InitAreaList()
{
	CsvReader* csv = new CsvReader("areaList.csv");
	AREA current;
	std::string name;
	for (int line = CSV_DATA_START_LINE; line < csv->GetLines(); line++)
	{
		name = csv->GetString(line, AREA_LIST_NUM::AREA_NAME);
		current.hImage		= image[name];
		current.leftTopX	= csv->GetInt(line, AREA_LIST_NUM::LEFT_TOP_X);
		current.leftTopY	= csv->GetInt(line, AREA_LIST_NUM::LEFT_TOP_Y);
		current.rightDownX	= csv->GetInt(line, AREA_LIST_NUM::RIGHT_DOWN_X);
		current.rightDownY	= csv->GetInt(line, AREA_LIST_NUM::RIGHT_DOWN_Y);
		areaList[name] = current;
	}
	delete csv;
}

Data::FRUIT_TYPE Data::NumberToFruitType(int number)
{
	switch (number)
	{
	case 0:
		return FRUIT_TYPE::SAKURANNBO;
	case 1:
		return FRUIT_TYPE::ITIGO;
	case 2:
		return FRUIT_TYPE::BUDOU;
	case 3:
		return FRUIT_TYPE::ORENNJI;
	case 4:
		return FRUIT_TYPE::MIKANN;
	case 5:
		return FRUIT_TYPE::RINNGO;
	case 6:
		return FRUIT_TYPE::NASI;
	case 7:
		return FRUIT_TYPE::MOMO;
	case 8:
		return FRUIT_TYPE::PAINAPPURU;
	case 9:
		return FRUIT_TYPE::MERONN;
	case 10:
		return FRUIT_TYPE::SUIKA;
	}
	// 見つからなかった
	return FRUIT_TYPE::MAX_FRUIT_TYPE;
}

void Data::InitSaveFruitData(int* score)
{
	saveFruitData.clear();
	CsvReader* csv = new CsvReader("saveData.csv"); // 読み込むデータ
	SAVE_FRUIT_DATA current; // 確認中のフルーツ

	if (csv->GetLines() > 0)
	{
		*score = csv->GetInt(0, 0);
	}

	for (int line = CSV_DATA_START_LINE; line < csv->GetLines(); line++)
	{
		current.type = NumberToFruitType(csv->GetInt(line, SAVE_FRUIT_DATA_NUM::TYPE));
		current.x = csv->GetFloat(line, SAVE_FRUIT_DATA_NUM::POS_X);
		current.y = csv->GetFloat(line, SAVE_FRUIT_DATA_NUM::POS_Y);
		saveFruitData.push_back(current);
	}
	delete csv;
}

