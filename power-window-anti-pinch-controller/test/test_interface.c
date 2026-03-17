/**
 * @file test_interface.c
 * @brief Power Window Anti-Pinch Controller - Interface Module Tests
 * 
 * Unit tests for hardware interface abstraction and mock operations.
 * 
 * @author Power Window Team
 * @version 1.0
 */

#include "PowerWindow_Interface.h"
#include <stdio.h>
#include <assert.h>

/* ============================================================================
 * Test Functions
 * ============================================================================ */

void test_interface_mock_operations(void)
{
    /* Initialize interface */
    PowerWindow_Interface_Init();
    
    /* Test button mock operations */
    PowerWindow_Interface_Mock_SetButton(POWERWINDOW_BUTTON_UP);
    assert(PowerWindow_Interface_ReadButton() == POWERWINDOW_BUTTON_UP);
    
    PowerWindow_Interface_Mock_SetButton(POWERWINDOW_BUTTON_DOWN);
    assert(PowerWindow_Interface_ReadButton() == POWERWINDOW_BUTTON_DOWN);
    
    PowerWindow_Interface_Mock_SetButton(POWERWINDOW_BUTTON_RELEASED);
    assert(PowerWindow_Interface_ReadButton() == POWERWINDOW_BUTTON_RELEASED);
    
    /* Test position mock operations */
    PowerWindow_Interface_Mock_SetPosition(1000);
    assert(PowerWindow_Interface_ReadPosition() == 1000);
    
    PowerWindow_Interface_Mock_SetPosition(2048);
    assert(PowerWindow_Interface_ReadPosition() == 2048);
    
    PowerWindow_Interface_Mock_SetPosition(4095);
    assert(PowerWindow_Interface_ReadPosition() == 4095);
    
    /* Test current mock operations */
    PowerWindow_Interface_Mock_SetCurrent(50);
    assert(PowerWindow_Interface_ReadCurrent() == 50);
    
    PowerWindow_Interface_Mock_SetCurrent(150);
    assert(PowerWindow_Interface_ReadCurrent() == 150);
    
    /* Test voltage mock operations */
    PowerWindow_Interface_Mock_SetVoltage(120);
    assert(PowerWindow_Interface_ReadVoltage() == 120);
    
    PowerWindow_Interface_Mock_SetVoltage(95);
    assert(PowerWindow_Interface_ReadVoltage() == 95);
    
    /* Test motor control */
    PowerWindow_Interface_SetMotor(POWERWINDOW_MOTOR_UP);
    assert(PowerWindow_Interface_Mock_GetMotor() == POWERWINDOW_MOTOR_UP);
    
    PowerWindow_Interface_SetMotor(POWERWINDOW_MOTOR_DOWN);
    assert(PowerWindow_Interface_Mock_GetMotor() == POWERWINDOW_MOTOR_DOWN);
    
    PowerWindow_Interface_SetMotor(POWERWINDOW_MOTOR_STOP);
    assert(PowerWindow_Interface_Mock_GetMotor() == POWERWINDOW_MOTOR_STOP);
    
    /* Test reset */
    PowerWindow_Interface_Mock_Reset();
    assert(PowerWindow_Interface_ReadButton() == POWERWINDOW_BUTTON_RELEASED);
    assert(PowerWindow_Interface_ReadPosition() == 2048);
    assert(PowerWindow_Interface_ReadCurrent() == 50);
    assert(PowerWindow_Interface_ReadVoltage() == 120);
    assert(PowerWindow_Interface_Mock_GetMotor() == POWERWINDOW_MOTOR_STOP);
    
    printf("    PASS: test_interface_mock_operations\n");
}
