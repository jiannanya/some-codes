
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