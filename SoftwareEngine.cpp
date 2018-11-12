#include "stdafx.h"
#include "SoftwareEngine.h"



CSoftwareEngine::CSoftwareEngine(HWND _hWnd, UINT _windowWidth, UINT _windowHeight)
	: mHwnd(_hWnd), mWindowWidth(_windowWidth), mWindowHeight(_windowHeight)
{
	mDrawBufferSize = mWindowWidth * mWindowHeight;
	mAspectRatio = mWindowHeight / static_cast<float>(mWindowWidth);
	mWidthRatio = static_cast<float>(1) / mWindowWidth;

	//버그가 있었는데 Rendering에서 렌더 비트맵 버퍼에 접근하여 값을 변경하고
	// Update에서 InvalidRgn을 호출하여 
	//WM_PAINT가 발생 될때 멤버 함수를 통해 BeginPaint,Bitblt,EndPaint 해주었을때 동작하지 않았다.

	// # 주의 #
	// CreateDIBSection을 통해 얻어온 비트맵은 아래쪽부터 시작한다
	// ex mDrawBuffer[100(x값) + 0(y값)]이면 화면 맨아래의 왼쪽부터 100칸째 픽셀의 색이 바뀐다.

	// SetBitmap을이용해 픽셀 하나하나 값을 지정해주기보단
	// 렌더버퍼의 비트맵 주소에 직접 접근하여 값을 할당해주는 것이 속도가 훨씬 빨랐다
	// 응용 프로그램이 지접 쓸 수있는 장치 독릭접 비트맵 (DIB)을 만듭니다.
	// (CreateDIBSection)
	// 드로우 버퍼(DX의 렌더타겟 생성)
	BITMAPINFO bmi;
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = mWindowWidth;
	bmi.bmiHeader.biHeight = mWindowHeight;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	HDC hDesktopDC = GetDC(mHwnd);
	hDibDrawTexture = CreateDIBSection(hDesktopDC, &bmi, DIB_RGB_COLORS, (void **)&mDrawBuffer, 0, 0);
	if (mDrawBuffer == NULL)
	{
		MessageBox(NULL, TEXT("backbuffer create failed!"), TEXT("Software Engine"), MB_OK);
	}
	hDibDrawTextureDC = CreateCompatibleDC(hDesktopDC);
	SelectObject(hDibDrawTextureDC, hDibDrawTexture);
		
	ReleaseDC(mHwnd, hDesktopDC);
}


CSoftwareEngine::~CSoftwareEngine()
{
	DeleteDC(hDibDrawTextureDC);
	DeleteObject(hDibDrawTexture);
}

void CSoftwareEngine::Update()
{
	Rendering();
	Draw();
}

void CSoftwareEngine::Draw()
{
	static HDC hdc;
	hdc = GetDC(mHwnd);
	BitBlt(hdc, 0, 0, mWindowWidth, mWindowHeight, hDibDrawTextureDC, NULL, NULL, SRCCOPY);
	ReleaseDC(mHwnd, hdc);
}

void CSoftwareEngine::Rendering()
{
	static int lines[2][4]=
	{
		//from		to
		{ 0,0	 ,  10,10 },
		{ 10,10  ,  50,10 }
	};
	static COLORREF color[2] =
	{
		RGB(255,0,0),
		RGB(0,255,0)
	};

	for (int i = 0; i < 2; ++i)
	{
		LineDrawDDA(lines[i][0], lines[i][1], lines[i][2], lines[i][3], color[i]);
	}
	
}

void CSoftwareEngine::LineDrawDDA(const int& _fx, const int& _fy, const int& _tx, const int& _ty, const DWORD &color)
{
	static int dx = 0;
	static int dy = 0;
	dx = _tx - _fx;
	dy = _ty - _fy;
		
	static int step = 0;
	if (abs(dx) > abs(dy))
		step = dx;
	else
		step = dy;

	static float Xincrement = 0.f;
	static float Yincrement = 0.f;
	Xincrement = dx / static_cast<float>(step);
	Yincrement = dy / static_cast<float>(step);

	static float x = 0;
	static float y = 0;
	x = _fx;
	y = _fy;
	static int i = 0;
	for (i = 0; i < step; ++i)
	{
		x += Xincrement;
		y += Yincrement;

		if (x >= static_cast<int>(mWindowWidth) || y >= static_cast<int>(mWindowHeight)
			|| x < 0 || y < 0)
			continue;

		// # 주의 #
		// CreateDIBSection을 통해 얻어온 비트맵은 아래쪽부터 시작한다
		// ex mDrawBuffer[100(x값) + 0(y값)]이면 화면 맨아래의 왼쪽부터 100칸째 픽셀의 색이 바뀐다.
											//따라서 화면높이에서 빼주어 위에서부터 시작하도록 한다.
		mDrawBuffer[static_cast<int>(x) + (mWindowHeight - static_cast<int>(y)) * mWindowWidth] = color;
	}
}

void CSoftwareEngine::LineDrawBLG(const int& _fx, const int& _fy, const int& _tx, const int& _ty, const COLORREF& color)
{

}
