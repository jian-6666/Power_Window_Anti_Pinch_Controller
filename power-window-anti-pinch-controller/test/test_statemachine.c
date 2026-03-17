/**
 * @file test_statemachine.c
 * @brief Power Window Anti-Pinch Controller - State Machine Tests
 * 
 * Unit tests for state machine transitions and logic.
 * 
 * @author Power Window Team
 * @version 1.0
 */

#include "PowerWindow_StateMachine.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

/* ============================================================================
 * Test Functions
 * ============================================================================ */

void test_statemachine_transitions(void)
{
    /* Initialize state machine */
    PowerWindow_StateMachine_Init();
    assert(PowerWindow_StateMachine_GetState() == POWERWINDOW_STATE_UNINITIALIZED);
    
    /* Test transition to IDLE */
    PowerWindow_StateMachine_RequestTransition(POWERWINDOW_STATE_IDLE);
    assert(PowerWindow_StateMachine_GetState() == POWERWINDOW_STATE_IDLE);
    
    /* Test CanAcceptCommand in IDLE state */
    assert(PowerWindow_StateMachine_CanAcceptCommand() == POWERWINDOW_TRUE);
    
    /* Test IsMoving in IDLE state */
    assert(PowerWindow_StateMachine_IsMoving() == POWERWINDOW_FALSE);
    
    /* Test transition to MANUAL_UP */
    PowerWindow_StateMachine_RequestTransition(POWERWINDOW_STATE_MANUAL_UP);
    assert(PowerWindow_StateMachine_GetState() == POWERWINDOW_STATE_MANUAL_UP);
    assert(PowerWindow_StateMachine_IsMoving() == POWERWINDOW_TRUE);
    
    /* Test transition back to IDLE */
    PowerWindow_StateMachine_RequestTransition(POWERWINDOW_STATE_IDLE);
    assert(PowerWindow_StateMachine_GetState() == POWERWINDOW_STATE_IDLE);
    
    /* Test transition to MANUAL_DOWN */
    PowerWindow_StateMachine_RequestTransition(POWERWINDOW_STATE_MANUAL_DOWN);
    assert(PowerWindow_StateMachine_GetState() == POWERWINDOW_STATE_MANUAL_DOWN);
    
    /* Test transition to IDLE */
    PowerWindow_StateMachine_RequestTransition(POWERWINDOW_STATE_IDLE);
    assert(PowerWindow_StateMachine_GetState() == POWERWINDOW_STATE_IDLE);
    
    /* Test transition to AUTO_UP */
    PowerWindow_StateMachine_RequestTransition(POWERWINDOW_STATE_AUTO_UP);
    assert(PowerWindow_StateMachine_GetState() == POWERWINDOW_STATE_AUTO_UP);
    
    /* Test transition to IDLE */
    PowerWindow_StateMachine_RequestTransition(POWERWINDOW_STATE_IDLE);
    assert(PowerWindow_StateMachine_GetState() == POWERWINDOW_STATE_IDLE);
    
    /* Test transition to AUTO_DOWN */
    PowerWindow_StateMachine_RequestTransition(POWERWINDOW_STATE_AUTO_DOWN);
    assert(PowerWindow_StateMachine_GetState() == POWERWINDOW_STATE_AUTO_DOWN);
    
    /* Test transition to IDLE */
    PowerWindow_StateMachine_RequestTransition(POWERWINDOW_STATE_IDLE);
    assert(PowerWindow_StateMachine_GetState() == POWERWINDOW_STATE_IDLE);
    
    /* Test transition to ANTI_PINCH (from MANUAL_UP) */
    PowerWindow_StateMachine_RequestTransition(POWERWINDOW_STATE_MANUAL_UP);
    PowerWindow_StateMachine_RequestTransition(POWERWINDOW_STATE_ANTI_PINCH);
    assert(PowerWindow_StateMachine_GetState() == POWERWINDOW_STATE_ANTI_PINCH);
    
    /* Test transition to IDLE from ANTI_PINCH */
    PowerWindow_StateMachine_RequestTransition(POWERWINDOW_STATE_IDLE);
    assert(PowerWindow_StateMachine_GetState() == POWERWINDOW_STATE_IDLE);
    
    /* Test transition to FAULT from any state */
    PowerWindow_StateMachine_RequestTransition(POWERWINDOW_STATE_FAULT);
    assert(PowerWindow_StateMachine_GetState() == POWERWINDOW_STATE_FAULT);
    
    /* Test CanAcceptCommand in FAULT state */
    assert(PowerWindow_StateMachine_CanAcceptCommand() == POWERWINDOW_FALSE);
    
    /* Test transition from FAULT to IDLE */
    PowerWindow_StateMachine_RequestTransition(POWERWINDOW_STATE_IDLE);
    assert(PowerWindow_StateMachine_GetState() == POWERWINDOW_STATE_IDLE);
    
    /* Test invalid transition (same state) */
    PowerWindow_StateMachine_RequestTransition(POWERWINDOW_STATE_IDLE);
    assert(PowerWindow_StateMachine_GetState() == POWERWINDOW_STATE_IDLE);
    
    /* Test invalid transition (MANUAL_DOWN to MANUAL_UP) */
    PowerWindow_StateMachine_RequestTransition(POWERWINDOW_STATE_MANUAL_DOWN);
    PowerWindow_StateMachine_RequestTransition(POWERWINDOW_STATE_MANUAL_UP);
    assert(PowerWindow_StateMachine_GetState() == POWERWINDOW_STATE_MANUAL_DOWN);
    
    /* Test GetPreviousState */
    PowerWindow_StateMachine_Init();
    PowerWindow_StateMachine_RequestTransition(POWERWINDOW_STATE_IDLE);
    PowerWindow_StateMachine_RequestTransition(POWERWINDOW_STATE_MANUAL_UP);
    assert(PowerWindow_StateMachine_GetPreviousState() == POWERWINDOW_STATE_IDLE);
    
    printf("    PASS: test_statemachine_transitions\n");
}
