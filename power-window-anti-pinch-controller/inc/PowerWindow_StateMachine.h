/**
 * @file PowerWindow_StateMachine.h
 * @brief Power Window Anti-Pinch Controller - State Machine Module
 * 
 * This module manages the system state machine with 8 states and implements
 * state transition logic based on system conditions.
 * 
 * @author Power Window Team
 * @version 1.0
 */

#ifndef POWERWINDOW_STATEMACHINE_H
#define POWERWINDOW_STATEMACHINE_H

#include "PowerWindow_Types.h"

/* ============================================================================
 * Function Declarations
 * ============================================================================ */

/**
 * @brief Initialize the state machine
 * 
 * @param None
 * @return None
 */
void PowerWindow_StateMachine_Init(void);

/**
 * @brief Update state machine based on runtime data
 * 
 * Evaluates state transition conditions and updates current state.
 * 
 * @param[in] runtimeData Pointer to runtime data structure
 * @return None
 */
void PowerWindow_StateMachine_Update(const PowerWindow_RuntimeDataType* runtimeData);

/**
 * @brief Get current system state
 * 
 * @return Current system state
 */
PowerWindow_SystemStateType PowerWindow_StateMachine_GetState(void);

/**
 * @brief Request state transition
 * 
 * Requests a transition to target state if valid.
 * 
 * @param[in] targetState Target state to transition to
 * @return None
 */
void PowerWindow_StateMachine_RequestTransition(PowerWindow_SystemStateType targetState);

/**
 * @brief Check if system can accept new commands
 * 
 * Returns true only when in IDLE state.
 * 
 * @return POWERWINDOW_TRUE if can accept commands, POWERWINDOW_FALSE otherwise
 */
PowerWindow_BoolType PowerWindow_StateMachine_CanAcceptCommand(void);

/**
 * @brief Check if system is currently moving
 * 
 * @return POWERWINDOW_TRUE if moving, POWERWINDOW_FALSE otherwise
 */
PowerWindow_BoolType PowerWindow_StateMachine_IsMoving(void);

/**
 * @brief Get previous state
 * 
 * @return Previous system state
 */
PowerWindow_SystemStateType PowerWindow_StateMachine_GetPreviousState(void);

#endif /* POWERWINDOW_STATEMACHINE_H */
