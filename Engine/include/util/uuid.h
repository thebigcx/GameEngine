#pragma once

#include <util/sole.hpp>
#include <string>

namespace Engine
{
namespace Utils
{

inline std::string genUUID()
{
    sole::uuid id = sole::uuid4();
    return id.str();
}

}
}