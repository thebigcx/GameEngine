#pragma once

#include <memory>

namespace Engine
{

template<typename T>
using Shared = std::shared_ptr<T>;

template<typename T, typename... Args>
constexpr Shared<T> createShared(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
using Unique = std::unique_ptr<T>;

template<typename T, typename... Args>
constexpr Unique<T> createUnique(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

}

#define ENGINE_ASSERT(check, msg) { if (!(check)) { std::cout << "* Assertion failed! *\n" << "Reason: " << msg << "\n"; } }

#define EXPAND_MACRO(x) x
#define STINGIFY_MACRO(x) #x

#define BEGIN_PROFILE_SESSION(s) std::cout << "[PROFILE] " << s << "\n"
#define END_PROFILE_SESSION(s)