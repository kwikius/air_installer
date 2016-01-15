#include "config.hpp"
#include "platform.hpp"
#include "stage.hpp"
#include <cstdlib>
#include <stdexcept>
#include <iostream>


#if defined(AIR_INSTALLER_PLATFORM_UNIX)
#include <sys/stat.h>
#endif

#if defined (AIR_INSTALLER_PLATFORM_WINDOWS)

#include <iostream>
#include "Windows.h"
#endif

 platform_t::platform_t() // may be null
   :m_os{
#if defined(AIR_INSTALLER_PLATFORM_UNIX)
     "Unix"
#elif defined (AIR_INSTALLER_PLATFORM_WINDOWS)
      "Windows"
#endif
   },
   // todo to dif between e.g pc and raspberry pi
   m_processor{""} 
   {
    if ( get_verbose()){
         std::cout << "Air Installer on " << m_os <<'\n';
    }
   }
   std::string const & platform_t::get_OS() const { return m_os;}
   std::string const & platform_t::get_processor() const {return m_processor;}
   std::string const & platform_t::get_temp_dir() const
   {
      return get_stage()->get_temp_dir_name();
   }
   bool platform_t::set_temp_dir(std::string const & dir)
   {
       return  m_stage.set_dir(dir);
   }
   stage_t const * platform_t::get_stage()const
   { 
      return &m_stage;
   }
   std::string const & platform_t::get_lib_dir() const {return m_lib_dir;}
   bool platform_t::set_lib_dir(std::string const & name) 
   {
      std::string dir = name;
      if ( dir.at(dir.length() -1) != get_platform()->get_dir_sep().at(0)){
        m_lib_dir = dir + get_platform()->get_dir_sep();
      }else{
        m_lib_dir = dir;
      }
      return true;
   }
   std::string const & platform_t::get_bin_dir() const {return m_bin_dir;}
   bool platform_t::set_bin_dir(std::string const & name) 
   {
      std::string dir = name;
      if ( dir.at(dir.length() -1) != get_platform()->get_dir_sep().at(0)){
        m_bin_dir = dir + get_platform()->get_dir_sep();
      }else{
        m_bin_dir = dir;
      }
      return true;
   }
   void platform_t::set_defaults()
   { 
     m_stage.set_default_dir();
    if (( m_lib_dir == "") || ( m_bin_dir == "")){
        // for testing
        const char* subdirs[] = {"lib_dummy","stuff"};
   #if defined(AIR_INSTALLER_PLATFORM_UNIX)
        // set lib dir to $HOME 
        const char* str_home = getenv("HOME");
   #elif defined (AIR_INSTALLER_PLATFORM_WINDOWS)
        const char*  str_home = getenv("USERPROFILE");
   #else
   #error "shouldnt get here"
   #endif
        if ( str_home){
           std::string lib_dir = str_home;   
        #if defined(AIR_INSTALLER_PLATFORM_UNIX)   
           struct stat sb;
           if ( (stat(lib_dir.c_str(), &sb) == 0) && S_ISDIR(sb.st_mode) ){
               for ( uint32_t i = 0; i < (sizeof(subdirs) / sizeof(subdirs[0])); ++i){
                  // no dir sep applied on linux
                  if ( lib_dir.at( lib_dir.length() -1) != get_platform()->get_dir_sep().at(0)){
                      lib_dir += get_platform()->get_dir_sep().at(0);
                  }
                  lib_dir += subdirs[i];
                  // continue making dirs
                    if ( stat(lib_dir.c_str(), &sb) == 0){
                        if (S_ISDIR(sb.st_mode)){
                           continue;
                        }else{
                           throw std::runtime_error("def home lib failed"); 
                        }
                    }else{
                       std::string cmd = "mkdir " + lib_dir;
                       system(cmd.c_str());
                    }
               }
               if ( m_lib_dir == "") {
                   this->set_lib_dir(lib_dir);
               }
               if ( m_bin_dir == ""){
                  this->set_bin_dir(lib_dir);
               }
            }
         #else
             DWORD flags = GetFileAttributes(lib_dir.c_str());
             if ( (flags != INVALID_FILE_ATTRIBUTES) && ( flags & FILE_ATTRIBUTE_DIRECTORY) ){
                 for ( uint32_t i = 0; i < (sizeof(subdirs) / sizeof(subdirs[0])); ++i){
                 if ( lib_dir.at( lib_dir.length() -1) != get_platform()->get_dir_sep().at(0)){
                    lib_dir += get_platform()->get_dir_sep().at(0);
                 }
                 lib_dir += subdirs[i];
                 flags = GetFileAttributes(lib_dir.c_str());
                 if ((flags != INVALID_FILE_ATTRIBUTES) && ( flags & FILE_ATTRIBUTE_DIRECTORY) ){
                   // subdir exists already
                   continue;
                 }else{
                    if ( !CreateDirectory(lib_dir.c_str(), NULL) ){
                       throw std::runtime_error("faied to create def bin and lib subdirs");
                    }
                 }
               }
               if ( m_lib_dir == "") {
                   this->set_lib_dir(lib_dir);
               }
               if ( m_bin_dir == ""){
                  this->set_bin_dir(lib_dir);
               }
             }else{
               throw std::runtime_error("faied to create def bin and lib dir");
             }
         #endif
            
        }else{
            throw std::runtime_error("couldnt get home dir");
        }
     }
   }
namespace {
   platform_t*  platform = nullptr;
}

platform_t* get_platform()
{
   if ( platform == nullptr){
     platform = new platform_t;  
   }
   return platform;
}

std::string platform_t::get_dir_sep()
{
#if defined(AIR_INSTALLER_PLATFORM_UNIX)
   return "/";
#elif defined (AIR_INSTALLER_PLATFORM_WINDOWS)
   return "\\";
#else
   #error "need to define dir sep for this platform"
#endif
}

void cleanup_platform()
{
   delete platform;
   platform = nullptr;
}





