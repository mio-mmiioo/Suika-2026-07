#pragma once
#include "GamaObject.h"
#include "Transform.h"

class Object3D : public GameObject
{
public:
	Object3D();
	virtual ~Object3D() override;
	virtual void Update() override;
	virtual void Draw() override;

	Transform GetTransform() { return transform_; }

protected:
	Transform transform_; // 位置、回転、拡縮の情報
	Object3D* parent_; // 親

	int hModel_; // 描画するモデル
	int hitModel_; // 当たり判定用のモデル
};
