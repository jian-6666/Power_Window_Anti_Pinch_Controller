/**
 * @file PowerWindow_Diag.h
 * @brief Power Window Anti-Pinch Controller - Diagnostic Module
 * 
 * This module manages diagnostic trouble codes (DTCs), provides diagnostic
 * data reading, and handles diagnostic commands.
 * 
 * @author Power Window Team
 * @version 1.0
 */

#ifndef POWERWINDOW_DIAG_H
#define POWERWINDOW_DIAG_H

#include "PowerWindow_Types.h"

/* ============================================================================
 * Function Declarations
 * ============================================================================ */

/**
 * @brief Initialize the diagnostic module
 * 
 * @param None
 * @return None
 */
void PowerWindow_Diag_Init(void);

/**
 * @brief Set a diagnostic trouble code
 * 
 * Adds the DTC to both active and history lists.
 * 
 * @param[in] dtc Diagnostic trouble code to set
 * @return None
 */
void PowerWindow_Diag_SetDTC(PowerWindow_DTCCodeType dtc);

/**
 * @brief Clear a diagnostic trouble code
 * 
 * Removes the DTC from active list but keeps it in history.
 * 
 * @param[in] dtc Diagnostic trouble code to clear
 * @return None
 */
void PowerWindow_Diag_ClearDTC(PowerWindow_DTCCodeType dtc);

/**
 * @brief Clear all diagnostic trouble codes
 * 
 * Clears all active DTCs but keeps history.
 * 
 * @param None
 * @return None
 */
void PowerWindow_Diag_ClearAllDTCs(void);

/**
 * @brief Get all active diagnostic trouble codes
 * 
 * @param[out] dtcList Pointer to array to store DTCs
 * @param[in] maxCount Maximum number of DTCs to retrieve
 * @return Number of active DTCs retrieved
 */
uint8_t PowerWindow_Diag_GetActiveDTCs(PowerWindow_DTCType* dtcList, uint8_t maxCount);

/**
 * @brief Get all history diagnostic trouble codes
 * 
 * @param[out] dtcList Pointer to array to store DTCs
 * @param[in] maxCount Maximum number of DTCs to retrieve
 * @return Number of history DTCs retrieved
 */
uint8_t PowerWindow_Diag_GetHistoryDTCs(PowerWindow_DTCType* dtcList, uint8_t maxCount);

/**
 * @brief Check if a specific DTC is active
 * 
 * @param[in] dtc Diagnostic trouble code to check
 * @return POWERWINDOW_TRUE if DTC is active, POWERWINDOW_FALSE otherwise
 */
PowerWindow_BoolType PowerWindow_Diag_IsDTCActive(PowerWindow_DTCCodeType dtc);

/**
 * @brief Read current window position percentage
 * 
 * @return Current position as percentage (0-100%)
 */
PowerWindow_PercentType PowerWindow_Diag_ReadPosition(void);

/**
 * @brief Read current system state
 * 
 * @return Current system state
 */
PowerWindow_SystemStateType PowerWindow_Diag_ReadState(void);

/**
 * @brief Read current learning state
 * 
 * @return Current learning state
 */
PowerWindow_LearnStateType PowerWindow_Diag_ReadLearnState(void);

/**
 * @brief Read all calibration parameters
 * 
 * @param[out] params Pointer to calibration parameters structure
 * @return None
 */
void PowerWindow_Diag_ReadCalibrationParams(PowerWindow_CalibrationParamsType* params);

/**
 * @brief Trigger learning procedure
 * 
 * Initiates the window position learning procedure.
 * 
 * @return POWERWINDOW_TRUE if learning procedure started successfully, 
 *         POWERWINDOW_FALSE if system is not in appropriate state
 */
PowerWindow_BoolType PowerWindow_Diag_TriggerLearning(void);

/* ============================================================================
 * Internal Update Functions (Called by other modules)
 * ============================================================================ */

/**
 * @brief Update diagnostic data (internal use)
 * 
 * @param[in] position Current position percentage
 * @param[in] state Current system state
 * @param[in] learnState Current learning state
 * @return None
 */
void PowerWindow_Diag_UpdateData(PowerWindow_PercentType position,
                                  PowerWindow_SystemStateType state,
                                  PowerWindow_LearnStateType learnState);

/**
 * @brief Update calibration parameters (internal use)
 * 
 * @param[in] params Pointer to calibration parameters
 * @return None
 */
void PowerWindow_Diag_UpdateCalibParams(const PowerWindow_CalibrationParamsType* params);

#endif /* POWERWINDOW_DIAG_H */
