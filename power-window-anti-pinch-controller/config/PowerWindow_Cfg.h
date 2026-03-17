/**
 * @file PowerWindow_Cfg.h
 * @brief Power Window Anti-Pinch Controller - Configuration Macros
 * 
 * This file defines all configuration macros including default values,
 * parameter ranges, and debounce counts for the Power Window system.
 * 
 * @author Power Window Team
 * @version 1.0
 */

#ifndef POWERWINDOW_CFG_H
#define POWERWINDOW_CFG_H

#include "PowerWindow_Types.h"

/* ============================================================================
 * Default Calibration Parameter Values
 * ============================================================================ */

/** @brief Default anti-pinch current threshold (10.0A, unit: 0.1A) */
#define POWERWINDOW_DEFAULT_ANTI_PINCH_CURRENT    100

/** @brief Default anti-pinch retract distance (100mm) */
#define POWERWINDOW_DEFAULT_RETRACT_DISTANCE      100

/** @brief Default position upper limit */
#define POWERWINDOW_DEFAULT_POSITION_UPPER        4095

/** @brief Default position lower limit */
#define POWERWINDOW_DEFAULT_POSITION_LOWER        0

/** @brief Default undervoltage threshold (10.0V, unit: 0.1V) */
#define POWERWINDOW_DEFAULT_UNDERVOLTAGE          100

/** @brief Default overcurrent threshold (20.0A, unit: 0.1A) */
#define POWERWINDOW_DEFAULT_OVERCURRENT           200

/** @brief Default undercurrent threshold (1.0A, unit: 0.1A) */
#define POWERWINDOW_DEFAULT_UNDERCURRENT          10

/** @brief Default auto mode trigger time (500ms) */
#define POWERWINDOW_DEFAULT_AUTO_TRIGGER_TIME     500

/* ============================================================================
 * Anti-Pinch Current Threshold Range
 * ============================================================================ */

/** @brief Minimum anti-pinch current threshold (5.0A, unit: 0.1A) */
#define POWERWINDOW_ANTI_PINCH_CURRENT_MIN        50

/** @brief Maximum anti-pinch current threshold (20.0A, unit: 0.1A) */
#define POWERWINDOW_ANTI_PINCH_CURRENT_MAX        200

/* ============================================================================
 * Anti-Pinch Retract Distance Range
 * ============================================================================ */

/** @brief Minimum anti-pinch retract distance (50mm) */
#define POWERWINDOW_RETRACT_DISTANCE_MIN          50

/** @brief Maximum anti-pinch retract distance (200mm) */
#define POWERWINDOW_RETRACT_DISTANCE_MAX          200

/* ============================================================================
 * Position Limit Range
 * ============================================================================ */

/** @brief Minimum position value */
#define POWERWINDOW_POSITION_MIN                  0

/** @brief Maximum position value */
#define POWERWINDOW_POSITION_MAX                  4095

/* ============================================================================
 * Undervoltage Threshold Range
 * ============================================================================ */

/** @brief Minimum undervoltage threshold (9.0V, unit: 0.1V) */
#define POWERWINDOW_UNDERVOLTAGE_MIN              90

/** @brief Maximum undervoltage threshold (11.0V, unit: 0.1V) */
#define POWERWINDOW_UNDERVOLTAGE_MAX              110

/* ============================================================================
 * Overcurrent Threshold Range
 * ============================================================================ */

/** @brief Minimum overcurrent threshold (15.0A, unit: 0.1A) */
#define POWERWINDOW_OVERCURRENT_MIN               150

/** @brief Maximum overcurrent threshold (30.0A, unit: 0.1A) */
#define POWERWINDOW_OVERCURRENT_MAX               300

/* ============================================================================
 * Undercurrent Threshold Range
 * ============================================================================ */

/** @brief Minimum undercurrent threshold (0.5A, unit: 0.1A) */
#define POWERWINDOW_UNDERCURRENT_MIN              5

/** @brief Maximum undercurrent threshold (5.0A, unit: 0.1A) */
#define POWERWINDOW_UNDERCURRENT_MAX              50

/* ============================================================================
 * Auto Mode Trigger Time Range
 * ============================================================================ */

/** @brief Minimum auto mode trigger time (300ms) */
#define POWERWINDOW_AUTO_TRIGGER_TIME_MIN         300

/** @brief Maximum auto mode trigger time (1000ms) */
#define POWERWINDOW_AUTO_TRIGGER_TIME_MAX         1000

/* ============================================================================
 * Debounce Counts
 * ============================================================================ */

/** @brief Overcurrent debounce count (100ms / 10ms = 10 cycles) */
#define POWERWINDOW_OVERCURRENT_DEBOUNCE_COUNT    10

/** @brief Undercurrent debounce count (100ms / 10ms = 10 cycles) */
#define POWERWINDOW_UNDERCURRENT_DEBOUNCE_COUNT   10

