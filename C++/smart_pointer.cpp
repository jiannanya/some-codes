//C++ 智能指针基本实现

// unique ptr
template<typename T>
class UniquePtr {
  using pointer = T*;

private:
  pointer data;

private:
  // 清内存并置空
  void _destroy(UniquePtr& target) noexcept {
      if (target.data != nullptr) {
          delete target.data;
          target.data = nullptr;
      }
  }

public:
  // 用于外部获取指针原始类型
  typedef T ElementType;

  // 默认构造函数, 赋值为nullptr
  UniquePtr() noexcept :data(nullptr) {}

  // 显式构造函数, 为了防止隐式类型转换
  explicit UniquePtr(const pointer& data) noexcept : data(data) {}

  // 析构函数
  ~UniquePtr() noexcept { _destroy(*this); }

  // 移动构造和移动赋值都存在, 用swap实现, 移动后记得清空对方
  UniquePtr(UniquePtr&& moving) noexcept : data(nullptr) {
      swap(moving);
      _destroy(moving);
  }
  // 因为有了nullptr_t构造, 因此可以进行nullptr赋值
  UniquePtr& operator=(UniquePtr&& moving) noexcept {
      if (this != &moving){
          swap(moving);
          _destroy(moving);
      }
      return *this;
  };

  // 拷贝构造和拷贝赋值都被禁止, 采用const&来写就是万能引用
  UniquePtr(const UniquePtr&) noexcept = delete;
  UniquePtr& operator=(const UniquePtr&) noexcept = delete;
  // 仅允许使用nullptr进行拷贝赋值, 因为这相当于reset
  UniquePtr& operator=(std::nullptr_t) noexcept {
      reset();
      return *this;
  }

  // 显式bool转换函数
  explicit operator bool() const noexcept { return data != nullptr; }

  // 只能指针都需要模仿原生指针, 注意不要对nullptr操作
  T& operator*() const noexcept { assert(data != nullptr); return *data; }
  pointer operator->() const noexcept { assert(data != nullptr); return data; }
  pointer get() const noexcept { return data; }

  // 用于交换指针的成员函数, 非常非常常用
  void swap(UniquePtr& other) noexcept {
      std::swap(data, other.data);
  }
  void swap(UniquePtr&& other) noexcept {
      std::swap(data, other.data);
  }

  // 将内部指针置为外部值并删去当前值, 注意防止自我赋值
  void reset(pointer p = nullptr) noexcept {
      swap(UniquePtr(p));
  }
};



class PtrBase {
public:
  // stl实现的智能指针还会在Counter中放入注册好的deleter
  struct Counter {
      int uses = 0;
      int weaks = 0;
  };
  using p_counter = Counter*;
  using p_mutex = std::mutex*;
  // 堆计数器用来在多个ptr间共享
  p_counter counter;
  // 堆内存mutex保证线程安全, 计数器为nullptr时才回收
  p_mutex mutex;

protected:
  // 这里用到了委托构造的技巧. 需要new计数器和互斥量
  // 注意由于用到new所以可能产生异常. 让异常逃离构造函数很麻烦, 因此用nothrow然后自己处理
  PtrBase() noexcept : PtrBase(
      new (std::nothrow) Counter(),
      new (std::nothrow) std::mutex())
  {}
  PtrBase(std::nullptr_t) noexcept : PtrBase() {}
  PtrBase(p_counter counter, p_mutex mutex) noexcept :
      counter(counter),
      mutex(mutex)
  {}

  void increase_shared_count() noexcept {
      if (counter != nullptr) {
          mutex->lock();
          ++(counter->uses);
          mutex->unlock();
      }
  }

  void increase_weak_count() noexcept {
      if (counter != nullptr) {
          mutex->lock();
          ++(counter->weaks);
          mutex->unlock();
      }
  }

  // 只要share计数为0就返回给指针本身以回收目标对象的内存
  bool reduce_shared_count() noexcept {
      bool is_zero = true;
      if (counter != nullptr) {
          mutex->lock();
          --(counter->uses);
          if (counter->uses > 0) {
              is_zero = false;
          }
          mutex->unlock();
      }
      return is_zero;
  }

