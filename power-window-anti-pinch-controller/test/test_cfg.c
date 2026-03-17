/**
 * @file test_cfg.c
 * @brief Power Window Anti-Pinch Controller - Configuration Module Tests
 * 
 * Unit tests for configuration parameter validation and initialization.
 * 
 * @author Power Window Team
 * @version 1.0
 */

#include "PowerWindow_Cfg.h"
#include <stdio.h>
#include <assert.h>

/* ============================================================================
 * Test Functions
 * ============================================================================ */

void test_cfg_init_default_params(void)
{
    PowerWindow_CalibrationParamsType params;
    
    /* Test initialization */
    PowerWindow_BoolType result = PowerWindow_Cfg_InitDefaultParams(&params);
    assert(result == POWERWINDOW_TRUE);
    
    /* Verify default values */
    assert(params.antiPinchCurrentThreshold == POWERWINDOW_DEFAULT_ANTI_PINCH_CURRENT);
    assert(params.antiPinchRetractDistance == POWERWINDOW_DEFAULT_RETRACT_DISTANCE);
    assert(params.positionUpperLimit == POWERWINDOW_DEFAULT_POSITION_UPPER);
    assert(params.positionLowerLimit == POWERWINDOW_DEFAULT_POSITION_LOWER);
    assert(params.undervoltageThreshold == POWERWINDOW_DEFAULT_UNDERVOLTAGE);
    assert(params.overcurrentThreshold == POWERWINDOW_DEFAULT_OVERCURRENT);
    assert(params.autoModeTriggerTime == POWERWINDOW_DEFAULT_AUTO_TRIGGER_TIME);
    
    /* Test NULL pointer handling */
    result = PowerWindow_Cfg_InitDefaultParams(NULL);
    assert(result == POWERWINDOW_FALSE);
    
    printf("    PASS: test_cfg_init_default_params\n");
}

void test_cfg_validate_params(void)
{
    PowerWindow_CalibrationParamsType params;
    
    /* Initialize with default values */
    PowerWindow_Cfg_InitDefaultParams(&params);
    
    /* Test valid parameters */
    PowerWindow_BoolType result = PowerWindow_Cfg_ValidateParams(&params);
    assert(result == POWERWINDOW_TRUE);
    
    /* Test invalid anti-pinch current (too low) */
    params.antiPinchCurrentThreshold = POWERWINDOW_ANTI_PINCH_CURRENT_MIN - 1;
    result = PowerWindow_Cfg_ValidateParams(&params);
    assert(result == POWERWINDOW_FALSE);
    
    /* Test invalid anti-pinch current (too high) */
    PowerWindow_Cfg_InitDefaultParams(&params);
    params.antiPinchCurrentThreshold = POWERWINDOW_ANTI_PINCH_CURRENT_MAX + 1;
    result = PowerWindow_Cfg_ValidateParams(&params);
    assert(result == POWERWINDOW_FALSE);
    
    /* Test invalid retract distance */
    PowerWindow_Cfg_InitDefaultParams(&params);
    params.antiPinchRetractDistance = POWERWINDOW_RETRACT_DISTANCE_MAX + 1;
    result = PowerWindow_Cfg_ValidateParams(&params);
    assert(result == POWERWINDOW_FALSE);
    
    /* Test invalid position limits */
    PowerWindow_Cfg_InitDefaultParams(&params);
    params.positionUpperLimit = params.positionLowerLimit;
    result = PowerWindow_Cfg_ValidateParams(&params);
    assert(result == POWERWINDOW_FALSE);
    
    /* Test invalid undervoltage threshold */
    PowerWindow_Cfg_InitDefaultParams(&params);
    params.undervoltageThreshold = POWERWINDOW_UNDERVOLTAGE_MIN - 1;
    result = PowerWindow_Cfg_ValidateParams(&params);
    assert(result == POWERWINDOW_FALSE);
    
    /* Test invalid overcurrent threshold */
    PowerWindow_Cfg_InitDefaultParams(&params);
    params.overcurrentThreshold = POWERWINDOW_OVERCURRENT_MAX + 1;
    result = PowerWindow_Cfg_ValidateParams(&params);
    assert(result == POWERWINDOW_FALSE);
    
    /* Test invalid auto trigger time */
    PowerWindow_Cfg_InitDefaultParams(&params);
    params.autoModeTriggerTime = POWERWINDOW_AUTO_TRIGGER_TIME_MAX + 1;
    result = PowerWindow_Cfg_ValidateParams(&params);
    assert(result == POWERWINDOW_FALSE);
    
    /* Test NULL pointer */
    result = PowerWindow_Cfg_ValidateParams(NULL);
    assert(result == POWERWINDOW_FALSE);
    
    printf("    PASS: test_cfg_validate_params\n");
}
