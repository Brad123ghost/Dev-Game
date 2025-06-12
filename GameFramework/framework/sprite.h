// COMP710 GP Framework 2024
#ifndef __SPRITE_H_
#define __SPRITE_H_

#include "vector2.h"

// Forward Declarations:
class Renderer;
class Texture;
class Camera;

enum DrawMode
{
	WORLD,
	SCREEN,
	OUTLINE
};

class Sprite
{
	// Member methods:
public:
	Sprite();
	~Sprite();

	bool Initialize(Texture& texture);
	void ReplaceTexture(Texture& texture);
	void Process(float deltaTime);
	/**
	 * @brief  Draw with the default viewport
	 *
	 * @param  renderer: reference to the renderer
	 * @param  mode (optionial): WORLD, SCREEN or OUTLINE, default is WORLD
	 * 
	 */
	void Draw(Renderer& renderer, DrawMode mode = WORLD);
	/**
	 * @brief  Draw with a specific camera
	 *
	 * @param  renderer: reference to the renderer
	 * @param  camera: current active camera.
	 * @param  mode (optionial): WORLD, SCREEN or OUTLINE, default is WORLD
	 *
	 */
	void Draw(Renderer& renderer, Camera* camera, DrawMode mode = WORLD);

	int GetWidth() const;
	int GetHeight() const;
	/**
	 * @brief  Set the X world position of the sprite.
	 *
	 * By default this is in world coordinates, 0 being at center of the screen.
	 * 
	 * If wanting to use as screen coordinates, pass SCREEN as a parameter when calling DRAW.
	 * 
	 */
	void SetX(int x);
	int GetX() const;
	/**
	 * @brief  Set the Y world position of the sprite.
	 *
	 * By default this is in world coordinates, 0 being at center of the screen.
	 *
	 * If wanting to use as screen coordinates, pass SCREEN as a parameter when calling DRAW.
	 *
	 */
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