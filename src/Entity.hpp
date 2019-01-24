#pragma once

#include "Components.hpp"
#include "EntityMemoryPool.hpp"
#include <cassert>
#include <vector>

class EntityManager;

const size_t MaxComponents = 32;

inline size_t GetComponentTypeID()
{
    static size_t lastID = 0;
    return lastID++;
}

template <typename T>
inline size_t GetComponentTypeID()
{
    static size_t typeID = GetComponentTypeID();
    return typeID;
}


class Entity
{
    friend class EntityMemoryPool;
    friend class EntityManager;

    size_t m_id;

public:

    Entity(const size_t id = 0) : m_id(id) {}

    size_t id() const { return m_id; }
    bool isActive() 
    {
        return std::get<std::vector<bool>>(EntityMemoryPool::Instance().getEntityData())[m_id];
    }

    void setActive(bool active)
    {
        std::get<std::vector<bool>>(EntityMemoryPool::Instance().getEntityData())[m_id] = active;
    }

    std::string & tag()
    {
        return std::get<std::vector<std::string>>(EntityMemoryPool::Instance().getEntityData())[m_id];
    }
/*
    template <typename T>
    inline bool hasComponent() const
    {
        return m_hasComponent[GetComponentTypeID<T>()];
    }*/

    template <typename T, typename... TArgs>
    inline T & addComponent(TArgs&&... mArgs)
    {
        getComponent<T>() = T(std::forward<TArgs>(mArgs)...);
        return getComponent<T>();
    }

    template<typename T>
    inline T & getComponent()
    {
        std::vector<T> & vec = std::get<std::vector<T>>(EntityMemoryPool::Instance().getEntityData());
        return vec[m_id];
    }

    template<typename T>
    inline void removeComponent()
    {
        //m_hasComponent[GetComponentTypeID<T>()] = false;
    }
};

