#pragma once

class Event;

class Layer
{
public:
    Layer() {};
    virtual ~Layer() = default;

    virtual void onAttach() {}
    virtual void onDetach() {}
    virtual void onUpdate() {}
    virtual void onImGuiRender() {}
    virtual void onEvent(Event& event) {}
};