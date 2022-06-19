#include "GUIwrap.h"
#include "../Common/GUIconst.h"
#include "../FileDialog/ImGuiFileDialog.h"
#include "../Graphics/Bindable/Texture.h"
#include <implot.h>
#include <imgui.h>
#include "../Common/WNDconst.h"



GUIwrap::GUIwrap():
	slidersActive(false)
{
}
GUIwrap::~GUIwrap()
{
	playTexture.reset();
	pauseTexture.reset();
	BassTexture.reset();
	MidTexture.reset();
	TrebleTexture.reset();
}
std::string GUIwrap::getUpdatedWavFile()
{
	return filename;
}

void GUIwrap::CreateTexture(Graphics& gfx)
{

	this->makeStyle();
	//this->SceneFrameBuffer = Texture::Resolve(gfx, "FrameBuffer", true);
	this->playTexture = Texture::Resolve(gfx, PLAY_IMAGE);
	this->pauseTexture = Texture::Resolve(gfx, PAUSE_IMAGE);

	this->BassTexture = Texture::Resolve(gfx, BASS_IMAGE);
	this->MidTexture = Texture::Resolve(gfx, MID_IMAGE);
	this->TrebleTexture = Texture::Resolve(gfx, TREBLE_IMAGE);
}

void GUIwrap::FileDialogButton()

{
	auto* style = &ImGui::GetStyle();
	style->Colors[ImGuiCol_Text] = ImVec4(0.2f, 0.2f, 0.3f, 1.0f);
	if (this->FileDialogActive) {

		style->Colors[ImGuiCol_Button] = ImVec4(0.2f, 0.8f, 0.2f, 1.0f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1f, 0.9f, 0.1f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.4f, 0.8f, 0.4f, 1.00f);
	}
	else {
		style->Colors[ImGuiCol_Button] = ImVec4(0.8f, 0.8f, 0.2f, 1.0f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.9f, 0.9f, 0.1f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.8f, 0.8f, 0.4f, 1.00f);
	}

	
	if (ImGui::Button("Choose Music")) {
		this->FileDialogActive = !this->FileDialogActive;
	}

	style->Colors[ImGuiCol_Button] = ImVec4(0.1f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.9f, 1.00f);

		
	ImGui::SameLine(0.0f,100.0f);
}

void GUIwrap::DisplayRation(float params[3])
{

	float sum = params[0] + params[1] + params[2];
	std::string bass = "Bass: " + std::to_string((int)(params[0] * 100 / sum)) + "%";
	std::string Mid = "Mid: " + std::to_string((int)(params[1] * 100 / sum)) + "%";
	std::string Treble = "Treble: " + std::to_string((int)(params[2] * 100 / sum)) + "%";

	ImGui::Text(bass.c_str());   ImGui::SameLine(0.0f);
	ImGui::Text(Mid.c_str());    ImGui::SameLine(0.0f);
	ImGui::Text(Treble.c_str()); ImGui::SameLine(0.0f);
}

void GUIwrap::ViewIndicator(const int& indicator)
{

	auto* style = &ImGui::GetStyle();
	style->FrameRounding = 10.0f;
	style->Colors[ImGuiCol_Button] = ImVec4(0.2f, 0.2f, 0.8f, 1.0f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1f, 0.1f, 0.9f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.4f, 0.4f, 0.8f, 1.00f);
	if (indicator == 1) {
		ImGui::Button("Both View");
	}
	else if (indicator == 2) {
		ImGui::Button("First View");
	}
	else if (indicator == 3) {
		ImGui::Button("Second View");
	}
	else if (indicator == 0) {
		style->Colors[ImGuiCol_Text] = ImVec4(0.2f, 0.2f, 0.3f, 1.0f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.8f, 0.8f, 0.2f, 1.0f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.9f, 0.9f, 0.1f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.8f, 0.8f, 0.4f, 1.00f);
		ImGui::Button("Custom View");

	}

	ImGui::SameLine(0.0f, 100.0f);
	style->FrameRounding = 40.0f;
	style->Colors[ImGuiCol_Button] = ImVec4(0.1f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.9f, 1.00f);
}

void GUIwrap::showFFT(double freq[], double magn[], float musParams[3])
{
	
	ImGui::Begin("Plot Full");
	if (ImPlot::BeginPlot("My Plot Full", "Frequency", "Magnitude")) {
		ImPlot::PlotLine("My Line Plot Full", freq, magn, 1024);
		ImPlot::EndPlot();
	}


	ImGui::End();
}

void GUIwrap::DrawStatusBar(float params[3],
	bool& playing,
	const int& viewIndicator,
	bool displayPlay,
	bool fileDialogActive,
	bool slidersActive,
	bool displayRatio,
	bool displayIndicators,
	bool displayCamPos,
	float x,
	float y,
	float z,
	float yaw,
	float pitch,
	float roll)
{
	ImGui::Begin("Status Bar", nullptr,
		//ImGuiWindowFlags_NoMove +
		//ImGuiWindowFlags_NoResize +
		//ImGuiWindowFlags_NoCollapse +
		ImGuiWindowFlags_NoScrollbar);
		
	ImGui::SameLine(0.0f);	
		if (displayPlay)
			this->makePauseButton(playing);

		if(fileDialogActive)
			this->FileDialogButton();

		if(slidersActive)
			this->makeSliderButton();

		if(displayRatio)
			this->DisplayRation(params);

		if (displayIndicators)
			this->ViewIndicator(viewIndicator);

		if (displayCamPos)
			this->DisplayCamPos(x,y,z, yaw, pitch, roll);
		
		

	ImGui::End();
}

