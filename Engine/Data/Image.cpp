#include "Image.h"
#include <vector>
#include <algorithm>

namespace Image
{
	std::vector<ImageData*> imageList; // 読み込んだ画像を管理するためのクラス

	float DEFAULT_ROTATE = 0.0f;	// 画像のrotateがセットされていない場合の値
	float DEFAULT_ALPHA = 1.0f;		// 画像のalphaがセットされていない場合の値

	/// <summary>
	/// 画像を描画する
	/// </summary>
	/// <param name="x">スクリーンのx座標</param>
	/// <param name="y">スクリーンのy座標</param>
	/// <param name="rect">画像を切り抜くサイズ</param>
	/// <param name="width">描画する画像の横幅</param>
	/// <param name="height">描画する画像の縦幅</param>
	/// <param name="hImage">画像のハンドル</param>
	/// <param name="rotate">描画するときの回転量</param>
	/// <param name="alpha">描画するときの透明度</param>
	void DrawGraph(int x, int y, Rect rect, int width, int height, int hImage, float rotate, float alpha);
}

int Image::Load(std::string fileName)
{
	ImageData* pImageData = new ImageData;
	pImageData->fileName = fileName;
	pImageData->pSprite = nullptr;

	for (auto& itr : imageList)
	{
		if (itr->fileName == fileName)
		{
			pImageData->pSprite = itr->pSprite;
			break;
		}
	}

	if (pImageData->pSprite == nullptr)
	{
		pImageData->pSprite = new Sprite();
		pImageData->pSprite->LoadTexture(fileName.c_str());
	}
	imageList.push_back(pImageData);
	return((int)(imageList.size() - 1));
}

void Image::DrawGraph(int x, int y, Rect rect, int width, int height, int hImage, float rotate, float alpha)
{
	RECT r;
	r.left = (LONG)rect.leftTopX;
	r.top = (LONG)rect.leftTopY;
	r.right = (LONG)rect.rightDownX;
	r.bottom = (LONG)rect.rightDownY;
	imageList[hImage]->pSprite->DrawGraph(x, y, r, width, height, rotate, alpha);
}

void Image::DrawGraph(int x, int y, int hImage)
{
	int width;
	int height;
	GetGraphSize(hImage, &width, &height);
	RECT rect;
	rect.left = (LONG)0;
	rect.top = (LONG)0;
	rect.right = (LONG)width;
	rect.bottom = (LONG)height;
	imageList[hImage]->pSprite->DrawGraph(x, y, rect, width, height, DEFAULT_ROTATE, DEFAULT_ALPHA);
}

void Image::DrawExtendGraph(int x, int y, int width, int height, int hImage)
{
	int imageWidth;
	int imageHeight;
	GetGraphSize(hImage, &imageWidth, &imageHeight);

	// 切り抜くわけではないので、元の画像サイズを取得
	RECT rect;
	rect.left = (LONG)0;
	rect.top = (LONG)0;
	rect.right = (LONG)imageWidth;
	rect.bottom = (LONG)imageHeight;
	imageList[hImage]->pSprite->DrawGraph(x, y, rect, width, height, DEFAULT_ROTATE, DEFAULT_ALPHA);
}

void Image::DrawExtendRotateGraph(int x, int y, int width, int height, float rotate, int hImage)
{
	int imageWidth;
	int imageHeight;
	GetGraphSize(hImage, &imageWidth, &imageHeight);
	RECT rect;
	rect.left = (LONG)0;
	rect.top = (LONG)0;
	rect.right = (LONG)imageWidth;
	rect.bottom = (LONG)imageHeight;
	float rotation = rotate * DirectX::XM_PI / 180.0f;
	imageList[hImage]->pSprite->DrawGraph(x, y, rect, width, height, rotate * DirectX::XM_PI / 180.0f, DEFAULT_ALPHA);
}

void Image::DrawRectExtendGraph(int x, int y, int width, int height, int rectX, int rectY, int rectWidth, int rectHeight, int hImage)
{
	RECT rect;
	rect.left = (LONG)rectX;
	rect.top = (LONG)rectY;
	rect.right = (LONG)rectWidth + rectX;
	rect.bottom = (LONG)rectHeight + rectY;
	imageList[hImage]->pSprite->DrawGraph(x, y, rect, width, height, DEFAULT_ROTATE, DEFAULT_ALPHA);
}

void Image::GetGraphSize(int hImage, int* width, int* height)
{
	DirectX::XMFLOAT2 size = imageList[hImage]->pSprite->GetGraphSize();
	*width = (int)size.x;
	*height = (int)size.y;
}

void Image::Relase()
{
	bool isReffered = false; // 参照されているか
	for (int i = 0; i < imageList.size(); i++)
	{
		isReffered = false;
		for (int j = i + 1; j < imageList.size(); j++)
		{
			if (imageList[i]->pSprite == imageList[j]->pSprite)
			{
				isReffered = true;
				break;
			}
		}
		if (isReffered == false)
		{
			SAFE_DELETE(imageList[i]->pSprite);
		}
		SAFE_DELETE(imageList[i]);
	}
	imageList.clear(); // 配列を空にする
}
