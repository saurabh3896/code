#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>
#include <exception>
#include <vector>
#include <iostream>

using namespace std;
using boost::property_tree::ptree;

////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
void print_all(ptree& pt, int space)
{
  string tab; 
  for (int i=0; i<space; i++) tab += " ";

  for (auto beg=pt.begin(), end=pt.end(); beg != end; ++beg)
  {
    if (beg->first == "<xmlattr>")
      continue;

    cout << tab << beg->first;

    if (!beg->second.empty())
    {
      print_all(beg->second, space + 2);
    }
    else
    {
      cout << tab << "[" << beg->second.data() << "]" << endl;
    }
  }
}

//
// 1. "array.dict" type
//
void print_selected(ptree& pt, string const& key, vector<int> const& selection)
{
  using namespace boost;

  auto pbeg = pt.get_child(key).begin();
  auto pend = pt.get_child(key).end();

  while (pbeg != pend)
  {
    ptree::iterator beg = pbeg->second.begin();
    ptree::iterator end = pbeg->second.end();
    int index = 0;
    while (beg != end)
    {
      if (find(selection.begin(), selection.end(), index) != selection.end())
      {
        string key   = beg->second.data(); beg++;
        string value = beg->second.data(); beg++;

        cout << "key   : " << key   << endl
          << "value : " << value << endl << endl;
      }
      else
      {
        std::advance(beg, 2);
      }
      index++;
    }
    ++pbeg;
  }
}

int main()
{
  try
  {
    ptree pt;
    // read_xml("History.xml", pt);
    read_xml("History.html", pt);

    vector<int> sel;
    sel.push_back(0);
    sel.push_back(1);
    sel.push_back(2);

    string key = "plist.dict.array";
    // string key = "html.body";
    print_selected(pt, key, sel);

    // print_all(pt, 0);

  }
  catch (std::exception &e)
  {
    std::cout << "error: " << e.what() << "\n";
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
