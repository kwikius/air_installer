#ifndef AIR_INSTALLER_FILE_UTILS_HPP_INCLUDED
#define AIR_INSTALLER_FILE_UTILS_HPP_INCLUDED

#include <string>

bool file_exists( std::string const & filename);
bool dir_exists(std::string const & dir_name);

std::string get_working_dir();
void change_wkg_dir_to(std::string const & dir);

std::string get_zoomworks_deps_dir();
std::string get_zoomworks_platform_deps_dir();

bool do_unzip( std::string const & src_zip, std::string const & dest_dir = "");

#endif // AIR_INSTALLER_FILE_UTILS_HPP_INCLUDED
