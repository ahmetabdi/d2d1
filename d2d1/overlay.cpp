#include "stdafx.h"
#include "overlay.h"
#include "d2d1.h"
#include "Process.h"
#include "Math.h"
#include "Player.h"


Graphics::Graphics() {
	factory = NULL;
	renderTarget = NULL;
	brush = NULL;
	m_pTextFormat = NULL;
	m_pDWriteFactory = NULL;
}

Graphics::~Graphics() {
	if (factory) factory->Release();
	if (renderTarget) renderTarget->Release();
	if (brush) brush->Release();
	if (m_pTextFormat) m_pTextFormat->Release();
	if (m_pDWriteFactory) m_pDWriteFactory->Release();
}

bool Graphics::Init(HWND windowHandle) {
	HRESULT hr;

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	if (hr != S_OK) return false;

	RECT rect;
	GetClientRect(windowHandle, &rect);

	factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)),
		D2D1::HwndRenderTargetProperties(windowHandle, D2D1::SizeU(rect.right, rect.bottom)),
		&renderTarget);

	if (hr != S_OK) return false;

	hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0, 0), &brush);
	if (hr != S_OK) return false;

	static const WCHAR msc_fontName[] = L"Consolas";
	static const FLOAT msc_fontSize = 14;


	if (SUCCEEDED(hr))
	{
		// Create a DirectWrite factory.
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(m_pDWriteFactory),
			reinterpret_cast<IUnknown **>(&m_pDWriteFactory)
			);
	}
	if (SUCCEEDED(hr))
	{
		// Create a DirectWrite text format object.
		hr = m_pDWriteFactory->CreateTextFormat(
			msc_fontName,
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			msc_fontSize,
			L"", //locale
			&m_pTextFormat
			);
	}
	if (SUCCEEDED(hr))
	{
		// Center the text horizontally and vertically.
		m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}

	return true;
}

void Graphics::ClearScreen(float r, float g, float b, float a) {
	renderTarget->Clear(D2D1::ColorF(r, g, b, a));
}

void Graphics::DrawCircle(float x, float y, float radius, float r, float g, float b, float a) {

	brush->SetColor(D2D1::ColorF(r, g, b, a));

	renderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), brush, 3.0f);
}

void Graphics::DrawText(wchar_t* text, int x, int y, int width, int height) {
	renderTarget->DrawText(text, wcslen(text), m_pTextFormat, D2D1::RectF(x, y, width, height), brush);
}

void Graphics::DrawLine(float StartX, float StartY, float EndX, float EndY, float size)
{
	renderTarget->DrawLine(
		D2D1::Point2F(StartX, StartY),
		D2D1::Point2F(EndX, EndY),
		brush,
		size
		);
}

void Graphics::DrawRect(float x, float y, float width, float height, float r, float g, float b, float a) {
	brush->SetColor(D2D1::ColorF(r, g, b, a));
	D2D1_RECT_F rectangle = D2D1::RectF(x, y, x + width, y + height);
	renderTarget->FillRectangle(&rectangle, brush);
}

void Graphics::DrawRadarCenter(float width, float height)
{
	DWORD ScreenCenterX = width / 2;
	DWORD ScreenCenterY = height / 2;
	float size = 1.0f;
	DrawLine(ScreenCenterX - 100, ScreenCenterY - 90, ScreenCenterX, ScreenCenterY + 10, size);
	DrawLine(ScreenCenterX + 100, ScreenCenterY - 90, ScreenCenterX, ScreenCenterY + 10, size);
	DrawLine(ScreenCenterX, ScreenCenterY + 10, ScreenCenterX, ScreenCenterY + 107, size);
	DrawLine(ScreenCenterX - 100, ScreenCenterY + 10, ScreenCenterX + 100, ScreenCenterY + 10, size);
	DrawLine(ScreenCenterX - 100, ScreenCenterY - 90, ScreenCenterX - 100, ScreenCenterY - 75, size);
	DrawLine(ScreenCenterX - 100, ScreenCenterY - 90, ScreenCenterX - 85, ScreenCenterY - 90, size);
	DrawLine(ScreenCenterX + 100, ScreenCenterY - 90, ScreenCenterX + 100, ScreenCenterY - 75, size);
	DrawLine(ScreenCenterX + 100, ScreenCenterY - 90, ScreenCenterX + 85, ScreenCenterY - 90, size);
	DrawLine(ScreenCenterX - 100, ScreenCenterY + 107, ScreenCenterX - 100, ScreenCenterY + 92, size);
	DrawLine(ScreenCenterX - 100, ScreenCenterY + 107, ScreenCenterX - 85, ScreenCenterY + 107, size);
	DrawLine(ScreenCenterX + 100, ScreenCenterY + 108, ScreenCenterX + 100, ScreenCenterY + 92, size);
	DrawLine(ScreenCenterX + 100, ScreenCenterY + 107, ScreenCenterX + 85, ScreenCenterY + 107, size);
}

