#ifndef MEMORY_POOL_HPP
#define MEMORY_POOL_HPP

#include <climits>
#include <cstddef>

template <typename T, size_t BlockSize = 4096>
class MemoryPool {
   public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using const_pointer = const T*;
    using const_reference = const T&;
    using size_type = size_t;
    using defference_type = ptrdiff_t;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_copy_assignment = std::false_type;
    using propagate_on_container_swap = std::true_type;

    /**
     * @Brief Equivalent allocator for other type.
     *
     * @tparam U: new Type name.
     */
    template <typename U>
    struct rebind {
        using MemoryPool<U> = other;
    };

    // constructors
    MemoryPool() noexcept;
    MemoryPool(const MemoryPool&) noexcept;
    MemoryPool(MemoryPool&&) noexcept;
    template <typename U>
    MemoryPool(const MemoryPool<U>&) noexcept;

    // destructor
    ~MemoryPool() noexcept;

    // assignment
    MemoryPool& operator=(const MemoryPool&) = delete;
    MemoryPool& operator=(MemoryPool&&) noexcept;

    /**
     * @Brief Get the address o x;
     *
     * @Param (const)reference: x
     *
     * @Returns a pointer to x;
     */
    pointer address(reference) const noexcept;
    const_pointer address(const_reference) const noexcept;

    /**
     * @Brief allocate an new object
     *
     * @Param n: n will be ignored
     * @Param hint: hint will be ignored
     *
     * @Returns a pointer  to new object
     */
    pointer allocate(size_type n = 1, const_pointer hint = nullptr);

    /**
     * @Brief release an object
     *
     * @Param p: Pointer to a block of storage;
     * @Param n: n will be ignored;
     */
    void deallocate(pointer p, size_type n = 1);

    /**
     * @Brief Maximum size possible to allocate.
     *
     * @Returns the number of elements that might be allocated as maximum
     */
    size_type max_size() const noexcept;

    /**
     * @Brief Construct an object.
     *
     * @Param p: Pointer to a location with enough storage space to contain an
     * element of type U.
     * @Param args: Arguments forwarded to the constructor.
     */
    template <typename U, typename... Args>
    void construct(U* p, Args&&... args);

    /**
     * @Brief Destroy an object
     *
     * @Param p: Pointer to the object to be destroyed.
     */
    template <typename U>
    void destroy(U* p);

    /**
     * @Brief create a new T object by Arguments.
     *
     * @Param args: Argumemts forwarded to the constructor.
     *
     * @ReturnsL: A pointer to the new object.
     */
    template <typename... Args>
    pointer newElement(Args... args);

    /**
     * @Brief delete a objcet
     *
     * @Param pointer: Pointer to the object to be deleted
     */
    void deleteElement(pointer);

   private:
    union slot {
        value_type element;
        slot* next;
    };

    using data_pointer = char*;
    using slot_type = slot;
    using slot_pointer = slot*;

    slot_pointer current_block;
    slot_pointer current_slot;
    slot_pointer last_slot;
    // free slot list
    slot_pointer free_slot;

    void allocateBlock();

    static_assert(BlockSize >= 2 * sizeof(slot_type), "BlockSize too small.");
};

#include "MemoryPool.tcc"

#endif
