// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
#include "SoftwareEngine.h"

#define WINDOW_WIDTH 1920 / 2
#define WINDOW_HEIGHT 1080 /2


// type cast : LPWSTR to WORD warning disable
#pragma warning(disable: 4302)

CSoftwareEngine* gSoftEngine = nullptr;

// equal to window name
static const LPTSTR WINDOW_CLASS_NAME = TEXT("SoftwareEngine");
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int CALLBACK WinMain(_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = (HICON)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = WINDOW_CLASS_NAME;
	wcex.hIconSm = (HICON)LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, TEXT("Call to RegisterClassEx failed!"), WINDOW_CLASS_NAME, NULL);
		return 1;
	}

	HWND hWnd = CreateWindowEx(NULL,
		WINDOW_CLASS_NAME,
		WINDOW_CLASS_NAME,
		WS_OVERLAPPEDWINDOW,
		0, 0,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);
	if (hWnd == NULL)
	{
		MessageBox(NULL, TEXT("Create window failed!"), WINDOW_CLASS_NAME, NULL);
		return 1;
	}

	// Init
	gSoftEngine = new CSoftwareEngine(hWnd, WINDOW_WIDTH, WINDOW_HEIGHT);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;

	while (true)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE) == FALSE)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if (msg.message == WM_QUIT)
		{
			break;
		}
		else
		{
			// Update
			// ...
			gSoftEngine->Update();
			gSoftEngine->Draw();
		}
	}


	// clear application
	// ...
	SAFE_DELETE(gSoftEngine);

	return 0;
}


LRESULT CALLBACK WndProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	switch (_msg)
	{
	case WM_PAINT:
	{
	/*	for (int i = 0; i < WINDOW_WIDTH*WINDOW_HEIGHT * 0.5f; ++i)
		{
			gSoftEngine->mDrawBuffer[i] = RGB(255, 255, 0);
		}*/

		//HDC hdc;
		//PAINTSTRUCT ps;
		//
		//hdc = BeginPaint(_hWnd, &ps);
		//BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, gSoftEngine->hDibDrawTextureDC, 0, 0, SRCCOPY);
		//EndPaint(_hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(NULL);
		break;
	default:
		break;
	}
	return DefWindowProc(_hWnd, _msg, _wParam, _lParam);
}
