#include "file_utils.hpp"
#include "config.hpp"
#include <stdexcept>

#if defined(AIR_INSTALLER_PLATFORM_UNIX)

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#if defined (AIR_INSTALLER_PLATFORM_WINDOWS)
#include "Windows.h"
#endif

bool file_exists( std::string const&  filename)
{
#if defined(AIR_INSTALLER_PLATFORM_UNIX)
  struct stat sb;
  return( (stat(filename.c_str(), &sb) == 0) && S_ISREG(sb.st_mode));
#elif defined(AIR_INSTALLER_PLATFORM_WINDOWS)
  DWORD flags GetFileAttributes(filename.c_str());
  return ( (flags != INVALID_FILE_ATTRIBUTES) && !(flags & FILE_ATTRIBUTE_DIRECTORY));
#else
  #error "need def for this platform"
#endif
}

bool dir_exists( std::string const& dirname)
{
#if defined(AIR_INSTALLER_PLATFORM_UNIX)
  struct stat sb;
  return( (stat(dirname.c_str(), &sb) == 0) && S_ISDIR(sb.st_mode));
#elif defined(AIR_INSTALLER_PLATFORM_WINDOWS)
  DWORD flags GetFileAttributes(dirname.c_str());
  return ( (flags != INVALID_FILE_ATTRIBUTES) && (flags & FILE_ATTRIBUTE_DIRECTORY));
#else
  #error "need def for this platform"
#endif
}

std::string get_working_dir()
{
#if defined(AIR_INSTALLER_PLATFORM_UNIX)
   char buf[1000];
   if ( getcwd(buf,1000) == nullptr){
      throw std::runtime_error("pwd failed");
   }
   return buf;
#elif defined(AIR_INSTALLER_PLATFORM_WINDOWS)
#error "todo"
#endif
}
void change_wkg_dir_to(std::string const & dir)
{
#if defined(AIR_INSTALLER_PLATFORM_UNIX)
   if ( chdir(dir.c_str()) != 0){
      throw std::runtime_error("cd failed");
   }
#elif defined(AIR_INSTALLER_PLATFORM_WINDOWS)
#error "todo"
#endif
}

std::string get_zoomworks_deps_dir() 
{
  return "http://www.zoomworks.org/Downloads/quantracker_dependencies/";
}
std::string get_zoomworks_linux_deps_dir()
{
   return get_zoomworks_deps_dir() + "Linux/";
}
std::string get_zoomworks_windows_deps_dir()
{
   return get_zoomworks_deps_dir() + "Windows/";
}