#include<cstdlib>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <cassert>

#include "config.hpp"
#include "simple_dependency.hpp"
#include "stage.hpp"
#include "platform.hpp"
#include "file_utils.hpp"

#if defined(AIR_INSTALLER_PLATFORM_WINDOWS)
/*
for Windows we need to create a subdir of the temp dir
 and unzip into that to make same functionality as the bzip2 Linux version
  , since the zip file just expands to various subfolders with no top level dir
  so we derive from the simple_dependency and override just the stage_dir function.
  This makes it compatible with the bzip version
*/
struct dep_arm_none_eabi : simple_dependency_t{

   dep_arm_none_eabi()
   :simple_dependency_t{
         dependency_t::ARM_NONE_EABI_GCC
         ,get_zoomworks_platform_deps_dir()
         ,"gcc-arm-none-eabi-5_2-2015q4-20151219-win32.zip"
         ,"gcc-arm-none-eabi-5_2-2015q4-20151219-win32.zip"
         ,"gcc-arm-none-eabi-5_2-2015q4"
         ,"gcc-arm-none-eabi-5_2-2015q4"
         , simple_dependency_t::compressed_type_zip
         | simple_dependency_t::uncompressed_type_dir
         | simple_dependency_t::target_dir_bin
   }{}


   bool stage_dir() override
   {
       // dir is not staged but may have been retrieved
      std::string retrieved_url = get_platform()->get_temp_dir() + m_unzip_rename;
    //  std::cout << retrieved_url <<'\n';
      if (!file_exists( retrieved_url) ) {
         retrieve_file();
      }

      std::string old_wkg_dir = get_working_dir();
      change_wkg_dir_to(get_platform()->get_temp_dir());

      // make the subdir to unzip into
      std::string subdir_cmd = "mkdir " + m_staged_name;
      system (subdir_cmd.c_str());
      assert( m_flags & compressed_type_zip);
      std::string cmd = "unzip -d " + m_staged_name + " " + m_unzip_rename;
      if ( system(cmd.c_str()) == -1){
         change_wkg_dir_to(old_wkg_dir);
         throw std::runtime_error("decompress failed\n");
      }  else{
         change_wkg_dir_to(old_wkg_dir);
         std::cout << "decompress successful\n";
         return true;
      }
   }
};

   dependency_t* make_dependency_arm_none_eabi_gcc()
   {
      return new dep_arm_none_eabi;
   }
#else
#if (! defined(AIR_INSTALLER_PLATFORM_UNIX))
#error "logic error"
#endif
// For Linux simple_dependency_t works ok
   dependency_t* make_dependency_arm_none_eabi_gcc()
{
    return new simple_dependency_t{
         dependency_t::ARM_NONE_EABI_GCC
         ,get_zoomworks_platform_deps_dir()
          ,"gcc-arm-none-eabi-5_2-2015q4-20151219-linux.tar.bz2"
          ,"gcc-arm-none-eabi-5_2-2015q4-20151219-linux.tar.bz2"
         ,"gcc-arm-none-eabi-5_2-2015q4"
         ,"gcc-arm-none-eabi-5_2-2015q4"
         , simple_dependency_t::compressed_type_bz2
         | simple_dependency_t::uncompressed_type_dir
         | simple_dependency_t::target_dir_bin
    };
}

#endif  // AIR_INSTALLER_PLATFORM_UNIX


