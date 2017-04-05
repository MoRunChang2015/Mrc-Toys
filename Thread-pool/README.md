Thread Pool
=============

A Simple Thread Pool in C++14

+ Usage:
```C++

    ThreadPool pool(SIZE_OF_THREAD_POOL);
    // result's type is std::future<result_of<Funtion(Arguments)>>
    auto result = pool.add_job(Function, Arguments...);
    
    std::cout << result.get() << std::endl;
```
