#include "ChiliCamera.h"
#include "EMaths.h"
#include <algorithm>


ChiliCamera::ChiliCamera() 
{
	this->Reset();
}


DirectX::XMMATRIX ChiliCamera::GetMatrix() const
{
	
	
	const DirectX::XMVECTOR forwardBaseVector = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const auto lookVector = DirectX::XMVector3Transform(
		forwardBaseVector,
		DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f)
	);
	const auto camPosition = DirectX::XMLoadFloat3(&pos);
	const auto camTarget = DirectX::XMVectorAdd(camPosition, lookVector);

	return	DirectX::XMMatrixLookAtLH(camPosition, camTarget, DirectX::XMVectorSet(0.0f,1.0f,0.0f, 0.0f));
	
}

void ChiliCamera::Reset()
{
	pos = { 0.0f,0.0f, 0.0f };
	pitch = 0.0f;
	yaw = 0.0f;
}

void ChiliCamera::Rotate(float dx, float dy)
{
	yaw = wrap_angle(yaw + dx*rotationSpeed);
	pitch = std::clamp(pitch + dy * rotationSpeed, 0.995f * -PI / 2.0f, 0.995f * PI / 2.0f);
}

void ChiliCamera::Translate(DirectX::XMFLOAT3 translation)
{
	DirectX::XMStoreFloat3(&translation, DirectX::XMVector3Transform(
		DirectX::XMLoadFloat3(&translation),
		DirectX::XMMatrixRotationRollPitchYaw(pitch,yaw,0.0f)*
		DirectX::XMMatrixScaling(travelSpeed, travelSpeed, travelSpeed)
	));
	pos = {
		pos.x + translation.x,
		pos.y + translation.y,
		pos.z + translation.z
	};
}

DirectX::XMFLOAT3 ChiliCamera::getPos()
{
	return this->pos;
}

DirectX::XMFLOAT2 ChiliCamera::getOrient()
{
	return {this->yaw, this->pitch};
}

void ChiliCamera::setOrient(float pitch, float yaw)
{
	this->pitch = pitch;
	this->yaw = yaw;
}

void ChiliCamera::setPos(DirectX::XMFLOAT3 pos)
{
	this->pos = pos;
}
