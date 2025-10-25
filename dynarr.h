#include <iostream>
using namespace std;

class Array {
    private:
        int* data;
        int capacity;
        int _size;
    
        void resize(int newCapacity) {
            int* newData = new int[newCapacity];
            for (int i = 0; i < _size; ++i)
                newData[i] = data[i];
            delete[] data;
            data = newData;
            capacity = newCapacity;
        }
    
    public:
        Array() {
            capacity = 4;
            _size = 0;
            data = new int[capacity];
        }
    
        ~Array() {
            delete[] data;
        }
    
        void push_back(int val) {
            if (_size >= capacity)
                resize(capacity * 2);
            data[_size++] = val;
        }
    
        void pop_back() {
            if (_size == 0) throw runtime_error("Array is empty");
            _size--;
        }
    
        int& operator[](int index) {
            if (index < 0 || index >= _size)
                throw runtime_error("Index out of range");
            return data[index];
        }
    
        int size() const { return _size; }
    
        void erase(int index) {
            if (index < 0 || index >= _size)
                throw runtime_error("Index out of range");
            for (int i = index; i < _size - 1; ++i)
                data[i] = data[i + 1];
            _size--;
        }
    
        void reverse(int left, int right) {
            while (left < right) {
                int tmp = data[left];
                data[left] = data[right];
                data[right] = tmp;
                left++;
                right--;
            }
        }
    
        void print() const {
            for (int i = 0; i < _size; ++i)
                cout << data[i] << " ";
            cout << endl;
        }
    };