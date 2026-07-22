#pragma once
#include "../../Engine/Scene/Scene.h"

class Button;
class Player;

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene() override;
	void Update() override;
	void Draw() override;

private:
	int hBackground_;
	Player* player_;
	Button* end_; // ゲーム終了ボタン

	bool isGameOver_; // ゲームオーバーならtrue
	float timer_; // ゲームオーバー後に自動遷移させるまでの時間
};