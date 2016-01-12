
#include "config.hpp"
#include "stage.hpp"
#include <stdexcept>

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
   #elif defined (AIR_INSTALLER_PLATFORM_WINDOWS)
      char temp_dir_name [ MAX_PATH];
      if (! (( GetTempPath(temp_dir_name) && CreateDirectory(temp_dir_name, NULL)){
         throw std::runtime_error("failed to create temp dir");
      }
   #endif
      m_temp_dir_name = dir_name;
   }

   stage_t::~stage_t()
   {
      if ( m_remove_dir_on_exit){
         std::string cmd = "rmdir " + m_temp_dir_name;
         system (cmd.c_str());
      }
   }