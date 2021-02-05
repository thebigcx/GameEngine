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
class NonOwning
{
public:
    explicit constexpr NonOwning()
    {

    }

    constexpr NonOwning(const Reference<T>& ref)
        : m_value(ref) {}

    constexpr NonOwning(const std::weak_ptr<T>& ptr)
        : m_value(ptr) {}

    inline constexpr bool valid() const
    {
        return m_value.lock() != nullptr;
    }

    inline constexpr Reference<T> operator->()
    {
        return m_value.lock();
    }

    inline constexpr T& operator*()
    {
        return *m_value.lock();
    }

    inline constexpr void operator=(const Reference<T>& other)
    {
        m_value = other;
    }

    inline constexpr operator Reference<T>()
    {
        return m_value.lock();
    }

private:
    std::weak_ptr<T> m_value;
};

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