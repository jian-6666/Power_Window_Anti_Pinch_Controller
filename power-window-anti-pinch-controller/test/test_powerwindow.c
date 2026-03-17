/**
 * @file test_powerwindow.c
 * @brief Power Window Anti-Pinch Controller - Main Module Tests
 * 
 * Unit tests for main PowerWindow control module.
 * 
 * @author Power Window Team
 * @version 1.0
 */

#include "PowerWindow.h"
#include "PowerWindow_Interface.h"
#include <stdio.h>
#include <assert.h>

/* ============================================================================
 * Test Functions
 * ============================================================================ */

void test_powerwindow_initialization(void)
{
    /* Initialize system */
    PowerWindow_Init();
    
    /* Verify initial state */
    PowerWindow_SystemStateType state = PowerWindow_GetCurrentState();
    assert(state == POWERWINDOW_STATE_IDLE);
    
    /* Verify initial position */
    PowerWindow_PercentType percent = PowerWindow_GetPositionPercent();
    assert(percent <= 100);
    
    /* Verify learning not completed initially */
    PowerWindow_BoolType learned = PowerWindow_IsLearnCompleted();
    assert(learned == POWERWINDOW_FALSE);
    
    printf("    PASS: test_powerwindow_initialization\n");
}

void test_powerwindow_position_conversion(void)
{
    /* Initialize system */
    PowerWindow_Init();
    
    /* Test position conversion at lower limit */
    PowerWindow_Interface_Mock_SetPosition(0);
    PowerWindow_MainFunction_10ms();
    PowerWindow_PercentType percent = PowerWindow_GetPositionPercent();
    assert(percent == 0);
    
    /* Test position conversion at upper limit */
    PowerWindow_Interface_Mock_SetPosition(4095);
    PowerWindow_MainFunction_10ms();
    percent = PowerWindow_GetPositionPercent();
    assert(percent == 100);
    
    /* Test position conversion at middle */
    PowerWindow_Interface_Mock_SetPosition(2048);
    PowerWindow_MainFunction_10ms();
    percent = PowerWindow_GetPositionPercent();
    assert(percent >= 45 && percent <= 55);  /* Allow some rounding */
    
    /* Test raw position retrieval */
    PowerWindow_PositionType rawPos = PowerWindow_GetRawPosition();
    assert(rawPos == 2048);
    
    printf("    PASS: test_powerwindow_position_conversion\n");
}
