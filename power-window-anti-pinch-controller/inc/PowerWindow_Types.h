/**
 * @file PowerWindow_Types.h
 * @brief Power Window Anti-Pinch Controller - Type Definitions
 * 
 * This file defines all basic types, enumerations, and structures used throughout
 * the Power Window Anti-Pinch Controller system.
 * 
 * @author Power Window Team
 * @version 1.0
 */

#ifndef POWERWINDOW_TYPES_H
#define POWERWINDOW_TYPES_H

#include <stdint.h>
#include <stdbool.h>

/* ============================================================================
 * Basic Type Definitions
 * ============================================================================ */

/** @brief Window state type */
typedef uint8_t PowerWindow_StateType;

/** @brief Window position type (0-4095) */
typedef uint16_t PowerWindow_PositionType;

/** @brief Position percentage type (0-100) */
typedef uint8_t PowerWindow_PercentType;

/** @brief Motor current type (unit: 0.1A) */
typedef uint16_t PowerWindow_CurrentType;

/** @brief System voltage type (unit: 0.1V) */
typedef uint16_t PowerWindow_VoltageType;

/** @brief Diagnostic Trouble Code type */
typedef uint16_t PowerWindow_DTCType;

/** @brief Boolean type */
typedef uint8_t PowerWindow_BoolType;

#define POWERWINDOW_TRUE  ((PowerWindow_BoolType)1)
#define POWERWINDOW_FALSE ((PowerWindow_BoolType)0)

/* ============================================================================
 * Enumeration Types
 * ============================================================================ */

/**
 * @brief Button state enumeration
 */
typedef enum {
    POWERWINDOW_BUTTON_RELEASED = 0,  /**< Button released */
    POWERWINDOW_BUTTON_UP       = 1,  /**< Up button pressed */
    POWERWINDOW_BUTTON_DOWN     = 2   /**< Down button pressed */
} PowerWindow_ButtonStateType;

/**
 * @brief Motor direction enumeration
 */
typedef enum {
    POWERWINDOW_MOTOR_STOP = 0,  /**< Motor stopped */
    POWERWINDOW_MOTOR_UP   = 1,  /**< Motor moving up */
    POWERWINDOW_MOTOR_DOWN = 2   /**< Motor moving down */
} PowerWindow_MotorDirectionType;

/**
 * @brief System state enumeration
 */
typedef enum {
    POWERWINDOW_STATE_UNINITIALIZED = 0,  /**< Uninitialized state */
    POWERWINDOW_STATE_IDLE          = 1,  /**< Idle state */
    POWERWINDOW_STATE_MANUAL_UP     = 2,  /**< Manual up state */
    POWERWINDOW_STATE_MANUAL_DOWN   = 3,  /**< Manual down state */
    POWERWINDOW_STATE_AUTO_UP       = 4,  /**< Auto up state */
    POWERWINDOW_STATE_AUTO_DOWN     = 5,  /**< Auto down state */
    POWERWINDOW_STATE_ANTI_PINCH    = 6,  /**< Anti-pinch state */
    POWERWINDOW_STATE_FAULT         = 7   /**< Fault state */
} PowerWindow_SystemStateType;

/**
 * @brief Diagnostic Trouble Code enumeration
 */
typedef enum {
    POWERWINDOW_DTC_NO_FAULT           = 0x0000,  /**< No fault */
    POWERWINDOW_DTC_POSITION_SENSOR    = 0x0001,  /**< Position sensor fault */
    POWERWINDOW_DTC_OVERCURRENT        = 0x0002,  /**< Overcurrent fault */
    POWERWINDOW_DTC_UNDERCURRENT       = 0x0003,  /**< Undercurrent fault */
    POWERWINDOW_DTC_UNDERVOLTAGE       = 0x0004,  /**< Undervoltage fault */
    POWERWINDOW_DTC_NVM_READ_FAIL      = 0x0005,  /**< NVM read failure */
    POWERWINDOW_DTC_NVM_WRITE_FAIL     = 0x0006,  /**< NVM write failure */
    POWERWINDOW_DTC_CALIBRATION_ERROR  = 0x0007   /**< Calibration error */
} PowerWindow_DTCCodeType;

