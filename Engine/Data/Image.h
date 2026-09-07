#pragma once
#include <string>
#include "../DirectX/Sprite.h"

namespace Image
{
	// 画像を管理するために必要なデータ
	struct IMAGE_DATA
	{
		Sprite* pSprite = nullptr;	// 2D画像を表示するためのクラス
		std::string fileName;		// ファイルの名前
	};

	// 四角形を指定する
	struct MY_RECT
	{
		float leftTopX;		// 左上のx座標
		float leftTopY;		// 左上のy座標
		float rightDownX;	// 右上のx座標
		float rightDownY;	// 右上のy座標
	};

	/// <summary>
	/// 指定したファイルの画像を読み込む
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <returns>何番目に読み込んだ画像か</returns>
	int Load(std::string fileName);

	/// <summary>
	/// 画像を表示
	/// </summary>
	/// <param name="x">スクリーンのx座標</param>
	/// <param name="y">スクリーンのy座標</param>
	/// <param name="hImage">画像のハンドル</param>
	void DrawGraph(int x, int y, int hImage);

	/// <summary>
	/// 拡縮した画像を表示
	/// </summary>
	/// <param name="x">スクリーンのx座標</param>
	/// <param name="y">スクリーンのy座標</param>
	/// <param name="width">描画する画像の横幅</param>
	/// <param name="height">描画する画像の縦幅</param>
	/// <param name="hImage">画像のハンドル</param>
	void DrawExtendGraph(int x, int y, int width, int height, int hImage);
	
	/// <summary>
	/// 拡縮した画像を回転表示
	/// x,yは回転の中心座標
	/// </summary>
	/// <param name="x">スクリーンのx座標</param>
	/// <param name="y">スクリーンのy座標</param>
	/// <param name="width">表示する画像の横幅</param>
	/// <param name="height">表示する画像の縦幅</param>
	/// <param name="rotate">表示する画像の回転量</param>
	/// <param name="hImage">画像のハンドル</param>
	void DrawExtendRotateGraph(int x, int y, int width, int height, float rotate, int hImage);

	/// <summary>
	/// 切り抜き拡縮した画像を表示
	/// </summary>
	/// <param name="x">スクリーンのx座標</param>
	/// <param name="y">スクリーンのy座標</param>
	/// <param name="width">表示する画像の横幅</param>
	/// <param name="height">表示する画像の縦幅</param>
	/// <param name="rectX">切り抜く左上のx座標</param>
	/// <param name="rectY">切り抜く左上のy座標</param>
	/// <param name="rectWidth">切り抜く横幅</param>
	/// <param name="rectHeight">切り抜く縦幅</param>
	/// <param name="hImage">切り抜く画像のハンドル</param>
	void DrawRectExtendGraph(int x, int y, int width, int height, int rectX, int rectY, int rectWidth, int rectHeight, int hImage);

	/// <summary>
	/// 画像のサイズを取得する
	/// </summary>
	/// <param name="hImage">画像のハンドル</param>
	/// <param name="width">画像の横幅を代入する変数</param>
	/// <param name="height">画像の縦幅を代入する変数</param>
	void GetGraphSize(int hImage, int* width, int* height);
	
	/// <summary>
	/// 解放処理
	/// </summary>
	void Relase();
}
