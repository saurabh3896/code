#include "hfs.h"

#include <fstream>
#include <string>

using namespace utility::hex;
////////////////////////////////////////////////////////////////////////////////
//
// Forward declarations
//
////////////////////////////////////////////////////////////////////////////////
class HFSFile;
class CatalogTree;
class ExtentsTree;
class AttributeTree;

template <typename KeyT, typename ValueT>
struct BTreeRecord;

template <typename HFSRecord>
struct BTreeNode;

typedef BTreeRecord<HFSPlusExtentKey, HFSPlusExtentData> ExtentsRecord;

////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
class HFSVolume
{
public:
  HFSVolume();
  virtual ~HFSVolume();

public:
  virtual auto open(std::string const& filename) -> bool;

  auto read(int64_t offset, size_t sz) -> ByteBuffer;
  auto read(int64_t offset, uint8_t* buffer, size_t sz) -> size_t;
  
  void write(int64_t offset, utility::hex::ByteBuffer& buffer);

  auto list_folder_contents(std::string const& path) -> void;
  auto read_file(std::string const& path, std::string const& mp) -> bool;

  auto get_extents_overflow_for_file(HFSPlusExtentKey const& key)
    -> ExtentsRecord;
  
  auto read_journal() -> ByteBuffer;
  
  auto id() -> int64_t;

  auto block_in_use(uint32_t no) -> bool;

  virtual auto protection_version() -> int16_t
  {
    return 0;
  }
  
public:
  auto block_size() -> uint32_t { return m_block_size; }

protected:
  uint32_t     m_block_size;
  HFSFile*     m_allocation_file;
  ByteBuffer   m_allocatioin_bitmap;
  HFSFile*     m_extents_file;
  HFSFile*     m_catalog_file;
  HFSFile*     m_attribute_file;
  ExtentsTree* m_extents_tree;
  CatalogTree* m_catalog_tree;
  AttributeTree* m_attribute_tree;
  
protected:
  uint64_t m_journal_offset;
  uint64_t m_journal_size;
  uint32_t m_sector_size;

protected:
  HFSPlusVolumeHeader m_header;

private:
  bool         m_opened;
  std::fstream m_stream;
  std::string  m_filename;
};

////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
