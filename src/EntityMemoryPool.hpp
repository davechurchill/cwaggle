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
    std::vector<CController>,
    std::vector<CSensorArray>,
    std::vector<CRobotType>,
    std::vector<CSteer>,
    std::vector<CColor>
> EntityData;

class EntityMemoryPool
{
    EntityData  m_data;
    size_t      m_numEntities = 0;

    std::vector<std::string>    m_tags;
    std::vector<bool>           m_active;
    std::vector<std::bitset<MaxComponents>> m_hasComponent;

    EntityMemoryPool()
    {
        getData<CTransform>().resize(MaxEntities);
        getData<CCircleBody>().resize(MaxEntities);
        getData<CCircleShape>().resize(MaxEntities);
        getData<CLineBody>().resize(MaxEntities);
        getData<CSensorArray>().resize(MaxEntities);
        getData<CSteer>().resize(MaxEntities);
        getData<CRobotType>().resize(MaxEntities);
        getData<CController>().resize(MaxEntities);
        getData<CColor>().resize(MaxEntities);
        m_hasComponent.resize(MaxEntities);
        m_tags.resize(MaxEntities);
        m_active.resize(MaxEntities);
    }

public:

    inline static EntityMemoryPool & Instance()
    {
        static EntityMemoryPool instance;
        return instance;
    }

    inline size_t addEntity(const std::string & tag)
    {
        getData<CTransform>()[m_numEntities]    = {};
        getData<CCircleBody>()[m_numEntities]   = {};
        getData<CCircleShape>()[m_numEntities]  = {};
        getData<CLineBody>()[m_numEntities]     = {};
        getData<CSensorArray>()[m_numEntities]  = {};
        getData<CColor>()[m_numEntities]        = {};
        getData<CRobotType>()[m_numEntities]    = {};
        getData<CController>()[m_numEntities]   = {};
        getData<CSteer>()[m_numEntities]        = {};
        m_hasComponent[m_numEntities]           = {};
        m_tags[m_numEntities]                   = tag;
        m_active[m_numEntities]                 = true;

        // return the pointer to the entity
        return m_numEntities++;
    }

    inline decltype(m_active) & getActive()
    {
        return m_active;
    }

    inline const decltype(m_tags) & getTags() const
    {
        return m_tags;
    }

    inline decltype(m_hasComponent) & hasComponent() 
    {
        return m_hasComponent;
    }

    template <typename T>
    inline std::vector<T> & getData()
    {
        return std::get<std::vector<T>>(m_data);
    }
};

