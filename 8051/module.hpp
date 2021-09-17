#ifndef __MODULE_HPP__
#define __MODULE_HPP__
#include <cstdint>
namespace disas8051{
   struct Module{
       const char *name;
       uint16_t start_addr;
       uint16_t end_addr;
   };
   enum Module_E{
        Flash_E,        // 0
        XRAM_E,         // 1
        GPIO_E,         // 2
        KBC_E,          // 3
        ESB_E,          // 4
        IKB_E,          // 5
        RSV0_E,         // 6
        RSV1_E,         // 7
        PECI_E,         // 8
        RSV2_E,         // 9
        OWM_E,          // 10
        RSV3_E,         // 11
        PWM_E,          // 12
        FAN_E,          // 13
        GPT_E,          // 14
        SDI_E,          // 15
        WDT_E,          // 16
        LPC_E,          // 17
        XBI_E,          // 18
        CIR_E,          // 19
        RSV4_E,         // 20
        PS2_E,          // 21
        EC_E,           // 22
        GPWU_E,         // 23
        SMBus_E,        // 24
        RSV5_E,         // 25
        RSV6_E,         // 26
        Module_NUM      // 27
};
   extern Module KB930_modules[];
   #define KB930_MODULES_NUM    (27) 
   extern int get_KB930_ModuleIndex(uint16_t address);
   const char* get_KB930_ModuleName(uint16_t address);
}
#endif

