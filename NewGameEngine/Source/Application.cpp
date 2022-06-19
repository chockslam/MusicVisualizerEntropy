#include "Application.h"
#include "Graphics/EMaths.h"
#include <memory>
#include <algorithm>
#include <iterator>
#include "Common/GDIPlusManager.h"
#include "Audio/AudioIO.h"
#include "Common/WNDconst.h"
#include "ObjectFactory.h"

GDIPlusManager gdipm;

namespace dx = DirectX;

Application::Application()
	:
	wnd("Entropy",
		0,
		W_WIDTH,
		W_HEIGHT),
	speed_factor(0.2f),
	kdTimer(0)
{
	wnd.Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 100.0f));	// Set initial projection matrix
	wnd.EnableCursor(); // Enable Cursor

	this->gm = std::make_shared<GUImanager>();
	this->gm->LoadTextures(wnd.Gfx());
	this->gm->Start(cam, wnd);
	
	this->om = std::make_shared<ObjectManager>();
	ObjectFactory::getInstance().SetUpObjectManager(om);
	ObjectFactory::getInstance().SetUpLevelManager();
	this->om->startScene(wnd.Gfx(), "Scene_1");

	
}


int Application::Go()
{
	//Start Message Loop/
	while (true)
	{
		// process all messages pending, but to not block for new messages
		if (const auto ecode = wnd.ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}


		const auto frameTime = this->timer.Mark() * this->speed_factor;

		DoFrame(frameTime);// Do Frame.
	}
}

Application::~Application()
{
	Codex::ResetAll();
}

void Application::DoFrame(float timeFrame)
{
	// Musparams update.
	musParams[0] = static_cast<float>(AudioIO::getInstance().audio->averageB) * weightOfParams[0];
	musParams[1] = static_cast<float>(AudioIO::getInstance().audio->averageM) * weightOfParams[1];
	musParams[2] = static_cast<float>(AudioIO::getInstance().audio->averageT) * weightOfParams[2];
	
	//wnd.Gfx().BeginGUIFrame(0.07f, 0.0f, 0.12f);
	//	
	//wnd.Gfx().EndGUIFrame();

	// Begin Frame with background colour = rgb(0.07,0.0,0.12)
	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);

		wnd.Gfx().SetCamera(cam.GetMatrix()); // Update Projection with camera view.	
		this->gm->Update(musParams, weightOfParams, wnd, cam, timeFrame);
		this->om->UpdateAll(wnd.Gfx(), cam.GetMatrix(), musParams, timeFrame);
		this->om->RenderAll(wnd.Gfx());
	
	wnd.Gfx().EndFrame(0.07f, 0.0f, 0.12f);
}



