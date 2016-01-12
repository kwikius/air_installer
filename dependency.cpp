#include "config.hpp"
#include "dependency.hpp"

namespace {
   std::vector<dependency_t*> deps_list;
}

dependency_t::dependency_t(int id):m_id{id}{}

std::vector<dependency_t*>* get_deps_list()
{
   return &deps_list;
}





