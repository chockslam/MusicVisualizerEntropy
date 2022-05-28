#pragma once

#include <string>
#include <list>
#include <memory>
#include "Graphics/Graphics.h"
#include "Graphics/Drawable/SolidSphere.h"
#include "Graphics/Bindable/ConstantBuffer.h"

class SceneObject
{
public:
	SceneObject(Graphics& gfx, const char* vs, const char* ps, float pos[3], const char* gs = nullptr);
	virtual ~SceneObject();
	virtual void Reset() ;
	virtual void Draw(Graphics& gfx) const ;
	void deactivate();
	void activate();
	bool IsActive() { return this->active; };
	virtual void Bind(Graphics& gfx, DirectX::FXMMATRIX view, float musParams[3]) const  = 0;
	
private:
	virtual void _Reset() = 0;

protected:
	bool active;
	float initPos[3];
	DirectX::XMFLOAT3 pos;											// position of the object
	std::list<std::shared_ptr<Drawable>> meshes;


};

