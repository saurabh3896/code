#pragma once

#include <vector>
#include <string>
#include <stdint.h>
#include <stdexcept>

namespace utility { namespace hex {
////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
class ByteBuffer
{
public:
    typedef std::vector<uint8_t> buffer_t;
    typedef buffer_t::size_type size_type;

public:
    enum { beg = 0, cur = 1, end = 2 };

public:
    ByteBuffer(size_t size=0);

    // This constructor is for hex conversion
    ByteBuffer(uint8_t* buffer, size_t s);

    ByteBuffer(ByteBuffer const& rhs);
    ByteBuffer(ByteBuffer&& rhs);
    ByteBuffer& operator=(ByteBuffer const& rhs);

    ~ByteBuffer() {}

public:  // operators
    operator uint8_t*()        { return m_buffer.data(); }
    operator char const*()     { return (char const*)m_buffer.data(); }
    operator void const*()     { return (void const*)m_buffer.data(); }

public:  // query
    auto has_remaining() -> bool;
    auto remaining() -> int64_t;
    auto empty() -> bool;
    auto size() -> int64_t     { return int64_t(m_buffer.size());     }
    auto capacity() -> int64_t { return int64_t(m_buffer.capacity()); }
    auto reserve(size_t) -> size_t;

public:
    auto offset() -> int64_t;
    auto offset(int64_t o) -> ByteBuffer&;
    auto reset() -> ByteBuffer&;
    auto flip()  -> ByteBuffer&; 

    auto get_buffer() -> buffer_t&;
    auto get_buffer(uint32_t size) -> buffer_t& ;

    auto skip(uint32_t offset) -> ByteBuffer&;
    auto unget(uint32_t offset) -> ByteBuffer&;
    auto append(ByteBuffer& b) -> ByteBuffer&;
    auto append(uint8_t* b, size_t sz) -> ByteBuffer&;

    auto load(buffer_t& buffer) -> void;
    auto peek1_at(uint32_t offset, int start=cur) -> uint8_t;

    // network I/O interface
    auto get_uint2_net() -> uint16_t;
    auto get_int2_net()  -> int16_t; 
    auto get_int4_net()  -> int32_t;
    auto get_uint4_net() -> uint32_t; 

    auto set_uint2_net(uint16_t) -> ByteBuffer&;
    auto set_uint4_net(uint32_t) -> ByteBuffer&;

    auto set_uint1(uint8_t) -> ByteBuffer&;
    auto set_string(char const* src) -> ByteBuffer&;
    auto set_binary(uint8_t* src, uint32_t size) -> ByteBuffer&;

    auto get_int1()     -> int8_t;
    auto get_uint1()    -> uint8_t;
    auto get_int2_be()  -> int16_t;
    auto get_uint2_be() -> uint16_t;
    auto get_int2_le()  -> int16_t;
    auto get_uint2_le() -> uint16_t;
    auto get_uint3_be() -> uint32_t;
    auto get_int4_be()  -> int32_t;
    auto get_int4_le()  -> int32_t;
    auto get_uint4_be() -> uint32_t;
    auto get_uint4_le() -> uint32_t;
    auto get_int8_be()  -> int64_t;

    auto get_uint8_le() -> uint64_t;
    auto get_uint8_be() -> uint64_t;
    auto get_binary(uint32_t size) -> uint8_t*;

    auto get_hex_string(uint32_t size) -> std::string;
    auto get_string() -> std::string;
    auto get_string(size_t size) -> std::string;
    auto c_str() -> char const*;

public:
    template<typename IntType>
    IntType get_int(size_t byte_count)
    {
        IntType res = 0;
        for (size_t i=0; i<byte_count; ++i)
        {
            uint8_t b = get_uint1();
            res |= IntType(b) << ((byte_count - 1 - i) * 8);
        }
        
        return res;
    }

protected:
    buffer_t m_buffer;
    uint32_t m_offset;
};

////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
}
}