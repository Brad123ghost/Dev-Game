#pragma once
#ifndef __NEWENTITY_H_
#define __NEWENTITY_H_

#include <memory>
#include <string>

#include "vector2.h"
#include "ctransform.h"
#include "cinput.h"
#include "csprite.h"
#include "componentstorage.h"

enum class eTag
{
	DEFAULT,
	PLAYER,
	ENEMY,
	PROJECTILE,
	ITEM
};

//typedef std::tuple<CTransform, CInput, CSprite> Components;

class NewEntity
{
public:
	friend class EntityManager;

	NewEntity(size_t id, eTag tag = eTag::DEFAULT, ComponentStorage* storage = nullptr);
	~NewEntity();

	void SetDead();
	bool IsAlive() const;
	eTag GetTag() const;
	std::string GetTagString() const;
	size_t GetId() const;



public:
	/*std::shared_ptr<CTransform> cTransform;
	std::shared_ptr<CInput> cInput;
	std::shared_ptr<CSprite> cSprite;*/
	//Components m_components;
	
	template <typename T>
	T* GetComponent()
	{
		return m_pComponentStorage->GetComponent<T>(m_iId);
	}

	template<typename T, typename... Args> 
	T& AddComponent(Args&&... args)
	{
		return m_pComponentStorage->AddComponent<T>(m_iId, std::forward<Args>(args)...);
	}

	template <typename T> 
	void RemoveComponent()
	{
		m_pComponentStorage->RemoveComponent<T>(m_iId);
	}

private:
	bool m_bAlive;
	eTag m_eTag;
	size_t m_iId = 0;
	ComponentStorage* m_pComponentStorage;
};

#endif // __NEWENTITY_H_