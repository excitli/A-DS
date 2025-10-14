#ifndef TAPPLICATION_H
#define TAPPLICATION_H
#include "tarray.h"

class TApplication
{
private:
    TArray tarray;
public:
    TApplication();
    void exec();
    void menu();
    void handleInput();
    void handleCalc();
    void sortArray();
    void resizeArray();
    void changeArrayElement();
    void display();
};


#endif // TAPPLICATION_H