void GUIwrap::DrawFileDialog()
{
	// display
	if (this->FileDialogActive) {
		ImGuiFileDialog::Instance()->OpenDialog("Song List",
			"Choose File",
			".wav",
			"wavSamples\\",
			1,
			nullptr,
			ImGuiFileDialogFlags_HideColumnType +								// hide column file type
			ImGuiFileDialogFlags_HideColumnSize +								// hide column file size
			ImGuiFileDialogFlags_HideColumnDate +
			ImGuiFileDialogFlags_DisableCreateDirectoryButton);					// hide column file date);
	}
	else {
		ImGuiFileDialog::Instance()->Close();
	}
	if (ImGuiFileDialog::Instance()->Display("Song List"))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string directory = ImGuiFileDialog::Instance()->GetFilePathName();
			std::string fileInThedirectory = ImGuiFileDialog::Instance()->GetCurrentPath();

			std::size_t found = directory.rfind("wavSamples\\");
			if (found != std::string::npos)
				filename = directory.substr(found);
		}

		// close
		//this->FileDialogActive = false;
		ImGuiFileDialog::Instance()->Close();
	}
}

void GUIwrap::DrawSliders(float weightOfParams[3])
{
	if (this->slidersActive) {
		ImGui::Begin("Adjust Weight Of Parameters", nullptr,
			ImGuiWindowFlags_NoCollapse +
			ImGuiWindowFlags_NoScrollbar);

			ImGui::SliderFloat(" ", &weightOfParams[0], 0.0f, 1.0f);
			ImGui::SameLine();
			ImGui::Image(BassTexture->GetTextureView().Get(), ImVec2(72, 36));
			
			ImGui::SliderFloat("  ", &weightOfParams[1], 0.0f, 1.0f);
			ImGui::SameLine();
			ImGui::Image(MidTexture->GetTextureView().Get(), ImVec2(72, 36));

			ImGui::SliderFloat("   ", &weightOfParams[2], 0.0f, 1.0f);
			ImGui::SameLine(0.0f, 25.0f);
			ImGui::Image(TrebleTexture->GetTextureView().Get(), ImVec2(36, 36));

		ImGui::End();
	}
	
}


bool GUIwrap::isSlidersActive()
{
	return slidersActive;
}

bool GUIwrap::isFileDialogActive()
{
	return FileDialogActive;
}

void GUIwrap::setSlidersActive(bool status)
{
	slidersActive = status;
}

void GUIwrap::setFileDialogActive(bool status)
{
	FileDialogActive = status;
}

void GUIwrap::makeSliderButton()
{
	auto* style = &ImGui::GetStyle();
	style->Colors[ImGuiCol_Text] = ImVec4(0.2f, 0.2f, 0.3f, 1.0f);
	if (this->slidersActive) {
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1f, 0.9f, 0.1f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.4f, 0.8f, 0.4f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.2f, 0.8f, 0.2f, 1.0f);
	}
	else {

		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.9f, 0.9f, 0.1f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.8f, 0.8f, 0.4f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.8f, 0.8f, 0.2f, 1.0f);
	}

	if (ImGui::Button("Parameters")) {
		this->slidersActive = !this->slidersActive;
	}
	
	style->Colors[ImGuiCol_Button] = ImVec4(0.1f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.9f, 1.00f);
	ImGui::SameLine(0.0f, 420.0f);
}

void GUIwrap::makePauseButton(bool& playing)
{
	std::string bText = (playing) ? "Playing" : "Not Playing";

	auto* style = &ImGui::GetStyle();
	if (playing) {
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1f, 0.9f, 0.1f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.4f, 0.8f, 0.4f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.2f, 0.8f, 0.2f, 1.0f);
	}
	else {
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.9f, 0.9f, 0.1f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.8f, 0.8f, 0.4f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.8f, 0.8f, 0.2f, 1.0f);
	}
	style->Colors[ImGuiCol_Text] = ImVec4(0.2f, 0.2f, 0.3f, 1.0f);
	bool trigg=false;
	if (playing) {
		trigg = ImGui::ImageButton(pauseTexture->GetTextureView().Get(), ImVec2(24, 16));
	}
	else {
		trigg = ImGui::ImageButton(playTexture->GetTextureView().Get(), ImVec2(24, 16));
	}
	if (trigg) {
		playing = !playing;
	}
	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.9f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.1f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	ImGui::SameLine(0.0f, 450.0f);
}

