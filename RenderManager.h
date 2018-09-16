
#pragma once

#include "Interfaces.h"

#include "Vector2D.h"

void Quad();


class TextureHolder
{
public:
	typedef std::shared_ptr<TextureHolder> Ptr;
	TextureHolder()
		: m_pSurface(I::Surface), m_iH(0), m_iW(0), m_bgColor(255, 255, 255, 255), m_bValid(false)
	{ };

	TextureHolder(const unsigned char* pRawRGBAData, unsigned int W, unsigned int H)
		: m_pSurface(I::Surface), m_iH(H), m_iW(W), m_bgColor(255, 255, 255, 255), m_bValid(false)
	{
		m_iTexture = m_pSurface->CreateNewTextureID(true);
		if (!m_iTexture)
			return;
		m_pSurface->DrawSetTextureRGBA(m_iTexture, pRawRGBAData, W, H);
		m_bValid = true;
	};

	bool IsValid() const
	{
		return m_bValid;
	};

	int GetTextureId() const
	{
		return m_iTexture;
	};

	bool Draw(int x, int y, float scale = 1.0)
	{
		//if (!m_pSurface->IsTextureIDValid(m_iTexture))
		//	return false;
		m_pSurface->DrawSetColor(m_bgColor);
		m_pSurface->DrawSetTexture(m_iTexture);
		//m_pSurface->DrawTexturedRect(x, y, x + m_iW * scale, y + m_iH * scale);
		return true;
	};

protected:
	unsigned int m_iTexture;
	unsigned int m_iW, m_iH;
	Color  m_bgColor;
	bool   m_bValid;

	ISurface* m_pSurface;
};
extern TextureHolder* anime;

namespace Draw
{
	void Initialise();

	// Normal Drawing functions
	void Clear(int x, int y, int w, int h, Color color);
	void Outline(int x, int y, int w, int h, Color color);
	void Line(int x, int y, int x2, int y2, Color color);
	void Line(Vector2D start_pos, Vector2D end_pos, Color col);
	void DrawRect(int x, int y, int w, int h, Color col);
	void PolyLine(int *x, int *y, int count, Color color);
	void Polygon(int count, Vertex_t* Vertexs, Color color);
	void PolygonOutline(int count, Vertex_t* Vertexs, Color color, Color colorLine);
	void PolyLine(int count, Vertex_t* Vertexs, Color colorLine);
	extern void DrawOutlinedRect(int x, int y, int w, int h, Color col);
	extern void DrawLine(int x0, int y0, int x1, int y1, Color col);
	void OutlinedCircle(int x, int y, int r, int seg, Color col);
	void DrawCircle(float x, float y, float r, float s, Color color);

	// Gradient Functions
	void GradientV(int x, int y, int w, int h, Color c1, Color c2);
	void GradientH(int x, int y, int w, int h, Color c1, Color c2);

	// Text functions
	namespace Fonts
	{
		extern DWORD Default;
		extern DWORD Menu;
		extern DWORD Blacksword;
		extern DWORD Font_Icons;
		extern DWORD Moare;
		extern DWORD badcache;
		extern DWORD Untitled1;
		extern DWORD Menu1;
		extern DWORD MenuBold;
		extern DWORD Suicide;
		extern DWORD Riffic;
		extern DWORD ESP;
		extern DWORD Font_Weapons;
	};

	void Text(int x, int y, Color color, DWORD font, const char* text);
	void Textf(int x, int y, Color color, DWORD font, const char* fmt, ...);
	void Text(int x, int y, Color color, DWORD font, const wchar_t* text);
	void text(int x, int y, const char* _Input, int font, Color color);
	RECT GetTextSize(DWORD font, const char* text);
	void DrawString(DWORD font, int x, int y, Color color, bool bCenter, const char* msg, ...);

	// Other rendering functions
	bool WorldToScreen(Vector &in, Vector &out);
	RECT GetViewport();
};

