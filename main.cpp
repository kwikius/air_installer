#include <cstdlib>
#include <cstring>
#include "config.hpp"
#include "platform.hpp"
#include "dependency_list.hpp"
#include "file_utils.hpp"

#if defined (AIR_INSTALLER_PLATFORM_WINDOWS)
#include "Windows.h"
#endif

/*
  TODO platform dependent, check that apps that are needed are installed
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
   bool result = true;
   while (argnum < argc) {
     int argid = arg_to_id(argv[argnum]);
     
	  switch(argid){
	  case STAGE_DIR:
	  case BIN_DIR:
	  case LIB_DIR:
      if ( argc > (argnum + 1) ){
         std::string target_dir = argv[argnum + 1];
         if (!dir_exists(target_dir)){
           std::cerr << "dir doesnt exist \"" << target_dir << "\"\n";
           return false;
         }
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
         std::cerr << "expected temp dir name\n";
         return false;
      }
	  default:
		  std::cerr << "Error: invalid command line arg '" <<  argv[argnum] <<  "'\n";
		  return false;
	  }
   }
   return result;
}

}// namespace



int main(int argc , const char* argv[])
{
   try{

      std::cout << "\n########################################################\n";
      std::cout << "Quantracker APM air installer V1.0 on " <<  get_platform()->get_OS() << " starting\n";
      std::cout << "########################################################\n\n";
      // checking for required shell apps

     // do_unzip("/home/andy/cpp/ap_temp/FreeRTOSV8.2.3.zip", "/home/andy/cpp/ap_lib/");
//      do_unzip("/home/andy/cpp/ap_temp/quantracker-master.zip", "");
//      return 0;

//#####################################################

      dependency_list_t deps;

      if ( parse_args(argc,argv)){

         std::cout << "args parsed\n";

         // setup any empty paths here
         get_platform()->set_defaults();
         std::cout << "defaults set\n";

         deps.add(dependency_t::ARM_NONE_EABI_GCC);
         deps.add(dependency_t::MAVLINK);
         deps.add(dependency_t::FREERTOS);
         deps.add(dependency_t::STM32_STDPERIPH_LIBS);
         deps.add(dependency_t::QUAN);
         deps.add(dependency_t::QUANTRACKER);

         if ( deps.install() ){

            std::cout << "Install was successful...\n\n";
            std::cout << "libraries are in " << get_platform()->get_lib_dir() <<'\n';
            std::cout << "binaries  are in " << get_platform()->get_bin_dir() << "\n\n";
            std::cout << "########################################################\n";
            std::cout << "Quantracker APM air installer V1.0 on " <<  get_platform()->get_OS() << " completed successfully\n";
            std::cout << "########################################################\n\n\n";
            cleanup();

         return EXIT_SUCCESS;
       }
      }else{
         return EXIT_FAILURE;
      }
   }catch (std::exception & e){
      std::cerr << "ERROR : \"" << e.what() << "\"\n ...quitting\n\n";
   }
}
