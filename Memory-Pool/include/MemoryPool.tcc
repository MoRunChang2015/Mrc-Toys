#ifndef MEMORY_POOL_TCC
#include <limits>

template <typename T, size_t BlockSize>
MemoryPool<T, BlockSize>::MemoryPool() noexcept : current_block(nullptr),
                                                  current_slot(nullptr),
                                                  last_slot(nullptr),
                                                  free_slot(nullptr) {}

template <typename T, size_t BlockSize>
MemoryPool<T, BlockSize>::MemoryPool(const MemoryPool&) noexcept
    : MemoryPool() {}

template <typename T, size_t BlockSize>
MemoryPool<T, BlockSize>::MemoryPool(MemoryPool&& memoryPool_t) noexcept
    : current_block(memoryPool_t.current_block),
      current_slot(memoryPool_t.current_slot),
      last_slot(memoryPool_t.last_slot),
      free_slot(memoryPool_t.free_slot) {
    memoryPool_t.current_block = nullptr;
    memoryPool_t.current_slot = nullptr;
    memoryPool_t.last_slot = nullptr;
    memoryPool_t.free_slot = nullptr;
}

template <typename T, size_t BlockSize>
template <typename U>
MemoryPool<T, BlockSize>::MemoryPool(const MemoryPool<U>&) noexcept
    : MemoryPool() {}

template <typename T, size_t BlockSize>
MemoryPool<T, BlockSize>::~MemoryPool() noexcept {
    auto cur = current_block;
    while (cur != nullptr) {
        auto prev = cur->next;
        operator delete(reinterpret_cast<void*>(cur));
        cur = prev;
    }
}

template <typename T, size_t BlockSize>
MemoryPool<T, BlockSize>& MemoryPool<T, BlockSize>::operator=(
    MemoryPool&& memoryPool_t) noexcept {
    if (this != &memoryPool_t) {
        std::swap(current_block, memoryPool_t.current_block);
        current_slot = memoryPool_t.current_slot;
        last_slot = memoryPool_t.current_slot;
        free_slot = memoryPool_t.free_slot;
    }
    return *this;
}

template <typename T, size_t BlockSize>
inline typename MemoryPool<T, BlockSize>::pointer
MemoryPool<T, BlockSize>::address(reference x) const noexcept {
    return &x;
}

template <typename T, size_t BlockSize>
inline typename MemoryPool<T, BlockSize>::const_pointer
MemoryPool<T, BlockSize>::address(const_reference x) const noexcept {
    return &x;
}

size_t calculate_padding(void* ptr, size_t align) {
    auto result = reinterpret_cast<uintptr_t>(ptr);
    return (align - result) % align;
}

template <typename T, size_t BlockSize>
void MemoryPool<T, BlockSize>::allocateBlock() {
    /**
     * Block structure:
     * current_block
     * |   current_slot                     last_slot
     * |   |                                |
     * |   |                                |<slot>|
     * +-------------------------------------------+
     * | | | slot | slot | slot | slot | slot |    |
     * +-------------------------------------------+
     *  ^ ^
     *  | |
     *  | padding
     *  slot_pointer to previous block
     *
     *               +-----------+    +-----------+
     *  free_slot -> | free slot | -> | free slot | -> NULL
     *               +-----------+    +-----------+
     */
    data_pointer newBlock =
        reinterpret_cast<data_pointer>(operator new(BlockSize));
    reinterpret_cast<slot_pointer>(newBlock)->next = current_block;
    current_block = reinterpret_cast<slot_pointer>(newBlock);

    auto padding =
        calculate_padding(newBlock + sizeof(slot_pointer), alignof(slot_type));
    current_slot = reinterpret_cast<slot_pointer>(
        newBlock + sizeof(slot_pointer) + padding);
    last_slot = reinterpret_cast<slot_pointer>(newBlock + BlockSize -
                                               sizeof(slot_type) + 1);
}

template <typename T, size_t BlockSize>
typename MemoryPool<T, BlockSize>::pointer MemoryPool<T, BlockSize>::allocate(
    size_t n, const_pointer hint) {
    if (free_slot != nullptr) {
        auto new_object = reinterpret_cast<pointer>(free_slot);
        free_slot = free_slot->next;
        return new_object;
    } else {
        if (current_slot >= last_slot) allocateBlock();
        return reinterpret_cast<pointer>(current_slot++);
    }
}

template <typename T, size_t BlockSize>
void MemoryPool<T, BlockSize>::deallocate(pointer p, size_type n) {
    if (p != nullptr) {
        reinterpret_cast<slot_pointer>(p)->next = free_slot;
        free_slot = reinterpret_cast<slot_pointer>(p);
    }
}

template <typename T, size_t BlockSize>
typename MemoryPool<T, BlockSize>::size_type
MemoryPool<T, BlockSize>::max_size() const noexcept {
    size_type maxBlocks = (std::numeric_limits<size_t>::max)() / BlockSize;
    return (BlockSize - sizeof(slot_pointer)) / sizeof(slot_type) * maxBlocks;
}

template <typename T, size_t BlockSize>
template <typename U, typename... Args>
void MemoryPool<T, BlockSize>::construct(U* p, Args&&... args) {
    new (p) U(std::forward<Args>(args)...);
}

template <typename T, size_t BlockSize>
template <typename U>
void MemoryPool<T, BlockSize>::destroy(U* p) {
    p->~U();
}

template <typename T, size_t BlockSize>
template <typename... Args>
typename MemoryPool<T, BlockSize>::pointer MemoryPool<T, BlockSize>::newElement(
    Args... args) {
    auto new_object = allocate();
    construct(new_object, std::forward<Args>(args)...);
    return new_object;
}

template <typename T, size_t BlockSize>
void MemoryPool<T, BlockSize>::deleteElement(pointer p) {
    if (p != nullptr) {
        destroy(p);
        deallocate(p);
    }
}
#endif
