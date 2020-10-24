#pragma once

#include <renderer/ui/text/TrueTypeFont.h>
#include <renderer/ui/UIComponent.h>
#include <util/Color.h>
#include <util/math/vector/Vector2.h>

class UITextComponent : public UIComponent
{
public:
    UITextComponent(TrueTypeFont& p_font);

    void setColor(const Color& color);

    void setString(const std::string& string);

    void setFont(TrueTypeFont& font);

    inline const Color& getColor() const { return m_color; };
    inline const TrueTypeFont* getFont()   const { return m_pFont; };

    inline const std::string& getString() const { return m_string; };

private:
    TrueTypeFont* m_pFont;

    std::string m_string = "";

    Color m_color;
};