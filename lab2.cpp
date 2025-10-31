#include <iostream>
using namespace std;
#include <cstdlib>

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

struct Run {
    int startPos;
    size_t lengthOfRun;
    Run(int s, int l) : startPos(s), lengthOfRun(l) {}
};


int getMinRunSize(int n) { // https://habr.com/ru/companies/infopulse/articles/133303/
    int remainderBit = 0;
    while (n >= 64) {
        remainderBit |= (n & 1);
        n >>= 1;
    }
    return n + remainderBit;
}



void insertionSort(Array& arr, int left, int right) {
    for (int i = left + 1; i <= right; ++i) {
        int curVal = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > curVal) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = curVal;
    }
}


int gallopLeft(int key, Array& arr, int startVal, int length) {
    int lastOffset = 0;
    int offset = 1;

    if (key > arr[startVal]) {
        int maxOffset = length;
        while (offset < maxOffset && key > arr[startVal + offset]) {
            lastOffset = offset;
            offset = (offset << 1) + 1; // offset = offset * pow(2, i) + 1;
            if (offset <= 0) offset = maxOffset;
        }
        if (offset > maxOffset) offset = maxOffset;

        lastOffset += startVal;
        offset += startVal;
    } else {
        return startVal;
    }

    while (lastOffset + 1 < offset) {
        int mid = lastOffset + ((offset - lastOffset) >> 1);
        if (key > arr[mid]) lastOffset = mid;
        else offset = mid;
    }
    return offset;
}


int gallopRight(int key, Array& arr, int startVal, int length) {
    int lastOffset = 0, offset = 1;

    if (key < arr[startVal]) {
        return startVal;
    }

    int maxOffset = length;
    while (offset < maxOffset && key >= arr[startVal + offset]) {
        lastOffset = offset;
        offset = (offset << 1) + 1;
        if (offset <= 0) offset = maxOffset;
    }
    if (offset > maxOffset) offset = maxOffset;

    lastOffset += startVal;
    offset += startVal;

    while (lastOffset + 1 < offset) {
        int mid = lastOffset + ((offset - lastOffset) >> 1);
        if (key >= arr[mid]) lastOffset = mid;
        else offset = mid;
    }
    return offset;
}



void merge(Array& arr, int left, int right, int mid) {
    int len1 = mid - left + 1;
    int len2 = right - mid;
    Array leftArr;
    Array rightArr;

    for (int i = 0; i < len1; i++) leftArr.push_back(arr[left + i]);
    for (int i = 0; i < len2; i++) rightArr.push_back(arr[mid + 1 + i]);

    int i = 0, j = 0, k = left;
    int gth = 10; 
    int countLeft = 0, countRight = 0;

    while (i < len1 && j < len2) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k++] = leftArr[i++];
            countLeft++;
            countRight = 0;
        } else {
            arr[k++] = rightArr[j++];
            countRight++;
            countLeft = 0;
        }

        if (countLeft >= gth) {
            cout << "galloping left!" << endl;
            int newPos = gallopLeft(rightArr[j], leftArr, i, len1 - i);
            while (i < newPos && i < len1)
                arr[k++] = leftArr[i++];
            countLeft = 0;
        } else if (countRight >= gth) {
            cout << "galloping right !" << endl;
            int newPos = gallopRight(leftArr[i], rightArr, j, len2 - j);
            while (j < newPos && j < len2)
                arr[k++] = rightArr[j++];
            countRight = 0;
        }
    }

    while (i < len1) arr[k++] = leftArr[i++];
    while (j < len2) arr[k++] = rightArr[j++];
}




void mergeTop(Array& arr, Array& runStackStart, Array& runStackLen, int i) {
    int left = runStackStart[i];
    int mid = runStackStart[i] + runStackLen[i] - 1;
    int right = runStackStart[i + 1] + runStackLen[i + 1] - 1;

    merge(arr, left, right, mid);
    runStackLen[i] += runStackLen[i + 1];
    runStackStart.erase(i + 1);
    runStackLen.erase(i + 1);
}


void mergeRun(Array& arr, Array& runStackStart, Array& runStackLen) {
    while (runStackLen.size() > 0) {
        int n = runStackLen.size();
        bool merged = false;

        if (n >= 3 && runStackLen[n - 3] <= runStackLen[n - 2] + runStackLen[n - 1]) {
            if (runStackLen[n - 3] < runStackLen[n - 1])
                mergeTop(arr, runStackStart, runStackLen, n - 3);
            else
                mergeTop(arr, runStackStart, runStackLen, n - 2);
            merged = true;
        } else if (runStackLen[n - 2] <= runStackLen[n - 1]) {
            mergeTop(arr, runStackStart, runStackLen, n - 2);
            merged = true;
        }

        if (!merged) break;
    }
}

