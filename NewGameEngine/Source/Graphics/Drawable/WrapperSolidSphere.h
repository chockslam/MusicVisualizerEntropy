/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)

#pragma once
#include "../Graphics.h"
#include "SolidSphere.h"
#include "..\..\SceneObject.h"
#include "../Bindable/ConstantBuffer.h"
//#include "..\..\Window\AudioIO.h"

/// <summary>
/// Wrapper around a standard SolidSphere to make manipulation with the object easier.
/// </summary>
class WrapperSolidSphere : public SceneObject
{
public:
	WrapperSolidSphere(Graphics& gfx, float radius,int latDiv,int longDiv, const char* vs, const char* ps, DirectX::XMFLOAT3 pos, const char* gs = nullptr, std::string channel = "left");
	~WrapperSolidSphere();
	void Reset() ;
	void Bind(Graphics& gfx, DirectX::FXMMATRIX view, float musParams[6], float timeFrame) const;
private:
	void _Reset();
	/// <summary>
	/// Pixel shader constant buffer.
	/// /color/ - contains music data from FFT (x = Bass, y = Mid-Range, z = Treble) 
	/// </summary>
	struct SolidSphereCBuf
	{
		alignas(16) DirectX::XMFLOAT3 color;
	};
private:
	
	SolidSphereCBuf cbData;											// Struct object declared above.
	mutable PixelConstantBuffer<SolidSphereCBuf> cbuf;				// Pixel Constant buffer that uses structure declared above as a data.
	mutable GeometryConstantBuffer<SolidSphereCBuf> cbufG;			// Geometry Constant buffer that uses structure declared above as a data.
};