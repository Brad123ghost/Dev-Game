#pragma once
#ifndef __DYNAMICTEXT_H_
#define __DYNAMICTEXT_H_

#include <SDL_ttf.h>
#include <string>
#include "texture.h"

class DynamicText : public Texture
{
    // Member methods:
public:
    DynamicText(const char* fontName, int pointSize, int x = 0, int y = 0);
    ~DynamicText();
    void LoadTextTexture(const char* fontname, int pointsize);

    void UpdateText(const std::string& newText);

public:
    SDL_Rect m_position;
    TTF_Font* m_pFont;
    const char* m_pcFontName;
    int m_iPointSize;
    std::string m_sCurrentText;
};

#endif // __DYNAMICTEXT_H_