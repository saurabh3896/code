#include "partial_regex.h"
#include <boost/scoped_array.hpp>

using namespace std;
////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
PartialRegex::PartialRegex()
  : m_offset(0)
  , m_buffer_size(4096)
{}

bool PartialRegex::search(boost::regex const& e, std::istream& is)
{
  using namespace boost; 
  scoped_array<char> buf(new char[m_buffer_size]);
  char* const     pbuf = buf.get();
  char const* next_pos = pbuf + m_buffer_size;

  matches results;

  bool ok = true;
  while (ok)
  {
    // 1. compact partial matched string to buffer[0] if available
    //    invariant: left_over + next_pos == (buf + sizeof(buf))
    ptrdiff_t left_over = (pbuf + m_buffer_size) - next_pos;
    memmove(pbuf, next_pos, left_over);

    // 2. read text from the outer stream
    ptrdiff_t to_read = next_pos - pbuf;
    is.read(pbuf + left_over, to_read);

    // 3. update the execution condition
    std::streamsize read = is.gcount();
    ok = (read == to_read);
    m_offset += read;

    // reset next position
    next_pos = pbuf + m_buffer_size;

    // 4. search
    cregex_iterator cur(pbuf, pbuf+read+left_over, e, match_default|match_partial);
    cregex_iterator last;

    // 5. no match.
    //    prepare next position if no match occurred
    if (cur == last)
      continue;

    // 6. match
    //    discriminate partial vs. complete
    while (cur != last)
    {
      int64_t base_offset = m_offset - read - left_over;
      if ((*cur)[0].matched == false)
      {
        next_pos = (*cur)[0].first;
        break;
      }
      else
      {
        uint32_t       offset = (*cur)[0].first  - pbuf;
        uint32_t match_length = (*cur)[0].second - (*cur)[0].first;

        m_results.push_back(match_result(base_offset + offset, match_length));
      }
      ++cur;
    }
  }

  return !m_results.empty();
}

////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
