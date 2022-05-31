/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)
#include "Drawable.h"
#include "../Bindable/IndexBuffer.h"
#include <cassert>
#include <typeinfo>
#include <iostream>

void Drawable::Draw(Graphics& gfx) const 
{
	if (this->active) {

		for (auto& b : binds)
		{
			b->Bind(gfx);													// Bind every D3D resource to the pipeline every frame
		}
		gfx.DrawIndexed(pIndexBuffer->GetCount());							// Draw vertices of the mesh via Indexing technique
	}
	
}


void Drawable::AddBind(std::shared_ptr<Bindable> bind) 
{
	if (this->active) {
		// special case for index buffer
		if (typeid(*bind) == typeid(IndexBuffer))
		{
			assert("Binding multiple index buffers not allowed" && pIndexBuffer == nullptr);
			pIndexBuffer = &static_cast<IndexBuffer&>(*bind);													// Index buffer needs to be set to the separate variable to call DrawIndexed(...). See Draw(...) Above.
		}
		binds.push_back(std::move(bind));																		// move pointer to the bindable to the vector of all Bindables every frame.
	}
}

