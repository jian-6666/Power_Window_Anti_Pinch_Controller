/**
 * @file PowerWindow_Diag.c
 * @brief Power Window Anti-Pinch Controller - Diagnostic Module Implementation
 * 
 * This module implements diagnostic trouble code management and diagnostic
 * data reading functions.
 * 
 * @author Power Window Team
 * @version 1.0
 */

#include "PowerWindow_Diag.h"
#include "PowerWindow_Cfg.h"
#include <string.h>

/* ============================================================================
 * Diagnostic Data Structure
 * ============================================================================ */

typedef struct {
    PowerWindow_DTCType activeDTCs[8];
    uint8_t activeDTCCount;
    PowerWindow_DTCType historyDTCs[16];
    uint8_t historyDTCCount;
    PowerWindow_PercentType currentPosition;
    PowerWindow_SystemStateType currentState;
    PowerWindow_LearnStateType learnState;
    PowerWindow_CalibrationParamsType calibParams;
} PowerWindow_DiagDataType;

static PowerWindow_DiagDataType diagData = {
    .activeDTCCount = 0,
    .historyDTCCount = 0,
    .currentPosition = 50,
    .currentState = POWERWINDOW_STATE_UNINITIALIZED,
    .learnState = POWERWINDOW_LEARN_NOT_STARTED
};

/* ============================================================================
 * Helper Functions
 * ============================================================================ */

/**
 * @brief Check if a DTC already exists in a list
 * 
 * @param[in] dtcList Pointer to DTC list
 * @param[in] count Number of DTCs in list
 * @param[in] dtc DTC to search for
 * @return Index if found, -1 if not found
 */
static int PowerWindow_Diag_FindDTC(const PowerWindow_DTCType* dtcList, 
                                     uint8_t count, 
                                     PowerWindow_DTCCodeType dtc)
{
    for (uint8_t i = 0; i < count; i++) {
        if (dtcList[i] == (PowerWindow_DTCType)dtc) {
            return (int)i;
        }
    }
    return -1;
}

/* ============================================================================
 * Function Implementations
 * ============================================================================ */

void PowerWindow_Diag_Init(void)
{
    memset(&diagData, 0, sizeof(PowerWindow_DiagDataType));
    diagData.currentPosition = 50;
    diagData.currentState = POWERWINDOW_STATE_UNINITIALIZED;
    diagData.learnState = POWERWINDOW_LEARN_NOT_STARTED;
    PowerWindow_Cfg_InitDefaultParams(&diagData.calibParams);
}

void PowerWindow_Diag_SetDTC(PowerWindow_DTCCodeType dtc)
{
    PowerWindow_DTCType dtcValue = (PowerWindow_DTCType)dtc;
    
    /* Add to active DTCs if not already present */
    if (PowerWindow_Diag_FindDTC(diagData.activeDTCs, diagData.activeDTCCount, dtc) == -1) {
        if (diagData.activeDTCCount < 8) {
            diagData.activeDTCs[diagData.activeDTCCount] = dtcValue;
            diagData.activeDTCCount++;
        }
    }
    
    /* Add to history DTCs if not already present */
    if (PowerWindow_Diag_FindDTC(diagData.historyDTCs, diagData.historyDTCCount, dtc) == -1) {
        if (diagData.historyDTCCount < 16) {
            diagData.historyDTCs[diagData.historyDTCCount] = dtcValue;
            diagData.historyDTCCount++;
        }
    }
}

void PowerWindow_Diag_ClearDTC(PowerWindow_DTCCodeType dtc)
{
    int index = PowerWindow_Diag_FindDTC(diagData.activeDTCs, diagData.activeDTCCount, dtc);
    
    if (index != -1) {
        /* Remove from active DTCs by shifting remaining elements */
        for (uint8_t i = (uint8_t)index; i < diagData.activeDTCCount - 1; i++) {
            diagData.activeDTCs[i] = diagData.activeDTCs[i + 1];
        }
        diagData.activeDTCCount--;
    }
}

void PowerWindow_Diag_ClearAllDTCs(void)
{
    diagData.activeDTCCount = 0;
    memset(diagData.activeDTCs, 0, sizeof(diagData.activeDTCs));
}

uint8_t PowerWindow_Diag_GetActiveDTCs(PowerWindow_DTCType* dtcList, uint8_t maxCount)
{
    if (dtcList == NULL) {
        return 0;
    }
    
    uint8_t count = (diagData.activeDTCCount < maxCount) ? diagData.activeDTCCount : maxCount;
    memcpy(dtcList, diagData.activeDTCs, count * sizeof(PowerWindow_DTCType));
    
    return count;
}

uint8_t PowerWindow_Diag_GetHistoryDTCs(PowerWindow_DTCType* dtcList, uint8_t maxCount)
{
    if (dtcList == NULL) {
        return 0;
    }
    
    uint8_t count = (diagData.historyDTCCount < maxCount) ? diagData.historyDTCCount : maxCount;
    memcpy(dtcList, diagData.historyDTCs, count * sizeof(PowerWindow_DTCType));
    
    return count;
}

PowerWindow_BoolType PowerWindow_Diag_IsDTCActive(PowerWindow_DTCCodeType dtc)
{
    return (PowerWindow_Diag_FindDTC(diagData.activeDTCs, diagData.activeDTCCount, dtc) != -1) ?
           POWERWINDOW_TRUE : POWERWINDOW_FALSE;
}

PowerWindow_PercentType PowerWindow_Diag_ReadPosition(void)
{
    return diagData.currentPosition;
}

PowerWindow_SystemStateType PowerWindow_Diag_ReadState(void)
{
    return diagData.currentState;
}

PowerWindow_LearnStateType PowerWindow_Diag_ReadLearnState(void)
{
    return diagData.learnState;
}

void PowerWindow_Diag_ReadCalibrationParams(PowerWindow_CalibrationParamsType* params)
{
    if (params != NULL) {
        memcpy(params, &diagData.calibParams, sizeof(PowerWindow_CalibrationParamsType));
    }
}

PowerWindow_BoolType PowerWindow_Diag_TriggerLearning(void)
{
    /* Only allow learning trigger when system is in idle state */
    if (diagData.currentState == POWERWINDOW_STATE_IDLE) {
        /* This function would typically call the main PowerWindow module
         * to start the learning procedure. For now, we'll just return success
         * to indicate the interface is available. The actual learning logic
         * is implemented in the main PowerWindow module.
         */
        return POWERWINDOW_TRUE;
    }
    
    return POWERWINDOW_FALSE;
}

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
                                  PowerWindow_LearnStateType learnState)
{
    diagData.currentPosition = position;
    diagData.currentState = state;
    diagData.learnState = learnState;
}

/**
 * @brief Update calibration parameters (internal use)
 * 
 * @param[in] params Pointer to calibration parameters
 * @return None
 */
void PowerWindow_Diag_UpdateCalibParams(const PowerWindow_CalibrationParamsType* params)
{
    if (params != NULL) {
        memcpy(&diagData.calibParams, params, sizeof(PowerWindow_CalibrationParamsType));
    }
}
