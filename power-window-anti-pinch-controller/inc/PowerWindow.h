/**
 * @file PowerWindow.h
 * @brief Power Window Anti-Pinch Controller - Main Control Module
 * 
 * This is the main control module that coordinates all subsystems and
 * implements the core power window control logic.
 * 
 * @author Power Window Team
 * @version 1.0
 */

#ifndef POWERWINDOW_H
#define POWERWINDOW_H

#include "PowerWindow_Types.h"

/* ============================================================================
 * Function Declarations
 * ============================================================================ */

/**
 * @brief Initialize the Power Window system
 * 
 * Initializes all subsystems and loads data from NVM.
 * 
 * @param None
 * @return None
 */
void PowerWindow_Init(void);

/**
 * @brief Main function called every 10ms
 * 
 * Executes the main control loop: read inputs, monitor safety,
 * update state machine, control outputs, update diagnostics.
 * 
 * @param None
 * @return None
 */
void PowerWindow_MainFunction_10ms(void);

/**
 * @brief Set button state
 * 
 * @param[in] buttonState Button state (RELEASED, UP, or DOWN)
 * @return None
 */
void PowerWindow_SetButtonState(PowerWindow_ButtonStateType buttonState);

/**
 * @brief Trigger learning procedure
 * 
 * Starts the window position learning process.
 * 
 * @param None
 * @return None
 */
void PowerWindow_TriggerLearnProcedure(void);

/**
 * @brief Stop motor immediately
 * 
 * @param None
 * @return None
 */
void PowerWindow_StopMotor(void);

/**
 * @brief Get current system state
 * 
 * @return Current system state
 */
PowerWindow_SystemStateType PowerWindow_GetCurrentState(void);

/**
 * @brief Get current window position as percentage
 * 
 * @return Position percentage (0-100%)
 */
PowerWindow_PercentType PowerWindow_GetPositionPercent(void);

/**
 * @brief Get current window position (raw sensor value)
 * 
 * @return Raw position value (0-4095)
 */
PowerWindow_PositionType PowerWindow_GetRawPosition(void);

/**
 * @brief Check if learning is completed
 * 
 * @return POWERWINDOW_TRUE if learning completed, POWERWINDOW_FALSE otherwise
 */
PowerWindow_BoolType PowerWindow_IsLearnCompleted(void);

/**
 * @brief Check if anti-pinch lockout is active
 * 
 * @return POWERWINDOW_TRUE if lockout is active, POWERWINDOW_FALSE otherwise
 */
PowerWindow_BoolType PowerWindow_IsAntiPinchLockoutActive(void);

/**
 * @brief Get main function call count
 * 
 * @return Number of times main function has been called
 */
uint32_t PowerWindow_GetMainFunctionCallCount(void);

/**
 * @brief Get execution time statistics
 * 
 * @param[out] minTime Minimum execution time
 * @param[out] maxTime Maximum execution time
 * @param[out] avgTime Average execution time
 * @return None
 */
void PowerWindow_GetExecutionTimeStats(uint32_t* minTime, uint32_t* maxTime, uint32_t* avgTime);

#endif /* POWERWINDOW_H */
