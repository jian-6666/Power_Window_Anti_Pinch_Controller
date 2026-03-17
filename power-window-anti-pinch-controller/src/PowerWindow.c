/**
 * @file PowerWindow.c
 * @brief Power Window Anti-Pinch Controller - Main Control Implementation
 * 
 * This module implements the main control logic for the power window system.
 * 
 * @author Power Window Team
 * @version 1.0
 */

#include "PowerWindow.h"
#include "PowerWindow_Interface.h"
#include "PowerWindow_StateMachine.h"
#include "PowerWindow_Safety.h"
#include "PowerWindow_NvM.h"
#include "PowerWindow_Diag.h"
#include "PowerWindow_Cfg.h"
#include <string.h>

/* ============================================================================
 * System Data Structure
 * ============================================================================ */

typedef struct {
    PowerWindow_RuntimeDataType runtimeData;
    PowerWindow_CalibrationParamsType calibParams;
    PowerWindow_NvMDataType nvmData;
    uint32_t lastPositionSaveTime;
    uint32_t antiPinchLockoutTime;
    uint32_t mainFunctionCallCount;
    uint32_t executionTimeMin;
    uint32_t executionTimeMax;
    uint32_t executionTimeSum;
    PowerWindow_BoolType learningInProgress;
    PowerWindow_BoolType learningUpperLimitReached;
} PowerWindow_SystemDataType;

static PowerWindow_SystemDataType systemData = {
    .lastPositionSaveTime = 0,
    .antiPinchLockoutTime = 0,
    .mainFunctionCallCount = 0,
    .executionTimeMin = 0xFFFFFFFFU,
    .executionTimeMax = 0,
    .executionTimeSum = 0,
    .learningInProgress = POWERWINDOW_FALSE,
    .learningUpperLimitReached = POWERWINDOW_FALSE
};

/* ============================================================================
 * Helper Functions
 * ============================================================================ */

/**
 * @brief Convert raw position to percentage
 * 
 * @param[in] rawPosition Raw position value (0-4095)
 * @return Position percentage (0-100%)
 */
static PowerWindow_PercentType PowerWindow_ConvertPositionToPercent(
    PowerWindow_PositionType rawPosition)
{
    PowerWindow_PositionType upperLimit = systemData.calibParams.positionUpperLimit;
    PowerWindow_PositionType lowerLimit = systemData.calibParams.positionLowerLimit;
    
    if (upperLimit <= lowerLimit) {
        return 0;
    }
    
    if (rawPosition <= lowerLimit) {
        return 0;
    }
    
    if (rawPosition >= upperLimit) {
        return 100;
    }
    
    uint32_t range = upperLimit - lowerLimit;
    uint32_t offset = rawPosition - lowerLimit;
    uint32_t percent = (offset * 100) / range;
    
    return (PowerWindow_PercentType)percent;
}

/**
 * @brief Read all inputs from hardware interface
 * 
 * @return None
 */
static void PowerWindow_ReadInputs(void)
{
    systemData.runtimeData.buttonState = PowerWindow_Interface_ReadButton();
    systemData.runtimeData.currentPosition = PowerWindow_Interface_ReadPosition();
    systemData.runtimeData.motorCurrent = PowerWindow_Interface_ReadCurrent();
    systemData.runtimeData.systemVoltage = PowerWindow_Interface_ReadVoltage();
    
    /* Update position percentage */
    systemData.runtimeData.currentPositionPercent = 
        PowerWindow_ConvertPositionToPercent(systemData.runtimeData.currentPosition);
}

/**
 * @brief Update motor output based on current state
 * 
 * @return None
 */
