#include "ObjectManager.h"


void ObjectManager::AddObject(std::shared_ptr<SceneObject> obj)
{
	this->pObjectList.push_back(std::move(obj));
}

void ObjectManager::UpdateAll(Graphics& gfx, DirectX::XMMATRIX viewMatrix, float musParams[3], float timeFrame)
{
	for (auto& obj : this->pObjectList) {
		obj->Bind(gfx, viewMatrix, musParams, timeFrame);
	}
	if(sm)
		sm->Update(timeFrame);
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

		if (partsys) {
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

			prop.LifeTime = 0.5f;
			prop.Position = partsys->getPos();
			prop.SizeBegin = 1.0f;
			prop.SizeEnd = 0.1f;
			prop.SizeVariation = 0.1f;


			DirectX::XMFLOAT3 Velocity;

			Velocity.x = 0.1f;
			Velocity.y = 0.1f;
			Velocity.z = 0.1f;

			prop.Velocity = Velocity;


			DirectX::XMFLOAT3 VelocityVariation;

			VelocityVariation.x = 0.2f;
			VelocityVariation.y = 0.2f;
			VelocityVariation.z = 0.2f;

			prop.VelocityVariation = VelocityVariation;
			partsys->Emit(prop);
		}

		

}


void ObjectManager::startScene(Graphics& gfx, std::string scene)
{
	this->sm->StartScene(gfx, scene);
}
