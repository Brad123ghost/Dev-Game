#include "dynamictext.h"
#include "renderer.h"
#include "assert.h"

DynamicText::DynamicText(const char* fontName, int pointSize, int x, int y)
	:m_pcFontName(fontName)
	, m_iPointSize(pointSize)
{
	m_position.x = x;
	m_position.y = y;
	TTF_Init();
	m_pFont = TTF_OpenFont(m_pcFontName, m_iPointSize);
	assert(m_pFont != nullptr);
	TTF_Quit();
}

DynamicText::~DynamicText()
{
	if (m_pFont)
	{
		TTF_CloseFont(m_pFont);
		//delete m_pFont;
		m_pFont = 0;
	}

	delete m_pcFontName;
	m_pcFontName = 0;
}



void DynamicText::UpdateText(const std::string& newText)
{
	// If text same as current, do nothing
	if (m_sCurrentText == newText) return;

	m_sCurrentText = newText;
	SDL_Color color = { 255, 255, 255, 255 };
	SDL_Surface* pSurface = TTF_RenderText_Blended(m_pFont, m_sCurrentText.c_str(), color);
	LoadSurfaceIntoTexture(pSurface);
	SDL_FreeSurface(pSurface);

	m_position.w = m_iWidth;
	m_position.h = m_iHeight;
}