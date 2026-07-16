#pragma once
#include "../Engine/Scene/Scene.h"

class TestScene : public Scene
{
public:
	TestScene();
	~TestScene() override;
	void Update() override;
	void Draw() override;
};