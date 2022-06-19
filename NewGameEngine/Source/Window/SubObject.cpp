/// CODE was written with help by OlympusMonsTutorial (https://www.youtube.com/c/OlympusMonsTutorials)
#include "SubObject.h"

SubObject::SubObject(std::string className, std::string classTitle, HICON icon)
	: m_Class(className), m_Title(classTitle), m_Hicon(icon), m_Inst(GetModuleHandle(nullptr))
{

}

SubObject::~SubObject()
{
}

VOID SubObject::RegisterNewClass() {
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); //size of the window in bits
	wcex.style = CS_CLASSDC;//CS_HREDRAW | CS_VREDRAW; //vertical and horizontal drawing
	wcex.cbClsExtra = 0; // extra memory for the program
	wcex.cbWndExtra = 0;

	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW); //defining cursor for the window
	wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(46,46,46)); //white background

	wcex.hIcon = m_Hicon; // task bar icon
	wcex.hIconSm = m_Hicon; // top-left corner icon

	wcex.lpszClassName = m_Class.c_str(); //defining class name

	wcex.lpszMenuName = nullptr; //not needed for game window

	wcex.hInstance = m_Inst; //instead of refererring to the function variable we refer to GetModuleHandle(NULL)

	wcex.lpfnWndProc = SetupMessageHandler; //our window istructions

	RegisterClassEx(&wcex); //Registering a class

}
LRESULT SubObject::SetupMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE) 
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		SubObject* const pWnd = static_cast<SubObject*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&SubObject::AssignMessageHandler));
		return pWnd->MessageHandler(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT SubObject::AssignMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SubObject* const pWnd = reinterpret_cast<SubObject*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->MessageHandler(hWnd, msg, wParam, lParam);
}
LRESULT SubObject::CommonMessageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, message, wParam, lParam);
}