void Graphics::DrawPlayerCenter(float width, float height) {

	INT w_back = FALSE;

	DWORD ScreenCenterX = width / 2;
	DWORD ScreenCenterY = height / 2;

	int radar_range = 55;
	int max_x = ScreenCenterX + 100;
	int max_y = ScreenCenterX + 100;
	int max_yn = ScreenCenterX - 100;
	int max_xn = ScreenCenterX - 100;
	int radar_xn = ScreenCenterX - radar_range;
	int radar_x = ScreenCenterX + radar_range;

	for (int x = 0; x < 10; x++) {
		PlayerList[x].ReadInformation(x);

		//Vector2D something = WorldToScreen(Vector3D(PlayerList[x].Position.x, PlayerList[x].Position.y, PlayerList[x].Position.z));
		//DrawCircle(something.X, something.Y, 5, 1.0f, 0.0f, 0.0f, 1.0f);

//		wchar_t buffer3[256];
	//	wsprintfW(buffer3, L"%i %i\n", int(something.X), int(something.Y));
		//OutputDebugStringW(buffer3);
		/*

		float angle1 = PlayerList[x].ViewAngle;//MyPlayer.ViewAngle;
		float angle = (angle1 * 3) + 90;// * (3.1415926 / 180);

		Vector3 LocalPlayerPos = MyPlayer.Position;

		Vector3 pvPos = PlayerList[x].Position;
		Vector3 pvOut;
		float dx = LocalPlayerPos.x - pvPos.x;
		float dy = LocalPlayerPos.y - pvPos.y;
		float dz = LocalPlayerPos.z - pvPos.z;
		float DistancePlayer = (float)sqrt((dx * dx) + (dy * dy) + (dz * dz));

		float fNewYaw = angle1 + 90.0;
		//Converts degrees into radians.
		//#define D3DXToRadian(degree) ((degree) * (D3DX_PI / 180.0f))
		float fy = angle;//D3DXToRadian(fNewYaw);
		float fc = cos(fy);
		float fs = sin(fy);
		float dx1 = pvPos.x - LocalPlayerPos.x;
		float dy1 = pvPos.z - LocalPlayerPos.z;

		float px = dy1 * (-fc) + (dx1 * fs);
		float py = dx1 * (-fc) - (dy1 * fs);

		px *= 10;
		px += ScreenCenterX - 2.5;

		py *= 10;
		py += ScreenCenterY - 2.5;// + 11;

		
		// 0x57 W
		// 0x41 A
		// 0x53 S
		// 0x44 D
		Vector3 Movement; 
		Movement.z = -(fs * 0.01f);
		Movement.y = 0;
		Movement.x = -(fc * 0.01f);
		Vector3 FinalPos = pvPos + Movement;
		*/

		if (GetAsyncKeyState(VK_SHIFT)) {
			//process->Write<Vector3>(PlayerList[x].EntityLoop + 0x3C, FinalPos);
			
			if (GetAsyncKeyState(0x57)) { // W
				process->Write<float>(PlayerList[x].EntityLoop + 0x3C, PlayerList[x].Position.x + 0.02);
			}
			if (GetAsyncKeyState(0x41)) { // A
				process->Write<float>(PlayerList[x].EntityLoop + 0x3C + 0x08, PlayerList[x].Position.z + 0.02);
			}
			if (GetAsyncKeyState(0x53)) { // S
				process->Write<float>(PlayerList[x].EntityLoop + 0x3C, PlayerList[x].Position.x - 0.02);
			}
			if (GetAsyncKeyState(0x44)) { // D
				process->Write<float>(PlayerList[x].EntityLoop + 0x3C + 0x08, PlayerList[x].Position.z - 0.02);
			} 
			if (GetAsyncKeyState(VK_SPACE)) {
				process->Write<float>(PlayerList[x].EntityLoop + 0x3C + 0x04, PlayerList[x].Position.y + 0.02);
			}
			if (GetAsyncKeyState(VK_MENU)) {
				process->Write<float>(PlayerList[x].EntityLoop + 0x3C + 0x04, PlayerList[x].Position.y - 0.02);
			}
			
		}
	
		//DrawRect(px, py, 6, 6, 0, 0, 0, 255);
		//DrawRect(px, py, 5, 5, 255, 150, 0, 255);
		
		//DrawRect(dx, dy, 3, 3, 0, 1, 0, 1);
	}
}

