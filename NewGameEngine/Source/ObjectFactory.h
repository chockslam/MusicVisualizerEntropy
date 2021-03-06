#pragma once
#include <memory>
#include "ObjectManager.h"
#include "Graphics/Particle_Systems/ParticleSystem.h"
class ObjectFactory
{
public:
    static ObjectFactory& getInstance()
    {
        static ObjectFactory    instance; // Guaranteed to be destroyed.
                              // Instantiated on first use.
        return instance;
    }
private:
    ObjectFactory() {}                // Constructor? (the {} brackets) are needed here.

    // C++ 11
    // =======
    // We can use the better technique of deleting the methods
    // we don't want.
public:
    ObjectFactory(ObjectFactory const&) = delete;
    void operator=(ObjectFactory const&) = delete;
    std::shared_ptr<ObjectManager> getOM() { return this->om; };
    void SetUpObjectManager(std::shared_ptr<ObjectManager> om);
    void SetUpLevelManager();
    void AddSphereRightChannel(Graphics& gfx, DirectX::XMFLOAT3 pos, float radius, float latDiv, float longDiv, const char* vs, const char* ps, const char* gs = nullptr);
    void AddSphere(Graphics& gfx, DirectX::XMFLOAT3 pos, float radius, float latDiv, float longDiv, const char* vs, const char* ps, const char* gs = nullptr);
    void AddParticleSystem(Graphics& gfx, DirectX::XMFLOAT3 Velocity, DirectX::XMFLOAT3 VelocityVariation, DirectX::XMFLOAT4 ColorBegin, DirectX::XMFLOAT4 ColorEnd, float SizeBegin, float SizeEnd, float SizeVariation, float LifeTime, const char* vs, const char* ps, DirectX::XMFLOAT3 pos, const char* gs = nullptr);
public:
    
    
    
private:
    std::shared_ptr<ObjectManager> om;
};

