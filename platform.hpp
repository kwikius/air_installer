#ifndef AIR_INSTALLER_PLATFORM_HPP_INCLUDED
#define AIR_INSTALLER_PLATFORM_HPP_INCLUDED

#include <string>
#include "stage.hpp"

struct platform_t{
   platform_t();
   stage_t const * get_stage() const;
   std::string get_OS() const;
   std::string  get_processor() const ;

   virtual ~platform_t(){}
   bool set_lib_dir(std::string const & dir);
   bool set_bin_dir(std::string const & dir);
   std::string get_lib_dir() const ;
   std::string get_bin_dir() const ;
   static std::string get_dir_sep();
private:
   void set_defaults();
   std::string m_os;    
   std::string m_processor;
   std::string m_lib_dir;
   std::string m_bin_dir;
   stage_t     m_stage;
};

platform_t* get_platform();
void        cleanup_platform();


#endif // AIR_INSTALLER_PLATFORM_HPP_INCLUDED
