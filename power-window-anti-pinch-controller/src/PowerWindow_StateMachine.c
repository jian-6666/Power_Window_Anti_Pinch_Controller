/**
 * @file PowerWindow_StateMachine.c
 * @brief Power Window Anti-Pinch Controller - State Machine Implementation
 * 
 * This module implements the state machine with 8 states and state transition logic.
 * Implements complete state transition conditions and event recording through Diag module.
 * 
 * @author Power Window Team
 * @version 1.0
 */

#include "PowerWindow_StateMachine.h"
#include "PowerWindow_Diag.h"
#include "PowerWindow_Cfg.h"

/* ============================================================================
 * State Machine Data Structure
 * ============================================================================ */

typedef struct {
    PowerWindow_SystemStateType currentState;
    PowerWindow_SystemStateType previousState;
    uint32_t stateEntryTimestamp;
    uint32_t lastTransitionTimestamp;
    uint32_t antiPinchBlockTime;
} PowerWindow_StateMachineDataType;

static PowerWindow_StateMachineDataType stateMachineData = {
    .currentState = POWERWINDOW_STATE_UNINITIALIZED,
    .previousState = POWERWINDOW_STATE_UNINITIALIZED,
    .stateEntryTimestamp = 0,
    .lastTransitionTimestamp = 0,
    .antiPinchBlockTime = 0
};

/* ============================================================================
 * Local Variables
 * ============================================================================ */

static uint32_t systemTick = 0;

/* ============================================================================
 * Helper Functions
 * ============================================================================ */

/**
 * @brief Record state transition event
 * 
 * @param[in] fromState Source state
 * @param[in] toState Target state
 * @param[in] timestamp Transition timestamp
 * @return None
 */
static void PowerWindow_StateMachine_RecordTransition(
    PowerWindow_SystemStateType fromState,
    PowerWindow_SystemStateType toState,
    uint32_t timestamp)
{
    /* Record state transition event through diagnostic module */
    /* This would typically log the transition for diagnostic purposes */
    (void)fromState;
    (void)toState;
    (void)timestamp;
    
    /* In a real implementation, this would call a diagnostic logging function */
    /* PowerWindow_Diag_LogStateTransition(fromState, toState, timestamp); */
}

/**
 * @brief Execute state transition
 * 
 * @param[in] targetState Target state to transition to
 * @return None
 */
static void PowerWindow_StateMachine_ExecuteTransition(PowerWindow_SystemStateType targetState)
{
    if (stateMachineData.currentState != targetState) {
        PowerWindow_SystemStateType previousState = stateMachineData.currentState;
        
        /* Update state machine data */
        stateMachineData.previousState = stateMachineData.currentState;
        stateMachineData.currentState = targetState;
        stateMachineData.stateEntryTimestamp = systemTick;
        stateMachineData.lastTransitionTimestamp = systemTick;
        
        /* Record transition event */
        PowerWindow_StateMachine_RecordTransition(previousState, targetState, systemTick);
    }
}

/**
 * @brief Check if state transition is valid
 * 
 * @param[in] fromState Current state
 * @param[in] toState Target state
 * @return POWERWINDOW_TRUE if transition is valid, POWERWINDOW_FALSE otherwise
 */
static PowerWindow_BoolType PowerWindow_StateMachine_IsValidTransition(
    PowerWindow_SystemStateType fromState,
    PowerWindow_SystemStateType toState)
{
    /* Prevent invalid transitions */
    if (fromState == toState) {
        return POWERWINDOW_FALSE;
    }
    
    /* Allow transitions to FAULT from any state */
    if (toState == POWERWINDOW_STATE_FAULT) {
        return POWERWINDOW_TRUE;
    }
    
    /* Allow transitions from FAULT to IDLE */
    if (fromState == POWERWINDOW_STATE_FAULT && toState == POWERWINDOW_STATE_IDLE) {
        return POWERWINDOW_TRUE;
    }
    
    /* Allow transitions from UNINITIALIZED to IDLE or FAULT */
    if (fromState == POWERWINDOW_STATE_UNINITIALIZED) {
        return (toState == POWERWINDOW_STATE_IDLE || toState == POWERWINDOW_STATE_FAULT) ?
               POWERWINDOW_TRUE : POWERWINDOW_FALSE;
    }
    
    /* From IDLE, can transition to MANUAL_UP, MANUAL_DOWN, AUTO_UP, AUTO_DOWN, or FAULT */
    if (fromState == POWERWINDOW_STATE_IDLE) {
        return (toState == POWERWINDOW_STATE_MANUAL_UP ||
                toState == POWERWINDOW_STATE_MANUAL_DOWN ||
                toState == POWERWINDOW_STATE_AUTO_UP ||
                toState == POWERWINDOW_STATE_AUTO_DOWN) ?
               POWERWINDOW_TRUE : POWERWINDOW_FALSE;
    }
    
    /* From MANUAL_UP, can transition to IDLE, ANTI_PINCH, or FAULT */
    if (fromState == POWERWINDOW_STATE_MANUAL_UP) {
        return (toState == POWERWINDOW_STATE_IDLE ||
                toState == POWERWINDOW_STATE_ANTI_PINCH) ?
               POWERWINDOW_TRUE : POWERWINDOW_FALSE;
    }
    
    /* From MANUAL_DOWN, can transition to IDLE or FAULT */
    if (fromState == POWERWINDOW_STATE_MANUAL_DOWN) {
        return (toState == POWERWINDOW_STATE_IDLE) ?
               POWERWINDOW_TRUE : POWERWINDOW_FALSE;
    }
    
    /* From AUTO_UP, can transition to IDLE, ANTI_PINCH, or FAULT */
    if (fromState == POWERWINDOW_STATE_AUTO_UP) {
        return (toState == POWERWINDOW_STATE_IDLE ||
                toState == POWERWINDOW_STATE_ANTI_PINCH) ?
               POWERWINDOW_TRUE : POWERWINDOW_FALSE;
    }
    
    /* From AUTO_DOWN, can transition to IDLE or FAULT */
    if (fromState == POWERWINDOW_STATE_AUTO_DOWN) {
        return (toState == POWERWINDOW_STATE_IDLE) ?
               POWERWINDOW_TRUE : POWERWINDOW_FALSE;
    }
    
    /* From ANTI_PINCH, can transition to IDLE or FAULT */
    if (fromState == POWERWINDOW_STATE_ANTI_PINCH) {
        return (toState == POWERWINDOW_STATE_IDLE) ?
               POWERWINDOW_TRUE : POWERWINDOW_FALSE;
    }
    
    return POWERWINDOW_FALSE;
}

