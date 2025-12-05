using namespace std;
#include <iostream>
#include <string>
#include <stdexcept>


class IntArray {
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
    IntArray() {
        capacity = 4;
        _size = 0;
        data = new int[capacity];
    }

    ~IntArray() {
        delete[] data;
    }

    void push_back(int val) {
        if (_size >= capacity)
            resize(capacity * 2);
        data[_size++] = val;
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
};
