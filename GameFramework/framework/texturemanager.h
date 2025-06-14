// COMP710 GP Framework 2024
#ifndef __TEXTUREMANAGER_H_
#define __TEXTUREMANAGER_H_

// Library includes:
#include <string>
#include <map>
#include <vector>

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
	std::map<std::string, Texture*> GetLoadedTextures() const { return m_pLoadedTextures; }
	std::vector<std::string> GetLoadedTextureKeys() const { return m_TextureKeys; }

	void DebugDraw();
protected:

private:
	TextureManager(const TextureManager& textureManager);
	TextureManager& operator=(const TextureManager& textureManager);

	// Member data:
public:

protected:
	std::map<std::string, Texture*> m_pLoadedTextures;
	std::map<std::string, bool> m_AnitmatedTexture;
	std::vector<std::string> m_TextureKeys;
private:
};

#endif // __TEXTUREMANAGER_H_