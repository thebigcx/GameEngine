#pragma once

#include <vector>

#include <script/mono/mono_method.h>
#include <script/mono/mono_object.h>
#include <script/mono/mono_exception.h>

namespace Engine
{
namespace mono
{

template<typename... Args>
std::vector<void*> to_mono_args(Args&&... args)
{
    //return std::vector<void*>(args);
    return std::vector<void*>();
}

template<typename T>
class method_invoker;

template<typename... Args>
class method_invoker<void(Args...)> : public method
{
public:
    void operator()(Args&&... args)
    {
        invoke(nullptr, std::forward<Args>(args)...);
    }

    void operator()(const object& obj, Args&&... args)
    {
        invoke(&obj, std::forward<Args>(args)...);
    }

private:
    void invoke(const object* obj, Args&&... args)
    {
        auto method = m_method;

        MonoObject* mono_obj = nullptr;
        if (obj)
        {
            mono_obj = obj->get();
        }

        auto mono_args = to_mono_args(std::forward<Args>(args)...);

        MonoObject* ex = nullptr;
        mono_runtime_invoke(method, mono_obj, mono_args.data(), &ex);
        if (ex)
        {
            throw thunk_exception(ex);
        }
    }

    template<typename Sig>
    friend method_invoker<Sig> make_method_invoker(const method& m);

    method_invoker(const method& m)
        : method(m) {}
};

template<typename Ret, typename... Args>
class method_invoker<Ret(Args...)> : public method
{
public:
    Ret operator()(Args&&... args)
    {
        return invoke(nullptr, std::forward<Args>(args)...);
    }

    Ret operator()(const object& obj, Args&&... args)
    {
        return invoke(&obj, std::forward<Args>(args)...);
    }

private:
    Ret invoke(const object* obj, Args&&... args)
    {

    }

    template<typename Sig>
    friend method_invoker<Sig> make_method_invoker(const method& m);

    method_invoker(const method& m)
        : method(m) {}
};


template<typename Sig>
method_invoker<Sig> make_method_invoker(const method& m)
{
    return method_invoker<Sig>(m);
}

}
}