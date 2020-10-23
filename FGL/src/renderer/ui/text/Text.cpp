#include <renderer/ui/text/Text.h>

Text::Text(Font& p_font)
: m_pFont(&p_font)
{

}

void Text::setColor(const Color& color)
{
    m_color = color;
}

void Text::setString(const std::string& string)
{
    m_string = string;
}

void Text::setFont(Font& font)
{
    m_pFont = &font;
}