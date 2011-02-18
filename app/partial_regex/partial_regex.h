#ifndef PARTIAL_REGEX_H_BC00NK13
#define PARTIAL_REGEX_H_BC00NK13

#include <stdint.h>
#include <iostream>
#include <vector>
#include <boost/regex.hpp>

////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
struct match_result
{  
  match_result() {}
  match_result(int64_t o, uint32_t l) : offset(o), length(l) {}

  int64_t  offset;
  uint32_t length;
};

typedef std::vector<match_result> matches;

class PartialRegex
{
public:
  PartialRegex();

  bool search(boost::regex const& e, std::istream& is);

  matches
       result() { return m_results; }

  void     buffer_size(uint32_t s) { m_buffer_size = s;    }
  uint32_t buffer_size()           { return m_buffer_size; }


private:
  int64_t  m_offset;
  uint32_t m_buffer_size;

  matches m_results;
};

////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
#endif
