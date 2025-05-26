#pragma once
#ifndef __CSPRITE_H_
#define __CSPRITE_H_

#include "sprite.h"
#include <memory>
#include "basecomponent.h"

class CSprite : public BaseComponent
{
public:
    CSprite() = default;
    explicit CSprite(std::shared_ptr<Sprite> sprite);

    void SetSprite(std::shared_ptr<Sprite> sprite);
    std::shared_ptr<Sprite> GetSprite() const;

private:
    std::shared_ptr<Sprite> m_sprite;
};

#endif // __CSPRITE_H_