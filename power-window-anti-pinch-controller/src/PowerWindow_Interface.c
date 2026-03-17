/**
 * @file PowerWindow_Interface.c
 * @brief Power Window Anti-Pinch Controller - Hardware Interface Implementation
 * 
 * This module implements hardware abstraction for input/output operations.
 * In test mode, it uses mock implementations for testing purposes.
 * 
 * @author Power Window Team
 * @version 1.0
 */

#include "PowerWindow_Interface.h"

/* ============================================================================
 * Mock Data Structure (Test Mode)
 * ============================================================================ */

#ifdef POWERWINDOW_TEST_MODE

typedef struct {
    PowerWindow_ButtonStateType mockButtonState;
    PowerWindow_PositionType mockPosition;
    PowerWindow_CurrentType mockCurrent;
    PowerWindow_VoltageType mockVoltage;
    PowerWindow_MotorDirectionType mockMotorDirection;
} PowerWindow_MockDataType;

static PowerWindow_MockDataType mockData = {
    .mockButtonState = POWERWINDOW_BUTTON_RELEASED,
    .mockPosition = 2048,
    .mockCurrent = 50,
    .mockVoltage = 120,
    .mockMotorDirection = POWERWINDOW_MOTOR_STOP
};

#endif /* POWERWINDOW_TEST_MODE */

/* ============================================================================
 * Function Implementations
 * ============================================================================ */

void PowerWindow_Interface_Init(void)
{
#ifdef POWERWINDOW_TEST_MODE
    PowerWindow_Interface_Mock_Reset();
#endif
}

PowerWindow_ButtonStateType PowerWindow_Interface_ReadButton(void)
{
#ifdef POWERWINDOW_TEST_MODE
    return mockData.mockButtonState;
#else
    /* In production, read from actual hardware */
    return POWERWINDOW_BUTTON_RELEASED;
#endif
}

PowerWindow_PositionType PowerWindow_Interface_ReadPosition(void)
{
#ifdef POWERWINDOW_TEST_MODE
    return mockData.mockPosition;
#else
    /* In production, read from actual hardware */
    return 2048;
#endif
}

PowerWindow_CurrentType PowerWindow_Interface_ReadCurrent(void)
{
#ifdef POWERWINDOW_TEST_MODE
    return mockData.mockCurrent;
#else
    /* In production, read from actual hardware */
    return 50;
#endif
}

PowerWindow_VoltageType PowerWindow_Interface_ReadVoltage(void)
{
#ifdef POWERWINDOW_TEST_MODE
    return mockData.mockVoltage;
#else
    /* In production, read from actual hardware */
    return 120;
#endif
}

void PowerWindow_Interface_SetMotor(PowerWindow_MotorDirectionType direction)
{
#ifdef POWERWINDOW_TEST_MODE
    mockData.mockMotorDirection = direction;
#else
    /* In production, set actual hardware */
    (void)direction;
#endif
}

/* ============================================================================
 * Mock Interface Functions (Test Mode Only)
 * ============================================================================ */

#ifdef POWERWINDOW_TEST_MODE

void PowerWindow_Interface_Mock_SetButton(PowerWindow_ButtonStateType state)
{
    mockData.mockButtonState = state;
}

void PowerWindow_Interface_Mock_SetPosition(PowerWindow_PositionType position)
{
    mockData.mockPosition = position;
}

void PowerWindow_Interface_Mock_SetCurrent(PowerWindow_CurrentType current)
{
    mockData.mockCurrent = current;
}

void PowerWindow_Interface_Mock_SetVoltage(PowerWindow_VoltageType voltage)
{
    mockData.mockVoltage = voltage;
}

PowerWindow_MotorDirectionType PowerWindow_Interface_Mock_GetMotor(void)
{
    return mockData.mockMotorDirection;
}

void PowerWindow_Interface_Mock_Reset(void)
{
    mockData.mockButtonState = POWERWINDOW_BUTTON_RELEASED;
    mockData.mockPosition = 2048;
    mockData.mockCurrent = 50;
    mockData.mockVoltage = 120;
    mockData.mockMotorDirection = POWERWINDOW_MOTOR_STOP;
}

#endif /* POWERWINDOW_TEST_MODE */