void timsort(Array& arr) {
    int n = arr.size();
    if (n < 2) return;
    int minRun = getMinRunSize(n);
    bool isSorted = true;
    Array runStart;
    Array runLen;
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] < arr[i - 1]) {
            isSorted = false;
        }
    }
    if (isSorted) {
        cout << "Already sorted" << endl;
        return;
    }

    int i = 0;
    while (i < n) {
        int runStartPos = i;
        int runEnd = i + 1;
        if (runEnd < n && arr[runEnd] < arr[runEnd - 1]) {
            while (runEnd < n && arr[runEnd] < arr[runEnd - 1]) runEnd++;
            arr.reverse(runStartPos, runEnd - 1);
        } else {
            while (runEnd < n && arr[runEnd] >= arr[runEnd - 1]) runEnd++;
        }
        int runLength = runEnd - runStartPos;
        if (runLength < minRun) {
            int forcedEnd = min(runStartPos + minRun, n);
            cout << "runLength: " << runLength<< endl;
            cout << "Starting insertion" << endl;
            insertionSort(arr, runStartPos, forcedEnd - 1);
            runLength = forcedEnd - runStartPos;    
            runEnd = forcedEnd;
        }
        runStart.push_back(runStartPos);
        runLen.push_back(runLength);
        runLen.print();
        mergeRun(arr, runStart, runLen);
        i = runEnd;
    }

    while (runLen.size() > 1) {
        mergeTop(arr, runStart, runLen, runLen.size() - 2);
    }
}


int main() {
    Array arr1;
    int sizeAr;
    int val;
    cout << "Size?: " << endl;
    cin >> sizeAr;
    for (int i = 0; i < sizeAr; ++i) {
        cin >> val;
        arr1.push_back(val);
    }
    //for (int i = 0; i < 10000; ++i) {arr1.push_back(rand() % 10000 + 1);}
    timsort(arr1);
    arr1.print();
    // vector<int> arr1;
    // for (int i = 0; i < 50; i++) arr1.push_back(i);
    // for (int i = 1000; i < 1050; i++) arr1.push_back(i);
    
    // timsort(arr1);
    // cout << "Result first 10: ";
    // for (int i = 0; i < 10; i++) cout << arr1[i] << " ";
    // cout << "\n\n";
    // //vector<int> arr = {1, 2, 5, 19, 192, 129, 15, 1, 1, 2, 3, 4, 5, 6, 7, 8, 12992, -123, 1239, -63346, 223};
    // vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 25, 26, 27, 28, 29, 39};
    // timsort(arr);
    // for (int i = 0; i < arr.size() - 1; i++) {cout << arr[i] << " ";}
    // vector<int> arr4;
    // for (int i = 0; i < 100; i++) arr4.push_back(i);
    // for (int i = 1000; i < 1100; i++) arr4.push_back(i);
    
    // cout << "Before merge: ";
    // for (int i = 0; i < 10; i++) cout << arr4[i] << " ";
    // cout << "xDxDxDxDxDxD";
    // for (int i = 100; i < 110; i++) cout << arr4[i] << " ";
    // cout << endl;

    //merge(arr4, 0, 199, 99);  // left=0, right=199, mid=99
    
    //cout << "After merge: ";
    //for (int i = 0; i < 100; i++) cout << arr4[i] << " ";
    //cout << endl;
    //timsort(arr4);
    //cout << "after ts" << endl;
    //for (int i = 0; i < 100; i++) cout << arr4[i] << " ";
    // не совсем понимаю нужен ли здесь пользовательский ввод, потому что в целом корректность сортировки трудно проверить, вводя вручную
    // сотни/тысячи/десятки тысяч элементов массива, поэтому в случае необходимости можно закомментироваь код выше и разкомментировать код ниже
    // vector<int> arr5;
    // for (int i = 0; i < 1000000; i++) {
    //     arr5.push_back(rand() % 100000 + 1);
    // }
    // timsort(arr5);
    // for (int i = 0; i < 1000000 - 1; i++) {
    //     cout << arr5[i] << " ";
    // }
    return 0;
}