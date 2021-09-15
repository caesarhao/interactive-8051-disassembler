#ifndef __MODULE_HPP__
#define __MODULE_HPP__
#endif
namespace disas8051{
   struct Module{
       const char *name;
       uint16_t start_addr;
       uint16_t end_addr;
   }; 
}
