// COMP710 GP Framework 2024

// This include:
#include "renderer.h"

// Local includes:
//#include "dynamictext.h"
#include "texture.h"
#include "texturemanager.h"
#include "logmanager.h"
#include "shader.h"
#include "vertexarray.h"
#include "sprite.h"
#include "animatedsprite.h"
#include "matrix4.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "inlinehelper.h"

// Library includes:
#include <SDL.h>
#include <SDL_image.h>
#include <glew.h>
#include <cassert>
#include <cmath>
#include <iostream>
#include <SDL_ttf.h>
//#include <vector>

Camera* g_pDefaultCamera = nullptr;

Renderer::Renderer()
	: m_pTextureManager(0)
	, m_pSpriteShader(0)
	, m_pSpriteVertexData(0)
	, m_glContext(0)
	, m_iWidth(0)
	, m_iHeight(0)
	, m_fClearRed(0)
	, m_fClearGreen(0)
	, m_fClearBlue(0)
	, m_gridSize(32.f)
{

}

Renderer::~Renderer()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	delete m_pSpriteShader;
	m_pSpriteShader = 0;

	delete m_pSpriteVertexData;
	m_pSpriteVertexData = 0;

	delete m_pGridShader;
	m_pGridShader = 0;

	delete m_pGridVertexData;
	m_pGridVertexData = 0;

	delete m_pOutlineShader;
	m_pOutlineShader = 0;

	delete m_pTextureManager;
	m_pTextureManager = 0;

	delete g_pDefaultCamera;
	g_pDefaultCamera = 0;

	delete m_pFontAtlas;
	m_pFontAtlas = 0;

	m_fLineData.clear();

	SDL_DestroyWindow(m_pWindow);
	IMG_Quit();
	SDL_Quit();
}

bool Renderer::Initialize(bool windowed, int width, int height)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LogSdlError();
		return false;
	}

	if (!windowed)
	{
		// Go fullscreen, with current resolution!
		int numDisplays = SDL_GetNumVideoDisplays();
		SDL_DisplayMode* currentDisplayMode = new SDL_DisplayMode[numDisplays];

		for (int k = 0; k < numDisplays; ++k)
		{
			int result = SDL_GetCurrentDisplayMode(k, &currentDisplayMode[k]);
		}

		// Use the widest display?
		int widest = 0;
		int andItsHeight = 0;

		for (int k = 0; k < numDisplays; ++k)
		{
			if (currentDisplayMode[k].w > widest)
			{
				widest = currentDisplayMode[k].w;
				andItsHeight = currentDisplayMode[k].h;
			}
		}

		delete[] currentDisplayMode;
		currentDisplayMode = 0;

		width = widest;
		height = andItsHeight;
	}

	bool initialized = InitializeOpenGL(width, height);

	SetFullscreen(!windowed);

	if (initialized)
	{
		m_pTextureManager = new TextureManager();
		assert(m_pTextureManager);
		initialized = m_pTextureManager->Initialize();
		TTF_Init();
		const char* glyphs = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
		m_pFontAtlas = new FontAtlas("arial.ttf", 24, glyphs, m_pTextureManager);
		TTF_Quit();
	}
	ImGuiContext* tempgui = ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(m_pWindow, m_glContext);
	ImGui_ImplOpenGL3_Init();
	ImGui::SetMouseCursor(ImGuiMouseCursor_None);

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	g_pDefaultCamera = new Camera(width, height);
	g_pDefaultCamera->SetCamSpeed(300.f);
	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
		std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
		// Handle error appropriately
	}
	
	return initialized;
}

