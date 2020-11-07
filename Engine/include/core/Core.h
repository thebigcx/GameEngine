#pragma once

#include <memory>

template<typename T>
using Shared = std::shared_ptr<T>;

template<typename T, typename ... Args>
constexpr Shared<T> createShared(Args&& ... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
using Unique = std::unique_ptr<T>;

template<typename T, typename ... Args>
constexpr Unique<T> createUnique(Args&& ... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

#define ENGINE_ASSERT(check, msg) { if (!(check)) { std::cout << "Assertion failed.\n"; } }