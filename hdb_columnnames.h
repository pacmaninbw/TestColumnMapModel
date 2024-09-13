#ifndef HDB_COLUMNNAMES_H
#define HDB_COLUMNNAMES_H

#include <map>
#include <string>
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
    std::string convertToString(ColumnIds id) const noexcept;
    ColumnIds convertToId(std::string columnName) const noexcept;

private:
/*
 * The 2 maps create a dictionary of strings to enums
*/
    std::map<ColumnIds, std::string> idToName;
    std::map<std::string, ColumnIds> nameToID;

    void addToDictionary(ColumnIds id, std::string columnName);
};

#endif // HDB_COLUMNNAMES_H
