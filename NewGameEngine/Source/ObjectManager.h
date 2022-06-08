#pragma once

#include <list>
#include <memory>
#include "SceneManager.h"
#include "SceneObject.h"
#include "Graphics/Particle_Systems/ParticleSystem.h"
// #include "Graphics/Particle_Systems/Particle.h"


class ObjectManager
{
public:
	ObjectManager() {};
	void AddObject(std::shared_ptr<SceneObject> obj);		
	void SetLevelManager(std::shared_ptr<SceneManager> sm) { if(!this->sm) this->sm = std::move(sm); };
	void UpdateAll(Graphics& gfx, DirectX::XMMATRIX viewMatrix, float musParams[3], float timeFrame);						// Update all objects in the pObjectList
	void RenderAll(Graphics& gfx);										// Render all objects in the pObjectList
	void DeleteAll();										// Delete all objects in the pObjectList && pObjectCollidable
	void DeleteInactive();									// Delete all objects that became 'inactive'
	void InactivateAll();									// Inactivate all objects.
	void EmitParticles();
	virtual ~ObjectManager() {};
	void startScene(Graphics& gfx, std::string scene);
private:
	template<class T> std::shared_ptr<T> QueryObject();
	template<class T> std::list<std::shared_ptr<T>> QueryObjectList();
	std::list<std::shared_ptr<SceneObject>> pObjectList;
	std::list<std::shared_ptr<ParticleSystem>> pPSList;
	std::shared_ptr<SceneManager> sm;
};

template<class T>
inline std::shared_ptr<T> ObjectManager::QueryObject()
{
	for (auto& pb : pObjectList)
	{
		if (auto pt = std::dynamic_pointer_cast<T>(pb))
		{
			return std::move(pt);
		}
	}
	return nullptr;
}

template<class T>
inline std::list<std::shared_ptr<T>> ObjectManager::QueryObjectList()
{
	std::list<std::shared_ptr<T>> list;
	for (auto& pb : pObjectList)
	{
		if (auto pt = std::dynamic_pointer_cast<T>(pb))
		{
			list.push_back(pt);
		}
	}
	return list;
}
