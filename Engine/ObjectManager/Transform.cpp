#include "Transform.h"

Transform::Transform()
	:matTranslate_(DirectX::XMMatrixIdentity()),
	matRotate_(DirectX::XMMatrixIdentity()),
	matScale_(DirectX::XMMatrixIdentity()),
	pParent_(nullptr)
{
	position_ = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotate_ = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale_ = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
}

Transform::~Transform()
{
}

void Transform::Calclation()
{
	matTranslate_ = DirectX::XMMatrixTranslation(position_.x, position_.y, position_.z);
	matRotate_ = DirectX::XMMatrixRotationRollPitchYaw(rotate_.x, rotate_.y, rotate_.z);
	matScale_ = DirectX::XMMatrixScaling(scale_.x, scale_.y, scale_.z);
}

DirectX::XMMATRIX Transform::GetWorldMatrix()
{
	Calclation();

	if (pParent_ != nullptr)
	{
		return matScale_ * matRotate_ * matTranslate_ * pParent_->GetWorldMatrix();
	}
	return matScale_ * matRotate_ * matTranslate_;
}

DirectX::XMMATRIX Transform::GetNormalMatrix()
{
	return matRotate_ * DirectX::XMMatrixInverse(nullptr, matScale_);
}

void Transform::MultiplyMatrices(DirectX::XMMATRIX matrix)
{
	// 行列を掛けるためにXMVECTORへ変換
	DirectX::XMVECTOR vPos = DirectX::XMLoadFloat3(&position_);
	DirectX::XMVECTOR vRot = DirectX::XMLoadFloat3(&rotate_);
	DirectX::XMVECTOR vSca = DirectX::XMLoadFloat3(&scale_);

	// ベクトルと行列の乗算
	vPos = DirectX::XMVector3TransformNormal(vPos, GetWorldMatrix());
	vRot = DirectX::XMVector3TransformNormal(vRot, GetWorldMatrix());
	vSca = DirectX::XMVector3TransformNormal(vSca, GetWorldMatrix());

	// 結果をセットする
	DirectX::XMStoreFloat3(&position_, vPos);
	DirectX::XMStoreFloat3(&position_, vRot);
	DirectX::XMStoreFloat3(&position_, vSca);
}

