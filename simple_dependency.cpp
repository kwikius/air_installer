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

std::string simple_dependency_t::get_target_dir()
{
    if ( m_flags & target_dir_bin){
      return get_platform()->get_bin_dir(); 
    }else{
      return get_platform()->get_lib_dir(); 
    }
}

// on entry file not retrieved
// return true if on exit retrieved file is in temp dir
bool simple_dependency_t::retrieve_file()
{
   std::string old_wkg_dir = get_working_dir();
   change_wkg_dir_to(get_platform()->get_temp_dir());

   std::string cmd = "wget --no-clobber --directory-prefix=" + get_platform()->get_temp_dir() 
      + " " + m_src_dir_url + m_src_filename;
   int result = system(cmd.c_str());

   if ( result == -1){
      // clean up . Possibly partially downloaded file
      std::string stage_src_file = get_platform()->get_temp_dir() + m_src_filename;
      if (file_exists(stage_src_file)){  
         cmd = "rm " + stage_src_file;
         system(cmd.c_str());
      }
      change_wkg_dir_to(old_wkg_dir);
      throw std::runtime_error("wget failed");
   }
   if ( m_unzip_rename != m_src_filename){
      cmd = "mv " +  m_src_filename + " " + m_unzip_rename;
      system(cmd.c_str());
   }
   change_wkg_dir_to(old_wkg_dir);
   return true;
}

// on entry dir is not staged
// return true if on exit the staged dir is in the temp dir
bool simple_dependency_t::stage_dir()
{
   // dir is not staged but may have been retrieved
   std::string retrieved_url = get_platform()->get_temp_dir() + m_unzip_rename;
   std::cout << retrieved_url <<'\n';
   if (!file_exists( retrieved_url) ) {
      retrieve_file();
   }
   // stage here ( uncompress/extract the dir from compressed file)
   std::string old_wkg_dir = get_working_dir();
   change_wkg_dir_to(get_platform()->get_temp_dir());
  
   std::string cmd ;
   if ( m_flags & compressed_type_bz2){
      cmd = "tar xvjf " + retrieved_url;
   }else{
      assert( m_flags & compressed_type_zip);
      cmd = "unzip " + retrieved_url;
   }
   
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
bool simple_dependency_t::move_dir()
{
    std::string staged_path = get_platform()->get_temp_dir() + m_staged_name; 
    std::cout << "staged path = " << staged_path << '\n';
    if ( !dir_exists(staged_path)){
       stage_dir();
    }
    // move staged dir to target dir
    std::string old_wkg_dir = get_working_dir();
    change_wkg_dir_to(get_platform()->get_temp_dir());
    std::string cmd = "mv " + staged_path + " " + get_target_dir() + m_target_name;
    if ( system(cmd.c_str()) == -1){
      change_wkg_dir_to(old_wkg_dir);
      throw std::runtime_error("move failed\n");
    }else{
      change_wkg_dir_to(old_wkg_dir);
      std::cout << "move successful\n";
      return true;
    }
}

bool simple_dependency_t::install()
{
    std::string installed_path = get_target_dir() + m_target_name; 
    std::cout << "installed path = " << installed_path <<'\n';
    if (dir_exists(installed_path)){
      std::cout << "dir_exists(installed_path)\n";
    }
    return dir_exists(installed_path) || move_dir();
}

bool simple_dependency_t::uninstall()
{
   return false;
}

dependency_t* make_dependency_mavlink()
{
    return new simple_dependency_t{
         dependency_t::MAVLINK
        , get_zoomworks_deps_dir()
         ,"mavlink.zip"
         ,"mavlink.zip"
         ,"c_library-master"
         ,"mavlink"
         , simple_dependency_t::compressed_type_zip
         | simple_dependency_t::uncompressed_type_dir
         | simple_dependency_t::target_dir_lib
    };
}

dependency_t* make_dependency_freertos()
{
    return new simple_dependency_t{
         dependency_t::FREERTOS
        , get_zoomworks_deps_dir()
         ,"FreeRTOSV8.2.3.zip"
         ,"FreeRTOSV8.2.3.zip"
         ,"FreeRTOSV8.2.3"
         ,"FreeRTOSV8.2.3"
         , simple_dependency_t::compressed_type_zip
         | simple_dependency_t::uncompressed_type_dir
         | simple_dependency_t::target_dir_lib
    };
}

dependency_t* make_dependency_stm32_stdperiph_libs()
{
   return new simple_dependency_t{
         dependency_t::STM32_STDPERIPH_LIBS
        , get_zoomworks_deps_dir()
         ,"stm32f4_dsp_stdperiph_lib.zip"
         ,"stm32f4_dsp_stdperiph_lib.zip"
         ,"STM32F4xx_DSP_StdPeriph_Lib_V1.6.1"
         ,"STM32F4xx_DSP_StdPeriph_Lib_V1.6.1"
         , simple_dependency_t::compressed_type_zip
         | simple_dependency_t::uncompressed_type_dir
         | simple_dependency_t::target_dir_lib
    };
}

dependency_t* make_dependency_quan()
{
    return new simple_dependency_t{
         dependency_t::QUAN
         ,"https://github.com/kwikius/quan-trunk/archive/"
         ,"master.zip"
         ,"quan-trunk-master.zip"
         ,"quan-trunk-master"
         ,"quan-trunk"
         , simple_dependency_t::compressed_type_zip
         | simple_dependency_t::uncompressed_type_dir
         | simple_dependency_t::target_dir_lib
    };
}

dependency_t* make_dependency_quantracker()
{
    return new simple_dependency_t{
         dependency_t::QUANTRACKER
         ,"https://github.com/kwikius/quantracker/archive/"
         ,"master.zip"
         ,"quantracker-master.zip"
         ,"quantracker-master"
         ,"quantracker"
         , simple_dependency_t::compressed_type_zip
         | simple_dependency_t::uncompressed_type_dir
         | simple_dependency_t::target_dir_lib
    };
}
