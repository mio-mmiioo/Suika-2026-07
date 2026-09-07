#pragma once
#include "../../Engine/ObjectManager/GamaObject.h"
#include "../MyLibrary/MyStruct.h"
#include "../Data.h"

class Fruit : public GameObject
{
public:
	/// <summary>
	/// 1つのフルーツ
	/// フルーツはGameMasterで管理される
	/// </summary>
	/// <param name="fruitType">フルーツの種類</param>
	/// <param name="position">フルーツの位置</param>
	Fruit(Data::FRUIT_TYPE fruitType, MY_POINT position);
	~Fruit() override;
	void Update() override;
	void Draw() override;

	/// <summary>
	/// 位置を取得する
	/// </summary>
	/// <returns>座標</returns>
	MY_POINT GetPosition() const { return position_; }
	
	/// <summary>
	/// 位置をセットする
	/// </summary>
	/// <param name="p">座標</param>
	void SetPosition(MY_POINT p) { position_ = p; }

	/// <summary>
	/// フルーツのデータを取得する
	/// </summary>
	/// <returns>フルーツのデータ</returns>
	Data::FRUIT_DATA GetFruitData() const { return fruitData_; }

	/// <summary>
	/// フルーツに加わっている力取得
	/// </summary>
	/// <returns>加わっている力</returns>
	MY_POINT GetVelocity() const { return velocity_; }

	/// <summary>
	/// フルーツに加わる力を加算する
	/// </summary>
	/// <param name="add">追加で加える力</param>
	void AddVelocity(MY_POINT add) { velocity_.x += add.x; velocity_.y += add.y; }

	/// <summary>
	/// フルーツがスリープ状態か確認する
	/// </summary>
	/// <returns>スリープ状態ならtrue</returns>
	bool GetIsSleep() const { return isSleep_; }

	/// <summary>
	/// 強制的にスリープにする
	/// GameOverになった際に呼ぶ
	/// </summary>
	void SetSleep() { isSleep_ = true; }

	/// <summary>
	/// スリープ状態を解除する
	/// </summary>
	void WakeUp();

	/// <summary>
	/// 自身にぶつかっているフルーツの数をセットする
	/// </summary>
	/// <param name="count">ぶつかっているフルーツの数</param>
	void SetSupportCount(int count) { supportCount_ = count; }
	
	/// <summary>
	/// ゲームオーバーの原因になったフルーツ
	/// </summary>
	void SetIsGameOverFruit();

private:
	Data::FRUIT_DATA fruitData_; // フルーツのデータ
	MY_POINT position_; // 位置

	bool isOnGround_;	// 箱の底にいるならtrue
	MY_POINT velocity_;	// 加わっている力

	bool isSleep_;			// スリープ( 物理演算をしない )ならtrue
	int supportCount_;		// 他のフルーツと接触している数
	int supportCountMin_;	// 他のフルーツと接触して、支えられる最小個数
	int sleepCount_;		// スリープ状態に持っていくためのカウント

	bool isGameOverFruit_; // 箱の外に出てしまったらtrue

	float angle_; // 角度
};
