
/*
* From Scott Meyers <<Effective C++>> --Item 4:Make sure that objects are initialized before they're used
*
* 1.仅当程序第一次执行到 GetInstance 函数时，执行 instance 对象的初始化；
* 2.利用C++ 11标准语编译器实现，静态局部变量（static修饰的）可以保证是线程安全的；
*/

template<typename T>
class Singleton
{
public:
    static T& GetInstance()
    {
        static T instance;
        return instance;
    }

    Singleton(T&&) = delete;
    Singleton(const T&) = delete;
    void operator= (const T&) = delete;

protected:
    Singleton() = default;
    virtual ~Singleton() = default;
};


// 一般线程安全单例-饿汉模式
class HungrySingleton{
private:
    int count = 0;
    static HungrySingleton* instance;
    HungrySingleton() = default;
public:
    ~HungrySingleton() = default;
    static HungrySingleton* getInstance(){
        return instance;
    }
    void showMessage(){
        cout<<count<<endl;
    }
};

/* 初始化类静态变量 */
HungrySingleton* HungrySingleton::instance = new HungrySingleton();


//一般线程安全单例-懒汉模式（双重校验锁）
class LazySingleton{
private:
    static LazySingleton* instance;
    static mutex _lock;
    int count = 1;
    LazySingleton() = default;
public:
    ~LazySingleton() = default;
    static LazySingleton* getInstance(){
        /* 缩小锁粒度，降低性能开销，提高效率 */
        if(instance==nullptr){
            lock_guard<mutex> locker(_lock);
            if(instance== nullptr){
                instance = new LazySingleton;
            }
        }
        return instance;
    }
    void showMessage(){
        cout<<count;
    }
};

/* 初始化类静态变量 */
mutex LazySingleton::_lock;
LazySingleton* LazySingleton::instance = nullptr;


//利用C++局部静态变量特性的线程安全单例
class LazySingleton{
private:
    int count = 1;
    LazySingleton() = default;
public:
    ~LazySingleton() = default;
    static LazySingleton* getInstance(){
        /* 局部静态变量只会在第一次声明的时候初始化，在c++11以及之后的版本可以做到线程安全 
        1.变量在代码第一次执行到变量声明的地方时初始化。
		2.初始化过程中发生异常的话视为未完成初始化，未完成初始化的话，需要下次有代码执行到相同位置时再次初始化。
        3.在当前线程执行到需要初始化变量的地方时，如果有其他线程正在初始化该变量，则阻塞当前线程，直到初始化完成为止。
        4.如果初始化过程中发生了对初始化的递归调用，则视为未定义行为
        */
        static LazySingleton instance;
        return &instance;
    }
    void showMessage(){
        cout<<count;
    }
};
