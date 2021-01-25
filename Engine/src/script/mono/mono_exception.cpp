#include <script/mono/mono_exception.h>
#include <script/mono/mono_object.h>

namespace Engine
{
namespace mono
{

thunk_exception::exception_info get_exception_info(MonoObject* ex)
{
    auto obj = object(ex);
    std::string type = obj.get_type().get_fullname();

    /*auto message_property = type.get_property("Message");
    auto message = make_property_invoker<std::string>(message_property).get_value(obj);

    auto stacktrace_property = type.get_property("StackTrace");
    auto stacktrace = make_property_invoker<std::string>(stacktrace_property).get_value(obj);*/
    std::string message = "";
    std::string stacktrace = "";

    return { type, message, stacktrace };
}

thunk_exception::thunk_exception(MonoObject* obj)
    : thunk_exception(get_exception_info(obj))
{

}

thunk_exception::thunk_exception(const exception_info& inf)
    : exception(inf.exception_typename + "(" + inf.message + ")\n" + inf.stacktrace), m_info(inf)
{

}

}
}