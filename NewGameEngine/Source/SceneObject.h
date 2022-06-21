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
	SceneObject(Graphics& gfx, const char* vs, const char* ps, DirectX::XMFLOAT3 pos, const char* gs = nullptr, std::string channel = "left");
	virtual ~SceneObject();
	virtual void Reset() ;
	virtual void Draw(Graphics& gfx) const ;
	void deactivate();
	void activate();
	virtual DirectX::XMFLOAT3 getPos() { return this->pos; };
	bool IsActive() { return this->active; };
	virtual void Bind(Graphics& gfx, DirectX::FXMMATRIX view, float musParams[6], float timeFrame) const  = 0;
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
	
	std::string channel;
	bool active;
	
	DirectX::XMFLOAT3 initPos;
	DirectX::XMFLOAT3 pos;											
	DirectX::XMFLOAT3 rot;											
	DirectX::XMFLOAT3 size;											
	std::vector<std::shared_ptr<Drawable>> meshes;

};