bool Renderer::InitializeOpenGL(int screenWidth, int screenHeight)
{
	m_iWidth = screenWidth;
	m_iHeight = screenHeight;

	m_pWindow = SDL_CreateWindow("COMP710 Game Framework", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	m_glContext = SDL_GL_CreateContext(m_pWindow);

	GLenum glewResult = glewInit();

	if (glewResult != GLEW_OK)
	{
		return false;
	}

	// Disable VSYN
	SDL_GL_SetSwapInterval(0);

	bool shadersLoaded = SetupSpriteShader();
	dump();
	return shadersLoaded;
}

void Renderer::Clear()
{
	glClearColor(m_fClearRed, m_fClearGreen, m_fClearBlue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void Renderer::Present()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(m_pWindow, m_glContext);
	}

	SDL_GL_SwapWindow(m_pWindow);
}

void Renderer::SetFullscreen(bool fullscreen)
{
	if (fullscreen)
	{
		//SDL_SetWindowFullscreen(m_pWindow, SDL_WINDOW_FULLSCREEN | SDL_SetWindowAlwaysOnTop);
		SDL_SetWindowFullscreen(m_pWindow, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_ALWAYS_ON_TOP);
		SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");
		SDL_SetWindowSize(m_pWindow, m_iWidth, m_iHeight);
	}
	else
	{
		SDL_SetWindowFullscreen(m_pWindow, 0);
	}
}

void Renderer::SetClearColor(unsigned char r, unsigned char g, unsigned char b)
{
	m_fClearRed = r / 255.0f;
	m_fClearGreen = g / 255.0f;
	m_fClearBlue = b / 255.0f;
}

void Renderer::GetClearColor(unsigned char& r, unsigned char& g, unsigned char& b)
{
	r = static_cast<unsigned char>(m_fClearRed * 255.0f);
	g = static_cast<unsigned char>(m_fClearGreen * 255.0f);
	b = static_cast<unsigned char>(m_fClearBlue * 255.0f);
}

int Renderer::GetWidth() const
{
	return m_iWidth;
}

int Renderer::GetHeight() const
{
	return m_iHeight;
}

Sprite* Renderer::CreateSprite(const char* pcFilename)
{
	assert(m_pTextureManager);

	Texture* pTexture = m_pTextureManager->GetTexture(pcFilename);
	//std::cout << "[Renderer] Creating texture: " << pcFilename << std::endl;
	Sprite* pSprite = new Sprite();
	if (!pSprite->Initialize(*pTexture))
	{
		LogManager::GetInstance().Log("Sprite Failed to Create!");
	}

	return (pSprite);
}

Texture* Renderer::CreateTexture(const char* pcFilename)
{
	return m_pTextureManager->GetTexture(pcFilename);
}

void Renderer::LogSdlError()
{
	LogManager::GetInstance().Log(SDL_GetError());
}

bool Renderer::SetupSpriteShader()
{
	const int STRIDE = 5;
	m_pSpriteShader = new Shader();

	bool loaded = m_pSpriteShader->Load("shaders\\sprite.vert", "shaders\\sprite.frag");

	m_pSpriteShader->SetActive();

	float vertices[] =
	{
		-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, // Top Left
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top Right
		 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, // Bottom Right 
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f // Bottom Left
	};

	unsigned int indices[] = { 0,1,2,2,3,0 };

	m_pSpriteVertexData = new VertexArray(vertices, 4, indices, 6, STRIDE);

	return loaded;
}

void Renderer::dump()
{
	m_pGridShader = new Shader();
	bool loaded = m_pGridShader->Load("shaders\\line.vert", "shaders\\line.frag");

	m_pOutlineShader = new Shader();
	m_pOutlineShader->Load("shaders\\outline.vert", "shaders\\outline.frag", "shaders\\outline.geom");
}

// Render Sprites using world coords
void Renderer::DrawSprite(Sprite& sprite, Camera* pCamera)
{
	m_pSpriteShader->SetActive();
	m_pSpriteVertexData->SetActive();

	float angleInDegrees = sprite.GetAngle();

	float sizeX = static_cast<float>(sprite.GetWidth());
	float sizeY = static_cast<float>(sprite.GetHeight());

	const float PI = 3.14159f;
	float angleInRadians = (angleInDegrees * PI) / 180.0f;

	Matrix4 world;
	SetIdentity(world);
	world.m[0][0] = cosf(angleInRadians) * (sizeX);
	world.m[0][1] = -sinf(angleInRadians) * (sizeX) * -1.0f; // Multiply by -1.0f, fixes rotation
	world.m[1][0] = sinf(angleInRadians) * (sizeY);
	world.m[1][1] = cosf(angleInRadians) * (sizeY) * -1.0f; // Multiply by -1.0f, fixes upsidedown sprites
	Vector2 screenPos = v2WorldToScreen({ static_cast<float>(sprite.GetX()), static_cast<float>(sprite.GetY()) });
	world.m[3][0] = screenPos.x;
	world.m[3][1] = screenPos.y;

	m_pSpriteShader->SetMatrixUniform("uWorldTransform", world);

	Camera* activeCam = pCamera ? pCamera : g_pDefaultCamera;
	Matrix4 view = activeCam->GetViewMatrix();

	Matrix4 orthoViewProj;
	CreateOrthoProjection(orthoViewProj, static_cast<float>(m_iWidth), static_cast<float>(m_iHeight));

	m_pSpriteShader->SetVector4Uniform("color", sprite.GetRedTint(), sprite.GetGreenTint(), sprite.GetBlueTint(), sprite.GetAlpha());
	Matrix4 newView = Multiply(orthoViewProj, view);
	m_pSpriteShader->SetMatrixUniform("uViewProj", newView);
	

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glUseProgram(0);
}

// Used to render sprites using screen coords
void Renderer::DrawUI(Sprite& sprite)
{
	m_pSpriteShader->SetActive();
	m_pSpriteVertexData->SetActive();

	float angleInDegrees = sprite.GetAngle();

	float sizeX = static_cast<float>(sprite.GetWidth());
	float sizeY = static_cast<float>(sprite.GetHeight());

	const float PI = 3.14159f;
	float angleInRadians = (angleInDegrees * PI) / 180.0f;

	Matrix4 world;
	SetIdentity(world);
	world.m[0][0] = cosf(angleInRadians) * (sizeX);
	world.m[0][1] = -sinf(angleInRadians) * (sizeX) * -1.0f; // Multiply by -1.0f, fixes rotation
	world.m[1][0] = sinf(angleInRadians) * (sizeY);
	world.m[1][1] = cosf(angleInRadians) * (sizeY) * -1.0f; // Multiply by -1.0f, fixes upsidedown sprites
	world.m[3][0] = static_cast<float>(sprite.GetX());
	world.m[3][1] = static_cast<float>(sprite.GetY());

	m_pSpriteShader->SetMatrixUniform("uWorldTransform", world);

	Camera* activeCam = g_pDefaultCamera;
	Matrix4 view = activeCam->GetViewMatrix();

	Matrix4 orthoViewProj;
	CreateOrthoProjection(orthoViewProj, static_cast<float>(m_iWidth), static_cast<float>(m_iHeight));

	m_pSpriteShader->SetVector4Uniform("color", sprite.GetRedTint(), sprite.GetGreenTint(), sprite.GetBlueTint(), sprite.GetAlpha());
	Matrix4 newView = Multiply(orthoViewProj, view);
	m_pSpriteShader->SetMatrixUniform("uViewProj", newView);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::DrawOutline(Sprite& sprite, Camera* pCamera)
{
	m_pOutlineShader->SetActive();
	//m_pSpriteShader->SetActive();
	m_pSpriteVertexData->SetActive();
	

	float angleInDegrees = sprite.GetAngle();

	float sizeX = static_cast<float>(sprite.GetWidth());
	float sizeY = static_cast<float>(sprite.GetHeight());

	const float PI = 3.14159f;
	float angleInRadians = (angleInDegrees * PI) / 180.0f;

	Matrix4 world;
	SetIdentity(world);
	world.m[0][0] = cosf(angleInRadians) * (sizeX);
	world.m[0][1] = -sinf(angleInRadians) * (sizeX) * -1.0f; // Multiply by -1.0f, fixes rotation
	world.m[1][0] = sinf(angleInRadians) * (sizeY);
	world.m[1][1] = cosf(angleInRadians) * (sizeY) * -1.0f; // Multiply by -1.0f, fixes upsidedown sprites
	Vector2 screenPos = v2WorldToScreen({ static_cast<float>(sprite.GetX()), static_cast<float>(sprite.GetY()) });
	world.m[3][0] = screenPos.x;
	world.m[3][1] = screenPos.y;

	m_pOutlineShader->SetMatrixUniform("uWorldTransform", world);

	Camera* activeCam = pCamera ? pCamera : g_pDefaultCamera;
	Matrix4 view = activeCam->GetViewMatrix();

	Matrix4 orthoViewProj;
	CreateOrthoProjection(orthoViewProj, static_cast<float>(m_iWidth), static_cast<float>(m_iHeight));

	m_pOutlineShader->SetVector4Uniform("color", 1.0f, 1.0f, 1.0f, 1.0f);
	Matrix4 newView = Multiply(orthoViewProj, view);
	m_pOutlineShader->SetMatrixUniform("uViewProj", newView);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sprite.GetTextureId());
	m_pOutlineShader->SetIntUniform("uTexture", 0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_pOutlineShader->SetIntUniform("uDrawTexture", 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	m_pOutlineShader->SetIntUniform("uDrawTexture", 1);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

void Renderer::DrawTest()
{

}

AnimatedSprite* Renderer::CreateAnimatedSprite(const char* pcFilename)
{
	assert(m_pTextureManager);

	Texture* pTexture = m_pTextureManager->GetTexture(pcFilename);
	pTexture->SetAnimated();
	AnimatedSprite* pSprite = new AnimatedSprite();
	if (!pSprite->Initialize(*pTexture))
	{
		LogManager::GetInstance().Log("Aniamted Sprite failed to create!");
		assert(0);
	}
	return pSprite;
}

void Renderer::DrawAnimatedSprite(AnimatedSprite& sprite, int frame)
{
	m_pSpriteShader->SetActive();

	float angleInDegrees = sprite.GetAngle();
	float sizeX = static_cast<float>(sprite.GetWidth());
	float sizeY = static_cast<float>(sprite.GetHeight());

	const float PI = 3.14159f;
	float angleInRadians = (angleInDegrees * PI) / 180.0f;

	Matrix4 world;
	SetIdentity(world);
	world.m[0][0] = cosf(angleInRadians) * (sizeX);
	world.m[0][1] = -sinf(angleInRadians) * (sizeX);
	world.m[1][0] = sinf(angleInRadians) * (sizeY);
	world.m[1][1] = cosf(angleInRadians) * (sizeY);
	world.m[3][0] = static_cast<float>(sprite.GetX());
	world.m[3][1] = static_cast<float>(sprite.GetY());

	m_pSpriteShader->SetMatrixUniform("uWorldTransform", world);
	Camera* activeCam = g_pDefaultCamera;
	Matrix4 view = activeCam->GetViewMatrix();
	Matrix4 orthoViewProj;
	CreateOrthoProjection(orthoViewProj, static_cast<float>(m_iWidth), static_cast<float>(m_iHeight));
	m_pSpriteShader->SetVector4Uniform("color", sprite.GetRedTint(), sprite.GetGreenTint(), sprite.GetBlueTint(), sprite.GetAlpha());
	Matrix4 newView = Multiply(view, orthoViewProj);
	m_pSpriteShader->SetMatrixUniform("uViewProj", newView);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)((frame * 6) * sizeof(GLuint)));
	//glUseProgram(0);
}


void Renderer::CreateStaticText(const char* pText, int pointsize)
{
	Texture* pTexture = new Texture();
	pTexture->LoadTextTexture(pText, "arial.ttf", pointsize);
	m_pTextureManager->AddTexture(pText, pTexture);
}
void Renderer::DrawText(const char* pText, float x, float y, float scale, const glm::vec4& color)
{
	m_pSpriteShader->SetActive();

	Matrix4 world;
	SetIdentity(world);

	m_pSpriteShader->SetMatrixUniform("uWorldTransform", world);
	Camera* activeCam = g_pDefaultCamera;
	Matrix4 view = activeCam->GetViewMatrix();
	Matrix4 orthoViewProj;
	CreateOrthoProjection(orthoViewProj, static_cast<float>(m_iWidth), static_cast<float>(m_iHeight));
	m_pSpriteShader->SetVector4Uniform("color", color.r, color.g, color.b, color.a);
	Matrix4 newView = Multiply(view, orthoViewProj);
	m_pSpriteShader->SetMatrixUniform("uViewProj", newView);

	glBindTexture(GL_TEXTURE_2D, m_pFontAtlas->textureId);

	float penX = x;
	float penY = y;

	for (const char* c = pText; *c; ++c)
	{
		auto it = m_pFontAtlas->glyphs.find(*c);
		if (it == m_pFontAtlas->glyphs.end()) continue;
		const Glyph& g = it->second;

		float xpos = penX + g.xOffset * scale;
		float ypos = penY + g.yOffset * scale;
		float w = g.width * scale;
		float h = g.height * scale;

		float vertices[20] = {
			// pos         // tex
			xpos,     ypos,      0.0f, g.u0, g.v0,
			xpos + w,   ypos,      0.0f, g.u1, g.v0,
			xpos + w,   ypos + h,    0.0f, g.u1, g.v1,
			xpos,     ypos + h,    0.0f, g.u0, g.v1
		};
		unsigned int indices[6] = { 0, 1, 2, 2, 3, 0 };

		VertexArray va(vertices, 4, indices, 6, 5);
		va.SetActive();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		penX += g.xAdvance * scale;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

//void Renderer::CreateDynamicText(const char* pText, int pointsize, int x, int y)
//{
//	DynamicText* pTexture = new DynamicText("arial.ttf", pointsize, x, y);
//	m_pTextureManager->AddTexture(pText, pTexture);
//}

void Renderer::DrawAABB(float x, float y, float width, float height)
 {
	m_pGridShader->SetActive();
	glPushMatrix();
	//glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	//glTranslatef(x, y, 0.0f);
	glBegin(GL_LINE_LOOP);
	/*glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);*/
	glVertex2f(10.f, 10.f);
	glEnd();

	
	glEnable(GL_BLEND);
	//glEnable(GL_TEXTURE_2D);
	glPopMatrix();
}

void Renderer::DrawDebug()
{
	m_pTextureManager->DebugDraw();
}

void Renderer::DrawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color)
{
	// point 1
	m_fLineData.push_back(p1.x);
	m_fLineData.push_back(p1.y);
	m_fLineData.push_back(p1.z);

	// color
	/*m_fLineData.push_back(color.r);
	m_fLineData.push_back(color.g);
	m_fLineData.push_back(color.b);*/

	// point 2
	m_fLineData.push_back(p2.x);
	m_fLineData.push_back(p2.y);
	m_fLineData.push_back(p2.z);

	// color
	/*m_fLineData.push_back(color.r);
	m_fLineData.push_back(color.g);
	m_fLineData.push_back(color.b);*/
}

void Renderer::DrawLine2D(const glm::vec2& p1, const glm::vec2& p2, const glm::vec3& color)
{
	DrawLine(glm::vec3(p1, 0), glm::vec3(p2, 0), color);
}

void Renderer::DrawLineFlush(Camera* pCamera)
{
	m_pGridShader->SetActive();

	const float PI = 3.14159f;
	float angleInRadians = (0 * PI) / 180.0f;
	const float sizeX = 1;
	const float sizeY = 1;

	Matrix4 world;
	SetIdentity(world);
	/*world.m[0][0] = cosf(angleInRadians) * (sizeX);
	world.m[0][1] = -sinf(angleInRadians) * (sizeX);
	world.m[1][0] = sinf(angleInRadians) * (sizeY);
	world.m[1][1] = cosf(angleInRadians) * (sizeY);*/

	m_pGridShader->SetMatrixUniform("uWorldTransform", world);
	Camera* activeCam = pCamera ? pCamera : g_pDefaultCamera;
	Matrix4 view = activeCam->GetViewMatrix();
	Matrix4 orthoViewProj;
	CreateOrthoProjection(orthoViewProj, static_cast<float>(m_iWidth), static_cast<float>(m_iHeight));
	m_pGridShader->SetVector4Uniform("color", 1,1,1,1);
	Matrix4 newView = Multiply(view, orthoViewProj);
	m_pGridShader->SetMatrixUniform("uViewProj", newView);

	static bool created = false;

	if (!created)
	{
		created = true;
	
		m_pGridVertexData = new VertexArray(m_fLineData.data(), m_fLineData.size(), nullptr, 0, 3);
	}
	else
	{
		m_pGridVertexData->dummy(m_fLineData.data(), m_fLineData.size(), 3);
	}

	// 6 floats make up a vertex (3 position 3 color)
	// divide by that to get number of vertices to draw
	int count = m_fLineData.size() / 3;
	m_pGridVertexData->SetActive(); // Bind the vertex array
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	/*glLineWidth(2.0f);*/
	glDrawArrays(GL_LINES, 0, count);

	m_fLineData.clear();
	//delete m_pGridVertexData;
	//m_pGridVertexData = 0;
}

void Renderer::GenerateGrid(int gSize, int cSize)
{
	const int MAX_SIZE = gSize * cSize;
	//const int GRID_SIZE = gSize;
	const int CELL_SPACING = cSize;

	for (int y = 0; y <= MAX_SIZE; y += CELL_SPACING)
	{
		DrawLine2D({ 0, y }, { MAX_SIZE, y });
	}

	for (int x = 0; x <= MAX_SIZE; x += CELL_SPACING)
	{
		DrawLine2D({ x, 0 }, { x, MAX_SIZE });
	}

}

bool Renderer::ReloadShaders()
{
	bool spriteShader = m_pSpriteShader->Load("shaders\\sprite.vert", "shaders\\sprite.frag");
	bool outlineShader = m_pOutlineShader->Load("shaders\\outline.vert", "shaders\\outline.frag", "shaders\\outline.geom");
	if (!spriteShader || !outlineShader)
	{
		LogManager::GetInstance().Log("Failed to reload shaders!");
		return false;
	}
	LogManager::GetInstance().Log("Shaders reloaded");
	return true;
}