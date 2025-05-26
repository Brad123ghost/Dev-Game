#pragma once
#ifndef __COMPONENTSTORAGE_H_
#define __COMPONENTSTORAGE_H_

#include <vector>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <stdexcept>
#include <iostream>

// Base class for type-erased component arrays
class IComponentArray
{
public:
    virtual ~IComponentArray() = default;
};

// Templated derived class for storing components of a specific type
template <typename T>
class ComponentArray : public IComponentArray
{
public:
    std::vector<std::unique_ptr<T>> components;

    T& AddComponent(size_t entityId, T&& component)
    {
        if (entityId >= components.size())
        {
            components.resize(entityId + 1);
        }
        components[entityId] = std::make_unique<T>(std::move(component));
        return *components[entityId];
    }

    T* GetComponent(size_t entityId)
    {
        // Check if the entityId is valid and the component exists
        if (entityId >= components.size() || !components[entityId])
        {
            return nullptr; // Return nullptr if the component does not exist
        }
        return components[entityId].get();
    }

    void RemoveComponent(size_t entityId)
    {
        // Check if the entityId is valid before attempting to remove
        if (entityId < components.size())
        {
            components[entityId].reset();
        }
    }
};

class ComponentStorage
{
public:
    ComponentStorage()
    {
        std::cout << "ComponentStorage initialized." << std::endl;
    }

    template <typename T, typename... Args>
    T& AddComponent(size_t entityId, Args&&... args)
    {
        auto& componentArray = GetOrCreateComponentArray<T>();
        return componentArray.AddComponent(entityId, T(std::forward<Args>(args)...));
    }

    template <typename T>
    T* GetComponent(size_t entityId)
    {
        auto& componentArray = GetOrCreateComponentArray<T>();
        return componentArray.GetComponent(entityId);
    }

    template <typename T>
    void RemoveComponent(size_t entityId)
    {
        auto& componentArray = GetOrCreateComponentArray<T>();
        componentArray.RemoveComponent(entityId);
    }

private:
    /*template <typename T>
    ComponentArray<T>& GetOrCreateComponentArray()
    {
        auto typeId = std::type_index(typeid(T));
        if (m_componentArrays.find(typeId) == m_componentArrays.end())
        {
            m_componentArrays[typeId] = std::make_unique<ComponentArray<T>>();
        }
        return *static_cast<ComponentArray<T>*>(m_componentArrays[typeId].get());
    }*/
    std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> m_componentArrays;

    template <typename T>
    ComponentArray<T>& GetOrCreateComponentArray()
    {
        auto typeId = std::type_index(typeid(T));

        // Debug log
        //std::cout << "Accessing component array for type: " << typeId.name() << std::endl;

        // Check if m_componentArrays is valid
        if (m_componentArrays.empty())
        {
            std::cout << "Warning: m_componentArrays is empty!" << std::endl;
        }

        // Check if the component array exists
        auto it = m_componentArrays.find(typeId);
        if (it == m_componentArrays.end())
        {
            //std::cout << "Component array not found. Creating new array." << std::endl;
            m_componentArrays[typeId] = std::make_unique<ComponentArray<T>>();
        }

        // Debug log
        //std::cout << "Returning component array for type: " << typeId.name() << std::endl;

        return *static_cast<ComponentArray<T>*>(m_componentArrays[typeId].get());
    }

private:
    //std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> m_componentArrays;
   
};

#endif // __COMPONENTSTORAGE_H_
