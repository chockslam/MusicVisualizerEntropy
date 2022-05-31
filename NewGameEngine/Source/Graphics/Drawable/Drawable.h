/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)

#pragma once
#include "../Graphics.h"
#include <DirectXMath.h>

class Bindable;

/// <summary>
/// Abstract Class that represents a drawable object.
/// </summary>
class Drawable
{
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual DirectX::XMMATRIX GetTransformXM() const  = 0;				// Transformation matrix of the object. Used by TransformCBuf object.
	virtual void Draw(Graphics& gfx) const ;									// Draw Object -> executed every frame.
	void SetPos(DirectX::XMFLOAT3 pos) { this->pos = pos; };
	virtual ~Drawable() = default;
	template<class T>															// Query Bindable object from the vector of bindables. Need to modify resources in runtime. Not Used in the program.
	T* QueryBindable() 
	{
		for (auto& pb : binds)
		{
			if (auto pt = dynamic_cast<T*>(pb.get()))
			{
				return pt;
			}
		}
		return nullptr;
	}
	//virtual void SetPos(DirectX::XMFLOAT3 pos) ;
	bool isActive() { return this->active; };
	void activate() { this->active = true; };
	void deactivate() { this->active = false; };
protected:
	void AddBind(std::shared_ptr<Bindable> bind) ;						// Add Bindable to the vector of bindables
	
	DirectX::XMFLOAT3 pos = { 1.0f,1.0f,1.0f };
	DirectX::XMFLOAT3 size = {1.0f, 1.0f, 1.0f};
	bool active;
private:
	const class IndexBuffer* pIndexBuffer = nullptr;							// Index Buffer
	std::vector<std::shared_ptr<Bindable>> binds;
	

};