#include "tapplication.h"
#include <iostream>
using namespace std;

TApplication::TApplication() : tarray(0) {}
void TApplication::menu() {
    std::cout << "Commands" << std::endl;
    std::cout << "1. Input an array" << std::endl;
    std::cout << "2. Calc mean and SKO" << std::endl;
    std::cout << "3. Sort" << std::endl;
    std::cout << "4. Resize" << std::endl;
    std::cout << "5. Change elem" << std::endl;
    std::cout << "6. Show array" << std::endl;
    std::cout << "0. Exit" << std::endl;
}

void TApplication::exec() {
    int choice = -1;
    while (choice != 0) {
        menu();
        std::cout << "Command" << ">>> " << endl;
        std::cin >> choice;

        switch (choice) {
            case 1:
            {
                system("cls");
                handleInput();
                system("pause");
                break;
            }
            case 2:
            {
                system("cls");
                handleCalc();
                system("pause");
                break;
            }
            case 3:
            {
                system("cls");
                sortArray();
                system("pause");
                break;
            }
            case 4:
            {
                system("cls");
                resizeArray();
                system("pause");
                break;
            }
            case 5:
            {
                system("cls");
                changeArrayElement();
                system("pause");
                break;
            }
            case 6:
            {
                system("cls");
                display();
                system("pause");
                break;
            }
            case 0:
            {
                system("cls");
                std::cout << "Exiting" << std::endl;
                break;
            }
            default: {
                system("cls");
                std::cout << "Incorrect command" << std::endl;
                break;
            }
        }
        std::cout << std::endl;
    }
}


void TApplication::handleInput() {
    size_t size;
    std::cout << "Input the size of the array" << std::endl;
    std::cin >> size;
    tarray.resize(size);
    std::cout << "Input " << size << " array elements" << endl;
    tarray.input();
    std::cout << "The array has been created" << std::endl;
    return;
}


void TApplication::handleCalc()
{
    if (tarray.getSize() == 0) {
        std::cout << "Empty list" << endl;
        return;
    }
    number mean = tarray.Mean();
    number sko = tarray.StandardDeviation();
    std::cout << "Average: " << mean << std::endl;
    cout << "SKO: " << sko << endl;
}

void TApplication::sortArray()
{
    if (tarray.getSize() == 0)
    {
        cout << "EMpty" << endl;
        return;
    }
    int choice = -1;
    cout << "1. Ascending" << endl;
    cout << "2.Descdending" << endl;
    cout << ">>> " << endl;
    cin >> choice;

    switch (choice)
    {
        case 1: {
            tarray.ascendingSort();
            cout << "Sorted" << endl;
            break;
        }
        case 2: {
            tarray.descendingSort();
            cout << "sorted" << endl;
            break;
        }
        default:
            cout << "Wrong input" << endl;
            break;
    }
}

void TApplication::resizeArray() {
    size_t newSize;
    cout << "Input the new size" << endl;
    cin >> newSize;
    tarray.resize(newSize);
    cout << "Done" << endl;
}

void TApplication::changeArrayElement()
{
    size_t currentSize = tarray.getSize();
    if (currentSize == 0) {
        cout << "Empty" << endl;
        return;
    }
    size_t index;
    number value;
    cout << "Input the index " << "0-" << currentSize - 1 << endl;
    cin >> index;

    if (index < 0 || index >= currentSize) {
        cout << "Wrong index" << endl;
        return;
    }

    cout << "Curent value" << tarray[index] << endl;
    cout << "Input new value" << endl;
    cin >> value;
    tarray[index] = value;
    cout << "Changed";
    //handleDisplay(); !!!!!!!!!!!! НЕ ЗАБЫТЬ
}


void TApplication::display() {
    if (tarray.getSize() <= 0) {
        cout << "Empty" << endl;
        return;
    }
    cout << "Array: " << endl;
    tarray.output();
}
