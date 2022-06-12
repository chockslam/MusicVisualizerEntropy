#include "GUImanager.h"
#include "GUIwrap.h"
#include "../Audio/AudioIO.h"


#include "../Common/WNDconst.h"
#include "../Common/GUIconst.h"

GUImanager::GUImanager()
{
	//GUIwrap::getInstance().EnableDocking();
}

void GUImanager::Update(float musParams[3], float weightOfParams[3], Window& wnd, ChiliCamera& cam, float timeFrame)
{
	
	this->ToggleCursor(wnd);

	this->kdTimer = (this->kdTimer == COOL_DOWN) ? COOL_DOWN : this->kdTimer + 1; // Handle "Cool Down for keyboard shortcuts" functionality.
	this->AudioWasPlaying = this->AudioIsPlaying;
	if (wnd.CursorEnabled) {

		this->HandleViews(wnd.kbd, cam);
		this->HandlePauseViaKeyboard(wnd.kbd);
		this->HandleActivatorsViaKeyboard(wnd.kbd);

		//GUIwrap::getInstance().DrawStatusBar(musParams, AudioIsPlaying, ViewIndicator, true, true, true, false, true);
		//GUIwrap::getInstance().DrawSliders(weightOfParams);
		//GUIwrap::getInstance().DrawFileDialog();
		//GUIwrap::getInstance().showDemoWindow();
		bool show = true;
		GUIwrap::getInstance().CreateDockSpace(&show);


		// Play New .wav file if it was chosen from file dialog.
		if (!GUIwrap::getInstance().getUpdatedWavFile().empty() && GUIwrap::getInstance().getUpdatedWavFile() != wavFileName) {
			playNewFile();
		}

		// Handle Play/Pause.
		if (AudioToggled()) {
			if (this->AudioIsPlaying) {
				AudioIO::getInstance().PlayPausedAudio();
			}
			else {
				AudioIO::getInstance().PauseAudio();
			}
		}
	}
	else{
		this->ViewIndicator = 0;
		this->Control(wnd, cam, timeFrame);
	}
	
}

void GUImanager::Start(ChiliCamera& cam)
{
	this->ViewOne(cam); // Start with the first view.

	// Play Default Audio when program is started.
	if (AudioIO::getInstance().OpenFile(WAV_FILE)) {
		AudioIO::getInstance().PlayAudio();
	}
	else {
		MessageBox(0, "Failed to Load Audio", 0, 0);
	}
	this->speed_factor = 10.0f;
	this->ViewIndicator = 1;
}

void GUImanager::LoadTextures(Graphics& gfx)
{
	GUIwrap::getInstance().CreateTexture(gfx);
}

bool GUImanager::AudioToggled()
{
	return AudioIsPlaying != AudioWasPlaying;
}

void GUImanager::HandlePauseViaKeyboard(Keyboard& kbd)
{
	if (kbd.KeyIsPressed('P') && (this->kdTimer % COOL_DOWN == 0)) {
		this->kdTimer = 0;
		this->AudioIsPlaying = !this->AudioIsPlaying;
	}
}

void GUImanager::HandleActivatorsViaKeyboard(Keyboard& kbd)
{
	if (kbd.KeyIsPressed('A') && (this->kdTimer % COOL_DOWN == 0)) {
		this->kdTimer = 0;
		GUIwrap::getInstance().setSlidersActive(!GUIwrap::getInstance().isSlidersActive());
	}
	if (kbd.KeyIsPressed('F') && (this->kdTimer % COOL_DOWN == 0)) {
		this->kdTimer = 0;
		GUIwrap::getInstance().setFileDialogActive(!GUIwrap::getInstance().isFileDialogActive());
	}
}

void GUImanager::HandleViews(Keyboard& kbd, ChiliCamera& cam)
{
	if (kbd.KeyIsPressed('1')) {
		this->ViewOne(cam);
	}
	else
	if (kbd.KeyIsPressed('2')) {
		this->ViewTwo(cam);
	}
	else
	if (kbd.KeyIsPressed('3')) {
		this->ViewThree(cam);
	}
}

void GUImanager::ViewOne(ChiliCamera& cam)
{
	this->ViewIndicator = 1;
	cam.setPos(BothViewPos);
	cam.setOrient(BothViewRot.x, BothViewRot.y);
}

void GUImanager::ViewTwo(ChiliCamera& cam)
{
	this->ViewIndicator = 2;
	cam.setPos(GSPos);
	cam.setOrient(GS_PS_Rot.x, GS_PS_Rot.y);
}

void GUImanager::ViewThree(ChiliCamera& cam)
{
	this->ViewIndicator = 3;
	cam.setPos(PSPos);
	cam.setOrient(GS_PS_Rot.x, GS_PS_Rot.y);
}

void GUImanager::Control(Window& wnd, ChiliCamera& cam, float timeFrame)
{
	this->LookAround(wnd.mouse, cam, timeFrame);
	this->MoveAround(wnd.kbd, cam, timeFrame);
}

void GUImanager::LookAround(Mouse& mouse, ChiliCamera& cam, float timeframe)
{
	while (const auto ev = mouse.ReadRawDelta()) {
		cam.Rotate((float)ev->x * timeframe, (float)ev->y * timeframe);
	}
}

void GUImanager::MoveAround(Keyboard& kbd, ChiliCamera& cam, float speed_factor)
{
	if (kbd.KeyIsPressed('W'))
		// cam.AdjustPosition(dx::XMVectorScale(cam.GetForwardVector(), speed_factor * 50.0f));
		cam.Translate({0.0f,0.0f,speed_factor});
	if (kbd.KeyIsPressed('S'))
		// cam.AdjustPosition(dx::XMVectorScale(cam.GetBackwardVector(), speed_factor * 50.0f));
		cam.Translate({ 0.0f,0.0f,-speed_factor });

	if (kbd.KeyIsPressed('A'))
		// cam.AdjustPosition(dx::XMVectorScale(cam.GetLeftVector(), speed_factor * 50.0f));
		cam.Translate({ -speed_factor,0.0f,0.0f });
	if (kbd.KeyIsPressed('D'))
		// cam.AdjustPosition(dx::XMVectorScale(cam.GetRightVector(), speed_factor * 50.0f));
		cam.Translate({ speed_factor,0.0f,0.0f });
}

void GUImanager::ToggleCursor(Window& wnd)
{
	while (const auto e = wnd.kbd.ReadKey())
	{
		if (!e->IsPress())
		{
			continue;
		}

		switch (e->GetCode())
		{
		case VK_ESCAPE:
			if (wnd.CursorEnabled)
			{
				wnd.DisableCursor();
				wnd.mouse.EnableRaw();
			}
			else
			{
				wnd.EnableCursor();
				wnd.mouse.DisableRaw();
			}
			break;
		}
	}
}

void GUImanager::playNewFile()
{
	wavFileName = GUIwrap::getInstance().getUpdatedWavFile();
	AudioIO::getInstance().SwitchAudioFile(wavFileName);
	AudioIsPlaying = true;
}