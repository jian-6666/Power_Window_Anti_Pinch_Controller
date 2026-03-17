/**
 * @file PowerWindow_Cfg.c
 * @brief Power Window Anti-Pinch Controller - Configuration Implementation
 * 
 * This file implements configuration functions for parameter validation and
 * initialization of the Power Window system.
 * 
 * @author Power Window Team
 * @version 1.0
 */

#include "PowerWindow_Cfg.h"

/* ============================================================================
 * Global Calibration Parameters Instance
 * ============================================================================ */

const PowerWindow_CalibrationParamsType PowerWindow_CalibrationParams = {
    .antiPinchCurrentThreshold = POWERWINDOW_DEFAULT_ANTI_PINCH_CURRENT,
    .antiPinchRetractDistance = POWERWINDOW_DEFAULT_RETRACT_DISTANCE,
    .positionUpperLimit = POWERWINDOW_DEFAULT_POSITION_UPPER,
    .positionLowerLimit = POWERWINDOW_DEFAULT_POSITION_LOWER,
    .undervoltageThreshold = POWERWINDOW_DEFAULT_UNDERVOLTAGE,
    .overcurrentThreshold = POWERWINDOW_DEFAULT_OVERCURRENT,
    .undercurrentThreshold = POWERWINDOW_DEFAULT_UNDERCURRENT,
    .autoModeTriggerTime = POWERWINDOW_DEFAULT_AUTO_TRIGGER_TIME
};

/* ============================================================================
 * Function Implementations
 * ============================================================================ */

PowerWindow_BoolType PowerWindow_Cfg_InitDefaultParams(
    PowerWindow_CalibrationParamsType* params)
{
    if (params == NULL) {
        return POWERWINDOW_FALSE;
    }
    
    params->antiPinchCurrentThreshold = POWERWINDOW_DEFAULT_ANTI_PINCH_CURRENT;
    params->antiPinchRetractDistance = POWERWINDOW_DEFAULT_RETRACT_DISTANCE;
    params->positionUpperLimit = POWERWINDOW_DEFAULT_POSITION_UPPER;
    params->positionLowerLimit = POWERWINDOW_DEFAULT_POSITION_LOWER;
    params->undervoltageThreshold = POWERWINDOW_DEFAULT_UNDERVOLTAGE;
    params->overcurrentThreshold = POWERWINDOW_DEFAULT_OVERCURRENT;
    params->undercurrentThreshold = POWERWINDOW_DEFAULT_UNDERCURRENT;
    params->autoModeTriggerTime = POWERWINDOW_DEFAULT_AUTO_TRIGGER_TIME;
    
    return POWERWINDOW_TRUE;
}

PowerWindow_BoolType PowerWindow_Cfg_ValidateParams(
    const PowerWindow_CalibrationParamsType* params)
{
    if (params == NULL) {
        return POWERWINDOW_FALSE;
    }
    
    if (!PowerWindow_Cfg_ValidateAntiPinchCurrent(params->antiPinchCurrentThreshold)) {
        return POWERWINDOW_FALSE;
    }
    
    if (!PowerWindow_Cfg_ValidateRetractDistance(params->antiPinchRetractDistance)) {
        return POWERWINDOW_FALSE;
    }
    
    if (!PowerWindow_Cfg_ValidatePositionLimits(params->positionUpperLimit, 
                                                 params->positionLowerLimit)) {
        return POWERWINDOW_FALSE;
    }
    
    if (!PowerWindow_Cfg_ValidateUndervoltageThreshold(params->undervoltageThreshold)) {
        return POWERWINDOW_FALSE;
    }
    
    if (!PowerWindow_Cfg_ValidateOvercurrentThreshold(params->overcurrentThreshold)) {
        return POWERWINDOW_FALSE;
    }
    
    if (!PowerWindow_Cfg_ValidateUndercurrentThreshold(params->undercurrentThreshold)) {
        return POWERWINDOW_FALSE;
    }
    
    if (!PowerWindow_Cfg_ValidateAutoTriggerTime(params->autoModeTriggerTime)) {
        return POWERWINDOW_FALSE;
    }
    
    return POWERWINDOW_TRUE;
}

PowerWindow_BoolType PowerWindow_Cfg_ValidateAntiPinchCurrent(
    PowerWindow_CurrentType threshold)
{
    return (threshold >= POWERWINDOW_ANTI_PINCH_CURRENT_MIN &&
            threshold <= POWERWINDOW_ANTI_PINCH_CURRENT_MAX) ? 
           POWERWINDOW_TRUE : POWERWINDOW_FALSE;
}

PowerWindow_BoolType PowerWindow_Cfg_ValidateRetractDistance(
    PowerWindow_PositionType distance)
{
    return (distance >= POWERWINDOW_RETRACT_DISTANCE_MIN &&
            distance <= POWERWINDOW_RETRACT_DISTANCE_MAX) ? 
           POWERWINDOW_TRUE : POWERWINDOW_FALSE;
}

PowerWindow_BoolType PowerWindow_Cfg_ValidatePositionLimits(
    PowerWindow_PositionType upperLimit,
    PowerWindow_PositionType lowerLimit)
{
    return (upperLimit > lowerLimit &&
            upperLimit <= POWERWINDOW_POSITION_MAX &&
            lowerLimit >= POWERWINDOW_POSITION_MIN) ? 
           POWERWINDOW_TRUE : POWERWINDOW_FALSE;
}

PowerWindow_BoolType PowerWindow_Cfg_ValidateUndervoltageThreshold(
    PowerWindow_VoltageType threshold)
{
    return (threshold >= POWERWINDOW_UNDERVOLTAGE_MIN &&
            threshold <= POWERWINDOW_UNDERVOLTAGE_MAX) ? 
           POWERWINDOW_TRUE : POWERWINDOW_FALSE;
}

PowerWindow_BoolType PowerWindow_Cfg_ValidateOvercurrentThreshold(
    PowerWindow_CurrentType threshold)
{
    return (threshold >= POWERWINDOW_OVERCURRENT_MIN &&
            threshold <= POWERWINDOW_OVERCURRENT_MAX) ? 
           POWERWINDOW_TRUE : POWERWINDOW_FALSE;
}

PowerWindow_BoolType PowerWindow_Cfg_ValidateUndercurrentThreshold(
    PowerWindow_CurrentType threshold)
{
    return (threshold >= POWERWINDOW_UNDERCURRENT_MIN &&
            threshold <= POWERWINDOW_UNDERCURRENT_MAX) ? 
           POWERWINDOW_TRUE : POWERWINDOW_FALSE;
}

PowerWindow_BoolType PowerWindow_Cfg_ValidateAutoTriggerTime(
    uint16_t triggerTime)
{
    return (triggerTime >= POWERWINDOW_AUTO_TRIGGER_TIME_MIN &&
            triggerTime <= POWERWINDOW_AUTO_TRIGGER_TIME_MAX) ? 
           POWERWINDOW_TRUE : POWERWINDOW_FALSE;
}
