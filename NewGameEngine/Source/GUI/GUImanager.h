#pragma once 
#include <string>
#include "../Window/Window.h"
#include "../Graphics/ChiliCamera.h"
class Graphics;
class GUImanager {
public:
	GUImanager();
	void Update(float musParams[3], float weightOfParams[3], Window& wnd, ChiliCamera& cam, float timeFrame);
	void Start(ChiliCamera& cam);

	void LoadTextures(Graphics& gfx);

private:
	bool AudioToggled();		// Checks whether the audio was toggled between "playing" and "paused"
	void HandlePauseViaKeyboard(Keyboard& kbd);
	void HandleActivatorsViaKeyboard(Keyboard& kbd);
	void HandleViews(Keyboard& kbd, ChiliCamera& cam);		// Facilitates switching between views.
	void ViewOne(ChiliCamera& cam);			// Go to View One
	void ViewTwo(ChiliCamera& cam);			// Go to View Two
	void ViewThree(ChiliCamera& cam);			// Go to View Three
	void Control(Window& wnd, ChiliCamera& cam, float timeFrame);
	void LookAround(Mouse& mouse, ChiliCamera& cam, float timeframe);
	void MoveAround(Keyboard& kbd, ChiliCamera& cam, float speed_factor);
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
	const DirectX::XMFLOAT3 BothViewPos = { -66.831947f, -3.133941f, -2.391678f };
	const DirectX::XMFLOAT3 GSPos = { -68.13694f, -3.833905f, 4.795311f };
	const DirectX::XMFLOAT3 PSPos = { -41.915050f, -3.93337f, -1.328098f };

	const DirectX::XMFLOAT2 GS_PS_Rot = { 0.09f, 0.01999f };
	const DirectX::XMFLOAT2 BothViewRot = { 0.07f,0.44999f };


};