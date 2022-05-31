#pragma once 
#include <string>
#include "../Window/Window.h"
#include "../Graphics/NewCamera.h"
class Graphics;
class GUImanager {
public:
	GUImanager();
	void Update(float musParams[3], float weightOfParams[3], Window& wnd, Camera& cam);
	void LoadTextures(Graphics& gfx);

private:
	bool AudioToggled();		// Checks whether the audio was toggled between "playing" and "paused"
	void HandlePauseViaKeyboard(Keyboard& kbd);
	void HandleActivatorsViaKeyboard(Keyboard& kbd);
	void HandleViews(Keyboard& kbd, Camera& cam);		// Facilitates switching between views.
	void ViewOne(Camera& cam);			// Go to View One
	void ViewTwo(Camera& cam);			// Go to View Two
	void ViewThree(Camera& cam);			// Go to View Three
	void Control(Window& wnd, Camera& cam);
	void LookAround(Mouse& mouse, Camera& cam);
	void MoveAround(Keyboard& kbd, Camera& cam);
	void ToggleCursor(Window& wnd);

	void playNewFile();
	// Facilitate a delay between presses of buttons.
	int kdTimer;

	// Variables that facilitate Play/Pause audio functionality
	bool AudioIsPlaying;
	bool AudioWasPlaying;

	// Speed factor represents the speed of camera.
	float speed_factor;
	//Keyboard kbd;

	// Integer that represent the current view.
	int ViewIndicator;

	std::string wavFileName;

	// Constants reflect position and rotation of camera for each view. 
	const DirectX::XMVECTOR BothViewRot = { 0.07f,0.44999f,0.0f };
	const DirectX::XMVECTOR BothViewPos = { -66.831947f, -3.133941f, -2.391678f };
	const DirectX::XMVECTOR GS_PS_Rot = { 0.09f, 0.01999f, 0.0f };
	const DirectX::XMVECTOR GSPos = { -68.13694f, -3.833905f, 4.795311f };
	const DirectX::XMVECTOR PSPos = { -41.915050f, -3.93337f, -1.328098f };


};