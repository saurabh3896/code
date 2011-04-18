#include <string>
#include <fstream>
#include <iostream>
#include <boost/regex.hpp>
#include "active_regex.h"

using namespace std;

int main(int argc, char const* argv[])
{
  std::fstream in;
  boost::regex e("\xff\xff");
  in.open("test_1g.bin", ios_base::binary | ios_base::in);
  ActiveRegex regex;
  regex.buffer_size(1024*4);
  if (regex.search(e, in, false))
  {
    matches const& r = regex.result();

    cout <<  r[1].offset;
  }

  return 0;
}