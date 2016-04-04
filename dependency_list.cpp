#include "dependency_list.hpp"
#include "make_dependency.hpp"
#include "platform.hpp"
#include "tabs_setting.hpp"
#include <stdexcept>
#include <cassert>
#include <fstream>
#include <iostream>

dependency_t* make_dependency( int id)
{
   switch ( id ){
     case dependency_t::ARM_NONE_EABI_GCC:
         return make_dependency_arm_none_eabi_gcc();
     case dependency_t::MAVLINK:
         return make_dependency_mavlink();
     case dependency_t::FREERTOS:
         return make_dependency_freertos();
     case dependency_t::STM32_STDPERIPH_LIBS:
         return make_dependency_stm32_stdperiph_libs();
     case dependency_t::QUAN:
         return make_dependency_quan();
     case dependency_t::QUANTRACKER:
         return make_dependency_quantracker();
     default:
         assert(false);
         return nullptr;
   }
}

bool dependency_list_t::install()
{
  for ( int i : m_dep_id_list){
    if ( !m_dependency_map[i]->install()){
      return false;
    }
    std::cout << "\n";
  } 
  return make_dependencies_mk();
}

bool dependency_list_t::make_dependencies_mk()
{
   
  std::string lib_dir = get_platform()->get_lib_dir();
  std::string bin_dir = get_platform()->get_bin_dir();
  std::string filename = lib_dir + "quantracker/Dependencies.mk";
  push_fun("write",filename);
  {
      std::ofstream out (filename.c_str());
      if ( ! out  || out.fail()){
         throw std::logic_error("file open failed\n");
      }
      out << "TOOLCHAIN_PREFIX = " << bin_dir << "gcc-arm-none-eabi-5_2-2015q4/\n";
      out << "TOOLCHAIN_GCC_VERSION = 5.2.1\n";
      out << "STM32FLASH = stm32flash\n";
      out << "QUAN_INCLUDE_PATH = " << lib_dir << "quan-trunk\n";
      out << "MAVLINK_INCLUDE_PATH = " << lib_dir << '\n';
      out << "FREE_RTOS_DIR = " << lib_dir << "FreeRTOSV8.2.3/FreeRTOS/\n";
      out << "STM32_STD_PERIPH_LIB_DIR = " << lib_dir << "STM32F4xx_DSP_StdPeriph_Lib_V1.6.1/Libraries/\n\n";
  }
  std::cout << tabs << "OK;   // (dependencies file written successfully)\n";
  pop_fun();
}

dependency_list_t::~dependency_list_t()
{
 // clean up map elements
   for(  auto & elem : m_dependency_map){
     delete elem.second;
   }
}

void dependency_list_t::add( int id)
{
   if ( m_dependency_map.find(id) == m_dependency_map.end()){
      dependency_t * dep = make_dependency(id);
      if (dep == nullptr){
         throw std::logic_error{"dependency id not found"};
      }
       m_dep_id_list.push_back(id);
       m_dependency_map[id] = dep;
   }
}