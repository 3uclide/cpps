#pragma once

#include <variant>

#include "cpps/cst/details/node-type.hpp"
#include "cpps/cst/node.hpp"
#include "cpps/token-ref.hpp"
#include "cpps/utility/type-list.hpp"

namespace CPPS {
class Token;
} // namespace CPPS

namespace CPPS::CST {

template<typename... TypesT>
class NodeVariant
{
public:
    template<typename T>
    using NodeTypeT = typename Details::NodeType<T>::Type;

public:
    NodeVariant() = default;

    template<typename T>
    requires(TypesContainsV<T, TypesT...>)
    NodeVariant(NodeTypeT<T>&& value); // cppcheck-suppress noExplicitConstructor

    template<typename T>
    requires(TypesContainsV<T, TypesT...>)
    NodeVariant& operator=(Node<T>&& value);
    NodeVariant& operator=(TokenRef&& value);

    template<typename T>
    [[nodiscard]] bool is() const;

    template<typename T>
    [[nodiscard]] const T& as() const;

    template<typename T>
    [[nodiscard]] T& as();

private:
    std::variant<NodeTypeT<TypesT>...> _variant;
};

template<typename... TypesT>
template<typename T>
requires(TypesContainsV<T, TypesT...>)
NodeVariant<TypesT...>::NodeVariant(NodeTypeT<T>&& value)
    : _variant(std::move(value))
{
}

template<typename... TypesT>
template<typename T>
requires(TypesContainsV<T, TypesT...>)
NodeVariant<TypesT...>& NodeVariant<TypesT...>::operator=(Node<T>&& value)
{
    _variant = std::move(value);
    return *this;
}

template<typename... TypesT>
NodeVariant<TypesT...>& NodeVariant<TypesT...>::operator=(TokenRef&& value)
{
    static_assert(TypesContainsV<Token, TypesT...>);
    _variant = std::move(value);
    return *this;
}

template<typename... TypesT>
template<typename T>
[[nodiscard]] bool NodeVariant<TypesT...>::is() const
{
    return std::holds_alternative<NodeTypeT<T>>(_variant);
}

template<typename... TypesT>
template<typename T>
[[nodiscard]] const T& NodeVariant<TypesT...>::as() const
{
    return std::get<NodeTypeT<T>>(_variant).get();
}

template<typename... TypesT>
template<typename T>
[[nodiscard]] T& NodeVariant<TypesT...>::as()
{
    return std::get<NodeTypeT<T>>(_variant).get();
}

} // namespace CPPS::CST
