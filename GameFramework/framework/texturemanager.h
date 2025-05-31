// COMP710 GP Framework 2024
#ifndef __TEXTUREMANAGER_H_
#define __TEXTUREMANAGER_H_

// Library includes:
#include <string>
#include <map>

// Forward Declarations:
class Texture;

class TextureManager
{
	// Member methods:
public:
	TextureManager();
	~TextureManager();

	bool Initialize();

	Texture* GetTexture(const char* pcFilename);
	void AddTexture(const char* key, Texture* pTexture);
	std::map<std::string, Texture*>& GetLoadedTextures() { return m_pLoadedTextures; }

	void DebugDraw();
	void SelectTextureDebugDraw();
	void ToggleSelectTexture();
protected:

private:
	TextureManager(const TextureManager& textureManager);
	TextureManager& operator=(const TextureManager& textureManager);

	// Member data:
public:

protected:
	std::map<std::string, Texture*> m_pLoadedTextures;

private:
	bool m_bShowSelect;
};

#endif // __TEXTUREMANAGER_H_