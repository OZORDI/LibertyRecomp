#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <atomic>
#include <memory>

namespace Install
{
    /**
     * Simple thread pool for parallel task execution.
     * Used for multi-threaded RPF extraction and file processing.
     */
    class ThreadPool
    {
    public:
        /**
         * Create thread pool with specified number of worker threads.
         * @param numThreads Number of threads (default: hardware concurrency)
         */
        explicit ThreadPool(size_t numThreads = 0);
        
        /**
         * Destructor - waits for all tasks to complete.
         */
        ~ThreadPool();
        
        // Non-copyable, non-movable
        ThreadPool(const ThreadPool&) = delete;
        ThreadPool& operator=(const ThreadPool&) = delete;
        ThreadPool(ThreadPool&&) = delete;
        ThreadPool& operator=(ThreadPool&&) = delete;
        
        /**
         * Enqueue a task for execution.
         * @param task Function to execute
         * @return Future to get the result
         */
        template<typename F, typename... Args>
        auto Enqueue(F&& task, Args&&... args)
            -> std::future<typename std::invoke_result<F, Args...>::type>;
        
        /**
         * Wait for all queued tasks to complete.
         */
        void Wait();
        
        /**
         * Get number of worker threads.
         */
        size_t GetThreadCount() const { return m_workers.size(); }
        
        /**
         * Get number of pending tasks.
         */
        size_t GetPendingCount() const;
        
        /**
         * Get number of active (running) tasks.
         */
        size_t GetActiveCount() const { return m_activeTasks.load(); }
        
    private:
        std::vector<std::thread> m_workers;
        std::queue<std::function<void()>> m_tasks;
        
        mutable std::mutex m_mutex;
        std::condition_variable m_condition;
        std::condition_variable m_doneCondition;
        
        std::atomic<bool> m_stop{false};
        std::atomic<size_t> m_activeTasks{0};
    };
    
    /**
     * Memory budget limiter for controlling concurrent memory usage.
     * Prevents OOM during parallel extraction of large files.
     */
    class MemoryBudget
    {
    public:
        /**
         * Create memory budget with specified limit.
         * @param limitBytes Maximum bytes that can be allocated
         */
        explicit MemoryBudget(size_t limitBytes);
        
        /**
         * Acquire memory from the budget (blocks if over limit).
         * @param bytes Number of bytes to acquire
         */
        void Acquire(size_t bytes);
        
        /**
         * Try to acquire memory without blocking.
         * @param bytes Number of bytes to acquire
         * @return true if acquired, false if would exceed budget
         */
        bool TryAcquire(size_t bytes);
        
        /**
         * Release memory back to the budget.
         * @param bytes Number of bytes to release
         */
        void Release(size_t bytes);
        
        /**
         * Get current usage.
         */
        size_t GetUsed() const { return m_used.load(); }
        
        /**
         * Get limit.
         */
        size_t GetLimit() const { return m_limit; }
        
    private:
        std::atomic<size_t> m_used{0};
        const size_t m_limit;
        std::mutex m_mutex;
        std::condition_variable m_cv;
    };
    
    // Template implementation
    template<typename F, typename... Args>
    auto ThreadPool::Enqueue(F&& task, Args&&... args)
        -> std::future<typename std::invoke_result<F, Args...>::type>
    {
        using ReturnType = typename std::invoke_result<F, Args...>::type;
        
        auto packagedTask = std::make_shared<std::packaged_task<ReturnType()>>(
            std::bind(std::forward<F>(task), std::forward<Args>(args)...));
        
        std::future<ReturnType> result = packagedTask->get_future();
        
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
            if (m_stop)
            {
                throw std::runtime_error("Cannot enqueue on stopped ThreadPool");
            }
            
            m_tasks.emplace([packagedTask]() { (*packagedTask)(); });
        }
        
        m_condition.notify_one();
        return result;
    }
    
} // namespace Install
