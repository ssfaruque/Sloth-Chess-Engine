//
//  timer.c
//  ChessEngine
//
//  Created by Sahil Faruque on 5/18/18.
//  Copyright © 2018 Sahil Faruque. All rights reserved.
//

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