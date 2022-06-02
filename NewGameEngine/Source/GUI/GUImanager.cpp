#include "GUImanager.h"
#include "GUIwrap.h"
#include "../Audio/AudioIO.h"


#include "../Common/WNDconst.h"
#include "../Common/GUIconst.h"

GUImanager::GUImanager()
{
	
}

void GUImanager::Update(float musParams[3], float weightOfParams[3], Window& wnd, Camera& cam, float timeFrame)
{
	
	this->ToggleCursor(wnd);

	this->kdTimer = (this->kdTimer == COOL_DOWN) ? COOL_DOWN : this->kdTimer + 1; // Handle "Cool Down for keyboard shortcuts" functionality.
	this->AudioWasPlaying = this->AudioIsPlaying;
	
	if (wnd.CursorEnabled) {

		this->HandleViews(wnd.kbd, cam);
		this->HandlePauseViaKeyboard(wnd.kbd);
		this->HandleActivatorsViaKeyboard(wnd.kbd);

		GUIwrap::getInstance().DrawStatusBar(musParams, AudioIsPlaying, ViewIndicator, true, true, true, false, true);
		GUIwrap::getInstance().DrawSliders(weightOfParams);
		GUIwrap::getInstance().DrawFileDialog();

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

void GUImanager::Start(Camera& cam)
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

void GUImanager::HandleViews(Keyboard& kbd, Camera& cam)
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

void GUImanager::ViewOne(Camera& cam)
{
	this->ViewIndicator = 1;
	cam.SetPosition(BothViewPos);
	cam.SetRotation(BothViewRot);
}

void GUImanager::ViewTwo(Camera& cam)
{
	this->ViewIndicator = 2;
	cam.SetPosition(GSPos);
	cam.SetRotation(GS_PS_Rot);
}

void GUImanager::ViewThree(Camera& cam)
{
	this->ViewIndicator = 3;
	cam.SetPosition(PSPos);
	cam.SetRotation(GS_PS_Rot);
}

void GUImanager::Control(Window& wnd, Camera& cam, float timeFrame)
{
	this->LookAround(wnd.mouse, cam);
	this->MoveAround(wnd.kbd, cam, timeFrame);
}

void GUImanager::LookAround(Mouse& mouse, Camera& cam)
{
	while (const auto ev = mouse.ReadRawDelta()) {
		cam.AdjustRotation((float)ev->y * 0.001f * this->speed_factor, (float)ev->x * 0.001f * this->speed_factor, 0);
	}
}

void GUImanager::MoveAround(Keyboard& kbd, Camera& cam, float speed_factor)
{
	if (kbd.KeyIsPressed('W'))
		cam.AdjustPosition(dx::XMVectorScale(cam.GetForwardVector(), speed_factor * 50.0f));

	if (kbd.KeyIsPressed('S'))
		cam.AdjustPosition(dx::XMVectorScale(cam.GetBackwardVector(), speed_factor * 50.0f));

	if (kbd.KeyIsPressed('A'))
		cam.AdjustPosition(dx::XMVectorScale(cam.GetLeftVector(), speed_factor * 50.0f));

	if (kbd.KeyIsPressed('D'))
		cam.AdjustPosition(dx::XMVectorScale(cam.GetRightVector(), speed_factor * 50.0f));

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