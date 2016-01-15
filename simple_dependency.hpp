#ifndef AIR_INSTALLER_SIMPLE_DEPENDENCY_HPP_INCLUDED
#define AIR_INSTALLER_SIMPLE_DEPENDENCY_HPP_INCLUDED

#include "dependency.hpp"
/*

install_target{
   target_name; // could be file or dir
   install_dir; // parent of target
};

// the src where the file is to be retrieved from
download_src{
   src_name;  // name of src zip file
   uncompressed_name; // name of 
   src_zip_type;  // zip or bzip
}

platform linux

  src_url  ->        platform_target_dir. target
  #zoomworks.linux.arm-non-eabi-gcc-bz2  ->   platform.bin-dir.arm-none-eabi-gcc-version
  zoomworks.FreeRTOS-zip                ->   platform.lib-dir.FreeRTOS-version
  #zoomworks.mavlink-zip                 ->   platform.lib-dir.mavlink
  zoomworks.stm32-stdperiph-zip         ->   platform.lib-dir.stdperiph-version

  github.kwikius.quan_trunk_master.quan_trunk -> platform.lib-dir.quan_trunk
  quantracker
  
*/

struct simple_dependency_t : dependency_t{
   static constexpr uint32_t compressed_type_zip = (1 << 0);
   static constexpr uint32_t compressed_type_bz2 = (1 << 1);
   static constexpr uint32_t uncompressed_type_file = (1 << 2);
   static constexpr uint32_t uncompressed_type_dir = (1 << 3);
   static constexpr uint32_t target_dir_bin = (1 << 4);
   static constexpr uint32_t target_dir_lib = (1 << 5);
   simple_dependency_t(
       int id,
       std::string const & src_dir_url  // '/' at end
      ,std::string const & src_filename
      ,std::string const & unzip_rename
      ,std::string const & staged_name
      ,std::string const & target_name
      ,uint32_t flags
   )
   :dependency_t{id}
   ,m_src_dir_url{src_dir_url}
   ,m_src_filename{src_filename}
   ,m_unzip_rename{unzip_rename}
   ,m_staged_name{staged_name}
   ,m_target_name{target_name}
   ,m_flags{flags}
   {
      if ( src_dir_url.at(src_dir_url.length()-1) != '/'){
        m_src_dir_url += "/";
      }
      //TODO  add check on flags
   }

   bool install();
   bool uninstall();
   bool move_dir();
   bool stage_dir();
   bool retrieve_file();

   std::string get_target_dir();
private:
   std::string  m_src_dir_url; // the src dir url - without the file name. ( So we can separate the filename easily)
   std::string const m_src_filename; // the name of the compressed file  without the directory/url info
   std::string const m_unzip_rename; // rename the unzipped file to prevent collisions
   std::string const m_staged_name; // The uncompressed top-level file/dir name  
   std::string const m_target_name; //  The name of the target - dir
   uint32_t const m_flags;
};

#endif // AIR_INSTALLER_SIMPLE_DEPENDENCY_HPP_INCLUDED
