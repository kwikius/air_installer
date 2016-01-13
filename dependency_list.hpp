#ifndef AIR_INSTALLER_DEPENDENCY_LIST_HPP_INCLUDED
#define AIR_INSTALLER_DEPENDENCY_LIST_HPP_INCLUDED

#include <map>
#include <list>

#include "dependency.hpp"


struct dependency_list_t {
   dependency_list_t(){}
   ~dependency_list_t();
   void add(int id);
   bool install();
private:
   // deps installed in order in list
   std::list<int> m_dep_id_list;
// owns deps create don heap
   std::map<int, dependency_t*> m_dependency_map;
};

#endif // AIR_INSTALLER_DEPENDENCY_LIST_HPP_INCLUDED
