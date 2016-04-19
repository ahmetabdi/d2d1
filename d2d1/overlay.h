#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <dwmapi.h>
#include <dwrite.h>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <tlhelp32.h>
#include "vector_helper.h"
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma comment(lib, "dwmapi")
#pragma comment(lib, "Kernel32")

class Graphics {

	ID2D1Factory* factory;
	ID2D1HwndRenderTarget * renderTarget;
	ID2D1SolidColorBrush* brush;

	IDWriteTextFormat* m_pTextFormat;
	IDWriteFactory* m_pDWriteFactory;

public:
	Graphics();
	~Graphics();

	bool Init(HWND windowHandle);

	D2D1_SIZE_F renderTargetSize;
	float screenCenterX;
	float screenCenterY;


	void BeginDraw() { renderTarget->BeginDraw(); }
	void EndDraw() { renderTarget->EndDraw(); }
	void Render();
	void GetInfo();

	wchar_t* NumberToString(int number) {
		wchar_t character[256];
		swprintf_s(character, L"%d", number);
		wprintf(L"%s\n", character);

		return character;
	}

	Vector2D WorldToScreen(Vector3D World);
	void ClearScreen(float r, float g, float b, float a);
	void DrawCircle(float x, float y, float radius, float r, float g, float b, float a);
	void DrawText(wchar_t* text, int x, int y, int width, int height);
	void DrawLine(float StartX, float StartY, float EndX, float EndY, float size);
	void DrawRect(float x, float y, float width, float height, float r, float g, float b, float a);
	void DrawRadarCenter(float width, float height);
	void DrawPlayerCenter(float width, float height);
};