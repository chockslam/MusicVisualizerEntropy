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



