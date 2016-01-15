#include "config.hpp"

namespace {
   bool flags_verbose = true;
}

void set_verbose(bool b) 
{ flags_verbose = b;}

bool get_verbose()
{
   return flags_verbose;
}

