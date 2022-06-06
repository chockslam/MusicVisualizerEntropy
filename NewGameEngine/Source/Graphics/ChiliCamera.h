#pragma once

#include <DirectXMath.h>
#include <string>

class ChiliCamera {
public:
	ChiliCamera();
	DirectX::XMMATRIX GetMatrix() const;
	void Reset();
	void Rotate(float dx, float dy);
	void Translate(DirectX::XMFLOAT3 translation);
	DirectX::XMFLOAT3 getPos();
	DirectX::XMFLOAT2 getOrient();
	void setOrient(float pitch, float yaw);
	void setPos(DirectX::XMFLOAT3 pos);
private:
	DirectX::XMFLOAT3 pos = {0.0f,0.0f,0.0f};
	float pitch;
	float yaw;
	static constexpr float travelSpeed = 48.0f;
	static constexpr float rotationSpeed = 0.8f;
};