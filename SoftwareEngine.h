#pragma once
class CSoftwareEngine
{
public:
	CSoftwareEngine(HWND _hWnd, UINT _windowWidth ,UINT _windowHeight);
	virtual ~CSoftwareEngine();

	virtual void Update();
	virtual void Draw();
	virtual void Rendering();

	HWND mHwnd			= NULL;
	UINT mWindowWidth	= 0U;
	UINT mWindowHeight	= 0U;

	//For Draw
	FLOAT		mAspectRatio		= 0.f;
	FLOAT		mWidthRatio			= 0.f;
	HBITMAP		hDibDrawTexture		= NULL;
	HDC			hDibDrawTextureDC	= NULL;
	LPDWORD		mDrawBuffer			= nullptr;
	SIZE_T		mDrawBufferSize		= 0;

	COLORREF& GetDrawBufferAt(int x, int y = 0)
	{
		return mDrawBuffer[x + y * mWindowWidth];
	}

	void LineDrawDDA(const int& _fx, const int& _fy, const int& _tx, const int& _ty, const COLORREF &color);
	void LineDrawBLG(const int& _fx, const int& _fy, const int& _tx, const int& _ty, const COLORREF &color);
};

