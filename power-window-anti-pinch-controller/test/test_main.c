/**
 * @file test_main.c
 * @brief Power Window Anti-Pinch Controller - Test Main Entry Point
 * 
 * This file provides the main entry point for running all unit tests.
 * 
 * @author Power Window Team
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>

/* Forward declarations of test functions */
extern void test_cfg_init_default_params(void);
extern void test_cfg_validate_params(void);
extern void test_interface_mock_operations(void);
extern void test_diag_dtc_management(void);
extern void test_nvm_crc_calculation(void);
extern void test_nvm_data_roundtrip(void);
extern void test_safety_anti_pinch_detection(void);
extern void test_statemachine_transitions(void);
extern void test_powerwindow_initialization(void);
extern void test_powerwindow_position_conversion(void);

/**
 * @brief Run all tests
 * 
 * @return 0 if all tests pass, 1 if any test fails
 */
int main(void)
{
    int test_count = 0;
    int pass_count = 0;
    
    printf("========================================\n");
    printf("Power Window Anti-Pinch Controller Tests\n");
    printf("========================================\n\n");
    
    /* Configuration tests */
    printf("Running configuration tests...\n");
    test_count++;
    printf("  [%d] test_cfg_init_default_params\n", test_count);
    test_cfg_init_default_params();
    pass_count++;
    
    test_count++;
    printf("  [%d] test_cfg_validate_params\n", test_count);
    test_cfg_validate_params();
    pass_count++;
    
    /* Interface tests */
    printf("\nRunning interface tests...\n");
    test_count++;
    printf("  [%d] test_interface_mock_operations\n", test_count);
    test_interface_mock_operations();
    pass_count++;
    
    /* Diagnostic tests */
    printf("\nRunning diagnostic tests...\n");
    test_count++;
    printf("  [%d] test_diag_dtc_management\n", test_count);
    test_diag_dtc_management();
    pass_count++;
    
    /* NVM tests */
    printf("\nRunning NVM tests...\n");
    test_count++;
    printf("  [%d] test_nvm_crc_calculation\n", test_count);
    test_nvm_crc_calculation();
    pass_count++;
    
    test_count++;
    printf("  [%d] test_nvm_data_roundtrip\n", test_count);
    test_nvm_data_roundtrip();
    pass_count++;
    
    /* Safety tests */
    printf("\nRunning safety tests...\n");
    test_count++;
    printf("  [%d] test_safety_anti_pinch_detection\n", test_count);
    test_safety_anti_pinch_detection();
    pass_count++;
    
    /* State machine tests */
    printf("\nRunning state machine tests...\n");
    test_count++;
    printf("  [%d] test_statemachine_transitions\n", test_count);
    test_statemachine_transitions();
    pass_count++;
    
    /* PowerWindow tests */
    printf("\nRunning PowerWindow tests...\n");
    test_count++;
    printf("  [%d] test_powerwindow_initialization\n", test_count);
    test_powerwindow_initialization();
    pass_count++;
    
    test_count++;
    printf("  [%d] test_powerwindow_position_conversion\n", test_count);
    test_powerwindow_position_conversion();
    pass_count++;
    
    /* Summary */
    printf("\n========================================\n");
    printf("Test Results: %d/%d passed\n", pass_count, test_count);
    printf("========================================\n");
    
    return (pass_count == test_count) ? 0 : 1;
}
