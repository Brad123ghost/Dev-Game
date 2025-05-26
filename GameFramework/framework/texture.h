// COMP710 GP Framework 2024
#ifndef __TEXTURE_H_
#define __TEXTURE_H_

class SDL_Surface;

class Texture
{
	// Member methods:
public:
	Texture();
	~Texture();

	bool Initialize(const char* pcFilename);

	void SetActive();

	int GetWidth() const;
	int GetHeight() const;

	void LoadTextTexture(const char* text, const char* fontname, int pointsize);
	void LoadSurfaceIntoTexture(SDL_Surface* pSurface);

	void DebugDraw();
protected:

private:
	Texture(const Texture& texture);
	Texture& operator=(const Texture& texture);

	// Member data:
public:

protected:
	unsigned int m_uiTextureId;
	int m_iWidth;
	int m_iHeight;
	const char* m_pcName;
	bool m_bIsSelected;
	bool m_bIsOpen;
private:

};

#endif // __TEXTURE_H_