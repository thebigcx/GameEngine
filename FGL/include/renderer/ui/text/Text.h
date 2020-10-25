#pragma once

class Text
{
public:
    Text() {}

    void setFont(const TrueTypeFont& font)
    {
        m_pFont = &font;
    }

    void setString(const std::string& string)
    {
        m_string = string;
    }

    inline const TrueTypeFont* getFont() const { return m_pFont; }
    inline const std::string& getString() const { return m_string; }

private:
    const TrueTypeFont* m_pFont;

    std::string m_string;
};