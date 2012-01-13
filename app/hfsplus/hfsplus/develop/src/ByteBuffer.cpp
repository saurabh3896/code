#include "stdafx.h"

#include "ByteBuffer.h"
#include "EndianSwap.h"

#include <cstdlib>
#include <cstring>
#include <iterator>

#if defined(WIN32) && defined(_DEBUG)
#define new DEBUG_NEW
#endif

using namespace std;

namespace utility { namespace hex {    
////////////////////////////////////////////////////////////////////////////////
//
// This constructor is to allocate initial buffer which is necessary for file 
// read.
//
////////////////////////////////////////////////////////////////////////////////
ByteBuffer::ByteBuffer(size_t size)
    : m_offset(0) 
{ 
    if (size > 0)
        m_buffer.resize(size); 
}

// This constructor is for pre-existing buffer
ByteBuffer::ByteBuffer(uint8_t* buffer, size_t sz)
    : m_offset(0)
{
    m_buffer.reserve(sz);
    m_buffer.assign(buffer, buffer + sz);
}

ByteBuffer::ByteBuffer(ByteBuffer const& rhs)
{
    if (this != &rhs)
    {
        m_offset = rhs.m_offset;
        m_buffer = rhs.m_buffer;
    }
}

ByteBuffer::ByteBuffer(ByteBuffer&& rhs)
{
    m_offset = rhs.m_offset;
    m_buffer.swap(rhs.m_buffer);
}

auto ByteBuffer::operator=(ByteBuffer const& rhs) -> ByteBuffer& 
{
    if (this != &rhs)
    {
        ByteBuffer b(rhs);
        std::swap(m_offset, b.m_offset);
        m_buffer.swap(b.m_buffer);
    }

    return *this;
}

auto ByteBuffer::has_remaining() -> bool 
{
    return m_offset < m_buffer.size();
}

auto ByteBuffer::remaining() -> int64_t
{
    return m_buffer.size() - m_offset;
}

auto ByteBuffer::empty() -> bool
{
    return m_buffer.empty();
}

auto ByteBuffer::flip() -> ByteBuffer&
{
    m_offset = 0;

    return *this;
}

auto ByteBuffer::offset() -> int64_t
{
    return m_offset;
}

auto ByteBuffer::offset(int64_t o) -> ByteBuffer&
{
    m_offset = uint32_t(o);

    return *this;
}

auto ByteBuffer::reset() -> ByteBuffer&
{
    m_offset = 0;
    m_buffer.clear();

    return *this;
}

auto ByteBuffer::skip(uint32_t offset) -> ByteBuffer& 
{
    m_offset += offset;

    return *this;
}

auto ByteBuffer::unget(uint32_t no) -> ByteBuffer&
{
    m_offset -= no;

    return *this;
}

auto ByteBuffer::reserve(size_t sz) -> size_t
{
    m_buffer.reserve(sz);

    return m_buffer.capacity();
}

auto ByteBuffer::append(ByteBuffer& buffer) -> ByteBuffer&
{
    auto& b = buffer.get_buffer();
    m_buffer.insert(m_buffer.end(), b.begin(), b.end());

    return *this;
}

auto ByteBuffer::append(uint8_t* b, size_t sz) -> ByteBuffer&
{
    m_buffer.insert(m_buffer.end(), b, b + sz);

    return *this;
}

auto ByteBuffer::load(buffer_t& buffer) -> void
{
    m_buffer.swap(buffer);
}

auto ByteBuffer::peek1_at(uint32_t offset, int start) -> uint8_t
{
    auto index = m_buffer.size() + 1;

    if (start == cur) index = m_offset + offset;
    if (start == beg) index = offset;
    if (start == end) index = m_offset - offset;

    if (index >= m_buffer.size())
        throw std::runtime_error("byte_byffer::peek1_at()");

    return m_buffer[index];
}

auto ByteBuffer::get_int1() -> int8_t
{ 
    int8_t res = (int8_t)m_buffer[m_offset];
    m_offset++;

    return res;
}

auto ByteBuffer::get_uint1() -> uint8_t
{
    if (m_offset >= m_buffer.size())
      throw std::runtime_error("get_uint1(): index out of range");

    uint8_t res = m_buffer[m_offset];
    m_offset++;

    return res;
}

auto ByteBuffer::get_uint2_net() -> uint16_t 
{
    uint16_t res = *(uint16_t *)(uint8_t *)&m_buffer[m_offset];
    m_offset += 2;

    return endian_swap_bytes<BIG_ENDIAN_ORDER, HOST_ENDIAN_ORDER>(res);
}

auto ByteBuffer::get_int2_net() -> int16_t 
{
    int16_t res = *(int16_t *)(uint8_t *)&m_buffer[m_offset];
    m_offset += 2;

    return endian_swap_bytes<BIG_ENDIAN_ORDER, HOST_ENDIAN_ORDER>(res);
}

auto ByteBuffer::get_int4_net() -> int32_t
{
    int32_t res = *(int32_t*)(uint8_t*)&m_buffer[m_offset];
    m_offset += 4;

    return endian_swap_bytes<BIG_ENDIAN_ORDER, HOST_ENDIAN_ORDER>(res);
}

auto ByteBuffer::get_uint4_net() -> uint32_t 
{
    uint32_t res = *(uint32_t*)(uint8_t*)&m_buffer[m_offset];
    m_offset += 4;

    return endian_swap_bytes<BIG_ENDIAN_ORDER, HOST_ENDIAN_ORDER>(res);
}

int16_t ByteBuffer::get_int2_le()
{
    int16_t res = *(int16_t*)(uint8_t*)&m_buffer[m_offset];
    m_offset += 2;

    return res;
}

uint16_t ByteBuffer::get_uint2_le()
{
    uint16_t res = *(uint16_t*)(uint8_t*)&m_buffer[m_offset];
    m_offset += 2;

    return res;
}

auto ByteBuffer::get_uint2_be() -> uint16_t
{
    return get_int<uint16_t>(2);
}

auto ByteBuffer::get_int2_be() -> int16_t 
{
    return get_int<int16_t>(2);
}

auto ByteBuffer::get_uint3_be() -> uint32_t 
{
    return get_int<uint32_t>(3);
}

auto ByteBuffer::get_int4_be() -> int32_t 
{
    return int32_t(get_int<uint32_t>(4));
}

auto ByteBuffer::get_int4_le() -> int32_t
{
    int32_t res = *(int32_t*)(uint8_t*)&m_buffer[m_offset];
    m_offset += 4;

    return res;
}

auto ByteBuffer::get_uint4_be() -> uint32_t 
{
    return get_int<uint32_t>(4);
}

uint32_t ByteBuffer::get_uint4_le()
{
    uint32_t res = *(uint32_t*)(uint8_t*)&m_buffer[m_offset];
    m_offset += 4;

    return res;
}

auto ByteBuffer::get_uint8_be() -> uint64_t 
{
    return get_int<uint64_t>(8);    
}

auto ByteBuffer::get_int8_be() -> int64_t 
{
    return get_int<int64_t>(8);
}

auto  ByteBuffer::get_uint8_le() -> uint64_t
{
    uint64_t res = *(uint64_t*)(uint8_t*)&m_buffer[m_offset];
    m_offset += 8;

    return res;
}

auto ByteBuffer::get_binary(uint32_t size) -> uint8_t* 
{
    if (m_offset + size > int64_t(m_buffer.size()))
        throw std::runtime_error("byte_byffer::get_binary(size)");

    uint8_t* res = (uint8_t*)&m_buffer[m_offset];
    m_offset += size;

    return res;
}

auto ByteBuffer::get_hex_string(uint32_t size) -> string 
{
    string result;

    for (uint32_t i=0; i<size; i++)
    {
        char b[10] = { 0 };
        sprintf(b, "%02x", m_buffer[m_offset + i]);
        result += string(b);
    }

    m_offset += size;

    return result;
}

auto ByteBuffer::get_string() -> string 
{
    auto offset = m_offset;
    for (; m_buffer[offset] != 0; ++offset);
    auto size = offset - m_offset; 

    string result = get_string(size);
    m_offset += 1; // consume NULL

    return result;
}

auto ByteBuffer::get_string(size_t size) -> string 
{
    string result((char*)&m_buffer[m_offset], size);
    m_offset += size;  

    return result;
}

auto ByteBuffer::c_str() -> char const* 
{
    char const* res = (char const*)&m_buffer[m_offset];
    for (; m_buffer[m_offset] != 0; ++m_offset);
    m_offset++;

    return res;
}

auto ByteBuffer::set_uint1(uint8_t src) -> ByteBuffer& 
{
    m_buffer.push_back(src);
    m_offset++;

    return *this;
}

auto ByteBuffer::set_uint2_net(uint16_t src) -> ByteBuffer& 
{
    src = endian_swap_bytes<HOST_ENDIAN_ORDER, BIG_ENDIAN_ORDER>(src);
    uint8_t no[2] = { 0 };
    memcpy((void*)no, (void*)&src, 2);

    for (size_t i=0; i<2; i++) m_buffer.push_back(no[i]);
    m_offset += 2;

    return *this;
}

auto ByteBuffer::set_uint4_net(uint32_t src) -> ByteBuffer& 
{
    src = endian_swap_bytes<HOST_ENDIAN_ORDER, BIG_ENDIAN_ORDER>(src);
    uint8_t no[4] = { 0 };
    memcpy((void*)no, (void*)&src, 4);

    for (size_t i=0; i<4; i++) m_buffer.push_back(no[i]);
    m_offset += 4;

    return *this;
}

auto ByteBuffer::set_string(char const* src) -> ByteBuffer& 
{
    if (src != NULL)
    {
        size_t len = strlen(src);
        for (size_t i=0; i<len; i++) m_buffer.push_back(src[i]);
        m_offset += len;
    }

    m_buffer.push_back(0);
    m_offset++;

    return *this;
}

auto ByteBuffer::set_binary(uint8_t* src, uint32_t len) -> ByteBuffer& 
{
    for (size_t i=0; i<len; i++) m_buffer.push_back(src[i]);
    m_offset += len;

    return *this;
}

auto ByteBuffer::get_buffer() -> ByteBuffer::buffer_t&   
{ 
    return m_buffer; 
}

auto ByteBuffer::get_buffer(uint32_t size) -> ByteBuffer::buffer_t& 
{
    m_buffer.resize(size);

    return m_buffer;
}

////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
}
} 