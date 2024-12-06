#include <atomic>
 
class SpinMutex {
    // 使用原子布尔变量来表示锁的状态，初始化为未锁定状态（false）
    std::atomic<bool> locked{false};
 
public:
    // 锁定函数
  void lock() {
    // 尝试短时间自旋锁
    const int spinCount = 1000;
    for (int i = 0; i < spinCount; ++i) {
        // 使用exchange尝试获取锁，如果locked当前为false，则设置为true并返回false
        // 如果locked当前为true，则保持为true并返回true
        if (!locked.exchange(true, std::memory_order_acquire)) {
            // 成功获取锁，立即返回
            return;
        }
        // 锁已被其他线程持有，继续自旋
    }

    // 自旋失败，让出CPU给其他线程
    std::this_thread::yield();

    // 如果仍然无法获取锁，则调用操作系统提供的futex机制等待锁释放
    // 注意：这里直接使用了Linux特定的futex系统调用，这通常不是跨平台的做法
    // 在实际应用中，平台无关的库
    futex_wait(&locked, true);

}
 
    // 解锁函数
    void unlock() {
        // 使用store函数将locked设置为false（未锁定状态）
        // 同时利用memory_order_release确保在释放锁之后的所有写入操作
        // 不会被重排序到释放锁之前
        locked.store(false, std::memory_order_release);
        // 解锁成功后，其他线程可以获取锁并进入临界区
    }
 

};
