

#include "config.hpp"
#include "platform.hpp"
#include "dependency.hpp"
#if defined (AIR_INSTALLER_PLATFORM_WINDOWS)
#include "Windows.h"
#endif

#include <iostream>

namespace {
  
   void cleanup()
   {
      cleanup_platform();
   }
}

// useage progname 

int main(int argc , const char* argv[])
{
   platform_t* platform = get_platform();
   
   std::cout << platform->get_OS() <<'\n';
   std::cout << platform->get_lib_dir() <<'\n';

   cleanup();
   return EXIT_SUCCESS;
}