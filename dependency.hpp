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
   };
   
   dependency_t(int id);
   virtual ~ dependency_t(){}
   virtual bool install() = 0;
   virtual bool uninstall() = 0;
   private:
      int m_id;
};

std::vector<dependency_t*>* get_deps_list();

#endif // AIR_INSTALLER_DEPENDENCY_HPP_INCLUDED
