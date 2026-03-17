/**
 * @file PowerWindow_Safety.c
 * @brief Power Window Anti-Pinch Controller - Safety Monitoring Implementation
 * 
 * This module implements safety monitoring functions for anti-pinch detection,
 * current monitoring, and voltage monitoring.
 * 
 * @author Power Window Team
 * @version 1.0
 */

#include "PowerWindow_Safety.h"
#include "PowerWindow_Cfg.h"

/* ============================================================================
 * Safety Data Structure
 * ============================================================================ */

typedef struct {
    uint32_t overcurrentDebounceCounter;
    uint32_t undercurrentDebounceCounter;
    uint32_t undervoltageDebounceCounter;
    uint32_t undervoltageRecoveryCounter;
    PowerWindow_BoolType overcurrentFault;
    PowerWindow_BoolType undercurrentFault;
    PowerWindow_BoolType undervoltageFault;
    PowerWindow_BoolType antiPinchDetected;
} PowerWindow_SafetyDataType;

static PowerWindow_SafetyDataType safetyData = {
    .overcurrentDebounceCounter = 0,
    .undercurrentDebounceCounter = 0,
    .undervoltageDebounceCounter = 0,
    .undervoltageRecoveryCounter = 0,
    .overcurrentFault = POWERWINDOW_FALSE,
    .undercurrentFault = POWERWINDOW_FALSE,
    .undervoltageFault = POWERWINDOW_FALSE,
    .antiPinchDetected = POWERWINDOW_FALSE
};

/* ============================================================================
 * Function Implementations
 * ============================================================================ */

void PowerWindow_Safety_Init(void)
{
    safetyData.overcurrentDebounceCounter = 0;
    safetyData.undercurrentDebounceCounter = 0;
    safetyData.undervoltageDebounceCounter = 0;
    safetyData.undervoltageRecoveryCounter = 0;
    safetyData.overcurrentFault = POWERWINDOW_FALSE;
    safetyData.undercurrentFault = POWERWINDOW_FALSE;
    safetyData.undervoltageFault = POWERWINDOW_FALSE;
    safetyData.antiPinchDetected = POWERWINDOW_FALSE;
}

PowerWindow_BoolType PowerWindow_Safety_MonitorAntiPinch(
    PowerWindow_CurrentType current,
    PowerWindow_SystemStateType state,
    PowerWindow_CurrentType antiPinchThreshold)
{
    /* Anti-pinch only applies during upward movement */
    if (state != POWERWINDOW_STATE_MANUAL_UP && state != POWERWINDOW_STATE_AUTO_UP) {
        safetyData.antiPinchDetected = POWERWINDOW_FALSE;
        return POWERWINDOW_FALSE;
    }
    
    /* Check if current exceeds anti-pinch threshold */
    if (current > antiPinchThreshold) {
        safetyData.antiPinchDetected = POWERWINDOW_TRUE;
        return POWERWINDOW_TRUE;
    }
    
    safetyData.antiPinchDetected = POWERWINDOW_FALSE;
    return POWERWINDOW_FALSE;
}

void PowerWindow_Safety_MonitorCurrent(
    PowerWindow_CurrentType current,
    PowerWindow_SystemStateType state,
    PowerWindow_CurrentType overcurrentThreshold,
    PowerWindow_CurrentType undercurrentThreshold)
{
    /* Only monitor current when motor is running */
    if (state == POWERWINDOW_STATE_IDLE || 
        state == POWERWINDOW_STATE_UNINITIALIZED || 
        state == POWERWINDOW_STATE_FAULT) {
        safetyData.overcurrentDebounceCounter = 0;
        safetyData.undercurrentDebounceCounter = 0;
        safetyData.overcurrentFault = POWERWINDOW_FALSE;
        safetyData.undercurrentFault = POWERWINDOW_FALSE;
        return;
    }
    
    /* Overcurrent detection with debounce */
    if (current > overcurrentThreshold) {
        safetyData.overcurrentDebounceCounter++;
        if (safetyData.overcurrentDebounceCounter >= POWERWINDOW_OVERCURRENT_DEBOUNCE_COUNT) {
            safetyData.overcurrentFault = POWERWINDOW_TRUE;
        }
    } else {
        safetyData.overcurrentDebounceCounter = 0;
        safetyData.overcurrentFault = POWERWINDOW_FALSE;
    }
    
    /* Undercurrent detection with debounce */
    if (current < undercurrentThreshold) {
        safetyData.undercurrentDebounceCounter++;
        if (safetyData.undercurrentDebounceCounter >= POWERWINDOW_UNDERCURRENT_DEBOUNCE_COUNT) {
            safetyData.undercurrentFault = POWERWINDOW_TRUE;
        }
    } else {
        safetyData.undercurrentDebounceCounter = 0;
        safetyData.undercurrentFault = POWERWINDOW_FALSE;
    }
}

void PowerWindow_Safety_MonitorVoltage(
    PowerWindow_VoltageType voltage,
    PowerWindow_VoltageType undervoltageThreshold)
{
    /* Undervoltage detection with debounce */
    if (voltage < undervoltageThreshold) {
        safetyData.undervoltageDebounceCounter++;
        safetyData.undervoltageRecoveryCounter = 0;
        
        if (safetyData.undervoltageDebounceCounter >= POWERWINDOW_UNDERVOLTAGE_DEBOUNCE_COUNT) {
            safetyData.undervoltageFault = POWERWINDOW_TRUE;
        }
    } else {
        /* Voltage recovery with debounce */
        safetyData.undervoltageRecoveryCounter++;
        
        if (safetyData.undervoltageRecoveryCounter >= POWERWINDOW_UNDERVOLTAGE_RECOVERY_COUNT) {
            safetyData.undervoltageDebounceCounter = 0;
            safetyData.undervoltageFault = POWERWINDOW_FALSE;
        }
    }
}

PowerWindow_BoolType PowerWindow_Safety_CheckFaults(void)
{
    return (safetyData.overcurrentFault || 
            safetyData.undercurrentFault || 
            safetyData.undervoltageFault) ? 
           POWERWINDOW_TRUE : POWERWINDOW_FALSE;
}

PowerWindow_BoolType PowerWindow_Safety_HasActiveFault(void)
{
    return PowerWindow_Safety_CheckFaults();
}

void PowerWindow_Safety_GetActiveFaults(PowerWindow_DTCType* faultList, uint8_t* count)
{
    if (faultList == NULL || count == NULL) {
        return;
    }
    
    *count = 0;
    
    if (safetyData.overcurrentFault) {
        faultList[(*count)++] = POWERWINDOW_DTC_OVERCURRENT;
    }
    
    if (safetyData.undercurrentFault) {
        faultList[(*count)++] = POWERWINDOW_DTC_UNDERCURRENT;
    }
    
    if (safetyData.undervoltageFault) {
        faultList[(*count)++] = POWERWINDOW_DTC_UNDERVOLTAGE;
    }
}

void PowerWindow_Safety_ResetDebounceCounters(void)
{
    safetyData.overcurrentDebounceCounter = 0;
    safetyData.undercurrentDebounceCounter = 0;
    safetyData.undervoltageDebounceCounter = 0;
    safetyData.undervoltageRecoveryCounter = 0;
}

void PowerWindow_Safety_ClearFaults(void)
{
    safetyData.overcurrentFault = POWERWINDOW_FALSE;
    safetyData.undercurrentFault = POWERWINDOW_FALSE;
    safetyData.undervoltageFault = POWERWINDOW_FALSE;
    safetyData.antiPinchDetected = POWERWINDOW_FALSE;
    PowerWindow_Safety_ResetDebounceCounters();
}
