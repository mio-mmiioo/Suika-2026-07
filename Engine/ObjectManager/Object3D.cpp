#include "Object3D.h"
#include "../Data/Model.h"

Object3D::Object3D()
	:parent_(nullptr),
	hModel_(-1),
	hitModel_(-1)
{

}

Object3D::~Object3D()
{
}

void Object3D::Update()
{
}

void Object3D::Draw()
{
	if (hModel_ > 0)
	{
		const DirectX::XMMATRIX& m = transform_.GetWorldMatrix();
		if (parent_ != nullptr)
		{
			transform_.MultiplyMatrices(m * parent_->GetTransform().GetWorldMatrix());
			Model::SetTransform(hModel_, transform_);
		}
		else
		{
			transform_.MultiplyMatrices(m);
			Model::SetTransform(hModel_, transform_);
		}
		Model::Draw(hModel_);
	}
}

