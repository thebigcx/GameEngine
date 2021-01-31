#include <renderer/Assets.h>

namespace Engine
{

Assets* Assets::m_instance = new Assets();

Assets::Assets()
{

}

Assets::~Assets()
{
    flush();
}

void Assets::flush()
{
    for (auto& cache : m_instance->m_caches)
    {
        delete cache.second;
    }

    m_instance->m_caches.clear();
}

}