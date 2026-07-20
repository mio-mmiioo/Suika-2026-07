#pragma once

class Fruit;

namespace GameMaster
{
	void Init();	// 初期化
	void Update();	// 更新処理
	void Draw();	// 描画処理
	void Release();	// 解放処理

	/// <summary>
	/// 管理するフルーツリストにフルーツを追加
	/// </summary>
	/// <param name="fruit">追加するフルーツのインスタンス</param>
	void AddFruit(Fruit* fruit);

	/// <summary>
	/// フルーツ同士の位置調整
	/// </summary>
	void FruitCheckPosition();

	/// <summary>
	/// フルーツがボックス内に収まっているか確認する
	/// </summary>
	/// <param name="fruit">フルーツのインスタンス</param>
	/// <returns>ボックスの底をはみ出しているとtrue</returns>
	bool FruitCheckBoxPosition(Fruit* fruit);
}
