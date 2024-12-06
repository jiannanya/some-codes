#include <atomic>
#include <thread>
#include <iostream>

class BiasedLock {
private:
    std::atomic<std::thread::id> owner_thread{std::thread::id()}; // 记录当前持有锁的线程ID
    std::atomic<bool> biased{true}; // 偏向锁标志，初始为true
    std::atomic<int> state{0}; // 锁状态：0=未锁定，1=偏向锁定，-1=其他线程正在竞争

public:
    BiasedLock() = default;
    ~BiasedLock() = default;

    // 禁止复制和赋值
    BiasedLock(const BiasedLock&) = delete;
    BiasedLock& operator=(const BiasedLock&) = delete;

    void lock() {
        std::thread::id current_thread = std::this_thread::get_id();

        // 检查偏向锁是否由当前线程持有
        if (biased.load(std::memory_order_relaxed) && owner_thread.load(std::memory_order_relaxed) == current_thread) {
            // 当前线程已经持有偏向锁，无需做任何操作
            return;
        }

        // 尝试获取偏向锁
        if (state.compare_exchange_weak(0, 1, std::memory_order_acquire, std::memory_order_relaxed)) {
            // 成功获取偏向锁，设置所有者线程ID
            owner_thread.store(current_thread, std::memory_order_release);
            return;
        }

        // 偏向锁已被其他线程持有或正在竞争，进入竞争锁的逻辑
        while (true) {
            // 检查锁状态
            int s = state.load(std::memory_order_acquire);
            if (s == 0) {
                // 锁未锁定，尝试获取偏向锁（但这次使用strong版本的compare_exchange_strong以确保成功）
                if (state.compare_exchange_strong(s, 1, std::memory_order_release, std::memory_order_relaxed)) {
                    // 成功获取偏向锁，设置所有者线程ID
                    owner_thread.store(current_thread, std::memory_order_relaxed);
                    biased.store(true, std::memory_order_relaxed); // 重置偏向标志（可选，因为我们已经成功获取了锁）
                    return;
                }
            } else if (s == 1 && owner_thread.load(std::memory_order_relaxed) == current_thread) {
                // 当前线程已经持有偏向锁（可能是之前被中断后重新尝试获取）
                return;
            } else {
                // 锁正在被其他线程持有或正在竞争，需要等待或尝试其他锁机制
                // 这里我们简化处理，只使用自旋等待作为示例
                // 在实际应用中，可能需要使用更复杂的等待机制（如条件变量）
                std::this_thread::yield(); // 让出CPU给其他线程
            }
        }
    }

    void unlock() {
        std::thread::id current_thread = std::this_thread::get_id();

        // 检查当前线程是否是锁的持有者
        if (owner_thread.load(std::memory_order_relaxed) == current_thread) {
            // 重置锁状态
            state.store(0, std::memory_order_release);
            owner_thread.store(std::thread::id(), std::memory_order_relaxed);
            // 注意：偏向标志在这里保持为true，因为我们假设下一次很可能还是由当前线程获取锁
            // 在实际应用中，可能需要更复杂的逻辑来动态地决定是否保持偏向锁状态
        } else {
            // 当前线程不是锁的持有者，不执行任何操作（或者可以抛出异常）
            // 注意：在实际应用中，这通常表示一个编程错误，因为你应该只解锁自己持有的锁
        }
    }

    // 注意：这个偏向锁实现缺少了许多实际实现中需要的细节，如：
    // - 撤销偏向锁的逻辑（当锁竞争变得频繁时）
    // - 处理中断和锁重入的逻辑
    // - 更高效的等待和通知机制（如使用条件变量）
    // - 错误处理和调试支持
};

int main() {
    BiasedLock lock;

    std::thread t1([&lock]() {
        lock.lock();
        std::cout << "Thread 1 acquired the lock." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 模拟一些工作
        lock.unlock();
        std::cout << "Thread 1 released the lock." << std::endl;
    });

    std::thread t2([&lock]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // 确保t1先获取锁
        lock.lock();
        std::cout << "Thread 2 acquired the lock." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 模拟一些工作
        lock.unlock();
        std::cout << "Thread 2 released the lock." << std::endl;
    });

    t1.join();
    t2.join();

    return 0;
}