static void PowerWindow_UpdateMotorOutput(void)
{
    PowerWindow_MotorDirectionType motorDirection = POWERWINDOW_MOTOR_STOP;
    
    switch (systemData.runtimeData.currentState) {
        case POWERWINDOW_STATE_MANUAL_UP:
        case POWERWINDOW_STATE_AUTO_UP:
            if (systemData.runtimeData.currentPosition < systemData.calibParams.positionUpperLimit) {
                motorDirection = POWERWINDOW_MOTOR_UP;
            }
            break;
            
        case POWERWINDOW_STATE_MANUAL_DOWN:
        case POWERWINDOW_STATE_AUTO_DOWN:
            if (systemData.runtimeData.currentPosition > systemData.calibParams.positionLowerLimit) {
                motorDirection = POWERWINDOW_MOTOR_DOWN;
            }
            break;
            
        case POWERWINDOW_STATE_ANTI_PINCH:
            motorDirection = POWERWINDOW_MOTOR_DOWN;
            break;
            
        default:
            motorDirection = POWERWINDOW_MOTOR_STOP;
            break;
    }
    
    systemData.runtimeData.motorDirection = motorDirection;
    PowerWindow_Interface_SetMotor(motorDirection);
}

/**
 * @brief Handle anti-pinch event
 * 
 * @return None
 */
static void PowerWindow_HandleAntiPinch(void)
{
    if (systemData.runtimeData.antiPinchActive) {
        /* Record anti-pinch event in diagnostics */
        PowerWindow_Diag_SetDTC(POWERWINDOW_DTC_NO_FAULT);  /* Event marker */
        
        /* Set lockout time to prevent immediate re-triggering */
        systemData.antiPinchLockoutTime = POWERWINDOW_ANTI_PINCH_LOCKOUT_TIME_MS / 
                                          POWERWINDOW_MAIN_CYCLE_TIME_MS;
    }
    
    /* Decrement lockout time if active */
    if (systemData.antiPinchLockoutTime > 0) {
        systemData.antiPinchLockoutTime--;
    }
}

/**
 * @brief Check and handle learning procedure
 * 
 * @return None
 */
static void PowerWindow_HandleLearning(void)
{
    if (!systemData.learningInProgress) {
        return;
    }
    
    /* Learning state machine */
    if (!systemData.learningUpperLimitReached) {
        /* Phase 1: Drive to upper limit */
        if (systemData.runtimeData.currentPosition >= systemData.calibParams.positionUpperLimit) {
            systemData.nvmData.learnedUpperLimit = systemData.runtimeData.currentPosition;
            systemData.learningUpperLimitReached = POWERWINDOW_TRUE;
        }
    } else {
        /* Phase 2: Drive to lower limit */
        if (systemData.runtimeData.currentPosition <= systemData.calibParams.positionLowerLimit) {
            systemData.nvmData.learnedLowerLimit = systemData.runtimeData.currentPosition;
            systemData.nvmData.learnState = POWERWINDOW_LEARN_COMPLETED;
            systemData.learningInProgress = POWERWINDOW_FALSE;
            systemData.learningUpperLimitReached = POWERWINDOW_FALSE;
            
            /* Save learning data to NVM */
            PowerWindow_NvM_SaveData(&systemData.nvmData);
        }
    }
}

/**
 * @brief Check if anti-pinch lockout is active (internal)
 * 
 * @return POWERWINDOW_TRUE if lockout is active, POWERWINDOW_FALSE otherwise
 */
static PowerWindow_BoolType PowerWindow_IsAntiPinchLockoutActive_Internal(void)
{
    return (systemData.antiPinchLockoutTime > 0) ? POWERWINDOW_TRUE : POWERWINDOW_FALSE;
}

/**
 * @brief Process button input and update button press time
 * 
 * @return None
 */
static void PowerWindow_ProcessButtonInput(void)
{
    static PowerWindow_ButtonStateType previousButtonState = POWERWINDOW_BUTTON_RELEASED;
    PowerWindow_ButtonStateType currentButtonState = systemData.runtimeData.buttonState;
    
    /* Handle button press time tracking */
    if (currentButtonState != POWERWINDOW_BUTTON_RELEASED) {
        if (previousButtonState == POWERWINDOW_BUTTON_RELEASED) {
            /* Button just pressed - reset timer */
            systemData.runtimeData.buttonPressTime = 0;
        } else if (currentButtonState == previousButtonState) {
            /* Button held - increment timer */
            systemData.runtimeData.buttonPressTime += POWERWINDOW_MAIN_CYCLE_TIME_MS;
        } else {
            /* Button changed direction - reset timer */
            systemData.runtimeData.buttonPressTime = 0;
        }
    } else {
        /* Button released - reset timer */
        systemData.runtimeData.buttonPressTime = 0;
    }
    
    previousButtonState = currentButtonState;
}
/**
 * @brief Check and save position to NVM if needed
 * 
 * @return None
 */
