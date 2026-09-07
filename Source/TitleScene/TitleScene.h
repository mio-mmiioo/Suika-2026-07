#pragma once
#include "../../Engine/Scene/Scene.h"
#include "../MyLibrary/ButtonArea.h"
#include <string>

class TitleScene : public Scene
{
public:
	TitleScene();
	~TitleScene() override;
	void Update() override;
	void Draw() override;

private:
	Button* newStart_;		// はじめる
	Button* continueStart_;	// 前回のつづきから始める
	AREA title_;			// タイトルの画像
	std::string bgm_;		// bgm
	int hBackground_;		// 背景

	float volume_; // 音量
};
