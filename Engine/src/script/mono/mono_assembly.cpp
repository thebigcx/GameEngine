#include <script/mono/mono_assembly.h>

namespace Engine
{
namespace mono
{

image assembly::get_image()
{
    return image(mono_assembly_get_image(m_assembly));
}

type assembly::get_type(const std::string& namespace_, const std::string& name)
{
    auto image = get_image();
    return type(mono_class_from_name(image.get(), namespace_.c_str(), name.c_str()));
}

}
}