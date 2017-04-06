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

template <typename T, size_t BlockSize>
typename MemoryPool<T, BlockSize>::pointer MemoryPool::allocate(
    size_t n, const_pointer hint) {}
#endif
