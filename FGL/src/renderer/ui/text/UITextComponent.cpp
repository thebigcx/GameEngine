#include <renderer/ui/text/UITextComponent.h>

UITextComponent::UITextComponent(Font& p_font)
: m_pFont(&p_font)
{

}

void UITextComponent::setColor(const Color& color)
{
    m_color = color;
}

void UITextComponent::setString(const std::string& string)
{
    m_string = string;
}

void UITextComponent::setFont(Font& font)
{
    m_pFont = &font;
}