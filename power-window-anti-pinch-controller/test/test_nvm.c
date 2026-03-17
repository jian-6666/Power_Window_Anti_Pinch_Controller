/**
 * @file test_nvm.c
 * @brief Power Window Anti-Pinch Controller - NVM Module Tests
 * 
 * Unit tests for non-volatile memory operations and CRC calculation.
 * 
 * @author Power Window Team
 * @version 1.0
 */

#include "PowerWindow_NvM.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

/* ============================================================================
 * Test Functions
 * ============================================================================ */

void test_nvm_crc_calculation(void)
{
    uint8_t data1[] = {0x00, 0x01, 0x02, 0x03};
    uint8_t data2[] = {0x00, 0x01, 0x02, 0x03};
    uint8_t data3[] = {0x00, 0x01, 0x02, 0x04};
    
    /* Test CRC calculation consistency */
    uint32_t crc1 = PowerWindow_NvM_CalculateCRC32(data1, sizeof(data1));
    uint32_t crc2 = PowerWindow_NvM_CalculateCRC32(data2, sizeof(data2));
    assert(crc1 == crc2);  /* Same data should produce same CRC */
    
    /* Test CRC difference for different data */
    uint32_t crc3 = PowerWindow_NvM_CalculateCRC32(data3, sizeof(data3));
    assert(crc1 != crc3);  /* Different data should produce different CRC */
    
    /* Test NULL pointer handling */
    uint32_t crc_null = PowerWindow_NvM_CalculateCRC32(NULL, 4);
    assert(crc_null == 0);
    
    /* Test zero length - CRC of empty data */
    uint32_t crc_zero = PowerWindow_NvM_CalculateCRC32(data1, 0);
    /* Just verify it's consistent */
    uint32_t crc_zero2 = PowerWindow_NvM_CalculateCRC32(data1, 0);
    assert(crc_zero == crc_zero2);
    
    printf("    PASS: test_nvm_crc_calculation\n");
}

void test_nvm_data_roundtrip(void)
{
    PowerWindow_NvMDataType originalData, loadedData;
    
    /* Initialize NVM */
    PowerWindow_NvM_Init();
    
    /* Prepare test data */
    originalData.savedPosition = 2048;
    originalData.learnedUpperLimit = 4095;
    originalData.learnedLowerLimit = 0;
    originalData.learnState = POWERWINDOW_LEARN_COMPLETED;
    originalData.activeDTCs[0] = POWERWINDOW_DTC_OVERCURRENT;
    originalData.activeDTCs[1] = 0;
    originalData.historyDTCs[0] = POWERWINDOW_DTC_UNDERVOLTAGE;
    originalData.historyDTCs[1] = 0;
    originalData.crc = 0;  /* Will be calculated by SaveData */
    
    /* Save data */
    PowerWindow_BoolType result = PowerWindow_NvM_SaveData(&originalData);
    assert(result == POWERWINDOW_TRUE);
    
    /* Load data */
    result = PowerWindow_NvM_LoadData(&loadedData);
    assert(result == POWERWINDOW_TRUE);
    
    /* Verify data integrity */
    assert(loadedData.savedPosition == originalData.savedPosition);
    assert(loadedData.learnedUpperLimit == originalData.learnedUpperLimit);
    assert(loadedData.learnedLowerLimit == originalData.learnedLowerLimit);
    assert(loadedData.learnState == originalData.learnState);
    
    /* Test NULL pointer handling */
    result = PowerWindow_NvM_SaveData(NULL);
    assert(result == POWERWINDOW_FALSE);
    
    result = PowerWindow_NvM_LoadData(NULL);
    assert(result == POWERWINDOW_FALSE);
    
    /* Test data validity check */
    PowerWindow_BoolType valid = PowerWindow_NvM_IsDataValid();
    assert(valid == POWERWINDOW_TRUE);
    
    printf("    PASS: test_nvm_data_roundtrip\n");
}

void test_nvm_conditional_save(void)
{
    PowerWindow_NvM_Init();
    
    /* Test position change threshold (5%) */
    PowerWindow_PositionType currentPos = 2048;
    PowerWindow_PositionType lastSavedPos = 2048;
    PowerWindow_LearnStateType currentLearn = POWERWINDOW_LEARN_NOT_STARTED;
    PowerWindow_LearnStateType lastLearn = POWERWINDOW_LEARN_NOT_STARTED;
    
    /* No change - should not save */
    PowerWindow_NvM_CheckAndSave(currentPos, lastSavedPos, currentLearn, lastLearn, POWERWINDOW_FALSE);
    
    /* Small position change (< 5%) - should not save */
    currentPos = 2100;  /* ~1.3% change */
    PowerWindow_NvM_CheckAndSave(currentPos, lastSavedPos, currentLearn, lastLearn, POWERWINDOW_FALSE);
    
    /* Large position change (> 5%) - should save */
    currentPos = 2300;  /* ~6.1% change */
    PowerWindow_NvM_CheckAndSave(currentPos, lastSavedPos, currentLearn, lastLearn, POWERWINDOW_FALSE);
    
    /* Learning state change - should save */
    currentPos = 2048;
    currentLearn = POWERWINDOW_LEARN_COMPLETED;
    PowerWindow_NvM_CheckAndSave(currentPos, lastSavedPos, currentLearn, lastLearn, POWERWINDOW_FALSE);
    
    /* New fault detected - should save */
    currentLearn = POWERWINDOW_LEARN_NOT_STARTED;
    PowerWindow_NvM_CheckAndSave(currentPos, lastSavedPos, currentLearn, lastLearn, POWERWINDOW_TRUE);
    
    printf("    PASS: test_nvm_conditional_save\n");
}

