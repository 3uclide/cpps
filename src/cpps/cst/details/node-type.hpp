#pragma once

#include <variant>

#include "cpps/token-ref.hpp"

namespace CPPS {

class Token;

namespace CST {

template<typename T>
class Node;

namespace Details {

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

} // namespace Details
} // namespace CST
} // namespace CPPS
