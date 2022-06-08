#include "SceneManager.h"
#include "ObjectFactory.h"


SceneManager::SceneManager()
	:
	partCount(0),
	kdTimeForParticles(0.01f)
{
}

void SceneManager::StartScene(Graphics& gfx, std::string scene)
	
{
	
	// Add Particle System
	AddParticleSystem(gfx);
	
	// Create 3 sphere-filled squares. Pixel Shader visualization.
	FillSpheresAlgorithm(gfx, DirectX::XMFLOAT3{-57.0f, -10.0f, 15.0f}, 10, "Solid_RGBeqBMT_PS.cso", "Solid_RGBeqBTM_PS.cso", "");
	FillSpheresAlgorithm(gfx, DirectX::XMFLOAT3 {-47.0f, -10.0f, 15.0f}, 10, "Solid_RGBeqMTB_PS.cso", "Solid_RGBeqTMB_PS.cso", "");
	FillSpheresAlgorithm(gfx, DirectX::XMFLOAT3 {-37.0f, -10.0f, 15.0f}, 10, "Solid_RGBeqMBT_PS.cso", "Solid_RGBeqTBM_PS.cso", "");
	
	// Geometry + Pixel Shader visualization
	makeGeometrySphere(gfx);

	


	//this->partCount = 0;


}

void SceneManager::Update(float musParams[3], float timeFrame)
{
	//if (this->partCount < 100) {
	//	this->partCount++;
	
	testPS->Update(musParams, timeFrame);

	if (this->kdTimeForParticles < 0.0f) {
		this->EmitParticles(musParams);
		this->kdTimeForParticles = 0.01f;
	}
	this->kdTimeForParticles -= timeFrame;
	
	//}
}

void SceneManager::EmitParticles(float musParams[3])
{
	ParticleProps prop;

	DirectX::XMFLOAT4 colorB;

	colorB.x = 128.0f;
	colorB.y = 164.0f;
	colorB.z = 0.0f;
	colorB.w = 255.0f;


	prop.ColorBegin = colorB;

	DirectX::XMFLOAT4 colorE;

	colorE.x = 0.0f;
	colorE.y = 0.0f;
	colorE.z = 1000.0f;
	colorE.w = 0.0f;

	prop.ColorEnd = colorE;

	prop.LifeTime = 3.5f;
	prop.Position = testPS->getPos();
	prop.SizeBegin = 1.5f;
	prop.SizeEnd = 0.4f;
	prop.SizeVariation = 0.3f;


	DirectX::XMFLOAT3 Velocity;

	Velocity.x = 0.1f * musParams[0];
	Velocity.y = 0.1f * musParams[1];
	Velocity.z = 0.1f * musParams[2];

	prop.Velocity = Velocity;


	DirectX::XMFLOAT3 VelocityVariation;

	VelocityVariation.x = 0.2f;
	VelocityVariation.y = 0.2f;
	VelocityVariation.z = 0.2f;

	prop.VelocityVariation = VelocityVariation;
	testPS->Emit(prop);
}

void SceneManager::AddParticleSystem(Graphics& gfx)
{
	testPS =  std::make_shared<ParticleSystem>(gfx, DirectX::XMFLOAT3{ 0.000005f,0.00005f,0.00005f }, DirectX::XMFLOAT3{ 0.000005f, 0.000005f, 0.000005f }, DirectX::XMFLOAT4{ 255.0f,255.0f ,255.0f ,255.0f }, DirectX::XMFLOAT4{ 255.0f,255.0f ,255.0f ,255.0f }, 0.5f, 0.1f, 1.5f, 1.0f, "SolidVS.cso", "Solid_RGBeqBTM_PS.cso", DirectX::XMFLOAT3{ -50.0f, -5.0f, 10.0f });
}

void SceneManager::makeGeometrySphere(Graphics& gfx)
{
	ObjectFactory::getInstance().AddSphere(
			gfx,
			DirectX::XMFLOAT3{ -70.0f, -2.0f, 15.0f },
			0.4f, 20, 90, "SolidVS.cso",
			"Solid_RGBeqBMT_PS.cso",
			"PrettyExplodeGS.cso"
		);
}


///// <summary>
///// Special way of construction music visualization that utilizes 2 pixel shaders for different types of spheres.
///// </summary>
///// <param name="offset">Position of the array of spheres</param>
///// <param name="size">size of the spheres (NOT WORKING PROPERLY)</param>
///// <param name="shader_1"> Name of the first shader </param>
///// <param name="shader_2"> Name of the second shader</param>
///// <param name="dest"> destination array, which is passed by reference, stores unique pointers to sphere objects</param>
///// <param name="gs">Optional geometry shader</param>
void SceneManager::FillSpheresAlgorithm(Graphics& gfx, DirectX::XMFLOAT3 offset, int size, std::string shader_1, std::string shader_2, std::string gs)
{

	int start = 1;
	int max = size;
	const char* gs_c = nullptr;
	if (!gs.empty())
		gs_c = gs.c_str();
	while (start <= max) {
		std::string shader;
		if (start % 2 == 0) {
			shader = shader_1;
		}
		else
		{
			shader = shader_2;
		}
		for (int i = start; i <= max; i++) {
			for (int j = start; j <= max; j++) {
				if (i == start || i == max) {
					if (gs_c)
						ObjectFactory::getInstance().AddSphere(
							gfx,
							DirectX::XMFLOAT3{
								offset.x + 1.0f * j,
								offset.y + 1.0f * i,
								offset.z
							},
							0.4f,
							12, 24,
							"SolidVS.cso",
							shader.c_str(),
							gs_c
						);
					else
						ObjectFactory::getInstance().AddSphere(
							gfx,
							DirectX::XMFLOAT3{
								offset.x + 1.0f * j,
								offset.y + 1.0f * i,
								offset.z
							},
							0.4f,
							12, 24,
							"SolidVS.cso",
							shader.c_str()
						);
				}
				else {
					if (j == start || j == max) {
						if (gs_c)
							ObjectFactory::getInstance().AddSphere(
								gfx,
								DirectX::XMFLOAT3{
									offset.x + 1.0f * j,
									offset.y + 1.0f * i,
									offset.z
								},
								0.4f,
								12, 24,
								"SolidVS.cso",
								shader.c_str(),
								gs_c
							);
						else
							ObjectFactory::getInstance().AddSphere(
								gfx,
								DirectX::XMFLOAT3{
									offset.x + 1.0f * j,
									offset.y + 1.0f * i,
									offset.z
								},
								0.4f,
								12, 24,
								"SolidVS.cso",
								shader.c_str(),
								gs_c
							);

					}
				}
			}
		}
		max--;
		start++;
	}
}
