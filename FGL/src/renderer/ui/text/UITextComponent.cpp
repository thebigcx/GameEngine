#include <renderer/ui/text/UITextComponent.h>

UITextComponent::UITextComponent()
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

void UITextComponent::setFont(TrueTypeFont& font)
{
    m_pFont = &font;
}