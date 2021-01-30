#include <script/mono/Assembly.h>
#include <script/mono/Image.h>
#include <script/mono/Type.h>

namespace Engine
{
namespace Mono
{

Image Assembly::getImage()
{
    return Image(mono_assembly_get_image(m_assembly));
}

Type Assembly::getType(const std::string& nameSpace, const std::string& name)
{
    auto image = getImage();
    return Type(mono_class_from_name(image.get(), nameSpace.c_str(), name.c_str()));
}

}
}