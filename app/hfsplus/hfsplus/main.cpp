#include "emf_volume.h"

#include <iostream>

using namespace std;

int main(int argc, char const* argv[])
{
  /**/
  EMFVolume v;
  if (!v.open("/Users/yielding/Desktop/20120117__IPHONE4_Physical_.nsi00"))
    return 1;
  
  v.decrypt_all_files();
  v.undelete("/Users/yielding/Desktop/deleted");
  
  /**/
  
  /** /
  HFSVolume v;

  if (!v.open("/Users/yielding/code/app/hfsplus/hfsplus/data/HFSPlus.dmg"))
    return false;

  v.list_folder_contents("/");
  
  if (v.read_file("/README.md", "/Users/yielding/tmp"))
    cout << "ok\n";
  / **/
  
  return 0;
}
