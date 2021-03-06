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
#include "tabs_setting.hpp"

namespace {
   std::string freertos_version_name = "FreeRTOSv9.0.0";
}

std::string const & get_freertos_version_name() { return freertos_version_name;}

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

   // make conditional on using an anonymous temp dir?
   std::string download_flags = "--no-clobber ";
   #if defined(AIR_INSTALLER_PLATFORM_WINDOWS)
   if (m_src_dir_url.find("https:",0,6) != std::string::npos ){
     download_flags += "--no-check-certificate ";
   }
   #endif
   push_fun("retrieving", m_target_name);
   //std::cout << "retrieving(" << m_target_name << " ... )\n{\n";
   std::string cmd = "wget " + download_flags + " --directory-prefix=" + get_platform()->get_temp_dir()
      + " " + m_src_dir_url + m_src_filename + " --output-document=" + m_unzip_rename;

   // see wgte man
   // 0 --> success
   // 1 generic error
   // 2 error parsing args
   // 3 file i/o error
   //
   int ret = system(cmd.c_str());
   int result = WEXITSTATUS(ret);
   if (result != 0){
      std::string str = "\nError  : " ;
      switch (result){
         case 0:
         
            break;
         case 1:
            str +=  "generic";
            break;
         case 2:
            str+= "parsing args";
            break;
         case 3:
            str +=  "file I/O failed";
            break;
         case 4:
            str += "network failure";
            break;
         case 5:
            str += "SSL verification failed";
            break;
         case 6:
            str += "username/password failure";
            break;
         case 7:
            str += "protocol failed";           
            break;
         case 8:
            str+= "server issued error response";
            break;
         default:
            str += "undiagnosed";
            break;
       }
      // clean up . Possibly partially downloaded file
      str += " downloading " + m_src_dir_url + m_src_filename;
      std::cerr << str << "\n\n";
      std::string stage_src_file = get_platform()->get_temp_dir() + m_src_filename;
      if (file_exists(stage_src_file)){
         cmd = "rm " + stage_src_file;
         system(cmd.c_str());
      }
      change_wkg_dir_to(old_wkg_dir);
      throw std::runtime_error("wget failed");
   }

   std::cout << tabs << "OK;   // (retrieve successful)\n";
   pop_fun();
   change_wkg_dir_to(old_wkg_dir);
   return true;
}

// on entry dir is not staged
// return true if on exit the staged dir is in the temp dir
bool simple_dependency_t::stage_dir()
{
   // dir is not staged but may have been retrieved
   std::string retrieved_url = get_platform()->get_temp_dir() + m_unzip_rename;
   if (!file_exists( retrieved_url) ) {
      if (!retrieve_file()){
         return false;
      }
   }
   // stage here ( uncompress/extract the dir from compressed file)
   std::string old_wkg_dir = get_working_dir();
   change_wkg_dir_to(get_platform()->get_temp_dir());
   push_fun("unzipping", retrieved_url);
   std::string cmd ;
   if ( m_flags & compressed_type_bz2){
      cmd = "tar xjf " + retrieved_url;
   }else{
      assert( m_flags & compressed_type_zip);
      cmd = "unzip -q " + retrieved_url;
   }

   if ( system(cmd.c_str()) == -1){
      change_wkg_dir_to(old_wkg_dir);
      throw std::runtime_error("decompress failed\n");
   }  else{
      
      statement("OK;   // (unzip successful)");
      change_wkg_dir_to(old_wkg_dir);
      pop_fun();
      return true;
   }
}

// on entry gcc dir not installed
// return true if on exit gcc dir is installed
bool simple_dependency_t::move_dir()
{
    std::string staged_path = get_platform()->get_temp_dir() + m_staged_name;
    if ( !dir_exists(staged_path)){
       stage_dir();
    }
    // move staged dir to target dir
    push_fun("moving", m_target_name + "," + get_target_dir());
    std::string old_wkg_dir = get_working_dir();
    change_wkg_dir_to(get_platform()->get_temp_dir());
    std::string cmd = "mv " + staged_path + " " + get_target_dir() + m_target_name;
    if ( system(cmd.c_str()) == -1){
      change_wkg_dir_to(old_wkg_dir);
      throw std::runtime_error("move failed\n");
    }else{
      statement("OK;   // (" +  m_target_name + " moved to " + get_target_dir() + ")");
      pop_fun();
      change_wkg_dir_to(old_wkg_dir);
      return true;
    }
}

bool simple_dependency_t::install()
{
    push_fun("install",m_target_name);
    statement("// Checking for previous installation of " + m_target_name + "...");
    std::string installed_path = get_target_dir() + m_target_name;
    if (! dir_exists(installed_path) ){
      if (! move_dir()){
         return false;
      }
    }
   
    statement("OK;   // (" +  m_target_name + " is installed in " + get_target_dir() + ")");
    pop_fun();
    return true;
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
         ,get_freertos_version_name() + ".zip"
         ,get_freertos_version_name() + ".zip"
         ,get_freertos_version_name()
         ,get_freertos_version_name()
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

dependency_t* make_dependency_mixer_lang()
{
    return new simple_dependency_t{
         dependency_t::QUAN
         ,"https://github.com/kwikius/mixer_lang/archive/"
         ,"master.zip"
         ,"mixer_lang-master.zip"
         ,"mixer_lang-master"
         ,"mixer_lang"
         , simple_dependency_t::compressed_type_zip
         | simple_dependency_t::uncompressed_type_dir
         | simple_dependency_t::target_dir_lib
    };
}
