
#pragma once

#include "RenderManager.h"

#define _CRT_SECURE_NO_WARNINGS

TextureHolder* anime;

// Font Instances
namespace Draw
{
	// Text functions
	namespace Fonts
	{
		DWORD Default;
		DWORD Menu;
		DWORD Untitled1;
		DWORD Font_Icons;
		DWORD MenuBold;
		DWORD Riffic;
		DWORD Suicide;
		DWORD badcache;
		DWORD Moare;
		DWORD ESP;
		DWORD Font_Weapons;
	};
};

// We don't use these anywhere else, no reason for them to be
// available anywhere else
enum EFontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

// Initialises the Drawing system, setting up fonts etc
void Draw::Initialise()
{
	Fonts::Default = 0x1D; // MainMenu Font from vgui_spew_fonts 
	Fonts::Menu = I::Surface->FontCreate();
	Fonts::Font_Icons = I::Surface->FontCreate();
	Fonts::MenuBold = I::Surface->FontCreate();
	Fonts::ESP = I::Surface->FontCreate();
	Fonts::Riffic = I::Surface->FontCreate();
	Fonts::Moare = I::Surface->FontCreate();
	Fonts::badcache = I::Surface->FontCreate();
	Fonts::Untitled1 = I::Surface->FontCreate();
	Fonts::Font_Weapons = I::Surface->FontCreate();

	I::Surface->SetFontGlyphSet(Fonts::Menu, "Courier New", 13, 3500, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW | FONTFLAG_OUTLINE);
	I::Surface->SetFontGlyphSet(Fonts::MenuBold, "Courier New", 12, 750, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW | FONTFLAG_OUTLINE);
	I::Surface->SetFontGlyphSet(Fonts::Font_Icons, "tabs-monarch", 55, 500, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
	I::Surface->SetFontGlyphSet(Fonts::ESP, "Courier New", 13, 300, 0, 0,  FONTFLAG_OUTLINE);
	I::Surface->SetFontGlyphSet(Fonts::Riffic, "impact", 27, 500, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS);
	I::Surface->SetFontGlyphSet(Fonts::Font_Weapons, "undefeated", 12, 500, 0, 0, FONTFLAG_ANTIALIAS);
	I::Surface->SetFontGlyphSet(Fonts::Untitled1, "Courier New", 13, 3500, 0, 0, FONTFLAG_OUTLINE);
	I::Surface->SetFontGlyphSet(Fonts::badcache, "badcache", 35, 800, 0, 0, FONTFLAG_ANTIALIAS);
	I::Surface->SetFontGlyphSet(Fonts::Moare,"Vanchrome Outline", 35, 800, 0, 0, FONTFLAG_ANTIALIAS);


}

RECT Draw::GetViewport()
{
	RECT Viewport = { 0, 0, 0, 0 };
	int w, h;
	I::Engine->GetScreenSize(w, h);
	Viewport.right = w; Viewport.bottom = h;
	return Viewport;
}

void Draw::Clear(int x, int y, int w, int h, Color color)
{
	I::Surface->DrawSetColor(color);
	I::Surface->DrawFilledRect(x, y, x + w, y + h);
}

void Draw::Outline(int x, int y, int w, int h, Color color)
{
	I::Surface->DrawSetColor(color);
	I::Surface->DrawOutlinedRect(x, y, x + w, y + h);
}

void Draw::Line(int x, int y, int x2, int y2, Color color)
{
	I::Surface->DrawSetColor(color);
	I::Surface->DrawLine(x, y, x2, y2);
}

void Draw::Line(Vector2D start_pos, Vector2D end_pos, Color col)
{
	Line(start_pos.x, start_pos.y, end_pos.x, end_pos.y, col);
}

void Draw::DrawRect(int x, int y, int w, int h, Color col)
{
	I::Surface->DrawSetColor(col);
	I::Surface->DrawFilledRect(x, y, x + w, y + h);
}

void Draw::PolyLine(int *x, int *y, int count, Color color)
{
	I::Surface->DrawSetColor(color);
	I::Surface->DrawPolyLine(x, y, count);
}

void Draw::DrawOutlinedRect(int x, int y, int w, int h, Color col)
{
	I::Surface->DrawSetColor(col);
	I::Surface->DrawOutlinedRect(x, y, x + w, y + h);
}

void Draw::DrawLine(int x0, int y0, int x1, int y1, Color col)
{
	I::Surface->DrawSetColor(col);
	I::Surface->DrawLine(x0, y0, x1, y1);
}

void Draw::OutlinedCircle(int x, int y, int r, int seg, Color col)
{
	I::Surface->DrawSetColor(col);
	I::Surface->DrawOutlinedCircle(x, y, r, seg);
}

void Draw::DrawCircle(float x, float y, float r, float s, Color color)
{
	float Step = PI * 2.0 / s;
	for (float a = 0; a < (PI*2.0); a += Step)
	{
		float x1 = r * cos(a) + x;
		float y1 = r * sin(a) + y;
		float x2 = r * cos(a + Step) + x;
		float y2 = r * sin(a + Step) + y;

		DrawLine(x1, y1, x2, y2, color);
	}
}

bool Draw::WorldToScreen(Vector &in, Vector &out)
{
	const matrix3x4& worldToScreen = I::Engine->WorldToScreenMatrix(); //Grab the world to screen matrix from CEngineClient::WorldToScreenMatrix

	float w = worldToScreen[3][0] * in[0] + worldToScreen[3][1] * in[1] + worldToScreen[3][2] * in[2] + worldToScreen[3][3]; //Calculate the angle in compSuicideon to the player's camera.
	out.z = 0; //Screen doesn't have a 3rd dimension.

	if (w > 0.001) //If the object is within view.
	{
		RECT ScreenSize = GetViewport();
		float fl1DBw = 1 / w; //Divide 1 by the angle.
		out.x = (ScreenSize.right / 2) + (0.5f * ((worldToScreen[0][0] * in[0] + worldToScreen[0][1] * in[1] + worldToScreen[0][2] * in[2] + worldToScreen[0][3]) * fl1DBw) * ScreenSize.right + 0.5f); //Get the X dimension and push it in to the Vector.
		out.y = (ScreenSize.bottom / 2) - (0.5f * ((worldToScreen[1][0] * in[0] + worldToScreen[1][1] * in[1] + worldToScreen[1][2] * in[2] + worldToScreen[1][3]) * fl1DBw) * ScreenSize.bottom + 0.5f); //Get the Y dimension and push it in to the Vector.
		return true;
	}

	return false;
}

void Draw::Text(int x, int y, Color color, DWORD font, const char* text)
{
	size_t origsize = strlen(text) + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t wcstring[newsize];
	mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

	I::Surface->DrawSetTextFont(font);

	I::Surface->DrawSetTextColor(color);
	I::Surface->DrawSetTextPos(x, y);
	I::Surface->DrawPrintText(wcstring, wcslen(wcstring));
	return;
}

void Draw::Text(int x, int y, Color color, DWORD font, const wchar_t* text)
{
	I::Surface->DrawSetTextFont(font);
	I::Surface->DrawSetTextColor(color);
	I::Surface->DrawSetTextPos(x, y);
	I::Surface->DrawPrintText(text, wcslen(text));
}

void Draw::text(int x, int y, const char* _Input, int font, Color color)
{
	int apple = 0;
	char Buffer[2048] = { '\0' };
	va_list Args;
	va_start(Args, _Input);
	vsprintf_s(Buffer, _Input, Args);
	va_end(Args);
	size_t Size = strlen(Buffer) + 1;
	wchar_t* WideBuffer = new wchar_t[Size];
	mbstowcs_s(0, WideBuffer, Size, Buffer, Size - 1);

	I::Surface->DrawSetTextColor(color);
	I::Surface->DrawSetTextFont(font);
	I::Surface->DrawSetTextPos(x, y);
	I::Surface->DrawPrintText(WideBuffer, wcslen(WideBuffer));
}

void Draw::Textf(int x, int y, Color color, DWORD font, const char* fmt, ...)
{
	if (!fmt) return; //if the passed string is null return
	if (strlen(fmt) < 2) return;

	//Set up va_list and buffer to hold the params 
	va_list va_alist;
	char logBuf[256] = { 0 };

	//Do sprintf with the parameters
	va_start(va_alist, fmt);
	_vsnprintf_s(logBuf + strlen(logBuf), 256 - strlen(logBuf), sizeof(logBuf) - strlen(logBuf), fmt, va_alist);
	va_end(va_alist);

	Text(x, y, color, font, logBuf);
}

RECT Draw::GetTextSize(DWORD font, const char* text)
{
	size_t origsize = strlen(text) + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t wcstring[newsize];
	mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

	RECT rect; int x, y;
	I::Surface->GetTextSize(font, wcstring, x, y);
	rect.left = x; rect.bottom = y;
	rect.right = x;
	return rect;
}

void Draw::GradientV(int x, int y, int w, int h, Color c1, Color c2)
{
	Clear(x, y, w, h, c1);
	BYTE first = c2.r();
	BYTE second = c2.g();
	BYTE third = c2.b();
	for (int i = 0; i < h; i++)
	{
		float fi = i, fh = h;
		float a = fi / fh;
		DWORD ia = a * 255;
		Clear(x, y + i, w, 1, Color(first, second, third, ia));
	}
}

void Draw::GradientH(int x, int y, int w, int h, Color c1, Color c2)
{
	Clear(x, y, w, h, c1);
	BYTE first = c2.r();
	BYTE second = c2.g();
	BYTE third = c2.b();
	for (int i = 0; i < w; i++)
	{
		float fi = i, fw = w;
		float a = fi / fw;
		DWORD ia = a * 255;
		Clear(x + i, y, 1, h, Color(first, second, third, ia));
	}
}

void Draw::Polygon(int count, Vertex_t* Vertexs, Color color)
{
	static int Texture = I::Surface->CreateNewTextureID(true); //need to make a texture with procedural true
	unsigned char buffer[4] = { 255, 255, 255, 255 };//{ color.r(), color.g(), color.b(), color.a() };

	I::Surface->DrawSetTextureRGBA(Texture, buffer, 1, 1); //Texture, char array of texture, width, height
	I::Surface->DrawSetColor(color); // keep this full color and opacity use the RGBA @top to set values.
	I::Surface->DrawSetTexture(Texture); // bind texture

	I::Surface->DrawTexturedPolygon(count, Vertexs);
}

void Draw::PolygonOutline(int count, Vertex_t* Vertexs, Color color, Color colorLine)
{
	static int x[128];
	static int y[128];

	Draw::Polygon(count, Vertexs, color);

	for (int i = 0; i < count; i++)
	{
		x[i] = Vertexs[i].m_Position.x;
		y[i] = Vertexs[i].m_Position.y;
	}

	Draw::PolyLine(x, y, count, colorLine);
}

void Draw::PolyLine(int count, Vertex_t* Vertexs, Color colorLine)
{
	static int x[128];
	static int y[128];

	for (int i = 0; i < count; i++)
	{
		x[i] = Vertexs[i].m_Position.x;
		y[i] = Vertexs[i].m_Position.y;
	}

	Draw::PolyLine(x, y, count, colorLine);
}

void Draw::DrawString(DWORD font, int x, int y, Color color, bool bCenter, const char* msg, ...)
{
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, msg);
	_vsnprintf(buf, sizeof(buf), msg, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];
	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	int r = 255, g = 255, b = 255, a = 255;
	color.GetColor(r, g, b, a);

	int iWidth, iHeight;

	I::Surface->GetTextSize(font, wbuf, iWidth, iHeight);
	I::Surface->DrawSetTextFont(font);
	I::Surface->DrawSetTextColor(r, g, b, a);
	I::Surface->DrawSetTextPos(!bCenter ? x : x - iWidth / 2, y - iHeight / 2);
	I::Surface->DrawPrintText(wbuf, wcslen(wbuf));
}