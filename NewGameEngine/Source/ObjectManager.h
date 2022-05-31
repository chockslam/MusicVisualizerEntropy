#pragma once

#include <list>
#include <memory>
#include "SceneManager.h"
#include "SceneObject.h"


class ObjectManager
{
public:
	ObjectManager() {};
	void AddObject(std::shared_ptr<SceneObject> obj);		// ObjectFactory related method
	void SetLevelManager(std::shared_ptr<SceneManager> sm) { if(!sm) this->sm = std::move(sm); };
	void UpdateAll(Graphics& gfx, DirectX::XMMATRIX viewMatrix, float musParams[3]);						// Update all objects in the pObjectList
	void RenderAll(Graphics& gfx);										// Render all objects in the pObjectList
	void DeleteAll();										// Delete all objects in the pObjectList && pObjectCollidable
	void DeleteInactive();									// Delete all objects that became 'inactive'
	void InactivateAll();									// Inactivate all objects.
	template<class T> std::shared_ptr<T> QueryObject();
	template<class T> std::list<std::shared_ptr<T>> QueryObjectList();
	virtual ~ObjectManager() {};
	void startScene(Graphics& gfx, std::string scene);
private:
	std::list<std::shared_ptr<SceneObject>> pObjectList;
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
