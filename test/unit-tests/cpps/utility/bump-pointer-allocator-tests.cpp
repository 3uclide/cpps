#include <catch2/catch_test_macros.hpp>
#include <cstdint>

#include "cpps/utility/bump-pointer-allocator.hpp"

namespace CPPS {

struct Object
{
    int value0{0};
    int value1{1};
    int value2{2};
    int value3{3};
};

TEST_CASE("BumpPointerAllocator allocate does not construct", "[BumpPointerAllocator], [Allocator]")
{
    static bool ctorCalled{false};

    struct ObjectCtor
    {
        ObjectCtor()
        {
            ctorCalled = true;
        }
    };

    BumpPointerAllocator<32> allocator;

    ObjectCtor* object = allocator.allocate<ObjectCtor>();

    CHECK(object);

    CHECK_FALSE(ctorCalled);
}

TEST_CASE("BumpPointerAllocator deallocate does not destruct", "[BumpPointerAllocator], [Allocator]")
{
    static bool dtorCalled{false};

    struct ObjectDtor
    {
        ~ObjectDtor()
        {
            dtorCalled = true;
        }
    };

    BumpPointerAllocator<32> allocator;

    ObjectDtor* object = allocator.allocate<ObjectDtor>();

    CHECK(object);

    allocator.deallocate(object);

    CHECK_FALSE(dtorCalled);
}

TEST_CASE("BumpPointerAllocator deallocate recycle memory if ptr was last allocation", "[BumpPointerAllocator], [Allocator]")
{
    BumpPointerAllocator<32> allocator;

    Object* object = allocator.allocate<Object>();

    allocator.deallocate(object);

    Object* newObject = allocator.allocate<Object>();

    CHECK(object == newObject);
}

TEST_CASE("BumpPointerAllocator memory usage", "[BumpPointerAllocator], [Allocator]")
{
    constexpr std::size_t BlockCapacity = sizeof(int) * 2;

    BumpPointerAllocator<BlockCapacity> allocator;

    int* obj0 = allocator.allocate<int>();
    *obj0 = std::numeric_limits<int>::max();

    CHECK(allocator.getBlockCount() == 1);
    CHECK(allocator.getAllocatedBytesCount() == BlockCapacity);
    CHECK(allocator.getUsedBytesCount() == sizeof(int));

    int* obj1 = allocator.allocate<int>();
    *obj1 = std::numeric_limits<int>::max();

    CHECK(allocator.getBlockCount() == 1);
    CHECK(allocator.getAllocatedBytesCount() == BlockCapacity);
    CHECK(allocator.getUsedBytesCount() == BlockCapacity);

    int* obj2 = allocator.allocate<int>();
    *obj2 = std::numeric_limits<int>::max();

    CHECK(allocator.getBlockCount() == 2);
    CHECK(allocator.getAllocatedBytesCount() == BlockCapacity * 2);
    CHECK(allocator.getUsedBytesCount() == BlockCapacity + sizeof(int));
}

} // namespace CPPS