void GUIwrap::DisplayCamPos(float x, float y, float z, float yaw, float pitch, float roll )
{

	std::string xS = "X: " + std::to_string(x);
	std::string yS = "Y: " + std::to_string(y);
	std::string zS = "Z: " + std::to_string(z);
	std::string yawS = "YAW: " + std::to_string(yaw);
	std::string pitchS = "PITCH: " + std::to_string(pitch);
	std::string rollS = "ROLL: " + std::to_string(roll);

	ImGui::Text(xS.c_str());  ImGui::SameLine(0.0f);
	ImGui::Text(yS.c_str());  ImGui::SameLine(0.0f);
	ImGui::Text(zS.c_str());  ImGui::SameLine(0.0f);
	ImGui::Text(yawS.c_str());  ImGui::SameLine(0.0f);
	ImGui::Text(pitchS.c_str());  ImGui::SameLine(0.0f);
	ImGui::Text(rollS.c_str());  ImGui::SameLine(0.0f);

}

void GUIwrap::EnableDocking()
{
	ImGuiIO& io = ImGui::GetIO();// (void) io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
}

void GUIwrap::showDemoWindow()
{
	bool show = true;
	ImGui::ShowDemoWindow(&show);
}

void GUIwrap::CreateDockSpace(bool* p_open)
{
		// If you strip some features of, this demo is pretty much equivalent to calling DockSpaceOverViewport()!
		// In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
		// In this specific demo, we are not using DockSpaceOverViewport() because:
		// - we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
		// - we allow the host window to have padding (when opt_padding == true)
		// - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport() in your code!)
		// TL;DR; this demo is more complicated than what you would normally use.
		// If we removed all the options we are showcasing, this demo would become:
		//     void ShowExampleAppDockSpace()
		//     {
		//         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		//     }

		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", p_open, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		else
		{
			//ShowDockingDisabledMessage();
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				ImGui::Separator();
				if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
					*p_open = false;
				ImGui::EndMenu();
			}
			//HelpMarker(
			//	"When docking is enabled, you can ALWAYS dock MOST window into another! Try it now!" "\n"
			//	"- Drag from window title bar or their tab to dock/undock." "\n"
			//	"- Drag from window menu button (upper-left button) to undock an entire node (all windows)." "\n"
			//	"- Hold SHIFT to disable docking (if io.ConfigDockingWithShift == false, default)" "\n"
			//	"- Hold SHIFT to enable docking (if io.ConfigDockingWithShift == true)" "\n"
			//	"This demo app has nothing to do with enabling docking!" "\n\n"
			//	"This demo app only demonstrate the use of ImGui::DockSpace() which allows you to manually create a docking node _within_ another window." "\n\n"
			//	"Read comments in ShowExampleAppDockSpace() for more details.");

			ImGui::EndMenuBar();
		}

		this->showDemoWindow();

		ImGui::End();

}

void GUIwrap::StartDockSpace(bool* p_open)
{
	// If you strip some features of, this demo is pretty much equivalent to calling DockSpaceOverViewport()!
// In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
// In this specific demo, we are not using DockSpaceOverViewport() because:
// - we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
// - we allow the host window to have padding (when opt_padding == true)
// - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport() in your code!)
// TL;DR; this demo is more complicated than what you would normally use.
// If we removed all the options we are showcasing, this demo would become:
//     void ShowExampleAppDockSpace()
//     {
//         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
//     }

	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", p_open, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImGui::GetMainViewport()->WorkSize, dockspace_flags);
	}
	else
	{
		//ShowDockingDisabledMessage();
	}
}

void GUIwrap::ToolBarMenu(bool* p_open)
{
	if (ImGui::BeginMenuBar())
	{   
		if (ImGui::BeginMenu("Options"))
		{
			ImGui::Separator();
			if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
				*p_open = false;
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

void GUIwrap::EndDockSpace()
{

	ImGui::End();
}

void GUIwrap::CreateViewPort()
{
	ImGui::Begin("Scene_1", nullptr);
		if(SceneFrameBuffer)
			ImGui::Image(SceneFrameBuffer, ImVec2(S_WIDTH, S_HEIGHT));
	ImGui::End();
}

void GUIwrap::resizeImGuiViewport()
{
	

}

//std::shared_ptr<Texture> GUIwrap::getFrameBuffer()
//{
//	return this->SceneFrameBuffer;
//}

void GUIwrap::setFrameBuffer(ID3D11ShaderResourceView* texView)
{
	this->SceneFrameBuffer = texView;
}

void GUIwrap::makeStyle()
{


	// Font taken from https://fonts.google.com/specimen/ZCOOL+QingKe+HuangYou
	ImGui::GetIO().Fonts->AddFontFromFileTTF("Fonts\\ZCOOLQingKeHuangYou-Regular.ttf", 15.0f);;



	auto* style = &ImGui::GetStyle();

	/// <summary>
	/// Visual interface (colors and etc) inspired by https://www.unknowncheats.me/forum/c-and-c-/189635-imgui-style-settings.html
	/// </summary>
	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(10, 10);
	style->FrameRounding = 40.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;
	style->Alpha = 1.0f;

	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.9f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.1f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Tab] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_TabActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
}
