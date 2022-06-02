#include "ObjectManager.h"


void ObjectManager::AddObject(std::shared_ptr<SceneObject> obj)
{
	this->pObjectList.push_back(std::move(obj));
}

void ObjectManager::UpdateAll(Graphics& gfx, DirectX::XMMATRIX viewMatrix, float musParams[3])
{
	for (auto& obj : this->pObjectList) {
		obj->Bind(gfx, viewMatrix, musParams);
	}
	if(sm)
		sm->Update();
}

void ObjectManager::RenderAll(Graphics& gfx)
{
	for (auto& obj : this->pObjectList) {
		obj->Draw(gfx);
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
		auto partsys = QueryObject<ParticleSystem>();

		ParticleProps prop;	

		DirectX::XMFLOAT4 colorB;
		
		colorB.x = 255.0f;
		colorB.y = 255.0f;
		colorB.z = 255.0f;
		colorB.w = 255.0f;
		

		prop.ColorBegin = colorB;

		DirectX::XMFLOAT4 colorE;
		
		colorE.x = 1.0f;
		colorE.y = 1.0f;
		colorE.z = 1.0f;
		colorE.w = 1.0f;
		
		prop.ColorEnd = colorE;

		prop.LifeTime = 2.0f;
		prop.Position = partsys->getPos();
		prop.SizeBegin = 2.0f;
		prop.SizeEnd = 0.3f;
		prop.SizeVariation = 0.1f;


		DirectX::XMFLOAT3 Velocity;

		Velocity.x = 0.05f;
		Velocity.y = 0.05f;
		Velocity.z = 0.05f;

		prop.Velocity = Velocity;
		

		DirectX::XMFLOAT3 VelocityVariation;

		VelocityVariation.x = 0.4f;
		VelocityVariation.y = 0.4f;
		VelocityVariation.z = 0.4f;
		
		prop.VelocityVariation = VelocityVariation;
		partsys->Emit(prop);

}


void ObjectManager::startScene(Graphics& gfx, std::string scene)
{
	this->sm->StartScene(gfx, scene);
}
