Memory Pool
=============

A Simple Memory Pool in C++11

+ No memory fragmentation.

+ Better performance then `std::allocate<T>`

+ Usage:
```C++
    MemoryPool<ELEMENT_TYPE, BLOCK_SIZE> pool;
    auto p = pool.allocate();
    pool.construct(p, args...);
    // or
    auto q = pool.newElement(args...);
    /**
     * do something with p, q
     */
    pool.destory(p);
    pool.deallocate(p);
    // or
    pool.deleteElement(q);
    
```

### Warning

+ MemorPool cannot allocate multiple objects with a single call to allocate and will simply ignore the count value you pass to the allocate/deallocate function. 

+ This is NOT thread safe. 