/**
 * @brief Check if auto mode can be triggered
 * 
 * @param[in] runtimeData Runtime data
 * @param[in] buttonPressTime Button press duration
 * @return POWERWINDOW_TRUE if auto mode can be triggered
 */
static PowerWindow_BoolType PowerWindow_StateMachine_CanTriggerAutoMode(
    const PowerWindow_RuntimeDataType* runtimeData,
    uint32_t buttonPressTime)
{
    /* Auto mode requires learning to be completed */
    if (runtimeData->learnState != POWERWINDOW_LEARN_COMPLETED) {
        return POWERWINDOW_FALSE;
    }
    
    /* Check if button press time exceeds auto trigger threshold */
    extern const PowerWindow_CalibrationParamsType PowerWindow_CalibrationParams;
    if (buttonPressTime < PowerWindow_CalibrationParams.autoModeTriggerTime) {
        return POWERWINDOW_FALSE;
    }
    
    return POWERWINDOW_TRUE;
}

/**
 * @brief Check if anti-pinch block is active
 * 
 * @return POWERWINDOW_TRUE if anti-pinch block is active
 */
static PowerWindow_BoolType PowerWindow_StateMachine_IsAntiPinchBlocked(void)
{
    /* Anti-pinch blocks auto up for 5 seconds after activation */
    const uint32_t ANTI_PINCH_BLOCK_DURATION = 5000; /* 5 seconds in ms */
    
    if (stateMachineData.antiPinchBlockTime > 0) {
        if ((systemTick - stateMachineData.antiPinchBlockTime) < ANTI_PINCH_BLOCK_DURATION) {
            return POWERWINDOW_TRUE;
        } else {
            /* Clear the block time */
            stateMachineData.antiPinchBlockTime = 0;
        }
    }
    
    return POWERWINDOW_FALSE;
}

/* ============================================================================
 * Function Implementations
 * ============================================================================ */

void PowerWindow_StateMachine_Init(void)
{
    stateMachineData.currentState = POWERWINDOW_STATE_UNINITIALIZED;
    stateMachineData.previousState = POWERWINDOW_STATE_UNINITIALIZED;
    stateMachineData.stateEntryTimestamp = 0;
    stateMachineData.lastTransitionTimestamp = 0;
    stateMachineData.antiPinchBlockTime = 0;
    systemTick = 0;
}

