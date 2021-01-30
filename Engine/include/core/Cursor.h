#pragma once

struct GLFWcursor;

namespace Engine
{

class Cursor
{
public:
    enum NativeCursor
    {
        Arrow,
        IBeam,
        Crosshair,
        Hand,
        HorizontalResize,
        VerticalResize
    };

public:
    Cursor();
    ~Cursor();

    void setCurrent(bool current = true);

    void createNativeCursor(NativeCursor cursor);

private:
    GLFWcursor* m_cursor;
};

}