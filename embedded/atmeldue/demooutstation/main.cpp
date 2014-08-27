/*
 * demooutstation.cpp
 *
 * Created: 8/27/2014 7:55:11 AM
 *  Author: jadamcrain
 */ 

#include "sam.h"

#include "AVRExecutor.h"

/**
 * \brief Application entry point.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	
	AVRExecutor exe;
	
	auto action = [](){};
	
	exe.PostLambda(action);

    while (1) 
    {
        //TODO:: Please write your application code 
    }
}
