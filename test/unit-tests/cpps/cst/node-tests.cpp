#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include "cpps/cst/node.hpp"
#include "cpps/utility/bump-pointer-allocator.hpp"

namespace CPPS::CST {

bool ctorCalled{false};
bool dtorCalled{false};

struct NodeMock
{
    NodeMock()
    {
        ctorCalled = true;
    }

    ~NodeMock()
    {
        dtorCalled = true;
    }

    int value{42};
};

struct NodeFixture
{
    NodeFixture()
    {
        ctorCalled = false;
        dtorCalled = false;
    }

    Node<NodeMock> _node;
    BumpPointerAllocator<> _allocator;
};

TEST_CASE_METHOD(NodeFixture, "Node create", "[Node], [CST]")
{
    Node<NodeMock> node;

    CHECK_FALSE(node);
    CHECK_FALSE(ctorCalled);
    CHECK_FALSE(dtorCalled);

    node.create(_allocator);

    CHECK(node);
    CHECK(ctorCalled);
    CHECK_FALSE(dtorCalled);
}

TEST_CASE_METHOD(NodeFixture, "Node destroy", "[Node], [CST]")
{
    _node.create(_allocator);
    _node.destroy(_allocator);

    CHECK_FALSE(_node);
    CHECK(dtorCalled);
}

TEST_CASE_METHOD(NodeFixture, "Node dtor", "[Node], [CST]")
{
    {
        Node<NodeMock> node;

        node.create(_allocator);
    }

    CHECK(dtorCalled);
}

TEST_CASE_METHOD(NodeFixture, "Node accessor", "[Node], [CST]")
{
    _node.create(_allocator);

    SECTION("Read")
    {
        SECTION("operator*")
        {
            CHECK((*std::as_const(_node)).value == 42);
        }

        SECTION("operator->")
        {
            CHECK(std::as_const(_node)->value == 42);
        }
    }

    SECTION("Write")
    {
        SECTION("operator*")
        {
            (*_node).value = 24;

            CHECK((*_node).value == 24);
        }

        SECTION("operator->")
        {
            _node->value = 24;

            CHECK(_node->value == 24);
        }
    }
}

} // namespace CPPS::CST