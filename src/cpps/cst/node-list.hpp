#pragma once

#include <cassert>
#include <iterator>
#include <memory>
#include <utility>
#include <vector>

#include "cpps/cst/node.hpp"
#include "cpps/source-location.hpp"

namespace CPPS::CST {

template<typename T>
class NodeList
{
public:
    class ConstIterator
    {
    public:
        explicit ConstIterator(typename std::vector<Node<T>>::const_iterator it);

        [[nodiscard]] bool operator==(const ConstIterator& other) const;
        [[nodiscard]] ConstIterator& operator++();
        [[nodiscard]] const T& operator*() const;

    private:
        typename std::vector<Node<T>>::const_iterator _it;
    };

    class Iterator
    {
    public:
        explicit Iterator(typename std::vector<Node<T>>::iterator it);

        [[nodiscard]] bool operator==(const Iterator& other) const;
        [[nodiscard]] Iterator& operator++();
        [[nodiscard]] T& operator*();

    private:
        typename std::vector<Node<T>>::iterator _it;
    };

public:
    NodeList() = default;

    explicit NodeList(std::vector<Node<T>>&& nodes);

    void add(Node<T>&& node);

    [[nodiscard]] std::size_t size() const;

    [[nodiscard]] const T& operator[](std::size_t index) const;
    [[nodiscard]] T& operator[](std::size_t index);

    [[nodiscard]] ConstIterator begin() const;
    [[nodiscard]] ConstIterator end() const;

    [[nodiscard]] Iterator begin();
    [[nodiscard]] Iterator end();

    [[nodiscard]] SourceLocation getLocation() const;

private:
    std::vector<Node<T>> _nodes;
};

template<typename T>
NodeList<T>::ConstIterator::ConstIterator(typename std::vector<Node<T>>::const_iterator it)
    : _it(it)
{
}

template<typename T>
bool NodeList<T>::ConstIterator::operator==(const ConstIterator& other) const
{
    return _it == other._it;
}

template<typename T>
typename NodeList<T>::ConstIterator& NodeList<T>::ConstIterator::operator++()
{
    ++_it;
    return *this;
}

template<typename T>
const T& NodeList<T>::ConstIterator::operator*() const
{
    return **_it;
}

template<typename T>
NodeList<T>::Iterator::Iterator(typename std::vector<Node<T>>::iterator it)
    : _it(it)
{
}

template<typename T>
bool NodeList<T>::Iterator::operator==(const Iterator& other) const
{
    return _it == other._it;
}

template<typename T>
typename NodeList<T>::Iterator& NodeList<T>::Iterator::operator++()
{
    ++_it;
    return *this;
}

template<typename T>
T& NodeList<T>::Iterator::operator*()
{
    return **_it;
}

template<typename T>
NodeList<T>::NodeList(std::vector<Node<T>>&& nodes)
    : _nodes(std::move(nodes))
{
}

template<typename T>
void NodeList<T>::add(Node<T>&& node)
{
    assert(node);
    _nodes.emplace_back(std::move(node));
}

template<typename T>
std::size_t NodeList<T>::size() const
{
    return _nodes.size();
}

template<typename T>
const T& NodeList<T>::operator[](std::size_t index) const
{
    assert(index < _nodes.size());
    return *_nodes[index];
}

template<typename T>
T& NodeList<T>::operator[](std::size_t index)
{
    assert(index < _nodes.size());
    return *_nodes[index];
}

template<typename T>
typename NodeList<T>::ConstIterator NodeList<T>::begin() const
{
    return ConstIterator{_nodes.begin()};
}

template<typename T>
typename NodeList<T>::ConstIterator NodeList<T>::end() const
{
    return ConstIterator{_nodes.begin()};
}

template<typename T>
typename NodeList<T>::Iterator NodeList<T>::begin()
{
    return Iterator{_nodes.begin()};
}

template<typename T>
typename NodeList<T>::Iterator NodeList<T>::end()
{
    return Iterator{_nodes.begin()};
}

template<typename T>
SourceLocation NodeList<T>::getLocation() const
{
    assert(!_nodes.empty());
    return _nodes[0]->getLocation();
}

} // namespace CPPS::CST
