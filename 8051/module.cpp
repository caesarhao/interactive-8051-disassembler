#include "module.hpp"
namespace disas8051 {
    Module KB930_modules[] = {
        {"Flash",   0x0000,     OxEBFF},
        {"XRAM",    0xEC00,     OxFBFF},
        {"GPIO",    0xFC00,     OxFC7F},
        {"KBC",     0xFC80,     OxFC8F},
        {"ESB",     0xFC90,     OxFC9F},
        {"IKB",     0xFCA0,     OxFCAF},
        {"RSV0",    0xFCB0,     OxFCBF},
        {"RSV1",    0xFCC0,     OxFCCF},
        {"PECI",    0xFCD0,     OxFCDF},
        {"RSV2",    0xFCE0,     OxFCEF},
        {"OWM",     0xFCF0,     OxFCFF},
        {"RSV3",    0xFD00,     OxFDFF},
        {"PWM",     0xFE00,     OxFE1F},
        {"FAN",     0xFE20,     OxFE4F},
        {"GPT",     0xFE50,     OxFE6F},
        {"SDI",     0xFE70,     OxFE7F},
        {"WDT",     0xFE80,     OxFE8F},
        {"LPC",     0xFE90,     OxFE9F},
        {"XBI",     0xFEA0,     OxFEBF},
        {"CIR",     0xFEC0,     OxFECF},
        {"RSV4",    0xFED0,     OxFEDF},
        {"PS2",     0xFEE0,     OxFEFF},
        {"EC",      0xFF00,     OxFF2F},
        {"GPWU",    0xFF30,     OxFF7F},
        {"SMBus",   0xFF80,     OxFFBF},
        {"RSV5",    0xFFC0,     OxFFCF},
        {"RSV6",    0xFFD0,     OxFFFF}

    };
    const char *GPIO_Reg[]={

    };
    int get_KB930_ModuleIndex(uint16_t address){
        for(int i = 0; i < KB930_MODULES_NUM; i++){
           if (address >= KB930_modules[i].start_addr && address <= KB930_modules[i].end_addr){
               return i;
           } 
        }
        return -1;
    }
   const char* get_KB930_ModuleName(uint16_t address){
        return KB930_modules[get_KB930_ModuleIndex(address)].name;
    } 
}
