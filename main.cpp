
#include <cstring>
#include "config.hpp"
#include "platform.hpp"
#include "dependency_list.hpp"

#if defined (AIR_INSTALLER_PLATFORM_WINDOWS)
#include "Windows.h"
#endif

/*
   msys utilities to test for
   
   msys-wget
   #mysy-tar ( not used on windows )
   #msys-bzip2 (not used on windows)
   msys-unzip
   unzip
*/

#include <iostream>

namespace {
  
   void cleanup()
   {
      cleanup_platform();
   }

   enum arg_id{
	  UNKNOWN_ARG
	  ,STAGE_DIR
	  ,BIN_DIR
	  ,LIB_DIR
   };

   int arg_to_id(const char* arg)
   {
	   if ( strcmp(arg,"--stage-dir") == 0 ){
		   return STAGE_DIR;
	   }
	   if ( strcmp(arg,"--bin-dir") == 0 ){
	   	   return BIN_DIR;
	   }
	   if ( strcmp(arg,"--lib-dir") == 0 ){
	  	   return LIB_DIR;
	   }
	   return UNKNOWN_ARG;
   }

// useage:
//progname [ --stage-dir dir ] [--lib-dir dir ] [--bin-dir dir]

bool parse_args(int argc , const char* argv[])
{
   int argnum = 1; // ignore progname
   while (argnum < argc) {
      int argid = arg_to_id(argv[argnum]);
	  switch(argid){
	  case STAGE_DIR:
	  case BIN_DIR:
	  case LIB_DIR:
         if ( argc > (argnum + 1) ){
            std::string target_dir = argv[argnum + 1];
            std::cout << "target dir = " << target_dir  <<'\n';
            bool result = false;
            switch (argid){
            case STAGE_DIR:
            	result = get_platform()->set_temp_dir(target_dir);
            	break;
      	    case BIN_DIR:
      	    	result = get_platform()->set_bin_dir(target_dir);
      	    	break;
      	    case LIB_DIR:
      	    	result = get_platform()->set_lib_dir(target_dir);
      	    	break;
            }
            if ( result ){
               argnum += 2;
               break;
            }else{
               return false; // has reported
            }
         }else{
            std::cout << "expected temp dir name\n";
            return false;
         }

	  default:
		  std::cout << "unknown arg\n";
		  return false;
	  }
   }
   return true;
}

}// namespace

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
