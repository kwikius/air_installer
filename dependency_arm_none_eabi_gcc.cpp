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
for windows we need to create a subdir of the temp dir
 and unzip into that to make same functionality as the bzip2 Linux version
  so we derive from the simple_depenency and override just the stage_dir function
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
         | simple_dependency_t::target_dir_lib
   }{}

   bool stage_dir()
   {
       // dir is not staged but may have been retrieved
      std::string retrieved_url = get_platform()->get_temp_dir() + m_unzip_rename;
      std::cout << retrieved_url <<'\n';
      if (!file_exists( retrieved_url) ) {
         retrieve_file();
      }

      std::string old_wkg_dir = get_working_dir();
      change_wkg_dir_to(get_platform()->get_temp_dir());

      // make the subdir to unzip into
      std::string subdir_cmd = "mkdir " + m_unzip_rename;
	   system (subdir_cmd.c_str());
      assert( m_flags & compressed_type_zip);
      std::string cmd = "unzip -d " + m_unzip_rename + " " + m_src_filename;
      if ( system(cmd.c_str()) == -1){
         change_wkg_dir_to(old_wkg_dir);
         throw std::runtime_error("decompress failed\n");
      }  else{
         change_wkg_dir_to(old_wkg_dir);
         std::cout << "decompress successful\n";
         return true;
      }
   }

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
         | simple_dependency_t::target_dir_lib
    };
}

#endif
 
#if 0
namespace {
   struct dep_arm_none_eabi : dependency_t{
       dep_arm_none_eabi(): dependency_t{ARM_NONE_EABI_GCC}
       ,m_toolchain_version{"gcc-arm-none-eabi-5_2-2015q4"}
       {}
       bool install();
       bool uninstall();
       bool move_dir();
       bool stage_dir();
       bool retrieve_file();
       
   private:
       std::string m_toolchain_version;
   };

   // on entry file not retrieved
   // return true if on exit retrieved file is in temp dir
   bool dep_arm_none_eabi::retrieve_file()
   {
      std::string src_file = m_toolchain_version +
      #if defined(AIR_INSTALLER_PLATFORM_UNIX)
          "-20151219-linux.tar.bz2";
      #elif defined(AIR_INSTALLER_PLATFORM_WINDOWS)
          "-20151219-win32.zip";
      #endif
      std::string src_url = get_zoomworks_platform_deps_dir() + src_file;
      std::string old_wkg_dir = get_working_dir();
      change_wkg_dir_to(get_platform()->get_temp_dir());

      std::string cmd = "wget --no-clobber --directory-prefix=" + get_platform()->get_temp_dir() + " " + src_url;
      int result = system(cmd.c_str());

      if ( result == -1){
         // clean up . Possibly partially downloaded file
         std::string stage_src_file = get_platform()->get_temp_dir() + src_file;
         if (file_exists(stage_src_file)){  
            cmd = "rm " + stage_src_file;
            system(cmd.c_str());
         }
         change_wkg_dir_to(old_wkg_dir);
         throw std::runtime_error("wget failed");
      }
      change_wkg_dir_to(old_wkg_dir);
      return true;
   }

   // on entry dir is not staged
   // return true if on exit the staged dir is in the temp dir
   bool dep_arm_none_eabi::stage_dir()
   {
      // dir is not staged but may have been retrieved
      std::string retrieved_url = get_platform()->get_temp_dir() + m_toolchain_version + 
      #if defined(AIR_INSTALLER_PLATFORM_UNIX)
             "-20151219-linux.tar.bz2";
      #elif defined(AIR_INSTALLER_PLATFORM_WINDOWS)
             "-20151219-win32.zip";
      #endif
      std::cout << retrieved_url <<'\n';
      if (!file_exists( retrieved_url) ) {
         retrieve_file();
      }
      // stage here ( uncompress/extract the dir from compressed file)
      std::string old_wkg_dir = get_working_dir();
      change_wkg_dir_to(get_platform()->get_temp_dir());
      #if defined(AIR_INSTALLER_PLATFORM_WINDOWS)
      // the windows zip doesnt have the top level dir
      // so we have to make the subdir to unzip it into
	   std::string subdir_cmd = "mkdir " + m_toolchain_version;
	   system (subdir_cmd.c_str());
      #endif
      std::string cmd = 
      #if defined(AIR_INSTALLER_PLATFORM_UNIX)
         "tar xvjf " 
      #elif defined(AIR_INSTALLER_PLATFORM_WINDOWS)
         // unzip into subdir we made
         "unzip -d " + m_toolchain_version + " "
      #endif
          + retrieved_url;
      if ( system(cmd.c_str()) == -1){
         change_wkg_dir_to(old_wkg_dir);
         throw std::runtime_error("decompress failed\n");
      }  else{
         change_wkg_dir_to(old_wkg_dir);
         std::cout << "decompress successful\n";
         return true;
      }
   }

   // on entry gcc dir not installed
   // return true if on exit gcc dir is installed
   bool dep_arm_none_eabi::move_dir()
   {
       std::string staged_path = get_platform()->get_temp_dir() + m_toolchain_version; 
       std::cout << "staged path = " << staged_path << '\n';
       if ( !dir_exists(staged_path)){
          stage_dir();
       }
       // move staged dir to bin dir
       std::string old_wkg_dir = get_working_dir();
       change_wkg_dir_to(get_platform()->get_temp_dir());
       std::string cmd = "mv " + staged_path + " " + get_platform()->get_bin_dir() + m_toolchain_version;
       if ( system(cmd.c_str()) == -1){
         change_wkg_dir_to(old_wkg_dir);
         throw std::runtime_error("move failed\n");
       }else{
         change_wkg_dir_to(old_wkg_dir);
         std::cout << "move successful\n";
         return true;
       }
   }

   bool dep_arm_none_eabi::install()
   {
       std::string installed_path = get_platform()->get_bin_dir() + m_toolchain_version; 
       std::cout << "installed path = " << installed_path <<'\n';
       if (dir_exists(installed_path)){
    	   std::cout << "dir_exists(installed_path)\n";
       }
       return dir_exists(installed_path) || move_dir();
   }

   bool dep_arm_none_eabi::uninstall()
   {
      return false;
   }

} // namespace 

#endif
//dependency_t* make_dependency_arm_none_eabi_gcc()
//{
//   return new dep_arm_none_eabi;
//}
