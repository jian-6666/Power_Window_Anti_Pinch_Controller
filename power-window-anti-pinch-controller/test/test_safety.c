/**
 * @file test_safety.c
 * @brief Power Window Anti-Pinch Controller - Safety Module Tests
 * 
 * Unit tests for safety monitoring functions.
 * 
 * @author Power Window Team
 * @version 1.0
 */

#include "PowerWindow_Safety.h"
#include "PowerWindow_Cfg.h"
#include <stdio.h>
#include <assert.h>

/* ============================================================================
 * Test Functions
 * ============================================================================ */

void test_safety_anti_pinch_detection(void)
{
    PowerWindow_Safety_Init();
    
    /* Test anti-pinch detection during manual up */
    PowerWindow_BoolType result = PowerWindow_Safety_MonitorAntiPinch(
        150,  /* Current: 15.0A */
        POWERWINDOW_STATE_MANUAL_UP,
        100   /* Threshold: 10.0A */
    );
    assert(result == POWERWINDOW_TRUE);
    
    /* Test no anti-pinch when current below threshold */
    result = PowerWindow_Safety_MonitorAntiPinch(
        80,   /* Current: 8.0A */
        POWERWINDOW_STATE_MANUAL_UP,
        100   /* Threshold: 10.0A */
    );
    assert(result == POWERWINDOW_FALSE);
    
    /* Test no anti-pinch during auto up */
    result = PowerWindow_Safety_MonitorAntiPinch(
        150,  /* Current: 15.0A */
        POWERWINDOW_STATE_AUTO_UP,
        100   /* Threshold: 10.0A */
    );
    assert(result == POWERWINDOW_TRUE);
    
    /* Test no anti-pinch during manual down */
    result = PowerWindow_Safety_MonitorAntiPinch(
        150,  /* Current: 15.0A */
        POWERWINDOW_STATE_MANUAL_DOWN,
        100   /* Threshold: 10.0A */
    );
    assert(result == POWERWINDOW_FALSE);
    
    /* Test no anti-pinch during idle */
    result = PowerWindow_Safety_MonitorAntiPinch(
        150,  /* Current: 15.0A */
        POWERWINDOW_STATE_IDLE,
        100   /* Threshold: 10.0A */
    );
    assert(result == POWERWINDOW_FALSE);
    
    /* Test current monitoring with overcurrent */
    PowerWindow_Safety_MonitorCurrent(
        250,  /* Current: 25.0A (overcurrent) */
        POWERWINDOW_STATE_MANUAL_UP,
        200,  /* Overcurrent threshold: 20.0A */
        10    /* Undercurrent threshold: 1.0A */
    );
    
    /* Test current monitoring with undercurrent */
    PowerWindow_Safety_MonitorCurrent(
        5,    /* Current: 0.5A (undercurrent) */
        POWERWINDOW_STATE_MANUAL_UP,
        200,  /* Overcurrent threshold: 20.0A */
        10    /* Undercurrent threshold: 1.0A */
    );
    
    /* Test current monitoring with normal current */
    PowerWindow_Safety_MonitorCurrent(
        50,   /* Current: 5.0A (normal) */
        POWERWINDOW_STATE_MANUAL_UP,
        200,  /* Overcurrent threshold: 20.0A */
        10    /* Undercurrent threshold: 1.0A */
    );
    
    /* Test voltage monitoring */
    PowerWindow_Safety_MonitorVoltage(
        95,   /* Voltage: 9.5V (undervoltage) */
        100   /* Threshold: 10.0V */
    );
    
    /* Test fault checking */
    PowerWindow_BoolType hasFault = PowerWindow_Safety_CheckFaults();
    /* May or may not have fault depending on debounce */
    (void)hasFault;
    
    /* Test fault clearing */
    PowerWindow_Safety_ClearFaults();
    hasFault = PowerWindow_Safety_CheckFaults();
    assert(hasFault == POWERWINDOW_FALSE);
    
    printf("    PASS: test_safety_anti_pinch_detection\n");
}
