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
    SECTION("ctor is called")
    {
        CHECK_FALSE(_node);
        CHECK_FALSE(ctorCalled);
        CHECK_FALSE(dtorCalled);

        _node.create(_allocator);

        CHECK(_node);
        CHECK(ctorCalled);
        CHECK_FALSE(dtorCalled);
    }

    SECTION("perfect forwarding")
    {
        struct RefOwner
        {
            explicit RefOwner(const NodeMock& node)
                : _node(node)
            {
            }

            std::reference_wrapper<const NodeMock> _node;
        };

        NodeMock mock;
        Node<RefOwner> node;
        node.create(_allocator, mock);

        CHECK(node);
        CHECK(&node->_node.get() == &mock);
    }
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

TEST_CASE_METHOD(NodeFixture, "Node move", "[Node], [CST]")
{
    Node<NodeMock> node;
    Node<NodeMock> newNode;

    newNode.create(_allocator);

    node = std::move(newNode);

    CHECK(node);
    CHECK_FALSE(newNode);
}

TEST_CASE_METHOD(NodeFixture, "Node accessor", "[Node], [CST]")
{
    _node.create(_allocator);

    SECTION("Read")
    {
        const Node<NodeMock>& node = _node;

        SECTION("operator*")
        {
            CHECK((*node).value == 42);
        }

        SECTION("operator->")
        {
            CHECK(node->value == 42);
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