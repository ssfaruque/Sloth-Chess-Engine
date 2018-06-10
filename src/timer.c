#include "timer.h"

#include <time.h>

double start;

void startTimer()
{
    start = clock();
}


double getElapsedTime()
{
    return (double)(clock() - start) / CLOCKS_PER_SEC;
}