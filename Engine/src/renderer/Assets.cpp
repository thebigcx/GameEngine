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
    for (auto& list : m_instance->m_lists)
    {
        delete list.second;
    }

    m_instance->m_lists.clear();
}

}