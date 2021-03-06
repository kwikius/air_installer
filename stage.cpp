
#include <cerrno>
#include <cstdlib>
#include <stdexcept>
#include <iostream>

#include "config.hpp"
#include "platform.hpp"
#include "stage.hpp"
#include "file_utils.hpp"


#if defined (AIR_INSTALLER_PLATFORM_WINDOWS)
#include <cstring>
#include <cstdio>
#include "Windows.h"
#include "io.h"
#endif

   stage_t::stage_t() : m_remove_dir_on_exit{true}
   {}

   bool stage_t::set_default_dir()
   {
     if ( m_temp_dir_name != ""){
         return true;
     }
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
          temp_dir_name += "air_tool_temp_";
          char fileidx_cstr [5];
          sprintf(fileidx_cstr,"%x",file_idx);
          temp_dir_name += fileidx_cstr;
          bool temp_dir_created = CreateDirectory(temp_dir_name.c_str(), NULL);
          if (temp_dir_created){
        	   m_temp_dir_name = temp_dir_name  + '\\';
        	   break;
          }
      }
	  if ( m_temp_dir_name == ""){
		 throw std::runtime_error("failed to create temp dir");
	  } else{
		// std::cout << "temp dir name = " << m_temp_dir_name;
	  }
   #endif  // (AIR_INSTALLER_PLATFORM_WINDOWS)
     return true;
   }

   bool stage_t::set_dir( std::string const & dir)
   {
       if ( dir_exists(dir)){
            if ( (m_temp_dir_name != "") && (m_remove_dir_on_exit == true) && (m_temp_dir_name != dir)){
                std::string cmd = "rmdir " + m_temp_dir_name;
               // std::cout  <<" cmd is " << cmd << '\n';
                int result =  system (cmd.c_str());
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
      // this only happens in case the stage dir was not specified at cmd line
      if ( m_remove_dir_on_exit &&  
            (get_platform()->get_temp_dir() != "") && 
               ( dir_exists(get_platform()->get_temp_dir()) ) ){
            std::string old_wkg_dir = get_working_dir();
            change_wkg_dir_to(get_platform()->get_temp_dir());
            system ("rm -f *.zip *.bz2");
            // for testing
//            std::cout << "Pres key for del\n";
//            char ch;
//            std::cin.get(ch);
            change_wkg_dir_to(old_wkg_dir);
            std::string cmd = "rmdir  " + get_platform()->get_temp_dir();
            system (cmd.c_str());

      }
   }
