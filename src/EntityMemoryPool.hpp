#pragma once

#include "Components.hpp"

#include <vector>

const size_t MaxEntities = 20000;
const size_t MaxComponents = 32;

typedef std::tuple <
    std::vector<CTransform>,
    std::vector<CCircleBody>,
    std::vector<CCircleShape>,
    std::vector<CLineBody>,
    std::vector<CColor>,
    std::vector<std::bitset<MaxComponents>>
> EntityData;

class EntityMemoryPool
{
    EntityData  m_data;
    size_t      m_numEntities = 0;

    std::vector<std::string> m_tags;
    std::vector<bool> m_active;

    EntityMemoryPool()
    {
        getData<CTransform>().resize(MaxEntities);
        getData<CCircleBody>().resize(MaxEntities);
        getData<CCircleShape>().resize(MaxEntities);
        getData<CLineBody>().resize(MaxEntities);
        getData<CColor>().resize(MaxEntities);
        getData<std::bitset<MaxComponents>>().resize(MaxEntities);
        m_tags.resize(MaxEntities);
        m_active.resize(MaxEntities);
    }

public:

    inline static EntityMemoryPool & Instance()
    {
        static EntityMemoryPool instance;
        return instance;
    }

    inline size_t EntityMemoryPool::addEntity(const std::string & tag)
    {
        getData<CTransform>()[m_numEntities] = {};
        getData<CCircleBody>()[m_numEntities] = {};
        getData<CCircleShape>()[m_numEntities] = {};
        getData<CLineBody>()[m_numEntities] = {};
        getData<CColor>()[m_numEntities] = {};
        getData<std::bitset<MaxComponents>>()[m_numEntities] = {};
        m_tags[m_numEntities] = tag;
        m_active[m_numEntities] = true;

        // return the pointer to the entity
        return m_numEntities++;
    }

    inline std::vector<bool> & getActive()
    {
        return m_active;
    }

    inline const std::vector<std::string> & getTags() const
    {
        return m_tags;
    }

    template <typename T>
    inline std::vector<T> & getData()
    {
        return std::get<std::vector<T>>(m_data);
    }
};

