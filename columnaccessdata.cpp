#include "columnaccessdata.h"
#include "hdb_columnnames.h"
#include "TableNameDictionary.h"
#include <iostream>

ColumnAccessDataMap::ColumnAccessDataMap(ColumnIds id,
        TableIds tabIdIn,
        std::string nameIn,
        std::initializer_list<std::string> (columns)
    )
    : columnId{id},
      tableID{tabIdIn},
      name{nameIn}
{
    for (auto column: columns)
    {
        columnsInTable.push_back(column);
    }
}

void ColumnAccessDataMap::showContents() const noexcept
{
    HDB_ColumnNames dictionary;
    TableNameDictionary tabDictionary;

    std::cout << "\tColumn ID converted to string " <<
        dictionary.convertToString(columnId) << "\n";
    std::cout << "\tColumn Name " << name << "\n";
    std::cout << "\tTable Name: " << tabDictionary.getTableName(tableID) << "\n";

     std::cout << "\tColumns to access in specified table:\n";
     for (auto columnName: columnsInTable)
     {
         std::cout << "\t\t" << columnName << "\n";
     }
     // Provide vertical spacing to make the test output more readable.
     std::cout << "\n\n";
}    

