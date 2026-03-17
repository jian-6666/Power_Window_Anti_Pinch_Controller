/**
 * @file PowerWindow_NvM.c
 * @brief Power Window Anti-Pinch Controller - Non-Volatile Memory Implementation
 * 
 * This module implements data persistence with CRC32 checksum verification,
 * conditional saving logic, and comprehensive NVM data management.
 * 
 * @author Power Window Team
 * @version 1.0
 */

#include "PowerWindow_NvM.h"
#include "PowerWindow_Cfg.h"
#include "PowerWindow_Diag.h"
#include <string.h>

/* ============================================================================
 * NVM Storage (Simulated)
 * ============================================================================ */

static PowerWindow_NvMDataType nvmStorage = {
    .savedPosition = 2048,
    .learnedUpperLimit = 4095,
    .learnedLowerLimit = 0,
    .learnState = POWERWINDOW_LEARN_NOT_STARTED,
    .crc = 0
};

static PowerWindow_BoolType nvmValid = POWERWINDOW_FALSE;
static PowerWindow_BoolType nvmInitialized = POWERWINDOW_FALSE;

/* ============================================================================
 * CRC32 Calculation
 * ============================================================================ */

uint32_t PowerWindow_NvM_CalculateCRC32(const uint8_t* data, uint32_t length)
{
    if (data == NULL) {
        return 0;
    }
    
    uint32_t crc = 0xFFFFFFFFU;
    
    for (uint32_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320U;
            } else {
                crc = crc >> 1;
            }
        }
    }
    
    return ~crc;
}

/* ============================================================================
 * Function Implementations
 * ============================================================================ */

void PowerWindow_NvM_Init(void)
{
    /* Initialize NVM storage with default values */
    memset(&nvmStorage, 0, sizeof(PowerWindow_NvMDataType));
    nvmStorage.savedPosition = POWERWINDOW_DEFAULT_POSITION_LOWER + 
                               ((POWERWINDOW_DEFAULT_POSITION_UPPER - POWERWINDOW_DEFAULT_POSITION_LOWER) / 2);
    nvmStorage.learnedUpperLimit = POWERWINDOW_DEFAULT_POSITION_UPPER;
    nvmStorage.learnedLowerLimit = POWERWINDOW_DEFAULT_POSITION_LOWER;
    nvmStorage.learnState = POWERWINDOW_LEARN_NOT_STARTED;
    
    /* Initialize DTC arrays */
    for (uint8_t i = 0; i < 8; i++) {
        nvmStorage.activeDTCs[i] = POWERWINDOW_DTC_NO_FAULT;
    }
    for (uint8_t i = 0; i < 16; i++) {
        nvmStorage.historyDTCs[i] = POWERWINDOW_DTC_NO_FAULT;
    }
    
    /* Calculate initial CRC */
    nvmStorage.crc = PowerWindow_NvM_CalculateCRC32(
        (const uint8_t*)&nvmStorage,
        sizeof(PowerWindow_NvMDataType) - sizeof(uint32_t)
    );
    
    nvmValid = POWERWINDOW_TRUE;
    nvmInitialized = POWERWINDOW_TRUE;
}

