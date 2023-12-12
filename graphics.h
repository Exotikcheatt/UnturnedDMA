#pragma once
#include <d2d1.h>
#pragma comment(lib, "d2d1")
#include <dwmapi.h> 
#pragma comment(lib, "dwmapi.lib")
#include <dwrite.h>
#pragma comment(lib, "Dwrite")

#include <windows.h>
#include <thread>
#include <string>

#include "vector.h"

namespace __GRAPHICS
{
	std::thread* RenderThread = NULL;
	HWND hwnd = NULL;

	ID2D1HwndRenderTarget* target = NULL;
	IDWriteFactory* w_factory = NULL;
	IDWriteTextFormat* w_format = NULL;
	void RenderLoop()
	{
		WNDCLASSEXW wc = { 0 };
		MSG msg = { };


		wchar_t* ranName = (wchar_t*)rand();
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.lpfnWndProc = DefWindowProc;
		wc.lpszClassName = ranName;
		RegisterClassExW(&wc);

		if (hwnd = CreateWindowEx(0, ranName, 0, WS_POPUP | WS_VISIBLE, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0, 0, 0, 0))
		{
			const MARGINS margins = { -1 ,-1, -1, -1 };
			SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), -1, LWA_ALPHA);
			DwmExtendFrameIntoClientArea(hwnd, &margins);
			DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&w_factory));
			ID2D1Factory* factory = NULL;
			D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &factory);
			factory->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(
					D2D1_RENDER_TARGET_TYPE_HARDWARE,
					D2D1::PixelFormat(
						DXGI_FORMAT_UNKNOWN,
						D2D1_ALPHA_MODE_PREMULTIPLIED
					)
				),
				D2D1::HwndRenderTargetProperties(
					hwnd,
					D2D1::SizeU(
						GetSystemMetrics(SM_CXSCREEN),
						GetSystemMetrics(SM_CYSCREEN)
					),
					D2D1_PRESENT_OPTIONS_IMMEDIATELY
				),
				&target
			);
			w_factory->CreateTextFormat(L"Consolas", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 10.0f, L"en-us", &w_format);
			factory->Release();
			factory = nullptr;
			SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_PALETTEWINDOW);
			ShowWindow(hwnd, SW_SHOW);
			do
			{
				SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
				SetWindowPos(hwnd, nullptr, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_NOMOVE | SWP_NOZORDER);
				ShowWindow(hwnd, SW_RESTORE);
				SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
				SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
				Sleep(10);
			} while (GetMessageW(&msg, 0, 0, 0) > 0);
			w_factory->Release();
			w_format->Release();
			target->Release();
			w_factory = nullptr;
			w_format = nullptr;
			target = nullptr;
			DestroyWindow(hwnd);
		}

	}
};