static void PowerWindow_CheckAndSavePosition(void)
{
    systemData.lastPositionSaveTime++;
    
    if (systemData.lastPositionSaveTime >= (POWERWINDOW_POSITION_SAVE_INTERVAL_MS / 
                                             POWERWINDOW_MAIN_CYCLE_TIME_MS)) {
        systemData.lastPositionSaveTime = 0;
        
        systemData.nvmData.savedPosition = systemData.runtimeData.currentPosition;
        PowerWindow_NvM_SaveData(&systemData.nvmData);
    }
}

/* ============================================================================
 * Function Implementations
 * ============================================================================ */

void PowerWindow_Init(void)
{
    /* Initialize all subsystems */
    PowerWindow_Interface_Init();
    PowerWindow_StateMachine_Init();
    PowerWindow_Safety_Init();
    PowerWindow_NvM_Init();
    PowerWindow_Diag_Init();
    
    /* Initialize calibration parameters */
    PowerWindow_Cfg_InitDefaultParams(&systemData.calibParams);
    
    /* Load data from NVM */
    if (PowerWindow_NvM_LoadData(&systemData.nvmData)) {
        systemData.runtimeData.currentPosition = systemData.nvmData.savedPosition;
    } else {
        systemData.nvmData.savedPosition = 2048;
        systemData.nvmData.learnState = POWERWINDOW_LEARN_NOT_STARTED;
        systemData.nvmData.learnedUpperLimit = systemData.calibParams.positionUpperLimit;
        systemData.nvmData.learnedLowerLimit = systemData.calibParams.positionLowerLimit;
    }
    
    /* Initialize runtime data */
    systemData.runtimeData.currentState = POWERWINDOW_STATE_UNINITIALIZED;
    systemData.runtimeData.learnState = systemData.nvmData.learnState;
    systemData.runtimeData.currentPositionPercent = 
        PowerWindow_ConvertPositionToPercent(systemData.runtimeData.currentPosition);
    
    /* Transition to IDLE state */
    PowerWindow_StateMachine_RequestTransition(POWERWINDOW_STATE_IDLE);
    systemData.runtimeData.currentState = PowerWindow_StateMachine_GetState();
}

void PowerWindow_MainFunction_10ms(void)
{
    uint32_t startTime = 0; /* In a real system, this would be a hardware timer */
    
    /* 1. Read inputs */
    PowerWindow_ReadInputs();
    
    /* 2. Process button input and update button press time */
    PowerWindow_ProcessButtonInput();
    
    /* 3. Monitor safety */
    systemData.runtimeData.antiPinchActive = PowerWindow_Safety_MonitorAntiPinch(
        systemData.runtimeData.motorCurrent,
        systemData.runtimeData.currentState,
        systemData.calibParams.antiPinchCurrentThreshold
    );
    
    PowerWindow_Safety_MonitorCurrent(
        systemData.runtimeData.motorCurrent,
        systemData.runtimeData.currentState,
        systemData.calibParams.overcurrentThreshold,
        systemData.calibParams.undercurrentThreshold
    );
    
    PowerWindow_Safety_MonitorVoltage(
        systemData.runtimeData.systemVoltage,
        systemData.calibParams.undervoltageThreshold
    );
    
    /* Check for faults and transition to FAULT state if needed */
    if (PowerWindow_Safety_CheckFaults()) {
        PowerWindow_StateMachine_RequestTransition(POWERWINDOW_STATE_FAULT);
    }
    
    /* 4. Update state machine */
    systemData.runtimeData.currentState = PowerWindow_StateMachine_GetState();
    PowerWindow_StateMachine_Update(&systemData.runtimeData);
    systemData.runtimeData.currentState = PowerWindow_StateMachine_GetState();
    
    /* 5. Update motor output */
    PowerWindow_UpdateMotorOutput();
    
    /* 6. Handle learning procedure */
    PowerWindow_HandleLearning();
    
    /* 7. Handle anti-pinch events */
    PowerWindow_HandleAntiPinch();
    
    /* 8. Update diagnostics */
    PowerWindow_Diag_UpdateData(
        systemData.runtimeData.currentPositionPercent,
        systemData.runtimeData.currentState,
        systemData.runtimeData.learnState
    );
    
    /* 9. Check and save position to NVM */
    PowerWindow_CheckAndSavePosition();
    
    /* 10. Update execution statistics */
    systemData.mainFunctionCallCount++;
    
    /* Calculate execution time (placeholder - in real system would use hardware timer) */
    uint32_t executionTime = 1; /* Placeholder value */
    if (executionTime < systemData.executionTimeMin) {
        systemData.executionTimeMin = executionTime;
    }
    if (executionTime > systemData.executionTimeMax) {
        systemData.executionTimeMax = executionTime;
    }
    systemData.executionTimeSum += executionTime;
}

