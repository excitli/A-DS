#include <iostream>
using namespace std;

// Сортировки:
// 1. Простые (n^2)
// 2. Эффективные (nlogn)

/*
 Существуют устойчивые сортирвовки (которые сохраяеют порядок, если у нас есть одинаковые элементы).
 Адаптивные сортировки - сортировки, способные работать быстрее на определенных входных данных.
 in-place сортировки (т.е. сортировки на месте). 
 У простых сортировок S(1) память, у эффективных вплоть до O(n)
 
 1. Селективная сортировка (выборкой)
 Есть входной массив -> проходим по массиву и находим самое большое значение за O(n). Ставим на 0 значение (первое, не занятое)
 Теперь проходим по тому же массиву с n - 1 элементами и так повторяем.

 void selectSort(size_t length, int* arr) {
    for (int i = 0; i < length; i++) {
        int index = i;
        for (int j = i; j < length; j++) {
            if (arr[j] < arr[i]) i = j;;
        }
        swap(arr[i], arr[index]);
    }
}

 2. Bubble Sort
 3. Insertion sort.
        5 || 9 7 3 2 4
        отсортированная часть слева
        выбираем 9 и ищем для нее значения слева. 5 9 || 7 3 2 4
        выбираем 7. 5 7 9 || 3 2 4 and so on


void insertionSort(size_t length, int* arr) {
    for (int i = 1; i < length; ++i) {
        int val = arr[i];
        for (int j = length - 1;j > i && arr[j] > value ; j--;) {
            arr[j] = arr[j - 1];
            arr[j - 1] = value;
        
        }
    
    }


}

Сортировку вставками можно улучшить бинарным поиском (т.е. мы ищем куда нужно поставить элемент с помощью бин. поиска)
Но, не работает, так как мы все равно двигаем элементы, а это n операций, хотя в текущих реалиях операции сдвига некоторых блоков массива быстрее.















*/


void selectSort(size_t length, int* arr) {
    for (int i = 0; i < length; i++) {
        int index = i;
        for (int j = i; j < length; j++) {
            if (arr[j] < arr[i]) i = j;;
        }
        swap(arr[i], arr[index]);
    }
}





int main() {
    int arr[] {1, 5, 2, 218, 192};
    selectSort(5, arr);
    for (int i = 0; i < 5; ++i) cout << arr[i] << endl;
    cout << "dsfs" << endl;
    return 0;
}