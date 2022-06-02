#include "SceneManager.h"
#include "ObjectFactory.h"


SceneManager::SceneManager()
	:
	partCount(0)
{
}

void SceneManager::StartScene(Graphics& gfx, std::string scene)
{
	
	// Add Particle System
	AddParticleSystem(gfx);
	
	// Create 3 sphere-filled squares. Pixel Shader visualization.
	FillSpheresAlgorithm(gfx,new float[] {-57.0f, -10.0f, 15.0f}, 10, "Solid_RGBeqBMT_PS.cso", "Solid_RGBeqBTM_PS.cso", "");
	FillSpheresAlgorithm(gfx ,new float[] {-47.0f, -10.0f, 15.0f}, 10, "Solid_RGBeqMTB_PS.cso", "Solid_RGBeqTMB_PS.cso", "");
	FillSpheresAlgorithm(gfx, new float[] {-37.0f, -10.0f, 15.0f}, 10, "Solid_RGBeqMBT_PS.cso", "Solid_RGBeqTBM_PS.cso", "");
	
	// Geometry + Pixel Shader visualization
	makeGeometrySphere(gfx);

	//this->partCount = 0;


}

void SceneManager::Update()
{
	//if (this->partCount < 100) {
	//	this->partCount++;
		ObjectFactory::getInstance().getOM()->EmitParticles();
	//}
}

void SceneManager::AddParticleSystem(Graphics& gfx)
{
	ObjectFactory::getInstance().AddParticleSystem(gfx, { 1.0f,1.0f,1.0f }, { 2.0f, 2.0f, 2.0f }, {255.0f,255.0f ,255.0f ,255.0f }, { 255.0f,255.0f ,255.0f ,255.0f },0.5f,0.1f, 1.5f, 1.0f, "SolidVS.cso", "Solid_RGBeqBTM_PS.cso", new float[3]{ -50.0f, -1.0f, 10.0f });
}

void SceneManager::makeGeometrySphere(Graphics& gfx)
{
	ObjectFactory::getInstance().AddSphere(
			gfx,
			new float[3]{ -70.0f, -2.0f, 15.0f },
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
void SceneManager::FillSpheresAlgorithm(Graphics& gfx, float offset[3], int size, std::string shader_1, std::string shader_2, std::string gs)
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
							new float[3]{
								offset[0] + 1.0f * j,
								offset[1] + 1.0f * i,
								offset[2]
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
							new float[3]{
								offset[0] + 1.0f * j,
								offset[1] + 1.0f * i,
								offset[2]
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
								new float[3]{
									offset[0] + 1.0f * j,
									offset[1] + 1.0f * i,
									offset[2]
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
								new float[3]{
									offset[0] + 1.0f * j,
									offset[1] + 1.0f * i,
									offset[2]
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
	if (offset) {
		delete[] offset;
		offset = nullptr;
	}
}