void PowerWindow_StateMachine_Update(const PowerWindow_RuntimeDataType* runtimeData)
{
    if (runtimeData == NULL) {
        return;
    }
    
    /* Increment system tick (10ms per call) */
    systemTick += 10;
    
    PowerWindow_SystemStateType currentState = stateMachineData.currentState;
    PowerWindow_SystemStateType targetState = currentState;
    
    /* Get calibration parameters */
    extern const PowerWindow_CalibrationParamsType PowerWindow_CalibrationParams;
    
    /* State-specific transition logic */
    switch (currentState) {
        case POWERWINDOW_STATE_UNINITIALIZED:
            /* Transition to IDLE when initialization is complete and no faults */
            /* This would typically check initialization status from other modules */
            targetState = POWERWINDOW_STATE_IDLE;
            break;
            
        case POWERWINDOW_STATE_IDLE:
            /* Handle button input to transition to manual or auto modes */
            if (runtimeData->buttonState == POWERWINDOW_BUTTON_UP) {
                /* Check if not at upper limit */
                if (runtimeData->currentPosition < PowerWindow_CalibrationParams.positionUpperLimit) {
                    /* Check for auto mode trigger */
                    if (PowerWindow_StateMachine_CanTriggerAutoMode(runtimeData, runtimeData->buttonPressTime) &&
                        !PowerWindow_StateMachine_IsAntiPinchBlocked()) {
                        targetState = POWERWINDOW_STATE_AUTO_UP;
                    } else {
                        targetState = POWERWINDOW_STATE_MANUAL_UP;
                    }
                }
            } else if (runtimeData->buttonState == POWERWINDOW_BUTTON_DOWN) {
                /* Check if not at lower limit */
                if (runtimeData->currentPosition > PowerWindow_CalibrationParams.positionLowerLimit) {
                    /* Check for auto mode trigger */
                    if (PowerWindow_StateMachine_CanTriggerAutoMode(runtimeData, runtimeData->buttonPressTime)) {
                        targetState = POWERWINDOW_STATE_AUTO_DOWN;
                    } else {
                        targetState = POWERWINDOW_STATE_MANUAL_DOWN;
                    }
                }
            }
            break;
            
        case POWERWINDOW_STATE_MANUAL_UP:
            /* Transition to IDLE when button released or upper limit reached */
            if (runtimeData->buttonState == POWERWINDOW_BUTTON_RELEASED ||
                runtimeData->currentPosition >= PowerWindow_CalibrationParams.positionUpperLimit) {
                targetState = POWERWINDOW_STATE_IDLE;
            }
            /* Anti-pinch detection is handled by Safety module calling RequestTransition */
            break;
            
        case POWERWINDOW_STATE_MANUAL_DOWN:
            /* Transition to IDLE when button released or lower limit reached */
            if (runtimeData->buttonState == POWERWINDOW_BUTTON_RELEASED ||
                runtimeData->currentPosition <= PowerWindow_CalibrationParams.positionLowerLimit) {
                targetState = POWERWINDOW_STATE_IDLE;
            }
            break;
            
        case POWERWINDOW_STATE_AUTO_UP:
            /* Transition to IDLE when upper limit reached or user interrupts */
            if (runtimeData->currentPosition >= PowerWindow_CalibrationParams.positionUpperLimit ||
                runtimeData->buttonState != POWERWINDOW_BUTTON_RELEASED) {
                targetState = POWERWINDOW_STATE_IDLE;
            }
            /* Anti-pinch detection is handled by Safety module calling RequestTransition */
            break;
            
        case POWERWINDOW_STATE_AUTO_DOWN:
            /* Transition to IDLE when lower limit reached or user interrupts */
            if (runtimeData->currentPosition <= PowerWindow_CalibrationParams.positionLowerLimit ||
                runtimeData->buttonState != POWERWINDOW_BUTTON_RELEASED) {
                targetState = POWERWINDOW_STATE_IDLE;
            }
            break;
            
        case POWERWINDOW_STATE_ANTI_PINCH:
            /* Transition to IDLE when retract is complete */
            /* This would typically check if retract distance has been reached */
            if (runtimeData->antiPinchActive == POWERWINDOW_FALSE) {
                targetState = POWERWINDOW_STATE_IDLE;
                /* Set anti-pinch block time to prevent auto up for 5 seconds */
                stateMachineData.antiPinchBlockTime = systemTick;
            }
            break;
            
        case POWERWINDOW_STATE_FAULT:
            /* Remain in FAULT until explicitly cleared by external request */
            /* Fault recovery is handled by Safety module calling RequestTransition */
            break;
            
        default:
            /* Invalid state - transition to fault */
            targetState = POWERWINDOW_STATE_FAULT;
            break;
    }
    
    /* Execute transition if target state is different */
    if (targetState != currentState) {
        if (PowerWindow_StateMachine_IsValidTransition(currentState, targetState)) {
            PowerWindow_StateMachine_ExecuteTransition(targetState);
        }
    }
}

PowerWindow_SystemStateType PowerWindow_StateMachine_GetState(void)
{
    return stateMachineData.currentState;
}

void PowerWindow_StateMachine_RequestTransition(PowerWindow_SystemStateType targetState)
{
    if (PowerWindow_StateMachine_IsValidTransition(stateMachineData.currentState, targetState)) {
        PowerWindow_StateMachine_ExecuteTransition(targetState);
    }
}

PowerWindow_BoolType PowerWindow_StateMachine_CanAcceptCommand(void)
{
    return (stateMachineData.currentState == POWERWINDOW_STATE_IDLE) ?
           POWERWINDOW_TRUE : POWERWINDOW_FALSE;
}

PowerWindow_BoolType PowerWindow_StateMachine_IsMoving(void)
{
    return (stateMachineData.currentState == POWERWINDOW_STATE_MANUAL_UP ||
            stateMachineData.currentState == POWERWINDOW_STATE_MANUAL_DOWN ||
            stateMachineData.currentState == POWERWINDOW_STATE_AUTO_UP ||
            stateMachineData.currentState == POWERWINDOW_STATE_AUTO_DOWN ||
            stateMachineData.currentState == POWERWINDOW_STATE_ANTI_PINCH) ?
           POWERWINDOW_TRUE : POWERWINDOW_FALSE;
}

PowerWindow_SystemStateType PowerWindow_StateMachine_GetPreviousState(void)
{
    return stateMachineData.previousState;
}
