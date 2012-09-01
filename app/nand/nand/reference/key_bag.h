#ifndef KEY_BAG_H_2PK4YOQP
#define KEY_BAG_H_2PK4YOQP

#include "ByteBuffer.h"
#include "PtreeParser.h"
#include "hfs_key.h"

#include <string>
#include <map>

////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
using ::utility::hex::ByteBuffer;
using ::utility::parser::PTreeParser;

struct ClassKey
{
    ClassKey() 
    { 
        clear(); 
    }

    bool read_all()
    { 
        return clas >= 0 && !wpky.empty();
    }

    void clear()
    {
        uuid.clear();
        clas = -1;
        wrap =  0;
        ktyp =  0;
        wpky.clear();
    }

    std::string uuid;
    int         clas;        // class number
    int         wrap;        // wrap flags 1: AES encrypted with key835, 2: AES wrapped with passcode key
    int         ktyp;
    std::string wpky;        // wrapped class key
    HFSKey      key;         // decrypted class key
};

class KeyBag
{
public:
    enum { kSystemKeyBag = 0,
           kBackupKeyBag = 1,
           kEscrowKeyBag = 2 };

public: // factory functions
    static KeyBag create_with_plist(PTreeParser const& ptree);

public:
    KeyBag();
    KeyBag(std::string const&, std::string const&);
   ~KeyBag();

public:
    bool init_ok() { return m_init_ok; }

public:
    void device_key(std::string const& dk) { m_device_key = dk; }

    auto unwrap_filekey_for_class(uint32_t pclass, ByteBuffer& wrapped_key)
        -> std::pair<bool, HFSKey>;

    bool unlock_with_passcode_key(std::string const&);

    bool unlock_backup_keybag_with_passcode(std::string const& passcode);
    
    auto get_passcode_key_from_passcode(std::string passcode) 
      -> std::vector<uint8_t>;

    void print_to(std::ostream& os);

private:
    void init_member();
    void set_dkey(std::string const& d);

private:
    auto unwrap_curve25519(uint32_t pclass, ByteBuffer& wrapped_key)
        -> std::pair<bool, HFSKey>;

    bool parse_binary_blob(ByteBuffer& data);

private:
    bool m_init_ok;

private:
    std::map<uint32_t, ClassKey> m_class_keys;

    std::string m_device_key;   // key 835

    uint32_t    m_vers;
    uint32_t    m_type;
    std::string m_uuid;
    std::string m_hmck;
    uint32_t    m_wrap;
    std::string m_salt;
    uint32_t    m_iter;
    ByteBuffer  m_pbky;

    // bool m_unlocked;
};

////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
#endif /* end of include guard: KEY_BAG_H_2PK4YOQP */