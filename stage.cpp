
#include "config.hpp"
#include "stage.hpp"
#include <stdexcept>

#if defined (AIR_INSTALLER_PLATFORM_WINDOWS)
#include <cstring>
#include <cstdio>
#include "Windows.h"
#include "io.h"
#endif

   stage_t::stage_t() : m_remove_dir_on_exit{true}
   {
   // create a tempdir for staging
   #if defined(AIR_INSTALLER_PLATFORM_UNIX)
      errno = 0;
      char temp_dir_name[] = "/tmp/air_installer_XXXXXX";
      char * dir_name = mkdtemp(temp_dir_name);
      if ( errno != 0){
         throw std::runtime_error("failed to create temp dir");
      }
      m_temp_dir_name = dir_name;
   #elif defined (AIR_INSTALLER_PLATFORM_WINDOWS)
      char dir_name_cstr [MAX_PATH];
      if (! ( GetTempPath(MAX_PATH,dir_name_cstr)  ) ){
         throw std::runtime_error("failed to get temp dir");
      }
      for(int file_idx = 0;file_idx < 256; ++file_idx){
          std::string temp_dir_name = dir_name_cstr;
          temp_dir_name += "\\air_tool_temp_";
          char fileidx_cstr [5];
          sprintf(fileidx_cstr,"%x",file_idx);
          temp_dir_name += file_idx;
          bool temp_dir_created = CreateDirectory(temp_dir_name.c_str(), NULL);
          if (temp_dir_created){
        	  m_temp_dir_name = temp_dir_name;
        	  break;
          }
      }
	  if ( m_temp_dir_name == ""){
		 throw std::runtime_error("failed to create temp dir");
	  }
                
   #endif  // (AIR_INSTALLER_PLATFORM_WINDOWS)
   }

   stage_t::~stage_t()
   {
      if ( m_remove_dir_on_exit){
    	 // prob need to delete the files in there
         std::string cmd = "rmdir " + m_temp_dir_name;
         system (cmd.c_str());
      }
   }