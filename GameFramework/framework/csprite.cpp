#include "csprite.h"

CSprite::CSprite(std::shared_ptr<Sprite> sprite)
    : m_sprite(sprite)
{
}

void CSprite::SetSprite(std::shared_ptr<Sprite> sprite)
{
    m_sprite = sprite;
}

std::shared_ptr<Sprite> CSprite::GetSprite() const
{
    return m_sprite;
}