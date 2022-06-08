#pragma once

#include <string>
#include <list>
#include <vector>
#include <memory>
#include "Graphics/Graphics.h"
#include "Graphics/Drawable/SolidSphere.h"
#include "Graphics/Bindable/ConstantBuffer.h"

class SceneObject
{
public:
	SceneObject(Graphics& gfx, const char* vs, const char* ps, DirectX::XMFLOAT3 pos, const char* gs = nullptr);
	virtual ~SceneObject();
	virtual void Reset() ;
	virtual void Draw(Graphics& gfx) const ;
	void deactivate();
	void activate();
	virtual DirectX::XMFLOAT3 getPos() { return this->pos; };
	bool IsActive() { return this->active; };
	virtual void Bind(Graphics& gfx, DirectX::FXMMATRIX view, float musParams[3], float timeFrame) const  = 0;
	void SetPos(DirectX::XMFLOAT3 pos);
	void SetSize(DirectX::XMFLOAT3 size);
	void adjustSize(DirectX::XMFLOAT3 dSize);
	void adjustPosition(DirectX::XMFLOAT3 dPos);
	DirectX::XMFLOAT3 GetPos();
	DirectX::XMFLOAT3 GetSize();

private:
	virtual void _Reset() = 0;

protected:
	std::string vs, ps, gs;
	bool active;
	float initPos[3];
	DirectX::XMFLOAT3 pos;											// position of the object
	DirectX::XMFLOAT3 size;											// position of the object
	
	std::vector<std::shared_ptr<Drawable>> meshes;
};

