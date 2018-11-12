#include "stdafx.h"
#include "SoftwareEngine.h"



CSoftwareEngine::CSoftwareEngine(HWND _hWnd, UINT _windowWidth, UINT _windowHeight)
	: mHwnd(_hWnd), mWindowWidth(_windowWidth), mWindowHeight(_windowHeight)
{
	mDrawBufferSize = mWindowWidth * mWindowHeight;
	mAspectRatio = mWindowHeight / static_cast<float>(mWindowWidth);
	mWidthRatio = static_cast<float>(1) / mWindowWidth;

	//���װ� �־��µ� Rendering���� ���� ��Ʈ�� ���ۿ� �����Ͽ� ���� �����ϰ�
	// Update���� InvalidRgn�� ȣ���Ͽ� 
	//WM_PAINT�� �߻� �ɶ� ��� �Լ��� ���� BeginPaint,Bitblt,EndPaint ���־����� �������� �ʾҴ�.

	// # ���� #
	// CreateDIBSection�� ���� ���� ��Ʈ���� �Ʒ��ʺ��� �����Ѵ�
	// ex mDrawBuffer[100(x��) + 0(y��)]�̸� ȭ�� �ǾƷ��� ���ʺ��� 100ĭ° �ȼ��� ���� �ٲ��.

	// SetBitmap���̿��� �ȼ� �ϳ��ϳ� ���� �������ֱ⺸��
	// ���������� ��Ʈ�� �ּҿ� ���� �����Ͽ� ���� �Ҵ����ִ� ���� �ӵ��� �ξ� ������
	// ���� ���α׷��� ���� �� ���ִ� ��ġ ������ ��Ʈ�� (DIB)�� ����ϴ�.
	// (CreateDIBSection)

	// ��ο� ����(DX�� ����Ÿ�� ����)
	// ����ü�� (hdc(Ÿ�� ��), defaultBitmap(���ҽ�)), ((BackBufferDC(Ÿ�� ��), BackBufferBitmap(���ҽ�))
	{
		BITMAPINFO bmi;
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = mWindowWidth;
		bmi.bmiHeader.biHeight = mWindowHeight;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;

		HDC hDesktopDC = GetDC(mHwnd);
		mBackBuffer = CreateDIBSection(hDesktopDC, &bmi, DIB_RGB_COLORS, (void **)&mBackBufferBytes, 0, 0);
		if (mBackBufferBytes == NULL)
		{
			MessageBox(NULL, TEXT("backbuffer create failed!"), TEXT("Software Engine"), MB_OK);
		}
		mBackBufferDC = CreateCompatibleDC(hDesktopDC);
		SelectObject(mBackBufferDC, mBackBuffer);

		ReleaseDC(mHwnd, hDesktopDC);
	}
}


CSoftwareEngine::~CSoftwareEngine()
{
	DeleteDC(mBackBufferDC);
	DeleteObject(mBackBuffer);
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
	BitBlt(hdc, 0, 0, mWindowWidth, mWindowHeight, mBackBufferDC, NULL, NULL, SRCCOPY);
	ReleaseDC(mHwnd, hdc);
}

void CSoftwareEngine::Rendering()
{
	static int Points[][2]=
	{
		//from
		{ mWindowWidth * 0.5f,25},
		{ mWindowWidth - mWindowWidth * 0.25f,mWindowHeight * 0.75f},
		{ mWindowWidth * 0.25f,mWindowHeight * 0.75f},
		{ mWindowWidth * 0.5f,25}
	};
	static COLORREF color[3] =
	{
		RGB(255,0,0),
		RGB(255,0,0),
		RGB(255,0,0),
	};

	for (int i = 0; (i + 1)< 4; ++i)
	{
		LineDrawDDA(Points[i][0], Points[i][1], Points[i + 1][0], Points[i + 1][1], color[i]);
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
		step = abs(dx);
	else
		step = abs(dy);

	static float Xincrement = 0.f;
	static float Yincrement = 0.f;
	Xincrement = dx / static_cast<float>(step);
	Yincrement = dy / static_cast<float>(step);

	static float x = 0;
	static float y = 0;
	x = static_cast<float>(_fx);
	y = static_cast<float>(_fy);
	static int i = 0;
	for (i = 0; i < step; ++i)
	{
		x += Xincrement;
		y += Yincrement;

		if (x >= static_cast<int>(mWindowWidth) || y >= static_cast<int>(mWindowHeight)
			|| x < 0 || y < 0)
			continue;

		// # ���� #
		// CreateDIBSection�� ���� ���� ��Ʈ���� �Ʒ��ʺ��� �����Ѵ�
		// ex mDrawBuffer[100(x��) + 0(y��)]�̸� ȭ�� �ǾƷ��� ���ʺ��� 100ĭ° �ȼ��� ���� �ٲ��.
											//���� ȭ����̿��� ���־� ���������� �����ϵ��� �Ѵ�.
		mBackBufferBytes[static_cast<int>(x) + (mWindowHeight - static_cast<int>(y)) * mWindowWidth] = color;
	}
}

void CSoftwareEngine::LineDrawBLG(const int& _fx, const int& _fy, const int& _tx, const int& _ty, const COLORREF& color)
{

}

void CSoftwareEngine::PolygonFillSL(const int _points[][2], const COLORREF color[], int _count)
{
	static int Ymin = 0;
	static int Ymax = 0;

	Ymin = _points[0][1];
	Ymax = _points[0][1];
	for (int i = 1; i < _count; ++i)
	{
		if (Ymax < _points[i][1])
			Ymax = _points[i][1];
		else if (Ymin > _points[i][1])
			Ymin = _points[i][1];
	}


}
