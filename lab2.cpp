#include <iostream>
using namespace std;
#include <cstdlib>


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



void insertionSort(std::vector<int>& arr, int left, int right) {
    for (int i = left + 1;i <= right; ++i) {
        int curVal = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > curVal) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = curVal;
    }
}


int gallopLeft(int key, vector<int>& arr, int startVal, int length) {
    int lastOffset = 0;
    int offset = -1;
    if (key < arr[startVal]) {
        int maxOffset = length;
        while (offset < maxOffset && key > arr[startVal + offset]) {
            lastOffset = offset;
            offset = (offset << 1) + 1;
            if (offset < 0) offset = maxOffset;
        }
        if (offset < maxOffset) offset = maxOffset;
        lastOffset += startVal;
        offset += startVal;
    }
    else {
        offset = startVal;
        lastOffset = startVal - 1;
    }
    while (lastOffset + 1 < offset) {
        int mid = lastOffset + ((offset - lastOffset) >> 1);
        if (key > arr[mid]) lastOffset = mid;
        else offset = mid;
    }
    cout << "gallop left!";
    return offset;
}


int gallopRight(int key, vector<int>& arr, int startVal, int length) {
    int lastOffset = 0, offset = 1;
    if (key < arr[startVal]) {
        return startVal;
    }
    int maxOffset = length;
    while (offset < maxOffset && key >= arr[startVal + offset]) {
        lastOffset = offset;
        offset = (offset << 1) + 1;
        if (offset <=0) offset = maxOffset;
    }
    if (offset > maxOffset) offset = maxOffset;
    lastOffset += startVal;
    offset += startVal;
    while (lastOffset + 1 < offset) {
        int mid = lastOffset + ((offset - lastOffset) >> 1);
        if (key >= arr[mid]) {
            lastOffset = mid;
        }
        else {
            offset = mid;
        }
    }
    cout << "Gallop right!";
    return offset;
}



void merge(vector<int>&arr, int left, int right, int mid) {
    int len1 = mid - left + 1;
    int len2 = right - mid;
    vector<int> leftArr(len1);
    vector<int> rightArr(len2);
    for (int i = 0; i < len1; i++) leftArr[i] = arr[left + i];
    for(int i = 0; i <len2; i++) rightArr[i] = arr[mid + i + 1];
    int i = 0, j = 0, k = left;
    int gth = 10;
    int countLeft = 0, countRight = 0;

    while (i < len1 && j < len2) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k++] = leftArr[i++];
            countLeft++;
            countRight = 0;
        }
        else {
            arr[k++] = rightArr[j++];
            countRight++;
            countLeft = 0;
        }
        if (countLeft >= gth) {
            cout << "left gallop activated in merge" << endl;
            int newPos = gallopLeft(rightArr[j], leftArr, i, len1 - i);
            while (j < newPos) arr[k++] = rightArr[j++];
            countLeft = 0;
        }
        else if (countRight >= gth) {
            cout << "right gallop activated in merge" << endl;
            int newPos = gallopLeft(leftArr[i], rightArr, j, len2 - j);
            while (i < newPos) arr[k++] = leftArr[i++];
            countRight = 0;
        }
    }
    while (i < len1) {
        arr[k++] = leftArr[i++];
    }
    while (j < len2) {
        arr[k++] = rightArr[j++];
    }
}




void mergeTop(vector<int>& arr, vector<Run>& runStack, int i) {
    int left = runStack[i].startPos;
    int mid = runStack[i].startPos  + runStack[i].lengthOfRun - 1;
    int right = runStack[i + 1].lengthOfRun -1 + runStack[i + 1].startPos;
    merge(arr, left, right, mid);
    runStack[i].lengthOfRun += runStack[i + 1].lengthOfRun;
    runStack.erase(runStack.begin() + i + 1);

}


void mergeRun(vector<int>&arr, vector<Run>& runStack) {
    while (runStack.size() > 1) {
        int n = runStack.size();
        bool merged = false;

        if (n >= 3 && runStack[n - 3].lengthOfRun <= runStack[n - 2].lengthOfRun + runStack[n-1].lengthOfRun) {
            if (runStack[n-3].lengthOfRun < runStack[n-1].lengthOfRun) mergeTop(arr, runStack, n - 3);
            else mergeTop(arr, runStack, n - 2);
            merged = true;
        }
        else if (runStack[n - 2].lengthOfRun <= runStack[n-1].lengthOfRun) {
            mergeTop(arr, runStack, n - 2);
            merged = true;
        }
        if (!merged) break;
    }
}

void timsort(vector<int>& arr) {
    int n = arr.size();
    if (n < 2) return;
    int minRun = getMinRunSize(n);
    vector <Run> runStack;
    
    int i = 0;
    while (i < n) {
        int runStart = i;
        int runEnd = i + 1;

        if (runEnd < n && arr[runEnd] < arr[runEnd - 1]) {
            while (runEnd < n && arr[runEnd] < arr[runEnd - 1]) runEnd++;
            reverse(arr.begin() + runStart, arr.begin() + runEnd);
        }
        else {
            while (runEnd < n && arr[runEnd] >= arr[runEnd - 1]) runEnd++;
        }
        int runLength = runEnd - runStart;
        if (runLength < minRun) {
            int forcedEnd = min(runStart + minRun, n);
            insertionSort(arr, runStart, forcedEnd - 1);
            runLength = forcedEnd - runStart;
            runEnd = forcedEnd;
        }
        runStack.push_back({runStart, runLength});
        mergeRun(arr, runStack);
        i = runEnd;
    }

    while(runStack.size() > 1) {
        mergeTop(arr, runStack, runStack.size() - 2);
    }
}


int main() {
    vector<int> arr1;
    for (int i = 0; i < 50; i++) arr1.push_back(i);
    for (int i = 1000; i < 1050; i++) arr1.push_back(i);
    
    timsort(arr1);
    cout << "Result first 10: ";
    for (int i = 0; i < 10; i++) cout << arr1[i] << " ";
    cout << "\n\n";
    //vector<int> arr = {1, 2, 5, 19, 192, 129, 15, 1, 1, 2, 3, 4, 5, 6, 7, 8, 12992, -123, 1239, -63346, 223};
    vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 25, 26, 27, 28, 29, 39};
    timsort(arr);
    for (int i = 0; i < arr.size() - 1; i++) {cout << arr[i] << " ";}
    vector<int> arr4;
    for (int i = 0; i < 100; i++) arr4.push_back(i);
    for (int i = 1000; i < 1100; i++) arr4.push_back(i);
    
    cout << "Before merge: ";
    for (int i = 0; i < 10; i++) cout << arr4[i] << " ";
    cout << "xDxDxDxDxDxD";
    for (int i = 100; i < 110; i++) cout << arr4[i] << " ";
    cout << endl;

    //merge(arr4, 0, 199, 99);  // left=0, right=199, mid=99
    
    //cout << "After merge: ";
    //for (int i = 0; i < 100; i++) cout << arr4[i] << " ";
    //cout << endl;
    timsort(arr4);
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