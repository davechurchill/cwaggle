#include "Entity.h"
#include "EntityMemoryPool.h"

EntityMemoryPool & Entity::getMemoryPool()
{
    return EntityMemoryPool::Instance();
}

bool Entity::isActive()
{
    return getMemoryPool().getActive()[m_id];
}

void Entity::setActive(bool active)
{
    getMemoryPool().getActive()[m_id] = active;
}

const std::string & Entity::tag()
{
    return getMemoryPool().getTags()[m_id];;
}