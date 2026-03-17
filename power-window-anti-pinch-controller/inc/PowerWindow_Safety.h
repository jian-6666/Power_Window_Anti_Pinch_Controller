/**
 * @file PowerWindow_Safety.h
 * @brief Power Window Anti-Pinch Controller - Safety Monitoring Module
 * 
 * This module implements safety monitoring including anti-pinch detection,
 * current monitoring, voltage monitoring, and fault detection.
 * 
 * @author Power Window Team
 * @version 1.0
 */

#ifndef POWERWINDOW_SAFETY_H
#define POWERWINDOW_SAFETY_H

#include "PowerWindow_Types.h"

/* ============================================================================
 * Function Declarations
 * ============================================================================ */

/**
 * @brief Initialize the safety module
 * 
 * @param None
 * @return None
 */
void PowerWindow_Safety_Init(void);

/**
 * @brief Monitor anti-pinch condition
 * 
 * Detects if motor current exceeds anti-pinch threshold during upward movement.
 * 
 * @param[in] current Motor current (unit: 0.1A)
 * @param[in] state Current system state
 * @param[in] antiPinchThreshold Anti-pinch current threshold
 * @return POWERWINDOW_TRUE if anti-pinch detected, POWERWINDOW_FALSE otherwise
 */
PowerWindow_BoolType PowerWindow_Safety_MonitorAntiPinch(
    PowerWindow_CurrentType current,
    PowerWindow_SystemStateType state,
    PowerWindow_CurrentType antiPinchThreshold
);

/**
 * @brief Monitor motor current for overcurrent and undercurrent conditions
 * 
 * @param[in] current Motor current (unit: 0.1A)
 * @param[in] state Current system state
 * @param[in] overcurrentThreshold Overcurrent threshold
 * @param[in] undercurrentThreshold Undercurrent threshold
 * @return None
 */
void PowerWindow_Safety_MonitorCurrent(
    PowerWindow_CurrentType current,
    PowerWindow_SystemStateType state,
    PowerWindow_CurrentType overcurrentThreshold,
    PowerWindow_CurrentType undercurrentThreshold
);

/**
 * @brief Monitor system voltage for undervoltage condition
 * 
 * @param[in] voltage System voltage (unit: 0.1V)
 * @param[in] undervoltageThreshold Undervoltage threshold
 * @return None
 */
void PowerWindow_Safety_MonitorVoltage(
    PowerWindow_VoltageType voltage,
    PowerWindow_VoltageType undervoltageThreshold
);

/**
 * @brief Check for active faults
 * 
 * @return POWERWINDOW_TRUE if any fault is active, POWERWINDOW_FALSE otherwise
 */
PowerWindow_BoolType PowerWindow_Safety_CheckFaults(void);

/**
 * @brief Check if system has active faults
 * 
 * @return POWERWINDOW_TRUE if faults exist, POWERWINDOW_FALSE otherwise
 */
PowerWindow_BoolType PowerWindow_Safety_HasActiveFault(void);

/**
 * @brief Get list of active faults
 * 
 * @param[out] faultList Pointer to array to store fault codes
 * @param[out] count Pointer to store number of faults
 * @return None
 */
void PowerWindow_Safety_GetActiveFaults(PowerWindow_DTCType* faultList, uint8_t* count);

/**
 * @brief Reset fault debounce counters
 * 
 * @return None
 */
void PowerWindow_Safety_ResetDebounceCounters(void);

/**
 * @brief Clear all faults
 * 
 * @return None
 */
void PowerWindow_Safety_ClearFaults(void);

#endif /* POWERWINDOW_SAFETY_H */
