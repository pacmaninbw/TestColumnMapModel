#ifndef HDB_COLUMNNAMES_H
#define HDB_COLUMNNAMES_H

#include <map>
#include <string_view>
#include "columnidenum.h"

/*
 * This class is not based on any QOBJECT. There is no need for it to handle
 * signals or slots. It provides a dictionary for all the possible column names
 * that might be used. *
 */

class HDB_ColumnNames
{
public:
    HDB_ColumnNames();
    std::string_view convertToString(ColumnIds id) const noexcept;
    ColumnIds convertToId(std::string_view columnName) const noexcept;

private:
/*
 * The 2 maps create a dictionary of strings to enums
*/
    std::map<ColumnIds, std::string_view> idToName;
    std::map<std::string_view, ColumnIds> nameToID;

    void addToDictionary(ColumnIds id, std::string_view columnName);
};

#endif // HDB_COLUMNNAMES_H
