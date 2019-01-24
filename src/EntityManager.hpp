#pragma once

#include <vector>

#include "Entity.hpp"
#include "EntityMemoryPool.hpp"

typedef std::vector<Entity> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;

class EntityManager
{
    EntityMemoryPool    m_entityPool;
    std::vector<Entity> m_entities;
    std::vector<Entity> m_entitiesToAdd;
    std::vector<Entity> m_entitiesToRemove;
    EntityMap           m_entityMap;
    size_t              m_totalEntities = 0;

    // helper function to avoid repeated code
    void EntityManager::removeDeadEntities(std::vector<Entity> & vec)
    {
        // use std::remove_if to remove dead entities
        // probably not the fastest solution, but it is safe
        vec.erase(std::remove_if(vec.begin(), vec.end(),
            [](Entity entity) { return !entity.isActive(); }), vec.end());
    }
    

public:

    EntityManager::EntityManager(size_t maxEntities = 20000)
        : m_entityPool(maxEntities)
    {
        m_entities.reserve(maxEntities);
        m_entitiesToAdd.reserve(maxEntities);
        m_entitiesToRemove.reserve(maxEntities);
    }

    void EntityManager::update()
    {
        if (m_entitiesToAdd.size() > 0)
        {
            // add all the entities that are pending
            for (auto e : m_entitiesToAdd)
            {
                // add it to the vector of all entities
                m_entities.push_back(e);

                // add it to the entity map in the correct place
                // map[key] will create an element at 'key' if it does not already exist
                //          therefore we are not in danger of adding to a vector that doesn't exist
                m_entityMap[e.tag()].push_back(e);
            }

            // clear the temporary vector since we have added everything
            m_entitiesToAdd.clear();
        }

        if (m_entitiesToRemove.size() > 0)
        {
            // clean up dead entities in all vectors
            removeDeadEntities(m_entities);
            for (auto & kv : m_entityMap)
            {
                // kv is a key-value pair contained in the map
                //    key   (kv.first):  the tag string
                //    value (kv.second): the vector storing entities
                removeDeadEntities(kv.second);
            }

            m_entitiesToRemove.clear();
        }
    }

    Entity EntityManager::addEntity(const std::string & tag)
    {
        auto newEntityPtr = m_entityPool.addEntity(tag);

        // add it to the vector of entities that will be added on next update() call
        m_entities.push_back(newEntityPtr);

        // return the pointer to the entity
        return newEntityPtr;
    }

    void EntityManager::destroyEntity(Entity entity)
    {
        entity.setActive(false);
        m_entitiesToRemove.push_back(entity);
    }

    EntityVec & EntityManager::getEntities()
    {
        return m_entities;
    }

    EntityVec & EntityManager::getEntities(const std::string & tag)
    {
        // return the vector in the map where all the entities with the same tag live
        return m_entityMap[tag];
    }
};