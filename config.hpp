#ifndef AIR_INSTALLER_CONFIG_HPP_INCLUDED
#define AIR_INSTALLER_CONFIG_HPP_INCLUDED

#if ( defined(__CYGWIN__) && !defined(_WIN32) ) || defined(__unix__)
#define AIR_INSTALLER_PLATFORM_UNIX
#elif defined(_WIN32)
#define AIR_INSTALLER_PLATFORM_WINDOWS
#else
#error "unknown platform"
#endif

bool get_verbose();
void set_verbose( bool b);

#endif // AIR_INSTALLER_CONFIG_HPP_INCLUDED