  // 只有当两种引用都为0时才可以回收计数器本身的内存, 记得所有对堆内存的修改都加锁
  void reduce_weak_count() noexcept {
      if (counter != nullptr) {
          mutex->lock();
          if (counter->weaks > 0) {
              --(counter->weaks);
          }
          if (counter->uses == 0 && counter->weaks == 0) {
              delete counter;
              counter = nullptr;
          }
          mutex->unlock();
      }
  }

  void check_mutex() noexcept {
      if (counter == nullptr) {
          delete mutex;
          mutex = nullptr;
      }
  }

  // new失败的时候做的补救措施
  void revert() noexcept {
      if (mutex != nullptr) {
          reduce_shared_count();
          reduce_weak_count();
          delete mutex;
          mutex = nullptr;
      }
      if (counter != nullptr) {
          delete counter;
          counter = nullptr;
      }
  }

  void swap(PtrBase& other) noexcept {
      std::swap(counter, other.counter);
      std::swap(mutex, other.mutex);
  }
};

template<typename T>class EnableSharedFromThis;
template<typename T> class WeakPtr;

// share ptr
template<typename T>
class SharedPtr : public PtrBase {
  using pointer = T*;
  // 需要和WeakPtr形成friend方便两者之间的转型
  friend class WeakPtr<T>;

private:
  // 原生指针
  pointer data;

private:
  // 先减少计数, 如果为0则释放资源
  void _destroy(SharedPtr& target) noexcept {
      if (data != nullptr) {
          if (target.reduce_shared_count()) {
              delete target.data;
              target.data = nullptr;
          }
          target.check_mutex();
      }
  }

  // 给WeakPtr用的构造
  SharedPtr(const WeakPtr<T>& wptr) noexcept : data(wptr.data), PtrBase(wptr.counter, wptr.mutex) {
      increase_shared_count();
  }

  void enable_weak_this() noexcept{
      if(std::is_convertible_v<T*,EnableSharedFromThis<T>*>){
            auto e = dynamic_cast<EnableSharedFromThis<T>*>(data);
            e->data = new WeakPtr<T>(data);
      }
  }

public:
  // 用于外部获取指针原始类型
  typedef T ElementType;

  // 默认构造函数, 全部赋为nullptr
  SharedPtr() noexcept : data(nullptr), PtrBase() {};

  // 记得显式以防止隐式类型转换
  explicit SharedPtr(const pointer& data) noexcept :
      data(data), PtrBase() {
      // nullptr代表空间申请失败
      if (counter == nullptr || mutex == nullptr) {
          this->data = nullptr;
          revert();
      }
      if (data != nullptr) {
          increase_shared_count();
          enable_weak_this();
      }


  }

  ~SharedPtr() noexcept {
      _destroy(*this);
  }

  // 拷贝构造, shared_ptr拷贝后会将计数器+1
  SharedPtr(const SharedPtr& copy) noexcept : data(copy.data), PtrBase(copy.counter, copy.mutex) {
      if (data != nullptr) {
          increase_shared_count();
          enable_weak_this();
      }
  }
  // 拷贝赋值, 采用copy-swap写法
  SharedPtr& operator=(const SharedPtr& copy) noexcept {
      SharedPtr tmp(copy);
      swap(copy);
      return *this;
  }
  // 用nullptr赋值时相当于清空
  SharedPtr& operator=(std::nullptr_t) noexcept {
      _destroy(*this);
      return *this;
  }

  // 移动构造函数, 由于是构造所以可以直接夺取指针内容
  // 析构的时候由于目标是nullptr所以自然结束掉
  SharedPtr(SharedPtr&& moving) noexcept : data(nullptr), PtrBase() {
      swap(moving);
      _destroy(moving);
      enable_weak_this();
  }
  // 移动赋值函数
  SharedPtr& operator=(SharedPtr&& moving) noexcept {
      if (this != &moving) {
          swap(moving);
          _destroy(moving);
          enable_weak_this();
      }
      return *this;
  }

  // 老三样
  pointer operator->() const noexcept { assert(data != nullptr); return data; }
  T& operator*() const noexcept { assert(data != nullptr); return *data; }
  pointer get() const noexcept { return data; }

