#include "ObjectManager.h"



void ObjectManager::AddObject(std::shared_ptr<SceneObject> obj)
{
	this->pObjectList.push_back(std::move(obj));
}

void ObjectManager::UpdateAll(Graphics& gfx, DirectX::XMMATRIX viewMatrix, float musParams[3], float timeFrame)
{
	for (auto& ps : this->pPSList) {
		ps->Update(musParams, timeFrame);
	}
	for (auto& obj : this->pObjectList) {
		obj->Bind(gfx, viewMatrix, musParams, timeFrame);
		obj->Draw(gfx);
	}
	if (sm)
		sm->Update(musParams,timeFrame);
}

void ObjectManager::RenderAll(Graphics& gfx)
{
	for (auto& obj : this->pObjectList) {
		//obj->Draw(gfx);
	}
}

void ObjectManager::DeleteAll()
{
	pObjectList.clear();
}

void ObjectManager::DeleteInactive()
{
	for (auto& obj : pObjectList)
	{
		if (!obj->IsActive())
			obj.reset();
	}
}

void ObjectManager::InactivateAll()
{
	for (auto& pObj : pObjectList) {
		pObj->deactivate();
	}
}

void ObjectManager::EmitParticles()
{

		

}


void ObjectManager::startScene(Graphics& gfx, std::string scene)
{
	this->sm->StartScene(gfx, scene);
}
