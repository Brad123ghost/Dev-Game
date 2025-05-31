// COMP710 GP Framework 2024

// This include:
#include "texture.h"

// Local includes:
#include "logmanager.h"

// Library include:
#include <SDL_image.h>
#include <cassert>
#include "glew.h"
#include "SDL_ttf.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <iostream>
Texture::Texture()
	: m_uiTextureId(0)
	, m_iHeight(0)
	, m_iWidth(0)
	, m_bIsSelected(false)
	, m_bIsOpen(false)
{

}

Texture::~Texture()
{
	glDeleteTextures(1, &m_uiTextureId);
}

bool Texture::Initialize(const char* pcFilename)
{
	m_pcName = pcFilename;
	SDL_Surface* pSurface = IMG_Load(pcFilename);

	if (pSurface)
	{
		m_iWidth = pSurface->w;
		m_iHeight = pSurface->h;

		int bytesPerPixel = pSurface->format->BytesPerPixel;

		unsigned int format = 0;

		if (bytesPerPixel == 3)
		{
			format = GL_RGB;
		}
		else if (bytesPerPixel == 4)
		{
			format = GL_RGBA;
		}

		glGenTextures(1, &m_uiTextureId);
		glBindTexture(GL_TEXTURE_2D, m_uiTextureId);

		glTexImage2D(GL_TEXTURE_2D, 0, format, m_iWidth, m_iHeight, 0, format, GL_UNSIGNED_BYTE, pSurface->pixels);

		SDL_FreeSurface(pSurface);
		pSurface = 0;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
	{
		LogManager::GetInstance().Log("Texture failed to load!");
		assert(0);
		return false;
	}

	return true;
}

void Texture::SetActive()
{
	glBindTexture(GL_TEXTURE_2D, m_uiTextureId);
}

int Texture::GetWidth() const
{
	assert(m_iWidth);
	return (m_iWidth);
}

int Texture::GetHeight() const
{
	assert(m_iHeight);
	return (m_iHeight);
}

void Texture::LoadTextTexture(const char* text, const char* fontname, int pointsize)
{
	m_pcName = text;
	TTF_Font* pFont = 0;
	TTF_Init();

	if (pFont == 0)
	{
		pFont = TTF_OpenFont(fontname, pointsize);
	}

	SDL_Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	SDL_Surface* pSurface = TTF_RenderText_Blended(pFont, text, color);

	LoadSurfaceIntoTexture(pSurface);

	TTF_CloseFont(pFont);
	pFont = 0;
}

void Texture::LoadFontAtlas(const char* text, SDL_Surface* pSurface)
{
	if (pSurface)
	{
		m_pcName = text;
		LoadSurfaceIntoTexture(pSurface);
	}
}

void Texture::LoadSurfaceIntoTexture(SDL_Surface* pSurface)
{
	if (pSurface)
	{
		m_iWidth = pSurface->w;
		m_iHeight = pSurface->h;
		int bytesPerPixel = pSurface->format->BytesPerPixel;
		unsigned int format = 0;

		if (bytesPerPixel == 3)
		{
			format = GL_RGB;
		}
		else if (bytesPerPixel == 4)
		{
			format = GL_RGBA;
		}
	
		// Unpack to render text correctly
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, pSurface->pitch / bytesPerPixel);
		glGenTextures(1, &m_uiTextureId);
		glBindTexture(GL_TEXTURE_2D, m_uiTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_iWidth, m_iHeight, 0, format, GL_UNSIGNED_BYTE, pSurface->pixels);
		// Reset the pack to render other sprites correctly
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		SDL_FreeSurface(pSurface);
		pSurface = 0;

		// Changed from nearest to linear to make text look better
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
}

void Texture::DebugDraw()
{
	ImGui::PushID(this);

	const float desiredWidth = 50.0f;
	const float desiredHeight = 50.0f;
	float widthScalingFactor = desiredWidth / static_cast<float>(m_iWidth);
	float heightScalingFactor = desiredHeight / static_cast<float>(m_iHeight);
	float scalingFactor = std::min(widthScalingFactor, heightScalingFactor);

	ImVec2 size;
	size.x = static_cast<float>(m_iWidth) * scalingFactor;
	size.y = static_cast<float>(m_iHeight) * scalingFactor;

	// Calculate display size
	const float display_size = 50.0f;
	ImVec2 selectableSize(display_size, display_size);

	// Get current position before drawing
	ImVec2 pos = ImGui::GetCursorScreenPos();

	ImVec2 offset;
	offset.x = (selectableSize.x - size.x) / 2.0f;
	offset.y = (selectableSize.y - size.y) / 2.0f;
	
	// Draw the image first
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddImage(
		(ImTextureID)(intptr_t)m_uiTextureId,
		ImVec2(pos.x + offset.x, pos.y + offset.y),
		ImVec2(pos.x + size.x + offset.x, pos.y + size.y + offset.y),
		ImVec2(0, 0),
		ImVec2(1, 1)
	);

	// Draw selectable on top of the image
	ImGui::SetCursorScreenPos(pos);
	if (ImGui::Selectable("##", &m_bIsSelected, 0, selectableSize))
	{
		m_bIsOpen = true;
	}

	if(!m_bIsSelected)
	{
		m_bIsOpen = false;
	}

	ImGui::PopID();
	if (m_bIsOpen)
	{
		std::string name = "Texture Info##" + std::to_string(m_uiTextureId);
		ImGui::Begin(name.c_str(), &m_bIsOpen,  ImGuiWindowFlags_AlwaysHorizontalScrollbar);
		ImGui::Text("Texture Name: %s", m_pcName);
		ImGui::Text("Texture ID: %d", m_uiTextureId);
		ImGui::Text("Texture Size: %d by %d", m_iWidth, m_iHeight);
		ImGui::Image((ImTextureID)(intptr_t)m_uiTextureId, { static_cast<float>(m_iWidth), static_cast<float>(m_iHeight) });
		ImGui::End();
	} 
	else
	{
		m_bIsSelected = false;
	}
}