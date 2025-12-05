#include <iostream>
using namespace std;
#include <string>
#include "intarray.h"



struct Edge {
    int u, v, weight;
    string u_name, v_name;

    Edge() : u(-1), v(-1), weight(0) {}
    Edge(int _u, int _v, int _w, string _un, string _vn) 
        : u(_u), v(_v), weight(_w), u_name(_un), v_name(_vn) {}

    bool operator<(const Edge& other) { return weight < other.weight; }
    bool operator>(const Edge& other) { return weight > other.weight; }
    bool operator<=(const Edge& other) { return weight <= other.weight; }
    bool operator>=(const Edge& other) { return weight >= other.weight; }
    bool operator==(const Edge& other) { return weight == other.weight; }
    bool operator!=(const Edge& other) { return weight != other.weight; }
};


class EdgeArray {
private:
    Edge* data;
    int capacity;
    int _size;

    void resize(int newCapacity) {
        Edge* newData = new Edge[newCapacity];
        for (int i = 0; i < _size; ++i)
            newData[i] = data[i];
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    EdgeArray() {
        capacity = 4;
        _size = 0;
        data = new Edge[capacity];
    }

    ~EdgeArray() {
        delete[] data;
    }

    void push_back(Edge val) {
        if (_size >= capacity)
            resize(capacity * 2);
        data[_size++] = val;
    }

    Edge& operator[](int index) {
        if (index < 0 || index >= _size)
            throw runtime_error("Index out of range");
        return data[index];
    }

    int size() const { return _size; }

    void reverse(int left, int right) {
        while (left < right) {
            Edge tmp = data[left];
            data[left] = data[right];
            data[right] = tmp;
            left++;
            right--;
        }
    }
};


int minVal(int a, int b) {
    return (a < b) ? a : b;
};

int getMinRunSize(int n) { 
    int remainderBit = 0;
    while (n >= 64) {
        remainderBit |= (n & 1);
        n >>= 1;
    }
    return n + remainderBit;
};

void insertionSort(EdgeArray& arr, int left, int right) {
    for (int i = left + 1; i <= right; ++i) {
        Edge curVal = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > curVal) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = curVal;
    }
};

int gallopLeft(Edge key, EdgeArray& arr, int startVal, int length) {
    int lastOffset = 0;
    int offset = 1;

    if (key > arr[startVal]) {
        int maxOffset = length;
        while (offset < maxOffset && key > arr[startVal + offset]) {
            lastOffset = offset;
            offset = (offset << 1) + 1; 
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
};

int gallopRight(Edge key, EdgeArray& arr, int startVal, int length) {
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
};

void merge(EdgeArray& arr, int left, int right, int mid) {
    int len1 = mid - left + 1;
    int len2 = right - mid;
    EdgeArray leftArr;
    EdgeArray rightArr;

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
            int newPos = gallopLeft(rightArr[j], leftArr, i, len1 - i);
            while (i < newPos && i < len1)
                arr[k++] = leftArr[i++];
            countLeft = 0;
        } else if (countRight >= gth) {
            int newPos = gallopRight(leftArr[i], rightArr, j, len2 - j);
            while (j < newPos && j < len2)
                arr[k++] = rightArr[j++];
            countRight = 0;
        }
    }

    while (i < len1) arr[k++] = leftArr[i++];
    while (j < len2) arr[k++] = rightArr[j++];
};

void mergeTop(EdgeArray& arr, IntArray& runStackStart, IntArray& runStackLen, int i) {
    int left = runStackStart[i];
    int mid = runStackStart[i] + runStackLen[i] - 1;
    int right = runStackStart[i + 1] + runStackLen[i + 1] - 1;

    merge(arr, left, right, mid);
    runStackLen[i] += runStackLen[i + 1];
    runStackStart.erase(i + 1);
    runStackLen.erase(i + 1);
}

void mergeRun(EdgeArray& arr, IntArray& runStackStart, IntArray& runStackLen) {
    while (runStackLen.size() > 1) {
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

void timsort(EdgeArray& arr) {
    int n = arr.size();
    if (n < 2) return;
    int minRun = getMinRunSize(n);
    
    bool isSorted = true;
    for (int i = 1; i < arr.size(); ++i) {
        if (arr[i] < arr[i - 1]) {
            isSorted = false;
            break;
        }
    }
    if (isSorted) return;

    IntArray runStart; 
    IntArray runLen;   

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
            int forcedEnd = minVal(runStartPos + minRun, n);
            insertionSort(arr, runStartPos, forcedEnd - 1);
            runLength = forcedEnd - runStartPos;    
            runEnd = forcedEnd;
        }
        runStart.push_back(runStartPos);
        runLen.push_back(runLength);
        mergeRun(arr, runStart, runLen);
        i = runEnd;
    }

    while (runLen.size() > 1) {
        mergeTop(arr, runStart, runLen, runLen.size() - 2);
    }
}
