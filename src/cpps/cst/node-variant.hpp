#pragma once

#include <utility>
#include <variant>

#include "cpps/cst/node-list.hpp"
#include "cpps/cst/node.hpp"
#include "cpps/token-ref.hpp"
#include "cpps/utility/type-list.hpp"

namespace CPPS {
class Token;
} // namespace CPPS

namespace CPPS::CST {

template<typename T>
struct NodeType;

template<typename... TypesT>
class NodeVariant
{
public:
    template<typename T>
    using NodeType = typename NodeType<T>::Type;

public:
    NodeVariant() = default;
    NodeVariant(NodeVariant&& other) = default;
    NodeVariant& operator=(NodeVariant&& other) = default;

    NodeVariant(const NodeVariant& other) = delete;
    NodeVariant& operator=(const NodeVariant& other) = delete;

    template<typename T>
    requires(TypesContainsV<T, TypesT...>)
    NodeVariant(NodeType<T>&& value); // cppcheck-suppress noExplicitConstructor

    template<typename T>
    requires(TypesContainsV<T, TypesT...>)
    NodeVariant& operator=(Node<T>&& value);
    NodeVariant& operator=(const Token& value) requires(TypesContainsV<Token, TypesT...>);

    template<typename T>
    [[nodiscard]] bool is() const;

    template<typename T>
    [[nodiscard]] const T& as() const;

    template<typename T>
    [[nodiscard]] T& as();

private:
    std::variant<NodeType<TypesT>...> _variant;
};

template<typename T>
struct NodeType
{
    using Type = Node<T>;
};

template<>
struct NodeType<std::monostate>
{
    using Type = std::monostate;
};

template<>
struct NodeType<Token>
{
    using Type = TokenRef;
};

template<typename... TypesT>
template<typename T>
requires(TypesContainsV<T, TypesT...>)
NodeVariant<TypesT...>::NodeVariant(NodeType<T>&& value)
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
NodeVariant<TypesT...>& NodeVariant<TypesT...>::operator=(const Token& value) requires(TypesContainsV<Token, TypesT...>)
{
    _variant = std::ref(value);
    return *this;
}

template<typename... TypesT>
template<typename T>
[[nodiscard]] bool NodeVariant<TypesT...>::is() const
{
    return std::holds_alternative<NodeType<T>>(_variant);
}

template<typename... TypesT>
template<typename T>
[[nodiscard]] const T& NodeVariant<TypesT...>::as() const
{
    return std::get<NodeType<T>>(_variant).get();
}

template<typename... TypesT>
template<typename T>
[[nodiscard]] T& NodeVariant<TypesT...>::as()
{
    return std::get<NodeType<T>>(_variant).get();
}

} // namespace CPPS::CST