float dotproduct(Vector3D dot, Vector3D other) { return ( dot.X * other.X + dot.Y * other.Y + dot.Z * other.Z); }

Vector2D Graphics::WorldToScreen(Vector3D World)
{
	//Get the local player position
	Vector3D PlayerOrigin = Vector3D(MyPlayer.Position.x, MyPlayer.Position.y, MyPlayer.Position.z);//RefDef->Origin;
	//Get the enemy position in relation to ours
	Vector3D Position = World - PlayerOrigin;//VectorSubtract(World, PlayerOrigin);
	Vector3D Transform;


	wchar_t buffer3[256];
	wsprintfW(buffer3, L"%i %i %i\n", int(Position.X), int(Position.Y), int(Position.Z));
	OutputDebugStringW(buffer3);

	//Get the Dot Products from the View Angles of the player
	Transform.X = dotproduct(Position, Vector3D(MyPlayer.Position.x, MyPlayer.Position.y, MyPlayer.Position.z));//RefDef->viewAxis[1]); //YAW (?)
	Transform.Y = dotproduct(Position, Vector3D(MyPlayer.Position.x, MyPlayer.Position.y, MyPlayer.Position.z));//RefDef->viewAxis[2]); //PITCH (?)
	Transform.Z = dotproduct(Position, Vector3D(MyPlayer.Position.x, MyPlayer.Position.y, MyPlayer.Position.z));//RefDef->viewAxis[0]); //ROLL

															 //Make sure the enemy is in front of the player. If not, return.
	//if (Transform.Z < 0.1f)
	//	return Vector2D(0,0);

	//Calculate the center of the screen
	Vector2D Center = Vector2D((float)screenCenterX / 2, (float)screenCenterY / 2);

	//Calculates the screen coordinates
	//*ScreenX = Center.X * (1 - (Transform.Y / RefDef->fov.x / Transform.Z));
	//*ScreenY = Center.Y * (1 - (Transform.Y / RefDef->fov.y / Transform.Z));

	float ScreenX = Center.X * (1 - (Transform.Y / MyPlayer.Position.x / Transform.Z));
	float ScreenY = Center.Y * (1 - (Transform.Y / MyPlayer.Position.y / Transform.Z));

	return Vector2D(ScreenX, ScreenY);
}

void Graphics::Render() {
	BeginDraw();

	ClearScreen(0.0f, 0.0f, 0.0f, 0.0f);
	MyPlayer.ReadInformation();

	//DrawRadarCenter(renderTargetSize.width, renderTargetSize.height);
	DrawPlayerCenter(renderTargetSize.width, renderTargetSize.height);

	//DrawText(buffer, 20, 75, 150, 50);

	DrawLine(screenCenterX - 5, screenCenterY, screenCenterX + 5, screenCenterY, 1.0f);
	DrawLine(screenCenterX, screenCenterY - 5, screenCenterX, screenCenterY + 5, 1.0f);

	//wchar_t buffer[256];
	//wsprintfW(buffer, L"health: %d", MyPlayer.Health);
	//DrawText(buffer, 20, 75, 150, 50);

	wchar_t buffer[256];
	wsprintfW(buffer, L"x: %d y: %d z: %d", int(MyPlayer.Position.x), int(MyPlayer.Position.y), int(MyPlayer.Position.z));
	DrawText(buffer, 20, 75, 250, 50);

	wchar_t buffer2[256];
	wsprintfW(buffer2, L"ViewAngle: %d", int(MyPlayer.ViewAngle));
	DrawText(buffer2, 20, 100, 250, 50);

	//for (int i = 0; i < 2; i++)
	//{
	//	PlayerList[i].ReadInformation(i);
	//	DrawRect(int(PlayerList[i].Position.x) + 150, int(PlayerList[i].Position.y) + (1 * i), 5, 5, 1, 0, 0, 1);
	//}

	DrawCircle(25, 100, 1, 1.0f, 0.0f, 0.0f, 1.0f);

	EndDraw();
}
