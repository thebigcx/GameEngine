#include <scene/GameObject.h>
#include <util/Transform.h>

namespace Engine
{

GameObject::GameObject(GameObject* parent)
    : m_parent(parent)
{
    m_components.clear();
}

GameObject::~GameObject()
{

}

GameObject* GameObject::getParent() const
{
    return m_parent;
}

void GameObject::onTransformChange(const Transform& transform)
{
    for (auto& child : m_children)
    {
        child->onTransformChange(transform);
    }
    
    for (auto& component : m_components)
    {
        if (component.first != typeid(Transform))
        {
            component.second->onTransformChange(transform);
        }
    }

    if (getComponent<Transform>() != &transform)
        getComponent<Transform>()->onTransformChange(transform);
}

void GameObject::removeChild(GameObject* child)
{
    child->destroy_();

    auto it = std::find(m_children.begin(), m_children.end(), child);
    if (it != m_children.end())
        m_children.erase(it);
}

GameObject* GameObject::createChild()
{
    auto object = new GameObject(this);
    object->createComponent<Transform>();
    m_children.push_back(object);
    return m_children.back();
}

std::vector<GameObject*> GameObject::getChildren()
{
    return m_children;
}

std::vector<GameObject*> GameObject::getChildrenRecursive()
{
    std::vector<GameObject*> children;

    std::function<void(GameObject*)> recurse;
    recurse = [&children, &recurse](GameObject* current)
    {
        for (auto child : current->getChildren())
        {
            children.push_back(child);
            recurse(child);
        }
    };

    recurse(this);

    return children;
}

void GameObject::destroy_()
{
    for (auto& component : m_components)
    {
        delete component.second;
    }
    m_components.clear();

    for (auto& child : m_children)
    {
        child->destroy_();
        delete child;
    }
    m_children.clear();
}

}