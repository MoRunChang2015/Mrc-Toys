#ifndef MEMORY_POOL_TCC

template <typename T, size_t BlockSize>
MemoryPool::MemoryPool() noexcept : current_block(nullptr),
                                    current_slot(nullptr),
                                    last_slot(nullptr),
                                    free_slot(nullptr) {}

template <typename T, size_t BlockSize>
MemoryPool::MemoryPool(const MemoryPool&) noexcept : MemoryPool() {}

template <typename T, size_t BlockSize>
MemoryPool::MemoryPool(MemoryPool&& memoryPool_t) noexcept
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
MemoryPool::MemoryPool(const MemoryPool<U>&) noexcept : MemoryPool() {}

template <typename T, size_t BlockSize>
MemoryPool::~MemoryPool() noexcept {
    auto cur = current_block;
    while (cur != nullptr) {
        auto prev = cur->next;
        operator delete(reinterpret_cast<void*>(cur));
        cur = prev;
    }
}

template <typename T, size_t BlockSize>
MemoryPool& MemoryPool::operator=(MemoryPool&& memoryPool_t) noexcept {
    if (this != &MemoryPool) {
        std::swap(current_block, memoryPool_t.current_block);
        current_slot = memoryPool_t.current_slot;
        last_slot = memoryPool_t.current_slot;
        free_slot = memoryPool_t.free_slot;
    }
    return *this;
}

template <typename T, size_t BlockSize>
inline typename MemoryPool<T, BlockSize>::pointer MemoryPool::address(
    reference x) const noexcept {
    return &x;
}

template <typename T, size_t BlockSize>
inline typename MemoryPool<T, BlockSize>::const_pointer MemoryPool::address(
    const_reference x) const noexcept {
    return &x;
}

size_t calculate_padding(void* ptr, size_t align) {
    auto result = reinterpret_cast<uintptr_t>(ptr);
    return (align - result) % align;
}

template <typename T, size_t BlockSize>
void allocateBlock() {
    /**
     * Block structure:
     * current_block
     * |   current_slot
     * |   |                                  last_slot
     * |   |                                  |
     * +--------------------------------------+
     * | | | slot | slot | slot | slot | slot |
     * +--------------------------------------+
     *  ^ ^
     *  | |
     *  | padding
     *  slot_pointer to previous block
     *
     *  free_slot -> +-----------+
     *               | free slot | -> +-----------+
     *               +-----------+    | free slot | -> nullptr
     *                                +-----------+
     */
    data_pointer newBlock =
        reinterpret_cast<data_pointer>(operator new(BlockSize));
    reinterpret_cast<slot_pointer>(newBlock)->next = current_block;
    current_block = reinterpret_cast<slot_pointer>(newBlock);

    auto padding =
        calculate_padding(newBlock + sizeof(slot_pointer), alignof(slot_type));
    current_slot = reinterpret_cast<slot_pointer>(
        newBlock + sizeof(slot_pointer) + padding);
    last_slot = reinterpret_cast<slot_pointer>(newBlock + BlockSize);
}

template <typename T, size_t BlockSize>
typename MemoryPool<T, BlockSize>::pointer MemoryPool::allocate(
    size_t n, const_pointer hint) {}
#endif
