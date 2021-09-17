#include "module.hpp"
namespace disas8051 {
    Module KB930_modules[] = {
        {"Flash",   0x0000,     OxEBFF},    // 0
        {"XRAM",    0xEC00,     OxFBFF},    // 1
        {"GPIO",    0xFC00,     OxFC7F},    // 2
        {"KBC",     0xFC80,     OxFC8F},    // 3
        {"ESB",     0xFC90,     OxFC9F},    // 4
        {"IKB",     0xFCA0,     OxFCAF},    // 5
        {"RSV0",    0xFCB0,     OxFCBF},    // 6
        {"RSV1",    0xFCC0,     OxFCCF},    // 7
        {"PECI",    0xFCD0,     OxFCDF},    // 8
        {"RSV2",    0xFCE0,     OxFCEF},    // 9
        {"OWM",     0xFCF0,     OxFCFF},    // 10
        {"RSV3",    0xFD00,     OxFDFF},    // 11
        {"PWM",     0xFE00,     OxFE1F},    // 12
        {"FAN",     0xFE20,     OxFE4F},    // 13
        {"GPT",     0xFE50,     OxFE6F},    // 14
        {"SDI",     0xFE70,     OxFE7F},    // 15
        {"WDT",     0xFE80,     OxFE8F},    // 16
        {"LPC",     0xFE90,     OxFE9F},    // 17
        {"XBI",     0xFEA0,     OxFEBF},    // 18
        {"CIR",     0xFEC0,     OxFECF},    // 19
        {"RSV4",    0xFED0,     OxFEDF},    // 20
        {"PS2",     0xFEE0,     OxFEFF},    // 21
        {"EC",      0xFF00,     OxFF2F},    // 22
        {"GPWU",    0xFF30,     OxFF7F},    // 23
        {"SMBus",   0xFF80,     OxFFBF},    // 24
        {"RSV5",    0xFFC0,     OxFFCF},    // 25
        {"RSV6",    0xFFD0,     OxFFFF}     // 26

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
