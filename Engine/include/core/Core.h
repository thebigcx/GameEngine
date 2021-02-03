#pragma once

#include <memory>

namespace Engine
{

template<typename T>
using Reference = std::shared_ptr<T>;
// TODO: look into custom deleters to allow for private destructors
template<typename T, typename... Args>
constexpr Reference<T> createReference(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
using Owned = std::unique_ptr<T>;

template<typename T, typename... Args>
constexpr Owned<T> createOwned(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using NonOwning = std::weak_ptr<T>;

template<typename T>
constexpr NonOwning<T> createNonOwning(const Reference<T>& ref)
{
    return NonOwning<T>(ref);
}
}

#define ENGINE_ASSERT(check, msg) { if (!(check)) { std::cout << "* Assertion failed! *\n" << "Reason: " << msg << "\n"; } }

#define EXPAND_MACRO(x) x
#define STINGIFY_MACRO(x) #x

#define BEGIN_PROFILE_SESSION(s) std::cout << "[PROFILE] " << s << "\n"
#define END_PROFILE_SESSION(s)