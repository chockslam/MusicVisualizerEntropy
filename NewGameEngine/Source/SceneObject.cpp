#include "SceneObject.h"

SceneObject::SceneObject(Graphics& gfx, const char* vs, const char* ps, float pos[3], const char* gs)
{
	this->vs.append(vs);

	if(gs)
		this->gs.append(gs);

	this->ps.append(ps);

	this->initPos[0] = pos[0];
	this->initPos[1] = pos[1];
	this->initPos[2] = pos[2];

	Reset();
	if (pos) {
		delete[] pos;
	}
	pos = nullptr;

}

SceneObject::~SceneObject()
{
	
}



void SceneObject::Reset() 
{

	this->pos = { this->initPos[0], this->initPos[1], this->initPos[2] };
	
}

void SceneObject::Draw(Graphics& gfx) const 
{
	// Set new position and draw
	if (this->active) {
		for (auto& mesh : this->meshes) {
			mesh->SetPos(this->pos);
			mesh->SetSize(this->size);
			mesh->Draw(gfx);
		}
	}
}

void SceneObject::deactivate()
{
	this->active = false;
}

void SceneObject::activate()
{
	this->active = true;
}

void SceneObject::SetPos(DirectX::XMFLOAT3 pos)
{
	this->pos = pos;
}

void SceneObject::SetSize(DirectX::XMFLOAT3 size)
{
	this->size = size;
}

void SceneObject::adjustSize(DirectX::XMFLOAT3 dSize)
{
	this->size.x += dSize.x;
	this->size.y += dSize.y;
	this->size.z += dSize.z;
}

void SceneObject::adjustPosition(DirectX::XMFLOAT3 dPos)
{
	this->pos.x += dPos.x;
	this->pos.y += dPos.y;
	this->pos.z += dPos.z;
}

DirectX::XMFLOAT3 SceneObject::GetPos()
{
	return this->pos;
}

 DirectX::XMFLOAT3 SceneObject::GetSize()
{
	return this->size;
}



