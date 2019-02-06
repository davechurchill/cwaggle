#pragma once

#include <cassert>
#include <vector>

class EntityManager;

#include "EntityMemoryPool.hpp"

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

    Entity(const size_t id = (size_t)-1) : m_id(id) {}

    size_t id() const 
    { 
        return m_id; 
    }

    operator size_t() const
    {
        return m_id;
    }

    bool operator == (Entity rhs) const
    {
        return m_id == rhs.m_id;
    }

    bool operator != (Entity rhs) const
    {
        return !(*this == rhs);
    }

    bool isActive()
    {
        return EntityMemoryPool::Instance().getActive()[m_id];
    }

    void setActive(bool active)
    {
        EntityMemoryPool::Instance().getActive()[m_id] = active;
    }

    const std::string & tag()
    {
        return EntityMemoryPool::Instance().getTags()[m_id];;
    }

    template <typename T>
    inline bool hasComponent()
    {
        return EntityMemoryPool::Instance().hasComponent()[m_id][GetComponentTypeID<T>()];
    }

    template <typename T, typename... TArgs>
    inline T & addComponent(TArgs&&... mArgs)
    {
        EntityMemoryPool::Instance().hasComponent()[m_id][GetComponentTypeID<T>()] = true;
        getComponent<T>() = T(std::forward<TArgs>(mArgs)...);
        return getComponent<T>();
    }

    template<typename T>
    inline T & getComponent()
    {
        static auto it = EntityMemoryPool::Instance().getData<T>().begin();
        return *(it + m_id);
    }

    template<typename T>
    inline void removeComponent()
    {
        getComponent<std::bitset<MaxComponents>>()[GetComponentTypeID<T>()] = false;
    }
};

