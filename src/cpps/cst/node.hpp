#pragma once

#include <cassert>
#include <utility>

#include "cpps/utility/bump-pointer-allocator.hpp"

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
    template<std::size_t AllocatorBlockCapacityT, typename... ArgsT>
    explicit Node(BumpPointerAllocator<AllocatorBlockCapacityT>& allocator, ArgsT... args);

    Node(Node&& other);
    Node& operator=(Node&& other) noexcept;
    ~Node();

    [[nodiscard]] operator bool() const;

    [[nodiscard]] const T& get() const;
    [[nodiscard]] T& get();

    [[nodiscard]] const T& operator*() const;
    [[nodiscard]] T& operator*();

    [[nodiscard]] const T* operator->() const;
    [[nodiscard]] T* operator->();

public:
    template<std::size_t AllocatorBlockCapacityT, typename... ArgsT>
    void create(BumpPointerAllocator<AllocatorBlockCapacityT>& allocator, ArgsT... args);

    template<std::size_t AllocatorBlockCapacityT>
    void destroy(BumpPointerAllocator<AllocatorBlockCapacityT>& allocator); // calling destroy is optional

private:
    template<typename... ArgsT>
    static void ctor(T* node, ArgsT... args);
    static void dtor(T* node);

private:
    T* _node{nullptr};
};

template<typename T>
template<std::size_t AllocatorBlockCapacityT, typename... ArgsT>
Node<T>::Node(BumpPointerAllocator<AllocatorBlockCapacityT>& allocator, ArgsT... args)
{
    create(allocator, std::forward<ArgsT>(args)...);
}

template<typename T>
Node<T>::Node(Node&& other)
    : _node(other._node)
{
    other._node = nullptr;
}

template<typename T>
Node<T>& Node<T>::operator=(Node&& other) noexcept
{
    if (this != &other)
    {
        if (_node != nullptr)
        {
            dtor(_node);
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
        dtor(_node);
    }
}

template<typename T>
Node<T>::operator bool() const
{
    return _node != nullptr;
}

template<typename T>
const T& Node<T>::get() const
{
    assert(_node != nullptr);
    return *_node;
}

template<typename T>
T& Node<T>::get()
{
    assert(_node != nullptr);
    return *_node;
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
template<std::size_t AllocatorBlockCapacityT, typename... ArgsT>
void Node<T>::create(BumpPointerAllocator<AllocatorBlockCapacityT>& allocator, ArgsT... args)
{
    assert(_node == nullptr);

    _node = allocator.template allocate<T>();

    ctor(_node, std::forward<ArgsT>(args)...);
}

template<typename T>
template<std::size_t AllocatorBlockCapacityT>
void Node<T>::destroy(BumpPointerAllocator<AllocatorBlockCapacityT>& allocator)
{
    assert(_node != nullptr);

    dtor(_node);

    allocator.template deallocate<T>(_node);

    _node = nullptr;
}

template<typename T>
template<typename... ArgsT>
void Node<T>::ctor(T* node, ArgsT... args)
{
    new (node) T(std::forward<ArgsT>(args)...);
}

template<typename T>
void Node<T>::dtor(T* node)
{
    node->~T();
}

} // namespace CPPS::CST
