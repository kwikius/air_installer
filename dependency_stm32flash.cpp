
#include<cstdlib>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <cassert>

#include "config.hpp"
#include "simple_dependency.hpp"
#include "stage.hpp"
#include "platform.hpp"
#include "file_utils.hpp"

/*
   download is same
   stage is same
   install 
*/

struct dependency_stm32flash : dependency_t{
// url = "http://sourceforge.net/projects/stm32flash/files/stm32flash-0.4.tar.gz"
  dependency_stm32flash()
 :dependency_t{dependency_t::STM32FLASH}{}

   bool install() override
   {
      // look for existence in bin dir
      // look for existence in global path
      
      // change to temp dir
      // make a subdir stm32flash
      return false;

   }

   bool uninstall() override
   {
      // look for existence in bin dir
      // look for existence in global path
      
      // change to temp dir
      // make a subdir stm32flash
      return false;

   }

};
