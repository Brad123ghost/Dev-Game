#pragma once
#ifndef __FONTATLAS_H_
#define __FONTATLAS_H_

#include <map>

class TextureManager;

struct Glyph
{
	float u0, v0, u1, v1;
	int width, height;
	int xOffset, yOffset;
	int xAdvance;
};

class FontAtlas
{
public:
	FontAtlas(const char* fontPath, int pointSize, const char* glyphs, TextureManager* pTextureManager);

public:
	std::map<char, Glyph> glyphs;
	unsigned int textureId;
	int atlasWidth, atlasHeight;
};

#endif // __FONTATLAS_H_