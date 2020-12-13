#include <scene/ecs/Registry.h>

template<typename T, typename Func>
void EntityView::each(const Func& func)
{
    for (auto& entity : m_entities)
    {
        if (entity->getRegistry()->has<T>(entity))
        {
            func(entity, entity->getRegistry()->get<T>(entity));
        }
    }
}