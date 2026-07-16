#include "Camera.h"
#include "Screen.h"

namespace Camera
{
	DirectX::XMVECTOR position;		// カメラの位置
	DirectX::XMVECTOR target;		// カメラの焦点
	DirectX::XMMATRIX viewMatrix;	// ビュー行列
	DirectX::XMMATRIX projMatrix;	// プロジェクション行列

	float nearZ;	// カメラからこの値より近いものは映らない
	float farZ;		// カメラからこの値より遠いものは映らない
}

void Camera::Initialize()
{
	position = DirectX::XMVectorSet(0, 3, -10, 0);
	target = DirectX::XMVectorSet(0, 0, 0, 0);
	nearZ = 0.1f;
	farZ = 100.0f;

	// プロジェクション行列の作成
	// カメラをズームさせたいなどの場合、この処理はUpdateに書く
	projMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, (FLOAT)Screen::WINDOW_WIDTH / (FLOAT)Screen::WINDOW_HEIGHT, nearZ, farZ);
}

void Camera::Update()
{
	// ビュー行列の作成
	// カメラ固定のゲームなら、Initializeに書く
	viewMatrix = DirectX::XMMatrixLookAtLH(position, target, DirectX::XMVectorSet(0, 1, 0, 0));
}

void Camera::SetPosition(DirectX::XMVECTOR pos)
{
	position = pos;
}

void Camera::SetTarget(DirectX::XMVECTOR tar)
{
	target = tar;
}

DirectX::XMMATRIX Camera::GetViewMatrix()
{
	return viewMatrix;
}

DirectX::XMMATRIX Camera::GetProjectionMatrix()
{
	return projMatrix;
}
