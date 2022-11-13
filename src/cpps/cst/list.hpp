#pragma once

#include <cassert>
#include <iterator>
#include <memory>
#include <utility>
#include <vector>

#include "cpps/source-location.hpp"

namespace CPPS::CST {

template<typename T>
class List
{
public:
    using Ptr = std::unique_ptr<T>;

    class ConstIterator
    {
    public:
        explicit ConstIterator(typename std::vector<Ptr>::const_iterator it);

        [[nodiscard]] bool operator==(const ConstIterator& other) const;
        [[nodiscard]] ConstIterator& operator++();
        [[nodiscard]] const T& operator*() const;

    private:
        typename std::vector<Ptr>::const_iterator _it;
    };

    class Iterator
    {
    public:
        explicit Iterator(typename std::vector<Ptr>::iterator it);

        [[nodiscard]] bool operator==(const Iterator& other) const;
        [[nodiscard]] Iterator& operator++();
        [[nodiscard]] T& operator*();

    private:
        typename std::vector<Ptr>::iterator _it;
    };

public:
    List() = default;

    explicit List(std::vector<Ptr>&& expressions);

    void add(Ptr&& ptr);

    [[nodiscard]] std::size_t size() const;

    [[nodiscard]] const T& operator[](std::size_t index) const;
    [[nodiscard]] T& operator[](std::size_t index);

    [[nodiscard]] ConstIterator begin() const;
    [[nodiscard]] ConstIterator end() const;

    [[nodiscard]] Iterator begin();
    [[nodiscard]] Iterator end();

    [[nodiscard]] SourceLocation getLocation() const;

private:
    std::vector<Ptr> _pointers;
};

template<typename T>
List<T>::ConstIterator::ConstIterator(typename std::vector<Ptr>::const_iterator it)
    : _it(it)
{
}

template<typename T>
bool List<T>::ConstIterator::operator==(const ConstIterator& other) const
{
    return _it == other._it;
}

template<typename T>
typename List<T>::ConstIterator& List<T>::ConstIterator::operator++()
{
    ++_it;
    return *this;
}

template<typename T>
const T& List<T>::ConstIterator::operator*() const
{
    return **_it;
}

template<typename T>
List<T>::Iterator::Iterator(typename std::vector<Ptr>::iterator it)
    : _it(it)
{
}

template<typename T>
bool List<T>::Iterator::operator==(const Iterator& other) const
{
    return _it == other._it;
}

template<typename T>
typename List<T>::Iterator& List<T>::Iterator::operator++()
{
    ++_it;
    return *this;
}

template<typename T>
T& List<T>::Iterator::operator*()
{
    return **_it;
}

template<typename T>
List<T>::List(std::vector<Ptr>&& expressions)
    : _pointers(std::move(expressions))
{
}

template<typename T>
void List<T>::add(Ptr&& ptr)
{
    assert(ptr);
    _pointers.emplace_back(std::move(ptr));
}

template<typename T>
std::size_t List<T>::size() const
{
    return _pointers.size();
}

template<typename T>
const T& List<T>::operator[](std::size_t index) const
{
    assert(index < _pointers.size());
    return *_pointers[index];
}

template<typename T>
T& List<T>::operator[](std::size_t index)
{
    assert(index < _pointers.size());
    return *_pointers[index];
}

template<typename T>
typename List<T>::ConstIterator List<T>::begin() const
{
    return ConstIterator{_pointers.begin()};
}

template<typename T>
typename List<T>::ConstIterator List<T>::end() const
{
    return ConstIterator{_pointers.begin()};
}

template<typename T>
typename List<T>::Iterator List<T>::begin()
{
    return Iterator{_pointers.begin()};
}

template<typename T>
typename List<T>::Iterator List<T>::end()
{
    return Iterator{_pointers.begin()};
}

template<typename T>
SourceLocation List<T>::getLocation() const
{
    assert(!_pointers.empty());
    return _pointers[0]->getLocation();
}

} // namespace CPPS::CST
