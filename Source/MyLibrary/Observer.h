#pragma once

namespace Observer
{
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 得点の代入
	/// </summary>
	/// <param name="s">得点</param>
	void SetScore(int s);

	/// <summary>
	/// 得点の加算
	/// </summary>
	/// <param name="addScore">加算する得点</param>
	void AddScore(int addScore);

	/// <summary>
	/// 得点の取得
	/// </summary>
	/// <returns>現在の得点</returns>
	int GetScore();

	/// <summary>
	/// 新しくゲームを始めるかセットする
	/// 呼び出さないならtrueがセットされる
	/// </summary>
	/// <param name="flag">新しくはじめるならtrue</param>
	void SetIsNewGame(bool flag);

	/// <summary>
	/// 新しくゲームを始めるか確認する
	/// </summary>
	/// <returns>新しくゲームを始めるならtrue</returns>
	bool GetIsNewGame();

	/// <summary>
	/// saveData.csvの情報を元に前回のフルーツを作成したかセットする
	/// </summary>
	/// <param name="flag">前回のフルーツを作成していたならtrue</param>
	void SetIsPrevGameCreated(bool flag);

	/// <summary>
	/// saveData.csvの情報を元に前回のフルーツを作成した確認する
	/// </summary>
	/// <returns>前回のフルーツを作成していたならtrue</returns>
	bool GetIsPrevGameCreated();
}
