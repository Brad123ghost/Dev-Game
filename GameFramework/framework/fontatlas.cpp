#include "fontatlas.h"
#include <SDL_ttf.h>
#include <SDL.h>
#include <glew.h>
#include <vector>
#include <cassert>
#include "texturemanager.h"
#include "texture.h"

FontAtlas::FontAtlas(const char* fontPath, int pointSize, const char* _glyphs, TextureManager* pTextureManager)
    : textureId(0), atlasWidth(0), atlasHeight(0)
{
    TTF_Font* font = TTF_OpenFont(fontPath, pointSize);
    assert(font);

    // 1. Render each glyph to a surface and measure size
    std::vector<SDL_Surface*> glyphSurfaces;
    int maxGlyphHeight = 0;
    int totalWidth = 0;

    for (const char* c = _glyphs; *c; ++c) {
        SDL_Color white = { 255, 255, 255, 255 };
        char str[2] = { *c, 0 };
        SDL_Surface* surf = TTF_RenderText_Blended(font, str, white);
        glyphSurfaces.push_back(surf);
        if (surf) {
            totalWidth += surf->w;
            if (surf->h > maxGlyphHeight) maxGlyphHeight = surf->h;
        }
    }

    atlasWidth = totalWidth;
    atlasHeight = maxGlyphHeight;

    // 2. Create the atlas surface
    SDL_Surface* atlasSurface = SDL_CreateRGBSurface(0, atlasWidth, atlasHeight, 32,
        0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

    // 3. Blit each glyph into the atlas and record metrics
    int x = 0;
    int i = 0;
    for (const char* c = _glyphs; *c; ++c, ++i) {
        SDL_Surface* surf = glyphSurfaces[i];
        if (!surf) continue;
        SDL_Rect dest = { x, 0, surf->w, surf->h };
        SDL_BlitSurface(surf, nullptr, atlasSurface, &dest);

        Glyph g;
        g.u0 = (float)x / atlasWidth;
        g.v0 = 0.0f;
        g.u1 = (float)(x + surf->w) / atlasWidth;
        g.v1 = (float)surf->h / atlasHeight;
        g.width = surf->w;
        g.height = surf->h;
        g.xOffset = 0;
        g.yOffset = 0;
        g.xAdvance = surf->w;

        glyphs[*c] = g;
        x += surf->w;

        SDL_FreeSurface(surf);
    }

    // 4. Upload atlasSurface to OpenGL
    // Refactor to add to texture manager
    if (pTextureManager)
    {
		Texture* atlasTexture = new Texture();
        atlasTexture->LoadFontAtlas("FontAtlas", atlasSurface);
		pTextureManager->AddTexture("FontAtlas", atlasTexture);
		textureId = atlasTexture->GetTextureId();
	}
	else
	{
		// Fall back to direct OpenGL upload if TextureManager is not available
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlasWidth, atlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, atlasSurface->pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
    }

    SDL_FreeSurface(atlasSurface);
    TTF_CloseFont(font);
}