PowerWindow_BoolType PowerWindow_NvM_LoadData(PowerWindow_NvMDataType* data)
{
    if (data == NULL) {
        return POWERWINDOW_FALSE;
    }
    
    if (!nvmInitialized) {
        /* NVM not initialized, set DTC and use defaults */
        PowerWindow_Diag_SetDTC(POWERWINDOW_DTC_NVM_READ_FAIL);
        
        /* Initialize with default values */
        memset(data, 0, sizeof(PowerWindow_NvMDataType));
        data->savedPosition = POWERWINDOW_DEFAULT_POSITION_LOWER + 
                              ((POWERWINDOW_DEFAULT_POSITION_UPPER - POWERWINDOW_DEFAULT_POSITION_LOWER) / 2);
        data->learnedUpperLimit = POWERWINDOW_DEFAULT_POSITION_UPPER;
        data->learnedLowerLimit = POWERWINDOW_DEFAULT_POSITION_LOWER;
        data->learnState = POWERWINDOW_LEARN_NOT_STARTED;
        
        for (uint8_t i = 0; i < 8; i++) {
            data->activeDTCs[i] = POWERWINDOW_DTC_NO_FAULT;
        }
        for (uint8_t i = 0; i < 16; i++) {
            data->historyDTCs[i] = POWERWINDOW_DTC_NO_FAULT;
        }
        
        return POWERWINDOW_FALSE;
    }
    
    /* Copy data from storage */
    memcpy(data, &nvmStorage, sizeof(PowerWindow_NvMDataType));
    
    /* Verify CRC32 */
    uint32_t calculatedCRC = PowerWindow_NvM_CalculateCRC32(
        (const uint8_t*)data,
        sizeof(PowerWindow_NvMDataType) - sizeof(uint32_t)
    );
    
    if (calculatedCRC != data->crc) {
        /* CRC mismatch, set DTC and use defaults */
        PowerWindow_Diag_SetDTC(POWERWINDOW_DTC_NVM_READ_FAIL);
        nvmValid = POWERWINDOW_FALSE;
        
        /* Initialize with default values */
        memset(data, 0, sizeof(PowerWindow_NvMDataType));
        data->savedPosition = POWERWINDOW_DEFAULT_POSITION_LOWER + 
                              ((POWERWINDOW_DEFAULT_POSITION_UPPER - POWERWINDOW_DEFAULT_POSITION_LOWER) / 2);
        data->learnedUpperLimit = POWERWINDOW_DEFAULT_POSITION_UPPER;
        data->learnedLowerLimit = POWERWINDOW_DEFAULT_POSITION_LOWER;
        data->learnState = POWERWINDOW_LEARN_NOT_STARTED;
        
        for (uint8_t i = 0; i < 8; i++) {
            data->activeDTCs[i] = POWERWINDOW_DTC_NO_FAULT;
        }
        for (uint8_t i = 0; i < 16; i++) {
            data->historyDTCs[i] = POWERWINDOW_DTC_NO_FAULT;
        }
        
        return POWERWINDOW_FALSE;
    }
    
    nvmValid = POWERWINDOW_TRUE;
    return POWERWINDOW_TRUE;
}

PowerWindow_BoolType PowerWindow_NvM_SaveData(const PowerWindow_NvMDataType* data)
{
    if (data == NULL) {
        PowerWindow_Diag_SetDTC(POWERWINDOW_DTC_NVM_WRITE_FAIL);
        return POWERWINDOW_FALSE;
    }
    
    /* Copy data to storage (excluding CRC field) */
    memcpy(&nvmStorage, data, sizeof(PowerWindow_NvMDataType) - sizeof(uint32_t));
    
    /* Calculate and store CRC32 */
    nvmStorage.crc = PowerWindow_NvM_CalculateCRC32(
        (const uint8_t*)&nvmStorage,
        sizeof(PowerWindow_NvMDataType) - sizeof(uint32_t)
    );
    
    /* Simulate write failure for testing purposes */
    /* In real implementation, this would be actual NVM write operation */
    static uint8_t writeFailureCounter = 0;
    if (writeFailureCounter > 0) {
        writeFailureCounter--;
        PowerWindow_Diag_SetDTC(POWERWINDOW_DTC_NVM_WRITE_FAIL);
        return POWERWINDOW_FALSE;
    }
    
    nvmValid = POWERWINDOW_TRUE;
    nvmInitialized = POWERWINDOW_TRUE;
    return POWERWINDOW_TRUE;
}

