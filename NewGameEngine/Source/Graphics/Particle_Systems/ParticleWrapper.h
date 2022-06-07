#include "../../SceneObject.h"

class ParticleWrapper : public SceneObject
{
	friend class ParticleSystem;
public:
	ParticleWrapper(Graphics& gfx, float radius, int latDiv, int longDiv, const char* vs, const char* ps, float pos[3], const char* gs = nullptr);
	void Reset();
	void Bind(Graphics& gfx, DirectX::FXMMATRIX view, float musParams[3], float timeFrame) const;
	
	bool isActive() { return this->active; };
	void activate() { this->active = true; };
	void deactivate() { this->active = false; };
private:

	void _Reset();

	struct ParticleCBuf
	{
		alignas(16) DirectX::XMFLOAT3 color;
	};

	ParticleCBuf cbData;											// Struct object declared above.
	mutable PixelConstantBuffer<ParticleCBuf> cbuf;							// Pixel Constant buffer that uses structure declared above as a data.
	mutable GeometryConstantBuffer<ParticleCBuf> cbufG;			// Geometry Constant buffer that uses structure declared above as a data.


	DirectX::XMFLOAT3 velocity;
	DirectX::XMFLOAT4 ColorBegin, ColorEnd;

	float currentColor[3];

	float SizeBegin, SizeEnd;

	float LifeTime = 1.0f;
	float LifeRemaining = 0.0f;

	float rotation;
};
