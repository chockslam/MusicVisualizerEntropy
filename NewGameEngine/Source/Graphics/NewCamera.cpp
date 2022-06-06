// CODE was written with help by Jpres (https://www.youtube.com/watch?v=sROnNl_7Ex8)

#include "NewCamera.h"

Camera::Camera()
{
	// Set Camera to (0,0) in the world position upon Initialization
	this->pos = dx::XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->posVector = XMLoadFloat3(&this->pos);
	this->rot = dx::XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateViewMatrix();
}


const dx::XMMATRIX& Camera::GetViewMatrix() const
{
	return this->viewMatrix;
}

const dx::XMMATRIX& Camera::GetProjectionMatrix() const
{
	return this->projectionMatrix;
}

const dx::XMVECTOR& Camera::GetPositionVector() const
{
	return this->posVector;
}

const dx::XMFLOAT3& Camera::GetPositionFloat3() const
{
	return this->pos;
}

const dx::XMVECTOR& Camera::GetRotationVector() const
{
	return this->rotVector;
}

const dx::XMFLOAT3& Camera::GetRotationFloat3() const
{
	return this->rot;
}

void Camera::SetPosition(const dx::XMVECTOR& pos)
{
	XMStoreFloat3(&this->pos, pos);
	this->posVector = pos;
	this->UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
	this->pos = dx::XMFLOAT3(x, y, z);
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateViewMatrix();
}

void Camera::AdjustPosition(const dx::XMVECTOR& pos)
{

	this->posVector = dx::XMVectorAdd(posVector, pos);
	XMStoreFloat3(&this->pos, this->posVector);
	this->UpdateViewMatrix();
}

void Camera::AdjustPosition(float x, float y, float z)
{
	this->pos.x += x;
	this->pos.y += y;
	this->pos.z += z;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateViewMatrix();
}

void Camera::SetRotation(const dx::XMVECTOR& rot)
{
	this->rotVector = rot;
	XMStoreFloat3(&this->rot, rot);
	this->UpdateViewMatrix();
}

void Camera::SetRotation(float x, float y, float z)
{
	this->rot = dx::XMFLOAT3(x, y, z);
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateViewMatrix();
}

void Camera::AdjustRotation(const dx::XMVECTOR& rot)
{
	this->rotVector = dx::XMVectorAdd(rotVector, rot);
	XMStoreFloat3(&this->rot, this->rotVector);
	this->UpdateViewMatrix();
}

void Camera::AdjustRotation(float x, float y, float z)
{
	this->rot.x += x;
	this->rot.y += y;
	this->rot.z += z;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateViewMatrix();
}

void Camera::SetLookAtPos(dx::XMFLOAT3 lookAtPos)
{
	// If not updated return
	if (lookAtPos.x == this->pos.x && lookAtPos.y == this->pos.y && lookAtPos.z == this->pos.z)
		return;

	// determine delta position
	lookAtPos.x = this->pos.x - lookAtPos.x;
	lookAtPos.y = this->pos.y - lookAtPos.y;
	lookAtPos.z = this->pos.z - lookAtPos.z;

	// Determining orientation of the camera. Algrothm.
	// It determines pitch and yaw orientation.
	float pitch = 0.0f;
	if (lookAtPos.y != 0.0f)
	{
		const float distance = (float) sqrt(lookAtPos.x * lookAtPos.x + lookAtPos.z * lookAtPos.z);
		pitch = (float) atan(lookAtPos.y / distance);
	}
	float yaw = 0.0f;
	if (lookAtPos.x != 0.0f)
	{
		pitch = (float) atan(lookAtPos.x / lookAtPos.z);
	}

	if (lookAtPos.z > 0)
		yaw += dx::XM_PI;

	// Set pitch and yaw rotation values. Roll is omitted due to the nature of the camera.
	this->SetRotation(pitch, yaw, 0.0f);
}

const dx::XMVECTOR& Camera::GetForwardVector()
{
	return this->vec_forward;
}

const dx::XMVECTOR& Camera::GetRightVector()
{
	return this->vec_right;
}

const dx::XMVECTOR& Camera::GetBackwardVector()
{
	return this->vec_backward;
}

const dx::XMVECTOR& Camera::GetLeftVector()
{
	return this->vec_left;
}

void Camera::UpdateViewMatrix() //Updates view matrix and also updates the movement vectors.
{
	
	//using namespace DirectX;
	//
	//const dx::XMVECTOR forwardBaseVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	//// apply the camera rotations to a base vector
	//const auto lookVector = XMVector3Transform(forwardBaseVector,
	//	XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, 0.0f)
	//);
	//// generate camera transform (applied to all objects to arrange them relative
	//// to camera position/orientation in world) from cam position and direction
	//// camera "top" always faces towards +Y (cannot do a barrel roll)
	//const auto camPosition = XMLoadFloat3(&pos);
	//const auto camTarget = camPosition + lookVector;
	//return XMMatrixLookAtLH(camPosition, camTarget, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	//
	// Calculate camera rotation matrix.
	dx::XMMATRIX camRotationMatrix = dx::XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z);
	// Calculate unit vector of cam target based off camera forward value transformed by cam rotation matrix.
	dx::XMVECTOR camTarget = dx::XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, camRotationMatrix);
	// Adjust cam target to be offset by the camera's current position.
	camTarget = dx::XMVectorAdd(camTarget, posVector);
	
	// Calculate up direction based on current rotation.
	dx::XMVECTOR upDir = XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, camRotationMatrix);
	// Rebuild view matrix.
	this->viewMatrix = dx::XMMatrixLookAtLH(this->posVector, camTarget, upDir);
	
	// Rotation matrix constructed from roll and pitch values.
	dx::XMMATRIX vecRotationMatrix = dx::XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, 0);
	
	// Update directional vectors.
	this->vec_forward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR,	vecRotationMatrix);
	this->vec_backward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR,	vecRotationMatrix);
	this->vec_left = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR,		vecRotationMatrix);
	this->vec_right = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR,		vecRotationMatrix);
}