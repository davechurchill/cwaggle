#pragma once

#include "Components.hpp"

#include <vector>

typedef std::tuple <
    std::vector<CBody>,
    std::vector<CColor>,
    std::vector<std::string>,
    std::vector<bool>
> EntityData;

class EntityMemoryPool
{
    friend class EntityManager;

    EntityData  m_data;
    size_t      m_numEntities = 0;

    EntityMemoryPool(size_t size)
    {
        std::get<std::vector<CBody>>(m_data).resize(size);
        std::get<std::vector<CColor>>(m_data).resize(size);
        std::get<std::vector<std::string>>(m_data).resize(size);
        std::get<std::vector<bool>>(m_data).resize(size, false);
    }

public:

    static EntityMemoryPool & Instance()
    {
        static EntityMemoryPool instance(20000);
        return instance;
    }

    size_t EntityMemoryPool::addEntity(const std::string & tag)
    {
        std::get<std::vector<CBody>>(m_data)[m_numEntities] = CBody();
        std::get<std::vector<CColor>>(m_data)[m_numEntities] = CColor();
        std::get<std::vector<std::string>>(m_data)[m_numEntities] = tag;
        std::get<std::vector<bool>>(m_data)[m_numEntities] = true;

        // return the pointer to the entity
        return m_numEntities++;
    }

    std::vector<CBody> & getBodies()
    {
        return std::get<std::vector<CBody>>(m_data);
    }

    EntityData & getEntityData()
    {
        return m_data;
    }
};
