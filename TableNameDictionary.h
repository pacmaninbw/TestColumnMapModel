#ifndef TABLENAMEDICTIONARY_H
#define TABLENAMEDICTIONARY_H

/*
 * Convert Table IDs to Table Names and Table Names to Table IDs
 */
#include "tableidenum.h"
#include <map>
#include <string_view>
#include <utility>

class TableNameDictionary
{
public:
    TableNameDictionary();
    ~TableNameDictionary();
    std::pair<std::string_view, std::string_view> getTableNames(TableIds id);
    std::string_view getTableName(TableIds id);
    std::string_view getShortName(TableIds id);
    TableIds getId(std::string_view tabName) const noexcept;

private:
    void addDefinition(
        TableIds id, std::string_view tableName, std::string_view shortTableName
        );
    std::map<TableIds, std::pair<std::string_view, std::string_view>> idToTabNames;
    std::map<std::string_view, TableIds> TableToId;
    std::map<std::string_view, TableIds> ShortNameToId;
};

#endif