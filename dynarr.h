#include <iostream>
using namespace std;

class nodeList {
    public:
        string data;
        nodeList* prev;
        nodeList* next;
        nodeList(string& val) : data(val), prev(nullptr), next(nullptr) {};
};

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


    class LinkedList {
        private:
            nodeList* head;
            nodeList* tail;
        public:
            LinkedList() : head(nullptr), tail(nullptr) {};
            void push(string value) {
                nodeList* current = new nodeList(value);
                if (!head) {
                    head = tail = current;
                } 
                else {
                    tail -> next = current;
                    current -> prev = tail;
                    tail = current;
                }
            }
            void rm(string value) {
                nodeList* current = head;
                while (current) {
                    if (current -> data == value) {
                        if (current -> prev) current -> prev -> next = current -> next;
                        else head = current -> next;
                        if (current -> next) current -> next -> prev = current -> prev;
                        else tail = current -> prev;
                        delete current;
                        return;
                    }
                    current = current -> next;
                }
            }
            void printList() {
                nodeList *current = head;
                while (current) {
                    cout << current -> data << endl;
                    current = current -> next;
                }
            }
            string pop() {
                if (!tail) throw runtime_error("Empty");
                string output = tail -> data;
        
                nodeList* temp = tail;
                tail = tail->prev;
        
                if (tail)
                    tail->next = nullptr;
                else
                    head = nullptr;
        
                delete temp;
                return output;
            }
            string showTail() {
                if (!tail) throw runtime_error("Empty");
                return tail->data;
            }
        
            bool isEmpty() {
                return head == nullptr;
            }
        
            ~LinkedList() {
                nodeList *current = head;
                while (current) {
                    nodeList* nextnodeList = current -> next;
                    delete current;
                    current = nextnodeList;
                }
                head = tail = nullptr;
            }
        
        };
        
        class Stack {
        private:
            LinkedList list;
        public:
            void pushStack(string val) {list.push(val);}
            void pop() {list.pop();}
            string top() {return list.showTail();}
            bool isEmpty() {return list.isEmpty();}
        };