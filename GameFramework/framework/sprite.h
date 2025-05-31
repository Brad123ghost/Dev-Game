// COMP710 GP Framework 2024
#ifndef __SPRITE_H_
#define __SPRITE_H_

#include "vector2.h"

// Forward Declarations:
class Renderer;
class Texture;
class Camera;

class Sprite
{
	// Member methods:
public:
	Sprite();
	~Sprite();

	bool Initialize(Texture& texture);
	void ReplaceTexture(Texture& texture);
	void Process(float deltaTime);
	void Draw(Renderer& renderer, Camera* camera = nullptr);

	int GetWidth() const;
	int GetHeight() const;

	void SetX(int x);
	int GetX() const;
	void SetY(int y);
	int GetY() const;

	void SetWorldPos(Vector2 worldPos);
	Vector2 GetWorldPos() const;

	void SetAngle(float angle);
	float GetAngle() const;

	void SetScale(Vector2 scale) const; 
	void SetScale(float scale);
	void SetXScale(float scale);
	void SetYScale(float scale);
	Vector2 GetScale() const;

	void SetAlpha(float alpha);
	float GetAlpha() const;

	void SetRedTint(float value);
	float GetRedTint() const;
	void SetGreenTint(float value);
	float GetGreenTint() const;
	void SetBlueTint(float value);
	float GetBlueTint() const;

	Texture* GetTexture() const;
	unsigned int GetTextureId() const;
	const char* GetTexturePath() const;

protected:
	float Clamp(float minimum, float vlaue, float maximum);

private:
	Sprite(const Sprite& sprite);
	Sprite& operator=(const Sprite& sprite);

	// Member data:
public:

protected:
	Texture* m_pTexture;
	int m_x;
	int m_y;

	Vector2 m_vWorldPos;

	float m_angle;
	int m_centerX;
	int m_centerY;

	int m_width;
	int m_height;

	float m_xScale;
	float m_yScale;
	float m_alpha;

	float m_tintRed;
	float m_tintGreen;
	float m_tintBlue;

private:
};

#endif // __SPRITE_H_