/**
 * @brief Learning state enumeration
 */
typedef enum {
    POWERWINDOW_LEARN_NOT_STARTED = 0,  /**< Learning not started */
    POWERWINDOW_LEARN_IN_PROGRESS = 1,  /**< Learning in progress */
    POWERWINDOW_LEARN_COMPLETED   = 2,  /**< Learning completed */
    POWERWINDOW_LEARN_FAILED      = 3   /**< Learning failed */
} PowerWindow_LearnStateType;

/* ============================================================================
 * Structure Types
 * ============================================================================ */

/**
 * @brief Calibration parameters structure
 * 
 * Contains all configurable parameters for the power window system.
 */
typedef struct {
    /** @brief Anti-pinch current threshold (5A-20A, unit: 0.1A) */
    PowerWindow_CurrentType antiPinchCurrentThreshold;
    
    /** @brief Anti-pinch retract distance (50mm-200mm) */
    PowerWindow_PositionType antiPinchRetractDistance;
    
    /** @brief Position upper limit (0-4095) */
    PowerWindow_PositionType positionUpperLimit;
    
    /** @brief Position lower limit (0-4095) */
    PowerWindow_PositionType positionLowerLimit;
    
    /** @brief Undervoltage threshold (9V-11V, unit: 0.1V) */
    PowerWindow_VoltageType undervoltageThreshold;
    
    /** @brief Overcurrent threshold (15A-30A, unit: 0.1A) */
    PowerWindow_CurrentType overcurrentThreshold;
    
    /** @brief Undercurrent threshold (0.5A-5.0A, unit: 0.1A) */
    PowerWindow_CurrentType undercurrentThreshold;
    
    /** @brief Auto mode trigger time (300ms-1000ms) */
    uint16_t autoModeTriggerTime;
} PowerWindow_CalibrationParamsType;

/**
 * @brief Runtime data structure
 * 
 * Contains all runtime state information of the power window system.
 */
typedef struct {
    /** @brief Current system state */
    PowerWindow_SystemStateType currentState;
    
    /** @brief Current window position (raw sensor value) */
    PowerWindow_PositionType currentPosition;
    
    /** @brief Current position as percentage (0-100%) */
    PowerWindow_PercentType currentPositionPercent;
    
    /** @brief Motor current (unit: 0.1A) */
    PowerWindow_CurrentType motorCurrent;
    
    /** @brief System voltage (unit: 0.1V) */
    PowerWindow_VoltageType systemVoltage;
    
    /** @brief Current button state */
    PowerWindow_ButtonStateType buttonState;
    
    /** @brief Current motor direction */
    PowerWindow_MotorDirectionType motorDirection;
    
    /** @brief Learning state */
    PowerWindow_LearnStateType learnState;
    
    /** @brief Anti-pinch active flag */
    PowerWindow_BoolType antiPinchActive;
    
    /** @brief Button press time (milliseconds) */
    uint32_t buttonPressTime;
    
    /** @brief Fault debounce counter */
    uint32_t faultDebounceCounter;
} PowerWindow_RuntimeDataType;

/**
 * @brief NVM data structure
 * 
 * Contains all data that needs to be persisted in non-volatile memory.
 */
typedef struct {
    /** @brief Saved window position */
    PowerWindow_PositionType savedPosition;
    
    /** @brief Learned upper limit position */
    PowerWindow_PositionType learnedUpperLimit;
    
    /** @brief Learned lower limit position */
    PowerWindow_PositionType learnedLowerLimit;
    
    /** @brief Learning state */
    PowerWindow_LearnStateType learnState;
    
    /** @brief Active diagnostic trouble codes (max 8) */
    PowerWindow_DTCType activeDTCs[8];
    
    /** @brief History diagnostic trouble codes (max 16) */
    PowerWindow_DTCType historyDTCs[16];
    
    /** @brief CRC32 checksum for data integrity */
    uint32_t crc;
} PowerWindow_NvMDataType;

#endif /* POWERWINDOW_TYPES_H */
