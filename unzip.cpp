
/*
   Basically derived from https://wkik/wxwidgets.org/WxZipInputStream
   Ultimately just needed on MS Windows, just here for testing
*/

#include <wx/zipstrm.h>
#include <wx/wfstream.h>
#include <wx/file.h>
#include <wx/filefn.h>
#include <wx/filename.h>
#include "file_utils.hpp"
#include <stdexcept>
//#include <wx/textfile.h>

#include <quan/gx/wxwidgets/from_wxString.hpp>
#include <quan/gx/wxwidgets/to_wxString.hpp>
#include <cassert>

using quan::gx::wxwidgets::from_wxString;
using quan::gx::wxwidgets::to_wxString;

/*
   src_zip must exist
   target_dir must exist or be empty string
*/
bool do_unzip( 
      std::string const & src_zip, 
      std::string const & target_dir)
{
   assert(file_exists(src_zip) );

   assert( (target_dir == "") || dir_exists(target_dir) );
  
   wxString wx_src_zip = to_wxString(src_zip);
   wxString wx_target_dir = to_wxString(target_dir);

   wxFileInputStream in(wx_src_zip);
   if (!in.IsOk()) {
      std::cerr << "Input file failed\n";
      return false;
   }
   wxZipInputStream zipin(in);
   if (!zipin.IsOk()) {
      std::cerr << "Create zip input stream failed\n";
      return false;
   }
   int const num_entries = zipin.GetTotalEntries();
   std::cout << "There are " << num_entries << " entries to extract\n";
   int const entries_per_bip = num_entries / 80;
   std::cout << "(each '>' represents another " << entries_per_bip << " entries extracted)\n";
   std::cout << "["<< std::flush;
   int bip_counter =0;
   for ( int i = 0; i < num_entries; ++i){
      
      if ( ++bip_counter >= entries_per_bip){
         bip_counter = 0;
         std::cout << ">" << std::flush;
      }
      // was created on heap
      wxZipEntry* entry = zipin.GetNextEntry();
      assert(entry);
      wxString entry_name = wx_target_dir + entry->GetName();
      
      if ( entry->IsDir()) {
         //std::cout << "extracting dir: " << from_wxString<char>(entry_name) <<'\n';
         if (! wxFileName::Mkdir(entry_name,entry->GetMode(),wxPATH_MKDIR_FULL)){
            delete entry;
            throw std::runtime_error("mkdir failed");
         }
      }else{
         if (zipin.OpenEntry(*entry)){
            if ( zipin.CanRead()){
               wxFileOutputStream file(entry_name);
               if (file.IsOk()){
                //   std::cout << "   extracting file: " << from_wxString<char>(entry_name) <<'\n';
                   zipin.Read(file);
                   delete entry;
               }else{
                  std::cerr << "zip create output file failed\n";
                  delete entry;
                  throw std::runtime_error("zip create output file element failed");
               }
               
            }else{
               std::cerr << "cannot read " << entry->GetName() <<'\n';
               delete entry;
               throw std::runtime_error("read zip entry failed");
            }
            
         }else{
               std::cerr << "cannot open " << entry->GetName() <<'\n';
               delete entry;
               throw std::runtime_error("read zip entry failed");
         }
      }
   }
   std::cout << "]/nzip file extracted successfully\n\n";
   return true;

}