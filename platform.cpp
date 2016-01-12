#include "config.hpp"
#include "platform.hpp"
#include "stage.hpp"
#include <cstdlib>
#include <stdexcept>

#if defined(AIR_INSTALLER_PLATFORM_UNIX)
#include <sys/stat.h>
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
     this->set_defaults();
   }
   std::string  platform_t::get_OS() const { return m_os;}
   std::string  platform_t::get_processor() const {return m_processor;}
   stage_t const * platform_t::get_stage()const
   { 
      return &m_stage;
   }
   std::string platform_t::get_lib_dir() const {return m_lib_dir;}
   bool platform_t::set_lib_dir(std::string const & name) { m_lib_dir = name; return true;}
   std::string platform_t::get_bin_dir() const {return m_bin_dir;}
   bool platform_t::set_bin_dir(std::string const & name) { m_bin_dir = name; return true;}
   void platform_t::set_defaults()
   {
#if defined(AIR_INSTALLER_PLATFORM_UNIX)
     // set lib dir to $HOME 
     const char* str_home = getenv("HOME");
     if ( str_home){
        std::string lib_dir = str_home;
        struct stat sb;
         if ( (stat(lib_dir.c_str(), &sb) == 0) && S_ISDIR(sb.st_mode) ){
             const char* subdirs[] = {"lib_dummy","stuff"};
             for ( uint32_t i = 0; i < (sizeof(subdirs) / sizeof(subdirs[0])); ++i){
                  lib_dir += "/";
                  lib_dir += subdirs[i];
                  // continue making dirs
                    if ( stat(lib_dir.c_str(), &sb) == 0){
                           if S_ISDIR(sb.st_mode){
                              continue;
                           }else{
                              throw std::runtime_error("def home lib failed"); 
                           }
                    }else{
                       std::string cmd = "mkdir " + lib_dir;
                       system(cmd.c_str());
                    }
             }
             this->set_lib_dir(lib_dir);
             this->set_bin_dir(lib_dir);
         }
     }else{
         throw std::runtime_error("couldnt get home dir");
     }
#elif defined (AIR_INSTALLER_PLATFORM_WINDOWS)
     // for windows try getenv("HOMEDRIVE") getenv("HOMEPATH")
      
      "Windows"
#endif
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





