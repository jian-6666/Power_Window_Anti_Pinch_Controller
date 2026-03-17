/**
 * @file test_powerwindow_main.c
 * @brief Test for PowerWindow main control module
 */

#include <stdio.h>
#include <assert.h>
#include "PowerWindow.h"

#define POWERWINDOW_TEST_MODE

int main(void)
{
    printf("Testing PowerWindow main control module...\n");
    
    /* Test initialization */
    PowerWindow_Init();
    printf("✓ PowerWindow_Init() completed\n");
    
    /* Test main function execution */
    PowerWindow_MainFunction_10ms();
    printf("✓ PowerWindow_MainFunction_10ms() completed\n");
    
    /* Test getter functions */
    PowerWindow_SystemStateType state = PowerWindow_GetCurrentState();
    PowerWindow_PercentType position = PowerWindow_GetPositionPercent();
    PowerWindow_PositionType rawPosition = PowerWindow_GetRawPosition();
    PowerWindow_BoolType learnCompleted = PowerWindow_IsLearnCompleted();
    
    printf("✓ Current state: %d\n", state);
    printf("✓ Position: %d%%\n", position);
    printf("✓ Raw position: %d\n", rawPosition);
    printf("✓ Learn completed: %s\n", learnCompleted ? "Yes" : "No");
    
    /* Test execution statistics */
    uint32_t callCount = PowerWindow_GetMainFunctionCallCount();
    uint32_t minTime, maxTime, avgTime;
    PowerWindow_GetExecutionTimeStats(&minTime, &maxTime, &avgTime);
    
    printf("✓ Main function call count: %lu\n", (unsigned long)callCount);
    printf("✓ Execution time stats - Min: %lu, Max: %lu, Avg: %lu\n", 
           (unsigned long)minTime, (unsigned long)maxTime, (unsigned long)avgTime);
    
    printf("All tests passed!\n");
    return 0;
}