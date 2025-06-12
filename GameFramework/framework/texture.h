// COMP710 GP Framework 2024
#ifndef __TEXTURE_H_
#define __TEXTURE_H_

struct SDL_Surface;

struct AnimateInfo
{
	int frameWidth;
	int frameHeight;
};

class Texture
{
	// Member methods:
public:
	Texture();
	~Texture();

	bool Initialize(const char* pcFilename);

	void SetActive();
	void SetAnimated() { m_bAnimated = true; }
	bool IsAnimated() const { return m_bAnimated; }
	void SetAnimateInfo(const AnimateInfo& animateInfo) { m_animateInfo = animateInfo; }
	int GetWidth() const;
	int GetHeight() const;
	unsigned int GetTextureId() const { return m_uiTextureId; }
	const char* GetPath() const { return m_pcName; }
	void LoadTextTexture(const char* text, const char* fontname, int pointsize);
	void LoadFontAtlas(const char* text, SDL_Surface* pSurface);
	void LoadSurfaceIntoTexture(SDL_Surface* pSurface);

	void DebugDraw(float sizing);
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
	bool m_bAnimated;
	AnimateInfo m_animateInfo;
private:

};

#endif // __TEXTURE_H_