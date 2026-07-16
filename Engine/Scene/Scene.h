#pragma once

// シーンを作成するときにはこれを継承して使う　シーンの親クラス
class Scene
{
public:
	Scene() {};
	virtual ~Scene() {};
	virtual void Update() {};
	virtual void Draw() {};
};
