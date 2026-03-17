/**
 * @file PowerWindow_Cfg.c
 * @brief Power Window Anti-Pinch Controller - Configuration Implementation
 * 
 * This file implements calibration parameter initialization and validation functions.
 * 
 * @author Power Window Team
 * @version 1.0
 */

#include "PowerWindow_Cfg.h"

/* ============================================================================
 * Public Function Implementations
 * ============================================================================ */

PowerWindow_BoolType PowerWindow_Cfg_InitDefaultParams(
    PowerWindow_CalibrationParamsType* params
)
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
    params->autoModeTriggerTime = POWERWINDOW_DEFAULT_AUTO_TRIGGER_TIME;
    
    return POWERWINDOW_TRUE;
}

PowerWindow_BoolType PowerWindow_Cfg_ValidateParams(
    const PowerWindow_CalibrationParamsType* params
)
{
    if (params == NULL) {
        return POWERWINDOW_FALSE;
    }
    
    /* Validate anti-pinch current threshold */
    if (!PowerWindow_Cfg_ValidateAntiPinchCurrent(params->antiPinchCurrentThreshold)) {
        return POWERWINDOW_FALSE;
    }
    
    /* Validate anti-pinch retract distance */
    if (!PowerWindow_Cfg_ValidateRetractDistance(params->antiPinchRetractDistance)) {
        return POWERWINDOW_FALSE;
    }
    
    /* Validate position limits */
    if (!PowerWindow_Cfg_ValidatePositionLimits(
        params->positionUpperLimit,
        params->positionLowerLimit
    )) {
        return POWERWINDOW_FALSE;
    }
    
    /* Validate undervoltage threshold */
    if (!PowerWindow_Cfg_ValidateUndervoltageThreshold(params->undervoltageThreshold)) {
        return POWERWINDOW_FALSE;
    }
    
    /* Validate overcurrent threshold */
    if (!PowerWindow_Cfg_ValidateOvercurrentThreshold(params->overcurrentThreshold)) {
        return POWERWINDOW_FALSE;
    }
    
    /* Validate auto mode trigger time */
    if (!PowerWindow_Cfg_ValidateAutoTriggerTime(params->autoModeTriggerTime)) {
        return POWERWINDOW_FALSE;
    }
    
    return POWERWINDOW_TRUE;
}

PowerWindow_BoolType PowerWindow_Cfg_ValidateAntiPinchCurrent(
    PowerWindow_CurrentType threshold
)
{
    if (threshold < POWERWINDOW_ANTI_PINCH_CURRENT_MIN ||
        threshold > POWERWINDOW_ANTI_PINCH_CURRENT_MAX) {
        return POWERWINDOW_FALSE;
    }
    
    return POWERWINDOW_TRUE;
}

PowerWindow_BoolType PowerWindow_Cfg_ValidateRetractDistance(
    PowerWindow_PositionType distance
)
{
    if (distance < POWERWINDOW_RETRACT_DISTANCE_MIN ||
        distance > POWERWINDOW_RETRACT_DISTANCE_MAX) {
        return POWERWINDOW_FALSE;
    }
    
    return POWERWINDOW_TRUE;
}

PowerWindow_BoolType PowerWindow_Cfg_ValidatePositionLimits(
    PowerWindow_PositionType upperLimit,
    PowerWindow_PositionType lowerLimit
)
{
    /* Check if upper limit is greater than lower limit */
    if (upperLimit <= lowerLimit) {
        return POWERWINDOW_FALSE;
    }
    
    /* Check if both limits are within valid range */
    if (upperLimit > POWERWINDOW_POSITION_MAX) {
        return POWERWINDOW_FALSE;
    }
    
    return POWERWINDOW_TRUE;
}

PowerWindow_BoolType PowerWindow_Cfg_ValidateUndervoltageThreshold(
    PowerWindow_VoltageType threshold
)
{
    if (threshold < POWERWINDOW_UNDERVOLTAGE_MIN ||
        threshold > POWERWINDOW_UNDERVOLTAGE_MAX) {
        return POWERWINDOW_FALSE;
    }
    
    return POWERWINDOW_TRUE;
}

PowerWindow_BoolType PowerWindow_Cfg_ValidateOvercurrentThreshold(
    PowerWindow_CurrentType threshold
)
{
    if (threshold < POWERWINDOW_OVERCURRENT_MIN ||
        threshold > POWERWINDOW_OVERCURRENT_MAX) {
        return POWERWINDOW_FALSE;
    }
    
    return POWERWINDOW_TRUE;
}

PowerWindow_BoolType PowerWindow_Cfg_ValidateAutoTriggerTime(
    uint16_t triggerTime
)
{
    if (triggerTime < POWERWINDOW_AUTO_TRIGGER_TIME_MIN ||
        triggerTime > POWERWINDOW_AUTO_TRIGGER_TIME_MAX) {
        return POWERWINDOW_FALSE;
    }
    
    return POWERWINDOW_TRUE;
}
