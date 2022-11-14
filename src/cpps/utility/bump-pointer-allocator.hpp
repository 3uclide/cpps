#pragma once

#include <array>
#include <cassert>
#include <vector>

namespace CPPS {

/**
 * ref https://fitzgeraldnick.com/2019/11/01/always-bump-downwards.html
 */
template<std::size_t BlockCapacityT = 1024>
requires(BlockCapacityT > 0)
class BumpPointerAllocator
{
public:
    BumpPointerAllocator() = default;

    explicit BumpPointerAllocator(std::size_t startBlockCount);

    template<typename T>
    requires(sizeof(T) <= BlockCapacityT)
    [[nodiscard]] T* allocate();

    template<typename T>
    void deallocate(const T* ptr);

    [[nodiscard]] void* allocate(std::size_t size, std::size_t align);
    void deallocate(const void* ptr, std::size_t, std::size_t align);

    [[nodiscard]] std::size_t getBlockCapacity() const;
    [[nodiscard]] std::size_t getBlockCount() const;

    [[nodiscard]] std::size_t getAllocatedBytesCount() const;
    [[nodiscard]] std::size_t getUsedBytesCount() const;

private:
    static bool isValid(std::size_t size, std::size_t align);

private:
    using Block = std::array<std::byte, BlockCapacityT>;
    using Blocks = std::vector<Block>;

private:
    Blocks _blocks{Block{std::byte{}}};
    std::byte* _ptr{_blocks.back().data() + BlockCapacityT};
};

template<std::size_t BlockCapacityT>
requires(BlockCapacityT > 0)
BumpPointerAllocator<BlockCapacityT>::BumpPointerAllocator(std::size_t startBlockCount)
{
    assert(startBlockCount > 0);
    _blocks.reserve(startBlockCount);
}

template<std::size_t BlockCapacityT>
requires(BlockCapacityT > 0)
template<typename T>
requires(sizeof(T) <= BlockCapacityT)
T* BumpPointerAllocator<BlockCapacityT>::allocate()
{
    return static_cast<T*>(allocate(sizeof(T), alignof(T)));
}

template<std::size_t BlockCapacityT>
requires(BlockCapacityT > 0)
template<typename T>
void BumpPointerAllocator<BlockCapacityT>::deallocate(const T* ptr)
{
    deallocate(ptr, sizeof(T), alignof(T));
}

template<std::size_t BlockCapacityT>
requires(BlockCapacityT > 0)
void* BumpPointerAllocator<BlockCapacityT>::allocate(std::size_t size, std::size_t align)
{
    assert(isValid(size, align));

    auto getNextAligned = [align, size](std::byte* ptr) -> std::byte* {
        const std::uintptr_t intPtr = reinterpret_cast<std::uintptr_t>(ptr);
        const std::uintptr_t newIntPtr = intPtr - size;
        const std::uintptr_t align_ = ~(align - 1);
        const std::uintptr_t alignedIntPtr = newIntPtr & align_;
        return static_cast<std::byte*>(reinterpret_cast<void*>(alignedIntPtr)); // NOLINT(performance-no-int-to-ptr)
    };

    std::byte* data = _blocks.back().data();

    std::byte* newPtr = getNextAligned(_ptr);

    if (newPtr < data)
    {
        _blocks.emplace_back(Block{std::byte{}});

        data = _blocks.back().data();

        newPtr = getNextAligned(data + BlockCapacityT);
    }

    _ptr = newPtr;

    return _ptr;
}

template<std::size_t BlockCapacityT>
requires(BlockCapacityT > 0)
void BumpPointerAllocator<BlockCapacityT>::deallocate(const void* ptr, std::size_t size, std::size_t align)
{
    assert(isValid(size, align));

    if (ptr == _ptr)
    {
        const std::uintptr_t intPtr = reinterpret_cast<std::uintptr_t>(_ptr);
        const std::uintptr_t newIntPtr = intPtr + size;
        const std::uintptr_t align_ = ~(align - 1);
        const std::uintptr_t alignedIntPtr = newIntPtr & align_;

        _ptr = static_cast<std::byte*>(reinterpret_cast<void*>(alignedIntPtr)); // NOLINT(performance-no-int-to-ptr)
    }
}

template<std::size_t BlockCapacityT>
requires(BlockCapacityT > 0)
[[nodiscard]] std::size_t BumpPointerAllocator<BlockCapacityT>::getBlockCapacity() const
{
    return BlockCapacityT;
}

template<std::size_t BlockCapacityT>
requires(BlockCapacityT > 0)
[[nodiscard]] std::size_t BumpPointerAllocator<BlockCapacityT>::getBlockCount() const
{
    return _blocks.size();
}

template<std::size_t BlockCapacityT>
requires(BlockCapacityT > 0)
[[nodiscard]] std::size_t BumpPointerAllocator<BlockCapacityT>::getAllocatedBytesCount() const
{
    return _blocks.size() * BlockCapacityT;
}

template<std::size_t BlockCapacityT>
requires(BlockCapacityT > 0)
[[nodiscard]] std::size_t BumpPointerAllocator<BlockCapacityT>::getUsedBytesCount() const
{
    assert(_ptr >= _blocks.back().data() && _ptr < _blocks.back().data() + BlockCapacityT);

    const std::size_t filledBlockBytesCount = (_blocks.size() - 1) * BlockCapacityT;
    const std::size_t remainingBlockBytesCount = reinterpret_cast<std::uintptr_t>(_blocks.back().data()) + BlockCapacityT - reinterpret_cast<std::uintptr_t>(_ptr);

    return filledBlockBytesCount + remainingBlockBytesCount;
}

template<std::size_t BlockCapacityT>
requires(BlockCapacityT > 0) bool
BumpPointerAllocator<BlockCapacityT>::isValid(std::size_t size, std::size_t align)
{
    return size <= BlockCapacityT && align > 0 && (align & (align - 1)) == 0;
}

} // namespace CPPS
