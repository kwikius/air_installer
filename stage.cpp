
#include "config.hpp"
#include "platform.hpp"
#include "stage.hpp"
#include "file_utils.hpp"
#include <stdexcept>
#include <iostream>

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
      char temp_dir_name_cstr[] = "/tmp/air_installer_XXXXXX";
      char * dir_name = mkdtemp(temp_dir_name_cstr);
      if ( ! dir_name  || ( errno != 0) ){
         throw std::runtime_error("failed to create temp dir");
      }
      
      m_temp_dir_name = std::string{temp_dir_name_cstr} + '/';
   #elif defined (AIR_INSTALLER_PLATFORM_WINDOWS)
      // for windows get the offical temp directory
      char dir_name_cstr [MAX_PATH];
      if (! ( GetTempPath(MAX_PATH,dir_name_cstr)  ) ){
         throw std::runtime_error("failed to get temp dir");
      }
      // make a unique dir name by adding a number to a name
      // give up after 255 attempts
      // If repeated install attempts then this may fill up
      // so may be best to tell user
      for(int file_idx = 0;file_idx < 256; ++file_idx){
          std::string temp_dir_name = dir_name_cstr;
          temp_dir_name += "\\air_tool_temp_";
          char fileidx_cstr [5];
          sprintf(fileidx_cstr,"%x",file_idx);
          temp_dir_name += file_idx;
          bool temp_dir_created = CreateDirectory(temp_dir_name.c_str(), NULL);
          if (temp_dir_created){
        	   m_temp_dir_name = temp_dir_name  + '\\';
        	   break;
          }
      }
	  if ( m_temp_dir_name == ""){
		 throw std::runtime_error("failed to create temp dir");
	  } 
   #endif  // (AIR_INSTALLER_PLATFORM_WINDOWS)
   }

   bool stage_t::set_dir( std::string const & dir)
   {
       if ( dir_exists(dir)){
            if ((m_remove_dir_on_exit == true) &&  (m_temp_dir_name != dir)){
                std::string cmd = "rmdir " + m_temp_dir_name;
                int result = system (cmd.c_str());
                if (result == -1){
                  std::cout << "warning couldnt remove old temp dir\n";
                }
            }
            m_remove_dir_on_exit = false;
            if ( dir.at(dir.length() -1) != get_platform()->get_dir_sep().at(0)){
               m_temp_dir_name = dir + get_platform()->get_dir_sep();
            }else{
              m_temp_dir_name = dir;
            }
            return true;
       }else{
         std::cout << "dir not found\n";
         return false;
       }
     
   }

   stage_t::~stage_t()
   {
      if ( m_remove_dir_on_exit){
         std::string cmd = "rmdir " + m_temp_dir_name;
         int result = system (cmd.c_str());
         if ( result != -1){
            if ( get_verbose()){
               std::cout << "Deleted stage dir \"" << m_temp_dir_name << "\"\n";
            }
         }
    	  //Will prob need to delete the files in there
         
      }
   }