  // 用于交换指针的成员函数
  void swap(SharedPtr& other) noexcept {
      std::swap(data, other.data);
      PtrBase::swap(other);
      enable_weak_this();
  }

  void swap(SharedPtr&& other) noexcept {
      std::swap(data, other.data);
      PtrBase::swap(other);
      enable_weak_this();
  }

  // 通过与新构造的对象交换来简化代码
  void reset(pointer p = nullptr) noexcept {
      swap(SharedPtr(p));
      enable_weak_this();
  }

  // 返回当前计数器次数
  int use_count() const noexcept { assert(&counter != nullptr); return counter->uses; }

  explicit operator bool() const noexcept { return data != nullptr; }
};

//weakptr
template<typename T>
class WeakPtr : public PtrBase {
  using pointer = T*;
  friend class SharedPtr<T>;

private:
  pointer data;

private:
  void _destroy(WeakPtr& target) noexcept {
      if (data != nullptr) {
          target.reduce_weak_count();
          target.check_mutex();
      }
  }

public:
  // 用于外部获取指针原始类型
  typedef T ElementType;
  WeakPtr() noexcept : data(nullptr), PtrBase() {}
  ~WeakPtr() noexcept { _destroy(*this); }
  WeakPtr(const SharedPtr<T>& sptr) noexcept :data(sptr.data), PtrBase(sptr.counter, sptr.mutex) {
      if (data != nullptr) {
          increase_weak_count();
      }
  }
  WeakPtr& operator=(const SharedPtr<T>& copy) noexcept {
      WeakPtr<T> tmp(copy);
      swap(tmp);
      return *this;
  }
  WeakPtr(const WeakPtr& copy) noexcept :data(copy.data), PtrBase(copy.counter, copy.mutex) {
      if (data != nullptr) {
          increase_weak_count();
      }
  }
  WeakPtr& operator=(const WeakPtr& copy) noexcept {
      WeakPtr tmp(copy);
      swap(tmp);
      if (data != nullptr) {
          increase_weak_count();
      }
      return *this;
  }
  WeakPtr& operator=(std::nullptr_t) noexcept {
      reset();
      return *this;
  }

  WeakPtr(WeakPtr&& moving) noexcept : data(nullptr), PtrBase() {
      swap(moving);
      _destroy(moving);
  }
  WeakPtr& operator=(WeakPtr&& moving) noexcept {
      if (this != &moving) {
          swap(moving);
          _destroy(moving);
      }
      return *this;
  }

  SharedPtr<T> lock() noexcept {
      if (expired()) {
          return SharedPtr<T>(nullptr);;
      }
      else {
          return SharedPtr<T>(*this);
      }
  }

  void reset() noexcept {
      swap(WeakPtr());
  }

  void swap(WeakPtr& other) noexcept {
      std::swap(data, other.data);
      PtrBase::swap(other);
  }

  void swap(WeakPtr&& other) noexcept {
      std::swap(data, other.data);
      PtrBase::swap(other);
  }

  int use_count() const noexcept { assert(counter != nullptr);  return counter->uses; }
  bool expired() const noexcept { return counter->uses == 0; }
};

// make_ptr
template<typename T, typename... Args>
inline UniquePtr<T> MakeUnique(Args&&... args) {
  return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

template<typename T, typename... Args>
inline SharedPtr<T> MakeShared(Args&&... args) {
  return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

// enable_share_frome_this
// https://stackoverflow.com/questions/34061515/how-stdenable-shared-from-thisshared-from-this-works
template<typename T>
class EnableSharedFromThis
{
    mutable WeakPtr<T> data;

protected:

    EnableSharedFromThis() noexcept {}
    EnableSharedFromThis(EnableSharedFromThis const&) noexcept {}
    EnableSharedFromThis& operator=(EnableSharedFromThis const&) noexcept{return *this;}
    ~EnableSharedFromThis() {}

public:

    SharedPtr<T> shared_from_this(){return SharedPtr<T>(data);}
    SharedPtr<T const> shared_from_this() const{return SharedPtr<const T>(data);}

    WeakPtr<T> weak_from_this() noexcept{ return data; }
    WeakPtr<const T> weak_from_this() const noexcept{ return data; }

    template <typename U> friend class SharedPtr;
};

