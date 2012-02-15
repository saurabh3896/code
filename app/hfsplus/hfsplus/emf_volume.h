#ifndef EMF_VOLUME_H_X03CV5QQ
#define EMF_VOLUME_H_X03CV5QQ

#include "hfs_volume.h"
#include "emf.h"

#include <openssl/aes.h>
#include <map>
#include <set>
////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
typedef BTreeRecord<HFSPlusCatalogKey, HFSPlusCatalogData> CatalogRecord;
typedef BTreeNode<CatalogRecord> CatalogNode;

struct HFSKey
{
  HFSKey(uint32_t sz=32): m_size(sz)
  {
    memset(m_fk, 0x00, sz);
  }

  HFSKey(HFSKey const& rhs)
  {
    if (this != &rhs)
    {
      m_size   = rhs.m_size;
      m_aeskey = rhs.m_aeskey;
      memcpy(m_fk, rhs.m_fk, m_size);
    }
  }

  bool operator<(HFSKey const& rhs) const
  {
    return memcmp(m_fk, rhs.m_fk, 32) < 0;
  }

  void set_decrypt(uint8_t* fk_, uint32_t sz=32)
  { 
    m_size = sz;
    memcpy(m_fk, fk_, m_size); 
    AES_set_decrypt_key(m_fk, m_size*8, &m_aeskey);
  }

  void set_encrypt(uint8_t* fk_, uint32_t sz=32)
  { 
    m_size = sz;
    memcpy(m_fk, fk_, m_size); 
    AES_set_encrypt_key(m_fk, m_size*8, &m_aeskey);
  }

  auto size()      -> size_t   { return m_size;   }
  auto as_aeskey() -> AES_KEY& { return m_aeskey; }
  auto as_buffer() -> uint8_t* { return m_fk;     }

private:
  AES_KEY  m_aeskey;
  uint32_t m_size;
  uint8_t  m_fk[32];
};

typedef std::set<HFSKey> HFSKeys;

////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
class EMFVolume: public HFSVolume
{
public:
  EMFVolume();
  virtual ~EMFVolume();

  virtual auto open(std::string const& filename) -> bool;

public:
  void decrypt_all_files();
  bool decrypt_file(CatalogRecord const& r);

  void undelete(std::string const&);
  
  auto iv_for_lba(uint32_t lba, uint32_t* iv, bool add=true) -> void;

  auto get_file_keys_for_cprotect(utility::hex::ByteBuffer& cp) 
    -> std::pair<bool, HFSKey>;
  
  auto unwrap_filekeys_for_class(uint32_t pclass, uint8_t* wrapped_key)
    -> std::pair<bool, HFSKey>;

  virtual auto protection_version() 
    -> int16_t  { return m_protect_version; }
  
  auto emfkey() 
    -> HFSKey&  { return m_emfkey; }
  
private:
  void undelete_based_on_journal(ByteBuffer& jnl
      , std::vector<CatalogRecord>& files
      , std::map<uint32_t, HFSKeys>&
      , std::string const&);

  void undelete_based_on_unused_area(HFSKeys const& keys, std::string const&);

  auto carve_journal(utility::hex::ByteBuffer& journal)
    -> std::pair<std::vector<CatalogRecord>, std::map<uint32_t, HFSKeys>>;

  template <typename RecordT, typename NodeT>
  auto carve_emf_journal(ByteBuffer& journal, uint32_t node_size)
    -> NodeT;

private:
  uint16_t m_protect_version;
  uint64_t m_lba_offset;
  HFSKey   m_emfkey;
  uint32_t m_class_keys_bitset;
  std::vector<HFSKey> m_class_keys;
  HFSCatalogNodeID m_metadata_dir;

private:
  std::vector<std::string> m_not_encrypted;
  std::set<CatalogRecord> m_active_files;
  std::set<HFSKey> m_active_file_keys;
  std::set<uint32_t> m_decrypted_lbas;

  uint32_t m_decrypted_count;
};

////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
#endif
