#include <iostream>
using namespace std;
//template <typename T> // :(
class Array {
private:
    string* data;
    int capacity;
    int size;
    void resize(int newCapacity) {
        string *newData = new string[newCapacity];
        for (int i = 0; i < size; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }
public:
    Array() {
        capacity = 4;
        size = 0;
        data = new string[capacity];
    }
    void push(string val) {
        if (size >= capacity) {
            resize(capacity * 2);
        }
        data[size++] = val;
    }
    string get(int index) {
        if (index < 0 || index >= size) throw runtime_error("Out of range");
        return data[index];
    }
    void set(int index, string val) {
        if (index < 0 || index >= size) throw runtime_error("Out of range");
        data[index] = val;
    }
    void pop() {
        if (size == 0) throw runtime_error("Empty");
        size--;
    }
    int getSize() {return size;}
    void print() {
        for (int i = 0; i < size; ++i) {
            cout << data[i] << " ";
        }
        cout << endl;
    }
    ~Array() {
        delete[] data;
    }
};

class Node {
public:
    string data;
    Node* prev;
    Node* next;
    Node(string& val) : data(val), prev(nullptr), next(nullptr) {};
};

class LinkedList {
private:
    Node* head;
    Node* tail;
public:
    LinkedList() : head(nullptr), tail(nullptr) {};
    void push(string value) {
        Node* current = new Node(value);
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
        Node * current = head;
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
        Node *current = head;
        while (current) {
            cout << current -> data << endl;
            current = current -> next;
        }
    }
    string pop() {
        if (!tail) throw runtime_error("Empty");
        string output = tail -> data;

        Node* temp = tail;
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
        Node *current = head;
        while (current) {
            Node* nextNode = current -> next;
            delete current;
            current = nextNode;
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

int priority(string op) {
    if (op == "+" || op == "-") return 1;
    if (op == "/" || op == "*") return 2;
    if (op == "^") return 3;
    if (op == "sin" || op == "cos") return 4;
}

bool isOperator(string token) {
    return token == "+" || token == "-" || token == "*" || token == "/" 
    || token == "^";
}

bool isFunction(string token) {
    return token == "sin" || token == "cos";
}

bool isRightAs(string op) {
    return op == "^";
}

void toRPN(const string& expression, string output[100], int& outPos) {
    Stack stack;
    outPos = 0;
    int i = 0;
    while (expression[i] != '\0') {
        if (isspace(expression[i])) {
            i++;
            continue;
        }
        if (isdigit(expression[i])) {
            string number; // since number is a sequence of digits
            while (isdigit(expression[i])) {
                number += expression[i++];

            }
            output[outPos++] = number;
        }
        else if (isalpha(expression[i])) {
            string trigFunc;
            while (isalpha(expression[i])) {
                trigFunc += expression[i++];
            }
        
            if (trigFunc == "sin" || trigFunc == "cos") {
                stack.pushStack(trigFunc);
            } else {
                throw runtime_error("Ne sinus i ne cosinus bratan");
            }
            continue;
        }
        else if (expression[i] == '(') {
            stack.pushStack("(");
            i++;
        }
        else if (expression[i] == ')') {
            while (stack.top() != "(" && !stack.isEmpty()) {
                output[outPos++] = stack.top();
                stack.pop();
            }
            if (stack.top() == "(" && !stack.isEmpty()) {
                stack.pop();
            }
            if (isFunction(stack.top()) && !stack.isEmpty()) {
                output[outPos++] = stack.top();
                stack.pop();
            }
            i++;
        }
        else {
            string oper(1, expression[i]);
            while (!stack.isEmpty() && (isOperator(stack.top()) || isFunction(stack.top())) && ((!isRightAs(oper) && priority(oper) <= priority(stack.top())) ||
            (isRightAs(oper) && priority(oper) <  priority(stack.top())))) { // fantasticheskoe uslovie cenoi v 20 minut
                output[outPos++] = stack.top();
                stack.pop();
            }
            stack.pushStack(oper);
            i++;
        }


    }
    while (!stack.isEmpty()) {
        output[outPos++] = stack.top();
        stack.pop();
    }

}



int main() {
    //string expression = "3 + 4 * 2 / (1 - 5) ^ 2 ^ 3";
    //string test1 = "cos (4) + 4 * 23 - 1 + (1 - 5)";
    //string test2 = "sin(3 + 4 * cos(2))"; нужны пробелы
    string expr;
    //cin >> expr;
    getline(cin, expr);
    string output[100];
    int count = 0;
    toRPN(expr, output, count);
    cout << "RPN" << endl;
    for (int i = 0; i < count; ++i) {
        cout << output[i] << " ";
    }
    cout << endl;
    return 0;
}