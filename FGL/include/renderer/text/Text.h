#pragma once

class Text
{
public:
    Text() 
        : m_characterSize(48)
    {

    }

    void setFont(const TrueTypeFont& font)
    {
        m_pFont = &font;
    }

    void setString(const std::string& string)
    {
        m_string = string;
    }

    void setColor(const Color& color)
    {
        m_color = color;
    }

    void setCharSize(const Vector2f& size)
    {
        m_characterSize = size;
    }

    inline const TrueTypeFont* getFont() const { return m_pFont; }
    inline const std::string& getString() const { return m_string; }
    inline const Color& getColor() const { return m_color; }
    inline const Vector2f& getCharSize() const { return m_characterSize; }

private:
    const TrueTypeFont* m_pFont;

    std::string m_string;
    Color m_color;
    Vector2f m_characterSize;
};