void PowerWindow_SetButtonState(PowerWindow_ButtonStateType buttonState)
{
    systemData.runtimeData.buttonState = buttonState;
#ifdef POWERWINDOW_TEST_MODE
    PowerWindow_Interface_Mock_SetButton(buttonState);
#endif
}

void PowerWindow_TriggerLearnProcedure(void)
{
    if (systemData.runtimeData.currentState == POWERWINDOW_STATE_IDLE) {
        systemData.learningInProgress = POWERWINDOW_TRUE;
        systemData.learningUpperLimitReached = POWERWINDOW_FALSE;
        systemData.nvmData.learnState = POWERWINDOW_LEARN_IN_PROGRESS;
        systemData.runtimeData.learnState = POWERWINDOW_LEARN_IN_PROGRESS;
        
        /* Start driving to upper limit */
        PowerWindow_StateMachine_RequestTransition(POWERWINDOW_STATE_AUTO_UP);
    }
}

void PowerWindow_StopMotor(void)
{
    PowerWindow_Interface_SetMotor(POWERWINDOW_MOTOR_STOP);
    systemData.runtimeData.motorDirection = POWERWINDOW_MOTOR_STOP;
}

PowerWindow_SystemStateType PowerWindow_GetCurrentState(void)
{
    return systemData.runtimeData.currentState;
}

PowerWindow_PercentType PowerWindow_GetPositionPercent(void)
{
    return systemData.runtimeData.currentPositionPercent;
}

PowerWindow_PositionType PowerWindow_GetRawPosition(void)
{
    return systemData.runtimeData.currentPosition;
}

PowerWindow_BoolType PowerWindow_IsLearnCompleted(void)
{
    return (systemData.runtimeData.learnState == POWERWINDOW_LEARN_COMPLETED) ?
           POWERWINDOW_TRUE : POWERWINDOW_FALSE;
}

PowerWindow_BoolType PowerWindow_IsAntiPinchLockoutActive(void)
{
    return PowerWindow_IsAntiPinchLockoutActive_Internal();
}

uint32_t PowerWindow_GetMainFunctionCallCount(void)
{
    return systemData.mainFunctionCallCount;
}

void PowerWindow_GetExecutionTimeStats(uint32_t* minTime, uint32_t* maxTime, uint32_t* avgTime)
{
    if (minTime != NULL) {
        *minTime = systemData.executionTimeMin;
    }
    if (maxTime != NULL) {
        *maxTime = systemData.executionTimeMax;
    }
    if (avgTime != NULL && systemData.mainFunctionCallCount > 0) {
        *avgTime = systemData.executionTimeSum / systemData.mainFunctionCallCount;
    } else if (avgTime != NULL) {
        *avgTime = 0;
    }
}
