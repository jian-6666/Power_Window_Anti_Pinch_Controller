/**
 * @file PowerWindow_NvM.h
 * @brief Power Window Anti-Pinch Controller - Non-Volatile Memory Module
 * 
 * This module manages data persistence, including CRC32 checksum calculation,
 * data loading/saving, and conditional save logic.
 * 
 * @author Power Window Team
 * @version 1.0
 */

#ifndef POWERWINDOW_NVM_H
#define POWERWINDOW_NVM_H

#include "PowerWindow_Types.h"

/* ============================================================================
 * Function Declarations
 * ============================================================================ */

/**
 * @brief Initialize the NVM module
 * 
 * @param None
 * @return None
 */
void PowerWindow_NvM_Init(void);

/**
 * @brief Load data from non-volatile memory
 * 
 * Reads data from NVM and verifies CRC32 checksum.
 * 
 * @param[out] data Pointer to NVM data structure
 * @return POWERWINDOW_TRUE if load successful and CRC valid, POWERWINDOW_FALSE otherwise
 */
PowerWindow_BoolType PowerWindow_NvM_LoadData(PowerWindow_NvMDataType* data);

/**
 * @brief Save data to non-volatile memory
 * 
 * Calculates CRC32 checksum and writes data to NVM.
 * 
 * @param[in] data Pointer to NVM data structure
 * @return POWERWINDOW_TRUE if save successful, POWERWINDOW_FALSE otherwise
 */
PowerWindow_BoolType PowerWindow_NvM_SaveData(const PowerWindow_NvMDataType* data);

/**
 * @brief Check and conditionally save data
 * 
 * Saves data if any of the following conditions are met:
 * - Position change exceeds threshold (5%)
 * - Learning completion state changed
 * - New fault code detected
 * 
 * @param[in] currentPosition Current window position
 * @param[in] lastSavedPosition Last saved position
 * @param[in] learnState Current learning state
 * @param[in] lastLearnState Last saved learning state
 * @param[in] newFaultDetected Flag indicating new fault was detected
 * @return None
 */
void PowerWindow_NvM_CheckAndSave(PowerWindow_PositionType currentPosition,
                                   PowerWindow_PositionType lastSavedPosition,
                                   PowerWindow_LearnStateType learnState,
                                   PowerWindow_LearnStateType lastLearnState,
                                   PowerWindow_BoolType newFaultDetected);

/**
 * @brief Check if NVM data is valid
 * 
 * @return POWERWINDOW_TRUE if data is valid, POWERWINDOW_FALSE otherwise
 */
PowerWindow_BoolType PowerWindow_NvM_IsDataValid(void);

/**
 * @brief Get current NVM data
 * 
 * Returns a pointer to the current NVM data structure.
 * 
 * @return Pointer to current NVM data
 */
const PowerWindow_NvMDataType* PowerWindow_NvM_GetCurrentData(void);

/**
 * @brief Update active DTCs in NVM data
 * 
 * Updates the active DTC list in NVM storage.
 * 
 * @param[in] activeDTCs Array of active DTCs
 * @param[in] count Number of active DTCs
 * @return None
 */
void PowerWindow_NvM_UpdateActiveDTCs(const PowerWindow_DTCType* activeDTCs, uint8_t count);

/**
 * @brief Update history DTCs in NVM data
 * 
 * Updates the history DTC list in NVM storage.
 * 
 * @param[in] historyDTCs Array of history DTCs
 * @param[in] count Number of history DTCs
 * @return None
 */
void PowerWindow_NvM_UpdateHistoryDTCs(const PowerWindow_DTCType* historyDTCs, uint8_t count);

/**
 * @brief Update learning data in NVM
 * 
 * Updates learning state and learned limits in NVM storage.
 * 
 * @param[in] learnState Learning state
 * @param[in] upperLimit Learned upper limit
 * @param[in] lowerLimit Learned lower limit
 * @return None
 */
void PowerWindow_NvM_UpdateLearningData(PowerWindow_LearnStateType learnState,
                                         PowerWindow_PositionType upperLimit,
                                         PowerWindow_PositionType lowerLimit);

/**
 * @brief Calculate CRC32 checksum
 * 
 * Calculates CRC32 checksum for data integrity verification.
 * 
 * @param[in] data Pointer to data buffer
 * @param[in] length Length of data in bytes
 * @return CRC32 checksum value
 */
uint32_t PowerWindow_NvM_CalculateCRC32(const uint8_t* data, uint32_t length);

#endif /* POWERWINDOW_NVM_H */
