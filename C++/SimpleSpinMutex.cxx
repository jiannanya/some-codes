#include <atomic>
 
class SpinMutex {
    // 使用原子布尔变量来表示锁的状态，初始化为未锁定状态（false）
    std::atomic<bool> locked{false};
 
public:
    // 锁定函数
    void lock() {
        // 使用exchange函数尝试将locked设置为true（锁定状态）
        // 同时利用memory_order_acquire确保在获取锁之前的所有读取操作
        // 不会被重排序到获取锁之后
        while (locked.exchange(true, std::memory_order_acquire)) {
            // 如果locked已经为true（即锁已被其他线程持有）
            // 则继续自旋等待，直到锁被释放（locked变为false）
            // 注意：这里没有添加任何形式的让步（如std::this_thread::yield()）
            // 在实际应用中，可能需要添加以避免忙等待导致的CPU资源浪费
        }
        // 锁定成功后，可以安全地在临界区内执行操作
    }
 
    // 解锁函数
    void unlock() {
        // 使用store函数将locked设置为false（未锁定状态）
        // 同时利用memory_order_release确保在释放锁之后的所有写入操作
        // 不会被重排序到释放锁之前
        locked.store(false, std::memory_order_release);
        // 解锁成功后，其他线程可以获取锁并进入临界区
    }
 
    // 注意：此类实现了一个非常基本的自旋锁
    // 在实际应用中，可能需要考虑更复杂的锁机制（如互斥锁、读写锁等）
    // 以及锁的优化（如避免忙等待、优先级继承等）
    // 此外，还需要注意避免死锁和优先级反转等问题
};
