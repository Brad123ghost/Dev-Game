// COMP710 GP Framework 2024
#ifndef __RENDERER_H_
#define __RENDERER_H_

#include "camera.h"

// Forward Declarations:
class TextureManager;
class Texture;
class Shader;
class VertexArray;
class Sprite;
class AnimatedSprite;
struct SDL_Window;


// Library includes:
#include <SDL.h>
#include <memory>
#include <vector>
#include "glm.hpp"
#include "fontatlas.h"

extern Camera* g_pDefaultCamera;

class Renderer
{
	// Member methods:
public:
	Renderer();
	~Renderer();

	bool Initialize(bool windowed, int width = 0, int height = 0);

	void Clear();
	void Present();

	void SetClearColor(unsigned char r, unsigned char g, unsigned char b);
	void GetClearColor(unsigned char& r, unsigned char& g, unsigned char& b);

	int GetWidth() const;
	int GetHeight() const;

	Sprite* CreateSprite(const char* pcFilename);
	Texture* CreateTexture(const char* pcFilename);
	//void DrawSprite(Sprite& sprite);
	void DrawSprite(Sprite& sprite, Camera* pCamera = nullptr);
	void DrawUI(Sprite& sprite);
	void DrawOutline(Sprite& sprite, Camera* pCamera = nullptr);
	AnimatedSprite* CreateAnimatedSprite(const char* pcFilename);
	void DrawAnimatedSprite(AnimatedSprite& sprite, int frame);
	void DrawTest();
	void CreateStaticText(const char* pText, int pointsize);
	void DrawText(const char* pText, float x, float y, float scale, const glm::vec4& color = glm::vec4(1.f));
	//void CreateDynamicText(const char* pText, int pointsize, int x, int y);

	void DrawAABB(float x, float y, float width, float height);

	Camera& GetDefaultCamera() { return *g_pDefaultCamera; }

	void DrawDebug();

	void DrawLine(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &color = glm::vec3(1.f));
	void DrawLine2D(const glm::vec2 &p1, const glm::vec2 &p2, const glm::vec3 &color = glm::vec3(1.f));
	void DrawLineFlush(Camera* pCamera);
	void GenerateGrid(int gSize, int cSize);

	SDL_Window* GetWindow() const { return m_pWindow; }

	TextureManager* GetTextureManager() const { return m_pTextureManager; }
	//Camera* GetCamera() { return m_pCamera; }
	
	bool ReloadShaders();

protected:
	bool InitializeOpenGL(int screenWidth, int screenHeight);
	void SetFullscreen(bool fullscreen);

	void LogSdlError();
	void dump();
	bool SetupSpriteShader();
private:
	Renderer(const Renderer& renderer);
	Renderer& operator=(const Renderer& renderer);

	// Member data:

public:

protected:
	FontAtlas* m_pFontAtlas;
	TextureManager* m_pTextureManager;
	SDL_Window* m_pWindow;
	SDL_GLContext m_glContext;

	Shader* m_pSpriteShader;
	VertexArray* m_pSpriteVertexData;

	Shader* m_pGridShader;
	VertexArray* m_pGridVertexData;

	Shader* m_pOutlineShader;

	std::vector<float> m_fLineData;

	std::vector<float> m_fCameraOutline;

	int m_iWidth;
	int m_iHeight;

	float m_fClearRed;
	float m_fClearGreen;
	float m_fClearBlue;

private:
	float m_gridSize;
};

#endif // __RENDERER_H_