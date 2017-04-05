#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
    using pool_size = size_t;
    using task = std::function<void()>;

   public:
    /**
     * @Brief Constructor
     *
     * @Param pool_size: The Number of threads in thread pool
     */
    ThreadPool(pool_size);

    /**
     * @Brief add a job into task queue
     *
     * @tparam Job: a callable object
     * @tparam Args: arguments for Job
     *
     * @Returns the result of the Job
     */
    template <typename Job, typename... Args>
    auto add_job(Job&&, Args&&...);

    /**
     * @Brief Destructor
     */
    ~ThreadPool();

   private:
    // workers in pool, keep track of threads so we can join them
    std::vector<std::thread> workers;

    // the task queue
    std::queue<task> tasks;

    // synchronization
    std::mutex queue_mutex;
    std::condition_variable condition_var;
    bool isStop;
};

ThreadPool::ThreadPool(pool_size size) : isStop(false) {
    for (pool_size i = 0; i < size; ++i) {
        workers.emplace_back([this] {
            task job;
            while (true) {
                {
                    std::unique_lock<std::mutex> lock(queue_mutex);

                    // Block this thread when this pool is running and there are
                    // no
                    // job in the task queue, check again when awake up.
                    this->condition_var.wait(lock, [this] {
                        return this->isStop || !this->tasks.empty();
                    });

                    // When the this pool is destructing and tasks queue is
                    // empty,
                    // end this thread.
                    if (this->isStop && this->tasks.empty()) return;

                    job = std::move(tasks.front());
                    tasks.pop();
                }
                job();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        isStop = true;
    }
    // Notify all workers
    condition_var.notify_all();

    // Join threads
    for (auto& worker : workers) {
        worker.join();
    }
}

template <typename Job, typename... Args>
auto ThreadPool::add_job(Job&& job, Args&&... args) {
    using return_type = typename std::result_of<Job(Args...)>::type;

    auto job_ptr = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<Job>(job), std::forward<Args>(args)...));

    auto result = job_ptr->get_future();

    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        if (isStop)
            throw std::runtime_error("push a job on stopped ThreadPool");
        tasks.emplace([job_ptr]{
            (*job_ptr)();
        });
    }

    condition_var.notify_one();
    return result;
}

#endif
