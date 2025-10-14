#ifndef TARRAY_H
#define TARRAY_H

#include <iostream>
#include "number.h"

class TArray
{
private:
    number* data;
    size_t size;
public:
    TArray();
    TArray(size_t n, number value = 0);
    ~TArray();

    size_t getSize();
    number& operator[](size_t index);

    void resize(size_t newSize);
    void input(std::istream& in = std::cin);
    void output(std::ostream& out = std::cout);

    number Mean();
    number StandardDeviation();


    void ascendingSort();
    void descendingSort();
    void bubbleSort(bool direction);


};
#endif // TARRAY_H
