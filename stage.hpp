#ifndef AIR_INSTALLER_STAGE_HPP_INCLUDED
#define AIR_INSTALLER_STAGE_HPP_INCLUDED

#include <string>

struct stage_t{
    // crete a temp dir for staging
    stage_t() ;
    ~stage_t();
    private:
     std::string m_temp_dir_name;
     bool m_remove_dir_on_exit;
};


#endif // AIR_INSTALLER_STAGE_HPP_INCLUDED
