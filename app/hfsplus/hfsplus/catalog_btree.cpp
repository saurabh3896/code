#include "catalog_btree.h"
#include "unicode_compare.h"

#include <boost/algorithm/string.hpp>
#include <boost/phoenix/core.hpp>
#include <boost/phoenix/operator.hpp>
#include <iostream>

using namespace std;
using namespace boost::phoenix::arg_names;
////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
namespace {
    const uint8_t METADATA_DIR[] = {
        0, 0, 0, 0, 'H', 'F', 'S', '+', ' ', 
        'P', 'r', 'i', 'v', 'a', 't', 'e', ' ', 'D', 'a', 't', 'a'
    };

    const size_t METADATA_SIZE = sizeof(METADATA_DIR);  // 21
}

////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
CatalogTree::CatalogTree(HFSFile* file)
    : BTree<CatalogTree>(file)
{
}

CatalogTree::~CatalogTree()
{
}

int CatalogTree::compare_keys(HFSPlusCatalogKey const& l, HFSPlusCatalogKey const& r) const
{
    if (l.parentID < r.parentID)
        return -1;

    if (l.parentID > r.parentID)
        return 1;

    return FastUnicodeCompare(l.nodeName.unicode, l.nodeName.length, 
                              r.nodeName.unicode, r.nodeName.length);
}

auto CatalogTree::search_by_cnid(HFSCatalogNodeID cnid) -> CatalogRecord
{
    HFSPlusCatalogKey key;
    key.parentID = cnid;

    auto thread = search(key);
    if (thread.data.recordType == 3 || thread.data.recordType == 4)
    {
        key.parentID = thread.data.thread.parentID;
        key.nodeName = thread.data.thread.nodeName;

        return search(key);
    }

    return CatalogRecord();
}

auto CatalogTree::get_record_from_path(string const& path) -> CatalogRecord
{
    using namespace boost;

    CatalogRecord leaf;
    if (!boost::starts_with(path, "/"))
        return leaf;

    if (path == "/")
        return search_by_cnid(kHFSRootFolderID);

    HFSCatalogNodeID parentID = kHFSRootFolderID;

    // REMAKR: pc[0] is always empty
    vector<string> pc; split(pc, path, is_any_of("/"));

    for (size_t i=1; i<pc.size(); i++)
    {
        if (pc[i].empty()) break;

        // TODO REFACTOR
        HFSPlusCatalogKey key;
        key.parentID = parentID;
        key.nodeName.from_ascii(pc[i]);
        key.keyLength = 6 + 2*pc[i].size();
        leaf = search(key);
        if (leaf.empty())
            return leaf;

        if (leaf.data.recordType != kHFSPlusFolderRecord)
            break;

        parentID = leaf.data.folder.folderID;
    }

    return leaf;
}

auto CatalogTree::get_folder_contents(HFSCatalogNodeID folderID)
    -> CatalogNode
{
    HFSPlusCatalogKey key;
    key.parentID = folderID;

    return search_multiple(key, (arg1 == folderID));
}

auto CatalogTree::metadata_dir_id() -> HFSCatalogNodeID
{
    string name((char const*)METADATA_DIR, METADATA_SIZE);
    HFSPlusCatalogKey key(kHFSRootFolderID, name);

    auto record = search(key);
    return record.empty() 
        ? -1 
        : record.data.folder.folderID;
}

////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
