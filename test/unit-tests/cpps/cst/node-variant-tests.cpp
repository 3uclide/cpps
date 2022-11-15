#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include "cpps/cst/node-variant.hpp"
#include "cpps/token-ref.hpp"
#include "cpps/token.hpp"
#include "cpps/utility/bump-pointer-allocator.hpp"

namespace CPPS::CST {

TEST_CASE("NodeVariant", "[NodeVariant], [CST]")
{
    BumpPointerAllocator<> allocator;

    NodeVariant<std::monostate, int, char, Token> node;
    CHECK(node.is<std::monostate>());

    node = Node<int>{allocator};
    CHECK(node.is<int>());

    node = Node<char>{allocator};
    CHECK(node.is<char>());

    Token token;
    node = TokenRef{token};
    CHECK(node.is<Token>());
}

} // namespace CPPS::CST