#include "tarray.h"
#include <cmath>
#include <algorithm>
using namespace std;

TArray::TArray() : data(nullptr), size(0) {}

TArray::TArray(size_t n, number value) : size(n)
{
    data = new number[size];
    for (size_t i = 0; i < size; i++) {
        data[i] = value;
    }
}


TArray::~TArray() {
    delete[] data;
}

size_t TArray::getSize() {
    return size;
}

number& TArray::operator[](size_t index) {
    return data[index];
}


void TArray::resize(size_t newSize) {
    number* newData = new number[newSize];
    size_t minSize = (size < newSize) ? size : newSize; // проверяем что неовый размер больше предыдущего, в противном случае, ресайз делает массив такого же размера
    for (size_t i = 0; i < minSize; i++) {
        newData[i] = data[i];
    }
    for (size_t j = minSize; j < newSize; j++) {
        newData[j] = 0;
    }
    delete[] data;
    data = newData;
    size = newSize;
}

void TArray::input(std::istream& in) {
    for (size_t i = 0; i < size; i++) {
        in >> data[i];
    }
}

void TArray::output(std::ostream& out) {
    for (size_t i = 0; i < size; i++) {
        out << data[i] << " ";
    }
    out << std::endl;
}


number TArray::Mean() {
    if (size == 0) return 0;
    number sum = 0;

    for (size_t i = 0; i < size; i++) {
        sum += data[i];
    }
    return sum / size;
}

number TArray::StandardDeviation() {
    if (size < 2) return 0;
    number quadraticSum = 0;
    number mean = Mean();

    for (size_t i = 0; i < size; i++) {
        quadraticSum += (mean - data[i]) * (mean - data[i]);
    }
    return std::sqrt(quadraticSum / (size - 1));
}


void TArray::bubbleSort(bool ascending) {
    if (size <= 1) return;

    for (size_t i = 0; i < size - 1; i++) {
        bool swapped = false;
        for (size_t j = 0; j < size - i - 1; j++) {
            bool needSwap = ascending ? (data[j] > data[j + 1]) : (data[j] < data[j + 1]);

            if (needSwap) {
                std::swap(data[j], data[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

void TArray::ascendingSort() {
    bubbleSort(true);
}

void TArray::descendingSort() {
    bubbleSort(false);
}








