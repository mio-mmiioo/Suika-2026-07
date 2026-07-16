#pragma once
#include "../../Engine/Scene/Scene.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene() override;
	void Update() override;
	void Draw() override;
};