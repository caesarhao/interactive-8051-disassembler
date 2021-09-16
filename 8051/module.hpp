#ifndef __MODULE_HPP__
#define __MODULE_HPP__
#include <cstdint>
namespace disas8051{
   struct Module{
       const char *name;
       uint16_t start_addr;
       uint16_t end_addr;
   };
   extern Module KB930_modules[];
   #define KB930_MODULES_NUM    (27) 
   extern int get_KB930_ModuleIndex(uint16_t address);
   const char* get_KB930_ModuleName(uint16_t address);
}
#endif

