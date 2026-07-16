#pragma once
#include "../../Engine/Scene/Scene.h"

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
};