// COMMP710 GP Framework 2024
// This include:
#include "sprite.h"

// Local includes:
#include "renderer.h"
#include "texture.h"

Sprite::Sprite()
	: m_pTexture(0)
	, m_x(0)
	, m_y(0)
	, m_vWorldPos(0.0f, 0.0f)
	, m_width(0)
	, m_height(0)
	, m_angle(0.0f)
	, m_centerX(0)
	, m_centerY(0)
	, m_xScale(1.0f)
	, m_yScale(1.0f)
	, m_alpha(1.0f)
	, m_tintRed(1.0f)
	, m_tintGreen(1.0f)
	, m_tintBlue(1.0f)
{

}

Sprite::~Sprite()
{

}

bool Sprite::Initialize(Texture& texture)
{
	m_pTexture = &texture;

	m_width = m_pTexture->GetWidth();
	m_height = m_pTexture->GetHeight();
	m_centerX = m_width / 2.0f;
	m_centerY = m_height / 2.0f;
	return true;
}

void Sprite::ReplaceTexture(Texture& texture)
{
	if (m_pTexture != &texture)
	{
		m_pTexture = &texture;
		m_width = m_pTexture->GetWidth();
		m_height = m_pTexture->GetHeight();
		m_centerX = m_width / 2.0f;
		m_centerY = m_height / 2.0f;
	}
}

void Sprite::Process(float deltaTime)
{

}

void Sprite::Draw(Renderer& renderer, DrawMode mode)
{
	Draw(renderer, nullptr, mode);
}

void Sprite::Draw(Renderer& renderer, Camera* camera, DrawMode mode)
{
	m_pTexture->SetActive();
	if(mode == WORLD)
		renderer.DrawSprite(*this, camera);
	else if(mode == SCREEN)
		renderer.DrawUI(*this);
}

void Sprite::SetX(int x)
{
	m_x = x;
}

void Sprite::SetY(int y)
{
	m_y = y;
}

int Sprite::GetX() const
{
	return m_x;
}

int Sprite::GetY() const
{
	return m_y;
}

void Sprite::SetWorldPos(Vector2 worldPos)
{
	/*m_vWorldPos.x = worldPos.x - m_centerX * m_scale;
	m_vWorldPos.y = worldPos.y - m_centerY * m_scale;*/
	m_vWorldPos = worldPos;
}

Vector2 Sprite::GetWorldPos() const
{

	return m_vWorldPos;
}

void Sprite::SetAngle(float angle)
{
	angle += 360;
	while (angle > 360.0f)
	{
		angle -= 360.0f;
	}

	m_angle = angle;
}

float Sprite::GetAngle() const
{
	return m_angle;
}

void Sprite::SetScale(Vector2 scale) const
{
	scale.x = m_xScale;
	scale.y = m_yScale;
}

void Sprite::SetScale(float scale)
{
	m_xScale = scale;
	m_yScale = scale;
}

void Sprite::SetXScale(float scale)
{
	m_xScale = scale;
}
void Sprite::SetYScale(float scale)
{
	m_yScale = scale;
}

Vector2 Sprite::GetScale() const
{
	return {m_xScale, m_yScale};
}

void Sprite::SetAlpha(float alpha)
{
	m_alpha = Clamp(0.0f, alpha, 1.0f);
}

float Sprite::GetAlpha() const
{
	return m_alpha;
}

int Sprite::GetWidth() const
{
	return static_cast<int>(ceilf(m_width * m_xScale));
}

int Sprite::GetHeight() const
{
	return static_cast<int>(ceilf(m_height * m_yScale));
}

float Sprite::Clamp(float minimum, float value, float maximum)
{
	if (value > maximum)
	{
		value = maximum;
	}
	else if (value < minimum)
	{
		value = minimum;
	}

	return value;
}

void Sprite::SetRedTint(float value)
{
	m_tintRed = Clamp(0.0f, value, 1.0f);
}

float Sprite::GetRedTint() const
{
	return m_tintRed;
}

void Sprite::SetGreenTint(float value)
{
	m_tintGreen = Clamp(0.0f, value, 1.0f);
}

float Sprite::GetGreenTint() const
{
	return m_tintGreen;
}

void Sprite::SetBlueTint(float value)
{
	m_tintBlue = Clamp(0.0f, value, 1.0f);
}

float Sprite::GetBlueTint() const
{
	return m_tintBlue;
}

Texture* Sprite::GetTexture() const
{
	return m_pTexture;
}

unsigned int Sprite::GetTextureId() const
{
	return m_pTexture->GetTextureId();
}

const char* Sprite::GetTexturePath() const
{
	if (m_pTexture)
	{
		return m_pTexture->GetPath();
	}
	return nullptr;
}