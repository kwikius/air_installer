

#include "config.hpp"
#include "platform.hpp"
#include "dependency_list.hpp"
#include <cstring>
#if defined (AIR_INSTALLER_PLATFORM_WINDOWS)
#include "Windows.h"
#endif

/*
   msys utilities to test for
   
   msys-wget
   mysy-tar ( Linux only)
   msys-bzip2 ( linux Only)
   msys-unzip
   unzip
*/

#include <iostream>

namespace {
  
   void cleanup()
   {
      cleanup_platform();
   }
}

// useage progname [ --stage dir ] [--install dir ]

bool parse_args(int argc , const char* argv[])
{
   int argnum = 1;
   while (argnum < argc) {

      if ( strcmp(argv[argnum],"--stage") == 0 ){
         if ( argc > (argnum + 1) ){
            std::string stage_dir = argv[argnum + 1];
            if ( get_platform()->set_temp_dir(stage_dir)){
               argnum += 2;
            }else{
               return false;
            }
            
         }else{
            std::cout << "xpected temp dir name\n";
            return false;
         }
      }else{
         std::cout << "unknown arg\n";
      }
   }
   return true;
}

int main(int argc , const char* argv[])
{
   try{
     // platform_t* platform = get_platform();
      dependency_list_t deps;
      if ( parse_args(argc,argv)){

        // (void) platform;

         deps.add(dependency_t::ARM_NONE_EABI_GCC);
         
         deps.install();

         cleanup();
         return EXIT_SUCCESS;
      }else{
         return EXIT_FAILURE;
      }
   }catch (std::exception & e){
      std::cout << "error " << e.what() << " quitting\n";
   }
}