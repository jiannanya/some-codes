template <typename T>
class MyVector{
private:
    T* data;
    int size;       // 存储数组中的元素个数
    int capacity;   // 存储数组中可以容纳的最大的元素个数
    // 复杂度为 O(n)
    void resize(int newCapacity){
        T *newData = new T[newCapacity];
        for( int i = 0 ; i < size ; i ++ ){
              newData[i] = data[i];
            }
        data = newData;
        capacity = newCapacity;
    }
public:
    MyVector(){
        data = new T[100];
        size = 0;
        capacity = 100;
    }
    // 平均复杂度为 O(1)
    void push_back(T e){
        if(size == capacity)
            resize(2 * capacity);
        data[size++] = e;
    }
    // 平均复杂度为 O(1)
    T pop_back(){
        size --;
        return data[size];
    }

};
