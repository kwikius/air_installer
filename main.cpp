#include <cstdlib>
#include <cstring>
#include "config.hpp"
#include "platform.hpp"
#include "dependency_list.hpp"
#include "file_utils.hpp"
#include "tabs_setting.hpp"

//dummy

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
   
   while (argnum < argc) {
     int argid = arg_to_id(argv[argnum]);
     bool result = false;
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
   return true;
}
//
}// namespace

tabs_setting_t tabs{3};

int main(int argc , const char* argv[])
{
   try{
      std::cout << "\n\n##################################";
      std::cout << "\nQuantracker installer V1.0";
      std::cout << "\n##################################\n\n";
      push_fun("install_package","quantracker-dependencies");

      // TODO add checking for required shell apps
      // wget
      // unzip
      // mv
      // mkdir
      // tar
      // rmdir
      // rm

      dependency_list_t deps;

      if ( parse_args(argc,argv)){

         // setup any empty paths here
         get_platform()->set_defaults();

         deps.add(dependency_t::ARM_NONE_EABI_GCC);
         deps.add(dependency_t::MAVLINK);
         deps.add(dependency_t::FREERTOS);
         deps.add(dependency_t::STM32_STDPERIPH_LIBS);
         deps.add(dependency_t::QUAN);
         deps.add(dependency_t::QUANTRACKER);

         if ( deps.install() ){
 
            statement("#########################################################");
            statement("Install was successful");
            statement("libraries are in " + get_platform()->get_lib_dir());
            statement("binaries  are in " + get_platform()->get_bin_dir());
            statement("#########################################################");
            cleanup();
            pop_fun();

            return EXIT_SUCCESS;
       }
      }else{
         return EXIT_FAILURE;
      }
   }catch (std::exception & e){
      std::cerr << "ERROR : \"" << e.what() << "\"\n ...quitting\n\n";
      return EXIT_FAILURE;
   }
}
