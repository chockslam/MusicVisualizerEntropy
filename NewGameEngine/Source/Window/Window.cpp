/// CODE was written with help by OlympusMonsTutorial (https://www.youtube.com/c/OlympusMonsTutorials) and ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle)

#include "Window.h"
Window::Window(std::string WindowName,HICON icon, INT width, INT height)
	: SubObject("EngineContainer", WindowName, icon), m_Width(width), m_Height(height)
{
	Initialize(WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU);
	SetupRawInput();
	pGfx = std::make_unique<Graphics>(m_Handle);
	ImGuiIO& io = ImGui::GetIO(); //(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

Window::~Window()
{
	ImGui_ImplWin32_Shutdown();
	

}

VOID Window::Initialize(UINT type)
{
	RegisterNewClass();

	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);

	RECT R = { 0, 0, m_Width, m_Height };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;
	
	m_Handle = CreateWindow(m_Class.c_str(), m_Title.c_str(), type,
		((desktop.right/2) - (m_Width/2)), ((desktop.bottom / 2) - (m_Height / 2)), m_Width, m_Height, nullptr, nullptr, m_Inst, (void*)this);
	
	if (!m_Handle) {
		MessageBox(0, "Failed to Create Window!", 0, 0);
		PostQuitMessage(0); //close window
	}
		
	ShowWindow(m_Handle, SW_SHOW);
	

		

}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

LRESULT Window::MessageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
			// we don't want the DefProc to handle this message because
			// we want our destructor to destroy the window, so return 0 instead of break
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;


			// clear keystate when window loses focus to prevent input getting "stuck"
		case WM_KILLFOCUS:
			kbd.ClearState();
			break;
		case WM_ACTIVATE:
			// confine/free cursor on window to foreground/background if cursor disabled
			if (!CursorEnabled)
			{
				if (wParam & WA_ACTIVE)
				{
					ConfineCursor();
					HideCursor();
				}
				else
				{
					FreeCursor();
					ShowCursor();
				}
			}
			break;

			/*********** KEYBOARD MESSAGES ***********/
		case WM_KEYDOWN:
			// syskey commands need to be handled to track ALT key (VK_MENU) and F10
		case WM_SYSKEYDOWN:
			// stifle this keyboard message if imgui wants to capture
			if (ImGui::GetIO().WantCaptureKeyboard)
			{
				break;
			}
			if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled()) // filter autorepeat
			{
				kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
			}
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			// stifle this keyboard message if imgui wants to capture
			if (ImGui::GetIO().WantCaptureKeyboard)
			{
				break;
			}
			kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
			break;
		case WM_CHAR:
			// stifle this keyboard message if imgui wants to capture
			if (ImGui::GetIO().WantCaptureKeyboard)
			{
				break;
			}
			kbd.OnChar(static_cast<unsigned char>(wParam));
			break;
			/*********** END KEYBOARD MESSAGES ***********/

			/************* MOUSE MESSAGES ****************/
		case WM_MOUSEMOVE:
		{

			const POINTS pt = MAKEPOINTS(lParam);
			// cursorless exclusive gets first dibs
			if (!CursorEnabled)
			{
				if (!mouse.IsInWindow())
				{
					SetCapture(hWnd);
					mouse.OnMouseEnter();
					HideCursor();
				}
			}
			// stifle this mouse message if imgui wants to capture
			if (ImGui::GetIO().WantCaptureMouse)
			{
				break;
			}
			// in client region -> log move, and log enter + capture mouse (if not previously in window)
			if (pt.x >= 0 && pt.x < m_Width && pt.y >= 0 && pt.y < m_Height)
			{
				mouse.OnMouseMove(pt.x, pt.y);
				if (!mouse.IsInWindow())
				{
					SetCapture(hWnd);
					mouse.OnMouseEnter();
				}
			}
			// not in client -> log move / maintain capture if button down
			else
			{
				if (wParam & (MK_LBUTTON | MK_RBUTTON))
				{
					mouse.OnMouseMove(pt.x, pt.y);
				}
				// button up -> release capture / log event for leaving
				else
				{
					ReleaseCapture();
					mouse.OnMouseLeave();
				}
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			SetForegroundWindow(hWnd);
			if (!CursorEnabled)
			{
				ConfineCursor();
				HideCursor();
			}
			// stifle this mouse message if imgui wants to capture
			if (ImGui::GetIO().WantCaptureMouse)
			{
				break;
			}
			
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnLeftPressed(pt.x, pt.y);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			// stifle this mouse message if imgui wants to capture
			if (ImGui::GetIO().WantCaptureMouse)
			{

				break;
			}
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnRightPressed(pt.x, pt.y);
			break;
		}
		case WM_LBUTTONUP:
		{
			// stifle this mouse message if imgui wants to capture
			if (ImGui::GetIO().WantCaptureMouse)
			{
				break;
			}
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnLeftReleased(pt.x, pt.y);
			// release mouse if outside of window
			if (pt.x < 0 || pt.x >= m_Width || pt.y < 0 || pt.y >= m_Height)
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
			break;
		}
		case WM_RBUTTONUP:
		{
			// stifle this mouse message if imgui wants to capture
			if (ImGui::GetIO().WantCaptureMouse)
			{
				break;
			}
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnRightReleased(pt.x, pt.y);
			// release mouse if outside of window
			if (pt.x < 0 || pt.x >= m_Width || pt.y < 0 || pt.y >= m_Height)
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
			break;
		}
		case WM_MOUSEWHEEL:
		{
			// stifle this mouse message if imgui wants to capture
			if (ImGui::GetIO().WantCaptureMouse)
			{
				break;
			}
			const POINTS pt = MAKEPOINTS(lParam);
			const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
			mouse.OnWheelDelta(pt.x, pt.y, delta);
			break;
		}
	case WM_INPUT:
	{
		if (!mouse.RawEnabled())
		{
			break;
		}
		UINT dataSize;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER)); //Need to populate data size first
	
		if (dataSize > 0)
		{
			std::unique_ptr<BYTE[]> rawdata = std::make_unique<BYTE[]>(dataSize);
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawdata.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
			{
				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.get());
				if (raw->header.dwType == RIM_TYPEMOUSE && 
					( raw->data.mouse.lLastX != 0 || raw->data.mouse.lLastY != 0 ) )
				{
					mouse.OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
				}
			}
		}

			break;
		}

	
	}
	return CommonMessageHandler(hWnd, message, wParam, lParam);
}

