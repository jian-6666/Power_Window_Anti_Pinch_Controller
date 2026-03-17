/**
 * @file test_diag.c
 * @brief Power Window Anti-Pinch Controller - Diagnostic Module Tests
 * 
 * Unit tests for diagnostic trouble code management.
 * 
 * @author Power Window Team
 * @version 1.0
 */

#include "PowerWindow_Diag.h"
#include <stdio.h>
#include <assert.h>

/* ============================================================================
 * Test Functions
 * ============================================================================ */

void test_diag_dtc_management(void)
{
    PowerWindow_DTCType dtcList[8];
    uint8_t count;
    
    /* Initialize diagnostic module */
    PowerWindow_Diag_Init();
    
    /* Test setting DTC */
    PowerWindow_Diag_SetDTC(POWERWINDOW_DTC_OVERCURRENT);
    assert(PowerWindow_Diag_IsDTCActive(POWERWINDOW_DTC_OVERCURRENT) == POWERWINDOW_TRUE);
    
    /* Test getting active DTCs */
    count = PowerWindow_Diag_GetActiveDTCs(dtcList, 8);
    assert(count == 1);
    assert(dtcList[0] == POWERWINDOW_DTC_OVERCURRENT);
    
    /* Test setting multiple DTCs */
    PowerWindow_Diag_SetDTC(POWERWINDOW_DTC_UNDERVOLTAGE);
    PowerWindow_Diag_SetDTC(POWERWINDOW_DTC_UNDERCURRENT);
    
    count = PowerWindow_Diag_GetActiveDTCs(dtcList, 8);
    assert(count == 3);
    
    /* Test clearing specific DTC */
    PowerWindow_Diag_ClearDTC(POWERWINDOW_DTC_OVERCURRENT);
    assert(PowerWindow_Diag_IsDTCActive(POWERWINDOW_DTC_OVERCURRENT) == POWERWINDOW_FALSE);
    assert(PowerWindow_Diag_IsDTCActive(POWERWINDOW_DTC_UNDERVOLTAGE) == POWERWINDOW_TRUE);
    
    count = PowerWindow_Diag_GetActiveDTCs(dtcList, 8);
    assert(count == 2);
    
    /* Test history DTCs (cleared DTCs should remain in history) */
    count = PowerWindow_Diag_GetHistoryDTCs(dtcList, 8);
    assert(count == 3);  /* All three DTCs should be in history */
    
    /* Test clearing all DTCs */
    PowerWindow_Diag_ClearAllDTCs();
    count = PowerWindow_Diag_GetActiveDTCs(dtcList, 8);
    assert(count == 0);
    
    /* History should still contain the DTCs */
    count = PowerWindow_Diag_GetHistoryDTCs(dtcList, 8);
    assert(count == 3);
    
    /* Test duplicate DTC handling */
    PowerWindow_Diag_Init();
    PowerWindow_Diag_SetDTC(POWERWINDOW_DTC_POSITION_SENSOR);
    PowerWindow_Diag_SetDTC(POWERWINDOW_DTC_POSITION_SENSOR);  /* Duplicate */
    
    count = PowerWindow_Diag_GetActiveDTCs(dtcList, 8);
    assert(count == 1);  /* Should only have one entry */
    
    /* Test NULL pointer handling */
    count = PowerWindow_Diag_GetActiveDTCs(NULL, 8);
    assert(count == 0);
    
    printf("    PASS: test_diag_dtc_management\n");
}
