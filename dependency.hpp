#ifndef AIR_INSTALLER_DEPENDENCY_HPP_INCLUDED
#define AIR_INSTALLER_DEPENDENCY_HPP_INCLUDED

#include <vector>

struct dependency_t{

   enum ID {
      ARM_NONE_EABI_GCC
      ,STM32_STDPERIPH_LIBS
      ,FREERTOS
      ,STM32FLASH
      ,MAVLINK
      ,QUAN
      ,QUANTRACKER
   };
   
   dependency_t(int id);
   virtual bool install() = 0;
   virtual bool uninstall() = 0;
   virtual ~ dependency_t(){}

   int get_id() const { return m_id;}

   private:
      int m_id;
};


#endif // AIR_INSTALLER_DEPENDENCY_HPP_INCLUDED
