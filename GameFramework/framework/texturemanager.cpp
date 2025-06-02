// COMP710 GP Framework 2024

// This include:
#include "texturemanager.h"

// Local includes:
#include "texture.h"
#include "logmanager.h"
#include "imgui.h"

// Library includes:
#include <cassert>
#include <SDL.h>
#include <iostream>
TextureManager::TextureManager()
	: m_bShowSelect(false)
{

}

TextureManager::~TextureManager()
{
	std::map<std::string, Texture*>::iterator iter = m_pLoadedTextures.begin();

	while (iter != m_pLoadedTextures.end())
	{
		Texture* pTexture = iter->second;
		delete pTexture;
		pTexture = 0;
		++iter;
	}

	m_pLoadedTextures.clear();
}

bool TextureManager::Initialize()
{
	LogManager::GetInstance().Log("[Info] TextureManager starting...");

	return true;
}

Texture* TextureManager::GetTexture(const char* pcFilename)
{
	Texture* pTexture = 0;

	if (m_pLoadedTextures.find(pcFilename) == m_pLoadedTextures.end())
	{
		// Not already loaded... so load...
		pTexture = new Texture();
		if (!pTexture->Initialize(pcFilename))
		{
			LogManager::GetInstance().Log("[Texture Manager] Texture failed to initialize!");
			assert(0);
		}
		m_TextureKeys.push_back(pcFilename);
		m_pLoadedTextures[pcFilename] = pTexture;
	}
	else
	{
		// It has already been loaded...
		pTexture = m_pLoadedTextures[pcFilename];
	}

	return pTexture;
}

void TextureManager::AddTexture(const char* key, Texture* pTexture)
{
	m_pLoadedTextures[key] = pTexture;
}

void TextureManager::DebugDraw()
{
	ImGui::Text("TextureManager");
	
	ImGui::Text("Stores %d textures", m_pLoadedTextures.size());

	std::map<std::string, Texture*>::iterator iter = m_pLoadedTextures.begin();
	
	float windowWidth = ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x;
	size_t count = 0;
	while (iter != m_pLoadedTextures.end())
	{
		Texture* pTexture = iter->second;
		pTexture->DebugDraw();
		ImGuiStyle& style = ImGui::GetStyle();
		float lastTexture = ImGui::GetItemRectMax().x;
		float nextTexture = lastTexture + style.ItemSpacing.x +50; // 50 is texture thumbnail width
	
		if (count + 1 < m_pLoadedTextures.size() && nextTexture < windowWidth)
			ImGui::SameLine();
		++iter;
		count++;
	}
}

void TextureManager::SelectTextureDebugDraw()
{
	if (m_bShowSelect)
	{
		std::map<std::string, Texture*>::iterator iter = m_pLoadedTextures.begin();
		ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_Once);
		ImGui::Begin("Replace Texture", &m_bShowSelect);
		ImGui::Text("Select texture window");
		float windowWidth = ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x;
		size_t count = 0;
		for (std::string e : m_TextureKeys)
		{
			Texture* pTexture = m_pLoadedTextures[e];
			pTexture->DebugDraw();
			ImGuiStyle& style = ImGui::GetStyle();
			float lastTexture = ImGui::GetItemRectMax().x;
			float nextTexture = lastTexture + style.ItemSpacing.x + 50; // 50 is texture thumbnail width

			if (count + 1 < m_pLoadedTextures.size() && nextTexture < windowWidth)
				ImGui::SameLine();
			count++;
		}
		ImGui::End();
	}
}
void TextureManager::ToggleSelectTexture()
{
	m_bShowSelect = !m_bShowSelect;
}