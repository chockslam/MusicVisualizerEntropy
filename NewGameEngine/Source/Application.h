#pragma once
#include "Window/Window.h"
#include "Graphics/ChiliCamera.h"
#include "Common/GUIconst.h"
#include "Common/Timer.h"
#include "Graphics/Drawable/WrapperSolidSphere.h"
#include "Audio/AudioIO.h"
#include "GUI/GUIwrap.h"
#include "ObjectManager.h"
#include "GUI/GUImanager.h"
#include "../resource.h"
//#include ""


/// <summary>
/// Class represents the Application itself.
/// </summary>
class Application
{
public:
	Application();
	int Go(); // used in main function
	~Application();
private:
	void DoFrame(float timeFrame); // Executes every frame
private:
	// Variables represents window.
	Window wnd;

	// Represents camera.
	ChiliCamera cam;


	// Represents a game timer
	Timer timer;
	float speed_factor;


	// Average of Bass, Mid and Treble domains generated by algorithm created in AudioIO.h
	float musParams[3];


	// Weigth of music parameters. Defined by user.
	float weightOfParams[3]{ 1.0f,1.0f,1.0f };

	
	// Facilitate a delay between presses of buttons.
	int kdTimer;

	
	// Integer that represent the current view.
	int ViewIndicator;

	std::shared_ptr<ObjectManager> om;
	std::shared_ptr<GUImanager> gm;

	// Constants reflect position and rotation of camera for each view. 
	const DirectX::XMVECTOR BothViewRot = { 0.07f,0.44999f,0.0f };
	const DirectX::XMVECTOR BothViewPos = { -66.831947f, -3.133941f, -2.391678f };
	const DirectX::XMVECTOR GS_PS_Rot = { 0.09f, 0.01999f, 0.0f };
	const DirectX::XMVECTOR GSPos = { -68.13694f, -3.833905f, 4.795311f };
	const DirectX::XMVECTOR PSPos = { -41.915050f, -3.93337f, -1.328098f };
	

};