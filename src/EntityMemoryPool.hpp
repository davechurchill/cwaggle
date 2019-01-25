#pragma once

#include "Components.hpp"

#include <vector>

const size_t MaxEntities = 20000;

typedef std::tuple <
    std::array<CTransform, MaxEntities>,
    std::array<CBody, MaxEntities>,
    std::array<CColor, MaxEntities>,
    std::array<std::string, MaxEntities>,
    std::array<bool, MaxEntities>
> EntityData;

class EntityMemoryPool
{
    friend class EntityManager;

    EntityData  * m_data;
    size_t      m_numEntities = 0;

    EntityMemoryPool()
    {
        m_data = new EntityData();
        //std::get<std::vector<CTransform>>(m_data).resize(size);
        //std::get<std::vector<CBody>>(m_data).resize(size);
        //std::get<std::vector<CColor>>(m_data).resize(size);
        //std::get<std::vector<std::string>>(m_data).resize(size);
        //std::get<std::vector<bool>>(m_data).resize(size, false);
    }

public:

    inline static EntityMemoryPool & Instance()
    {
        static EntityMemoryPool instance;
        return instance;
    }

    inline size_t EntityMemoryPool::addEntity(const std::string & tag)
    {
        getData<CTransform>()[m_numEntities] = CTransform();
        getData<CBody>()[m_numEntities] = CBody();
        getData<CColor>()[m_numEntities] = CColor();
        getData<std::string>()[m_numEntities] = tag;
        getData<bool>()[m_numEntities] = true;

        // return the pointer to the entity
        return m_numEntities++;
    }

    template <typename T>
    inline std::array<T, MaxEntities> & getData()
    {
        return std::get<std::array<T, MaxEntities>>(*m_data);
    }
};
