/// CODE was written with help by OlympusMonsTutorial (https://www.youtube.com/c/OlympusMonsTutorials) and ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle)


#pragma once

#include "SubObject.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <optional>
#include <memory>
#include "../Graphics/Graphics.h"
#include <imgui_impl_win32.h>
//#include "AudioIO.h"

/// <summary>
/// Class that represent the Window. Child class of SubObject, which represents any window-like object, for example Splash Screen.
/// </summary>
class Window : public SubObject {
public:
	Window(std::string WindowName,HICON icon , INT width, INT height);
	~Window();	
	virtual VOID Initialize(UINT type) override;																// Initialize window.
	virtual LRESULT			 MessageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;	// Message Loop.
	Graphics& Gfx();																							// Returns pointer to graphics object.
	static std::optional<int> ProcessMessages() ;
	void EnableCursor();
	void DisableCursor();
	int getWidth() { return this->m_Width; };
	int getHeight() { return this->m_Height; };
private:
	void SetupRawInput();		// Set-up Raw mouse input, which needed for "Look - Around" functionality implemented for camera movement. 
	void HideCursor();			
	void ShowCursor();			
	void DisableImGuiMouse();
	void EnableImGuiMouse();
	void ConfineCursor();
	void FreeCursor();
public:
	Keyboard kbd;					// Represents Keyboard
	Mouse mouse;					// Represents Mouse
	bool CursorEnabled;			
protected:
	INT	m_Width;					// width of the window
	INT	m_Height;					// height of the window
	std::unique_ptr<Graphics> pGfx; // Pointer to Graphics object that handles DirectX11 initialization.
};