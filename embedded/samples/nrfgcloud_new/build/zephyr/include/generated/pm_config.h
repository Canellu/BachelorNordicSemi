/* File generated by C:/Users/Einar/ncs/v1.4.99-dev1/nrf/scripts/partition_manager_output.py, do not modify */
#ifndef PM_CONFIG_H__
#define PM_CONFIG_H__
#define PM_SPM_ADDRESS 0x0
#define PM_SPM_END_ADDRESS 0xc000
#define PM_SPM_SIZE 0xc000
#define PM_SPM_NAME spm
#define PM_SPM_ID 0
#define PM_spm_ID PM_SPM_ID
#define PM_spm_IS_ENABLED 1
#define PM_0_LABEL SPM
#define PM_SPM_DEV_NAME "NRF_FLASH_DRV_NAME"
#define PM_APP_ADDRESS 0xc000
#define PM_APP_END_ADDRESS 0x100000
#define PM_APP_SIZE 0xf4000
#define PM_APP_NAME app
#define PM_APP_ID 1
#define PM_app_ID PM_APP_ID
#define PM_app_IS_ENABLED 1
#define PM_1_LABEL APP
#define PM_APP_DEV_NAME "NRF_FLASH_DRV_NAME"
#define PM_OTP_ADDRESS 0xff8108
#define PM_OTP_END_ADDRESS 0xff83fc
#define PM_OTP_SIZE 0x2f4
#define PM_OTP_NAME otp
#define PM_SPM_SRAM_ADDRESS 0x20000000
#define PM_SPM_SRAM_END_ADDRESS 0x20010000
#define PM_SPM_SRAM_SIZE 0x10000
#define PM_SPM_SRAM_NAME spm_sram
#define PM_SRAM_SECURE_ADDRESS 0x20000000
#define PM_SRAM_SECURE_END_ADDRESS 0x20010000
#define PM_SRAM_SECURE_SIZE 0x10000
#define PM_SRAM_SECURE_NAME sram_secure
#define PM_BSDLIB_SRAM_ADDRESS 0x20010000
#define PM_BSDLIB_SRAM_END_ADDRESS 0x20020000
#define PM_BSDLIB_SRAM_SIZE 0x10000
#define PM_BSDLIB_SRAM_NAME bsdlib_sram
#define PM_SRAM_PRIMARY_ADDRESS 0x20020000
#define PM_SRAM_PRIMARY_END_ADDRESS 0x20040000
#define PM_SRAM_PRIMARY_SIZE 0x20000
#define PM_SRAM_PRIMARY_NAME sram_primary
#define PM_NUM 2
#define PM_ALL_BY_SIZE "otp spm bsdlib_sram spm_sram sram_secure sram_primary app"
#define PM_ADDRESS 0xc000
#define PM_SIZE 0xf4000
#define PM_SRAM_ADDRESS 0x20020000
#define PM_SRAM_SIZE 0x20000
#endif /* PM_CONFIG_H__ */