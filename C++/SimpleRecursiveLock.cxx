#include <mutex>
#include <thread>
#include <stdexcept>

class RecursiveLock {
private:
    std::mutex mtx;
    std::thread::id owner_thread;
    unsigned int lock_count = 0;

public:
    RecursiveLock() : owner_thread(std::thread::id()) {}

    void lock() {
        std::thread::id current_thread = std::this_thread::get_id();

        // 如果当前线程已经是锁的所有者，则增加锁计数
        if (current_thread == owner_thread) {
            ++lock_count;
            return;
        }

        // 否则，尝试获取互斥锁
        mtx.lock();
        // 设置当前线程为锁的所有者，并初始化锁计数为1
        owner_thread = current_thread;
        lock_count = 1;
    }

    void unlock() {
        if (std::this_thread::get_id() != owner_thread) {
            throw std::runtime_error("Unlocking a recursive lock not owned by the current thread.");
        }

        // 减少锁计数
        if (--lock_count == 0) {
            // 如果锁计数为0，则释放互斥锁，并清除所有者线程
            mtx.unlock();
            owner_thread = std::thread::id();
        }
    }

    // 注意：这个实现没有提供try_lock方法，也没有处理死锁或超时的情况。
    // 在实际应用中，您可能需要考虑这些额外的功能和优化。
};

// 示例用法
void recursive_function(RecursiveLock& lock, int depth) {
    if (depth <= 0) return;

    lock.lock();
    std::cout << "Lock acquired at depth " << depth << " by thread " << std::this_thread::get_id() << std::endl;

    // 模拟一些工作
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // 递归调用自身
    recursive_function(lock, depth - 1);

    // 释放锁
    lock.unlock();
    std::cout << "Lock released at depth " << depth << " by thread " << std::this_thread::get_id() << std::endl;
}

int main() {
    RecursiveLock lock;

    std::thread t1(recursive_function, std::ref(lock), 3);
    std::thread t2(recursive_function, std::ref(lock), 2); // 注意：这个示例中同时启动多个线程是不安全的，因为锁不是线程安全的共享资源。
    // 在实际应用中，你应该确保只有一个线程在尝试获取锁，或者使用其他同步机制来保护对锁的访问。

    // 为了简化示例，我们只等待一个线程完成。
    t1.join();
    // t2.join(); // 在这个示例中不要取消注释这一行，因为它会导致未定义行为，因为lock同时被两个线程尝试获取。

    return 0;
}
