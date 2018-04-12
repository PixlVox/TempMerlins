#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

//Classes
#include"Time.h"
#include"Game.h"

HWND InitWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow){

	MSG msg = { 0 };

	//Classes
	Game game;
	Time time;

	//Create window
	HWND wndHandle = InitWindow(hInstance);

	if (wndHandle){

		ShowWindow(wndHandle, nCmdShow);

		//Init time
		time.initTime();

		//Init Game
		game.init(&hInstance, &wndHandle);

		while (WM_QUIT != msg.message){

			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)){

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else{

				//Update deltaTime
				time.update(wndHandle);

				//Update game
				game.update(time.getDeltaTime());
			
			}

		}

		DestroyWindow(wndHandle);
	}

	return (int)msg.wParam;

}

HWND InitWindow(HINSTANCE hInstance){

	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = L"TEMP";
	if (!RegisterClassEx(&wcex))
		return false;

	RECT rc = { 0, 0, WIN_WIDTH, WIN_HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	HWND handle = CreateWindow(
		L"TEMP",
		L"TEMP",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	return handle;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}