/** @brief Undervoltage debounce count (200ms / 10ms = 20 cycles) */
#define POWERWINDOW_UNDERVOLTAGE_DEBOUNCE_COUNT   20

/** @brief Undervoltage recovery debounce count (500ms / 10ms = 50 cycles) */
#define POWERWINDOW_UNDERVOLTAGE_RECOVERY_COUNT   50

/* ============================================================================
 * System Timing Constants
 * ============================================================================ */

/** @brief Main function cycle time (10ms) */
#define POWERWINDOW_MAIN_CYCLE_TIME_MS            10

/** @brief Position save interval (1000ms) */
#define POWERWINDOW_POSITION_SAVE_INTERVAL_MS     1000

/** @brief Position change threshold for NVM save (5%) */
#define POWERWINDOW_POSITION_CHANGE_THRESHOLD     5

/** @brief Anti-pinch lockout time after event (5000ms) */
#define POWERWINDOW_ANTI_PINCH_LOCKOUT_TIME_MS    5000

/* ============================================================================
 * Position Conversion Constants
 * ============================================================================ */

/** @brief Minimum position percentage */
#define POWERWINDOW_POSITION_PERCENT_MIN          0

/** @brief Maximum position percentage */
#define POWERWINDOW_POSITION_PERCENT_MAX          100

/* ============================================================================
 * Function Declarations
 * ============================================================================ */

/**
 * @brief Initialize calibration parameters with default values
 * 
 * @param[out] params Pointer to calibration parameters structure
 * @return POWERWINDOW_TRUE if initialization successful, POWERWINDOW_FALSE otherwise
 */
PowerWindow_BoolType PowerWindow_Cfg_InitDefaultParams(
    PowerWindow_CalibrationParamsType* params
);

/**
 * @brief Validate calibration parameters
 * 
 * Checks if all calibration parameters are within valid ranges.
 * 
 * @param[in] params Pointer to calibration parameters structure
 * @return POWERWINDOW_TRUE if all parameters are valid, POWERWINDOW_FALSE otherwise
 */
PowerWindow_BoolType PowerWindow_Cfg_ValidateParams(
    const PowerWindow_CalibrationParamsType* params
);

/**
 * @brief Validate anti-pinch current threshold
 * 
 * @param[in] threshold Anti-pinch current threshold value
 * @return POWERWINDOW_TRUE if valid, POWERWINDOW_FALSE otherwise
 */
PowerWindow_BoolType PowerWindow_Cfg_ValidateAntiPinchCurrent(
    PowerWindow_CurrentType threshold
);

/**
 * @brief Validate anti-pinch retract distance
 * 
 * @param[in] distance Anti-pinch retract distance value
 * @return POWERWINDOW_TRUE if valid, POWERWINDOW_FALSE otherwise
 */
PowerWindow_BoolType PowerWindow_Cfg_ValidateRetractDistance(
    PowerWindow_PositionType distance
);

/**
 * @brief Validate position limits
 * 
 * @param[in] upperLimit Position upper limit value
 * @param[in] lowerLimit Position lower limit value
 * @return POWERWINDOW_TRUE if valid, POWERWINDOW_FALSE otherwise
 */
PowerWindow_BoolType PowerWindow_Cfg_ValidatePositionLimits(
    PowerWindow_PositionType upperLimit,
    PowerWindow_PositionType lowerLimit
);

/**
 * @brief Validate undervoltage threshold
 * 
 * @param[in] threshold Undervoltage threshold value
 * @return POWERWINDOW_TRUE if valid, POWERWINDOW_FALSE otherwise
 */
PowerWindow_BoolType PowerWindow_Cfg_ValidateUndervoltageThreshold(
    PowerWindow_VoltageType threshold
);

/**
 * @brief Validate overcurrent threshold
 * 
 * @param[in] threshold Overcurrent threshold value
 * @return POWERWINDOW_TRUE if valid, POWERWINDOW_FALSE otherwise
 */
PowerWindow_BoolType PowerWindow_Cfg_ValidateOvercurrentThreshold(
    PowerWindow_CurrentType threshold
);

/**
 * @brief Validate undercurrent threshold
 * 
 * @param[in] threshold Undercurrent threshold value
 * @return POWERWINDOW_TRUE if valid, POWERWINDOW_FALSE otherwise
 */
PowerWindow_BoolType PowerWindow_Cfg_ValidateUndercurrentThreshold(
    PowerWindow_CurrentType threshold
);

/**
 * @brief Validate auto mode trigger time
 * 
 * @param[in] triggerTime Auto mode trigger time value
 * @return POWERWINDOW_TRUE if valid, POWERWINDOW_FALSE otherwise
 */
PowerWindow_BoolType PowerWindow_Cfg_ValidateAutoTriggerTime(
    uint16_t triggerTime
);

#endif /* POWERWINDOW_CFG_H */
