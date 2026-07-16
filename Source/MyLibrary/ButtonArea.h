#pragma once

struct Area
{
	int leftTopX;	// 左上のx座標
	int leftTopY;	// 左上のy座標
	int rightDownX; // 右下のx座標
	int rightDownY; // 右下のy座標
	int hImage;		// 画像
};

namespace ButtonArea
{
	/// <summary>
	/// 範囲内にマウスカーソルがあるか調べる
	/// </summary>
	/// <param name="area">確認する範囲</param>
	/// <returns>範囲内にマウスカーソルがあるとtrue</returns>
	bool IsMouseKeepInArea(Area area);

	/// <summary>
	/// 範囲内にマウスカーソルが入った瞬間か調べる
	/// </summary>
	/// <param name="area">確認する範囲</param>
	/// <returns>範囲内にマウスカーソルが入った瞬間だとtrue</returns>
	bool IsMouseInArea(Area area);

	/// <summary>
	/// 範囲内に指定した座標が入った瞬間か調べる
	/// </summary>
	/// <param name="area">確認する範囲</param>
	/// <param name="mouseX">マウスのx座標</param>
	/// <param name="mouseY">マウスのy座標</param>
	/// <returns>範囲内に指定した座標が入った瞬間だとtrue</returns>
	bool IsMouseInArea(Area area, int mouseX, int mouseY);

	/// <summary>
	/// 指定sた範囲に拡縮した画像を描画
	/// </summary>
	/// <param name="area">指定した範囲</param>
	void DrawArea(Area area);
}

class Button
{
public:
	/// <summary>
	/// コンストラクタ、ボタンの範囲を指定する
	/// </summary>
	/// <param name="normal">通常時の範囲</param>
	/// <param name="select">選択時の範囲</param>
	Button(Area normal, Area select);
	~Button();
	void Update();
	void Draw() const;

	/// <summary>
	/// 範囲内が押されているか調べる
	/// </summary>
	/// <returns>範囲内が押されているとtrue</returns>
	bool GetIsPushArea() const { return isPushArea_; }

	/// <summary>
	/// 範囲が選択され続けているか調べる
	/// </summary>
	/// <returns>範囲が選択され続けているとtrue</returns>
	bool GetIsKeepOnArea() const { return isOnArea_; }

	/// <summary>
	/// 範囲が選択された瞬間か調べる
	/// </summary>
	/// <returns>範囲が選択瞬間だとtrue</returns>
	bool GetIsOnArea() const;

private:
	Area normal_;		// 通常時
	Area select_;		// 選択中
	bool isPushArea_;	// ボタンが押された
	bool isOnArea_;		// ボタン上にある
};
