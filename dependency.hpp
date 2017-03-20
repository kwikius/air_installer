#ifndef AIR_INSTALLER_DEPENDENCY_HPP_INCLUDED
#define AIR_INSTALLER_DEPENDENCY_HPP_INCLUDED

#include <vector>
#include <string>

struct dependency_t{

   enum ID {
      ARM_NONE_EABI_GCC
      ,STM32_STDPERIPH_LIBS
      ,FREERTOS
      ,STM32FLASH
      ,MAVLINK
      ,QUAN
      ,QUANTRACKER
      ,MIXER_LANG
   };
   
   dependency_t(int id);
   virtual bool install() = 0;
   virtual bool uninstall() = 0;
   virtual ~ dependency_t(){}

   int get_id() const { return m_id;}

   private:
      int m_id;
};

std::string const & get_gcc_arm_none_eabi_version_name();
std::string const & get_gcc_arm_none_eabi_version_number();
std::string const & get_freertos_version_name();


#endif // AIR_INSTALLER_DEPENDENCY_HPP_INCLUDED