Graphics& Window::Gfx()
{
	return *pGfx;
}

std::optional<int> Window::ProcessMessages() 
{
	MSG msg;
	// while queue has messages, remove and dispatch them (but do not block on empty queue)
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		// check for quit because peekmessage does not signal this via return val
		if (msg.message == WM_QUIT)
		{
			// return optional wrapping int (arg to PostQuitMessage is in wparam) signals quit
			return (int)msg.wParam;
		}

		// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// return empty optional when not quitting app
	return {};
}

void Window::EnableCursor()
{

	CursorEnabled = true;
	ShowCursor();
	EnableImGuiMouse();
	FreeCursor();
}

void Window::DisableCursor()
{

	CursorEnabled = false;
	HideCursor();
	DisableImGuiMouse();
	ConfineCursor();
}

void Window::SetupRawInput()
{
	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01; // mouse page
	rid.usUsage = 0x02; // mouse usage
	rid.dwFlags = 0;
	rid.hwndTarget = nullptr;
	if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
	{
		OutputDebugString("Error setting up Raw input device");
	}
}

void Window::HideCursor()
{
	while (::ShowCursor(FALSE) >= 0);
}

void Window::ShowCursor()
{
	while (::ShowCursor(TRUE) < 0);
}

void Window::EnableImGuiMouse()
{
	ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
}

void Window::ConfineCursor()
{
	RECT rect;
	GetClientRect(GetHandle(), &rect);
	MapWindowPoints(GetHandle(), nullptr, reinterpret_cast<POINT*>(&rect), 2);
	ClipCursor(&rect);
}

void Window::FreeCursor()
{
	ClipCursor(nullptr);
}

void Window::DisableImGuiMouse()
{
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
}

