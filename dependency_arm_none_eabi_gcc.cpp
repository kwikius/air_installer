
#include "config.hpp"
#include "dependency.hpp"
#include "platform.hpp"

// source is 

namespace {
   struct dep_arm_none_eabi : dependency_t{
       dep_arm_none_eabi(): dependency_t{ARM_NONE_EABI_GCC}
       ,m_toolchain_version{"gcc-arm-none-eabi-5_2-2015q4-20151219"}
       {}
       bool install();
       bool uninstall();
   private:
       std::string m_toolchain_version;
   };

   bool dep_arm_none_eabi::install()
   {
      // check for installed
      std::string install_dir = get_platform()->get_bin_dir();
      std::string install_path = install_dir + "/" + m_toolchain_version; 
      return false;
   }

   bool dep_arm_none_eabi::uninstall()
   {
      return false;
   }


}
