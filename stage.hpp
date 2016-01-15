#ifndef AIR_INSTALLER_STAGE_HPP_INCLUDED
#define AIR_INSTALLER_STAGE_HPP_INCLUDED

#include <string>

struct stage_t{
    // crete a temp dir for staging
    stage_t() ;
    ~stage_t();
     std::string const & get_temp_dir_name() const { return m_temp_dir_name;}
     bool set_dir( std::string const & dir);

     bool set_default_dir();
    private:
     std::string m_temp_dir_name;
     bool m_remove_dir_on_exit;
};


#endif // AIR_INSTALLER_STAGE_HPP_INCLUDED