void PowerWindow_NvM_CheckAndSave(PowerWindow_PositionType currentPosition,
                                   PowerWindow_PositionType lastSavedPosition,
                                   PowerWindow_LearnStateType learnState,
                                   PowerWindow_LearnStateType lastLearnState,
                                   PowerWindow_BoolType newFaultDetected)
{
    PowerWindow_BoolType shouldSave = POWERWINDOW_FALSE;
    
    /* Check position change threshold (5%) */
    uint32_t positionRange = POWERWINDOW_POSITION_MAX - POWERWINDOW_POSITION_MIN;
    
    if (positionRange > 0) {
        uint32_t positionDiff;
        if (currentPosition >= lastSavedPosition) {
            positionDiff = currentPosition - lastSavedPosition;
        } else {
            positionDiff = lastSavedPosition - currentPosition;
        }
        
        uint32_t changePercent = (positionDiff * 100) / positionRange;
        
        if (changePercent >= POWERWINDOW_POSITION_CHANGE_THRESHOLD) {
            shouldSave = POWERWINDOW_TRUE;
        }
    }
    
    /* Check learning state change */
    if (learnState != lastLearnState) {
        shouldSave = POWERWINDOW_TRUE;
    }
    
    /* Check new fault detected */
    if (newFaultDetected == POWERWINDOW_TRUE) {
        shouldSave = POWERWINDOW_TRUE;
    }
    
    /* Save if any condition is met */
    if (shouldSave == POWERWINDOW_TRUE) {
        PowerWindow_NvMDataType dataToSave;
        
        /* Copy current storage data */
        memcpy(&dataToSave, &nvmStorage, sizeof(PowerWindow_NvMDataType));
        
        /* Update with new values */
        dataToSave.savedPosition = currentPosition;
        dataToSave.learnState = learnState;
        
        /* Save the updated data */
        PowerWindow_NvM_SaveData(&dataToSave);
    }
}

PowerWindow_BoolType PowerWindow_NvM_IsDataValid(void)
{
    return nvmValid;
}

const PowerWindow_NvMDataType* PowerWindow_NvM_GetCurrentData(void)
{
    return &nvmStorage;
}

void PowerWindow_NvM_UpdateActiveDTCs(const PowerWindow_DTCType* activeDTCs, uint8_t count)
{
    if (activeDTCs == NULL || count > 8) {
        return;
    }
    
    /* Clear existing active DTCs */
    for (uint8_t i = 0; i < 8; i++) {
        nvmStorage.activeDTCs[i] = POWERWINDOW_DTC_NO_FAULT;
    }
    
    /* Copy new active DTCs */
    for (uint8_t i = 0; i < count; i++) {
        nvmStorage.activeDTCs[i] = activeDTCs[i];
    }
    
    /* Update CRC */
    nvmStorage.crc = PowerWindow_NvM_CalculateCRC32(
        (const uint8_t*)&nvmStorage,
        sizeof(PowerWindow_NvMDataType) - sizeof(uint32_t)
    );
}

void PowerWindow_NvM_UpdateHistoryDTCs(const PowerWindow_DTCType* historyDTCs, uint8_t count)
{
    if (historyDTCs == NULL || count > 16) {
        return;
    }
    
    /* Clear existing history DTCs */
    for (uint8_t i = 0; i < 16; i++) {
        nvmStorage.historyDTCs[i] = POWERWINDOW_DTC_NO_FAULT;
    }
    
    /* Copy new history DTCs */
    for (uint8_t i = 0; i < count; i++) {
        nvmStorage.historyDTCs[i] = historyDTCs[i];
    }
    
    /* Update CRC */
    nvmStorage.crc = PowerWindow_NvM_CalculateCRC32(
        (const uint8_t*)&nvmStorage,
        sizeof(PowerWindow_NvMDataType) - sizeof(uint32_t)
    );
}

void PowerWindow_NvM_UpdateLearningData(PowerWindow_LearnStateType learnState,
                                         PowerWindow_PositionType upperLimit,
                                         PowerWindow_PositionType lowerLimit)
{
    nvmStorage.learnState = learnState;
    nvmStorage.learnedUpperLimit = upperLimit;
    nvmStorage.learnedLowerLimit = lowerLimit;
    
    /* Update CRC */
    nvmStorage.crc = PowerWindow_NvM_CalculateCRC32(
        (const uint8_t*)&nvmStorage,
        sizeof(PowerWindow_NvMDataType) - sizeof(uint32_t)
    );
}
