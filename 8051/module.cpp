#include "module.hpp"
namespace disas8051 {
    Module KB930_modules[] = {
        {"Flash",   0x0000,     OxEBFF},    // 0, 59kB
        {"XRAM",    0xEC00,     OxFBFF},    // 1, 4kB
        {"GPIO",    0xFC00,     OxFC7F},    // 2, 128
        {"KBC",     0xFC80,     OxFC8F},    // 3, 16
        {"ESB",     0xFC90,     OxFC9F},    // 4, 16
        {"IKB",     0xFCA0,     OxFCAF},    // 5, 16
        {"RSV0",    0xFCB0,     OxFCBF},    // 6, 16
        {"RSV1",    0xFCC0,     OxFCCF},    // 7, 16
        {"PECI",    0xFCD0,     OxFCDF},    // 8, 16
        {"RSV2",    0xFCE0,     OxFCEF},    // 9, 16
        {"OWM",     0xFCF0,     OxFCFF},    // 10, 16
        {"RSV3",    0xFD00,     OxFDFF},    // 11, 256
        {"PWM",     0xFE00,     OxFE1F},    // 12, 32
        {"FAN",     0xFE20,     OxFE4F},    // 13, 48
        {"GPT",     0xFE50,     OxFE6F},    // 14, 32
        {"SDI",     0xFE70,     OxFE7F},    // 15, 16
        {"WDT",     0xFE80,     OxFE8F},    // 16, 16
        {"LPC",     0xFE90,     OxFE9F},    // 17, 16
        {"XBI",     0xFEA0,     OxFEBF},    // 18, 32
        {"CIR",     0xFEC0,     OxFECF},    // 19, 16
        {"RSV4",    0xFED0,     OxFEDF},    // 20, 16
        {"PS2",     0xFEE0,     OxFEFF},    // 21, 32
        {"EC",      0xFF00,     OxFF2F},    // 22, 48
        {"GPWU",    0xFF30,     OxFF7F},    // 23, 80
        {"SMBus",   0xFF80,     OxFFBF},    // 24, 64
        {"RSV5",    0xFFC0,     OxFFCF},    // 25, 16
        {"RSV6",    0xFFD0,     OxFFFF}     // 26, 48

    };
    const char *GPIO_Reg[]={
        "GPIOFS00","GPIOFS08","GPIOFS10","GPIOFS18","GPIOFS20","GPIOFS28","GPIOFS30","GPIOFS38",    // 0x00 - 0x07
        "GPIOFS40","GPIOFS48","GPIOFS50","GPIOFS58","RSV0C","RSV0D","RSV0E","RSV0F",    // 0x08 - 0x0F
        "GPIOOE00","GPIOOE08","GPIOOE10","GPIOOE18","GPIOOE20","GPIOOE28","GPIOOE30","GPIOOE38",    // 0x10 - 0x17
        "GPIOOE40","GPIOOE48","GPIOOE50","GPIOOE58","GPXAOE00","GPXAOE08","RSV1E","GPXDOE00",    // 0x18 - 0x1F
        "GPIOD00","GPIOD08","GPIOD10","GPIOD18","GPIOD20","GPIOD28","GPIOD30","GPIOD38",    // 0x20 - 0x27
        "GPIOD40","GPIOD48","GPIOD50","GPIOD58","GPXAD00","GPXAD08","RSV2E","GPXDD00",    // 0x28 - 0x2F
        "GPIOIN00","GPIOIN08","GPIOIN10","GPIOIN18","GPIOIN20","GPIOIN28","GPIOIN30","GPIOIN38",    // 0x30 - 0x37
        "GPIOIN40","GPIOIN48","GPIOIN50","GPIOIN58","GPXAIN00","GPXAIN08","RSV3E","GPXDIN00",    // 0x38 - 0x3F
        "GPIOPU00","GPIOPU08","GPIOPU10","GPIOPU18","GPIOPU20","GPIOPU28","GPIOPU30","RSV47",    // 0x40 - 0x47
        "GPIOPU40","GPIOPU48","GPIOPU50","GPIOPU58","GPXAPU00","GPXAPU08","RSV4E","GPXDPU00",    // 0x48 - 0x4F
        "GPIOOD00","GPIOOD08","GPIOOD10","GPIOOD18","GPIOOD20","GPIOOD28","GPIOOD30","RSV57",    // 0x50 - 0x57
        "GPIOOD40","GPIOOD48","GPIOOD50","GPIOOD58","RSV5C","RSV5D","RSV5E","RSV5F",    // 0x58 - 0x5F
        "GPIOIE00","GPIOIE08","GPIOIE10","GPIOIE18","GPIOIE20","GPIOIE28","GPIOIE30","GPIOIE38",    // 0x60 - 0x67
        "GPIOIE40","GPIOIE48","GPIOIE50","GPIOEE58","GPXAIE00","GPXAIE08","RSV6E","GPXDIE00",    // 0x68 - 0x6F
        "GPIO_MISC","GPIO_MISC2","GPIO_TMR","GPX_MISC","PADING74","PADING75","PADING76","PADING77", // 0x70 - 0x77
        "PADING78","PADING79","PADING7A","PADING7B","PADING7C","PADING7D","PADING7E","PADING7F"     // 0x78 - 0x7F
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