D2D1_COLOR_F color(float r, float g, float b, float a = 255.f)
{
	return D2D1::ColorF(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

#define CLEAR D2D1::ColorF(0,0,0,0)
#define WHITE D2D1::ColorF(1,1,1)
#define GRAY D2D1::ColorF(.5f,.5f,.5f)
#define BLACK D2D1::ColorF(0,0,0)
#define RED D2D1::ColorF(1,0,0)
#define YELLOW D2D1::ColorF(1,1,0)
#define GREEN D2D1::ColorF(0,1,0)
#define BLUE D2D1::ColorF(0,0,1)
#define PURPLE D2D1::ColorF(.5f,0,.5f)
#define PINK D2D1::ColorF(1,0,1)
#define ORANGE D2D1::ColorF(1,.5f,0)
#define BROWN D2D1::ColorF(.5f,.25f,.25f)

class Overlay
{
public:
	Overlay()
	{
		__GRAPHICS::RenderThread = new std::thread(__GRAPHICS::RenderLoop);
		__GRAPHICS::RenderThread->detach();
		do { Sleep(1); } while (!__GRAPHICS::target);
	}
	void CleanUp()
	{
		SendMessageW(__GRAPHICS::hwnd, WM_CLOSE, 0, 0);
		while (__GRAPHICS::RenderThread->joinable())
			__GRAPHICS::RenderThread->join();
	}
	void Begin();
	void End();
	void Line(Vector2 p0, Vector2 p1, D2D1_COLOR_F color, float thickness = 1.f);
	void RoundRect(Vector2 point, Vector2 size, Vector2 roundness, D2D1_COLOR_F fill_color, float rotation = 0.f, bool filled = true, D2D1_COLOR_F outline_color = CLEAR, float outlineThickness = 0.f);
	void TextW(Vector2 point, std::wstring txt, float font_size, D2D1_COLOR_F txt_color, float rotation = 0.f);
	void TextA(Vector2 point, std::string txt, float font_size, D2D1_COLOR_F txt_color, float rotation = 0.f);
}*draw;

void Overlay::Begin()
{
	__GRAPHICS::target->BeginDraw();
	__GRAPHICS::target->Clear(D2D1::ColorF(0, 0, 0, 0));
}

void Overlay::End()
{
	__GRAPHICS::target->EndDraw();
}

inline void Overlay::Line(Vector2 p0, Vector2 p1, D2D1_COLOR_F color, float thickness)
{
	ID2D1SolidColorBrush* solid_brush = NULL;
	__GRAPHICS::target->CreateSolidColorBrush(color, &solid_brush);
	if (solid_brush)
		__GRAPHICS::target->DrawLine(D2D1::Point2F(p0.x, p0.y), D2D1::Point2F(p1.x, p1.y), solid_brush, thickness);
	solid_brush->Release();
}

inline void Overlay::RoundRect(Vector2 point, Vector2 size, Vector2 roundness, D2D1_COLOR_F fill_color, float rotation, bool filled, D2D1_COLOR_F outline_color, float outlineThickness)
{
	ID2D1SolidColorBrush* solid_brush = NULL;
	D2D1_RECT_F r = D2D1::RectF(point.x, point.y, point.x + size.x, point.y + size.y);
	if (rotation)
		__GRAPHICS::target->SetTransform(D2D1::Matrix3x2F::Rotation(rotation, D2D1::Point2F(point.x + size.x / 2, point.y + size.y / 2)));
	if (filled)
	{
		__GRAPHICS::target->CreateSolidColorBrush(fill_color, &solid_brush);
		if (solid_brush)
			__GRAPHICS::target->FillRoundedRectangle(D2D1::RoundedRect(r, roundness.x, roundness.y), solid_brush);
		solid_brush->Release();
	}
	__GRAPHICS::target->CreateSolidColorBrush(outline_color, &solid_brush);
	if (solid_brush)
		__GRAPHICS::target->DrawRoundedRectangle(D2D1::RoundedRect(r, roundness.x, roundness.y), solid_brush, outlineThickness);
	solid_brush->Release();
	if (rotation)
		__GRAPHICS::target->SetTransform(D2D1::Matrix3x2F::Rotation(0.f, D2D1::Point2F(point.x + size.x / 2, point.y + size.y / 2)));
}

inline void Overlay::TextW(Vector2 point, std::wstring txt, float font_size, D2D1_COLOR_F txt_color, float rotation)
{
	if (rotation)
		__GRAPHICS::target->SetTransform(D2D1::Matrix3x2F::Rotation(rotation, D2D1::Point2F(point.x, point.y)));
	point -= Vector2(0, 1.0f);
	ID2D1SolidColorBrush* solid_brush = NULL;
	IDWriteTextLayout* w_layout = NULL;
	if (SUCCEEDED(__GRAPHICS::w_factory->CreateTextLayout(txt.c_str(), (uint32_t)txt.size(), __GRAPHICS::w_format, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), &w_layout)))
	{
		__GRAPHICS::target->CreateSolidColorBrush(txt_color, &solid_brush);
		DWRITE_TEXT_RANGE range = { 0, (UINT32)txt.size() };
		w_layout->SetFontSize(font_size, range);
		if (solid_brush)
			__GRAPHICS::target->DrawTextLayout(D2D1::Point2F(point.x, point.y), w_layout, solid_brush);
		w_layout->Release();
		solid_brush->Release();
	}
	if (rotation)
		__GRAPHICS::target->SetTransform(D2D1::Matrix3x2F::Rotation(0.f, D2D1::Point2F(point.x, point.y)));
}

inline void Overlay::TextA(Vector2 point, std::string txt, float font_size, D2D1_COLOR_F txt_color, float rotation)
{
	std::wstring txt_fix;
	txt_fix.resize(txt.size());
	for (int i = 0; i < txt.size(); i++)
		txt_fix.at(i) = (wchar_t)txt.at(i);

	if (rotation)
		__GRAPHICS::target->SetTransform(D2D1::Matrix3x2F::Rotation(rotation, D2D1::Point2F(point.x, point.y)));
	point -= Vector2(0, 1.0f);
	ID2D1SolidColorBrush* solid_brush = NULL;
	IDWriteTextLayout* w_layout = NULL;
	if (SUCCEEDED(__GRAPHICS::w_factory->CreateTextLayout(txt_fix.c_str(), (uint32_t)txt_fix.size(), __GRAPHICS::w_format, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), &w_layout)))
	{
		__GRAPHICS::target->CreateSolidColorBrush(txt_color, &solid_brush);
		DWRITE_TEXT_RANGE range = { 0, (UINT32)txt_fix.size() };
		w_layout->SetFontSize(font_size, range);
		if (solid_brush)
			__GRAPHICS::target->DrawTextLayout(D2D1::Point2F(point.x, point.y), w_layout, solid_brush);
		w_layout->Release();
		solid_brush->Release();
	}
	if (rotation)
		__GRAPHICS::target->SetTransform(D2D1::Matrix3x2F::Rotation(0.f, D2D1::Point2F(point.x, point.y)));
}
