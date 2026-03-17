/**
 * @file PowerWindow_Interface.h
 * @brief Power Window Anti-Pinch Controller - Hardware Interface Abstraction
 * 
 * This module provides hardware abstraction for input/output operations.
 * In test mode, it uses mock implementations for testing purposes.
 * 
 * @author Power Window Team
 * @version 1.0
 */

#ifndef POWERWINDOW_INTERFACE_H
#define POWERWINDOW_INTERFACE_H

#include "PowerWindow_Types.h"

/* ============================================================================
 * Function Declarations
 * ============================================================================ */

/**
 * @brief Initialize the interface module
 * 
 * @param None
 * @return None
 */
void PowerWindow_Interface_Init(void);

/**
 * @brief Read button state
 * 
 * @return Current button state (RELEASED, UP, or DOWN)
 */
PowerWindow_ButtonStateType PowerWindow_Interface_ReadButton(void);

/**
 * @brief Read position sensor value
 * 
 * @return Current position (0-4095)
 */
PowerWindow_PositionType PowerWindow_Interface_ReadPosition(void);

/**
 * @brief Read motor current
 * 
 * @return Current motor current (unit: 0.1A)
 */
PowerWindow_CurrentType PowerWindow_Interface_ReadCurrent(void);

/**
 * @brief Read system voltage
 * 
 * @return Current system voltage (unit: 0.1V)
 */
PowerWindow_VoltageType PowerWindow_Interface_ReadVoltage(void);

/**
 * @brief Set motor direction
 * 
 * @param[in] direction Motor direction (STOP, UP, or DOWN)
 * @return None
 */
void PowerWindow_Interface_SetMotor(PowerWindow_MotorDirectionType direction);

/* ============================================================================
 * Mock Interface Functions (Test Mode Only)
 * ============================================================================ */

#ifdef POWERWINDOW_TEST_MODE

/**
 * @brief Set mock button state (test mode only)
 * 
 * @param[in] state Button state to set
 * @return None
 */
void PowerWindow_Interface_Mock_SetButton(PowerWindow_ButtonStateType state);

/**
 * @brief Set mock position value (test mode only)
 * 
 * @param[in] position Position value to set
 * @return None
 */
void PowerWindow_Interface_Mock_SetPosition(PowerWindow_PositionType position);

/**
 * @brief Set mock current value (test mode only)
 * 
 * @param[in] current Current value to set
 * @return None
 */
void PowerWindow_Interface_Mock_SetCurrent(PowerWindow_CurrentType current);

/**
 * @brief Set mock voltage value (test mode only)
 * 
 * @param[in] voltage Voltage value to set
 * @return None
 */
void PowerWindow_Interface_Mock_SetVoltage(PowerWindow_VoltageType voltage);

/**
 * @brief Get mock motor direction (test mode only)
 * 
 * @return Current motor direction
 */
PowerWindow_MotorDirectionType PowerWindow_Interface_Mock_GetMotor(void);

/**
 * @brief Reset mock data to default values (test mode only)
 * 
 * @return None
 */
void PowerWindow_Interface_Mock_Reset(void);

#endif /* POWERWINDOW_TEST_MODE */

#endif /* POWERWINDOW_INTERFACE_H */