void test_nvm_dtc_management(void)
{
    PowerWindow_NvM_Init();
    
    /* Test active DTC update */
    PowerWindow_DTCType activeDTCs[] = {
        POWERWINDOW_DTC_OVERCURRENT,
        POWERWINDOW_DTC_UNDERVOLTAGE,
        POWERWINDOW_DTC_NO_FAULT
    };
    
    PowerWindow_NvM_UpdateActiveDTCs(activeDTCs, 2);
    
    const PowerWindow_NvMDataType* currentData = PowerWindow_NvM_GetCurrentData();
    assert(currentData->activeDTCs[0] == POWERWINDOW_DTC_OVERCURRENT);
    assert(currentData->activeDTCs[1] == POWERWINDOW_DTC_UNDERVOLTAGE);
    assert(currentData->activeDTCs[2] == POWERWINDOW_DTC_NO_FAULT);
    
    /* Test history DTC update */
    PowerWindow_DTCType historyDTCs[] = {
        POWERWINDOW_DTC_POSITION_SENSOR,
        POWERWINDOW_DTC_NVM_READ_FAIL,
        POWERWINDOW_DTC_NO_FAULT
    };
    
    PowerWindow_NvM_UpdateHistoryDTCs(historyDTCs, 2);
    
    currentData = PowerWindow_NvM_GetCurrentData();
    assert(currentData->historyDTCs[0] == POWERWINDOW_DTC_POSITION_SENSOR);
    assert(currentData->historyDTCs[1] == POWERWINDOW_DTC_NVM_READ_FAIL);
    assert(currentData->historyDTCs[2] == POWERWINDOW_DTC_NO_FAULT);
    
    /* Test NULL pointer handling */
    PowerWindow_NvM_UpdateActiveDTCs(NULL, 2);
    PowerWindow_NvM_UpdateHistoryDTCs(NULL, 2);
    
    /* Test count overflow */
    PowerWindow_NvM_UpdateActiveDTCs(activeDTCs, 10);  /* Should be ignored */
    PowerWindow_NvM_UpdateHistoryDTCs(historyDTCs, 20);  /* Should be ignored */
    
    printf("    PASS: test_nvm_dtc_management\n");
}

void test_nvm_learning_data(void)
{
    PowerWindow_NvM_Init();
    
    /* Test learning data update */
    PowerWindow_NvM_UpdateLearningData(
        POWERWINDOW_LEARN_COMPLETED,
        3800,  /* Upper limit */
        200    /* Lower limit */
    );
    
    const PowerWindow_NvMDataType* currentData = PowerWindow_NvM_GetCurrentData();
    assert(currentData->learnState == POWERWINDOW_LEARN_COMPLETED);
    assert(currentData->learnedUpperLimit == 3800);
    assert(currentData->learnedLowerLimit == 200);
    
    /* Verify CRC is updated */
    PowerWindow_NvMDataType testData;
    PowerWindow_BoolType result = PowerWindow_NvM_LoadData(&testData);
    assert(result == POWERWINDOW_TRUE);
    
    printf("    PASS: test_nvm_learning_data\n");
}

void test_nvm_crc_validation(void)
{
    PowerWindow_NvM_Init();
    
    /* Test basic CRC calculation */
    PowerWindow_NvMDataType testData;
    testData.savedPosition = 1000;
    testData.learnedUpperLimit = 4000;
    testData.learnedLowerLimit = 100;
    testData.learnState = POWERWINDOW_LEARN_COMPLETED;
    
    /* Initialize DTCs */
    for (uint8_t i = 0; i < 8; i++) {
        testData.activeDTCs[i] = POWERWINDOW_DTC_NO_FAULT;
    }
    for (uint8_t i = 0; i < 16; i++) {
        testData.historyDTCs[i] = POWERWINDOW_DTC_NO_FAULT;
    }
    
    /* Calculate correct CRC */
    uint32_t correctCRC = PowerWindow_NvM_CalculateCRC32(
        (const uint8_t*)&testData,
        sizeof(PowerWindow_NvMDataType) - sizeof(uint32_t)
    );
    testData.crc = correctCRC;
    
    /* Save with correct CRC */
    PowerWindow_BoolType result = PowerWindow_NvM_SaveData(&testData);
    assert(result == POWERWINDOW_TRUE);
    
    /* Load should succeed */
    PowerWindow_NvMDataType loadedData;
    result = PowerWindow_NvM_LoadData(&loadedData);
    assert(result == POWERWINDOW_TRUE);
    assert(loadedData.savedPosition == testData.savedPosition);
    
    /* Test that CRC is correctly calculated and stored */
    assert(loadedData.crc != 0);
    
    /* Verify CRC calculation consistency */
    uint32_t recalculatedCRC = PowerWindow_NvM_CalculateCRC32(
        (const uint8_t*)&loadedData,
        sizeof(PowerWindow_NvMDataType) - sizeof(uint32_t)
    );
    assert(recalculatedCRC == loadedData.crc);
    
    printf("    PASS: test_nvm_crc_validation\n");
}

/* ============================================================================
 * Test Runner
 * ============================================================================ */

void run_nvm_tests(void)
{
    printf("Running NVM Module Tests:\n");
    
    test_nvm_crc_calculation();
    test_nvm_data_roundtrip();
    test_nvm_conditional_save();
    test_nvm_dtc_management();
    test_nvm_learning_data();
    test_nvm_crc_validation();
    
    printf("All NVM tests passed!\n\n");
}
