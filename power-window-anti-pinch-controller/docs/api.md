# Power Window Anti-Pinch Controller - API Reference

## Overview

This document provides complete API reference for the Power Window Anti-Pinch Controller system. The system is organized into 6 modules, each with well-defined interfaces.

## Table of Contents

1. [PowerWindow (Main Module)](#powerwindow-main-module)
2. [PowerWindow_StateMachine](#powerwindow_statemachine)
3. [PowerWindow_Safety](#powerwindow_safety)
4. [PowerWindow_NvM](#powerwindow_nvm)
5. [PowerWindow_Diag](#powerwindow_diag)
6. [PowerWindow_Interface](#powerwindow_interface)
7. [PowerWindow_Cfg](#powerwindow_cfg)
8. [Type Definitions](#type-definitions)

---

## PowerWindow (Main Module)

### Overview

The main control module that coordinates all subsystems and implements the core power window control logic.

### Functions

#### PowerWindow_Init()

```c
void PowerWindow_Init(void);
```

**Description**: Initializes the Power Window system and all subsystems.

**Parameters**: None

**Return Value**: None

**Behavior**:
- Initializes all subsystems (Interface, StateMachine, Safety, NvM, Diag)
- Loads calibration parameters
- Loads data from NVM
- Transitions to IDLE state

**Example**:
```c
PowerWindow_Init();
```

---

#### PowerWindow_MainFunction_10ms()

```c
void PowerWindow_MainFunction_10ms(void);
```

**Description**: Main periodic function called every 10ms by the AUTOSAR runtime.

**Parameters**: None

**Return Value**: None

**Behavior**:
1. Reads all inputs (button, position, current, voltage)
2. Monitors safety conditions
3. Updates state machine
4. Controls motor output
5. Handles learning procedure
6. Updates diagnostics
7. Conditionally saves data to NVM

**Example**:
```c
/* Called periodically by AUTOSAR RTE */
PowerWindow_MainFunction_10ms();
```

---

#### PowerWindow_SetButtonState()

```c
void PowerWindow_SetButtonState(PowerWindow_ButtonStateType buttonState);
```

**Description**: Sets the button state for manual control.

**Parameters**:
- `buttonState`: Button state (RELEASED, UP, or DOWN)

**Return Value**: None

**Example**:
```c
PowerWindow_SetButtonState(POWERWINDOW_BUTTON_UP);
```

---

#### PowerWindow_TriggerLearnProcedure()

```c
void PowerWindow_TriggerLearnProcedure(void);
```

**Description**: Triggers the window position learning procedure.

**Parameters**: None

**Return Value**: None

**Behavior**:
- Only works when system is in IDLE state
- Drives window to upper limit and records position
- Drives window to lower limit and records position
- Saves learned positions to NVM

**Example**:
```c
PowerWindow_TriggerLearnProcedure();
```

---

#### PowerWindow_StopMotor()

```c
void PowerWindow_StopMotor(void);
```

**Description**: Immediately stops the motor.

**Parameters**: None

**Return Value**: None

**Example**:
```c
PowerWindow_StopMotor();
```

---

#### PowerWindow_GetCurrentState()

```c
PowerWindow_SystemStateType PowerWindow_GetCurrentState(void);
```

**Description**: Gets the current system state.

**Parameters**: None

**Return Value**: Current system state

**Possible Values**:
- POWERWINDOW_STATE_UNINITIALIZED
- POWERWINDOW_STATE_IDLE
- POWERWINDOW_STATE_MANUAL_UP
- POWERWINDOW_STATE_MANUAL_DOWN
- POWERWINDOW_STATE_AUTO_UP
- POWERWINDOW_STATE_AUTO_DOWN
- POWERWINDOW_STATE_ANTI_PINCH
- POWERWINDOW_STATE_FAULT

**Example**:
```c
PowerWindow_SystemStateType state = PowerWindow_GetCurrentState();
if (state == POWERWINDOW_STATE_IDLE) {
    /* System is ready for commands */
}
```

---

#### PowerWindow_GetPositionPercent()

```c
PowerWindow_PercentType PowerWindow_GetPositionPercent(void);
```

**Description**: Gets the current window position as a percentage.

**Parameters**: None

**Return Value**: Position percentage (0-100%)

**Example**:
```c
PowerWindow_PercentType percent = PowerWindow_GetPositionPercent();
printf("Window position: %d%%\n", percent);
```

---

#### PowerWindow_GetRawPosition()

```c
PowerWindow_PositionType PowerWindow_GetRawPosition(void);
```

**Description**: Gets the current window position as raw sensor value.

**Parameters**: None

**Return Value**: Raw position value (0-4095)

**Example**:
```c
PowerWindow_PositionType rawPos = PowerWindow_GetRawPosition();
```

---

#### PowerWindow_IsLearnCompleted()

```c
PowerWindow_BoolType PowerWindow_IsLearnCompleted(void);
```

**Description**: Checks if window position learning is completed.

**Parameters**: None

**Return Value**: POWERWINDOW_TRUE if learning completed, POWERWINDOW_FALSE otherwise

**Example**:
```c
if (PowerWindow_IsLearnCompleted()) {
    /* Auto mode is available */
}
```

---

## PowerWindow_StateMachine

### Overview

Manages the system state machine with 8 states and implements state transition logic.

### Functions

#### PowerWindow_StateMachine_Init()

```c
void PowerWindow_StateMachine_Init(void);
```

**Description**: Initializes the state machine to UNINITIALIZED state.

---

#### PowerWindow_StateMachine_Update()

```c
void PowerWindow_StateMachine_Update(const PowerWindow_RuntimeDataType* runtimeData);
```

**Description**: Updates state machine based on runtime data.

**Parameters**:
- `runtimeData`: Pointer to runtime data structure

---

#### PowerWindow_StateMachine_GetState()

```c
PowerWindow_SystemStateType PowerWindow_StateMachine_GetState(void);
```

**Description**: Gets the current state.

**Return Value**: Current system state

---

#### PowerWindow_StateMachine_RequestTransition()

```c
void PowerWindow_StateMachine_RequestTransition(PowerWindow_SystemStateType targetState);
```

**Description**: Requests a state transition if valid.

**Parameters**:
- `targetState`: Target state to transition to

---

#### PowerWindow_StateMachine_CanAcceptCommand()

```c
PowerWindow_BoolType PowerWindow_StateMachine_CanAcceptCommand(void);
```

**Description**: Checks if system can accept new commands.

**Return Value**: POWERWINDOW_TRUE only when in IDLE state

---

#### PowerWindow_StateMachine_IsMoving()

```c
PowerWindow_BoolType PowerWindow_StateMachine_IsMoving(void);
```

**Description**: Checks if system is currently moving.

**Return Value**: POWERWINDOW_TRUE if moving, POWERWINDOW_FALSE otherwise

---

## PowerWindow_Safety

### Overview

Implements safety monitoring including anti-pinch detection, current monitoring, and voltage monitoring.

### Functions

#### PowerWindow_Safety_Init()

```c
void PowerWindow_Safety_Init(void);
```

**Description**: Initializes the safety module.

---

#### PowerWindow_Safety_MonitorAntiPinch()

```c
PowerWindow_BoolType PowerWindow_Safety_MonitorAntiPinch(
    PowerWindow_CurrentType current,
    PowerWindow_SystemStateType state,
    PowerWindow_CurrentType antiPinchThreshold
);
```

**Description**: Monitors for anti-pinch condition.

**Parameters**:
- `current`: Motor current (unit: 0.1A)
- `state`: Current system state
- `antiPinchThreshold`: Anti-pinch threshold

**Return Value**: POWERWINDOW_TRUE if anti-pinch detected

**Note**: Only detects during upward movement (MANUAL_UP or AUTO_UP)

---

#### PowerWindow_Safety_MonitorCurrent()

```c
void PowerWindow_Safety_MonitorCurrent(
    PowerWindow_CurrentType current,
    PowerWindow_SystemStateType state,
    PowerWindow_CurrentType overcurrentThreshold
);
```

**Description**: Monitors motor current for overcurrent and undercurrent conditions.

**Parameters**:
- `current`: Motor current (unit: 0.1A)
- `state`: Current system state
- `overcurrentThreshold`: Overcurrent threshold

---

#### PowerWindow_Safety_MonitorVoltage()

```c
void PowerWindow_Safety_MonitorVoltage(
    PowerWindow_VoltageType voltage,
    PowerWindow_VoltageType undervoltageThreshold
);
```

**Description**: Monitors system voltage for undervoltage condition.

**Parameters**:
- `voltage`: System voltage (unit: 0.1V)
- `undervoltageThreshold`: Undervoltage threshold

---

#### PowerWindow_Safety_CheckFaults()

```c
PowerWindow_BoolType PowerWindow_Safety_CheckFaults(void);
```

**Description**: Checks for active faults.

**Return Value**: POWERWINDOW_TRUE if any fault is active

---

## PowerWindow_NvM

### Overview

Manages non-volatile memory operations including CRC32 checksum calculation and data persistence.

### Functions

#### PowerWindow_NvM_Init()

```c
void PowerWindow_NvM_Init(void);
```

**Description**: Initializes the NVM module.

---

#### PowerWindow_NvM_LoadData()

```c
PowerWindow_BoolType PowerWindow_NvM_LoadData(PowerWindow_NvMDataType* data);
```

**Description**: Loads data from non-volatile memory.

**Parameters**:
- `data`: Pointer to NVM data structure

**Return Value**: POWERWINDOW_TRUE if load successful and CRC valid

---

#### PowerWindow_NvM_SaveData()

```c
PowerWindow_BoolType PowerWindow_NvM_SaveData(const PowerWindow_NvMDataType* data);
```

**Description**: Saves data to non-volatile memory.

**Parameters**:
- `data`: Pointer to NVM data structure

**Return Value**: POWERWINDOW_TRUE if save successful

---

#### PowerWindow_NvM_CalculateCRC32()

```c
uint32_t PowerWindow_NvM_CalculateCRC32(const uint8_t* data, uint32_t length);
```

**Description**: Calculates CRC32 checksum for data integrity verification.

**Parameters**:
- `data`: Pointer to data buffer
- `length`: Length of data in bytes

**Return Value**: CRC32 checksum value

---

## PowerWindow_Diag

### Overview

Provides diagnostic interfaces including fault code management and diagnostic data reading.

### Functions

#### PowerWindow_Diag_Init()

```c
void PowerWindow_Diag_Init(void);
```

**Description**: Initializes the diagnostic module.

---

#### PowerWindow_Diag_SetDTC()

```c
void PowerWindow_Diag_SetDTC(PowerWindow_DTCCodeType dtc);
```

**Description**: Sets a diagnostic trouble code.

**Parameters**:
- `dtc`: Diagnostic trouble code to set

**Behavior**: Adds DTC to both active and history lists

---

#### PowerWindow_Diag_ClearDTC()

```c
void PowerWindow_Diag_ClearDTC(PowerWindow_DTCCodeType dtc);
```

**Description**: Clears a diagnostic trouble code.

**Parameters**:
- `dtc`: Diagnostic trouble code to clear

**Behavior**: Removes from active list but keeps in history

---

#### PowerWindow_Diag_GetActiveDTCs()

```c
uint8_t PowerWindow_Diag_GetActiveDTCs(PowerWindow_DTCType* dtcList, uint8_t maxCount);
```

**Description**: Gets all active diagnostic trouble codes.

**Parameters**:
- `dtcList`: Pointer to array to store DTCs
- `maxCount`: Maximum number of DTCs to retrieve

**Return Value**: Number of active DTCs retrieved

---

#### PowerWindow_Diag_GetHistoryDTCs()

```c
uint8_t PowerWindow_Diag_GetHistoryDTCs(PowerWindow_DTCType* dtcList, uint8_t maxCount);
```

**Description**: Gets all history diagnostic trouble codes.

**Parameters**:
- `dtcList`: Pointer to array to store DTCs
- `maxCount`: Maximum number of DTCs to retrieve

**Return Value**: Number of history DTCs retrieved

---

#### PowerWindow_Diag_ReadPosition()

```c
PowerWindow_PercentType PowerWindow_Diag_ReadPosition(void);
```

**Description**: Reads current window position percentage.

**Return Value**: Position percentage (0-100%)

---

#### PowerWindow_Diag_ReadState()

```c
PowerWindow_SystemStateType PowerWindow_Diag_ReadState(void);
```

**Description**: Reads current system state.

**Return Value**: Current system state

---

#### PowerWindow_Diag_ReadLearnState()

```c
PowerWindow_LearnStateType PowerWindow_Diag_ReadLearnState(void);
```

**Description**: Reads current learning state.

**Return Value**: Current learning state

---

## PowerWindow_Interface

### Overview

Provides hardware abstraction for input/output operations with mock support for testing.

### Functions

#### PowerWindow_Interface_Init()

```c
void PowerWindow_Interface_Init(void);
```

**Description**: Initializes the interface module.

---

#### PowerWindow_Interface_ReadButton()

```c
PowerWindow_ButtonStateType PowerWindow_Interface_ReadButton(void);
```

**Description**: Reads button state.

**Return Value**: Button state (RELEASED, UP, or DOWN)

---

#### PowerWindow_Interface_ReadPosition()

```c
PowerWindow_PositionType PowerWindow_Interface_ReadPosition(void);
```

**Description**: Reads position sensor value.

**Return Value**: Position value (0-4095)

---

#### PowerWindow_Interface_ReadCurrent()

```c
PowerWindow_CurrentType PowerWindow_Interface_ReadCurrent(void);
```

**Description**: Reads motor current.

**Return Value**: Current value (unit: 0.1A)

---

#### PowerWindow_Interface_ReadVoltage()

```c
PowerWindow_VoltageType PowerWindow_Interface_ReadVoltage(void);
```

**Description**: Reads system voltage.

**Return Value**: Voltage value (unit: 0.1V)

---

#### PowerWindow_Interface_SetMotor()

```c
void PowerWindow_Interface_SetMotor(PowerWindow_MotorDirectionType direction);
```

**Description**: Sets motor direction.

**Parameters**:
- `direction`: Motor direction (STOP, UP, or DOWN)

---

### Mock Interface Functions (Test Mode Only)

#### PowerWindow_Interface_Mock_SetButton()

```c
void PowerWindow_Interface_Mock_SetButton(PowerWindow_ButtonStateType state);
```

**Description**: Sets mock button state (test mode only).

---

#### PowerWindow_Interface_Mock_SetPosition()

```c
void PowerWindow_Interface_Mock_SetPosition(PowerWindow_PositionType position);
```

**Description**: Sets mock position value (test mode only).

---

#### PowerWindow_Interface_Mock_SetCurrent()

```c
void PowerWindow_Interface_Mock_SetCurrent(PowerWindow_CurrentType current);
```

**Description**: Sets mock current value (test mode only).

---

#### PowerWindow_Interface_Mock_SetVoltage()

```c
void PowerWindow_Interface_Mock_SetVoltage(PowerWindow_VoltageType voltage);
```

**Description**: Sets mock voltage value (test mode only).

---

#### PowerWindow_Interface_Mock_GetMotor()

```c
PowerWindow_MotorDirectionType PowerWindow_Interface_Mock_GetMotor(void);
```

**Description**: Gets mock motor direction (test mode only).

**Return Value**: Current motor direction

---

## PowerWindow_Cfg

### Overview

Provides configuration parameter validation and initialization.

### Functions

#### PowerWindow_Cfg_InitDefaultParams()

```c
PowerWindow_BoolType PowerWindow_Cfg_InitDefaultParams(
    PowerWindow_CalibrationParamsType* params
);
```

**Description**: Initializes calibration parameters with default values.

**Parameters**:
- `params`: Pointer to calibration parameters structure

**Return Value**: POWERWINDOW_TRUE if successful

---

#### PowerWindow_Cfg_ValidateParams()

```c
PowerWindow_BoolType PowerWindow_Cfg_ValidateParams(
    const PowerWindow_CalibrationParamsType* params
);
```

**Description**: Validates all calibration parameters.

**Parameters**:
- `params`: Pointer to calibration parameters structure

**Return Value**: POWERWINDOW_TRUE if all parameters are valid

---

## Type Definitions

### Basic Types

```c
typedef uint8_t PowerWindow_StateType;
typedef uint16_t PowerWindow_PositionType;      /* 0-4095 */
typedef uint8_t PowerWindow_PercentType;        /* 0-100 */
typedef uint16_t PowerWindow_CurrentType;       /* unit: 0.1A */
typedef uint16_t PowerWindow_VoltageType;       /* unit: 0.1V */
typedef uint16_t PowerWindow_DTCType;
typedef uint8_t PowerWindow_BoolType;
```

### Enumerations

```c
typedef enum {
    POWERWINDOW_BUTTON_RELEASED = 0,
    POWERWINDOW_BUTTON_UP       = 1,
    POWERWINDOW_BUTTON_DOWN     = 2
} PowerWindow_ButtonStateType;

typedef enum {
    POWERWINDOW_MOTOR_STOP = 0,
    POWERWINDOW_MOTOR_UP   = 1,
    POWERWINDOW_MOTOR_DOWN = 2
} PowerWindow_MotorDirectionType;

typedef enum {
    POWERWINDOW_STATE_UNINITIALIZED = 0,
    POWERWINDOW_STATE_IDLE          = 1,
    POWERWINDOW_STATE_MANUAL_UP     = 2,
    POWERWINDOW_STATE_MANUAL_DOWN   = 3,
    POWERWINDOW_STATE_AUTO_UP       = 4,
    POWERWINDOW_STATE_AUTO_DOWN     = 5,
    POWERWINDOW_STATE_ANTI_PINCH    = 6,
    POWERWINDOW_STATE_FAULT         = 7
} PowerWindow_SystemStateType;

typedef enum {
    POWERWINDOW_DTC_NO_FAULT           = 0x0000,
    POWERWINDOW_DTC_POSITION_SENSOR    = 0x0001,
    POWERWINDOW_DTC_OVERCURRENT        = 0x0002,
    POWERWINDOW_DTC_UNDERCURRENT       = 0x0003,
    POWERWINDOW_DTC_UNDERVOLTAGE       = 0x0004,
    POWERWINDOW_DTC_NVM_READ_FAIL      = 0x0005,
    POWERWINDOW_DTC_NVM_WRITE_FAIL     = 0x0006,
    POWERWINDOW_DTC_CALIBRATION_ERROR  = 0x0007
} PowerWindow_DTCCodeType;

typedef enum {
    POWERWINDOW_LEARN_NOT_STARTED = 0,
    POWERWINDOW_LEARN_IN_PROGRESS = 1,
    POWERWINDOW_LEARN_COMPLETED   = 2,
    POWERWINDOW_LEARN_FAILED      = 3
} PowerWindow_LearnStateType;
```

### Structures

```c
typedef struct {
    PowerWindow_CurrentType antiPinchCurrentThreshold;
    PowerWindow_PositionType antiPinchRetractDistance;
    PowerWindow_PositionType positionUpperLimit;
    PowerWindow_PositionType positionLowerLimit;
    PowerWindow_VoltageType undervoltageThreshold;
    PowerWindow_CurrentType overcurrentThreshold;
    uint16_t autoModeTriggerTime;
} PowerWindow_CalibrationParamsType;

typedef struct {
    PowerWindow_SystemStateType currentState;
    PowerWindow_PositionType currentPosition;
    PowerWindow_PercentType currentPositionPercent;
    PowerWindow_CurrentType motorCurrent;
    PowerWindow_VoltageType systemVoltage;
    PowerWindow_ButtonStateType buttonState;
    PowerWindow_MotorDirectionType motorDirection;
    PowerWindow_LearnStateType learnState;
    PowerWindow_BoolType antiPinchActive;
    uint32_t buttonPressTime;
    uint32_t faultDebounceCounter;
} PowerWindow_RuntimeDataType;

typedef struct {
    PowerWindow_PositionType savedPosition;
    PowerWindow_PositionType learnedUpperLimit;
    PowerWindow_PositionType learnedLowerLimit;
    PowerWindow_LearnStateType learnState;
    PowerWindow_DTCType activeDTCs[8];
    PowerWindow_DTCType historyDTCs[16];
    uint32_t crc;
} PowerWindow_NvMDataType;
```

---

## Constants

### Default Values

```c
#define POWERWINDOW_DEFAULT_ANTI_PINCH_CURRENT    100  /* 10.0A */
#define POWERWINDOW_DEFAULT_RETRACT_DISTANCE      100  /* 100mm */
#define POWERWINDOW_DEFAULT_POSITION_UPPER        4095
#define POWERWINDOW_DEFAULT_POSITION_LOWER        0
#define POWERWINDOW_DEFAULT_UNDERVOLTAGE          100  /* 10.0V */
#define POWERWINDOW_DEFAULT_OVERCURRENT           200  /* 20.0A */
#define POWERWINDOW_DEFAULT_AUTO_TRIGGER_TIME     500  /* 500ms */
```

### Boolean Values

```c
#define POWERWINDOW_TRUE  ((PowerWindow_BoolType)1)
#define POWERWINDOW_FALSE ((PowerWindow_BoolType)0)
```

---

## Error Handling

All functions that can fail return a boolean or error code:

- **POWERWINDOW_TRUE**: Operation successful
- **POWERWINDOW_FALSE**: Operation failed

Functions that return void do not indicate failure. Check system state using diagnostic functions.

---

## Thread Safety

The system is designed for single-threaded operation. All functions should be called from the same context or protected with appropriate synchronization mechanisms.

---

## Performance

- **Main Function Cycle**: 10ms
- **Input Sampling**: 10ms
- **Safety Monitoring**: 10ms
- **Fault Debounce**: 100-200ms (configurable)

---

## Version

API Version: 1.0.0

</content>
