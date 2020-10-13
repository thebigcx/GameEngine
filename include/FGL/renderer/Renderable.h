#pragma once

class Renderable
{
public:
    virtual ~Renderable() = default;

    virtual int getVertexCount();
};