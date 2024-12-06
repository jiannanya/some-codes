#include <mutex>
#include <condition_variable>
#include <shared_mutex> // 仅为了演示std::shared_lock的用法，实际实现中不使用
#include <thread>
#include <iostream>

class ReadWriteLock {
private:
    std::mutex mtx; // 保护写者计数器和读写者状态的互斥锁
    int writers = 0; // 当前正在写入的线程数
    int readers = 0; // 当前正在读取的线程数
    std::condition_variable cv; // 用于通知等待的线程

public:
    void lock_read() {
        std::unique_lock<std::mutex> lock(mtx);
        // 如果有写者在写入，则等待
        cv.wait(lock, [this] { return writers == 0; });
        // 增加读者计数
        ++readers;
    }

    void unlock_read() {
        std::lock_guard<std::mutex> lock(mtx);
        // 减少读者计数，如果最后一个读者离开且没有写者，则可能需要通知等待的写者
        if (--readers == 0 && writers > 0) {
            cv.notify_one();
        }
    }

    void lock_write() {
        std::unique_lock<std::mutex> lock(mtx);
        // 如果有读者或写者在，则等待
        cv.wait(lock, [this] { return readers == 0 && writers == 0; });
        // 增加写者计数（这里写者计数为1表示有写者，0表示没有）
        ++writers;
    }

    void unlock_write() {
        std::lock_guard<std::mutex> lock(mtx);
        // 减少写者计数，并通知等待的读者或写者
        --writers;
        cv.notify_all();
    }

    // 注意：这个实现没有处理读者优先或写者优先的策略，也没有处理锁升级（从读锁升级到写锁）和降级（从写锁降级到读锁）的复杂情况。
    // 在实际应用中，您可能需要考虑这些额外的功能和优化。
};

// 示例用法
void reader(ReadWriteLock& rw_lock, int id) {
    rw_lock.lock_read();
    std::cout << "Reader " << id << " is reading." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 模拟读取操作
    rw_lock.unlock_read();
    std::cout << "Reader " << id << " finished reading." << std::endl;
}

void writer(ReadWriteLock& rw_lock, int id) {
    rw_lock.lock_write();
    std::cout << "Writer " << id << " is writing." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(200)); // 模拟写入操作
    rw_lock.unlock_write();
    std::cout << "Writer " << id << " finished writing." << std::endl;
}

int main() {
    ReadWriteLock rw_lock;

    std::thread t1(reader, std::ref(rw_lock), 1);
    std::thread t2(reader, std::ref(rw_lock), 2);
    std::thread t3(writer, std::ref(rw_lock), 1);
    std::thread t4(writer, std::ref(rw_lock), 2);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
