#pragma once

namespace CPPS::CST {

/*
 * The Node class is responsible to call the ctor and dtor of T.
 */
template<typename T>
class Node
{
public:
    Node() = default;
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;

public:
    explicit Node(auto& allocator);
    Node(Node&& other);
    Node& operator=(Node&& other);
    ~Node();

    operator bool() const;

    const T& operator*() const;
    T& operator*();

    const T* operator->() const;
    T* operator->();

public:
    void create(auto& allocator);
    void destroy(auto& allocator);

private:
    void ctor();
    void dtor();

private:
    T* _node{nullptr};
};

template<typename T>
Node<T>::Node(auto& allocator)
{
    create(allocator);
}

template<typename T>
Node<T>::Node(Node&& other)
    : _node(other._node)
{
    other._node = nullptr;
}

template<typename T>
Node<T>& Node<T>::operator=(Node&& other)
{
    if (this != &other)
    {
        if (_node != nullptr)
        {
            dtor();
        }

        _node = other._node;

        other._node = nullptr;
    }

    return *this;
}

template<typename T>
Node<T>::~Node()
{
    if (_node != nullptr)
    {
        dtor();
    }
}

template<typename T>
Node<T>::operator bool() const
{
    return _node != nullptr;
}

template<typename T>
const T& Node<T>::operator*() const
{
    assert(_node != nullptr);
    return *_node;
}

template<typename T>
T& Node<T>::operator*()
{
    assert(_node != nullptr);
    return *_node;
}

template<typename T>
const T* Node<T>::operator->() const
{
    assert(_node != nullptr);
    return _node;
}

template<typename T>
T* Node<T>::operator->()
{
    assert(_node != nullptr);
    return _node;
}

template<typename T>
void Node<T>::create(auto& allocator)
{
    assert(_node == nullptr);

    _node = allocator.template allocate<T>();

    ctor();
}

template<typename T>
void Node<T>::destroy(auto& allocator)
{
    assert(_node != nullptr);

    dtor();

    allocator.template deallocate<T>(_node);

    _node = nullptr;
}

template<typename T>
void Node<T>::ctor()
{
    new (_node) T();
}

template<typename T>
void Node<T>::dtor()
{
    _node->~T();
}

} // namespace CPPS::CST
