#include "dependency_list.hpp"
#include "make_dependency.hpp"
#include <stdexcept>

dependency_t* make_dependency( int id)
{
   switch ( id ){
     case dependency_t::ARM_NONE_EABI_GCC:
         return make_dependency_arm_non_eabi_gcc();
     default:
         return nullptr;
   }
}

bool dependency_list_t::install()
{
  for ( int i : m_dep_id_list){
    if ( !m_dependency_map[i]->install()){
      return false;
    }
  }
  return true;
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