#pragma once

#include <iterator>
#include <memory>
#include <vector>

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

        bool operator==(const ConstIterator& other) const;

        ConstIterator& operator++();

        const T& operator*() const;

    private:
        typename std::vector<Ptr>::const_iterator _it;
    };

public:
    List() = default;
    List(std::vector<Ptr>&& expressions);

    std::size_t size() const;

    const T& operator[](std::size_t index) const;

    ConstIterator begin() const;
    ConstIterator end() const;

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
List<T>::List(std::vector<Ptr>&& expressions)
    : _pointers(std::move(expressions))
{
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
typename List<T>::ConstIterator List<T>::begin() const
{
    return ConstIterator{_pointers.begin()};
}

template<typename T>
typename List<T>::ConstIterator List<T>::end() const
{
    return ConstIterator{_pointers.begin()};
}

} // namespace CPPS::CST
